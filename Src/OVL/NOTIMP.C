/*--------------------------------------------------------------------------------- */
/* Notimp.c - DOS Glide2x OVL unimplemented but exported functions                  */
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
/* dgVoodoo: NotImp.c																		*/
/*			 Ismert funkciójú, de nem implementált függvények								*/
/*------------------------------------------------------------------------------------------*/

#define E(name)

void DisplayUnimplemented(char *s)	{
}

/*void EXPORT grAADrawLine(GrVertex *va, GrVertex *vb) {}*/
/*void EXPORT grAADrawPoint(GrVertex *p) { E("grAADrawPoint") }*/
/*void EXPORT grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[]) {
}*/
/*void EXPORT grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[]) {
}*/
/*void EXPORT grAADrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c,
                                           FxBool antialiasAB, FxBool antialiasBC, FxBool antialiasCA) {
}*/
/*void EXPORT grAlphaBlendFunction(       GrAlphaBlendFnc_t rgb_sf,
	GrAlphaBlendFnc_t rgb_df,
	GrAlphaBlendFnc_t alpha_sf,
	GrAlphaBlendFnc_t alpha_df 
)  {E("grAlphaBlendFunction") }*/
/*void EXPORT grAlphaCombine(     GrCombineFunction_t func, 
	GrCombineFactor_t factor,
	GrCombineLocal_t local, 
	GrCombineOther_t other, 
	FxBool invert 
        ) {E("grAlphaCombine") }*/
/*void EXPORT grAlphaTestFunction( GrCmpFnc_t function ) {E("grAlphaTestFunction") }
void EXPORT grAlphaTestReferenceValue( GrAlpha_t value ) {E("grAlphaTestReferenceValue") }*/
//void EXPORT grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth ) { }
/*int EXPORT grBufferNumPending( void ) { E("grBufferNumPending")}*/
/*void EXPORT grBufferSwap( int swap_interval ) { }*/
/*void EXPORT grChromakeyMode( GrChromakeyMode_t mode ) {E("grChromakeyMode") }
void EXPORT grChromakeyValue( GrColor_t value ) {E("grChromakeyValue") }*/
/*void EXPORT grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy ) {E("grClipWindow") }*/
/*void EXPORT grColorCombine(     GrCombineFunction_t func, 
	GrCombineFactor_t factor,
	GrCombineLocal_t local, 
	GrCombineOther_t other, 
	FxBool invert 
        ) {E("grColorCombine") }*/
//void EXPORT grColorMask( FxBool rgb, FxBool alpha ) {E("grColorMask") }
/*void EXPORT grConstantColorValue( GrColor_t color ) {E("grConstantColorValue") }*/
/*void EXPORT grCullMode( GrCullMode_t mode ) {
}*/
//void EXPORT grDepthBiasLevel( FxI16 level ) {E("grDepthBiasLevel") }
/*void EXPORT grDepthBufferFunction( GrCmpFnc_t func ) {E("grDepthBufferFunction") }*/
/*void EXPORT grDepthBufferMode( GrDepthBufferMode_t mode ) {E("grDepthBufferMode") }*/
/*void EXPORT grDepthMask( FxBool enable ) {E("grDepthMask") }*/
/*void EXPORT grDisableAllEffects( void ) {
} */
/*void EXPORT grDitherMode( GrDitherMode_t mode ) { E("grDitherMode")}*/
/*void EXPORT grDrawLine( const GrVertex *a, const GrVertex *b ) {}*/
/*void EXPORT grDrawPlanarPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
}

}*/
/*void EXPORT grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] ) {
}

}*/
/*void EXPORT grDrawPoint( const GrVertex *a ) { E("grDrawPoint")}*/
/*void EXPORT grDrawPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
}

}*/
/*void EXPORT grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] ) {
}
}*/
//void EXPORT grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c ) { E("grDrawTriangle")}
void EXPORT grErrorSetCallback( void (*function)(const char *string, FxBool fatal) ) {E("grErrorSetCallback") }
/*void EXPORT grFogColorValue( GrColor_t value ) { E("grFogColorValue")}
void EXPORT grFogMode( GrFogMode_t mode ) { E("grFogMode")}
void EXPORT grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] ) {E("grFogTable") }*/
/*void EXPORT grGammaCorrectionValue( float value ) {E("grGammaCorrectionValue") }*/
/*void EXPORT grGlideGetVersion( char version[80] ) { }*/
/*void EXPORT grGlideGetState( GrState *state ) {
_asm {
//iuy: jmp iuy
}
}*/
/*void EXPORT grGlideInit( void ) { }*/
/*void EXPORT grGlideSetState( const GrState *state ) {
//_asm ghj125: jmp ghj125
}*/
/*void EXPORT grGlideShutdown( void ) { }*/
/*void EXPORT grHints( GrHint_t type, FxU32 hintMask ) {
_asm ghj126: jmp ghj126
}*/
/*void EXPORT grLfbConstantAlpha( GrAlpha_t alpha ) { E("grLfbConstantAlpha")}*/
/*void EXPORT grLfbConstantDepth( FxU16 depth ) {E("grLfbConstantDepth") }*/
/*FxBool EXPORT grLfbReadRegion(  GrBuffer_t src_buffer, 
	FxU32 src_x, FxU32 src_y, 
	FxU32 src_width, FxU32 src_height, 
        FxU32 dst_stride, void *dst_data ) {
//_asm jkl123: jmp jkl123
}*/
/*FxBool EXPORT grLfbWriteRegion( GrBuffer_t dst_buffer, 
	FxU32 dst_x, FxU32 dst_y, 
	GrLfbSrcFmt_t src_format, 
	FxU32 src_width, FxU32 src_height, 
	FxU32 src_stride, void *src_data 
        ) {
//_asm jkl124: jmp jkl124
}*/
/*void EXPORT grRenderBuffer( GrBuffer_t buffer ) { }*/
/*FxBool EXPORT grSstControl( FxU32 mode) { }*/
/*FxBool EXPORT grSstIsBusy( void ) { }*/
/*void EXPORT grSstOrigin( GrOriginLocation_t origin ) { E("grSstOrigin")}*/
/*FxBool EXPORT grSstQueryBoards( GrHwConfiguration *hwConfig ) { }
FxBool EXPORT grSstQueryHardware( GrHwConfiguration *hwConfig ) { }*/
/*FxU32 EXPORT grSstScreenHeight( void ) { E("grSstScreenHeight") }
FxU32 EXPORT grSstScreenWidth( void ) { E("grSstScreenWidth")}*/
void EXPORT grSstSelect( int which_sst ) { }
/*FxU32 EXPORT grSstStatus( void ) {E("grSstStatus") }*/
/*FxU32 EXPORT grSstVideoLine( void ) {E("grSstVideoLine") }*/
/*FxBool EXPORT grSstVRetraceOn( void ) {E("grSstVRetraceOn") }*/
/*void EXPORT grSstWinClose( void ) {E("grSstWinClose") }*/
/*FxBool EXPORT grSstWinOpen(    FxU32 hwnd,
	GrScreenResolution_t res,
	GrScreenRefresh_t ref,
	GrColorFormat_t cformat,
	GrOriginLocation_t org_loc,
	int num_buffers,
	int num_aux_buffers 
        ) { }*/
/*FxU32 EXPORT grTexCalcMemRequired(      GrLOD_t smallLod, GrLOD_t largeLod,
                                        GrAspectRatio_t aspect, GrTextureFormat_t format ) { E("grTexCalcMemRequired")}*/
/*void EXPORT grTexClampMode( GrChipID_t tmu,
 			 GrTextureClampMode_t sClampMode,
                         GrTextureClampMode_t tClampMode ) { E("grTexClampMode")}*/
/*void EXPORT grTexCombine(
             GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor, 
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert,
             FxBool alpha_invert
             ) {E("grTexCombine") }*/
/*void EXPORT grTexDownloadMipMap( GrChipID_t tmu, FxU32 startAddress,
                                                  FxU32 evenOdd, GrTexInfo *info ) {E("grTexDownloadMipMap") }*/
/*void EXPORT grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress,
				   GrLOD_t thisLod, GrLOD_t largeLod,
       				GrAspectRatio_t aspectRatio,
				GrTextureFormat_t format,
                                FxU32 evenOdd, void *data ) {E("grTexDownloadMipMapLevel") }*/
/*void EXPORT grTexDownloadMipMapLevelPartial(    GrChipID_t tmu, FxU32 startAddress,
						GrLOD_t thisLod, GrLOD_t largeLod,
						GrAspectRatio_t aspectRatio,
						GrTextureFormat_t format,
						FxU32 evenOdd, void *data,
                                                int start, int end ) {E("grTexDownloadMipMapLevelPartial") }*/
/*void EXPORT grTexDownloadTable( GrChipID_t tmu, GrTexTable_t type, void *data ) {E("grTexDownloadTable") }*/
/*void EXPORT grTexDownloadTablePartial( GrChipID_t tmu, GrTexTable_t type, void *data,
                                                                int start, int end ) {E("grTexDownloadTablePartial") }*/
/*void EXPORT grTexFilterMode( GrChipID_t tmu,
						GrTextureFilterMode_t minFilterMode,
                                                GrTextureFilterMode_t magFilterMode ) {E("grTexFilterMode") }*/
/*FxU32 EXPORT grTexMinAddress( GrChipID_t tmu ) {E("grTexMinAddress") }*/
/*FxU32 EXPORT grTexMaxAddress( GrChipID_t tmu ) {E("grTexMaxAddress") }*/
/*void EXPORT grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend ) {E("grTexMipMapMode") }*/
/*void EXPORT grTexMultibase( GrChipID_t tmu, FxBool enable ) {E("grTexMultibase") }
void EXPORT grTexMultibaseAddress( GrChipID_t tmu, GrTexBaseRange_t range,
                                                        FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info ) {E("grTexMultibaseAddress") }*/
/*void EXPORT grTexNCCTable( GrChipID_t tmu, GrNCCTable_t table ) {E("grTexNCCTable") }*/
/*void EXPORT grTexSource( GrChipID_t tmu, FxU32 startAddress, 
                                  FxU32 evenOdd, GrTexInfo *info ) {E("grTexSource") }*/
/*FxU32 EXPORT grTexTextureMemRequired( FxU32 evenOdd, GrTexInfo *info ) {E("grTexTextureMemRequired") }*/
/*FxBool EXPORT gu3dfGetInfo( const char *filename, Gu3dfInfo *info ) {E("gu3dfGetInfo") }*/
/*FxBool EXPORT gu3dfLoad( const char *filename, Gu3dfInfo *info ) { E("gu3dfLoad")}*/
/*void EXPORT guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc ) {
_asm    {
        mov edx,8
//dra8:        jmp dra8
}

}*/
/*void EXPORT guAlphaSource( GrAlphaSource_t mode ) {E("guAlphaSource") }*/
/*void EXPORT guColorCombineFunction( GrColorCombineFnc_t fnc ) {E("guColorCombineFunction") }*/
//void EXPORT guDrawTriangleWithClip(const GrVertex *va, const GrVertex *vb, const GrVertex *vc ) {E("guDrawTriangleWithClip") }
/*void EXPORT guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {E("guFogGenerateExp") }
void EXPORT guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) { E("guFogGenerateExp2")}
void EXPORT guFogGenerateLinear( GrFog_t fogTable[GR_FOG_TABLE_SIZE],
						  float nearW, float farW ) {E("guFogGenerateLinear") }
float EXPORT guFogTableIndexToW( int i ) { E("guFogTableIndexToW")}*/

/*GrMipMapId_t EXPORT guTexAllocateMemory(        GrChipID_t tmu,
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
                                                FxBool lodBlend )       {
}*/

/*FxBool EXPORT guTexChangeAttributes( GrMipMapId_t mmid,
					int width, int height,
					GrTextureFormat_t format,
					GrMipMapMode_t mmMode,
					GrLOD_t smallLod, GrLOD_t largeLod,
					GrAspectRatio_t aspectRatio,
					GrTextureClampMode_t sClampMode,
					GrTextureClampMode_t tClampMode,
					GrTextureFilterMode_t minFilterMode,
                                        GrTextureFilterMode_t magFilterMode ) { }*/
/*void EXPORT guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func ) {E("guTexCombineFunction") }*/
/*void EXPORT guTexDownloadMipMap( GrMipMapId_t mmid, const void *src,
                                  const GuNccTable *nccTable ) { E("guTexDownloadMipMap")}*/
/*void EXPORT guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src ) {E("guTexDownloadMipMapLevel") }*/
/*GrMipMapId_t *EXPORT guTexGetCurrentMipMap ( GrChipID_t tmu ) { }*/
//GrMipMapInfo *EXPORT guTexGetMipMapInfo( GrMipMapId_t mmid ) { }
/*FxU32 EXPORT guTexMemQueryAvail( GrChipID_t tmu ) { E("guTexMemQueryAvail")}*/
/*void EXPORT guTexMemReset( void ) {E("guTexMemReset") }*/
/*void EXPORT guTexSource( GrMipMapId_t mmid ) { E("guTexSource")}*/


/* Amely függvényeket semmiképp sem lehet implementálni DirectX 7.0-ban: */

void EXPORT grSstPerfStats( GrSstPerfStats_t *pStats ) {E("grSstPerfStats") }
void EXPORT grSstResetPerfStats( void ) { E("grSstResetPerfStats")}
void EXPORT grSstIdle( void ) {E("grSstIdle") }

void EXPORT grTexDetailControl( GrChipID_t tmu, int lodBias,
						 FxU8 detailScale, float detailMax ) { E("grTexDetailControl")}
