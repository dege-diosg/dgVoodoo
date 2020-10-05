/*--------------------------------------------------------------------------------- */
/* Log.c - Glide implementation, stuffs related to logging                          */
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
/* dgVoodoo: Log.c																			*/
/*			 Glide runtime loggoláshoz kapcsolódó definíciók								*/
/*------------------------------------------------------------------------------------------*/

#include	<stdio.h>
#include	<stdarg.h>
#include	"dgVoodooBase.h"
#include	<glide.h>
#include	"Log.h"

unsigned char	logenabled =  0;

#if			LOGGING

#pragma	message ( "Compiling with Glide-runtime logging" )

char *res_t[]				= {	"GR_RESOLUTION_320x200","GR_RESOLUTION_320x240", "GR_RESOLUTION_400x256",
								"GR_RESOLUTION_512x384", "GR_RESOLUTION_640x200", "GR_RESOLUTION_640x350",
								"GR_RESOLUTION_640x400", "GR_RESOLUTION_640x480", "GR_RESOLUTION_800x600",
								"GR_RESOLUTION_960x720", "GR_RESOLUTION_856x480", "GR_RESOLUTION_512x256",
								"GR_RESOLUTION_1024x768", "GR_RESOLUTION_1280x1024", "GR_RESOLUTION_1600x1200",
								"GR_RESOLUTION_400x300" };

char *ref_t[]				= {	"GR_REFRESH_60Hz", "GR_REFRESH_70Hz", "GR_REFRESH_72Hz",
								"GR_REFRESH_75Hz", "GR_REFRESH_80Hz", "GR_REFRESH_90Hz",
								"GR_REFRESH_100Hz", "GR_REFRESH_85Hz", "GR_REFRESH_120Hz" };

char *cFormat_t[]			= { "GR_COLORFORMAT_ARGB", "GR_COLORFORMAT_ABGR", "GR_COLORFORMAT_RGBA", "GR_COLORFORMAT_BGRA" };

char *locOrig_t[]			= { "GR_ORIGIN_UPPER_LEFT", "GR_ORIGIN_LOWER_LEFT" };

char *evenOdd_t[]			= { NULL, "GR_MIPMAPLEVELMASK_EVEN", "GR_MIPMAPLEVELMASK_ODD", "GR_MIPMAPLEVELMASK_BOTH"};

char *lod_t[]				= {	"GR_LOD_256", "GR_LOD_128", "GR_LOD_64", "GR_LOD_32",
								"GR_LOD_16", "GR_LOD_8", "GR_LOD_4", "GR_LOD_2", "GR_LOD_1" };

char *texbaserange_t[]		= { "GR_TEXBASE_256", "GR_TEXBASE_128", "GR_TEXBASE_64", "GR_TEXBASE_32_TO_1"};

char *aspectr_t[]			= {	"GR_ASPECT_8x1", "GR_ASPECT_4x1", "GR_ASPECT_2x1",
								"GR_ASPECT_1x1", "GR_ASPECT_1x2", "GR_ASPECT_1x4",
								"GR_ASPECT_1x8" };

char *tformat_t[]			= {	"GR_TEXFMT_RGB_332", "GR_TEXFMT_YIQ_422", "GR_TEXFMT_ALPHA_8",
								"GR_TEXFMT_INTENSITY_8", "GR_TEXFMT_ALPHA_INTENSITY_44",
								"GR_TEXFMT_P_8", "GR_TEXFMT_RSVD0", "GR_TEXFMT_RSVD1",
								"GR_TEXFMT_ARGB_8332", "GR_TEXFMT_AYIQ_8422",
								"GR_TEXFMT_RGB_565", "GR_TEXFMT_ARGB_1555",
								"GR_TEXFMT_ARGB_4444", "GR_TEXFMT_ALPHA_INTENSITY_88",
								"GR_TEXFMT_AP_88", "GR_TEXFMT_RSVD2" };

char *combfunc_t[]			= {	"GR_COMBINE_FUNCTION_ZERO","GR_COMBINE_FUNCTION_LOCAL",
								"GR_COMBINE_FUNCTION_LOCAL_ALPHA","GR_COMBINE_FUNCTION_SCALE_OTHER",
								"GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL",
								"GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA",
								"GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL",
								"GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL",
								"GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA",
								"GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL",
								"GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA" };

char *combfact_t[]			= {	"GR_COMBINE_FACTOR_ZERO", "GR_COMBINE_FACTOR_LOCAL",
								"GR_COMBINE_FACTOR_OTHER_ALPHA","GR_COMBINE_FACTOR_LOCAL_ALPHA",
								"GR_COMBINE_FACTOR_TEXTURE_ALPHA","GR_COMBINE_FACTOR_TEXTURE_RGB",
								/*"GR_COMBINE_FACTOR_LOD_FRACTION",*/ "?? Undefined", "?? Undefined",
								"GR_COMBINE_FACTOR_ONE",
								"GR_COMBINE_FACTOR_ONE_MINUS_LOCAL","GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA",
								"GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA","GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA",
								"GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION" };

char *local_t[]				= {	"GR_COMBINE_LOCAL_ITERATED","GR_COMBINE_LOCAL_CONSTANT",
								"GR_COMBINE_LOCAL_DEPTH" };

char *other_t[]				= {	"GR_COMBINE_OTHER_ITERATED","GR_COMBINE_OTHER_TEXTURE",
								"GR_COMBINE_OTHER_CONSTANT" };

char *alphabfnc_srct[]		= { "GR_BLEND_ZERO", "GR_BLEND_SRC_ALPHA", "GR_BLEND_DST_COLOR",
								"GR_BLEND_DST_ALPHA", "GR_BLEND_ONE", "GR_BLEND_ONE_MINUS_SRC_ALPHA",
								"GR_BLEND_ONE_MINUS_DST_COLOR", "GR_BLEND_ONE_MINUS_DST_ALPHA",
								"GR_BLEND_RESERVED_8","GR_BLEND_RESERVED_9","GR_BLEND_RESERVED_A",
								"GR_BLEND_RESERVED_B","GR_BLEND_RESERVED_C","GR_BLEND_RESERVED_D",
								"GR_BLEND_RESERVED_E","GR_BLEND_ALPHA_SATURATE" };

char *alphabfnc_dstt[]		= {	"GR_BLEND_ZERO", "GR_BLEND_SRC_ALPHA", "GR_BLEND_SRC_COLOR",
								"GR_BLEND_DST_ALPHA", "GR_BLEND_ONE", "GR_BLEND_ONE_MINUS_SRC_ALPHA",
								"GR_BLEND_ONE_MINUS_SRC_COLOR", "GR_BLEND_ONE_MINUS_DST_ALPHA",
								"GR_BLEND_RESERVED_8","GR_BLEND_RESERVED_9","GR_BLEND_RESERVED_A",
								"GR_BLEND_RESERVED_B","GR_BLEND_RESERVED_C","GR_BLEND_RESERVED_D",
								"GR_BLEND_RESERVED_E","GR_BLEND_PREFOG_COLOR" };

char *cmpfnc_t[]			= {	"GR_CMP_NEVER","GR_CMP_LESS","GR_CMP_EQUAL","GR_CMP_LEQUAL",
								"GR_CMP_GREATER","GR_CMP_NOTEQUAL","GR_CMP_GEQUAL","GR_CMP_ALWAYS" };

char *locktype_t[]			= {	"GR_LFB_READ_ONLY", "GR_LFB_WRITE_ONLY", "GR_LFB_IDLE", "GR_LFB_NOIDLE" };

char *buffer_t[]			= {	"GR_BUFFER_FRONTBUFFER", "GR_BUFFER_BACKBUFFER", "GR_BUFFER_AUXBUFFER",
								"GR_BUFFER_DEPTHBUFFER", "GR_BUFFER_ALPHABUFFER", "GR_BUFFER_TRIPLEBUFFER" };

char *lfbwritemode_t[]		= {	"GR_LFBWRITEMODE_565", "GR_LFBWRITEMODE_555", "GR_LFBWRITEMODE_1555",
								"GR_LFBWRITEMODE_RESERVED1", "GR_LFBWRITEMODE_888", "GR_LFBWRITEMODE_8888",
								"GR_LFBWRITEMODE_RESERVED2", "GR_LFBWRITEMODE_RESERVED3",
								"GR_LFBWRITEMODE_RESERVED4", "GR_LFBWRITEMODE_RESERVED5",
								"GR_LFBWRITEMODE_RESERVED6", "GR_LFBWRITEMODE_RESERVED7",
								"GR_LFBWRITEMODE_565_DEPTH", "GR_LFBWRITEMODE_555_DEPTH",
								"GR_LFBWRITEMODE_1555_DEPTH", "GR_LFBWRITEMODE_ZA16",
								"GR_LFBWRITEMODE_ANY" };

char *bypassmode_t[]		= { "GR_LFBBYPASS_DISABLE", "GR_LFBBYPASS_ENABLE" };

char *origin_t[]			= {	"GR_ORIGIN_UPPER_LEFT", "GR_ORIGIN_LOWER_LEFT", "GR_ORIGIN_ANY" };

char *textable_t[]			= {	"GR_TEXTABLE_NCC0", "GR_TEXTABLE_NCC1", "GR_TEXTABLE_PALETTE" };

char *ncctable_t[]			= {	"GR_NCCTABLE_NCC0", "GR_NCCTABLE_NCC1" };

char *depthbuffermode_t[]	= { "GR_DEPTHBUFFER_DISABLE", "GR_DEPTHBUFFER_ZBUFFER", "GR_DEPTHBUFFER_WBUFFER",
								"GR_DEPTHBUFFER_ZBUFFER_COMPARE_TO_BIAS", "GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS" };

char *lfbsrcfmt_t[]			= { "GR_LFB_SRC_FMT_565", "GR_LFB_SRC_FMT_555", "GR_LFB_SRC_FMT_1555", "GR_LFB_SRC_FMT_888",
								"GR_LFB_SRC_FMT_8888", "GR_LFB_SRC_FMT_565_DEPTH", "#define GR_LFB_SRC_FMT_555_DEPTH",
								"GR_LFB_SRC_FMT_1555_DEPTH", "GR_LFB_SRC_FMT_ZA16", "GR_LFB_SRC_FMT_RLE16" };

char *colorcombinefunc_t[]	= { "GR_COLORCOMBINE_ZERO", "GR_COLORCOMBINE_CCRGB", "GR_COLORCOMBINE_ITRGB",
								"GR_COLORCOMBINE_ITRGB_DELTA0", "GR_COLORCOMBINE_DECAL_TEXTURE",
								"GR_COLORCOMBINE_TEXTURE_TIMES_CCRGB", "GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB",
								"GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_DELTA0", "GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ALPHA",
								"GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA", "GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA_ADD_ITRGB",
								"GR_COLORCOMBINE_TEXTURE_ADD_ITRGB", "GR_COLORCOMBINE_TEXTURE_SUB_ITRGB",
								"GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA", "GR_COLORCOMBINE_DIFF_SPEC_A",
								"GR_COLORCOMBINE_DIFF_SPEC_B", "GR_COLORCOMBINE_ONE" };

char *alphasource_t[]		= {	"GR_ALPHASOURCE_CC_ALPHA", "GR_ALPHASOURCE_ITERATED_ALPHA", "GR_ALPHASOURCE_TEXTURE_ALPHA",
								"GR_ALPHASOURCE_TEXTURE_ALPHA_TIMES_ITERATED_ALPHA" };

char *dithermode_t[]		= {	"GR_DITHER_DISABLE", "GR_DITHER_2x2", "GR_DITHER_4x4" };

char *cullmode_t[]			= {	"GR_CULL_DISABLE", "GR_CULL_NEGATIVE", "GR_CULL_POSITIVE" };

char *hint_t[]				= {	"GR_HINT_STWHINT", "GR_HINT_FIFOCHECKHINT", "GR_HINT_FPUPRECISION",
								"GR_HINT_ALLOW_MIPMAP_DITHER",
								"GR_HINT_LFB_WRITE", "GR_HINT_LFB_PROTECT", "GR_HINT_LFB_RESET" };

char *sstmode_t[]			= { "GR_CONTROL_ACTIVATE", "GR_CONTROL_DEACTIVATE", "GR_CONTROL_RESIZE", "GR_CONTROL_MOVE" };

char *fogmode_t[]			= { "GR_FOG_DISABLE", "GR_FOG_WITH_ITERATED_ALPHA", "GR_FOG_WITH_TABLE", "GR_FOG_WITH_ITERATED_Z" };

char *fogmodemod_t[]		= {	"0", "GR_FOG_MULT2", "GR_FOG_ADD2" };

char *fxbool_t[]			= {	"FXFALSE","FXTRUE" };

char *chromakeymode_t[]		= {	"GR_CHROMAKEY_DISABLE", "GR_CHROMAKEY_ENABLE" };

char *d3dzbuffmode_t[]		= { "D3DZB_FALSE","D3DZB_TRUE","D3DZB_USEW"};

char *mipmapmode_t[]		= { "GR_MIPMAP_DISABLE", "GR_MIPMAP_NEAREST",  "GR_MIPMAP_DITHER" };

char *clampmode_t[]			= { "GR_TEXTURECLAMP_WRAP", "GR_TEXTURECLAMP_CLAMP" };

char *filtermode_t[]		= { "GR_TEXTUREFILTER_POINT_SAMPLED", "GR_TEXTUREFILTER_BILINEAR" };

char *texturecombinefnc_t[] = { "GR_TEXTURECOMBINE_ZERO", "GR_TEXTURECOMBINE_DECAL", "GR_TEXTURECOMBINE_OTHER",
								"GR_TEXTURECOMBINE_ADD", "GR_TEXTURECOMBINE_MULTIPLY", "GR_TEXTURECOMBINE_SUBTRACT",
								"GR_TEXTURECOMBINE_DETAIL", "GR_TEXTURECOMBINE_DETAIL_OTHER", "GR_TEXTURECOMBINE_TRILINEAR_ODD",
								"GR_TEXTURECOMBINE_TRILINEAR_EVEN", "GR_TEXTURECOMBINE_ONE" };


void __cdecl LOG(int flush, char *message, ... )	{
FILE	*loghandle;
    	
	va_list(arg);
    va_start(arg, message);

	if (logenabled || flush)	{
		loghandle = fopen("dgVoodoo.log","a");
		if (loghandle!=NULL)	{
			vfprintf(loghandle,message,arg);
			fclose(loghandle);
		}
	}
	va_end(arg);
}

#endif

/*void __cdecl QLOG(char *message, ... )	{
    	
	va_list(arg);
    va_start(arg, message);

	loghandle=fopen("dgVoodoo.log","a");
	if (loghandle!=NULL)	{
		vfprintf(loghandle,message,arg);
		fclose(loghandle);
	}
	va_end(arg);
}*/
