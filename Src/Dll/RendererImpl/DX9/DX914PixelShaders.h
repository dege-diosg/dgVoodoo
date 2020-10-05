/*--------------------------------------------------------------------------------- */
/* DX914PixelShaders.h - dgVoodoo general definitions for pixel shader (ps 1.4)     */
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
/* dgVoodoo: DX914PixelShaders.h															*/
/*																							*/
/* PS 1.4 definíciók																		*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DX914PSHADERS_H
#define		DX914PSHADERS_H

#include	<windows.h>
#include	"D3d9Types.h"

extern "C" {

#include	"Debug.h"

}

#ifdef		DEBUG

#define		DISASMSHADER	1

#else

#define		DISASMSHADER	0

#endif

/*-----------------------------------------------------------------------------------------	*/
/*.................................. Definíciók a shaderekhez ..............................*/

//Konstansregiszterek kiosztása
//	c0	-	constant color
//	c1	-	-(constant color)		(negated)
//	c2	-	1-(constant color)		(inverted)
//	c3	-	 0.0
//	c4	-	 1.0
//  c5	-	-1.0
//	c6	-	colorkey
//  c7  -	alpha reference value

#define		PSVERSION_14			0xFFFF0104

/* Pixel shaderekben szereplô regiszterek jelölései */
#define		PSSR_V(n)				((n & 0x7FF) | ((D3DSPR_INPUT & 0x7) << 28) | ((D3DSPR_INPUT & 0x18) << 8) | 0x80E40000)
#define		PSSR_C(n)				((n & 0x7FF) | ((D3DSPR_CONST & 0x7) << 28) | ((D3DSPR_CONST & 0x18) << 8) | 0x80E40000)
#define		PSSR_T(n)				((n & 0x7FF) | ((D3DSPR_TEXTURE & 0x7) << 28) | ((D3DSPR_TEXTURE & 0x18) << 8) | 0x80E40000)
#define		PSSR_R(n)				((n & 0x7FF) | ((D3DSPR_TEMP & 0x7) << 28) | ((D3DSPR_TEMP & 0x18) << 8) | 0x80E40000)

#define		PSDR_R(n)				((n & 0x7FF) | ((D3DSPR_TEMP & 0x7) << 28) | ((D3DSPR_TEMP & 0x18) << 8) | 0x80000000)
#define		PSDR_C(n)				((n & 0x7FF) | ((D3DSPR_CONST & 0x7) << 28) | ((D3DSPR_CONST & 0x18) << 8) | 0x80000000)

#define		PSR_TEMPLATE			0x00008000
#define		PSR_TEXALPHATEMPLATE	0x00004000				//Spec: colorCombine factorhoz, az texCombine alpha template
															//táblázatát kell használni


#define		GetRegNumber(reg)		((reg) & 0xFF)
#define		GetRegType(reg)			((reg >> 28) & 0x7)
#define		GetRegSwizzle(reg)		(reg & 0x00FF0000)
#define		GetRegTypeAndNumber(reg)	(reg & 0x700000FF)

enum PSSRegModifier {
	
	PSRM_None			=	0,
	PSRM_Negate			=	0x01000000,
	PSRM_Complement		=	0x06000000,
	PSRM_AlphaSwizzle	=	0x00FF0000,
	
	PSRM_RSwizzle		=	0x00000000,
};


enum PSDRegModifier {

	PDRM_Sat			=	0x00100000
};


enum PSInstrModifier {

	PSIM_CoIssue		=	0x40000000

};


enum PDRegWriteMask {

	PDWM_RGB			=	0x00070000,
	PDWM_Alpha			=	0x00080000,
	PDWM_RGBA			=	0x000F0000,

	PDWM_YZW			=	0x000E0000
};

#define		PSREGMODIFIERMASK		0x0FFF0000


enum PSRegTemplateIndex	{

	CLocal		=	0,
	ALocal,
	NegCLocal,
	COther,
	Factor,
	InvFactor,
	NumOfTemplateRegs

};


#define		PSSR_CLOCAL				(CLocal | PSR_TEMPLATE)
#define		PSSR_ALOCAL				(ALocal | PSR_TEMPLATE)
#define		PSSR_NEGCLOCAL			(NegCLocal | PSR_TEMPLATE)
#define		PSSR_COTHER				(COther | PSR_TEMPLATE)
#define		PSSR_FACTOR				(Factor | PSR_TEMPLATE)
#define		PSSR_INVFACTOR			(InvFactor | PSR_TEMPLATE)

//Csak Texture combine eseten a texld utasitasokhoz
#define		PSDR_CLOCAL				(CLocal | PSR_TEMPLATE)
#define		PSDR_ALOCAL				(ALocal | PSR_TEMPLATE)
#define		PSDR_ACLOCAL			(NumOfTemplateRegs | PSR_TEMPLATE)		//texld után alkalmazva 2 texld utasítást generál
																			// = (texld PSDR_CLOCAL, t0; texld PSDR_ALOCAL, t0;)

#define		MAX_14SHADER_LENGTH		2

/* Shader tokenek */
typedef struct	{

	DWORD	instruction, dstReg;
	DWORD	srcRegs[3];

} ShaderTokens;


/*-----------------------------------------------------------------------------------------	*/
/*....................................... Shaderek .........................................*/

extern	DWORD					localRegLookUp[];
extern	DWORD					otherRegLookUp[];

extern	DWORD					tFactorRegLookUp[];
extern	char					tFuncShaderTableLookUp[];

extern	unsigned char			funcToShaderIndex[];


#if		DISASMSHADER

extern	void					DisAsmShader (DWORD *pShader);

#else

#define							DisAsmShader(pShader)

#endif

#endif