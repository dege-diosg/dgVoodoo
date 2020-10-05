/*--------------------------------------------------------------------------------- */
/* DX7Texturing.hpp - dgVoodoo 3D rendering interface for texturing,                */
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

#include "DX7RendererBase.hpp"
#include "RendererTexturing.hpp"

class	DX7Texturing: public DX7RendererBase,
					  public RendererTexturing
{
private:
	static struct NamedFormatData {
		
		DDPIXELFORMAT	ddsdPixelFormat;			/* Pixelform�tum DirectDraw-f�le le�r�sa */
	
	} namedFormatData[NumberOfDXTexFormats];

	int					managedTextures;

private:
	static  HRESULT		CALLBACK TexEnumFormatsCallback(LPDDPIXELFORMAT lpDDpf, void* namedFormats);

public:
	virtual void		Init (int useManagedTextures);
	virtual void		DeInit ();
	virtual void		GetSupportedTextureFormats (NamedFormat* namedFormats);
	virtual int			AreNonPow2TexturesSupported ();

	virtual int			CreateDXTexture (enum DXTextureFormats format, unsigned int x, unsigned int y, int mipMapNum,
										 TextureType* mipMaps, int hwGeneratedTexture);
	virtual void		DestroyDXTexture (TextureType texture);
	virtual void*		GetPtrToTexture (TextureType texture, unsigned int mipMapLevel, unsigned int *pitch);
	virtual void		ReleasePtrToTexture (TextureType texture, unsigned int mipMapLevel);
	virtual void		SetSrcColorKeyForTexture (TextureType texture, unsigned int colorKey);
	virtual void		SetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex, unsigned int colorKey);
	virtual unsigned int GetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex);
	virtual TextureType	GetAuxTextureForNativeCk (DXTextureFormats dxTexFormatIndex);
	virtual int			FillTexture (TextureType texture, unsigned int colorKeyARGB, unsigned int colorKey, _pixfmt* textureFormat);
	virtual int			CanRestoreTextures ();
	virtual int			CanPreserveTextures ();
	virtual int			RestoreTexturePhysicallyIfNeeded (TextureType texture);

	virtual void		SetTextureAtStage (int stageNum, TextureType texture);
	virtual void		SetTextureStageMinFilter (int stageNum, GrTextureFilterMode_t mode);
	virtual void		SetTextureStageMagFilter (int stageNum, GrTextureFilterMode_t mode);
	virtual void		SetTextureStageClampModeU (int stageNum, GrTextureClampMode_t clampMode);
	virtual void		SetTextureStageClampModeV (int stageNum, GrTextureClampMode_t clampMode);
	virtual void		SetTextureStageLodBias (int stageNum, float bias);
	virtual void		SetTextureStageMipMapMode (int stageNum, GrMipMapMode_t mipMapMode, FxBool lodBlend);
	
	virtual PaletteType	CreatePalette (PALETTEENTRY*	initialPalEntries);
	virtual void		DestroyPalette (PaletteType palette);
	virtual int			AssignPaletteToTexture (TextureType texture, PaletteType newPalette);
	virtual void		SetPaletteEntries (TextureType palette, PALETTEENTRY* entries);
	virtual void		GeneratePalettedTexture (TextureType* dstTextures, TextureType* srcTextures,
												 unsigned int width, unsigned int height,
												 unsigned int mipMapLevel, int mipMapNum, unsigned int* palette);
};