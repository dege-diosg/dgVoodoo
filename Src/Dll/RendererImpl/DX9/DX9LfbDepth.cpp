/*--------------------------------------------------------------------------------- */
/* DX9LfbDepth.cpp - dgVoodoo 3D rendering interface for                            */
/*                        Linear Frame/Depth buffers, DX9 implementation            */
/*                                                                                  */
/* Copyright (C) 2007 Dege                                                          */
/*                                                                                  */
/* This library is free software; you can redistribute it and/or                    */
/* modify it under the terms of the GNU Lesser General Public                       */
/* License as published by the Free Software Foundation; either                     */
/* version 2.1 of the License, or (at your option) any later version.               */
/*                                                                                  */
/* This library is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                */
/* Lesser General Public License for more details.                                  */
/*                                                                                  */
/* You should have received a copy of the GNU Lesser General Public                 */
/* License along with this library; if not, write to the Free Software              */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   */
/*--------------------------------------------------------------------------------- */


extern "C" {

#include	"Debug.h"
#include	"dgVoodooGlide.h"
#include	"Log.h"
#include	"dgVoodooBase.h"
#include	"Dgw.h"
#include	"LfbDepth.h"
#include	"Main.h"

}

//#include	"DX9BaseFunctions.h"
#include	"DX9LfbDepth.hpp"
#include	"DX9Drawing.hpp"

//Gány
#include	"DX9General.hpp"
#include	"DX9Texturing.hpp"


#define		LOCK_AUXDIRTY	0x1			/* Az aux bufferbe elmentettünk egy részt */

/* Képességbitek */
#define	ZBUFF_OK			1
#define WBUFF_OK			2

#define WNEAR				0.01f
#define WFAR				65536.0f


/*------------------------------------------------------------------------------------------*/
/*......................... DX9 implementáció az Lfb kezeléséhez ...........................*/

typedef struct {

	LPDIRECT3DSURFACE9		lpD3DScaleBuffHw9;	/* hardveres méretezopuffer (BUFF_RESIZING) */
	LPDIRECT3DSURFACE9		lpD3DAuxHw9;		/* Hardveres segédpuffer a screenshot label-ek mentéséhez */
	unsigned int			lockFlags;
	unsigned int			auxSaveX;
	unsigned int			auxSaveY;

} LockData;

static	LockData	lockData[3];


DX9LfbDepth::DX9LfbDepth ()
{
	depthCaps = 0;
	depthBufferingMode = None;
	auxBuffWidth = auxBuffHeight = 0;
	lfbUsage = ColorBuffersOnly;
	depthBufferCreated = compareStencilModeUsed = 0;
	realVoodoo = 0;
}


DX9LfbDepth::~DX9LfbDepth ()
{
}
	

int		DX9LfbDepth::InitLfb (LfbUsage lfbUsage, int /*useHwScalingBuffers*/, int realVoodoo)
{
	dx9LfbDepth = this;

	ZeroMemory (&lockData, 3 * sizeof(LockData));
	auxBuffWidth = auxBuffHeight = 0;

	GetLfbFormat (NULL);

	this->realVoodoo = realVoodoo;
	this->lfbUsage = lfbUsage;

	grayScaleRendering = (config.dx9ConfigBits & CFG_DX9GRAYSCALERENDERING) ? 1 : 0;

	LfbAllocOptionalResources ();

	return (LfbAllocInternalBuffers (lfbUsage, realVoodoo));
}


void	DX9LfbDepth::DeInitLfb ()
{
	LfbReleaseInternalBuffers ();

	LfbReleaseOptionalResources ();
}


int		DX9LfbDepth::LfbAllocInternalBuffers (LfbUsage lfbUsage, int /*realVoodoo*/)
{
	DEBUG_BEGINSCOPE("DX9LfbDepth::LfbAllocInternalBuffers", DebugRenderThreadId);
	
	if (lfbUsage == ColorBuffersOnly)
		return (1);

	// Segédpufferek mutatóinak nullázása: nem biztos, hogy szükség lesz rájuk.
	unsigned int i;
	for(i = 0; i < 3; i++) lockData[i].lpD3DScaleBuffHw9 /* = lockData[i].lpDDScaleBuffSw*/ = NULL;
	
	// Ha az LFB-elérés tiltva, akkor készen vagyunk.
	if ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) == (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) )
		return(1);

	magStretchFilter = (d3dCaps9.StretchRectFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR) ? D3DTEXF_LINEAR : D3DTEXF_POINT;
	minStretchFilter = (d3dCaps9.StretchRectFilterCaps & D3DPTFILTERCAPS_MINFLINEAR) ? D3DTEXF_LINEAR : D3DTEXF_POINT;

	for (i = 0; i < movie.cmiBuffNum; i++) {
		if (lfbUsage != OneScalingBuffer || (i == 1)) {
			HRESULT hr = lpD3Ddevice9->CreateRenderTarget (LfbGetConvBuffXRes (), appYRes, renderTargetFormat,
														   D3DMULTISAMPLE_NONE, 0, TRUE, &lockData[i].lpD3DScaleBuffHw9, NULL);
			if (FAILED (hr)) {
				for (i--; i > 0; i--)
					lockData[i].lpD3DScaleBuffHw9->Release ();
				return (0);
			}
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (1);
}


void	DX9LfbDepth::LfbSetupDefaultRenderStates ()
{
	D3DMATRIX	projectionMatrix;
	ZeroMemory(&projectionMatrix, sizeof(D3DMATRIX));
	projectionMatrix._11 = projectionMatrix._22 = projectionMatrix._33 = projectionMatrix._44 = 1.0f;
	projectionMatrix._33 = (WNEAR / (WFAR - WNEAR)) + 1.0f;
	projectionMatrix._44 = WNEAR;
	projectionMatrix._43 = 0.0f;
	projectionMatrix._34 = 1.0f;
	lpD3Ddevice9->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	
	lpD3Ddevice9->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	lpD3Ddevice9->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	lpD3Ddevice9->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	lpD3Ddevice9->SetRenderState(D3DRS_STENCILREF, 0);
	lpD3Ddevice9->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
}


void	DX9LfbDepth::LfbReleaseInternalBuffers ()
{
	LockData* lockDataPtr = lockData;
	
	for(int i = 0; i < 3; i++)	{
		if (lockDataPtr->lpD3DScaleBuffHw9 != NULL) {
			lockDataPtr->lpD3DScaleBuffHw9->Release ();
			lockDataPtr->lpD3DScaleBuffHw9 = NULL;
		}
		lockDataPtr->lockFlags = 0;
		lockDataPtr++;
	}
}


void	DX9LfbDepth::LfbAllocOptionalResources ()
{
	if (grayScaleRendering && ((d3dCaps9.TextureCaps & (D3DPTEXTURECAPS_POW2 | D3DPTEXTURECAPS_NONPOW2CONDITIONAL)) != D3DPTEXTURECAPS_POW2)) {
		
		HRESULT hr = lpD3Ddevice9->CreateTexture (movie.cmiXres, movie.cmiYres, 1, D3DUSAGE_RENDERTARGET, renderTargetFormat, D3DPOOL_DEFAULT,
												  &lpD3DBackBufferTexture, NULL);

		if (!FAILED (hr)) {
			hr = lpD3DBackBufferTexture->GetSurfaceLevel (0, &lpD3DBackBufferTextureSurface);
			if (!FAILED (hr))
				return;

			lpD3DBackBufferTexture->Release ();
		}
	}

	lpD3DBackBufferTexture = NULL;
	lpD3DBackBufferTextureSurface = NULL;
}


void	DX9LfbDepth::LfbReleaseOptionalResources ()
{
	if (lpD3DBackBufferTexture != NULL) {
		lpD3DBackBufferTextureSurface->Release ();
		lpD3DBackBufferTexture->Release ();
		lpD3DBackBufferTexture = NULL;
	}
}


int		DX9LfbDepth::CreateDepthBuffer (unsigned int width, unsigned int height, unsigned int bitDepth,
									    int /*createWithStencil*/, unsigned int* zBitDepth, unsigned int* stencilBitDepth)
{
	if (bitDepth == 0)
		bitDepth = 32;

	D3DFORMAT depthFormats16[] = {D3DFMT_D16, D3DFMT_D16_LOCKABLE, D3DFMT_D15S1};
	D3DFORMAT depthFormats32[] = {D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D32, D3DFMT_D24X8, D3DFMT_D24FS8, D3DFMT_D32F_LOCKABLE};
	unsigned int stencilBitDepth16[] = {0, 0, 1};
	unsigned int stencilBitDepth32[] = {8, 4, 0, 0, 8, 0};

	int numOfDepthFormats = (bitDepth == 32) ? 6 : 3;
	D3DFORMAT* possibleFormat = (bitDepth == 32) ? depthFormats32 : depthFormats16;
	unsigned int* possibleStencilBd = (bitDepth == 32) ? stencilBitDepth32 : stencilBitDepth16;

	D3DFORMAT format;
	unsigned int stencilBd;

	HRESULT hr;

	int i;
	for (i = 0; i < numOfDepthFormats; i++) {
		hr = lpD3D9->CheckDeviceFormat (this->adapter, this->deviceType, this->deviceDisplayFormat,
										D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, possibleFormat[i]);

		if (hr == D3D_OK) {
			hr = lpD3D9->CheckDepthStencilMatch (this->adapter, this->deviceType, this->deviceDisplayFormat,
												 this->renderTargetFormat, possibleFormat[i]);
			if (!FAILED (hr)) {
				format = possibleFormat[i];
				stencilBd = possibleStencilBd[i];
				break;
			}
		}
	}

	if (i != numOfDepthFormats) {
		HRESULT hr = lpD3Ddevice9->CreateDepthStencilSurface (width, height, format, D3DMULTISAMPLE_NONE, 0, FALSE,
															  &lpD3Ddepth9, NULL);
		if (!FAILED (hr))
			lpD3Ddevice9->SetDepthStencilSurface (lpD3Ddepth9);
	}

	*zBitDepth = bitDepth;
	*stencilBitDepth = stencilBd;
	return (!FAILED (hr));
}


void	DX9LfbDepth::DestroyDepthBuffer ()
{
	lpD3Ddevice9->SetDepthStencilSurface (NULL);
	lpD3Ddepth9->Release ();
	lpD3Ddepth9 = NULL;
	depthBufferCreated = 0;
}


int		DX9LfbDepth::GetLfbFormat (_pixfmt* format)
{
	ConvertToPixFmt (renderTargetFormat, &backBufferFormat);
	if (format != NULL)
		*format = backBufferFormat;

	return (1);
}


void	DX9LfbDepth::ClearBuffers ()
{
	for (int i = 0; i < 3; i++) {
		if (renderBuffers[i] != NULL) {
			lpD3Ddevice9->ColorFill (renderBuffers[i], NULL, 0);
		}
		if (lockData[i].lpD3DScaleBuffHw9 != NULL) {
			lpD3Ddevice9->ColorFill (lockData[i].lpD3DScaleBuffHw9, NULL, 0);
		}
		if (lockData[i].lpD3DAuxHw9 != NULL) {
			lpD3Ddevice9->ColorFill (lockData[i].lpD3DAuxHw9, NULL, 0);
		}
	}
}


void	DX9LfbDepth::FillScalingBuffer (int /*buffer*/, RECT* /*rect*/, unsigned int /*colorKey*/, int /*mirrored*/, int /*pointerIsInUse*/)
{
}


void	DX9LfbDepth::BlitToScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, int mirrored, int /*scaled*/, int /*pointerIsInUse*/)
{

	DEBUGCHECK (0, mirrored, "\n   Error: DX9LfbDepth::BlitToScalingBuffer: Mirrored blitting is unsupported by D3D9!");
	DEBUGCHECK (1, mirrored, "\n   Hiba: DX9LfbDepth::BlitToScalingBuffer: A tükrözéses blittelést a D3D9 nem támogatja!");

	D3DTEXTUREFILTERTYPE filterType = ((movie.cmiXres > appXRes && movie.cmiYres > appYRes) ? minStretchFilter : magStretchFilter);

	lpD3Ddevice9->StretchRect (renderBuffers[buffer], srcRect, lockData[buffer].lpD3DScaleBuffHw9, dstRect, filterType);
}


void	DX9LfbDepth::BlitFromScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, BlitType blitType,
											 int mirrored, int /*scaled*/)
{
	DEBUGCHECK (0, mirrored, "\n   Error: DX9LfbDepth::BlitToScalingBuffer: Mirrored blitting is unsupported by D3D9!");
	DEBUGCHECK (1, mirrored, "\n   Hiba: DX9LfbDepth::BlitToScalingBuffer: A tükrözéses blittelést a D3D9 nem támogatja!");

	DEBUGCHECK (0, blitType == CopyWithColorKey, "\n   Error: DX9LfbDepth::BlitToScalingBuffer: Colorkeyed blitting is unsupported by D3D9!");
	DEBUGCHECK (1, blitType == CopyWithColorKey, "\n   Hiba: DX9LfbDepth::BlitToScalingBuffer: A colorkey-es blittelést a D3D9 nem támogatja!");

	D3DTEXTUREFILTERTYPE filterType = ((movie.cmiXres < appXRes && movie.cmiYres < appYRes) ? minStretchFilter : magStretchFilter);

	lpD3Ddevice9->StretchRect (lockData[buffer].lpD3DScaleBuffHw9, srcRect, renderBuffers[buffer], dstRect, filterType);
}


int		DX9LfbDepth::IsBlitTypeSupported (BlitType blitType)
{
	return (blitType == Copy);
}


int		DX9LfbDepth::IsMirroringSupported ()
{
	return (0);
}


int		DX9LfbDepth::CopyBetweenScalingBuffers (int /*srcBuffer*/, int /*dstBuffer*/, RECT* /*rect*/)
{
	return (1);
}


int		DX9LfbDepth::AreHwScalingBuffersUsed ()
{
	return (1);
}


int		DX9LfbDepth::CreateAuxBuffers (unsigned int width, unsigned int height)
{	
	int i;
	for(i = 0; i < (int) movie.cmiBuffNum; i++)	{
		HRESULT hr = lpD3Ddevice9->CreateRenderTarget (width, height, renderTargetFormat,
													   D3DMULTISAMPLE_NONE, 0, TRUE, &lockData[i].lpD3DAuxHw9, NULL);
		if (FAILED (hr))
			break;

		lockData[i].lockFlags &= ~LOCK_AUXDIRTY;
	}
	if (i != (int) movie.cmiBuffNum) {
		for (; i >= 0; i--) {
			lockData[i].lpD3DAuxHw9->Release ();
			lockData[i].lpD3DAuxHw9 = NULL;
		}
		return (0);
	}
	auxBuffWidth = width;
	auxBuffHeight = height;
	return (1);
}


void	DX9LfbDepth::DestroyAuxBuffers ()
{
	for(unsigned int i = 0; i < movie.cmiBuffNum; i++)	{
		if (lockData[i].lpD3DAuxHw9 != NULL) {
			lockData[i].lpD3DAuxHw9->Release ();
			lockData[i].lpD3DAuxHw9 = NULL;
		}
	}
}


void	DX9LfbDepth::BlitToAuxBuffer (int buffer, RECT* srcRect)
{
	/*if (lockData[buffer].lpDDAuxHw->IsLost() != DD_OK)
		lockData[buffer].lpDDAuxHw->Restore();*/
	

	lpD3Ddevice9->StretchRect (renderBuffers[buffer], srcRect, lockData[buffer].lpD3DAuxHw9, NULL, D3DTEXF_POINT);
	lockData[buffer].lockFlags |= LOCK_AUXDIRTY;
}


void	DX9LfbDepth::BlitFromAuxBuffer (int buffer, unsigned int x, unsigned y)
{
	if (lockData[buffer].lockFlags & LOCK_AUXDIRTY) {
		RECT rtRect = {x, y, x + auxBuffWidth, y + auxBuffHeight};
		lpD3Ddevice9->StretchRect (lockData[buffer].lpD3DAuxHw9, NULL, renderBuffers[buffer], &rtRect, D3DTEXF_POINT);
		lockData[buffer].lockFlags &= ~LOCK_AUXDIRTY;
	}
}


void	DX9LfbDepth::RotateBufferProperties (int numOfBuffers, int rotateBuffers)
{
	LPDIRECT3DSURFACE9 lpD3DScaleBuffHw9 = NULL;
	
	if (rotateBuffers) {
		lpD3DScaleBuffHw9 = lockData[0].lpD3DScaleBuffHw9;
	}
		
	LPDIRECT3DSURFACE9 lpD3DAuxHw9 = lockData[0].lpD3DAuxHw9;
	unsigned int lockFlags = lockData[0].lockFlags;
	for(int i = 1; i < numOfBuffers; i++)	{
		if (rotateBuffers) {
			lockData[i-1].lpD3DScaleBuffHw9 = lockData[i].lpD3DScaleBuffHw9;
		}
		lockData[i-1].lpD3DAuxHw9 = lockData[i].lpD3DAuxHw9;
		lockData[i-1].lockFlags = lockData[i].lockFlags;
	}
	if (rotateBuffers) {
		lockData[numOfBuffers-1].lpD3DScaleBuffHw9 = lpD3DScaleBuffHw9;
	}
	lockData[numOfBuffers-1].lpD3DAuxHw9 = lpD3DAuxHw9;
	lockData[numOfBuffers-1].lockFlags = lockFlags;
}


int		DX9LfbDepth::LockBuffer (int buffer, BufferType bufferType, void** buffPtr, unsigned int* pitch)
{
	LPDIRECT3DSURFACE9		lpD3D9Buff = NULL;
	LockData*				lockDataPtr = lockData + buffer;
	
	switch (bufferType) {
		case ColorBuffer:
			lpD3D9Buff = renderBuffers[buffer];
			break;

		case ScalingBuffer:
			lpD3D9Buff = lockDataPtr->lpD3DScaleBuffHw9;
			break;
	}

	if (lpD3D9Buff == NULL)
		return (0);

	D3DLOCKED_RECT d3dLockedRect;

	if (FAILED (lpD3D9Buff->LockRect(&d3dLockedRect, NULL, D3DLOCK_NOSYSLOCK))) 
		return(0);

	*buffPtr = d3dLockedRect.pBits;
	*pitch = d3dLockedRect.Pitch;
	return (1);
}


int		DX9LfbDepth::UnlockBuffer (int buffer, BufferType bufferType)
{
	LPDIRECT3DSURFACE9		lpD3D9Buff;
	LockData*				lockDataPtr = lockData + buffer;
	
	switch (bufferType) {
		case ColorBuffer:
			lpD3D9Buff = renderBuffers[buffer];
			break;

		case ScalingBuffer:
			lpD3D9Buff = lockDataPtr->lpD3DScaleBuffHw9;
			break;
	}

	if (lpD3D9Buff == NULL)
		return (0);

	return (!FAILED (lpD3D9Buff->UnlockRect ()));
}


int		DX9LfbDepth::CreatePrimaryPalette ()
{
	return (0);
}


void	DX9LfbDepth::DestroyPrimaryPalette ()
{
}


void	DX9LfbDepth::SetPrimaryPaletteEntries (PALETTEENTRY* /*entries*/)
{
}

/*------------------------------------------------------------------------------------------*/
/*..................... DX9 implementáció a mélységi puffereléshez .........................*/

int		DX9LfbDepth::InitDepthBuffering (enum DepthBufferingDetectMethod detectMethod)
{
	lpD3Ddepth9 = NULL;

	depthCaps = 0;
	depthBufferingMode = None;
	depthBufferCreated = compareStencilModeUsed = 0;
	
	depthCaps |= ZBUFF_OK;
	
	LfbSetupDefaultRenderStates ();

	LfbDetermineWBufferingMethod (detectMethod);

	return (1);
}


void	DX9LfbDepth::DeInitDepthBuffering ()
{
	if (depthBufferCreated)
		DestroyDepthBuffer ();
	
	depthBufferingMode = None;
}


void	DX9LfbDepth::LfbDetermineWBufferingMethod(enum DepthBufferingDetectMethod detectMethod)	{

	if (d3dCaps9.MaxVertexW < 65528.0f)	{
		DEBUGLOG(0,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Maximum W-based depth value that device supports is %f, true W-buffering CANNOT be used!", d3dCaps9.MaxVertexW);
		DEBUGLOG(1,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Az eszköz által támogatott maximális W-alapú mélységi érték %f, a valódi W-pufferelés NEM használható!", d3dCaps9.MaxVertexW);
		return;
	}

	switch (detectMethod) {
		case AlwaysUseW:
			depthCaps |= WBUFF_OK;
			DEBUGLOG(0,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Being forced to use true W-buffering!");
			DEBUGLOG(1,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: A wrapper valódi W-pufferelés használatára van kényszerítve!");
			break;

		case AlwaysUseZ:
			DEBUGLOG(0,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Being forced to use emulated W-buffering!");
			DEBUGLOG(1,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: A wrapper emulált W-pufferelés használatára van kényszerítve!");
			break;

		case DetectViaDriver:
			if (d3dCaps9.RasterCaps & D3DPRASTERCAPS_WBUFFER)	{
				depthCaps |= WBUFF_OK;
				DEBUGLOG(0,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering can be used!");
				DEBUGLOG(1,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés használható!");
			} else {
				DEBUGLOG(0,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering CANNOT be used, using emulated W-buffering instead!");
				DEBUGLOG(1,"\nDX9LfbDepth::LfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés NEM használható, helyette emuláció!");
			}
			break;
		
		default:
			break;

	}
}


void		DX9LfbDepth::SetDepthBufferingMode (GrDepthBufferMode_t mode)
{
	unsigned int stencilBitDepth = 0;

	if (mode != GR_DEPTHBUFFER_DISABLE)	{
		if (!depthBufferCreated)	{

			unsigned int zBitDepth = (config.Flags2 & CFG_DEPTHEQUALTOCOLOR) ? movie.cmiBitPP : 0;
			
			if (!CreateDepthBuffer (movie.cmiXres, movie.cmiYres, zBitDepth, 1, &zBitDepth, &stencilBitDepth)) {
				DEBUGLOG(0,"\ngrDepthBufferMode: Used depth buffer bit depth: %d, stencil bits: %d", zBitDepth, stencilBitDepth);
				DEBUGLOG(0,"\n   Error: grDepthBufferMode: Creating depth buffer failed");
				DEBUGLOG(1,"\ngrDepthBufferMode: A használt bitmélység: %d, stencil bitek: %d", zBitDepth, stencilBitDepth);
				DEBUGLOG(1,"\n   Hiba: grDepthBufferMode: A mélységi puffer létrehozása nem sikerült");
				return;
			}

			if (stencilBitDepth == 0)	{
				DEBUGLOG(0,"\n   Warning: DX9LfbDepth::SetDepthBufferingMode: Couldn't create a depth buffer with stencil component. 'Compare to bias' depth buffering modes won't work.\
							\n												  Tip: your videocard either does not support stencil buffers (old cards) or only does it at higher bit depths.\
							\n												  If you are using a 16 bit mode then try to use the wrapper in 32 bit mode!");

				DEBUGLOG(1,"\n   Figyelmeztetés: DX9LfbDepth::SetDepthBufferingMode: Nem sikerült olyan mélységi puffert létrehozni, amely rendelkezik stencil-összetevõvel.\
							\n														 A 'Compare to bias'-módok nem fognak mûködni a mélységi puffer használatakor.\
							\n														 Tipp: a videókártyád vagy egyáltalán nem támogatja a stencil-puffereket (régi kártyák) vagy csak nagyobb\
							\n														 bitmélység mellett. Ha egy 16 bites módot használsz, próbálj ki egy 32 bites módot!");
			}
			
			depthBufferCreated = 1;
			//lpD3Ddevice->SetRenderTarget (renderBuffers[renderbuffer], 0);
		}
	}

	char	stencilModeUsed = 0;
	DWORD	d3dMode;
	switch(mode)	{

		case GR_DEPTHBUFFER_ZBUFFER_COMPARE_TO_BIAS:
			stencilModeUsed = 1;					//no break;
		
		case GR_DEPTHBUFFER_ZBUFFER:
			d3dMode = D3DZB_TRUE;
			depthBufferingMode = ZBuffering;
			break;
		
		case GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS:
			stencilModeUsed = 1;					//no break;
		
		case GR_DEPTHBUFFER_WBUFFER:			
			if (!(depthCaps & WBUFF_OK)) {
				d3dMode = D3DZB_TRUE;
				depthBufferingMode = WBufferingEmu;
			} else {
				d3dMode = D3DZB_USEW;
				depthBufferingMode = WBuffering;

				//lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, astate.perspective = TRUE);
			}
			break;
		
		default:
			d3dMode = D3DZB_FALSE;
			depthBufferingMode = None;
			break;
	}

	HRESULT hr = lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, d3dMode);
	DEBUGCHECK(0, FAILED (hr), "\n   Error: grDepthBufferMode: Return of setting depth buffer mode: %0x, error",hr);
	DEBUGCHECK(1, FAILED (hr), "\n   Hiba: grDepthBufferMode: A mélységi puffer módjának beállításának visszatérési értéke: %0x, hiba",hr);

	if (stencilModeUsed != compareStencilModeUsed)	{
		if (stencilBitDepth)
			lpD3Ddevice9->SetRenderState(D3DRS_STENCILENABLE, stencilModeUsed ? TRUE : FALSE);

		//dx7Drawing->SetDepthBias (astate.depthbiaslevel, compareStencilModeUsed = stencilModeUsed);
	}
}


void		DX9LfbDepth::SetDepthBufferFunction (GrCmpFnc_t function)
{
	lpD3Ddevice9->SetRenderState (D3DRS_ZFUNC, cmpFuncLookUp[function]);
}


void		DX9LfbDepth::EnableDepthMask (int enable)
{
	lpD3Ddevice9->SetRenderState (D3DRS_ZWRITEENABLE, enable ? TRUE : FALSE);
}


void		DX9LfbDepth::SetDepthBias (FxI16 level)
{
	dx9Drawing->SetDepthBias (level, depthBufferingMode == WBufferingEmu, compareStencilModeUsed);
}


enum DX9LfbDepth::DepthBufferingMode	DX9LfbDepth::GetTrueDepthBufferingMode ()
{
	return depthBufferingMode;
}


void		DX9LfbDepth::ReleaseDefaultPoolResources ()
{
	LfbReleaseOptionalResources ();

	if (depthBufferCreated)
		DestroyDepthBuffer ();

	LfbReleaseInternalBuffers ();
	DestroyAuxBuffers ();
}


int			DX9LfbDepth::ReallocDefaultPoolResources ()
{
	LfbAllocOptionalResources ();

	if (!LfbAllocInternalBuffers (lfbUsage, realVoodoo))
		return (0);

	if ((auxBuffWidth != 0) && (auxBuffHeight != 0)) {
		if (!CreateAuxBuffers (auxBuffWidth, auxBuffHeight))
			return (0);
	}

	LfbSetupDefaultRenderStates ();

	return (1);
}


int			DX9LfbDepth::Is3DNeeded ()
{
	return grayScaleRendering;
}


void		DX9LfbDepth::ConvertToGrayScale (LPDIRECT3DSURFACE9 buffer)
{
	if (lpD3DBackBufferTextureSurface != NULL) {
		RECT rect = {0, 0, movie.cmiXres, movie.cmiYres};

		lpD3Ddevice9->StretchRect (buffer, &rect, lpD3DBackBufferTextureSurface, &rect, D3DTEXF_NONE);

		dx9General->SaveRenderState (RendererGeneral::GrayScaleRendering);
		dx9General->SetRenderState (RendererGeneral::GrayScaleRendering, 0, 0, 0);
		

		dx9Texturing->SetTextureAtStage (0, lpD3DBackBufferTexture);
		lpD3Ddevice9->BeginScene ();
		dx9Drawing->DrawTile (0.0f, 0.0f, (float) movie.cmiXres, (float) movie.cmiYres,
							  0.5f / ((float) movie.cmiXres), 0.5f / ((float) movie.cmiYres),
							  (((float) movie.cmiXres) + 0.5f) / ((float) movie.cmiXres),
							  (((float) movie.cmiYres) + 0.5f) / ((float) movie.cmiYres),
							  1.0f, 0.5f, 0);

		lpD3Ddevice9->EndScene ();

		dx9General->RestoreRenderState (RendererGeneral::GrayScaleRendering);
	}
}
