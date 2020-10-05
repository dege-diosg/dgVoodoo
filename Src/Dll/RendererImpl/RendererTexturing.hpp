/*--------------------------------------------------------------------------------- */
/* RendererTexturing.hpp - dgVoodoo 3D rendering abstract interface for texturing   */
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
/* dgVoodoo: RendererTexturing.hpp															*/
/*			 A Texturing interfésze															*/
/*------------------------------------------------------------------------------------------*/

#include "texture.h"
#include "RendererTypes.hpp"


class	RendererTexturing
{
public:
	virtual void		Init (int useManagedTextures) = 0;
	virtual void		DeInit () = 0;
	virtual void		GetSupportedTextureFormats (NamedFormat* namedFormats) = 0;
	virtual int			AreNonPow2TexturesSupported () = 0;

	virtual int			CreateDXTexture (DXTextureFormats format,
										 unsigned int x, unsigned int y, int mipMapNum, TextureType* mipMaps,
										 int hwGeneratedTexture) = 0;
	virtual void		DestroyDXTexture (TextureType texture) = 0;
	virtual void*		GetPtrToTexture (TextureType texture, unsigned int mipMapLevel, unsigned int *pitch) = 0;
	virtual void		ReleasePtrToTexture (TextureType texture, unsigned int mipMapLevel) = 0;
	virtual void		SetSrcColorKeyForTexture (TextureType texture, unsigned int colorKey) = 0;
	virtual void		SetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex, unsigned int colorKey) = 0;
	virtual unsigned int GetSrcColorKeyForTextureFormat (DXTextureFormats dxTexFormatIndex) = 0;
	virtual TextureType	GetAuxTextureForNativeCk (DXTextureFormats dxTexFormatIndex) = 0;
	virtual int			FillTexture (TextureType texture, unsigned int colorKeyARGB, unsigned int colorKey, _pixfmt* textureFormat) = 0;
	virtual int			CanRestoreTextures () = 0;
	virtual int			CanPreserveTextures () = 0;
	virtual int			RestoreTexturePhysicallyIfNeeded (TextureType texture) = 0;

	virtual void		SetTextureAtStage (int stageNum, TextureType texture) = 0;
	virtual void		SetTextureStageMinFilter (int stageNum, GrTextureFilterMode_t mode) = 0;
	virtual void		SetTextureStageMagFilter (int stageNum, GrTextureFilterMode_t mode) = 0;
	virtual void		SetTextureStageClampModeU (int stageNum, GrTextureClampMode_t clampMode) = 0;
	virtual void		SetTextureStageClampModeV (int stageNum, GrTextureClampMode_t clampMode) = 0;
	virtual void		SetTextureStageLodBias (int stageNum, float bias) = 0;
	virtual void		SetTextureStageMipMapMode (int stageNum, GrMipMapMode_t mipMapMode, FxBool lodBlend) = 0;
	
	virtual PaletteType	CreatePalette (PALETTEENTRY* initialPalEntries) = 0;
	virtual void		DestroyPalette (PaletteType palette) = 0;
	virtual int			AssignPaletteToTexture (TextureType texture, PaletteType newPalette) = 0;
	virtual void		SetPaletteEntries (TextureType palette, PALETTEENTRY* entries) = 0;
	virtual void		GeneratePalettedTexture (TextureType* dstTexture, TextureType* srcTexture,
												 unsigned int width, unsigned int height,
												 unsigned int mipMapLevel, int mipMapNum, unsigned int* palette) = 0;
};
