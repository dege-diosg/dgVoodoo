/*--------------------------------------------------------------------------------- */
/* Texture.h - dgVoodoo Glide implementation, stuffs related to texturing           */
/*                                                                                  */
/* Copyright (C) 2004 Dege                                                          */
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
/* dgVoodoo: Texture.h																		*/
/*			 Függvények a textúrázáshoz														*/
/*------------------------------------------------------------------------------------------*/

#ifndef		TEXTURE_H
#define		TEXTURE_H

#include	<glide.h>
#include	"MMConv.h"

#include	"RendererTypes.hpp"

/* Definíciók, hogy az egyes texture combining függvények mit implementálnak */
#define TCF_ZERO				0			/* nulla */
#define TCF_CLOCAL				1			/* textúra rgb */
#define TCF_ALOCAL				2			/* textúra alpha */


/* Az egyes támogatott DirectX-es textúraformátumok típusai */
enum DXTextureFormats {

	Pf8_P8				= 0,
	Pf16_ARGB1555		= 1,
	Pf16_ARGB4444		= 2,
	Pf16_RGB555			= 3,
	Pf16_RGB565			= 4,
	Pf32_ARGB8888		= 5,
	Pf32_RGB888			= 6,

	Pf8_AuxP8			= 7,		//8 bit paletteconverter aux texture
	Pf16_AuxP8A8		= 8,		//16 bit paletteconverter aux texture
	NumberOfDXTexFormats,
	Pf_Invalid

};


/* Struktúra, amely egy adott típusú (nevû) pixelformátumot tárol */
typedef struct _NamedFormat {

	int						isValid;					/* formátum támogatott? */
	unsigned char			*missing;					/* pointer más formátumtípusokra, ha ez nem támogatott */
	_pixfmt					pixelFormat;				/* Pixelformátum leírása */
	unsigned int			*constPalettes;
	unsigned int			colorKeyInTexFmt;			/* Az aktuális colorkey textúraformátumban */
	enum DXTextureFormats	dxTexFormatIndex;			/* A formátum indexe */

} NamedFormat;

struct	TexCacheEntry;

extern	char			texCombineFuncImp[];

/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

int						TexInitAtGlideInit();
void					TexCleanUpAtShutDown();
int						TexInit();
void					TexCleanUp();
void					TexReleaseAllCachedTextures ();
void					TexCreateSpecialTextures ();
enum DXTextureFormats	TexGetMatchingDXFormat (GrTextureFormat_t glideTexFormat);
unsigned int			TexGetFormatConstPaletteIndex (GrTextureFormat_t glideTexFormat);
_pixfmt*				TexGetFormatPixFmt (GrTextureFormat_t glideTexFormat);
_pixfmt*				TexGetDXFormatPixelFormat (enum DXTextureFormats format);
unsigned int*			TexGetDXFormatConstPalette (enum DXTextureFormats format, unsigned int paletteIndex);
int						TexIsDXFormatSupported (enum DXTextureFormats format);
void					TexGetDXTexPixFmt (enum DXTextureFormats format, _pixfmt *pixFormat);

void					GlideTexSource();
void					TexUpdateNativeColorKeyState();
void					GlideTexSetTexturesAsLost();
void					TexUpdateMultitexturingState(struct TexCacheEntry *tinfo);
void					TexSetCurrentTexture(struct TexCacheEntry *tinfo);
void					TexUpdateTexturePalette();
void					TexUpdateTextureNccTable();
void					TexUpdateColorKeyState ();
void					TexUpdateAlphaBasedColorKeyState();
int						GlideCopyMipMapInfo(GrMipMapInfo *dst, GrMipMapId_t mmid);
unsigned int			GlideGetUTextureSize(GrMipMapId_t mmid, GrLOD_t lod, int lodvalid);

/*------------------------------------------------------------------------------------------*/
/*............................. Glide-függvények predeklarációja ...........................*/

FxU32			EXPORT	grTexMinAddress(GrChipID_t tmu);

FxU32			EXPORT	grTexMaxAddress(GrChipID_t tmu);

FxU32			EXPORT	grTexCalcMemRequired(GrLOD_t smallLod, GrLOD_t largeLod,
											 GrAspectRatio_t aspect,
											 GrTextureFormat_t format);

FxU32			EXPORT	grTexTextureMemRequired(FxU32 evenOdd, GrTexInfo *info);

void			EXPORT	grTexFilterMode(GrChipID_t tmu,
									    GrTextureFilterMode_t minFilterMode,
									    GrTextureFilterMode_t magFilterMode);

void			EXPORT	grTexClampMode(GrChipID_t tmu,
									   GrTextureClampMode_t sClampMode,
									   GrTextureClampMode_t tClampMode);

void			EXPORT	grTexLodBiasValue(GrChipID_t tmu, float bias);

void			EXPORT	grTexDownloadMipMap(GrChipID_t tmu, FxU32 startAddress,
											FxU32 evenOdd, GrTexInfo *info);

void			EXPORT	grTexDownloadMipMapLevelPartial(GrChipID_t tmu, FxU32 startAddress,
														GrLOD_t thisLod, GrLOD_t largeLod,
														GrAspectRatio_t aspectRatio,
														GrTextureFormat_t format,
														FxU32 evenOdd, void *data,
														unsigned int start,
														unsigned int end);

void			EXPORT	grTexDownloadMipMapLevel(GrChipID_t tmu, FxU32 startAddress,
												 GrLOD_t thisLod, GrLOD_t largeLod,
												 GrAspectRatio_t aspectRatio,
												 GrTextureFormat_t format,
												 FxU32 evenOdd, void *data);

void			EXPORT	grTexSource(GrChipID_t tmu, FxU32 startAddress, 
								    FxU32 evenOdd, GrTexInfo *info);

void			EXPORT	grTexMipMapMode(GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend);

void			EXPORT	grTexCombine(GrChipID_t tmu, GrCombineFunction_t rgb_function,
									 GrCombineFactor_t rgb_factor, 
									 GrCombineFunction_t alpha_function,
									 GrCombineFactor_t alpha_factor,
									 FxBool rgb_invert,
									 FxBool alpha_invert);

void			EXPORT	grTexDownloadTable(GrChipID_t tmu, GrTexTable_t type, void *data);

void			EXPORT	grTexDownloadTablePartial(GrChipID_t tmu, GrTexTable_t type, void *data,
												  int start, int end);

void			EXPORT	grTexNCCTable(GrChipID_t tmu, GrNCCTable_t table);

void			EXPORT	grTexMultibase(GrChipID_t tmu, FxBool enable);

void			EXPORT	grTexMultibaseAddress(GrChipID_t tmu, GrTexBaseRange_t range,
									 FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info);

FxBool			EXPORT	gu3dfGetInfo(const char *filename, Gu3dfInfo *info);

FxBool			EXPORT	gu3dfLoad(const char *filename, Gu3dfInfo *info);

void			EXPORT	grChromakeyMode(GrChromakeyMode_t mode);

void			EXPORT	grChromakeyValue(GrColor_t value);

GrMipMapId_t	EXPORT	guTexAllocateMemory(GrChipID_t tmu,
											FxU8 evenOddMask,
											int width, int height,
											GrTextureFormat_t format,
											GrMipMapMode_t mmMode,
											GrLOD_t smallLod, GrLOD_t largeLod,
											GrAspectRatio_t aspectRatio,
											GrTextureClampMode_t sClampMode,
											GrTextureClampMode_t tClampMode,
											GrTextureFilterMode_t minFilterMode,
											GrTextureFilterMode_t magFilterMode,
											float lodBias,
											FxBool lodBlend );

FxBool			EXPORT	guTexChangeAttributes(GrMipMapId_t mmid,
											  int width, int height,
											  GrTextureFormat_t format,
											  GrMipMapMode_t mmMode,
											  GrLOD_t smallLod, GrLOD_t largeLod,
											  GrAspectRatio_t aspectRatio,
											  GrTextureClampMode_t sClampMode,
											  GrTextureClampMode_t tClampMode,
											  GrTextureFilterMode_t minFilterMode,
											  GrTextureFilterMode_t magFilterMode );

void			EXPORT	guTexDownloadMipMap(GrMipMapId_t mmid, const void *src,
											const GuNccTable *nccTable );

void			EXPORT	guTexDownloadMipMapLevel(GrMipMapId_t mmid, GrLOD_t lod, const void **src);

GrMipMapId_t	EXPORT	guTexGetCurrentMipMap (GrChipID_t tmu);

unsigned int	EXPORT	guTexGetMipMapInfo( GrMipMapId_t mmid );

void			EXPORT	guTexMemReset(void);

FxU32			EXPORT	guTexMemQueryAvail(GrChipID_t tmu);

void			EXPORT	guTexSource(GrMipMapId_t mmid);

void			EXPORT	guTexCombineFunction(GrChipID_t tmu, GrTextureCombineFnc_t func);

void			EXPORT	grTexCombineFunction(GrChipID_t tmu, GrTextureCombineFnc_t func);


#endif