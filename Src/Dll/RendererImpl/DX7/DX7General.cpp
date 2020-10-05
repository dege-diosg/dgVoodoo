/*--------------------------------------------------------------------------------- */
/* DX7General.cpp - dgVoodoo 3D general rendering interface, DX7 implementation     */
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


/*------------------------------------------------------------------------------------------*/
/* dgVoodoo: DX7General.cpp																	*/
/*			 DX7 renderer general class implementation										*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

extern "C" {

#include	"Debug.h"
#include	"dgVoodooGlide.h"
#include	"Log.h"
#include	"dgVoodooBase.h"
#include	"Dgw.h"
#include	"Draw.h"
#include	"Main.h"
//#include	"DX7FFPixelShaders.h"

}

#include	"DX7General.hpp"
#include	"DX7LfbDepth.hpp"
#include	"DX7Drawing.hpp"
#include	"DX7BaseFunctions.h"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define		STATEDESC_DELIMITER			0xEEEEEEEE

/*-----------------------------------------------------------------------------------------	*/
/*....................................... Táblák ...........................................*/

/* A Glide alpha blending módok (faktorok) és a D3D módjai (faktorai)között leképezô táblázat */
/* Megtehetjük, mert a Glide-konstansok 0-tól valameddig vannak definiálva ilyen sorrendben. */

static DWORD alphaModeLookupSrc[] = {D3DBLEND_ZERO,				/* GR_BLEND_ZERO */
									 D3DBLEND_SRCALPHA,			/* GR_BLEND_SRC_ALPHA */
									 D3DBLEND_DESTCOLOR,		/* GR_BLEND_DST_COLOR */
									 D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
									 D3DBLEND_ONE,				/* GR_BLEND_ONE */
									 D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
									 D3DBLEND_INVDESTCOLOR,		/* GR_BLEND_ONE_MINUS_DST_COLOR */													   
									 D3DBLEND_INVDESTALPHA,		/* GR_BLEND_ONE_MINUS_DST_ALPHA */
									 0,0,0,0,0,0,0,				/* ezek fenntartottak */						  
									 D3DBLEND_SRCALPHASAT};		/* GR_BLEND_ALPHA_SATURE */

/* Magyarázat a GR_BLEND_PREFOG_COLOR-hoz: ez a blendelési mód a forrásszínt (SRCCOLOR) */
/* használja blending factorként, ua. mint a GR_BLEND_SRC_COLOR, annyi különbséggel, hogy */
/* ezen blending factorra nincs hatással a köd (az ugyanis még az alpha blending elõtt */
/* történik). */
static DWORD alphaModeLookupDst[] = {D3DBLEND_ZERO,				/* GR_BLEND_ZERO */
									 D3DBLEND_SRCALPHA,			/* GR_BLEND_SRC_ALPHA */
									 D3DBLEND_SRCCOLOR,			/* GR_BLEND_SRC_COLOR */
									 D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
									 D3DBLEND_ONE,				/* GR_BLEND_ONE */
									 D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
									 D3DBLEND_INVSRCCOLOR,		/* GR_BLEND_ONE_MINUS_SRC_COLOR */													
									 D3DBLEND_INVDESTALPHA,		/* GR_BLEND_ONE_MINUS_DST_ALPHA */
									 0,0,0,0,0,0,0,				/* ezek fenntartottak */
									 D3DBLEND_SRCCOLOR};		/* GR_BLEND_PREFOG_COLOR */

/* combine local paramétert konvertáló táblázat */
static DWORD combinelocallookup[] =		{D3DTA_DIFFUSE, D3DTA_TFACTOR, D3DTA_DIFFUSE};

/* combine other paramétert konvertáló táblázat */
static DWORD combineotherlookup[] =		{D3DTA_DIFFUSE, D3DTA_TEXTURE, D3DTA_TFACTOR};

static DWORD combineargd3dlookup[] =	{D3DTOP_BLENDDIFFUSEALPHA, 0, D3DTOP_BLENDTEXTUREALPHA, D3DTOP_BLENDFACTORALPHA};




static DWORD		scrShotSaveStates[] =	{D3DRENDERSTATE_ALPHATESTENABLE, D3DRENDERSTATE_ALPHABLENDENABLE,
											 D3DRENDERSTATE_SRCBLEND, D3DRENDERSTATE_DESTBLEND, D3DRENDERSTATE_ZENABLE,
											 D3DRENDERSTATE_CULLMODE, D3DRENDERSTATE_ZWRITEENABLE, 
											 D3DRENDERSTATE_TEXTUREPERSPECTIVE, D3DRENDERSTATE_COLORKEYENABLE,
											 D3DRENDERSTATE_FOGENABLE,
											 STATEDESC_DELIMITER};

static DWORD		scrShotSaveTSStates[]=	{0, D3DTSS_COLOROP,  0, D3DTSS_COLORARG1,  0, D3DTSS_COLORARG2,
											 0, D3DTSS_ALPHAOP, 0, D3DTSS_ALPHAARG1, 0, D3DTSS_ALPHAARG2,
											 0, D3DTSS_ADDRESSU, 0, D3DTSS_ADDRESSV, 0, D3DTSS_MAGFILTER,
											 0, D3DTSS_MINFILTER, 1, D3DTSS_COLOROP,
											 STATEDESC_DELIMITER};



static DWORD		lfbSaveStatesNoPipeline[] =	{D3DRENDERSTATE_ALPHATESTENABLE, D3DRENDERSTATE_ALPHABLENDENABLE,
												 D3DRENDERSTATE_ZENABLE, D3DRENDERSTATE_CULLMODE,
												 D3DRENDERSTATE_COLORKEYENABLE, D3DRENDERSTATE_FOGENABLE,
												 D3DRENDERSTATE_TEXTUREFACTOR, D3DRENDERSTATE_TEXTUREPERSPECTIVE,
												 STATEDESC_DELIMITER };

static DWORD		lfbSaveStatesPipeline[]   =	{D3DRENDERSTATE_CULLMODE, D3DRENDERSTATE_COLORKEYENABLE,
												 D3DRENDERSTATE_TEXTUREFACTOR, D3DRENDERSTATE_ZWRITEENABLE,
												 D3DRENDERSTATE_TEXTUREPERSPECTIVE,
												 STATEDESC_DELIMITER };

static DWORD		lfbSaveTSStatesNoPipeline[] =
												{0, D3DTSS_COLOROP, 0, D3DTSS_COLORARG1, 0, D3DTSS_ALPHAOP,
												 0, D3DTSS_ALPHAARG1, 0, D3DTSS_MAGFILTER, 0, D3DTSS_MINFILTER,
												 1, D3DTSS_COLOROP,
												 STATEDESC_DELIMITER};


static DWORD		gammaSaveStates[]	=	{D3DRENDERSTATE_ALPHATESTENABLE, D3DRENDERSTATE_ALPHABLENDENABLE,
											 D3DRENDERSTATE_SRCBLEND, D3DRENDERSTATE_DESTBLEND,
											 D3DRENDERSTATE_ZENABLE, D3DRENDERSTATE_CULLMODE,
											 STATEDESC_DELIMITER};

static DWORD		gammaSaveTSStates[]	=	{0, D3DTSS_COLOROP,  0, D3DTSS_COLORARG1, 1, D3DTSS_COLOROP,
											 STATEDESC_DELIMITER};




/*------------------------------------------------------------------------------------------*/
/*...................... DX7 implementáció az általános dolgokhoz ..........................*/


static const DWORD	dxCullModeLookUp[] = {D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW};


HRESULT WINAPI DX7General::BackbufferEnumSurfacesCallBack(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDDSURFACEDESC2 /*lpDDSurfaceDesc*/,
														  LPVOID lpContext)
{
	*(LPDIRECTDRAWSURFACE7*) lpContext = lpDDSurface;
	return (DDENUMRET_CANCEL);
}


HRESULT WINAPI DX7General::DisplayModeEnumCallBack (LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext)
{
DisplayMode				displayMode;
DISPLAYMODEENUMCALLBACK callbackFunc = (DISPLAYMODEENUMCALLBACK) lpContext;
	
	displayMode.xRes = lpDDSurfaceDesc->dwWidth;
	displayMode.yRes = lpDDSurfaceDesc->dwHeight;
	//displayMode.bitDepth = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
	displayMode.refreshRate = lpDDSurfaceDesc->dwRefreshRate;
	ConvertDDPixelFormatToPixFmt (&lpDDSurfaceDesc->ddpfPixelFormat, &displayMode.pixFmt, 0);

	(*callbackFunc) (&displayMode);
	return (DDENUMRET_OK);
}


int			DX7General::Create (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
								unsigned int bufferNum, unsigned int flags)
{
DDSURFACEDESC2	ddsd;
HRESULT			hr = S_OK;

	ZeroMemory (&displayMode, sizeof (DisplayMode));
	displayMode.xRes = xRes;
	displayMode.yRes = yRes;
	displayMode.pixFmt.BitPerPixel = bitPP;
	displayMode.refreshRate = monRefreshRate;
	createFlags = flags;
	this->bufferNum = bufferNum;

	for (int i = 0; i < MAX_TEXSTAGES; i++) {
		cachedColorOp[i] = cachedAlphaOp[i] = cachedColorArg1[i] = cachedAlphaArg1[i] = cachedColorArg2[i] = cachedAlphaArg2[i] = 0xEEEEEEEE;
	}
	nativeTntCkMethodUsed = 0;

	astagesnum = cstagesnum = maxUsedStageNum = 0;

	if (flags & SWM_USECLOSESTFREQ) {
		lpDD->EnumDisplayModes(DDEDM_REFRESHRATES, NULL, &displayMode, SetWorkingModeEnumRefreshRatesCallback);
		monRefreshRate = displayMode.refreshRate >> 16;
		displayMode.refreshRate &= 0xFFFF;
	}

	//Ha fullscreen, videómód beállítása
	if (!(flags & SWM_WINDOWED)) {
		//My old TNT2 card needed this behavior... Had to be some timing issue, but I left it...
		int	tryNum = 5;
		while (tryNum--) {
			hr = lpDD->SetDisplayMode (xRes, yRes, bitPP, monRefreshRate, 0);
			if (hr != 0x80004005)
				break;
		}
		if (FAILED (hr))
			return (0);
	}

	DEBUGLOG (0, "\nSetDisplayMode... OK");
	
	ZeroMemory (&ddsd, sizeof (DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);

	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	//Ha fullscreen, flipping chain elõállítása
	if (!(flags & SWM_WINDOWED)) {
		ddsd.dwFlags |= DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX | ((flags & SWM_3D) ? DDSCAPS_3DDEVICE : 0);
		ddsd.dwBackBufferCount = (bufferNum >= 3) ? 2 : (bufferNum-1);
	}
	
	hr = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL);

	if (!FAILED (hr)) {

		DEBUGLOG (0, "\nCreate primary surface... OK");

		if (!(flags & SWM_WINDOWED)) {
			ddsd.dwFlags = DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
			
			hr = lpDDSPrimary->GetAttachedSurface (&ddsd.ddsCaps, &lpDDSBack);
			lpDDSThird = NULL;
			if (!FAILED (hr)) {
				DEBUGLOG (0, "\nGet backbuffer... OK");
				if (bufferNum >= 3) {
					hr = lpDDSBack->EnumAttachedSurfaces (&lpDDSThird, BackbufferEnumSurfacesCallBack);
				}
				if (!FAILED (hr)) {
					DEBUGLOG (0, "\nAll... OK");
					movie.cmiBitPP = bitPP;
					movie.cmiByPP = bitPP / 8;
					renderBuffers[FrontBuffer] = lpDDSPrimary;
					renderBuffers[BackBuffer] = lpDDSBack;
					renderBuffers[ThirdBuffer] = lpDDSThird;
					return (1);
				}
			}
		} else {
			hr = lpDDSPrimary->SetClipper (lpDDClipper);
			if (!FAILED (hr)) {
				ddsd.dwWidth = xRes;
				ddsd.dwHeight = yRes;
				ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
				ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | ((flags & SWM_3D) ? DDSCAPS_3DDEVICE : 0);
				ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);

				lpDDSPrimary->GetPixelFormat (&ddsd.ddpfPixelFormat);
				movie.cmiBitPP = ddsd.ddpfPixelFormat.dwRGBBitCount;
				movie.cmiByPP = ddsd.ddpfPixelFormat.dwRGBBitCount / 8;
				int i;
				for (i = 0; i < (int) bufferNum; i++) {
					hr = lpDD->CreateSurface (&ddsd, renderBuffers + i, NULL);
					if (FAILED (hr))
						break;
				}
				if (!FAILED (hr))
					return (1);

				for (; i >= 0; i--)
					renderBuffers[i]->Release ();
			}
			lpDDSPrimary->SetClipper (NULL);
		}
		lpDDSPrimary->Release ();
	}
	if (!(flags & SWM_WINDOWED))
		lpDD->RestoreDisplayMode ();

	return (0);
}


void	DX7General::Destroy ()
{
	lpDDSPrimary->SetClipper (NULL);
	
	if (createFlags & SWM_WINDOWED) {
		for (int i = 2; i >= 0; i--) {
			if (renderBuffers[i] != NULL)
				renderBuffers[i]->Release ();
			renderBuffers[i] = NULL;
		}
	} else {
		lpDDSBack->Release ();
	}

	lpDDSPrimary->Release ();

	lpDDSPrimary = lpDDSBack = lpDDSThird = NULL;
}


void	DX7General::FlipBuffer (int atNextVerticalRetrace)
{
	if (lpDD->TestCooperativeLevel () == DD_OK) {
		if (createFlags & SWM_WINDOWED) {
			RefreshDisplayByBuffer (BackBuffer);
		} else {
			lpDDSPrimary->Flip (NULL, (atNextVerticalRetrace ? 0 : DDFLIP_NOVSYNC) | DDFLIP_WAIT);
		}
	}
}


HRESULT WINAPI	DX7General::SetWorkingModeEnumRefreshRatesCallback(LPDDSURFACEDESC2 lpDDsd, LPVOID lpContext)
{
	DisplayMode* displayMode = (DisplayMode*) lpContext;

	if ( (lpDDsd->ddpfPixelFormat.dwRGBBitCount == displayMode->pixFmt.BitPerPixel/*bitDepth*/) &&
		(lpDDsd->dwWidth == displayMode->xRes) && (lpDDsd->dwHeight == displayMode->yRes) ) {

		int foundRefRate = displayMode->refreshRate >> 16;
		int reqRefRate = displayMode->refreshRate & 0xFFFF;
		if ( ABS((int) lpDDsd->dwRefreshRate - reqRefRate) < ABS(foundRefRate - reqRefRate) ) foundRefRate = lpDDsd->dwRefreshRate;
		displayMode->refreshRate = (foundRefRate << 16) + reqRefRate;
	}
	return(D3DENUMRET_OK);
}


/* Egy pixel shaderben megadott argumentumot átalakít D3DTA formátumra.  */
/* Figyelembe veszi, ha a texture combine egység a kimenetet invertálja. */
DWORD /*_inline*/ _fastcall	DX7General::GlideGetStageArg(DWORD type, DWORD d3dFactor, DWORD d3dLocal, DWORD d3dOther, int texinvert)
{
register DWORD	modifier = 0;
	
	if (type & PS_INVERT)
		modifier ^= D3DTA_COMPLEMENT;
	
	if (type & PS_ALPHA)
		modifier |= D3DTA_ALPHAREPLICATE;
	
	switch(type & (~(PS_ALPHA | PS_INVERT)) )	{
		case PS_LOCAL:
			return(d3dLocal ^ modifier);
		
		case PS_OTHER:
			if ( texinvert && ((d3dOther & D3DTA_SELECTMASK) == D3DTA_TEXTURE) )
				modifier ^= D3DTA_COMPLEMENT;
			return(d3dOther ^ modifier);
		
		case PS_FACTOR:
			return(d3dFactor ^ modifier);
		
		default:
			return(type ^ modifier);
	}
}



int			DX7General::InitRendererApi ()
{
	dx7General = this;
	return InitDX7 ();
}


void		DX7General::UninitRendererApi ()
{
	UninitDX7 ();
}


int			DX7General::GetApiCaps ()
{
	return (CAP_FRONTBUFFAVAILABLE);
}


int			DX7General::SetWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										unsigned int bufferNum, HWND hWnd, unsigned short adapter, unsigned short device,
										unsigned int flags)
{
	int	success = EnumDX7Adapters ();
	
	if (success && (adapter < GetNumOfDX7Adapters ())) {
		if (CreateDirectDraw7OnAdapter (adapter)) {
			lpDD = GetDirectDraw7 ();

			DWORD clFlags = (flags & SWM_WINDOWED) ? DDSCL_NORMAL : (DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
			lpDD->SetCooperativeLevel (hWnd, clFlags | DDSCL_FPUPRESERVE);

			if (flags & SWM_WINDOWED) {
				lpDD->CreateClipper (0, &lpDDClipper, NULL);
				lpDDClipper->SetHWnd (0, hWnd);
			} else {
				lpDDClipper = NULL;
			}

			if (Create (xRes, yRes, bitPP, monRefreshRate, bufferNum, flags)) {
				this->hWnd = hWnd;
				this->adapter = adapter;
				if (CreateDirect3D7 ()) {
					lpD3D = GetDirect3D7 ();
					if (Enum3DDevicesOnDirect3D7 ()) {
						if (device <= GetNumOf3D7Devices ()) {
							if (device == 0) {
								for (device = 1; device <= GetNumOf3D7Devices (); device++) {
									if (IsHwDevice (device-1))
										break;
								}
							}
							this->device = device-1;
							
							if (flags & SWM_3D) {
							
								HRESULT hr = lpD3D->CreateDevice (*GetDeviceGUID (device-1), renderBuffers[1], &lpD3Ddevice);
								if (!FAILED (hr))
									return (1);
							} else {
								return (1);
							}
						}
						DestroyDirect3D7 ();
					}
				}
				Destroy ();
			}
			lpDD->SetCooperativeLevel (hWnd, DDSCL_NORMAL);
			if (lpDDClipper != NULL)
				lpDDClipper->Release ();
			DestroyDirectDraw7 ();
		}
	}
	return (0);
}


int			DX7General::ModifyWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										   unsigned int bufferNum, unsigned int flags)
{
	flags = (createFlags & SWM_WINDOWED) | (flags & ~SWM_WINDOWED);

	if ((xRes != displayMode.xRes) || (yRes != displayMode.yRes) || (bufferNum != this->bufferNum) ||
		((!(flags & SWM_WINDOWED)) && ((bitPP != displayMode.pixFmt.BitPerPixel/*bitDepth*/) || (monRefreshRate != displayMode.refreshRate)))) {

		if (lpD3Ddevice != NULL)
			lpD3Ddevice->Release ();
		
		if (lpD3D != NULL) {
			lpD3D->EvictManagedTextures ();
			lpD3D->Release ();
		}

		lpD3Ddevice = NULL;
		lpD3D = NULL;

		Destroy ();
		if (Create (xRes, yRes, bitPP, monRefreshRate, bufferNum, flags)) {
			DEBUGLOG (0, "\nCreate... OK");
			if (flags & SWM_3D) {
				if (CreateDirect3D7 ()) {
					DEBUGLOG (0, "\nCreateDirect3D7... OK");
					lpD3D = GetDirect3D7 ();
					HRESULT hr = lpD3D->CreateDevice (*GetDeviceGUID (device), renderBuffers[1], &lpD3Ddevice);
					if (!FAILED (hr))
						return (1);

					DEBUGLOG (0, "\nCreateDevice... device:%d, hr:%0x", device, hr);
					
					DestroyDirect3D7 ();
				}
				Destroy ();
			} else {
				return (1);
			}
		}
		lpDD->SetCooperativeLevel (hWnd, DDSCL_NORMAL);
		if (lpDDClipper != NULL)
			lpDDClipper->Release ();
		DestroyDirectDraw7 ();
		return (0);
	}
	return (1);
}


void		DX7General::RestoreWorkingMode ()
{
	if (lpD3Ddevice != NULL)
		lpD3Ddevice->Release ();
	
	if (lpD3D != NULL) {
		lpD3D->EvictManagedTextures ();
		lpD3D->Release ();
	}

	lpD3Ddevice = NULL;
	lpD3D = NULL;

	Destroy ();
	if (lpDDClipper != NULL)
		lpDDClipper->Release ();

	lpDDClipper = NULL;

	lpDD->RestoreDisplayMode ();
	lpDD->SetCooperativeLevel (hWnd, DDSCL_NORMAL);
	DestroyDirectDraw7 ();
}


int			DX7General::GetCurrentDisplayMode (unsigned short adapter, unsigned short /*device*/, DisplayMode* displayMode)
{
	int	success = 0;
	if (InitDX7 ()) {
		EnumDX7Adapters ();
		if (adapter < GetNumOfDX7Adapters ()) {
			if (CreateDirectDraw7OnAdapter (adapter)) {
				DX7DisplayMode	dx7DisplayMode;
				DDPIXELFORMAT	ddpf;
				GetCurrentDD7AdapterDisplayMode (&dx7DisplayMode, &ddpf);
				displayMode->xRes = dx7DisplayMode.xRes;
				displayMode->yRes = dx7DisplayMode.yRes;
				//displayMode->bitDepth = dx7DisplayMode.bitDepth;
				displayMode->refreshRate = dx7DisplayMode.refreshRate;
				ConvertDDPixelFormatToPixFmt (&ddpf, &displayMode->pixFmt, 0);
				DestroyDirectDraw7 ();
				success = 1;
			}
		}
		UninitDX7 ();
	}
	return (success);
}


int			DX7General::EnumDisplayModes (unsigned short adapter, unsigned short /*device*/, unsigned int byRefreshRates,
										  DISPLAYMODEENUMCALLBACK callbackFunc)
{
	int	success = 0;
	if (InitDX7 ()) {
		EnumDX7Adapters ();
		if (adapter < GetNumOfDX7Adapters ()) {
			if (CreateDirectDraw7OnAdapter (adapter)) {
				LPDIRECTDRAW7 lpDD = GetDirectDraw7 ();
				lpDD->EnumDisplayModes (byRefreshRates ? DDEDM_REFRESHRATES : 0, NULL, callbackFunc, DisplayModeEnumCallBack);
				DestroyDirectDraw7 ();
				success = 1;
			}
		}
		UninitDX7 ();
	}
	return (success);
}


/* Ez a függvény csak a naplózó verzióban szerepel: kiírja a meghajtó képességeit. */
void		DX7General::WriteOutRendererCaps ()
{
D3DDEVICEDESC7	devdesc;
DDCAPS			ddcaps, helcaps;

	ZeroMemory(&ddcaps, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);
	CopyMemory(&helcaps, &ddcaps, sizeof(ddcaps));
	lpDD->GetCaps(&ddcaps, &helcaps);
	DEBUGLOG(2,"\n\n------------------------- dgVoodoo: DirectDraw caps ----------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n\
----------------------- dgVoodoo: DirectDraw HEL caps --------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n",\
				ddcaps.dwCaps,\
				ddcaps.dwCaps2,\
				ddcaps.dwCKeyCaps,\
				ddcaps.dwFXCaps,\
				helcaps.dwCaps,\
				helcaps.dwCaps2,\
				helcaps.dwCKeyCaps,\
				helcaps.dwFXCaps);\

	lpD3Ddevice->GetCaps(&devdesc);
	DEBUGLOG(2,"\n------------------------- dgVoodoo: driver description -------------------------\n \
				dwDevCaps: %x\n\
			dpcLineCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
			dpcTriCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
				dwDeviceRenderBitDepth: %0x\n\
				dwDeviceZBufferBitDepth: %0x\n\
				dwMinTextureWidth: %0x\n\
				dwMinTextureHeight: %0x\n\
				dwMaxTextureWidth: %0x\n\
				dwMaxTextureHeight: %0x\n\
				dwMaxTextureRepeat: %0x\n\
				dwMaxTextureAspectRatio: %0x\n\
				dwMaxAnisotropy: %0x\n\
				dvGuardBandLeft: %f\n\
				dvGuardBandTop: %f\n\
				dvGuardBandRight: %f\n\
				dvGuardBandBottom: %f\n\
				dvExtentsAdjust: %f\n\
				dwStencilCaps: %0x\n\
				dwFVFCaps: %0x\n\
				dwTextureOpCaps: %0x\n\
				wMaxTextureBlendStages: %0x\n\
				wMaxSimultaneousTextures: %0x\n\
				dwMaxActiveLights: %0x\n\
				dvMaxVertexW: %f\n\
				wMaxUserClipPlanes: %0x\n\
				wMaxVertexBlendMatrices: %0x\n\
				dwVertexProcessingCaps: %0x\n", \
				devdesc.dwDevCaps, \
				devdesc.dpcLineCaps.dwMiscCaps, \
				devdesc.dpcLineCaps.dwRasterCaps, \
				devdesc.dpcLineCaps.dwZCmpCaps, \
				devdesc.dpcLineCaps.dwSrcBlendCaps, \
				devdesc.dpcLineCaps.dwDestBlendCaps, \
				devdesc.dpcLineCaps.dwAlphaCmpCaps, \
				devdesc.dpcLineCaps.dwShadeCaps, \
				devdesc.dpcLineCaps.dwTextureCaps, \
				devdesc.dpcLineCaps.dwTextureFilterCaps, \
				devdesc.dpcLineCaps.dwTextureBlendCaps, \
				devdesc.dpcLineCaps.dwTextureAddressCaps, \
				devdesc.dpcLineCaps.dwStippleWidth, \
				devdesc.dpcLineCaps.dwStippleHeight, \
				devdesc.dpcTriCaps.dwMiscCaps, \
				devdesc.dpcTriCaps.dwRasterCaps, \
				devdesc.dpcTriCaps.dwZCmpCaps, \
				devdesc.dpcTriCaps.dwSrcBlendCaps, \
				devdesc.dpcTriCaps.dwDestBlendCaps, \
				devdesc.dpcTriCaps.dwAlphaCmpCaps, \
				devdesc.dpcTriCaps.dwShadeCaps, \
				devdesc.dpcTriCaps.dwTextureCaps, \
				devdesc.dpcTriCaps.dwTextureFilterCaps, \
				devdesc.dpcTriCaps.dwTextureBlendCaps, \
				devdesc.dpcTriCaps.dwTextureAddressCaps, \
				devdesc.dpcTriCaps.dwStippleWidth, \
				devdesc.dpcTriCaps.dwStippleHeight, \
				devdesc.dwDeviceRenderBitDepth, \
				devdesc.dwDeviceZBufferBitDepth, \
				devdesc.dwMinTextureWidth, \
				devdesc.dwMinTextureHeight, \
				devdesc.dwMaxTextureWidth, \
				devdesc.dwMaxTextureHeight, \
				devdesc.dwMaxTextureRepeat, \
				devdesc.dwMaxTextureAspectRatio, \
				devdesc.dwMaxAnisotropy, \
				devdesc.dvGuardBandLeft, \
				devdesc.dvGuardBandTop, \
				devdesc.dvGuardBandRight, \
				devdesc.dvGuardBandBottom, \
				devdesc.dvExtentsAdjust, \
				devdesc.dwStencilCaps, \
				devdesc.dwFVFCaps, \
				devdesc.dwTextureOpCaps, \
				devdesc.wMaxTextureBlendStages, \
				devdesc.wMaxSimultaneousTextures, \
				devdesc.dwMaxActiveLights, \
				devdesc.dvMaxVertexW, \
				devdesc.wMaxUserClipPlanes, \
				devdesc.wMaxVertexBlendMatrices, \
				devdesc.dwVertexProcessingCaps);
}


int			DX7General::CreateFullScreenGammaControl (GammaRamp* originalGammaRamp)
{
	DDCAPS		ddcaps = {sizeof(DDCAPS)};
	DDGAMMARAMP ddGammaRamp;
	
	lpDD->GetCaps(&ddcaps, NULL);
	lpDDGamma = NULL;
	if (ddcaps.dwCaps2 & DDCAPS2_PRIMARYGAMMA)	{
		if (!lpDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl, (void **) &lpDDGamma))	{
			lpDDGamma->GetGammaRamp(0, &ddGammaRamp);
			CopyMemory (originalGammaRamp, &ddGammaRamp, sizeof (GammaRamp));
			return (1);
		}
	}
	return (0);
}


void		DX7General::DestroyFullScreenGammaControl ()
{
	if (lpDDGamma != NULL)
		lpDDGamma->Release ();
	lpDDGamma = NULL;
}


void		DX7General::SetGammaRamp (GammaRamp* gammaRamp)
{
	DDGAMMARAMP ddGammaRamp;
	CopyMemory (&ddGammaRamp, gammaRamp, sizeof (GammaRamp));

	lpDDGamma->SetGammaRamp(0, &ddGammaRamp);
}


RendererGeneral::DeviceStatus DX7General::GetDeviceStatus ()
{
	RendererGeneral::DeviceStatus retVal = (lpDD->TestCooperativeLevel() == DD_OK) ? StatusOk : StatusLost;
	if (retVal == StatusLost) {
		dx7Drawing->ResourcesMayLost ();
	}
	return retVal;
}


void		DX7General::PrepareForDeviceReset ()
{
}


int			DX7General::DeviceReset ()
{
	return (1);
}


int			DX7General::ReallocResources ()
{
	if (createFlags & SWM_WINDOWED) {
		for (int i = 0; i < 3; i++) {
			if (renderBuffers[i] != NULL) {
				if (!RestoreSurface (renderBuffers[i]))
					return (0);
			}
		}
	}
	if (!RestoreSurface (lpDDSPrimary))
		return (0);

	if (lpDDdepth != NULL) {
		if (!RestoreSurface (lpDDdepth))
			return (0);
	}
	if (dx7LfbDepth != NULL) {
		if (!dx7LfbDepth->RestoreResources ()) {
			return (0);
		}
	}

	if (dx7Drawing != NULL) {
		if (!dx7Drawing->RestoreResources ()) {
			return (0);
		}
	}
	return (1);
}


int			DX7General::BeginScene ()
{
	return (!FAILED (lpD3Ddevice->BeginScene ()));
}


void		DX7General::EndScene ()
{
	lpD3Ddevice->EndScene ();
}


void		DX7General::ShowContentChangesOnFrontBuffer ()
{
	lpD3Ddevice->EndScene();
	RefreshDisplayByBuffer (FrontBuffer);
	lpD3Ddevice->BeginScene();
}


int		DX7General::RefreshDisplayByBuffer (int buffer)
{
	if (createFlags & SWM_WINDOWED) {
		POINT	point = {0, 0};
		RECT	clientRect;
		ClientToScreen (hWnd, &point);
		GetClientRect (hWnd, &clientRect);
		OffsetRect (&clientRect, point.x, point.y);
		lpDDSPrimary->Blt (&clientRect, renderBuffers[buffer], NULL, DDBLT_WAIT, NULL);
	}
	return (1);
}


void		DX7General::SwitchToNextBuffer (unsigned int swapInterval, unsigned int renderBuffer)
{
	unsigned int i;

	if (swapInterval == 0) FlipBuffer (0);
	else {
		/* ha a tényleges frissítési freq-hoz igazítjuk magunkat */
		//if (!(config.Flags & CFG_SETREFRESHRATE))	{
			
			/* ha full screen-en futunk, akkor csak eggyel kevesebb visszafutást várunk meg */
			if (!(createFlags & SWM_WINDOWED)) swapInterval--;
			
			/* megvárjuk a visszafutásokat */
			for(i = 0; i < swapInterval; i++)	{
				int vbbool;
				while(1)	{
					lpDD->GetVerticalBlankStatus(&vbbool);
					if (!vbbool) break;
				}
				while(1)	{
					lpDD->GetVerticalBlankStatus(&vbbool);
					if (vbbool) break;
				}
			}
			//x = GetBufferNumPending();
			FlipBuffer (1);
			//if (x == GetBufferNumPending()) return;

		//} else {
		/* ha a timer-hez igazítjuk magunkat */
		//	movie.cmiFrames = 0;
		//	while((volatile) movie.cmiFrames < (swapinterval - 1));
		//}
	}

	if (createFlags & SWM_WINDOWED) {
		LPDIRECTDRAWSURFACE7 firstSurface = renderBuffers[0];

		for (i = 1; i < bufferNum; i++)
			renderBuffers[i-1] = renderBuffers[i];

		renderBuffers[i-1] = firstSurface;
	}

	if (lpD3Ddevice != NULL)
		lpD3Ddevice->SetRenderTarget(renderBuffers[renderTarget = renderBuffer], 0);
}


int			DX7General::WaitUntilBufferSwitchDone ()
{
	if (!(createFlags & SWM_WINDOWED)) {
		HRESULT hr;
		int buffer = renderTarget == 0 ? bufferNum - 1 : renderTarget;

		while ((hr = renderBuffers[buffer]->GetFlipStatus (DDGFS_ISFLIPDONE)) == DDERR_WASSTILLDRAWING)
			Sleep (1);

		if (FAILED (hr))
			return (0);
	}
	return (1);
}


int			DX7General::GetNumberOfPendingBufferFlippings ()
{
	HRESULT hr;
	int		flipNum = 0;
	
	for (unsigned int i = 1; i < bufferNum; i++) {
		if (createFlags & SWM_WINDOWED)
			hr = renderBuffers[i]->GetBltStatus (DDGBS_ISBLTDONE);
		else
			hr = renderBuffers[i]->GetFlipStatus (DDGFS_ISFLIPDONE);

		if (hr == DDERR_WASSTILLDRAWING)
			flipNum++;
	}
	return flipNum;
}


int			DX7General::GetVerticalBlankStatus ()
{
	int	vbool;
	
	lpDD->GetVerticalBlankStatus(&vbool);
	return vbool;
}


unsigned int DX7General::GetCurrentScanLine ()
{
	DWORD	line;
	
	getScanLineCount++;

	if (lpDD->GetScanLine(&line) == DD_OK)
		return(line);
	else
		return (getScanLineCount % appYRes);
}


int			DX7General::SetRenderTarget (int buffer)
{
	lpD3Ddevice->SetRenderTarget(renderBuffers[renderTarget = buffer], 0);
	return (1);
}


void		DX7General::ClearBuffer (unsigned int flags, RECT* rect, unsigned int color, float depth)
{
	DWORD clearFlags = ((flags & CLEARFLAG_COLORBUFFER) ? D3DCLEAR_TARGET : 0) |
					   ((flags & CLEARFLAG_DEPTHBUFFER) ? D3DCLEAR_ZBUFFER : 0);

	if (stencilBitDepth) clearFlags |= D3DCLEAR_STENCIL;

	if (rect != NULL) {
		D3DRECT d3dRect;

		d3dRect.x1 = rect->left;
		d3dRect.x2 = rect->right;
		d3dRect.y1 = rect->top;
		d3dRect.y2 = rect->bottom;
		lpD3Ddevice->Clear(1, &d3dRect, clearFlags, color, depth, 0);
	} else {
		lpD3Ddevice->Clear(0, NULL, clearFlags, color, depth, 0);
	}
}


void		DX7General::SetClipWindow (RECT* clipRect)
{
D3DVIEWPORT7		viewport;

	viewport.dwX = (DWORD) clipRect->left;
	viewport.dwWidth = (DWORD) (clipRect->right - clipRect->left);
	viewport.dwY = (DWORD) clipRect->top;
	viewport.dwHeight = (DWORD) (clipRect->bottom - clipRect->top);
	viewport.dvMinZ = 0.0f;
	viewport.dvMaxZ = 1.0f;
	lpD3Ddevice->SetViewport(&viewport);
}


void		DX7General::SetCullMode (GrCullMode_t mode)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CULLMODE, dxCullModeLookUp[mode]);
}


void		DX7General::SetColorAndAlphaWriteMask (FxBool rgb, FxBool /*alpha*/)
{
	if (rgb) {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlendEnabled);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlendFunc);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, dstBlendFunc);
	} else {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	}
}


void		DX7General::EnableAlphaBlending (int enable)
{
	alphaBlendEnabled = enable ? TRUE : FALSE;
	if (astate.colorMask) {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, enable ? TRUE : FALSE);
	}
}


void		DX7General::SetRgbAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc)
{
	srcBlendFunc = alphaModeLookupSrc[srcFunc];
	dstBlendFunc = alphaModeLookupDst[dstFunc];
	if (astate.colorMask) {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlendFunc);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, dstBlendFunc);
	}
}


void		DX7General::SetAlphaAlphaBlendingFunction (GrAlphaBlendFnc_t /*srcFunc*/, GrAlphaBlendFnc_t /*dstFunc*/)
{
}


void		DX7General::SetDestAlphaBlendFactorDirectly (DestAlphaBlendFactor destAlphaBlendFactor)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destAlphaBlendFactor == DestAlphaFactorZero ? D3DBLEND_ZERO : D3DBLEND_ONE);
}


void		DX7General::EnableAlphaTesting (int enable)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, enable ? TRUE : FALSE);
}


void		DX7General::SetAlphaTestFunction (GrCmpFnc_t function)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, cmpFuncLookUp[function]);
}


void		DX7General::SetAlphaTestRefValue (GrAlpha_t value)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, value);
}


void		DX7General::SetFogColor (unsigned int fogColor)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, fogColor);
}


void		DX7General::EnableFog (int enable)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, enable ? TRUE : FALSE);
}


void		DX7General::SetDitheringMode (GrDitherMode_t mode)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, ((mode == GR_DITHER_2x2) || (mode == GR_DITHER_4x4)) ? TRUE : FALSE);
}


void		DX7General::SetConstantColor (unsigned int constantColor)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, constantColor);
}


void		DX7General::ConfigureTexelPixelPipeLine (GrCombineFunction_t /*rgb_function*/, GrCombineFactor_t /*rgb_factor*/,
													 GrCombineFunction_t /*alpha_function*/, GrCombineFactor_t /*alpha_factor*/,
													 FxBool /*rgb_invert*/, FxBool /*alpha_invert*/)
{
}


void		DX7General::ConfigureColorPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
CPixelShader3Stages *setting;
unsigned int		i,cfuncpindex;

	DWORD d3dlocal = combinelocallookup[local];
	DWORD d3dother = combineotherlookup[other];
	DWORD d3dfactor = 0;
	if ( (d3dother == D3DTA_TEXTURE) && (astate.flags & STATE_TCLOCALALPHA) ) d3dother |= D3DTA_ALPHAREPLICATE;

	setting = pixelShadersfa;
	switch(factor)	{		
		case GR_COMBINE_FACTOR_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_RGB:
			setting = pixelShadersfloc;
			d3dfactor = D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
			break;
		
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = alphaOther;
			break;

		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
			d3dfactor = alphaLocal;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
			
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
			d3dfactor = alphaLocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = alphaOther | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ZERO:
			setting = pixelShadersf0;
			break;

		case GR_COMBINE_FACTOR_ONE:
			setting = pixelShadersf1;
			break;

		default:							// ha nem valamelyik konstans
			setting = pixelShadersf0;
	}
	
	cfuncpindex = grfunctranslating[func];

	// Argumentumok és opkód kiolvasása a shaderbõl
	this->cstagesnum = 1;
	i = astate.flags & STATE_TCCINVERT;
	colorOp[0] = setting[cfuncpindex].colorop_0;
	colorArg1[0] = GlideGetStageArg(setting[cfuncpindex].colorarg1_0, d3dfactor, d3dlocal, d3dother, i);
	colorArg2[0] = GlideGetStageArg(setting[cfuncpindex].colorarg2_0, d3dfactor, d3dlocal, d3dother, i);
	if ( (colorOp[1] = setting[cfuncpindex].colorop_1) != D3DTOP_DISABLE )	{
		this->cstagesnum = 2;
		colorArg1[1] = GlideGetStageArg(setting[cfuncpindex].colorarg1_1, d3dfactor, d3dlocal, d3dother, i);
		colorArg2[1] = GlideGetStageArg(setting[cfuncpindex].colorarg2_1, d3dfactor, d3dlocal, d3dother, i);
		if ( (colorOp[2] = setting[cfuncpindex].colorop_2) != D3DTOP_DISABLE )	{
			this->cstagesnum = 3;
			colorArg1[2] = GlideGetStageArg(setting[cfuncpindex].colorarg1_2, d3dfactor, d3dlocal, d3dother, i);
			colorArg2[2] = GlideGetStageArg(setting[cfuncpindex].colorarg2_2, d3dfactor, d3dlocal, d3dother, i);
		}
	}
		
	// A speciális opkódok feldolgozása (implicit argumentumokkal)
	// Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ
	switch(colorOp[1])	{		
		case PS_BLENDFACTORALPHA:
			colorOp[1] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = colorArg1[1];
				colorArg1[1] = colorArg2[1];
				colorArg2[1] = i;
			}
			break;
	}	

	//colorOp[0]=D3DTOP_SELECTARG1;
	//colorArg1[0]=D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
	//colorOp[1]=D3DTOP_DISABLE;
	
	if (invert)	{
		colorOp[this->cstagesnum] = D3DTOP_SELECTARG2;
		colorArg1[this->cstagesnum] = D3DTA_CURRENT;
		colorArg2[this->cstagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		this->cstagesnum++;
	}
}


void		DX7General::ConfigureAlphaPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
APixelShader2Stages		*setting;
unsigned int			i, tcInvert, afuncpindex;

	DWORD d3dlocal = combinelocallookup[local];
	DWORD d3dother = combineotherlookup[other];
	DWORD d3dfactor = 0;
	
	switch(factor)	{
		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_LOCAL:
			d3dfactor = d3dlocal;
			break;
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = d3dother;
			break;
		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = d3dother | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;
	}
	setting = alphaPixelShaders;
	afuncpindex = grfunctranslating[func];
	
	if (factor == GR_COMBINE_FACTOR_ZERO) setting = alphaPixelShadersf0;
	if (factor == GR_COMBINE_FACTOR_ONE) setting = alphaPixelShadersf1;
	
	i = 0;
	if (astate.flags & STATE_APMULTITEXTURING)	{
		if ( (d3dother == D3DTA_TEXTURE) &&
			( (setting[func].alphaarg1_0 == PS_OTHER) || (setting[func].alphaarg2_0 == PS_OTHER) ) )	{
			alphaOp[0] = D3DTOP_SELECTARG1;
			alphaArg1[0] = D3DTA_DIFFUSE;
			alphaArg2[0] = D3DTA_DIFFUSE;
			i++;
		}
	}

	tcInvert = astate.flags & STATE_TCAINVERT;
	this->astagesnum = i + 1;
	alphaOp[i] = setting[afuncpindex].alphaop_0;
	alphaArg1[i] = GlideGetStageArg(setting[afuncpindex].alphaarg1_0, d3dfactor, d3dlocal, d3dother, tcInvert);
	alphaArg2[i] = GlideGetStageArg(setting[afuncpindex].alphaarg2_0, d3dfactor, d3dlocal, d3dother, tcInvert);
	if ( (alphaOp[i+1] = setting[afuncpindex].alphaop_1) != D3DTOP_DISABLE )	{
		this->astagesnum = i+2;
		alphaArg1[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg1_1, d3dfactor, d3dlocal, d3dother, tcInvert);
		alphaArg2[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg2_1, d3dfactor, d3dlocal, d3dother, tcInvert);
	}

	// A speciális opkódok feldolgozása (implicit argumentumokkal)
	// Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ
	switch(alphaOp[i])	{
		case PS_BLENDFACTORALPHA:
			alphaOp[i] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = alphaArg1[i];
				alphaArg1[i] = alphaArg2[i];
				alphaArg2[i] = i;
			}
			break;
	}		
	if (invert)	{
		alphaOp[this->astagesnum] = D3DTOP_SELECTARG2;
		alphaArg2[this->astagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		this->astagesnum++;
	}
	/*if (astate.flags & STATE_COLORKEYTNT)	{
		alphaOp[0] = D3DTOP_SELECTARG1;
		alphaArg1[0] = D3DTA_TEXTURE;
		this->astagesnum = 1;
	}*/
	
	alphaLocal = d3dlocal;
	alphaOther = d3dother;
}


void		DX7General::ComposeConcretePixelPipeLine ()
{
	unsigned int i = 0;

	/* Alpha stage-ek beállítása */
	if (!nativeTntCkMethodUsed) {
		for(; i < this->astagesnum; i++)	{
			if (this->cachedAlphaOp[i] != alphaOp[i])
				lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = alphaOp[i]);
			if (this->cachedAlphaArg1[i] != alphaArg1[i])
				lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = alphaArg1[i]);
			if (this->cachedAlphaArg2[i] != alphaArg2[i])
				lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG2, this->cachedAlphaArg2[i] = alphaArg2[i]);
		}
	} else {
		if (this->cachedAlphaOp[0] != D3DTOP_SELECTARG1)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[0] = D3DTOP_SELECTARG1);
		if (this->cachedAlphaArg1[0] != D3DTA_TEXTURE)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[0] = D3DTA_TEXTURE);
		i = 1;
	}

	for (;i < MAX_TEXSTAGES; i++)	{
		if (this->cachedAlphaOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = D3DTOP_SELECTARG1);
		if (this->cachedAlphaArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = D3DTA_CURRENT);
	}

	/* Color stage-ek beállítása */
	for(i = 0; i < this->cstagesnum; i++)	{
		if (this->cachedColorOp[i] != colorOp[i])
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = colorOp[i]);
		if (this->cachedColorArg1[i] != colorArg1[i])
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLORARG1, this->cachedColorArg1[i] = colorArg1[i]);
		if (this->cachedColorArg2[i] != colorArg2[i])
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLORARG2, this->cachedColorArg2[i] = colorArg2[i]);
	}

	for(i = this->cstagesnum; i < this->astagesnum; i++)	{
		if (this->cachedColorOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_SELECTARG1);
		if (this->cachedColorArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLORARG1, this->cachedColorArg1[i] = D3DTA_CURRENT);
	}

	if (alphaCkAlphaOp[0] != D3DTOP_DISABLE) {
		DWORD cArg2 = D3DTA_CURRENT;
		DWORD aArg2 = D3DTA_CURRENT;

		int onlyInvertColorOutput = 0;
		int onlyInvertAlphaOutput = 0;
		//Ha az utolsó color- és alpha-stage-k csak a kimenetet invertálják, vagy csak "kitöltõ" stage-ek,
		//akkor ezt a stage-t fel lehet használni a mûvelethez, egyébként új stage
		if ( (this->astagesnum < i) || (onlyInvertColorOutput = ((this->astagesnum == i) && (astate.ainvertp))) )	{
			if ( (this->cstagesnum < i) || (onlyInvertAlphaOutput = ((this->cstagesnum == i) && (astate.cinvertp))) )	{
				if (onlyInvertColorOutput) cArg2 = D3DTA_CURRENT | D3DTA_COMPLEMENT;
				if (onlyInvertAlphaOutput) aArg2 = D3DTA_CURRENT | D3DTA_COMPLEMENT;
				i--;
			}
		}

		if (this->cachedAlphaOp[i] != alphaCkAlphaOp[0])
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = alphaCkAlphaOp[0]);
		if (this->cachedAlphaArg1[i] != alphaCkAlphaArg1[0])
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = alphaCkAlphaArg1[0]);
		if (this->cachedAlphaArg2[i] != aArg2)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_ALPHAARG2, this->cachedAlphaArg2[i] = aArg2);

		if (this->cachedColorOp[i] != D3DTOP_SELECTARG2)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_SELECTARG2);
		if (this->cachedColorArg2[i] != cArg2)
			lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLORARG2, this->cachedColorArg2[i] = cArg2);

		i++;
	}
	maxUsedStageNum = i;

	if (i != MAX_TEXSTAGES)
		if (this->cachedColorOp[i] != D3DTOP_DISABLE) lpD3Ddevice->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_DISABLE);
	
	DWORD	rpersp;
	if ( ( (astate.flags & STATE_COLORCOMBINEUSETEXTURE) || (astate.flags & STATE_ALPHACOMBINEUSETEXTURE) ) ||
		( (!(astate.vertexmode & VMODE_WBUFFEMU))) ) rpersp = TRUE;
		else rpersp = FALSE;
	if (rpersp != astate.perspective)
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, astate.perspective = rpersp);

}


void		DX7General::GetCkMethodPreferenceOrder (ColorKeyMethod*	ckMethodPreference)
{
	ckMethodPreference[0] = RendererGeneral::AlphaBased;
	ckMethodPreference[1] = (config.Flags & CFG_CKMFORCETNTINAUTOMODE) ? RendererGeneral::NativeTNT : RendererGeneral::Native;
	ckMethodPreference[2] = RendererGeneral::Disabled;
}


int			DX7General::IsAlphaTestUsedForAlphaBasedCk ()
{
	return (1);
}


int			DX7General::EnableAlphaColorkeying (AlphaCkModeInPipeline alphaCkModeInPipeline, int alphaOutputUsed)
{
	nativeTntCkMethodUsed = 0;

	DWORD op = D3DTOP_DISABLE;
	if (alphaCkModeInPipeline != AlphaDisabled) {

		//Az AlphaTest milyen függvényt használ? < AlphaRef vagy > AlphaRef ?
		DWORD arg = D3DTA_TEXTURE;
		op = D3DTOP_MODULATE;
		if (alphaCkModeInPipeline == AlphaForFncLess) {
			op = D3DTOP_ADD;
			arg = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
		}
		
		//Ha az alpha-kimenet nincs felhasználva, akkor használhatjuk közvetlenül az alpha-textúrát
		if (!alphaOutputUsed)
			op = D3DTOP_SELECTARG1;

		alphaCkAlphaArg1[0] = arg;
	}
	alphaCkAlphaOp[0] = op;
	return (1);
}


int			DX7General::EnableNativeColorKeying (int enable, int forTnt)
{
	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, enable ? TRUE : FALSE);
	nativeTntCkMethodUsed = enable && forTnt;
	return forTnt;
}


int			DX7General::EnableAlphaItRGBLighting (int /*enable*/)
{
	return (0);
}


int			DX7General::GetTextureUsageFlags ()
{
	int	textureUsage = STATE_TEXTURE0;
	int flags = 0;
	
	for(unsigned int i = 0; i < maxUsedStageNum; i++)	{
		if ( ((this->cachedAlphaArg1[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) || ((this->cachedAlphaArg2[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) ||
			 ((this->cachedColorArg1[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) || ((this->cachedColorArg2[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) )
			 flags |= textureUsage;
		textureUsage <<= 1;
	}

	return (flags);
}


int			DX7General::GetNumberOfTextureStages ()
{
	D3DDEVICEDESC7		devDesc;

	lpD3Ddevice->GetCaps(&devDesc);
	return (devDesc.wMaxTextureBlendStages);
}


void		DX7General::SaveRenderState (RenderStateSaveType renderStateSaveType)
{
	DWORD*	savedRenderStates = NULL;
	DWORD*	savedTextureStageStates = NULL;
	
	switch (renderStateSaveType) {
		
		case ScreenShotLabel:
			savedRenderStates = scrShotSaveStates;
			savedTextureStageStates = scrShotSaveTSStates;
			break;

		case LfbNoPipeline:
			savedRenderStates = lfbSaveStatesNoPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			break;

		case LfbPipeline:
			savedRenderStates = lfbSaveStatesPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			break;

		case GammaCorrection:
			savedRenderStates = gammaSaveStates;
			savedTextureStageStates = gammaSaveTSStates;
			break;
	}

	int i, pos;

	for(i = 0; savedRenderStates[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->GetRenderState ((D3DRENDERSTATETYPE) savedRenderStates[i], savedState+i);

	pos = i;
	for(i = 0; savedTextureStageStates[i] != STATEDESC_DELIMITER; i+=2)
		lpD3Ddevice->GetTextureStageState (savedTextureStageStates[i], (D3DTEXTURESTAGESTATETYPE) savedTextureStageStates[i+1],
										   savedState+(pos++));

	//lpD3Ddevice->GetTexture(0, (LPDIRECTDRAWSURFACE7 *) savedState + pos);		
}


void		DX7General::RestoreRenderState (RenderStateSaveType renderStateSaveType)
{
	DWORD*	savedRenderStates;
	DWORD*	savedTextureStageStates;
	
	switch (renderStateSaveType) {
		
		case ScreenShotLabel:
			savedRenderStates = scrShotSaveStates;
			savedTextureStageStates = scrShotSaveTSStates;
			break;

		case LfbNoPipeline:
			savedRenderStates = lfbSaveStatesNoPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			break;

		case LfbPipeline:
			savedRenderStates = lfbSaveStatesPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			break;

		case GammaCorrection:
			savedRenderStates = gammaSaveStates;
			savedTextureStageStates = gammaSaveTSStates;
			break;
	}

	int i, pos;

	for(i = 0; savedRenderStates[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->SetRenderState ((D3DRENDERSTATETYPE) savedRenderStates[i], savedState[i]);

	pos = i;
	for(i = 0; savedTextureStageStates[i] != STATEDESC_DELIMITER; i+=2)
		lpD3Ddevice->SetTextureStageState (savedTextureStageStates[i], (D3DTEXTURESTAGESTATETYPE) savedTextureStageStates[i+1],
										   savedState[pos++]);

	lpD3Ddevice->SetTexture(0, (LPDIRECTDRAWSURFACE7) astate.lpDDTex[0]/*savedState[pos]*/);
}


void		DX7General::SetRenderState (RenderStateSaveType renderStateSaveType, int lfbEnableNativeCk, int lfbUseConstAlpha,
										unsigned int lfbConstAlpha)
{
	switch (renderStateSaveType) {
		case ScreenShotLabel:
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);	
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

			lpD3Ddevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	

			lpD3Ddevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			break;

		case LfbNoPipeline:
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			break;

		case LfbPipeline:
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
			break;

		case GammaCorrection:
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_DESTCOLOR);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

			lpD3Ddevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			break;
	}

	if ((renderStateSaveType == LfbNoPipeline) || (renderStateSaveType == LfbPipeline)) {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE);
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, lfbEnableNativeCk ? TRUE : FALSE);
		lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		lpD3Ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		lpD3Ddevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		lpD3Ddevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		if (!lfbUseConstAlpha) {
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		} else {
			lpD3Ddevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			lpD3Ddevice->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, lfbConstAlpha << 24);
		}
	}
}


int			DX7General::UsesPixelShaders ()
{
	return (0);
}


int			DX7General::Is3DNeeded ()
{
	return (0);
}