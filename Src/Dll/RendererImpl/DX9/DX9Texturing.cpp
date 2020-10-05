/*--------------------------------------------------------------------------------- */
/* DX9Texturing.cpp - dgVoodoo 3D rendering interface for texturing,                */
/*                    DX9 implementation                                            */
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

#include	"DX9Texturing.hpp"
#include	"DX9General.hpp"
#include	"DX9Drawing.hpp"


struct DX9Texturing::NamedFormatData DX9Texturing::namedFormatData[NumberOfDXTexFormats];

/*------------------------------------------------------------------------------------------*/
/*........................... DX9 implementáció a textúrázáshoz ............................*/

void		DX9Texturing::Init (int useManagedTextures)
{
	dx9Texturing = this;

	managedTextures = useManagedTextures;
	ZeroMemory (stageTextures, sizeof (stageTextures));
}


void		DX9Texturing::DeInit ()
{
	for (int i = 0; i < NumberOfDXTexFormats; i++) {
		if (namedFormatData[i].colorKeyTexture != NULL)
			namedFormatData[i].colorKeyTexture->Release ();
		namedFormatData[i].colorKeyTexture = NULL;
	}
	if (paletteTexture != NULL) {
		paletteTexture->Release ();
		paletteTexture = NULL;
	}
}


void		DX9Texturing::GetSupportedTextureFormats (NamedFormat* namedFormats)
{
	DXTextureFormats dxTextureFormats[] = {Pf8_P8, Pf16_ARGB1555, Pf16_ARGB4444, Pf16_RGB555,
										   Pf16_RGB565, Pf32_ARGB8888, Pf32_RGB888, Pf8_AuxP8, Pf16_AuxP8A8};
	D3DFORMAT d3d9Formats[] = {D3DFMT_P8, D3DFMT_A1R5G5B5, D3DFMT_A4R4G4B4, D3DFMT_X1R5G5B5,
							   D3DFMT_R5G6B5, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_L8, D3DFMT_A8L8};

	for (int i = 0; i < NumberOfDXTexFormats; i++) {

		ConvertToPixFmt (d3d9Formats[i], &namedFormats[i].pixelFormat);
		
		namedFormatData[dxTextureFormats[i]].d3d9TextureFromat = d3d9Formats[i];
		namedFormatData[dxTextureFormats[i]].bitPerPixel = namedFormats[i].pixelFormat.BitPerPixel;
		namedFormatData[dxTextureFormats[i]].colorKeyTexture = NULL;
		namedFormatData[dxTextureFormats[i]].colorKey = 0x1;

		HRESULT hr = lpD3D9->CheckDeviceFormat (this->adapter, this->deviceType, this->deviceDisplayFormat,
												D3DUSAGE_DYNAMIC, D3DRTYPE_TEXTURE, d3d9Formats[i]);

		//Palettás textúrák támogatása letiltva
		DXTextureFormats format = dxTextureFormats[i];
		if (hr == D3D_OK && (format != Pf8_P8)) {

			if (usePixelShaders) {
				if (format != Pf8_AuxP8 && format != Pf16_AuxP8A8) {
					HRESULT hr = lpD3Ddevice9->CreateTexture (1, 1, 1, 0, d3d9Formats[i], D3DPOOL_MANAGED,
															  &namedFormatData[dxTextureFormats[i]].colorKeyTexture, NULL);

					if (!FAILED (hr)) {
						SetSrcColorKeyForTextureFormat ((DXTextureFormats) i, 0);
						namedFormats[dxTextureFormats[i]].isValid = 1;
					} else {
						DEBUGLOG (0, "\n   Warning: DX9Texturing::GetSupportedTextureFormats: Creating colorkey texture has failed.");
						DEBUGLOG (1, "\n   Figyelmeztetés: DX9Texturing::GetSupportedTextureFormats: A colorkey textúra létrehozása nem sikerült.");
					} 
				} else if ((config.dx9ConfigBits & CFG_HWPALETTEGEN) && !managedTextures) {
					namedFormats[dxTextureFormats[i]].isValid = 1;
				}
			}
		}
	}
	paletteTexture = NULL;

	//AP multitexturing is not implemented with pixel shaders, so AuxP8 and AuxP8A8 must be both supported or unsupported
	bool auxPalTexSupported = (namedFormats[Pf8_AuxP8].isValid == 1 && namedFormats[Pf16_AuxP8A8].isValid == 1);

	if  (auxPalTexSupported)
	{
		DXTextureFormats palTexFormats[] = {Pf32_RGB888, Pf32_ARGB8888, Pf16_RGB565, Pf16_RGB555, Pf16_ARGB1555, Pf16_ARGB4444};

		D3DFORMAT palTexFmt = D3DFMT_UNKNOWN;
		for (int i = 0; i < sizeof(palTexFormats)/sizeof(DXTextureFormats); i++) {
			if (namedFormats[palTexFormats[i]].isValid) {
				palTexFmt = namedFormatData[palTexFormats[i]].d3d9TextureFromat;
				break;
			}
		}

		if (palTexFmt != D3DFMT_UNKNOWN) {
			ConvertToPixFmt (palTexFmt, &paletteTextureFormat);
			ConvertToPixFmt (D3DFMT_X8R8G8B8, &rgbPaletteFormat);

			HRESULT hr = lpD3Ddevice9->CreateTexture (256, 1, 1, 0, palTexFmt, D3DPOOL_MANAGED,
																	&paletteTexture, NULL);
			auxPalTexSupported = !FAILED (hr);
		} else {
			auxPalTexSupported = false;
		}
	}
	
	if (!auxPalTexSupported)
		namedFormats[Pf8_AuxP8].isValid = namedFormats[Pf16_AuxP8A8].isValid = 0;
}


int			DX9Texturing::AreNonPow2TexturesSupported ()
{
	DWORD cap = d3dCaps9.TextureCaps & (D3DPTEXTURECAPS_POW2 | D3DPTEXTURECAPS_NONPOW2CONDITIONAL);
	return ((cap == (D3DPTEXTURECAPS_POW2 | D3DPTEXTURECAPS_NONPOW2CONDITIONAL)) || (cap == 0));
}


int			DX9Texturing::CreateDXTexture (enum DXTextureFormats format, unsigned int x, unsigned int y, int mipMapNum,
										   TextureType* mipMaps, int hwGeneratedTexture)
{
	if (format > NumberOfDXTexFormats)
		return (0);

	LPDIRECT3DTEXTURE9 lpD3DTexture9;

	HRESULT hr = lpD3Ddevice9->CreateTexture (x, y, mipMapNum, hwGeneratedTexture ? D3DUSAGE_RENDERTARGET : ((managedTextures ? 0 : D3DUSAGE_DYNAMIC)),
											  namedFormatData[format].d3d9TextureFromat,
											  managedTextures ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &lpD3DTexture9, NULL);
	if (!FAILED (hr)) {
		for (int i = 0; i < mipMapNum; i++) {
			mipMaps[i] = (TextureType) lpD3DTexture9;
		}
		return (1);
	}

	return (0);
}


void		DX9Texturing::DestroyDXTexture (TextureType texture)
{
	((LPDIRECT3DTEXTURE9) texture)->Release ();
}


void*		DX9Texturing::GetPtrToTexture (TextureType texture, unsigned int mipMapLevel, unsigned int *pitch)
{
	D3DLOCKED_RECT lockedRectInfo;

	HRESULT hr = ((LPDIRECT3DTEXTURE9) texture)->LockRect (mipMapLevel, &lockedRectInfo, NULL, D3DLOCK_NOSYSLOCK);

	if (!FAILED (hr)) {
		if (pitch != NULL) {
			*pitch = (unsigned int) lockedRectInfo.Pitch;
		}
		return lockedRectInfo.pBits;
	}

	return NULL;
}


void		DX9Texturing::ReleasePtrToTexture (TextureType texture, unsigned int mipMapLevel)
{
	((LPDIRECT3DTEXTURE9) texture)->UnlockRect (mipMapLevel);
}


void		DX9Texturing::SetSrcColorKeyForTexture (TextureType /*texture*/, unsigned int /*colorKey*/)
{
}


void		DX9Texturing::SetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex, unsigned int colorKey)
{
	if (usePixelShaders && (namedFormatData[dxTexFormatIndex].colorKey != colorKey)) {

		unsigned int texPitch;
		void* texPtr = GetPtrToTexture ((TextureType) namedFormatData[dxTexFormatIndex].colorKeyTexture, 0, &texPitch);

		if (texPtr != NULL) {
			if (namedFormatData[dxTexFormatIndex].bitPerPixel == 16) {
				unsigned short* texPtr16 = (unsigned short*) texPtr;
				*texPtr16 = (unsigned short) colorKey;
			} else {
				unsigned int* texPtr32 = (unsigned int*) texPtr;
				*texPtr32 = colorKey;
			}
			ReleasePtrToTexture ((TextureType) namedFormatData[dxTexFormatIndex].colorKeyTexture, 0);
		} else
			colorKey++;

		namedFormatData[dxTexFormatIndex].colorKey = colorKey;
	}
}


unsigned int DX9Texturing::GetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex)
{
	return (namedFormatData[dxTexFormatIndex].colorKey);
}


TextureType	DX9Texturing::GetAuxTextureForNativeCk (DXTextureFormats dxTexFormatIndex)
{
	return (namedFormatData[dxTexFormatIndex].colorKeyTexture);
}


int			DX9Texturing::FillTexture (TextureType texture, unsigned int /*colorKeyARGB*/, unsigned int colorKey, _pixfmt* textureFormat)
{
	D3DSURFACE_DESC textureDesc;
	HRESULT hr = ((LPDIRECT3DTEXTURE9) texture)->GetLevelDesc (0, &textureDesc);
	
	if (!FAILED (hr)) {
		D3DLOCKED_RECT lockedRectInfo;

		HRESULT hr = ((LPDIRECT3DTEXTURE9) texture)->LockRect (0, &lockedRectInfo, NULL, D3DLOCK_NOSYSLOCK);

		if (!FAILED (hr)) {
			
				MMCFillBuffer (lockedRectInfo.pBits, textureFormat->BitPerPixel, colorKey, textureDesc.Width, textureDesc.Height,
								lockedRectInfo.Pitch);

			((LPDIRECT3DTEXTURE9) texture)->UnlockRect (0);
			return (1);
		}
	}
	return (0);
}


int			DX9Texturing::CanRestoreTextures ()
{
	return (managedTextures);
}


int			DX9Texturing::CanPreserveTextures ()
{
	return (managedTextures);
}


int			DX9Texturing::RestoreTexturePhysicallyIfNeeded (TextureType /*texture*/)
{
	return(0);
}


void		DX9Texturing::SetTextureAtStage (int stageNum, TextureType texture)
{
	lpD3Ddevice9->SetTexture(stageNum, stageTextures[stageNum] = (LPDIRECT3DTEXTURE9) texture);
}


void		DX9Texturing::SetTextureStageMinFilter (int stageNum, GrTextureFilterMode_t mode)
{
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_MINFILTER,
								   (mode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTEXF_POINT : D3DTEXF_LINEAR);
}


void		DX9Texturing::SetTextureStageMagFilter (int stageNum, GrTextureFilterMode_t mode)
{
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_MAGFILTER,
								   (mode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTEXF_POINT : D3DTEXF_LINEAR);
}


void		DX9Texturing::SetTextureStageClampModeU (int stageNum, GrTextureClampMode_t clampMode)
{
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_ADDRESSU,
								   (clampMode == GR_TEXTURECLAMP_WRAP) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
}


void		DX9Texturing::SetTextureStageClampModeV (int stageNum, GrTextureClampMode_t clampMode)
{
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_ADDRESSV,
								   (clampMode == GR_TEXTURECLAMP_WRAP) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
}


void		DX9Texturing::SetTextureStageLodBias (int stageNum, float bias)
{
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_MIPMAPLODBIAS, *( (LPDWORD) (&bias)) );
}


void		DX9Texturing::SetTextureStageMipMapMode (int stageNum, GrMipMapMode_t mipMapMode, FxBool lodBlend)
{
DWORD val;

	switch(mipMapMode)	{
		case GR_MIPMAP_NEAREST:
			val = (lodBlend == FXTRUE) ? D3DTEXF_LINEAR : D3DTEXF_POINT;
			break;
		case GR_MIPMAP_NEAREST_DITHER:
			val = D3DTEXF_LINEAR;
			break;
		default:
			val = D3DTEXF_NONE;
	}
	lpD3Ddevice9->SetSamplerState (stageNum, D3DSAMP_MIPFILTER, val);
}



PaletteType	DX9Texturing::CreatePalette (PALETTEENTRY*	/*initialPalEntries*/)
{
	return NULL;
}


void		DX9Texturing::DestroyPalette (PaletteType /*palette*/)
{
}


int			DX9Texturing::AssignPaletteToTexture (TextureType /*texture*/, PaletteType /*newPalette*/)
{
	return (0);
}


void		DX9Texturing::SetPaletteEntries (TextureType /*palette*/, PALETTEENTRY* /*palEntries*/)
{
}


void		DX9Texturing::GeneratePalettedTexture (TextureType* dstTextures, TextureType* srcTextures,
												   unsigned int width, unsigned int height,
												   unsigned int mipMapLevel, int mipMapNum, unsigned int* palette)
{
	void* palTexPtr = GetPtrToTexture ((TextureType) paletteTexture, 0, NULL);

	MMCConvertAndTransferData (&rgbPaletteFormat, &paletteTextureFormat, 0, 0, 0, palette, palTexPtr, 256, 1, 0, 0,
							   NULL, NULL, CONVERTTYPE_COPY, PALETTETYPE_UNCONVERTED);

	//CopyMemory (palTexPtr, palette, 256*4);

	ReleasePtrToTexture ((TextureType) paletteTexture, 0);

	DWORD at, ab, zen, cm, fe, cwmask;

	lpD3Ddevice9->GetRenderState(D3DRS_ALPHATESTENABLE, &at);
	lpD3Ddevice9->GetRenderState(D3DRS_ALPHABLENDENABLE, &ab);	
	lpD3Ddevice9->GetRenderState(D3DRS_ZENABLE, &zen);
	lpD3Ddevice9->GetRenderState(D3DRS_CULLMODE, &cm);
	lpD3Ddevice9->GetRenderState(D3DRS_FOGENABLE, &fe);
	lpD3Ddevice9->GetRenderState(D3DRS_COLORWRITEENABLE, &cwmask);

	lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
	lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	lpD3Ddevice9->SetRenderState(D3DRS_FOGENABLE, FALSE);
	lpD3Ddevice9->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);

	DWORD minF0, magF0, minF1, magF1;
	lpD3Ddevice9->GetSamplerState (0, D3DSAMP_MINFILTER, &minF0);
	lpD3Ddevice9->GetSamplerState (0, D3DSAMP_MAGFILTER, &magF0);
	lpD3Ddevice9->GetSamplerState (1, D3DSAMP_MINFILTER, &minF1);
	lpD3Ddevice9->GetSamplerState (1, D3DSAMP_MAGFILTER, &magF1);


	lpD3Ddevice9->SetTexture(1, (LPDIRECT3DTEXTURE9) paletteTexture);
	lpD3Ddevice9->SetSamplerState (0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	lpD3Ddevice9->SetSamplerState (0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	lpD3Ddevice9->SetSamplerState (1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	lpD3Ddevice9->SetSamplerState (1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	lpD3Ddevice9->SetDepthStencilSurface (NULL);
	lpD3Ddevice9->SetPixelShader (dx9General->lpD3DStaticShaders[DX9General::PaletteGenerator]);

	//DEBUGLOG (2, "\nGeneratePalettedTexture (w=%d, h=%d, mipLevel=%d, mipNum=%d)", width, height, mipMapLevel, mipMapNum);
	
	for (int i = 0; i < mipMapNum; i++) {
		if (dstTextures[i] != NULL) {
			
			lpD3Ddevice9->SetTexture(0, (LPDIRECT3DTEXTURE9) srcTextures[i]);
		
			//DEBUGLOG (2, "\n   i: %d, dstTextures[i]:%p",i,dstTextures[i]);
		
			LPDIRECT3DSURFACE9 texSurface;
			((LPDIRECT3DTEXTURE9) dstTextures[i])->GetSurfaceLevel (mipMapLevel+i, &texSurface);
			lpD3Ddevice9->SetRenderTarget (0, texSurface);

			dx9Drawing->DrawTile (0.0f, 0.0f, (float) width, (float) height,
								  0.5f/width, 0.5f/height, (width+0.5f)/width, (height+0.5f)/height,
								  1.0f, 0.0f, 0);

			texSurface->Release ();
		}
		width >>= 1;
		height >>= 1;
	}

	lpD3Ddevice9->SetRenderTarget(0, renderBuffers[actRenderTarget]);
	lpD3Ddevice9->SetDepthStencilSurface (lpD3Ddepth9);

	lpD3Ddevice9->SetSamplerState (0, D3DSAMP_MINFILTER, minF0);
	lpD3Ddevice9->SetSamplerState (0, D3DSAMP_MAGFILTER, magF0);
	lpD3Ddevice9->SetSamplerState (1, D3DSAMP_MINFILTER, minF1);
	lpD3Ddevice9->SetSamplerState (1, D3DSAMP_MAGFILTER, magF1);
	lpD3Ddevice9->SetTexture(0, stageTextures[0]);
	lpD3Ddevice9->SetTexture(1, stageTextures[1]);

	lpD3Ddevice9->SetRenderState(D3DRS_COLORWRITEENABLE, cwmask);
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE, at);
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, ab);	
	lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, zen);
	lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, cm);
	lpD3Ddevice9->SetRenderState(D3DRS_FOGENABLE, fe);

	lpD3Ddevice9->SetPixelShader (dx9General->currentPixelShader);
}


int			DX9Texturing::ReallocDefaultPoolResources ()
{
	return (1);
}