/*--------------------------------------------------------------------------------- */
/* DX7LfbDepth.cpp - dgVoodoo 3D rendering interface for                            */
/*                        Linear Frame/Depth buffers, DX7 implementation            */
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

#include	"DX7BaseFunctions.h"
#include	"DX7LfbDepth.hpp"
#include	"DX7Drawing.hpp"


#define		LOCK_RSOFTWARE	0x1			/* Tartalom a szoftveres pufferben */
#define		LOCK_AUXDIRTY	0x2			/* Az aux bufferbe elmentettünk egy részt */

/* Hardver blittelési képeségei */
#define BLT_FASTBLT			1			/* A segédpuffereket a videómemóriában hoztuk létre */
#define BLT_CANMIRROR		2			/* A hardver képes az y-tükrözésre blittelés közben */
#define BLT_CANSRCCOLORKEY	4			/* A hardver támogatja a source colorkeying-es blittelést */
#define BLT_CANBLTSYSMEM	8			/* A system memóriába is képes hardveresen blittelni - nem használt */


/* Képességbitek */
#define	ZBUFF_OK			1
#define WBUFF_OK			2

#define WNEAR				0.01f
#define WFAR				65536.0f


/*------------------------------------------------------------------------------------------*/
/*......................... DX7 implementáció az Lfb kezeléséhez ...........................*/

typedef struct {

	LPDIRECTDRAWSURFACE7	lpDDScaleBuffHw;	/* hardveres méretezopuffer (BUFF_RESIZING) */
	LPDIRECTDRAWSURFACE7	lpDDScaleBuffSw;	/* szoftveres méretezopuffer (BUFF_RESIZING) */
	LPDIRECTDRAWSURFACE7	lpDDAuxHw;			/* Hardveres segédpuffer a screenshot label-ek mentéséhez */
	unsigned int			lockFlags;
	unsigned int			auxSaveX;
	unsigned int			auxSaveY;

} LockData;

static	LockData	lockData[3];


typedef struct {

	unsigned int		dontUseStencil;
	unsigned int		maxZBitDepth;
	unsigned int		detectedZBitDepth;
	unsigned int		detectedStencilBitDepth;
	DDPIXELFORMAT		selectedZBufferFormat;

} ZBufferFormatEnumeration;
	

int		DX7LfbDepth::InitLfb (LfbUsage lfbUsage, int useHwScalingBuffers, int realVoodoo)
{
	dx7LfbDepth = this;

	ZeroMemory (&lfbDesc, sizeof(DDSURFACEDESC2));
	lfbDesc.dwSize = sizeof(DDSURFACEDESC2);

	ZeroMemory (&lockData, 3 * sizeof(LockData));
	lpDDTempScaleBuffSw = NULL;

	this->lfbUsage = lfbUsage;

	GetLfbFormat (NULL);

	return (LfbAllocInternalBuffers (lfbUsage, useHwScalingBuffers, realVoodoo));
}


void	DX7LfbDepth::DeInitLfb ()
{
int i;

	LockData* lockDataPtr = lockData;
	
	for(i = 0; i < 3; i++)	{
		if (lockDataPtr->lpDDScaleBuffHw != NULL)
			lockDataPtr->lpDDScaleBuffHw->Release ();
		if (lockDataPtr->lpDDScaleBuffSw != NULL)
			lockDataPtr->lpDDScaleBuffSw->Release ();
		lockDataPtr++;
	}

	if (lpDDTempScaleBuffSw != NULL)
		lpDDTempScaleBuffSw->Release ();
	
	lpDDTempScaleBuffSw = NULL;
}


int		DX7LfbDepth::LfbAllocInternalBuffers (LfbUsage lfbUsage, int useHwScalingBuffers, int realVoodoo)
{
	unsigned int	i, hwbuff;
	DDCAPS			caps;

	DEBUG_BEGINSCOPE("DX7LfbDepth::LfbAllocInternalBuffers", DebugRenderThreadId);

	if (lfbUsage == ColorBuffersOnly)
		return (1);
	
	// Segédpufferek mutatóinak nullázása: nem biztos, hogy szükség lesz rájuk.
	for(i = 0; i < 3; i++) lockData[i].lpDDScaleBuffHw = lockData[i].lpDDScaleBuffSw = NULL;
	
	// Ha az LFB-elérés tiltva, akkor készen vagyunk.
	if ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) == (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) ) return(1);

	ZeroMemory (&caps, sizeof(DDCAPS));
	caps.dwSize = sizeof(DDCAPS);
	lpDD->GetCaps (&caps, NULL);
	bltCaps = 0;
	
	// Feltételezzük, hogy ha a kártya képes az egyes blittelési módokra, akkor azok kombinációira is képes!
	// (ezt sajnos a flagekbõl nem lehet kideríteni)
	if (hwbuff = ((caps.dwCaps & DDCAPS_BLT) && useHwScalingBuffers))	{
		if ( (caps.dwCaps & DDCAPS_COLORKEY) && (caps.dwCKeyCaps & DDCKEYCAPS_SRCBLT) ) bltCaps |= BLT_CANSRCCOLORKEY;
		if ( (caps.dwFXCaps & DDFXCAPS_BLTMIRRORUPDOWN) ) bltCaps |= BLT_CANMIRROR;
	}
	if (drawresample)	{
		if (!(caps.dwCaps & DDCAPS_BLTSTRETCH)) hwbuff = 0;
	} else {
		if ( (!(bltCaps & BLT_CANMIRROR)) && (!(bltCaps & BLT_CANSRCCOLORKEY)) && (!realVoodoo) ) hwbuff = 0;
	}
	if (caps.dwCaps & DDCAPS_CANBLTSYSMEM )	{
		bltCaps |= BLT_CANBLTSYSMEM;
	}
	
	// Globális tulajdonságok beállitása
	lfbDesc.dwHeight = appYRes;
	lfbDesc.dwWidth = LfbGetConvBuffXRes ();//convbuffxres;
	lfbDesc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	lfbDesc.dwFlags |= DDSD_CKSRCBLT;
   
	if (hwbuff)	{
		lfbDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		for(i = 0; i < movie.cmiBuffNum; i++)	{
			if (lfbUsage != OneScalingBuffer || (i == 1)) {
				if (lpDD->CreateSurface(&lfbDesc, &lockData[i].lpDDScaleBuffHw, NULL) != DD_OK) break;
			}
		}
		// Ha nem tudtuk az összeset, akkor majd a sysmembõl
		if (i != movie.cmiBuffNum)	{
			for(i = 0; i < movie.cmiBuffNum; i++)
				if (lockData[i].lpDDScaleBuffHw) lockData[i].lpDDScaleBuffHw->Release();
			bltCaps &= ~(BLT_CANMIRROR | BLT_CANSRCCOLORKEY);
			hwbuff = 0;
		} else bltCaps |= BLT_FASTBLT;
	}
	
	lfbDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	
	// Egy átmeneti szoftveres puffer a REALVOODOO opcióhoz (pufferek rotálásához)
	if (lfbUsage != OneScalingBuffer) {
		if (lpDD->CreateSurface(&lfbDesc, &lpDDTempScaleBuffSw, NULL) != DD_OK)
			return(0);
	}
	
	if ((!hwbuff) || ( (!(bltCaps & BLT_CANMIRROR)) || (!(bltCaps & BLT_CANSRCCOLORKEY)) ) )	{
		for(i = 0; i < movie.cmiBuffNum; i++)	{
			if (lfbUsage != OneScalingBuffer || (i == 1)) {
				if (lpDD->CreateSurface(&lfbDesc, &lockData[i].lpDDScaleBuffSw, NULL) != DD_OK) break;
			}
		}
		// Ha nem megy, akkor hiba
		if (i != movie.cmiBuffNum)	{
			for(i = 0; i < movie.cmiBuffNum; i++)
				if (lockData[i].lpDDScaleBuffSw) lockData[i].lpDDScaleBuffSw->Release();
			return(0);
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (1);
}


HRESULT	CALLBACK	DX7LfbDepth::CreateDepthBufferCallback (LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext)
{
ZBufferFormatEnumeration*	pZBufferEnumeration = (ZBufferFormatEnumeration*) lpContext;
	
	if (lpDDPixFmt->dwFlags & DDPF_ZBUFFER) {
		if (pZBufferEnumeration->dontUseStencil && (lpDDPixFmt->dwFlags & DDPF_STENCILBUFFER))
			return D3DENUMRET_OK;
		if (lpDDPixFmt->dwZBufferBitDepth <= pZBufferEnumeration->maxZBitDepth) {
			if (lpDDPixFmt->dwZBufferBitDepth >= pZBufferEnumeration->detectedZBitDepth) {
				if (!pZBufferEnumeration->dontUseStencil) {
					if (lpDDPixFmt->dwZBufferBitDepth != pZBufferEnumeration->detectedZBitDepth)
						pZBufferEnumeration->detectedStencilBitDepth = 0;

					if (lpDDPixFmt->dwStencilBitDepth < pZBufferEnumeration->detectedStencilBitDepth)
						return D3DENUMRET_OK;
					pZBufferEnumeration->detectedStencilBitDepth = lpDDPixFmt->dwStencilBitDepth;
				}
				pZBufferEnumeration->detectedZBitDepth = lpDDPixFmt->dwZBufferBitDepth;
				pZBufferEnumeration->selectedZBufferFormat = *lpDDPixFmt;
			}
		}
	}
	return D3DENUMRET_OK;
}


int		DX7LfbDepth::AttachDetachZBuffer (int attach)
{
	HRESULT					hr;
	LPDIRECTDRAWSURFACE7	buffers[3];

	buffers[0] = renderBuffers[0];
	buffers[1] = renderBuffers[1];
	buffers[2] = movie.cmiBuffNum >=3 ? renderBuffers[2] : NULL;

	if (attach) {
		int	i;
		for (i = 0; i <3; i++) {
			if (buffers[i] != NULL) {
				hr = buffers[i]->AddAttachedSurface (lpDDdepth);
				if (FAILED (hr))
					break;
			}
		}
		if (FAILED (hr)) {
			for (; i >= 0; i--) {
				buffers[i]->DeleteAttachedSurface (0, lpDDdepth);
			}
		}
	} else {
		for (int i = 0; i <3; i++) {
			if (buffers[i] != NULL) {
				hr = buffers[i]->DeleteAttachedSurface (0, lpDDdepth);
			}
		}
	}
	return !FAILED (hr);
}


int		DX7LfbDepth::CreateDepthBuffer (unsigned int width, unsigned int height, unsigned int bitDepth,
									    int createWithStencil, unsigned int* zBitDepth, unsigned int* stencilBitDepth)
{
ZBufferFormatEnumeration zBufferEnumeration;	
int						 tryNewFormat;
DDSURFACEDESC2			 zBuffDesc;

	if (bitDepth == 0)
		bitDepth = 32;
	do {
		zBufferEnumeration.dontUseStencil = !createWithStencil;
		zBufferEnumeration.maxZBitDepth = bitDepth;
		zBufferEnumeration.detectedZBitDepth = zBufferEnumeration.detectedStencilBitDepth = 0;
		lpD3D->EnumZBufferFormats (*GetDeviceGUID (device), CreateDepthBufferCallback, &zBufferEnumeration);

		tryNewFormat = 0;
		if (zBufferEnumeration.detectedZBitDepth != 0) {
			ZeroMemory (&zBuffDesc, sizeof (DDSURFACEDESC2));
			zBuffDesc.dwSize = sizeof (DDSURFACEDESC2);
			zBuffDesc.dwWidth = width;
			zBuffDesc.dwHeight = height;
			zBuffDesc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
			zBuffDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS;
			zBuffDesc.ddpfPixelFormat = zBufferEnumeration.selectedZBufferFormat;
			HRESULT hr = lpDD->CreateSurface (&zBuffDesc, &lpDDdepth, NULL);
			
			if (FAILED (hr)) {
				if (bitDepth != 16) {
					bitDepth = 16;
					tryNewFormat = 1;
				}
				lpDDdepth = NULL;
			}
		}
	} while (tryNewFormat);

	*zBitDepth = zBufferEnumeration.detectedZBitDepth;
	*stencilBitDepth = zBufferEnumeration.detectedStencilBitDepth;
	
	return (lpDDdepth != NULL) ? AttachDetachZBuffer (1) : 0;
}


void	DX7LfbDepth::DestroyDepthBuffer ()
{
	AttachDetachZBuffer (0);
	lpDDdepth->Release ();
	lpDDdepth = NULL;
}


int		DX7LfbDepth::GetLfbFormat (_pixfmt* format)
{
	lfbDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	renderBuffers[0]->GetPixelFormat(&lfbDesc.ddpfPixelFormat);

	ConvertDDPixelFormatToPixFmt (&lfbDesc.ddpfPixelFormat, &backBufferFormat, 0);
	if (format != NULL)
		*format = backBufferFormat;

	return (1);
}


void	DX7LfbDepth::ClearBuffers ()
{
DDBLTFX					ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = 0;

	for (int i = 0; i < 3; i++) {
		if (renderBuffers[i] != NULL) {
			renderBuffers[i]->Blt (NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		}
		if (lockData[i].lpDDScaleBuffHw != NULL) {
			lockData[i].lpDDScaleBuffHw->Blt (NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		}
		if (lockData[i].lpDDScaleBuffSw != NULL) {
			lockData[i].lpDDScaleBuffSw->Blt (NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		}
		if (lockData[i].lpDDAuxHw != NULL) {
			lockData[i].lpDDScaleBuffHw->Blt (NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		}
	}
}


void	DX7LfbDepth::FillScalingBuffer (int buffer, RECT* rect, unsigned int colorKey, int mirrored, int pointerIsInUse)
{
LPDIRECTDRAWSURFACE7	lpDDScaleBuff;
DDCOLORKEY				ddColorKey;
DDBLTFX					ddbltfx;

	LockData* lockDataPtr = lockData + buffer;

	if (pointerIsInUse) {
		lpDDScaleBuff = (lockDataPtr->lockFlags & LOCK_RSOFTWARE) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;
	} else {
		if ((mirrored && (!(bltCaps & BLT_CANMIRROR))) || (!(bltCaps & (BLT_CANSRCCOLORKEY | BLT_FASTBLT))) ) {
			lpDDScaleBuff = lockDataPtr->lpDDScaleBuffSw;
			lockDataPtr->lockFlags |= LOCK_RSOFTWARE;
		} else {
			lpDDScaleBuff = lockDataPtr->lpDDScaleBuffHw;
			lockDataPtr->lockFlags &= ~LOCK_RSOFTWARE;
		}
	}

	ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);

	/* Sajna a DirectX nem támogat colokey-maszkot, ezért ezt nem tudjuk figyelembe venni */
	ddColorKey.dwColorSpaceHighValue =
	ddColorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &backBufferFormat);
	
	lpDDScaleBuff->SetColorKey(DDCKEY_SRCBLT, &ddColorKey);
	
	ddbltfx.dwFillColor = ddColorKey.dwColorSpaceLowValue;
	lpDDScaleBuff->Blt(rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}


void	DX7LfbDepth::BlitToScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, int mirrored, int /*scaled*/, int pointerIsInUse)
{
LPDIRECTDRAWSURFACE7	lpDDScaleBuff;
DWORD					blitMode;
DDBLTFX					ddbltfx;

	LockData* lockDataPtr = lockData + buffer;

	if (pointerIsInUse) {
		lpDDScaleBuff = (lockDataPtr->lockFlags & LOCK_RSOFTWARE) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;
	} else {
		if (mirrored && (!(bltCaps & BLT_CANMIRROR)) || (!(bltCaps & BLT_FASTBLT)) ) {
			lpDDScaleBuff = lockDataPtr->lpDDScaleBuffSw;
			lockDataPtr->lockFlags |= LOCK_RSOFTWARE;
		} else {
			lpDDScaleBuff = lockDataPtr->lpDDScaleBuffHw;
			lockDataPtr->lockFlags &= ~LOCK_RSOFTWARE;
		}
	}

	ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);

	if (mirrored) {
		ddbltfx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
		blitMode = DDBLT_DDFX;
	} else {
		blitMode = 0;
	}

	lpDDScaleBuff->Blt(dstRect, renderBuffers[buffer], srcRect, blitMode | DDBLT_WAIT, &ddbltfx);
}


void	DX7LfbDepth::BlitFromScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, BlitType blitType,
											 int mirrored, int /*scaled*/)
{
LPDIRECTDRAWSURFACE7	lpDDScaleBuff;
DWORD					blitMode;
DDBLTFX					ddBltFx;

	LockData* lockDataPtr = lockData + buffer;

	ZeroMemory(&ddBltFx, sizeof(DDBLTFX));
	ddBltFx.dwSize = sizeof(DDBLTFX);
	
	lpDDScaleBuff = (lockDataPtr->lpDDScaleBuffHw == NULL) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;
	if (lockDataPtr->lockFlags & LOCK_RSOFTWARE) {
		lockDataPtr->lpDDScaleBuffSw;
	}

	//lpDDScaleBuff = (lockDataPtr->lockFlags & LOCK_RSOFTWARE) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;

	if (mirrored)	{
		ddBltFx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
		blitMode = DDBLT_DDFX;
	} else {
		blitMode = 0;
	}
	if (blitType == CopyWithColorKey)
		blitMode |= DDBLT_KEYSRC;

	renderBuffers[buffer]->Blt (dstRect, lpDDScaleBuff, srcRect, blitMode | DDBLT_WAIT, &ddBltFx);
}


int		DX7LfbDepth::IsBlitTypeSupported (BlitType /*blitType*/)
{
	return (1);
}


int		DX7LfbDepth::IsMirroringSupported ()
{
	return (1);
}


int		DX7LfbDepth::CopyBetweenScalingBuffers (int srcBuffer, int dstBuffer, RECT* rect)
{
	LPDIRECTDRAWSURFACE7	lpDDSrcScaleBuff;
	if (srcBuffer < 3) {
		if (!(lockData[srcBuffer].lockFlags & LOCK_RSOFTWARE))
			return (0);
		lpDDSrcScaleBuff = lockData[srcBuffer].lpDDScaleBuffSw;
	} else {
		lpDDSrcScaleBuff = lpDDTempScaleBuffSw;
	}

	if (lpDDSrcScaleBuff == NULL)
		return (0);

	LPDIRECTDRAWSURFACE7	lpDDDstScaleBuff;
	if (dstBuffer < 3) {
		lpDDDstScaleBuff = lockData[dstBuffer].lpDDScaleBuffSw;
	} else
		lpDDDstScaleBuff = lpDDTempScaleBuffSw;

	if (lpDDDstScaleBuff == NULL)
		return (0);

	lpDDDstScaleBuff->BltFast(0, 0, lpDDSrcScaleBuff, rect, DDBLTFAST_NOCOLORKEY);

	return (1);
}


int		DX7LfbDepth::AreHwScalingBuffersUsed ()
{
	return bltCaps & BLT_FASTBLT;
}


int		DX7LfbDepth::CreateAuxBuffers (unsigned int width, unsigned int height)
{
	lfbDesc.dwWidth = width;
	lfbDesc.dwHeight = height;
	lfbDesc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	lfbDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;// | DDSCAPS_VIDEOMEMORY;

	int i;
	for(i = 0; i < (int) movie.cmiBuffNum; i++)	{
		if (lpDD->CreateSurface(&lfbDesc, &lockData[i].lpDDAuxHw, NULL) != DD_OK)
			break;
		lockData[i].lockFlags &= ~LOCK_AUXDIRTY;
	}
	if (i != (int) movie.cmiBuffNum) {
		for (; i >= 0; i--) {
			lockData[i].lpDDAuxHw->Release ();
			lockData[i].lpDDAuxHw = NULL;
		}
		return (0);
	}
	return (1);
}


void	DX7LfbDepth::DestroyAuxBuffers ()
{
	for(unsigned int i = 0; i < movie.cmiBuffNum; i++)	{
		if (lockData[i].lpDDAuxHw != NULL) {
			lockData[i].lpDDAuxHw->Release ();
			lockData[i].lpDDAuxHw = NULL;
		}
	}
}


void	DX7LfbDepth::BlitToAuxBuffer (int buffer, RECT* srcRect)
{
	if (lockData[buffer].lpDDAuxHw->IsLost() != DD_OK)
		lockData[buffer].lpDDAuxHw->Restore();
	
	lockData[buffer].lpDDAuxHw->BltFast(0, 0, renderBuffers[buffer], srcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	lockData[buffer].lockFlags |= LOCK_AUXDIRTY;
}


void	DX7LfbDepth::BlitFromAuxBuffer (int buffer, unsigned int x, unsigned y)
{
	if (lockData[buffer].lockFlags & LOCK_AUXDIRTY) {
		renderBuffers[buffer]->BltFast(x, y, lockData[buffer].lpDDAuxHw, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
		lockData[buffer].lockFlags &= ~LOCK_AUXDIRTY;
	}
}


void	DX7LfbDepth::RotateBufferProperties (int numOfBuffers, int rotateBuffers)
{
	LPDIRECTDRAWSURFACE7 lpDDScaleBuffSw = NULL;
	LPDIRECTDRAWSURFACE7 lpDDScaleBuffHw = NULL;
	
	if (rotateBuffers) {
		lpDDScaleBuffSw = lockData[0].lpDDScaleBuffSw;
		lpDDScaleBuffHw = lockData[0].lpDDScaleBuffHw;
	}
		
	LPDIRECTDRAWSURFACE7 lpDDAuxBuffHw = lockData[0].lpDDAuxHw;
	unsigned int lockFlags = lockData[0].lockFlags;
	for(int i = 1; i < numOfBuffers; i++)	{
		if (rotateBuffers) {
			lockData[i-1].lpDDScaleBuffSw = lockData[i].lpDDScaleBuffSw;
			lockData[i-1].lpDDScaleBuffHw = lockData[i].lpDDScaleBuffHw;
		}
		lockData[i-1].lpDDAuxHw = lockData[i].lpDDAuxHw;
		lockData[i-1].lockFlags = lockData[i].lockFlags;
	}
	if (rotateBuffers) {
		lockData[numOfBuffers-1].lpDDScaleBuffSw = lpDDScaleBuffSw;
		lockData[numOfBuffers-1].lpDDScaleBuffHw = lpDDScaleBuffHw;
	}
	lockData[numOfBuffers-1].lpDDAuxHw = lpDDAuxBuffHw;
	lockData[numOfBuffers-1].lockFlags = lockFlags;
}


int		DX7LfbDepth::LockBuffer (int buffer, BufferType bufferType, void** buffPtr, unsigned int* pitch)
{
	LPDIRECTDRAWSURFACE7	lpDDBuff = NULL;
	LockData*				lockDataPtr = lockData + buffer;
	
	switch (bufferType) {
		case ColorBuffer:
			lpDDBuff = renderBuffers[buffer];
			break;

		case ScalingBuffer:
			lpDDBuff = (lockDataPtr->lpDDScaleBuffHw == NULL) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;
			if (lockDataPtr->lockFlags & LOCK_RSOFTWARE) {
				lockDataPtr->lpDDScaleBuffSw;
			}
			break;
	}

	if (lpDDBuff->Lock(NULL, &lfbDesc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL) != DD_OK) return(0);

	*buffPtr = lfbDesc.lpSurface;
	*pitch = lfbDesc.lPitch;
	return (1);
}


int		DX7LfbDepth::UnlockBuffer (int buffer, BufferType bufferType)
{
	LPDIRECTDRAWSURFACE7	lpDDBuff;
	LockData*				lockDataPtr = lockData + buffer;
	
	switch (bufferType) {
		case ColorBuffer:
			lpDDBuff = renderBuffers[buffer];
			break;

		case ScalingBuffer:
			lpDDBuff = (lockDataPtr->lpDDScaleBuffHw == NULL) ? lockDataPtr->lpDDScaleBuffSw : lockDataPtr->lpDDScaleBuffHw;
			if (lockDataPtr->lockFlags & LOCK_RSOFTWARE) {
				lockDataPtr->lpDDScaleBuffSw;
			}
			break;
	}

	if (lpDDBuff->Unlock(NULL) != DD_OK)
		return(0);

	return (1);
}


/*int		DX7LfbDepth::RestoreBufferIfNeeded (int buffer, BufferType bufferType)
{
	LPDIRECTDRAWSURFACE7	lpDDBuff;
	switch (bufferType) {
		case ColorBuffer:
			lpDDBuff = renderBuffers[buffer];
			break;

		case ScalingBuffer:
			lpDDBuff = lockData[buffer].lpDDScaleBuffHw;
			break;
	}
	if (lpDDBuff->IsLost() != DD_OK)	{
		lpDDBuff->Restore();
		return (1);
	}
	return (0);
}*/


int		DX7LfbDepth::CreatePrimaryPalette ()
{
PALETTEENTRY		dPalette[256];
	
	ZeroMemory (dPalette, 256*sizeof (PALETTEENTRY));
	
	lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, /*| DDPCAPS_PRIMARYSURFACE*/ dPalette, &lpDDPalette, NULL);
	renderBuffers[0]->SetPalette (lpDDPalette);

	return (1);
}


void	DX7LfbDepth::DestroyPrimaryPalette ()
{
	renderBuffers[0]->SetPalette (NULL);
	lpDDPalette->Release ();
}


void	DX7LfbDepth::SetPrimaryPaletteEntries (PALETTEENTRY* entries)
{
	lpDDPalette->SetEntries(0, 0, 256, entries);
}

/*------------------------------------------------------------------------------------------*/
/*..................... DX7 implementáció a mélységi puffereléshez .........................*/

int		DX7LfbDepth::InitDepthBuffering (enum DepthBufferingDetectMethod detectMethod)
{
	DDCAPS		caps = {sizeof(DDCAPS)};
	D3DMATRIX	projectionMatrix;
	
	lpDDdepth = NULL;

	depthCaps = 0;
	depthBufferingMode = None;
	depthBufferCreated = compareStencilModeUsed = 0;
	
	lpDD->GetCaps(&caps, NULL);
	if (caps.ddsCaps.dwCaps & DDSCAPS_ZBUFFER) {
		depthCaps |= ZBUFF_OK;
	} else {
		DEBUGLOG(0,"\n   Warning: Close to fatal: DX7LfbDepth::InitDepthBuffering: Z-buffering is not supported by the driver!!");
		DEBUGLOG(1,"\n   Figyelmeztetés: Majdnem végzetes: DX7LfbDepth::InitDepthBuffering: a Z-pufferelést a driver nem támogatja!!");
	}
	
	ZeroMemory(&projectionMatrix, sizeof(D3DMATRIX));
	projectionMatrix._11 = projectionMatrix._22 = projectionMatrix._33 = projectionMatrix._44 = 1.0f;
	projectionMatrix._33 = (WNEAR / (WFAR - WNEAR)) + 1.0f;
	projectionMatrix._44 = WNEAR;
	projectionMatrix._43 = 0.0f;
	projectionMatrix._34 = 1.0f;
	lpD3Ddevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &projectionMatrix);
	
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILPASS, D3DSTENCILOP_ZERO);
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILFAIL, D3DSTENCILOP_KEEP);
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILREF, 0);
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILMASK, 0xFFFFFFFF);

	LfbDetermineWBufferingMethod (detectMethod);

	return (1);
}


void	DX7LfbDepth::DeInitDepthBuffering ()
{
	if (depthBufferCreated)
		DestroyDepthBuffer ();
	
	depthBufferingMode = None;
	depthBufferCreated = 0;
}


void	DX7LfbDepth::LfbDetermineWBufferingMethod(enum DepthBufferingDetectMethod detectMethod)	{
D3DDEVICEDESC7	devdesc;

	lpD3Ddevice->GetCaps(&devdesc);
	if (devdesc.dvMaxVertexW < 65528.0f)	{
		DEBUGLOG(0,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Maximum W-based depth value that device supports is %f, true W-buffering CANNOT be used!", devdesc.dvMaxVertexW);
		DEBUGLOG(1,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Az eszköz által támogatott maximális W-alapú mélységi érték %f, a valódi W-pufferelés NEM használható!", devdesc.dvMaxVertexW);
		return;
	}

	switch (detectMethod) {
		case AlwaysUseW:
			depthCaps |= WBUFF_OK;
			DEBUGLOG(0,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Being forced to use true W-buffering!");
			DEBUGLOG(1,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: A wrapper valódi W-pufferelés használatára van kényszerítve!");
			break;

		case AlwaysUseZ:
			DEBUGLOG(0,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Being forced to use emulated W-buffering!");
			DEBUGLOG(1,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: A wrapper emulált W-pufferelés használatára van kényszerítve!");
			break;

		case DetectViaDriver:
			if ( (devdesc.dpcLineCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER) &&
				(devdesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER) )	{
				depthCaps |= WBUFF_OK;
				DEBUGLOG(0,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering can be used!");
				DEBUGLOG(1,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés használható!");
			} else {
				DEBUGLOG(0,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering CANNOT be used, using emulated W-buffering instead!");
				DEBUGLOG(1,"\nDX7LfbDepth::LfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés NEM használható, helyette emuláció!");
			}
			break;
		
		default:
			break;

	}
}


void		DX7LfbDepth::SetDepthBufferingMode (GrDepthBufferMode_t mode)
{
	DWORD			d3dMode;
	char			stencilModeUsed;

	if (mode != GR_DEPTHBUFFER_DISABLE)	{
		if (!depthBufferCreated)	{

			if (!(depthCaps & ZBUFF_OK)){
				DEBUGLOG(0,"\n   Error: DX7LfbDepth::SetDepthBufferingMode: application is using depth buffering but it is not supported by the driver!!");
				DEBUGLOG(1,"\n   Hiba: DX7LfbDepth::SetDepthBufferingMode: az alkalmazás mélységi pufferelést használ, de a driver nem támogatja!!");
				return;
			}


			zBitDepth = (config.Flags2 & CFG_DEPTHEQUALTOCOLOR) ? movie.cmiBitPP : 0;
			if (!CreateDepthBuffer (movie.cmiXres, movie.cmiYres, zBitDepth, 1, &zBitDepth, &stencilBitDepth)) {
				DEBUGLOG(0,"\ngrDepthBufferMode: Used depth buffer bit depth: %d, stencil bits: %d", zBitDepth, stencilBitDepth);
				DEBUGLOG(0,"\n   Error: grDepthBufferMode: Creating depth buffer failed");
				DEBUGLOG(1,"\ngrDepthBufferMode: A használt bitmélység: %d, stencil bitek: %d", zBitDepth, stencilBitDepth);
				DEBUGLOG(1,"\n   Hiba: grDepthBufferMode: A mélységi puffer létrehozása nem sikerült");
				return;
			}

			if (stencilBitDepth == 0)	{
				DEBUGLOG(0,"\n   Warning: DX7LfbDepth::SetDepthBufferingMode: Couldn't create a depth buffer with stencil component. 'Compare to bias' depth buffering modes won't work.\
							\n												  Tip: your videocard either does not support stencil buffers (old cards) or only does it at higher bit depths.\
							\n												  If you are using a 16 bit mode then try to use the wrapper in 32 bit mode!");

				DEBUGLOG(1,"\n   Figyelmeztetés: DX7LfbDepth::SetDepthBufferingMode: Nem sikerült olyan mélységi puffert létrehozni, amely rendelkezik stencil-összetevõvel.\
							\n														 A 'Compare to bias'-módok nem fognak mûködni a mélységi puffer használatakor.\
							\n														 Tipp: a videókártyád vagy egyáltalán nem támogatja a stencil-puffereket (régi kártyák) vagy csak nagyobb\
							\n														 bitmélység mellett. Ha egy 16 bites módot használsz, próbálj ki egy 32 bites módot!");
			}
			
			depthBufferCreated = 1;
			lpD3Ddevice->SetRenderTarget (renderBuffers[renderbuffer], 0);
		}
	}

	stencilModeUsed = 0;
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

				lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, astate.perspective = TRUE);
			}
			break;
		
		default:
			d3dMode = D3DZB_FALSE;
			depthBufferingMode = None;
			break;
	}

	HRESULT hr = lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, d3dMode);
	DEBUGCHECK(0,(hr!=DD_OK),"\n   Error: grDepthBufferMode: Return of setting depth buffer mode: %0x, error",hr);
	DEBUGCHECK(1,(hr!=DD_OK),"\n   Hiba: grDepthBufferMode: A mélységi puffer módjának beállításának visszatérési értéke: %0x, hiba",hr);

	if (stencilModeUsed != compareStencilModeUsed)	{
		if (stencilBitDepth)
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_STENCILENABLE, stencilModeUsed ? TRUE : FALSE);

		dx7Drawing->SetDepthBias (astate.depthbiaslevel, depthBufferingMode, compareStencilModeUsed = stencilModeUsed);
	}
}


void		DX7LfbDepth::SetDepthBufferFunction (GrCmpFnc_t function)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZFUNC, cmpFuncLookUp[function]);
}


void		DX7LfbDepth::EnableDepthMask (int enable)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, enable ? TRUE : FALSE);
}


void		DX7LfbDepth::SetDepthBias (FxI16 level)
{
	dx7Drawing->SetDepthBias (level, depthBufferingMode == WBufferingEmu, compareStencilModeUsed);
}


enum DX7LfbDepth::DepthBufferingMode	DX7LfbDepth::GetTrueDepthBufferingMode ()
{
	return depthBufferingMode;
}


int			DX7LfbDepth::RestoreResources ()
{

	for (int i = 0; i < 3; i++) {
		if (lockData[i].lpDDScaleBuffHw != NULL) {
			if (!RestoreSurface (lockData[i].lpDDScaleBuffHw))
				return (0);
		}
		if (lockData[i].lpDDAuxHw != NULL) {
			if (!RestoreSurface (lockData[i].lpDDAuxHw))
				return (0);
		}
	}
	return (1);
}