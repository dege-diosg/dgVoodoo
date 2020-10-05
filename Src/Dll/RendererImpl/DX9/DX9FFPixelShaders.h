/*--------------------------------------------------------------------------------- */
/* DX9FFPixelShaders.h - dgVoodoo general definitions for fixed function            */
/*                       shader-like templates used in DX9 implementation           */
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
/* dgVoodoo: pshaders.h																		*/
/*																							*/
/* Pixel shader definíciók (fixed function multitexturing shaders)							*/
/*------------------------------------------------------------------------------------------*/

#ifndef		PSHADERS_H
#define		PSHADERS_H

#include	<windows.h>
#include	"DDraw.h"
#include	"D3d.h"

/*-----------------------------------------------------------------------------------------	*/
/*.................................. Definíciók a shaderekhez ..............................*/

/* 3 szintû shaderek a color combine mûveletekhez és 2 szintû shaderek az alpha combine mûveletekhez */
/* A shaderek a textúrakiosztást az egyes stage-eken az alábbi módon feltételezik: */

/* Normál textúrázáshoz */
/* -- 0. stage: textúra			-- */
/* -- 1. stage: textúra			-- */
/* -- 2. stage: meghatározatlan -- */

/* AP multitextúrázáshoz */
/* -- 0. stage: color textúra	-- */
/* -- 1. stage: alpha textúra	-- */
/* -- 2. stage: alpha textúra	-- */

/* Pixel shaderekben szereplô speciális mûveletek jelölései */
#define		PS_ALPHAOPBASE			0x40000000
#define		PS_BLENDFACTORALPHA		PS_ALPHAOPBASE			/* a cfactor által jelölt alfa szerinti */
															/* lineáris alphablending */

/* Pixel shaderekben szereplô argumentumok jelölései */
#define		PS_ARGBASE				0x20000000
#define		PS_INVERT				0x40000000
#define		PS_ALPHA				0x80000000

#define		PS_OTHER				(PS_ARGBASE + 0)		/* A cother által megadott argumentum */
#define		PS_LOCAL				(PS_ARGBASE + 1)		/* A clocal által megadott argumentum */
#define		PS_FACTOR				(PS_ARGBASE + 2)		/* A cfactor által megadott argumentum */
#define		PS_AOTHER				(PS_OTHER | PS_ALPHA)	/* Az aother által megadott argumentum */
#define		PS_ALOCAL				(PS_LOCAL | PS_ALPHA)	/* Az alocal által megadott argumentum */
#define		PS_AFACTOR				(PS_FACTOR | PS_ALPHA)	/* Az afactor által megadott argumentum */
#define		PS_INVFACTOR			(PS_FACTOR | PS_INVERT)	/* A cfactor által megadott argumentum invertáltja */
#define		PS_INVAFACTOR			(PS_AFACTOR | PS_INVERT)/* Az afactor által megadott argumentum invertáltja */


/* Shader definíció az alpha combine-hoz */
typedef struct	{

	DWORD alphaop_0, alphaarg1_0, alphaarg2_0;
	DWORD alphaop_1, alphaarg1_1, alphaarg2_1;	

} APixelShader2Stages;


/* Shader definíció a color combine-hoz */
typedef struct	{

	DWORD colorop_0, colorarg1_0, colorarg2_0;
	DWORD colorop_1, colorarg1_1, colorarg2_1;
	DWORD colorop_2, colorarg1_2, colorarg2_2;

} CPixelShader3Stages;

/*-----------------------------------------------------------------------------------------	*/
/*....................................... Shaderek .........................................*/

extern	CPixelShader3Stages		pixelShadersf1[];
extern	CPixelShader3Stages		pixelShadersf0[];
extern	CPixelShader3Stages		pixelShadersfloc[];
extern	CPixelShader3Stages		pixelShadersfa[];
extern	APixelShader2Stages		alphaPixelShaders[];
extern	APixelShader2Stages		alphaPixelShadersf0[];
extern	APixelShader2Stages		alphaPixelShadersf1[];
extern	unsigned char			grfunctranslating[];


#endif