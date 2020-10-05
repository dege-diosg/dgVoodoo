/*--------------------------------------------------------------------------------- */
/* DX914PixelShaders.cpp - dgVoodoo general definitions for pixel shader (ps 1.4)   */
/*                       templates used in DX9 implementation                       */
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
/* dgVoodoo: DX914PixelShaders.cpp															*/
/*			 PS 1.4 definíciók 																*/
/*------------------------------------------------------------------------------------------*/
#include "dgVoodooBase.h"
#include "DX9General.hpp"

extern "C" {

#include "Log.h"

}


#if		DISASMSHADER

#include <stdio.h>

#endif

unsigned char			funcToShaderIndex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 10};

DWORD					localRegLookUp[] = {PSSR_V(0), PSSR_C(0), PSSR_C(3), PSSR_C(3)};
DWORD					otherRegLookUp[] = {PSSR_V(0), PSSR_COTHER, PSSR_C(0)};

DWORD					tFactorRegLookUp[] = {PSSR_C(3), PSSR_R(0), PSSR_C(3), PSSR_R(0), PSSR_R(0),
											  PSSR_C(3), PSSR_C(3), PSSR_C(3), PSSR_C(3), PSSR_R(0) | PSRM_Complement,
											  PSSR_C(3), PSSR_R(0) | PSRM_Complement, PSSR_R(0) | PSRM_Complement,
											  PSSR_C(3)};

char					tFuncShaderTableLookUp[] = {0, 2, 2, 2, 2, 2, 0, 0, 0, 1, 2, 2, 2, 2, 2};

/*................................. ColorCombine, AlphaCombine .............................*/

/* PS 1.4 arra az esetre, amikor f=1 */
ShaderTokens			DX9General::cShaderTokens14_f1[] =
						{
						/* zero */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_COTHER,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_ADD,	PSDR_R(0),	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_ADD,	PSDR_R(0),	PSSR_COTHER,	PSSR_ALOCAL,	0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*(cother-clocal) */
							{D3DSIO_SUB,	PSDR_R(0),	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_COTHER,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_SUB,	PSDR_R(0),	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_ADD,	PSDR_R(0),	PSSR_R(0),		PSSR_ALOCAL,	0},

						/*  (1-f)*clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_SUB,	PSDR_R(0),	PSSR_ALOCAL,	PSSR_CLOCAL,	0},
							{D3DSIO_END,	0,			0,				0,				0}
						};


/* PS 1.4 arra az esetre, amikor f=0 */
ShaderTokens			DX9General::cShaderTokens14_f0[] =
						{
						/* zero */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*(cother-clocal) */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  (1-f)*clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0}
						};


/* PS 1.4 */
ShaderTokens			DX9General::cShaderTokens14_f[] =
						{
						/* zero */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_C(3),		0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* clocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_CLOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* alocal */
							{D3DSIO_MOV,	PSDR_R(0),	PSSR_ALOCAL,	0,				0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother */
							{D3DSIO_MUL,	PSDR_R(0),	PSSR_FACTOR,	PSSR_COTHER,	0},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_MAD,	PSDR_R(0),	PSSR_FACTOR,	PSSR_COTHER,	PSSR_CLOCAL},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_MAD,	PSDR_R(0),	PSSR_FACTOR,	PSSR_COTHER,	PSSR_ALOCAL},
							{D3DSIO_END,	0,			0,				0,				0},

						/* f*(cother-clocal) */
							{D3DSIO_SUB,	PSDR_R(3),	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_MUL,	PSDR_R(0),	PSSR_R(3),		PSSR_FACTOR,	0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_LRP,	PSDR_R(0),	PSSR_FACTOR,	PSSR_COTHER,	PSSR_CLOCAL},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_SUB,	PSDR_R(3),	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_MAD,	PSDR_R(0),	PSSR_R(3),		PSSR_FACTOR,	PSSR_ALOCAL},

						/*  (1-f)*clocal */
							{D3DSIO_MUL,	PSDR_R(0),	PSSR_INVFACTOR,	PSSR_CLOCAL,	0},
							{D3DSIO_END,	0,			0,				0,				0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_MAD,	PSDR_R(0),	PSSR_FACTOR,	PSSR_NEGCLOCAL,	PSSR_ALOCAL},
							{D3DSIO_END,	0,			0,				0,				0},
						};


/*.............................. TexColorCombine, TexAlphaCombine ..........................*/

// cother = 0, mivel 1 tmu-t támogatunk

/* PS 1.4 arra az esetre, amikor f=1 */
ShaderTokens			DX9General::tShaderTokens14_f1[] =
						{
						/* zero */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},
						
						/* clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*cother */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*(cother-clocal) */
						/*   az eredmény -clocal lenne, de a szaturáció miatt mindig 0 lesz */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ACLOCAL,			0,				0,				0},
							{D3DSIO_ADD,	PSDR_R(0) | PDRM_Sat,	PSSR_NEGCLOCAL,	PSSR_ALOCAL,	0},

						/*  (1-f)*clocal */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ACLOCAL,			0,				0,				0},
							{D3DSIO_ADD,	PSDR_R(0) | PDRM_Sat,	PSSR_NEGCLOCAL,	PSSR_ALOCAL,	0}
						};


						/* PS 1.4 arra az esetre, amikor f=0 */
ShaderTokens			DX9General::tShaderTokens14_f0[] =
						{
						/* zero */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},
						
						/* clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*cother */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*(cother-clocal) */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/*  (1-f)*clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0}
						};


						/* PS 1.4 */
ShaderTokens			DX9General::tShaderTokens14_f[] =
						{
						/* zero */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},
						
						/* clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*cother */
							{D3DSIO_END,	PSSR_C(3),				0,				0,				0},
							{D3DSIO_END,	0,						0,				0,				0},

						/* f*cother+clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_CLOCAL,			0,				0,				0},

						/* f*cother+alocal */
							{D3DSIO_TEX,	PSDR_ALOCAL,			0,				0,				0},
							{D3DSIO_END,	PSSR_ALOCAL,			0,				0,				0},

						/* f*(cother-clocal) */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_MUL,	PSDR_R(0) | PDRM_Sat,	PSSR_FACTOR,	PSSR_NEGCLOCAL,	0},

						/* f*cother+(1-f)*clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_MUL,	PSDR_R(0) | PDRM_Sat,	PSSR_INVFACTOR,	PSSR_CLOCAL,	0},

						/*  f*(cother-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ACLOCAL,			0,				0,				0},
							{D3DSIO_MAD,	PSDR_R(0) | PDRM_Sat,	PSSR_FACTOR,	PSSR_NEGCLOCAL,	PSSR_ALOCAL},

						/*  (1-f)*clocal */
							{D3DSIO_TEX,	PSDR_CLOCAL,			0,				0,				0},
							{D3DSIO_MUL,	PSDR_R(0) | PDRM_Sat,	PSSR_INVFACTOR,	PSSR_CLOCAL,	0},

						/*  f*(-clocal)+alocal */
							{D3DSIO_TEX,	PSDR_ACLOCAL,			0,				0,				0},
							{D3DSIO_MAD,	PSDR_R(0) | PDRM_Sat,	PSSR_FACTOR,	PSSR_NEGCLOCAL,	PSSR_ALOCAL}
						};


/*...................................... Chroma key shader .................................*/

// sub		r3.rgb, cother, clocal
// cmp		r3.rgb, r3,	r3, -r3
// dp3_sat	r3.rgb, r3, c4
// cmp		r3.rgb, -r3, c5
// mov		r5.rgb, alocal
// phase
// texkill	r3
ShaderTokens			DX9General::chromaShaderTokens14[] =
						{
							{D3DSIO_SUB,	PSDR_R(3) | PDWM_RGB,	PSSR_COTHER,	PSSR_CLOCAL,	0},
							{D3DSIO_CMP,	PSDR_R(3) | PDWM_RGB,	PSSR_R(3),		PSSR_R(3),				    PSSR_R(3) | PSRM_Negate},
							{D3DSIO_DP3,	PSDR_R(3) | PDWM_RGB  | PDRM_Sat,	PSSR_R(3),		PSSR_C(4),					0},
							{D3DSIO_CMP,	PSDR_R(3) | PDWM_RGB,	PSSR_R(3) | PSRM_Negate,		PSSR_C(5),					PSSR_C(3),},
							{D3DSIO_MOV,	PSDR_R(5) | PDWM_RGB,	PSSR_ALOCAL,	0,							0},
							{D3DSIO_PHASE,	0,						0,				0,							0},
							{D3DSIO_TEXKILL,PSDR_R(3) | PDWM_RGB | PDWM_Alpha,		0,							0,							0},
							{D3DSIO_END,	0,						0,				0,							0}
						};


ShaderTokens			DX9General::chromaShaderTokens14_noAlphaSave[] =
						{
							{D3DSIO_SUB,	PSDR_R(2) | PDWM_RGB,	PSSR_COTHER,	PSSR_CLOCAL/*PSSR_C(6)*/,	0},
							{D3DSIO_CMP,	PSDR_R(2) | PDWM_RGB,	PSSR_R(2),		PSSR_R(2),		PSSR_R(2) | PSRM_Negate},
							{D3DSIO_DP3,	PSDR_R(2) | PDWM_RGB  | PDRM_Sat,	PSSR_R(2),		PSSR_C(4),					0},
							{D3DSIO_CMP,	PSDR_R(2) | PDWM_RGB,	PSSR_R(2) | PSRM_Negate,		PSSR_C(5),					PSSR_C(3),},
							{D3DSIO_PHASE,	0,						0,				0,							0},
							{D3DSIO_TEXKILL,PSDR_R(2) | PDWM_RGB | PDWM_Alpha,		0,							0,							0},
							{D3DSIO_END,	0,						0,				0,							0}
						};


ShaderTokens			DX9General::chromaShaderTokensAlphaBased14[] =
						{
							{D3DSIO_SUB,	PSDR_R(3) | PDWM_RGB,	PSSR_CLOCAL,	PSSR_C(7) | PSRM_AlphaSwizzle,	0},
							{D3DSIO_MOV,	PSDR_R(5) | PDWM_RGB,	PSSR_ALOCAL,	0,							0},
							{D3DSIO_PHASE,	0,						0,				0,							0},
							{D3DSIO_TEXKILL,PSDR_R(3) | PDWM_RGB | PDWM_Alpha,		0,							0,							0},
							{D3DSIO_END,	0,						0,				0,							0}
						};


ShaderTokens			DX9General::chromaShaderTokensAlphaBasedNoAlphaSave14[] =
						{
							{D3DSIO_SUB,	PSDR_R(3) | PDWM_RGB,	PSSR_CLOCAL,	PSSR_C(7) | PSRM_AlphaSwizzle,	0},
							{D3DSIO_PHASE,	0,						0,				0,							0},
							{D3DSIO_TEXKILL,PSDR_R(3) | PDWM_RGB | PDWM_Alpha,		0,							0,							0},
							{D3DSIO_END,	0,						0,				0,							0}
						};

/*...................................... Alpha ITRGB shader ................................*/

ShaderTokens			DX9General::alphaItRGBShaderTokens[] =
						{
							{D3DSIO_CND,	PSDR_R(2) | PDWM_RGB,	PSSR_COTHER,	PSSR_C(0),			PSSR_V(0)},
							//{D3DSIO_LRP,	PSDR_R(2) | PDWM_RGB,	PSSR_COTHER,	PSSR_V(0),			PSSR_C(0)},
							{D3DSIO_END,	0,						0,				0,							0}
						};

/*..........................................................................................*/
/*................................. LFB-textúracsempézõ shaderek ...........................*/

DWORD					DX9General::lfbTextureTileShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_END
						};


DWORD					DX9General::lfbTextureTileConstAlphaShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_MOV,		PSDR_R(0) | PDWM_Alpha,					PSSR_C(0),
							D3DSIO_END
						};


DWORD					DX9General::lfbTextureTileWithColorKeyShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_TEX,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_SUB,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_R(0),	PSSR_R(1),
							D3DSIO_CMP,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_R(1),	PSSR_R(1),	PSSR_R(1)  | PSRM_Negate,
							D3DSIO_DP4,		PSDR_R(1) | PDWM_RGBA | PDRM_Sat,		PSSR_R(1),	PSSR_C(4),
							D3DSIO_CMP,		PSDR_R(1) | PDWM_RGB,					PSSR_R(1) | PSRM_Negate,	PSSR_C(5),					PSSR_C(3),
							D3DSIO_MOV,		PSDR_R(2) | PDWM_RGB,					PSSR_R(0) | PSRM_AlphaSwizzle,
							D3DSIO_PHASE,
							D3DSIO_TEXKILL, PSDR_R(1) | PDWM_RGB | PDWM_Alpha,
							D3DSIO_MOV,		PSDR_R(0) | PDWM_Alpha,					PSSR_R(2) & ~PSRM_AlphaSwizzle,
							D3DSIO_END
						};


DWORD					DX9General::lfbTextureTileConstAlphaWithColorKeyShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_TEX,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_T(0),
							D3DSIO_SUB,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_R(0),	PSSR_R(1),
							D3DSIO_CMP,		PSDR_R(1) | PDWM_RGB | PDWM_Alpha,		PSSR_R(1),	PSSR_R(1),	PSSR_R(1) | PSRM_Negate,
							D3DSIO_DP4,		PSDR_R(1) | PDWM_RGBA | PDRM_Sat,		PSSR_R(1),	PSSR_C(4),
							D3DSIO_CMP,		PSDR_R(1) | PDWM_RGB,					PSSR_R(1) | PSRM_Negate,	PSSR_C(5),					PSSR_C(3),
							D3DSIO_PHASE,
							D3DSIO_TEXKILL, PSDR_R(1) | PDWM_RGB | PDWM_Alpha,
							D3DSIO_MOV,		PSDR_R(0) | PDWM_Alpha,					PSSR_C(0),
							D3DSIO_END
						};

/*................................. ScreenShot Label shader ...........................*/

DWORD					DX9General::scrShotShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGBA,					PSSR_T(0),
							D3DSIO_END
						};

/*................................. Gamma Correction shader ...........................*/

DWORD					DX9General::gammaCorrShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_MOV,		PSDR_R(0) | PDWM_RGBA,					PSSR_V(0),
							D3DSIO_END
						};


/*..................................... Paletter shader ...............................*/

//  ps_1_4
//	def		c0, 0.0f, 0.0f, 0.0f, 1.0f-(1.0f/256.0f)
//	def		c1, 0.0f, 0.0f, 0.0f, (1.0f/2*256.0f)
//  texld	r0, t0
//  mov		r2, r0.a
//	mad		r0.rgb, r0, c0.a, c1.a
//  phase
//  texld	r1, r0
//  mov		r0.rgb, r1
// +mov		r0.a, r2.r
const	float	pS_c0rgb = 0.0f;
const	float	pS_c0a = 1.0f-(1.0f/256.0f);
const	float	pS_c1a = 1.0f/(2*256.0f);
DWORD					DX9General::paletteShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_DEF,		PSDR_C(0) | PDWM_RGBA,	*((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c0a),
							D3DSIO_DEF,		PSDR_C(1) | PDWM_RGBA,	*((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c0rgb), *((DWORD*)&pS_c1a),
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGBA,					PSSR_T(0),
							D3DSIO_MOV,		PSDR_R(2) | PDWM_RGBA,					PSSR_R(0) | PSRM_AlphaSwizzle,
							D3DSIO_MAD,		PSDR_R(0) | PDWM_RGB,					PSSR_R(0),		PSSR_C(0) | PSRM_AlphaSwizzle, PSSR_C(1) | PSRM_AlphaSwizzle,
							D3DSIO_PHASE,
							D3DSIO_TEX,		PSDR_R(1) | PDWM_RGBA,					PSSR_R(0),
							D3DSIO_MOV,		PSDR_R(0) | PDWM_RGB,					PSSR_R(1),
							D3DSIO_MOV | PSIM_CoIssue,		PSDR_R(0) | PDWM_Alpha,					(PSSR_R(2) & ~PSRM_AlphaSwizzle) | PSRM_RSwizzle,
							D3DSIO_END
						};


/*..........................................................................................*/
/*............................... Fekete-fehérré konvertáló shader .........................*/

DWORD					DX9General::grayScaleShaderTokens[] =
						{
							PSVERSION_14,
							D3DSIO_TEX,		PSDR_R(0) | PDWM_RGBA,					PSSR_T(0),
							D3DSIO_DP3,		PSDR_R(0) | PDWM_RGB  | PDRM_Sat,		PSSR_R(0),	PSSR_C(0),
							D3DSIO_END
						};


#if		DISASMSHADER

void	DisAsmShader (DWORD* pShader)
{
	int	dstNum;
	int	srcNum;
	if (*pShader == PSVERSION_14) {
		DEBUGLOG(2, "\nps_1_4\n");
		pShader++;
	}
	
	char outputLine[256];
	char* instr;
	
	while (*pShader != D3DSIO_END) {
		dstNum = srcNum = 0;
		outputLine[0] = 0;
		strcat (outputLine, (*pShader & PSIM_CoIssue) ? "\n+" : "\n ");

		switch (*pShader & 0xFFFF) {
			case D3DSIO_TEX:
				dstNum = 1;
				srcNum = 1;
				instr = "texld";
				break;

			case D3DSIO_CMP:
				dstNum = 1;
				srcNum = 3;
				instr = "cmp";
				break;

			case D3DSIO_CND:
				dstNum = 1;
				srcNum = 3;
				instr = "cnd";
				break;

			case D3DSIO_DP3:
				dstNum = 1;
				srcNum = 2;
				instr = "dp3";
				break;

			case D3DSIO_DP4:
				dstNum = 1;
				srcNum = 2;
				instr = "dp4";
				break;

			case D3DSIO_PHASE:
				instr = "phase";
				break;

			case D3DSIO_TEXKILL:
				dstNum = 1;
				instr = "texkill";
				break;

			case D3DSIO_MOV:
				dstNum = 1;
				srcNum = 1;
				instr = "mov";
				break;

			case D3DSIO_ADD:
				dstNum = 1;
				srcNum = 2;
				instr = "add";
				break;

			case D3DSIO_SUB:
				dstNum = 1;
				srcNum = 2;
				instr = "sub";
				break;

			case D3DSIO_MUL:
				dstNum = 1;
				srcNum = 2;
				instr = "mul";
				break;

			case D3DSIO_MAD:
				dstNum = 1;
				srcNum = 3;
				instr = "mad";
				break;

			case D3DSIO_LRP:
				dstNum = 1;
				srcNum = 3;
				instr = "lrp";
				break;
			
			default:
				instr = "";	
				sprintf (outputLine, "???(%0x)", *pShader);
				break;
		}
		pShader++;
		/* dst reg */
		if (dstNum) {
			char* modifier = (*pShader & PDRM_Sat) ? "_sat" : "";
			int spaceFillLen = 12 - (strlen (instr) + strlen (modifier));
			char fullInstr[256];
			sprintf (fullInstr, "%s%s", instr, modifier);
			strcat (outputLine, fullInstr);
			for (; spaceFillLen > 0; spaceFillLen--) {
				strcat (outputLine, " ");
			}
			DEBUGLOG (2, outputLine);

			switch (GetRegType (*pShader)) {
				case D3DSPR_TEMP:
					DEBUGLOG (2,"r");
					break;

				case D3DSPR_TEXTURE:
					DEBUGLOG (2,"t");
					break;

				case D3DSPR_CONST:
					DEBUGLOG (2,"c");
					break;

				case D3DSPR_INPUT:
					DEBUGLOG (2,"v");
					break;
			}
			DEBUGLOG(2, "%d", GetRegNumber (*pShader));
			switch ((*pShader) & 0xF0000) {
				case PDWM_RGB:
					DEBUGLOG (2,".rgb");
					break;

				case PDWM_Alpha:
					DEBUGLOG (2,".a");
					break;

				case PDWM_RGB | PDWM_Alpha:
					break;

				case 0:
					DEBUGLOG (2,".empty_writemask");
					break;

				default:
					DEBUGLOG (2,".unused_writemask");
					break;
			}
			pShader++;
		}
		/* src regs*/
		for (int i = 0; i < srcNum; i++) {
			if ((i != 0) || dstNum)
				DEBUGLOG (2,", ");
			else
				DEBUGLOG (2,"     ");

			switch ((*pShader) & 0xF000000) {
				case PSRM_Negate:
					DEBUGLOG (2, "-");
					break;

				case PSRM_Complement:
					DEBUGLOG (2, "1-");
					break;

				default:
					break;
			}
			switch (GetRegType (*pShader)) {
				case D3DSPR_TEMP:
					DEBUGLOG (2,"r");
					break;

				case D3DSPR_TEXTURE:
					DEBUGLOG (2,"t");
					break;

				case D3DSPR_CONST:
					DEBUGLOG (2,"c");
					break;

				case D3DSPR_INPUT:
					DEBUGLOG (2,"v");
					break;
			}
			DEBUGLOG(2, "%d", GetRegNumber (*pShader));
			switch ((*pShader) & 0xFF0000) {
				case PSRM_AlphaSwizzle:
					DEBUGLOG (2,".a");
					break;

				case PSRM_RSwizzle:
					DEBUGLOG (2,".r");
					break;

				case 0x00E40000:
					break;

				default:
					DEBUGLOG (2,".unused_swizzle");
					break;
			}
			pShader++;
		}
	}
	DEBUGLOG (2,"\n");
}

#endif
