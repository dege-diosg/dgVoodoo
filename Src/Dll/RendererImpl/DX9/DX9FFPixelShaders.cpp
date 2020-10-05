/*--------------------------------------------------------------------------------- */
/* DX9FFPixelShaders.cpp - dgVoodoo general definitions for fixed function          */
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
/* dgVoodoo: DX9FFPixelShaders.cpp															*/
/*			 Pixel shader definíciók (fixed function)										*/
/*------------------------------------------------------------------------------------------*/
#include "DX9General.hpp"

unsigned char			DX9General::grfunctranslating[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 10};

/* 3 stage-es pixelshaderek arra az esetre, amikor f=1 */
CPixelShader3Stages		DX9General::pixelShadersf1[] =
						{	
						/* zero, imp: zero OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* clocal, imp: clocal OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* alocal, imp: alocal OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother, imp: 1*other, OK */
						{D3DTOP_SELECTARG1,	PS_OTHER,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* f*cother+clocal, imp: 1*cother+clocal, OK */
						{D3DTOP_ADD,		PS_OTHER,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother+alocal, imp: 1*cother+alocal, OK */
						{D3DTOP_ADD,		PS_OTHER,		PS_ALOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						 

						/* f*(cother-clocal), imp: 1*(cother-clocal), OK */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother+(1-f)*clocal, imp: 1*cother+0*clocal, OK */
						{D3DTOP_SELECTARG1, PS_OTHER,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						 /* f*(cother-clocal)+alocal, imp: 1*(cother-clocal)+alocal, OK */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_ALOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 						 
						/* (1-f)*clocal, imp: 0*clocal, OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,							
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(-clocal)+alocal, imp: alocal-1*(clocal), OK */
						{D3DTOP_ADD,		PS_ALOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0}
						};

/* 3 stage-es pixelshaderek arra az esetre, amikor f=0 */
CPixelShader3Stages		DX9General::pixelShadersf0[] =
						{	
						/* zero, imp: zero, OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* clocal, imp: clocal, OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* alocal, imp: alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother, imp: 0*cother, OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* f*cother+clocal, imp: 0*cother+clocal, OK */						
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother+alocal, imp: 0*cother+alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(cother-clocal), imp: 0*(cother-clocal), OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother+(1-f)*clocal, imp: 0*cother+1*clocal, OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* f*(cother-clocal)+alocal, 0*(cother-clocal)+alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* (1-f)*clocal, imp: 1*clocal, OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(-clocal)+alocal, imp: 0*(-clocal)+alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						};

/* 3 stage-es pixelshaderek arra az esetre, amikor f=clocal vagy f=(1-clocal) vagy f=texture_rgb */
/* Megjegyz: az f=texture_rgb lehetõség a glide.h-ban definiálva van, azonban a dokumentáció */
/* nem említi. Az egyes stage-ek textúrakiosztásai miatt AP multitextúrázás esetén jelenleg */
/* nem is mûködik. */
CPixelShader3Stages		DX9General::pixelShadersfloc[] =
						{	
						/* zero, imp: zero, OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* clocal, imp: clocal, OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* alocal, imp: alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother, imp: f*cother, OK */
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						 

						/* f*cother+clocal, imp: f*cother+clocal, OK */
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother+alocal, imp: f*cother+alocal,  OK */
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_ALOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* f*(cother-clocal), imp: f*(cother-clocal), OK */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_SELECTARG1,	D3DTA_CURRENT,	0,
						 D3DTOP_MODULATE,	PS_FACTOR,		D3DTA_CURRENT},

						/* f*cother+(1-f)*clocal, imp: f*cother+clocal, APPROX */
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_ADD,		PS_LOCAL,		D3DTA_CURRENT,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(cother-clocal)+alocal, imp: f*(cother-clocal)+alocal, OK */
						/* Ez a shader multitextúrázás és factor=texture_rgb esetén nem jól mûködik! */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_MODULATE,	D3DTA_CURRENT,	PS_FACTOR,
						 D3DTOP_ADD,		PS_ALOCAL,		D3DTA_CURRENT},

						/* (1-f)*clocal, imp: (1-f)*clocal, OK */
						{D3DTOP_MODULATE,	PS_INVFACTOR,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(-clocal)+alocal, imp: alocal-f*(clocal), OK */
						{D3DTOP_MODULATE,	PS_FACTOR,		PS_LOCAL,
						 D3DTOP_SUBTRACT,	PS_ALOCAL,		D3DTA_CURRENT,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0}
						};
						 

/* 3 stage-es pixelshaderek arra az esetre, amikor f=alfa vagy f=1-alfa */
CPixelShader3Stages		DX9General::pixelShadersfa[] =
						{	
						/* zero, imp: zero, OK */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* clocal, imp: clocal, OK */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* alocal, imp: alocal, OK */
						{D3DTOP_SELECTARG1,	PS_ALOCAL,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*cother, imp: f*other, OK */
						{D3DTOP_SELECTARG1,	PS_OTHER,		D3DTA_CURRENT,
						 D3DTOP_MODULATE,	PS_AFACTOR,		D3DTA_CURRENT,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* f*cother+clocal, imp: f*cother+clocal, OK */
						{D3DTOP_SELECTARG1,	PS_OTHER,		D3DTA_CURRENT,
						 D3DTOP_MODULATE,	PS_AFACTOR,		D3DTA_CURRENT,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_LOCAL},

						/* f*cother+alocal, imp: f*cother+alocal, OK */ 
						{D3DTOP_SELECTARG1,	PS_OTHER,		D3DTA_CURRENT,
						 D3DTOP_MODULATE,	PS_AFACTOR,		D3DTA_CURRENT,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_ALOCAL},

						/* f*(cother-clocal), imp: f*(cother-clocal), OK */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_MODULATE,	PS_AFACTOR,		D3DTA_CURRENT,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* f*cother+(1-f)*clocal, imp: f*cother+(1-f)*clocal, OK */
						{D3DTOP_SELECTARG1,	PS_OTHER,		0,
						 PS_BLENDFACTORALPHA,D3DTA_CURRENT,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(cother-clocal)+alocal, imp: f*(cother-clocal)+alocal, OK */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_MODULATE,	PS_AFACTOR,		D3DTA_CURRENT,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_ALOCAL},

						/* (1-f)*clocal, imp: (1-f)*clocal, OK */
						{D3DTOP_SELECTARG1,	PS_LOCAL,		0,
						 D3DTOP_MODULATE,	PS_INVAFACTOR,	D3DTA_CURRENT,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* f*(-clocal)+alocal, imp: alocal-f*(clocal), OK */
						{D3DTOP_SELECTARG1,	PS_LOCAL,		0,
						 D3DTOP_MODULATE,	PS_FACTOR,		D3DTA_CURRENT,
						 D3DTOP_SUBTRACT,	PS_ALOCAL,		D3DTA_CURRENT}
						};						

/*-------------------------------------------------------------------------------------------*/

/* 2 stage-es alfa pixelshaderek arra az esetre, amikor f nem 1 vagy 0 */
APixelShader2Stages		DX9General::alphaPixelShaders[] =
						{	
						/* GR_COMBINE_FUNCTION_ZERO */
						/* zero */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FACTOR_LOCAL */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* GR_COMBINE_FACTOR_LOCAL_ALPHA */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							 
						/* GR_COMBINE_FUNCTION_SCALE_OTHER */
						/* f * aother */
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL */
						/* f*aother+alocal*/
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_LOCAL},
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA */
						/* f*aother+alocal*/
						{D3DTOP_MODULATE,	PS_OTHER,		PS_FACTOR,
						 D3DTOP_ADD,		D3DTA_CURRENT,	PS_LOCAL},
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL */
						/* f*(aother-alocal) */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_MODULATE,	PS_FACTOR,		D3DTA_CURRENT},

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL */
						/* f*(aother - alocal) + alocal = f*aother + (1-f)*alocal */
						{PS_BLENDFACTORALPHA,	PS_OTHER,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(aother - alocal) + alocal */
						{PS_BLENDFACTORALPHA,	PS_OTHER,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL */
						/* (1-f)*alocal	*/
						{D3DTOP_MODULATE,	PS_INVFACTOR,	PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(-alocal)+alocal	*/
						{D3DTOP_MODULATE,	PS_FACTOR,		PS_LOCAL,
						 D3DTOP_SUBTRACT,	PS_LOCAL,		D3DTA_CURRENT},
						};

/* 2 stage-es alfa pixelshaderek arra az esetre, amikor f=0 */
APixelShader2Stages		DX9General::alphaPixelShadersf0[] =
						{	
						/* GR_COMBINE_FUNCTION_ZERO */
						/* zero */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FACTOR_LOCAL */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* GR_COMBINE_FACTOR_LOCAL_ALPHA */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							 
						/* GR_COMBINE_FUNCTION_SCALE_OTHER */
						/* f * aother */
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL */
						/* f*aother+alocal*/
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA */
						/* f*aother+alocal*/
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL */
						/* f*(aother-alocal) */						
						{D3DTOP_SUBTRACT,		PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL */
						/* f*(aother - alocal) + alocal */						
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(aother - alocal) + alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						
					
						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL */
						/* (1-f)*alocal	*/
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(-alocal)+alocal	*/
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						};

/* 2 stage-es alfa pixelshaderek arra az esetre, amikor f=1 */
APixelShader2Stages		DX9General::alphaPixelShadersf1[] =
						{	
						/* GR_COMBINE_FUNCTION_ZERO */
						/* zero */
						{D3DTOP_SUBTRACT,		PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FACTOR_LOCAL */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* GR_COMBINE_FACTOR_LOCAL_ALPHA */
						/* alocal */
						{D3DTOP_SELECTARG1, PS_LOCAL,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							 
						/* GR_COMBINE_FUNCTION_SCALE_OTHER */
						/* f * aother */
						{D3DTOP_SELECTARG1,	PS_OTHER,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
							
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL */
						/* f*aother+alocal*/
						{D3DTOP_ADD,		PS_OTHER,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA */
						/* f*aother+alocal*/
						{D3DTOP_ADD,		PS_OTHER,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						
						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL */
						/* f*(aother-alocal) */
						{D3DTOP_SUBTRACT,	PS_OTHER,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL */
						/* f*(aother - alocal) + alocal */
						{D3DTOP_SELECTARG1,	PS_OTHER,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},

						/* GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(aother - alocal) + alocal */			
						{D3DTOP_SELECTARG1,	PS_OTHER,		0,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},
						 
						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL */
						/* (1-f)*alocal	*/
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0},						

						/* GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA */
						/* f*(-alocal)+alocal	*/
						{D3DTOP_SUBTRACT,	PS_LOCAL,		PS_LOCAL,
						 D3DTOP_DISABLE,	D3DTA_CURRENT,	0}
						};