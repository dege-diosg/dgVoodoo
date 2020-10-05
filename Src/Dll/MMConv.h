/*--------------------------------------------------------------------------------- */
/* MMConv.h - C-header for my MultiMedia Converter Lib                              */
/* (duplicated file)                                                                */
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
/* dgVoodoo:  MultiMedia Converter (MMX-implement�ci�)										*/
/*				Ez a modul az egyes sz�nform�tumok k�z�tti konverzi�t						*/
/*				biztos�tja																	*/
/*																							*/
/*				T�mogatott form�tumok (tetsz�leges komponens-sorrenddel):					*/
/*				-  8 bit: P8																*/
/*				- 16 bit: RGB555, RGB565, ARGB1555, ARGB4444, AP88							*/
/*				- 32 bit: RGB888, ARGB8888													*/
/*																							*/
/*				Konverzi� fajt�i:															*/
/*					-  8bit -> 16 vagy 32 bit												*/
/*					- 16bit -> 16 vagy 32 bit												*/
/*					- 16bit -> 16 vagy 32 bit maszkolt ColorKey-jel							*/
/*					- 32bit -> 16 vagy 32 bit												*/
/*					- 32bit -> 16 vagy 32 bit maszkolt ColorKey-jel							*/
/*					(A P8 �s AP88 form�tum nem lehet c�lform�tum)							*/
/*------------------------------------------------------------------------------------------*/
#ifndef	MMCONVERTER_H
#define MMCONVERTER_H

#include	"DDraw.h"
#include	"D3d.h"

/*------------------------------------------------------------------------------------------*/
/*.................................... Defin�ci�k ..........................................*/

/* A konvert�l�s fajt�i */
#define CONVERTTYPE_COPY			0	/* teljes m�sol�s �s konvert�l�s */
#define CONVERTTYPE_COLORKEY		1   /* csak a nem kulcssz�n� pixelek konvert�l�sa */
#define CONVERTTYPE_RAWCOPY			2	/* csak m�sol�s (egyez� form�tumokhoz) */
#define CONVERTTYPE_RAWCOPYCOLORKEY	3	/* csak m�sol�s kulcssz�nnel (egyez� form�tumokhoz) */


/* Az �tadott paletta t�pusai */
#define PALETTETYPE_UNCONVERTED		0	/* 32 bit ARGB form�tum */
#define PALETTETYPE_CONVERTED		1	/* Az adott form�tumra konvert�lt */


/*------------------------------------------------------------------------------------------*/
/*.................................... Strukt�r�k  .........................................*/

/* Sz�nform�tumot le�r� strukt�ra */
typedef struct	{

	unsigned int BitPerPixel;
	unsigned int RBitCount, GBitCount, BBitCount, ABitCount;
	unsigned int RPos, GPos, BPos, APos;

} _pixfmt;


/*------------------------------------------------------------------------------------------*/
/*.................................... F�ggv�nyek  .........................................*/

/* Az srcFormat form�tum� puffert dstFormat form�tum�v� konvert�lja */
void _stdcall MMCConvertAndTransferData(_pixfmt *srcFormat, _pixfmt *dstFormat,
										unsigned int colorKey, unsigned int colorKeyMask,
										unsigned int constAlpha,
										void *src, void *dst,
										unsigned int x, unsigned int y,
										int srcPitch, int dstPitch,
										unsigned int *palette, unsigned char *paletteMap,
										int convType, int palType);

/* Adott puffert colorkey sz�nnel t�lt fel */
void _stdcall MMCFillBuffer(void *dst, unsigned int bitsPerPixel, unsigned int colorKey,
						    unsigned int x, unsigned int y,
							int dstPitch);

/* Egy ARGB-ben megadott sz�nt �tkonvert�l a megadott form�tumra */
unsigned int _stdcall MMCGetPixelValueFromARGB (unsigned int color, _pixfmt *byFmt);

/* Egy adott form�tum� sz�nt �tkonvert�l ARGB-re */
unsigned int _stdcall MMCGetARGBFromPixelValue (unsigned int color, _pixfmt *byFmt, unsigned int constAlpha);

/* Az srcFmt form�tum� sz�nt dstFmt form�tum�ra konvert�lja */
/* Ez a k�vetkez�vel ekvivalens: MMCGetPixelValueFromARGB (MMCGetARGBFromPixelValue (srcColor, srcFmt, unsigned int constAlpha), dstFmt) */
unsigned int _stdcall MMCConvertPixel (unsigned int srcColor, _pixfmt *srcFmt, _pixfmt *dstFmt, unsigned int constAlpha);

/* Egy DDPIXELFORMAT sz�nform�tum-le�r�t konvert�l _pixfmt -re */
void _stdcall MMCConvertToPixFmt(LPDDPIXELFORMAT ddpf, _pixfmt *pf, int createMissingAlpha);

/* K�t form�tumot hasonl�t �ssze */
int _stdcall MMCIsPixfmtTheSame(_pixfmt *format1, _pixfmt *format2);

/* Egy 32 bit ARGB form�tum� palett�t pixfmt-form�tum�ra konvert�l */
void _stdcall MMCConvARGBPaletteToPixFmt(unsigned int *src, unsigned int *dst,
										 int from, int to, _pixfmt *dstFormat);

/* Egy 32 bit ARGB form�tum� palett�t 32 bit ABGR-re konvert�l */
void _stdcall MMCConvARGBPaletteToABGR (unsigned int *src, unsigned int *dst);

/*------------------------------------------------------------------------------------------*/


#endif
