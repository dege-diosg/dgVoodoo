/*--------------------------------------------------------------------------------- */
/* DX7Texturing.cpp - dgVoodoo 3D rendering interface for texturing,                */
/*                    DX7 implementation                                            */
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
#include	"Main.h"

}

#include	"DX7Texturing.hpp"


struct DX7Texturing::NamedFormatData DX7Texturing::namedFormatData[NumberOfDXTexFormats];

/*------------------------------------------------------------------------------------------*/
/*........................... DX7 implementáció a textúrázáshoz ............................*/

HRESULT	CALLBACK DX7Texturing::TexEnumFormatsCallback(LPDDPIXELFORMAT lpDDpf, void* namedFormats)
{
_pixfmt			enumPixFmt;
NamedFormat		*destNamedFormat;
int				i;

	DEBUG_BEGINSCOPE("TexEnumFormatsCallback", DebugRenderThreadId);
	
	if (lpDDpf->dwFourCC != 0) return(D3DENUMRET_OK);
	
	i = Pf_Invalid;
	
	/* Ha a kártya esetleg támogatja a 8 bites palettás textúrákat */
	if ( (lpDDpf->dwRGBBitCount == 8) && (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) && 
		(lpDDpf->dwFlags & DDPF_PALETTEINDEXED8))	{
		i = Pf8_P8;
		ZeroMemory(&enumPixFmt, sizeof(_pixfmt));
		enumPixFmt.BitPerPixel = 8;

	} else {

		if ( ((lpDDpf->dwFlags & DDPF_BUMPDUDV) == DDPF_BUMPDUDV) ||
			((lpDDpf->dwFlags & (DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) == (DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) ) {
			_asm nop
			return(D3DENUMRET_OK);
		}

		/* Egyébként megvizsgáljuk a 16 és 32 bites formátumokat */
		if ( (lpDDpf->dwRGBBitCount != 16) && (lpDDpf->dwRGBBitCount != 32) ) return(D3DENUMRET_OK);
		ConvertDDPixelFormatToPixFmt (lpDDpf, &enumPixFmt, 0);

		switch(lpDDpf->dwRGBBitCount)	{
			case 32:
				if ( (enumPixFmt.RBitCount != 8) || (enumPixFmt.GBitCount != 8) || (enumPixFmt.BBitCount != 8) ) return(D3DENUMRET_OK);
				if (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS ))	{
					i = Pf32_RGB888;
				} else {
					if (enumPixFmt.ABitCount == 8) i = Pf32_ARGB8888;
				}
				break;
			case 16:
				/* Ellenôrzés az ARGB 4444 formátumhoz */
				if ( (enumPixFmt.RBitCount == 4) && (enumPixFmt.GBitCount == 4) && (enumPixFmt.BBitCount == 4) && (enumPixFmt.ABitCount == 4) &&
					 (lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) {
					i = Pf16_ARGB4444;
				}
				/* Ellenôrzés a ARGB 1555 formátumhoz */
				if ( (enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5) && (enumPixFmt.ABitCount == 1) &&
				     (lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) {
					i = Pf16_ARGB1555;
				}
				/* Ellenôrzés a RGB 555 formátumhoz */
				if ( (enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5) &&
					 (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS))) {
					i = Pf16_RGB555;
				}
				/* Ellenôrzés a RGB 565 formátumhoz */
				if ( ((enumPixFmt.RBitCount == 6) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5)) ||
					 ((enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 6) && (enumPixFmt.BBitCount == 5)) ||
					 ((enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 6)) &&
					 (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS))) {

					i = Pf16_RGB565;
				}
				break;
			default:
				break;
		}
	}
	
	if (i == Pf_Invalid) return(D3DENUMRET_OK);

	destNamedFormat = ((NamedFormat *) namedFormats) + i;
	
	CopyMemory(&namedFormatData[i].ddsdPixelFormat, lpDDpf, sizeof(DDPIXELFORMAT));
	CopyMemory(&destNamedFormat->pixelFormat, &enumPixFmt, sizeof(_pixfmt));
	destNamedFormat->isValid = 1;

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(D3DENUMRET_OK);
}


void		DX7Texturing::Init (int useManagedTextures)
{
	managedTextures = useManagedTextures;
}


void		DX7Texturing::DeInit ()
{
}


void		DX7Texturing::GetSupportedTextureFormats (NamedFormat* namedFormats)
{
	lpD3Ddevice->EnumTextureFormats(&TexEnumFormatsCallback, (void*) namedFormats);
}


int			DX7Texturing::AreNonPow2TexturesSupported ()
{
	return (0);
}


int			DX7Texturing::CreateDXTexture (enum DXTextureFormats format, unsigned int x, unsigned int y, int mipMapNum,
										   TextureType* mipMaps, int /*hwGeneratedTexture*/)
{
DDSURFACEDESC2			ddTexDesc;
LPDIRECTDRAWSURFACE7	lpD3DTexture;
LPDIRECTDRAWSURFACE7	lpD3DNext;
LPDIRECTDRAWSURFACE7	textureMipMaps[9];
int						i;
HRESULT					hr;

	if (format > NumberOfDXTexFormats)
		return (0);

	ZeroMemory (&ddTexDesc, sizeof(DDSURFACEDESC2));
	ddTexDesc.dwSize = sizeof(DDSURFACEDESC2);

	ddTexDesc.dwWidth = x;
	ddTexDesc.dwHeight = y;
	ddTexDesc.ddpfPixelFormat = namedFormatData[format].ddsdPixelFormat;

	ddTexDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;// | (managedTextures ? DDSCAPS_VIDEOMEMORY;
	ddTexDesc.ddsCaps.dwCaps2 = managedTextures ? DDSCAPS2_TEXTUREMANAGE : 0;
	ddTexDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS | DDSD_CKSRCBLT;

	if ( (ddTexDesc.dwMipMapCount = mipMapNum) != 1 )	{
		ddTexDesc.dwFlags |= DDSD_MIPMAPCOUNT;
		ddTexDesc.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	}

	lpD3DTexture = NULL;
	hr = lpDD->CreateSurface(&ddTexDesc, &lpD3DTexture, NULL);

	if ((hr != DD_OK) || (lpD3DTexture == NULL)) {
		DEBUGLOG(0,"\n   Warning:  TexCreateDX7Texture: creating dd alpha texture surface has failed");
		DEBUGLOG(1,"\n   Figyelmeztetés: TexCreateDX7Texture: a dd alfa-textúra létrehozása nem sikerült");
		LOG(1,"    TexCreateDX7Texture failed");
		return(0);
	}
	
	ddTexDesc.ddsCaps.dwCaps2 = 0;
	ddTexDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
	textureMipMaps[0] = lpD3DTexture;
	
	for(i = 1; i < mipMapNum; i++)	{
		hr = lpD3DTexture->GetAttachedSurface(&ddTexDesc.ddsCaps, &lpD3DNext);
		if ((hr != DD_OK) || (lpD3DNext == NULL)) {
			DEBUGLOG(0,"\n   Warning:  TexCreateDX7Texture: obtaining mipmap surface has failed");
			DEBUGLOG(1,"\n   Figyelmeztetés: TexCreateDX7Texture: a mipmap-objektum lekérdezése nem sikerült");

			lpD3DTexture->Release ();
			return (0);
		} 
		textureMipMaps[i] = lpD3DTexture = lpD3DNext;
	}

	CopyMemory ((LPDIRECTDRAWSURFACE7*) mipMaps, textureMipMaps, mipMapNum * sizeof(LPDIRECTDRAWSURFACE7));
	return (1);
}


void		DX7Texturing::DestroyDXTexture (TextureType texture)
{
	((LPDIRECTDRAWSURFACE7) texture)->Release ();
}


void*		DX7Texturing::GetPtrToTexture (TextureType texture, unsigned int /*mipMapLevel*/, unsigned int *pitch)
{
	DDSURFACEDESC2	ddTexDesc = {sizeof(DDSURFACEDESC2)};

	if (((LPDIRECTDRAWSURFACE7) texture)->Lock(NULL, &ddTexDesc, DDLOCK_WAIT, NULL) == DD_OK) {
		*pitch = ddTexDesc.lPitch;
		return ddTexDesc.lpSurface;
	}
	return NULL;
}


void		DX7Texturing::ReleasePtrToTexture (TextureType texture, unsigned int /*mipMapLevel*/)
{
	((LPDIRECTDRAWSURFACE7) texture)->Unlock(NULL);
}


void		DX7Texturing::SetSrcColorKeyForTexture (TextureType texture, unsigned int colorKey)
{
	DDCOLORKEY	colorkey;

	colorkey.dwColorSpaceLowValue = colorkey.dwColorSpaceHighValue = colorKey;
	((LPDIRECTDRAWSURFACE7) texture)->SetColorKey(DDCKEY_SRCBLT, &colorkey);
}


void		DX7Texturing::SetSrcColorKeyForTextureFormat (DXTextureFormats /*dxTexFormatIndex*/, unsigned int /*colorKey*/)
{
}


unsigned int DX7Texturing::GetSrcColorKeyForTextureFormat (DXTextureFormats /*dxTexFormatIndex*/)
{
	return (0);
}


TextureType	DX7Texturing::GetAuxTextureForNativeCk (DXTextureFormats /*dxTexFormatIndex*/)
{
	return (NULL);
}


int			DX7Texturing::FillTexture (TextureType texture, unsigned int /*colorKeyARGB*/, unsigned int colorKey, _pixfmt* /*textureFormat*/)
{
	DDBLTFX	ddBltFx;

	ZeroMemory(&ddBltFx, sizeof(DDBLTFX));
	ddBltFx.dwSize = sizeof(DDBLTFX);
	ddBltFx.dwFillColor = colorKey;
	if (((LPDIRECTDRAWSURFACE7) texture)->Blt (NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx) != DD_OK)
		return (0);

	return (1);
}


int			DX7Texturing::CanRestoreTextures ()
{
	return (1);
}


int			DX7Texturing::CanPreserveTextures ()
{
	return managedTextures;
}


int			DX7Texturing::RestoreTexturePhysicallyIfNeeded (TextureType texture)
{
	if (texture != NULL) {
		
		if (((LPDIRECTDRAWSURFACE7) texture)->IsLost() == DDERR_SURFACELOST)
			if (((LPDIRECTDRAWSURFACE7) texture)->Restore() == DD_OK) return(1);
	
	}
	return(0);
}


void		DX7Texturing::SetTextureAtStage (int stageNum, TextureType texture)
{
	lpD3Ddevice->SetTexture(stageNum, (LPDIRECTDRAWSURFACE7) texture);
}


void		DX7Texturing::SetTextureStageMinFilter (int stageNum, GrTextureFilterMode_t mode)
{
	lpD3Ddevice->SetTextureStageState (stageNum, D3DTSS_MINFILTER,
									   (mode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTFN_POINT : D3DTFN_LINEAR);
}


void		DX7Texturing::SetTextureStageMagFilter (int stageNum, GrTextureFilterMode_t mode)
{
	lpD3Ddevice->SetTextureStageState (stageNum, D3DTSS_MAGFILTER,
									   (mode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTFN_POINT : D3DTFN_LINEAR);
}


void		DX7Texturing::SetTextureStageClampModeU (int stageNum, GrTextureClampMode_t clampMode)
{
	lpD3Ddevice->SetTextureStageState(stageNum, D3DTSS_ADDRESSU,
									  (clampMode == GR_TEXTURECLAMP_WRAP) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
}


void		DX7Texturing::SetTextureStageClampModeV (int stageNum, GrTextureClampMode_t clampMode)
{
	lpD3Ddevice->SetTextureStageState(stageNum, D3DTSS_ADDRESSV,
									  (clampMode == GR_TEXTURECLAMP_WRAP) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
}


void		DX7Texturing::SetTextureStageLodBias (int stageNum, float bias)
{
	lpD3Ddevice->SetTextureStageState(stageNum, D3DTSS_MIPMAPLODBIAS, *( (LPDWORD) (&bias)) );
}


void		DX7Texturing::SetTextureStageMipMapMode (int stageNum, GrMipMapMode_t mipMapMode, FxBool lodBlend)
{
DWORD val;

	switch(mipMapMode)	{
		case GR_MIPMAP_NEAREST:
			val = (lodBlend == FXTRUE) ? D3DTFP_LINEAR : D3DTFP_POINT;
			break;
		case GR_MIPMAP_NEAREST_DITHER:
			val = D3DTFP_LINEAR;
			break;
		default:
			val = D3DTFP_NONE;
	}
	lpD3Ddevice->SetTextureStageState(stageNum, D3DTSS_MIPFILTER, val);
}



PaletteType	DX7Texturing::CreatePalette (PALETTEENTRY*	initialPalEntries)
{
	LPDIRECTDRAWPALETTE lpDDPal;

	lpDDPal = NULL;
	if (lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, initialPalEntries, &lpDDPal, NULL) == DD_OK)
		return (PaletteType) lpDDPal;
	else
		return NULL;
}


void		DX7Texturing::DestroyPalette (PaletteType palette)
{
	((LPDIRECTDRAWPALETTE) palette)->Release ();
}


int			DX7Texturing::AssignPaletteToTexture (TextureType texture, PaletteType newPalette)
{
	if (texture != NULL) {
		
		if (((LPDIRECTDRAWSURFACE7) texture)->SetPalette((LPDIRECTDRAWPALETTE) newPalette) == DD_OK)
			return (1);
	
	}

	DEBUGLOG (0, "\n	Error: TexAssignPaletteToTexture: Assigning palette to texture has failed");
	DEBUGLOG (1, "\n	Hiba: TexAssignPaletteToTexture: A palettát nem sikerült a textúrához rendelni");
	return (0);
}


void		DX7Texturing::SetPaletteEntries (TextureType palette, PALETTEENTRY* palEntries)
{
	((LPDIRECTDRAWPALETTE) palette)->SetEntries(0, 0, 256, palEntries);
}


void		DX7Texturing::GeneratePalettedTexture (TextureType* /*dstTexture*/, TextureType* /*srcTexture*/,
												   unsigned int /*width*/, unsigned int /*height*/,
												   unsigned int /*mipMapLevel*/, int /*mipMapNum*/, unsigned int* /*palette*/)
{
}
