;-----------------------------------------------------------------------------
; MMCONV.INC - Include used in MultiMedia Converter lib
;
; Copyright (C) 2004 Dege
;
; This library is free software; you can redistribute it and/or
; modify it under the terms of the GNU Lesser General Public
; License as published by the Free Software Foundation; either
; version 2.1 of the License, or (at your option) any later version.
;
; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; Lesser General Public License for more details.
;
; You should have received a copy of the GNU Lesser General Public
; License along with this library; if not, write to the Free Software
; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
;-----------------------------------------------------------------------------


;----------------------------------------------------------------------------
;dgVoodoo:  MultiMedia Converter
;
;	    MMConv.inc (MMX-implement�ci�)
;
;	    Ez a modul az egyes sz�nform�tumok k�z�tti konverzi�t
;           biztos�tja
;
;	    T�mogatott form�tumok (tetsz�leges komponens-sorrenddel):
;		-  8 bit: P8
;		- 16 bit: RGB555, RGB565, ARGB1555, ARGB4444
;		- 32 bit: RGB888, ARGB8888
;
;	    Konverzi� fajt�i:
;		-  8bit -> 16 vagy 32 bit
;		- 16bit -> 16 vagy 32 bit
;		- 16bit -> 16bit maszkolt ColorKey-jel
;		- 16bit -> 32bit maszkolt ColorKey-jel
;		- 32bit -> 16 vagy 32 bit
;		- 32bit -> 16bit maszkolt ColorKey-jel
;		- 32bit -> 32bit maszkolt ColorKey-jel
;----------------------------------------------------------------------------

MMXCODESHIFTMASK	EQU	0FF20h

;Sz�nform�tumot le�r� strukt�ra
_pixfmt         STRUC

BitPerPixel     DD      ?
RBitCount       DD      ?
GBitCount       DD      ?
BBitCount       DD      ?
ABitCount       DD      ?
RPos            DD      ?
GPos            DD      ?
BPos            DD      ?
APos            DD      ?

_pixfmt         ENDS

;DirectDraw sz�nform�tum-le�r� strukt�ra
DDPIXELFORMAT   STRUC

dwSize          DD      ?
dwFlags         DD      ?
dwFourCC        DD      ?

dwRGBBitCount   DD      ?
dwRBitMask      DD      ?
dwGBitMask      DD      ?
dwBBitMask      DD      ?
dwRGBAlphaBitMask DD    ?

DDPIXELFORMAT   ENDS

;------------------------------------------------------------------------------------------

CONVERTTYPE_COPY                EQU     0       ;teljes m�sol�s �s konvert�l�s
CONVERTTYPE_COLORKEY            EQU     1       ;csak a nem kulcssz�n� pixelek konvert�l�sa
CONVERTTYPE_RAWCOPY             EQU     2       ;csak m�sol�s (egyez� form�tumokhoz)
CONVERTTYPE_RAWCOPYCOLORKEY     EQU     3       ;csak m�sol�s kulcssz�nnel (egyez� form�tumokhoz)

;Az �tadott paletta t�pusai
PALETTETYPE_UNCONVERTED		EQU	0	;32 bit ARGB form�tum
PALETTETYPE_CONVERTED		EQU	1	;Az adott form�tumra konvert�lt

;------------------------------------------------------------------------------------------

IsPixFmtTheSame		MACRO	pixfmt1, pixfmt2

	push	pixfmt1
	push	pixfmt2
	call	_MMCIsPixfmtTheSame
	xor	al,1h

ENDM