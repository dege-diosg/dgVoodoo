;-----------------------------------------------------------------------------
; MMCONV8TOXX.ASM - MultiMedia Converter lib, convering from paletted format to
;                   16 or 32 bit format
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


;-------------------------------------------------------------------
; MMConv:	MMConv8toXX.ASM
;		Palettás formátumról 16 vagy 32 bitre konvertáló
;		rész
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;........................... Makrók ................................

CheckInit8ToXX	MACRO
LOCAL   InitOk

        or      _8toXXInit,0h
        jne     InitOk
        call    _8toXX_doinit
InitOk:

ENDM


GetShiftCode_8xx	MACRO	SrcPos, DstPos, DstBitCount
LOCAL	MMXShiftCodeOk

		mov		eax,DstPos
		add		eax,DstBitCount
		sub		eax,SrcPos
		or		ah,20h
		jns		MMXShiftCodeOk
		neg		eax
MMXShiftCodeOk:
		xchg	al,ah

ENDM


GetSingleShiftCode_8xx	MACRO	SrcPos, DstPos, DstBitCount

		mov		eax,SrcPos
		sub		eax,DstBitCount
		sub		eax,DstPos

ENDM


BSWAP_EAX       MACRO
        DB      0Fh,0C8h
ENDM

;-------------------------------------------------------------------
;........................... Függvények ............................

.code

;-------------------------------------------------------------------
;Egy ARGB palettát az adott PixFmt formátumú palettává konvertálja
;a megadott tarományban

;void MMCConvARGBPaletteToPixFmt(unsigned int *src, unsigned int *dst,
;                                int from, int to, _pixfmt dstFormat);

PUBLIC  _MMCConvARGBPaletteToPixFmt@20
PUBLIC  _MMCConvARGBPaletteToPixFmt
_MMCConvARGBPaletteToPixFmt@20:
_MMCConvARGBPaletteToPixFmt:
        push    esi
        push    edi
		push	ebx
		mov     ecx,[esp + 3*4 + 3*4]
		mov		esi,[esp + 1*4 + 3*4]
        mov     edi,[esp + 2*4 + 3*4]
		lea		esi,[esi+4*ecx]
		lea		edi,[edi+4*ecx]
		sub		ecx,[esp + 4*4 + 3*4]
		jae		_MMCCPTOPXFMT_END
		neg		ecx
        mov     ebx,[esp + 5*4 + 3*4]
		call	_8toXX_ConvertPalette
_MMCCPTOPXFMT_END:
		pop		ebx
		pop		edi
		pop		esi
		ret		5*4

;-------------------------------------------------------------------
;Egy ARGB palettát ABGR formátumú palettává konvertál

;void MMCConvARGBPaletteToABGR (unsigned int *src, unsigned int *dst);

PUBLIC	_MMCConvARGBPaletteToABGR@8
PUBLIC	_MMCConvARGBPaletteToABGR
_MMCConvARGBPaletteToABGR@8:
_MMCConvARGBPaletteToABGR:
		push	edi
		mov		edx,[esp + 1*4 + 1*4]
		mov		edi,[esp + 2*4 + 1*4]
		mov		ecx,256
_MMCCPTOBGRA_Next:
		mov		eax,[edx+4*ecx-4]
		BSWAP_EAX
		ror		eax,8h			;
		dec		ecx
		mov		[edi+4*ecx],eax		;
		jne		_MMCCPTOBGRA_Next
		pop		edi
		ret		8

BeginOf8toXXConvs		LABEL	NEAR

;-------------------------------------------------------------------
;Egy ARGB palettát egy adott PixFmt formátumú palettává konvertál

_8toXX_ConvertPalette:
		push	ecx
		lea		eax,_8toXXlastUsedPixFmt
		IsPixFmtTheSame	ebx, eax
		je		_8toXXCP_format_cached
		call	_8toXXInitConvCode
_8toXXCP_format_cached:
		pop		ecx
		mov		eax,(_pixfmt PTR [ebx]).BitPerPixel
        cmp     eax,16
        je      _8toXXCP_OK
        cmp     eax,32
        jne     _8toXXCP_END
_8toXXCP_OK:

		movq	mm4,_8toXXARGBEntry_RedMask
		movq	mm5,_8toXXARGBEntry_GreenMask
		movq	mm6,_8toXXARGBEntry_BlueMask
		mov		edx,ecx
		cmp		(_pixfmt PTR [ebx]).ABitCount,0h
		jne		_8toXXCP_PaletteWithAlpha
		shr		ecx,1h
		je		_8toXXCP_OneEntry
_8toXXCP_ConvertEntries:
		movq	mm0,[esi+8*ecx-8]	;
		movq	mm1,mm0			;
		pand	mm0,mm4
_8toXX_RedShift1:
		pslld	mm0,0h			;	;Red shl/shr
		movq	mm2,mm1
		pand	mm1,mm5			;
		pand	mm2,mm6
_8toXX_GreenShift1:
		pslld	mm1,0h			;	;Green shl/shr
		dec		ecx
_8toXX_BlueShift1:
		pslld	mm2,0h			;	;Blue shl/shr
		por		mm0,mm1
		por		mm0,mm2			;
		movq	[edi+8*ecx],mm0		;
		jne		_8toXXCP_ConvertEntries
		test	dl,1h
		je		_8toXXCP_END
_8toXXCP_OneEntry:
		movd	mm0, DWORD PTR [esi+4*edx-4]	;
		movq	mm1,mm0			;
		pand	mm0,mm4
_8toXX_RedShift2:
		pslld	mm0,0h			;	;Red shl/shr
		movq	mm2,mm1
		pand	mm1,mm5			;
		pand	mm2,mm6
_8toXX_GreenShift2:
		pslld	mm1,0h			;	;Green shl/shr
_8toXX_BlueShift2:
		pslld	mm2,0h			;	;Blue shl/shr
		por		mm0,mm1
		por		mm0,mm2			;
		movd	DWORD PTR [edi+4*edx-4],mm0	;
		jmp		_8toXXCP_END

_8toXXCP_PaletteWithAlpha:
		movq	mm7,_8toXXARGBEntry_AlphaMask
		shr		ecx,1h
		je		_8toXXCP_WAOneEntry
_8toXXCP_WAConvertEntries:
		movq	mm0,[esi+8*ecx-8]	;
		movq	mm1,mm0			;
		pand	mm0,mm4
_8toXX_RedShiftWA1:
		pslld	mm0,0h			;	;Red shl/shr
		movq	mm2,mm1
		pand	mm1,mm5			;
		movq	mm3,mm2
_8toXX_GreenShiftWA1:
		pslld	mm1,0h			;	;Green shl/shr
		pand	mm2,mm6
		por	mm0,mm1				;
_8toXX_BlueShiftWA1:
		pslld	mm2,0h				;Blue shl/shr
		pand	mm3,mm7			;
		por		mm0,mm2
_8toXX_AlphaShiftWA1:
		pslld	mm3,0h			;	;Alpha shl/shr
		dec		ecx
		por		mm0,mm3			;
		movq	[edi+8*ecx],mm0		;
		jne		_8toXXCP_WAConvertEntries
		test	dl,1h
		je		_8toXXCP_END
_8toXXCP_WAOneEntry:
		movd	mm0,DWORD PTR [esi+4*edx-4]	;
		movq	mm1,mm0			;
		pand	mm0,mm4
_8toXX_RedShiftWA2:
		pslld	mm0,0h			;	;Red shl/shr
		movq	mm2,mm1
		pand	mm1,mm5			;
		movq	mm3,mm2
_8toXX_GreenShiftWA2:
		pslld	mm1,0h			;	;Green shl/shr
		pand	mm2,mm6
		por		mm0,mm1			;
_8toXX_BlueShiftWA2:
		pslld	mm2,0h				;Blue shl/shr
		pand	mm3,mm7			;
		por		mm0,mm2			
_8toXX_AlphaShiftWA2:
		pslld	mm3,0h			;	;Alpha shl/shr
		por		mm0,mm3			;
		movd	DWORD PTR [edi+4*edx-4],mm0	;
	
_8toXXCP_END:
		emms
        ret

;-------------------------------------------------------------------
; Általános 8->16 konverter

_general8to16converter:
		mov		_8toXXConverter, OFFSET _8to16_
		mov		_8toXXMiniConverter, OFFSET _8to16mini_
		mov		cl,1h
		or		(_pixfmt PTR [ebx]).ABitCount,0h
		je		_general8toXXconverter

		mov		_8toXXConverter, OFFSET _AP88to16_
		mov		_8toXXMiniConverter, OFFSET _AP88to16mini_

		push	eax
		lea		eax,_AP88to16lastUsedPixFmt
		IsPixFmtTheSame	edx, eax
		je		_AP88to16CP_format_cached
		call	_AP88to16InitConvCode
_AP88to16CP_format_cached:
		pop		eax

		jmp		_general8toXXconverter

;-------------------------------------------------------------------
; Általános 8->32 konverter

_general8to32converter:
		mov		_8toXXConverter, OFFSET _8to32_
		mov		_8toXXMiniConverter, OFFSET _8to32mini_
		mov		cl,2h
		or		(_pixfmt PTR [ebx]).ABitCount,0h
		je		_general8toXXconverter

		mov		_8toXXConverter, OFFSET _AP88to32_
		mov		_8toXXMiniConverter, OFFSET _AP88to32mini_

		push	eax
		lea		eax,_AP88to32lastUsedPixFmt
		IsPixFmtTheSame	edx, eax
		je		_AP88to32CP_format_cached
		call	_AP88to32InitConvCode
_AP88to32CP_format_cached:
		pop		eax

		jmp		_general8toXXconverter	

;-------------------------------------------------------------------
; Általános 8->xx konverter

_general8toXXconverter:
		mov		ebp, 256 * 4
		test	esp,7h				;stackpointer
							; 8 byte-os határon?
		je		_8toXXESPOK
		add		ebp,4h
_8toXXESPOK:
		sub		esp,ebp				;konvertált palette
							;a stack-en
		push	ebp

		mov		ebx,palette
		cmp		eax,PALETTETYPE_CONVERTED
		je		_8toXXDontConvertPalette
		push	esi
		push	edi
		push	ecx
		mov		esi,ebx
		mov		ebx,edx
		lea		edi,[esp+4*4]
		mov		ecx,100h
		call	_8toXX_ConvertPalette
		mov		ebx,edi
		pop		ecx
		pop		edi
		pop		esi
_8toXXDontConvertPalette:

		mov		ebp,x
		cmp		ebp,8h				;ha < 8 pixel, egybõl minikonverter
		jb		_8toXX_postmini
		cmp		ebp,2*8				;ha < 2*8 pixel, nincs pre-mini
		jb		_8toXX
		mov		edx,edi
		and		edx,7h				;célpointer 8 bájtos határon?
		je		_8toXX
		sub		edx,8
		neg		edx
		shr		edx,cl
		sub		ebp,edx
		push	ebp
		mov		ebp,edx
		call    [_8toXXMiniConverter]
		pop		ebp
;ez a rész egyszerre 8 pixelt konvertál
_8toXX:
		call	[_8toXXConverter]
_8toXX_postmini:
		or		ebp,ebp
		je		_8toXX_end
		call    [_8toXXMiniConverter]
_8toXX_end:
		pop		eax	
		add		esp,eax
		ret

_8to16_:
		mov		eax,ebp
		and		ebp, NOT 7h
		sub		eax,ebp
		push	eax
		cmp		paletteMap,0h
		jne		_8to16_withpalmap
		add		esi,ebp
		lea		edi,[edi+2*ebp]
		neg		ebp
		push	esi
		push	edi
		push	DWORD PTR y
_8to16_bY8NextY:
		push	ebp
		push	esi
		push	edi
_8to16_bY8NextX:
		movq	mm0,[esi+ebp]	;
		xor		edx,edx
		movq	mm1,mm0		;
		psrlq   mm0,32
		xor		ecx,ecx		;
		movd	eax,mm1
		mov		dl,al		;
		mov		cl,ah
		shr		eax,16		;
		movd	mm4, DWORD PTR [ebx+4*edx]
		mov		dl,al		;
		movd	mm5, DWORD PTR [ebx+4*ecx]
		psllq	mm5,16		;
		mov		cl,ah
		por		mm4,mm5		;
		movd	mm5, DWORD PTR [ebx+4*edx]
		movd	mm6, DWORD PTR [ebx+4*ecx] ;
		psllq	mm5,32
		psllq	mm6,48		;
		por		mm4,mm5
		por		mm4,mm6		;
		movd	eax,mm0

		mov		dl,al		;
		mov		cl,ah
		shr		eax,16		;
		movd	mm2, DWORD PTR [ebx+4*edx]
		mov		dl,al		;
		movd	mm5, DWORD PTR [ebx+4*ecx]
		psllq	mm5,16		;
		mov		cl,ah
		por		mm2,mm5		;
		movd	mm5, DWORD PTR [ebx+4*edx]
		movd	mm6, DWORD PTR [ebx+4*ecx] ;
		psllq	mm5,32
		psllq	mm6,48		;
		por		mm2,mm5
		por		mm2,mm6		;
		movq	[edi+2*ebp],mm4
		movq	[edi+2*ebp+8],mm2 ;
		nop
		add		ebp,8h		;
		js		_8to16_bY8NextX
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_8to16_bY8NextY
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_8to16mini_:
		add		esi,ebp
		lea		edi,[edi+2*ebp]
		push	edi
		push	esi
		neg		ebp
		xor		eax,eax
		mov		edx,y
		mov		ecx,paletteMap
		or		ecx,ecx
		jne		_8to16mini_withpalmap
_8to16miniNextY:
		push	edi
		push	esi
		push	ebp
_8to16miniNextX:
		mov		al,[esi+ebp]
		nop
		mov		ecx,[ebx+4*eax]
		inc		ebp
		mov		[edi+2*ebp-2],cx
		jne		_8to16miniNextX
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_8to16miniNextY
		pop		esi
		pop		edi
		ret

_8to16_withpalmap:
		push	esi
		push	edi
		xchg	eax,ebp
		mov		ebp,paletteMap
		push	DWORD PTR y
		xchg	ebx,ebp
_8to16_bY8NextY_wp:
		push	esi
		push	edi
		push	eax
		shr		eax,3h
		push	eax
_8to16_bY8NextX_wp:
		movq	mm0,[esi]		;
		xor		edx,edx
		movq	mm1,mm0			;
		psrlq   mm0,32
		xor		ecx,ecx			;
		movd	eax,mm1
		mov		dl,al			;
		mov		cl,ah
		mov		BYTE PTR [ebx+edx],1h	;
		movd	mm4, DWORD PTR [ebp+4*edx]
		shr		eax,16			;
		mov		BYTE PTR [ebx+ecx],1h
		mov		dl,al			;
		movd	mm5, DWORD PTR [ebp+4*ecx]
		psllq	mm5,16			;
		mov		cl,ah
		por		mm4,mm5			;
		movd	mm5, DWORD PTR [ebp+4*edx]
		movd	mm6, DWORD PTR [ebp+4*ecx]		;
		psllq	mm5,32
		mov		BYTE PTR [ebx+edx],1h	;
		psllq	mm6,48
		mov		BYTE PTR [ebx+ecx],1h	;
		por		mm4,mm5
		por		mm4,mm6			;
		movd	eax,mm0

		mov		dl,al			;
		mov		cl,ah
		shr		eax,16			;
		movd	mm2, DWORD PTR [ebp+4*edx]
		mov		BYTE PTR [ebx+edx],1h	;
		mov		dl,al
		movd	mm5, DWORD PTR [ebp+4*ecx]		;
		mov		BYTE PTR [ebx+ecx],1h
		psllq	mm5,16			;
		mov		cl,ah
		mov		BYTE PTR [ebx+edx],1h	;
		por		mm2,mm5
		movd	mm5, DWORD PTR [ebp+4*edx]		;
		add		esi,8h
		movd	mm6, DWORD PTR [ebp+4*ecx]		;
		psllq	mm5,32
		mov		BYTE PTR [ebx+ecx],1h	;
		psllq	mm6,48
		por		mm2,mm5			;
		add		edi,16
		por		mm2,mm6			;
		movq	[edi-16],mm4
		movq	[edi-8],mm2 		;
		nop
		dec		DWORD PTR [esp]		;
		jne		_8to16_bY8NextX_wp
		pop		edx
		pop		eax
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_8to16_bY8NextY_wp
		pop		edx
		pop		edi
		pop		esi
		pop		ebp
		add		esi,eax
		lea		edi,[edi+2*eax]
		ret

_8to16mini_withpalmap:
		xor		eax,eax
_8to16miniNextY_wp:
		push	edi
		push	esi
		push	ebp
_8to16miniNextX_wp:
		mov		al,[esi+ebp]		;
		mov		BYTE PTR [ecx+eax],1h	;
		mov		eax,[ebx+4*eax]
		mov		[edi+2*ebp],ax		;
		inc		ebp
		mov		eax,0h			;
		jne		_8to16miniNextX_wp
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_8to16miniNextY_wp
		pop		esi
		pop		edi
		ret

;-------------------------------------------------------------------

_8to32_:
		mov		eax,ebp
		and		ebp, NOT 7h
		sub		eax,ebp
		push	eax
		cmp		paletteMap,0h
		jne		_8to32_withpalmap
		add		esi,ebp
		lea		edi,[edi+4*ebp]
		neg		ebp
		push	esi
		push	edi	
		push	DWORD PTR y
_8to32_bY8NextY:
		push	ebp
		push	esi
		push	edi
_8to32_bY8NextX:
		movq	mm0,[esi+ebp]	;
		xor		edx,edx
		movq	mm1,mm0		;
		psrlq   mm0,32
		xor		ecx,ecx		;
		movd	eax,mm1
		mov		dl,al		;
		mov		cl,ah
		shr		eax,16		;
		movd	mm5, DWORD PTR [ebx+4*ecx]
		mov		cl,ah		;
		movd	mm4, DWORD PTR [ebx+4*edx]
		psllq	mm5,32		;
		mov		dl,al
		por		mm4,mm5		;
		movd	mm5, DWORD PTR [ebx+4*edx]
		movd	mm6, DWORD PTR [ebx+4*ecx] ;
		movd	eax,mm0
		movq	[edi+4*ebp],mm4 ;
		psllq	mm6,32
		mov		dl,al		;
		por		mm5,mm6
		mov		cl,ah		;
		movq	[edi+4*ebp+8],mm5

		shr		eax,16		;
		movd	mm2, DWORD PTR [ebx+4*edx]
		mov		dl,al		;
		movd	mm5, DWORD PTR [ebx+4*ecx]
		psllq	mm5,32		;
		mov		cl,ah
		por		mm2,mm5		;
		movd	mm6, DWORD PTR [ebx+4*ecx]
		movd	mm5, DWORD PTR [ebx+4*edx] ;
		psllq	mm6,32
		por		mm5,mm6		;
		movq	[edi+4*ebp+16],mm2
		movq	[edi+4*ebp+24],mm5 ;
		nop
		add		ebp,8h		;
		js		_8to32_bY8NextX
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_8to32_bY8NextY
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_8to32mini_:
		add		esi,ebp
		lea		edi,[edi+4*ebp]
		push	edi
		push	esi
		neg		ebp
		xor		eax,eax
		mov		edx,y
		cmp		paletteMap,0h
		jne		_8to32mini_withpalmap
_8to32miniNextY:
		push	edi
		push	esi
		push	ebp
_8to32miniNextX:
		mov		al,[esi+ebp]		;
		nop
		mov		ecx,[ebx+4*eax]		;
		inc		ebp
		mov		[edi+4*ebp-4],ecx	;
		jne		_8to32miniNextX
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_8to32miniNextY
		pop		esi
		pop		edi
		ret

_8to32_withpalmap:
		push	esi
		push	edi
		xchg	eax,ebp
		xor		edx,edx
		mov		ebp,paletteMap
		push	DWORD PTR y
		xchg	ebx,ebp
_8to32_bY8NextY_wp:
		push	esi
		push	edi
		push	eax
		shr		eax,3h
		push	eax
_8to32_bY8NextX_wp:
		movq	mm0,[esi]		;
		add		esi,8h
		movq	mm1,mm0			;
		psrlq   mm0,32
		xor		ecx,ecx			;
		movd	eax,mm1
		mov		dl,al			;
		mov		cl,ah
		shr		eax,16			;
		mov		BYTE PTR [ebx+edx],1h
		movd	mm5, DWORD PTR [ebp+4*ecx] 	;
		mov		BYTE PTR [ebx+ecx],1h
		mov		cl,ah			;
		psllq	mm5,32
		movd	mm4, DWORD PTR [ebp+4*edx] 	;
		mov		dl,al
		por		mm4,mm5			;
		mov		BYTE PTR [ebx+edx],1h
		movd	mm5, DWORD PTR [ebp+4*edx] 	;
		movd	eax,mm0
		movd	mm6, DWORD PTR [ebp+4*ecx] 	;
		movq	[edi],mm4
		mov		BYTE PTR [ebx+ecx],1h 	;
		psllq	mm6,32
		mov		dl,al			;
		por		mm5,mm6
		mov		cl,ah			;
		movq	[edi+8],mm5

		shr		eax,16			;
		mov		BYTE PTR [ebx+edx],1h
		movd	mm2, DWORD PTR [ebp+4*edx] 	;
		mov		BYTE PTR [ebx+ecx],1h
		mov		dl,al			;
		movd	mm5, DWORD PTR [ebp+4*ecx]
		mov		BYTE PTR [ebx+edx],1h 	;
		psllq	mm5,32
		mov		cl,ah			;
		por		mm2,mm5
		mov		BYTE PTR [ebx+ecx],1h 	;
		movd	mm6, DWORD PTR [ebp+4*ecx]
		movd	mm5, DWORD PTR [ebp+4*edx] 	;
		psllq	mm6,32
		por		mm5,mm6			;
		movq	[edi+16],mm2
		movq	[edi+24],mm5 		;
		add		edi,32
		dec		DWORD PTR [esp]		;
		jne		_8to32_bY8NextX_wp
		pop		ecx
		pop		eax
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_8to32_bY8NextY_wp
		pop		ecx
		pop		edi
		pop		esi
		pop		ebp
		add		esi,eax
		lea		edi,[edi+4*eax]
		ret

_8to32mini_withpalmap:
		mov		ecx,paletteMap
		xor		eax,eax
_8to32miniNextY_wp:
		push	edi
		push	esi
		push	ebp
_8to32miniNextX_wp:
		mov		al,[esi+ebp]		;
		mov		BYTE PTR [ecx+eax],1h	;
		mov		eax,[ebx+4*eax]
		mov		[edi+4*ebp],eax		;
		inc		ebp
		mov		eax,0h			;
		jne		_8to32miniNextX_wp
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_8to32miniNextY_wp
		pop		esi
		pop		edi
		ret

;-------------------------------------------------------------------
;AP88->16 konverter

_AP88to16_:
		movd	mm7,_AP88to16AlphaMask
		mov		eax,ebp
		and		ebp, NOT 3h
		sub		eax,ebp
		push	eax
		lea		esi,[esi+2*ebp]
		lea		edi,[edi+2*ebp]
		shr		ebp,2h
		neg		ebp
		push	esi
		push	edi
		push	DWORD PTR y
		mov		ecx,paletteMap
		xor		edx,edx
		or		ecx,ecx
		jne		_AP88to16_withpalmap
_AP88to16_bY4NextY:
		push	ebp
		push	esi
		push	edi
_AP88to16_bY4NextX:
		movq	mm0,[esi+8*ebp]	;	;a3p3a2p2a1p1a0p0
		movq	mm1,mm0		;
		psrlq   mm0,32
		movd	eax,mm1		;
		movq	mm4,mm1
		mov		dl,al		;
_AP88to16_AlphaShift1:
		psrlq	mm4,0h			;a0 shr
		movd	mm3, DWORD PTR [ebx+4*edx]	;	;p0
		pand	mm4,mm7			;a0 mask
		shr		eax,16		;
		por		mm3,mm4
		mov		dl,al		;
		movq	mm4,mm1
		movd	mm2, DWORD PTR [ebx+4*edx]	;	;p1
_AP88to16_AlphaShift2:
		psrlq	mm4,0h			;a1 shr
		movd	eax,mm0		;
		pand	mm4,mm7			;a1 mask
		mov		dl,al		;
		por		mm2,mm4
		shr		eax,16		;
		psllq	mm2,16
		movd	mm5, DWORD PTR [ebx+4*edx]	;	;p2
		por		mm3,mm2
		movq	mm4,mm1		;
_AP88to16_AlphaShift3:
		psrlq	mm1,0h			;a3 shr
_AP88to16_AlphaShift4:
		psrlq	mm4,0h		;	;a2 shr
		mov		dl,al
		pand	mm4,mm7		;	;a2 mask
		pand	mm1,mm7			;a3 mask
		movd	mm2, DWORD PTR [ebx+4*edx] ;	;p3
		por		mm5,mm1
		psllq	mm5,32		;
		por		mm2,mm4
		por		mm3,mm5		;
		psllq	mm2,48
		por		mm3,mm2		;
		inc		ebp
		movq	[edi+8*ebp-8],mm3	;
		jnz		_AP88to16_bY4NextX
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to16_bY4NextY
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_AP88to16mini_:
		lea		esi,[esi+2*ebp]
		lea		edi,[edi+2*ebp]
		push	edi
		push	esi
		neg		ebp
		mov		edx,y
		mov		ecx,paletteMap
		or		ecx,ecx
		jne		_AP88to16mini_withpalmap
_AP88to16miniNextY:
		push	edi
		push	esi
		push	ebp
_AP88to16miniNextX:
		mov		ax,[esi+2*ebp]		;
		mov		cl,al			;
_AP88to16_AlphaShiftmini:
		shr		eax,0h			;
_AP88to16mini_AlphaMask:
		and		eax,12345678h		;
		or		eax,[ebx+4*ecx]
		inc		ebp
		mov		[edi+2*ebp-2],ax
		jne		_AP88to16miniNextX
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_AP88to16miniNextY
		pop		esi
		pop		edi
		ret

_AP88to16_withpalmap:
_AP88to16_bY4NextY_withpalmap:
		push	ebp
		push	esi
		push	edi
_AP88to16_bY4NextX_withpalmap:
		movq	mm0,[esi+8*ebp]	;	;a3p3a2p2a1p1a0p0
		movq	mm1,mm0		;
		psrlq   mm0,32
		movd	eax,mm1		;
		movq	mm4,mm1
		mov	dl,al		;
_AP88to16_AlphaShift1_withpalmap:
		psrlq	mm4,0h			;a0 shr
		movd	mm3, DWORD PTR [ebx+4*edx]	;	;p0
		pand	mm4,mm7			;a0 mask
		shr		eax,16		;
		mov		BYTE PTR [ecx+edx],1h
		por		mm3,mm4		;
		mov		dl,al
		movq	mm4,mm1		;
		mov		BYTE PTR [ecx+edx],1h
		movd	mm2, DWORD PTR [ebx+4*edx]	;	;p1
_AP88to16_AlphaShift2_withpalmap:
		psrlq	mm4,0h			;a1 shr
		movd	eax,mm0		;
		pand	mm4,mm7			;a1 mask
		mov		dl,al		;
		por		mm2,mm4
		shr		eax,16		;
		mov		BYTE PTR [ecx+edx],1h
		movd	mm5, DWORD PTR [ebx+4*edx]	;	;p2
		psllq	mm2,16
		por		mm3,mm2		;
		movq	mm4,mm1
_AP88to16_AlphaShift3_withpalmap:
		psrlq	mm1,0h		;	;a3 shr
		mov		dl,al
_AP88to16_AlphaShift4_withpalmap:
		psrlq	mm4,0h		;	;a2 shr
		mov		BYTE PTR [ecx+edx],1h
		pand	mm4,mm7		;
		pand	mm1,mm7
		movd	mm2, DWORD PTR [ebx+4*edx] ;	;p3
		por		mm5,mm1
		psllq	mm5,32		;
		por		mm2,mm4
		por		mm3,mm5		;
		psllq	mm2,48
		por		mm3,mm2		;
		inc		ebp
		movq	[edi+8*ebp-8],mm3	;
		jnz		_AP88to16_bY4NextX_withpalmap
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to16_bY4NextY_withpalmap
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_AP88to16mini_withpalmap:
		push	edx
		xor		edx,edx
_AP88to16miniNextY_withpalmap:
		push	edi
		push	esi
		push	ebp
_AP88to16miniNextX_withpalmap:
		mov		ax,[esi+2*ebp]		;
		mov		dl,al			;
_AP88to16_AlphaShiftmini_withpalmap:
		shr		eax,0h			;
		mov		BYTE PTR [ecx+edx],1h ;
_AP88to16mini_AlphaMask_withpalmap:
		and		eax,12345678h
		or		eax,[ebx+4*edx]		;
		inc		ebp
		mov		[edi+2*ebp-2],ax	;
		jnz		_AP88to16miniNextX_withpalmap
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to16miniNextY_withpalmap
		pop		eax
		pop		esi
		pop		edi
		ret

;-------------------------------------------------------------------
;AP88->32 konverter

_AP88to32_:
		movd	mm7,_AP88to32AlphaMask
		mov		eax,ebp
		and		ebp, NOT 3h
		sub		eax,ebp
		push	eax
		lea		esi,[esi+2*ebp]
		lea		edi,[edi+4*ebp]
		shr		ebp,1h
		neg		ebp
		push	esi
		push	edi
		push	DWORD PTR y
		mov		ecx,paletteMap
		xor		edx,edx
		or		ecx,ecx
		jne		_AP88to32_withpalmap
_AP88to32_bY4NextY:
		push	ebp
		push	esi
		push	edi
_AP88to32_bY4NextX:
		movq	mm0,[esi+4*ebp]	;	;a3p3a2p2a1p1a0p0
		movq	mm1,mm0		;
		psrlq   mm0,32
		movd	eax,mm1		;
		movq	mm4,mm1
		mov		dl,al		;
_AP88to32_AlphaShift1:
		psrlq	mm4,0h			;a0 shl/shr
		movd	mm3, DWORD PTR [ebx+4*edx]	;	;p0
		pand	mm4,mm7			;a0 mask
		shr		eax,16		;
		por		mm3,mm4
		mov		dl,al		;
		movq	mm4,mm1
		movd	mm2, DWORD PTR [ebx+4*edx]	;	;p1
_AP88to32_AlphaShift2:
		psrlq	mm4,0h			;a1 shr
		movd	eax,mm0		;
		pand	mm4,mm7			;a1 mask
		mov		dl,al		;
		por		mm2,mm4
		shr		eax,16		;
		psllq	mm2,32
		movd	mm5, DWORD PTR [ebx+4*edx]	;	;p2
		por		mm3,mm2
		movq	mm4,mm1		;
_AP88to32_AlphaShift3:
		psrlq	mm1,0h			;a2 shr
		movq	[edi+8*ebp],mm3	;
_AP88to32_AlphaShift4:
	psrlq	mm4,0h			;a3 shr
		pand	mm4,mm7		;	;a3 mask
		mov		dl,al
		movd	mm2, DWORD PTR [ebx+4*edx]	;	;p3
		pand	mm1,mm7			;a2 mask
		por		mm2,mm4		;
		por		mm5,mm1
		psllq	mm2,32		;
		add		ebp,2h
		por		mm2,mm5		;
		movq	[edi+8*ebp-8],mm2	;
		jnz		_AP88to32_bY4NextX
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to32_bY4NextY
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_AP88to32mini_:
		lea		esi,[esi+2*ebp]
		lea		edi,[edi+4*ebp]
		push	edi
		push	esi
		neg		ebp
		mov		edx,y
		mov		ecx,paletteMap
		or		ecx,ecx
		jne		_AP88to32mini_withpalmap
_AP88to32miniNextY:
		push	edi
		push	esi
		push	ebp
_AP88to32miniNextX:
		mov		ax,[esi+2*ebp]		;
		mov		cl,al			;
_AP88to32_AlphaShiftmini:
		ror		eax,0h			;
_AP88to32mini_AlphaMask:
		and		eax,12345678h		;
		or		eax,[ebx+4*ecx]
		inc		ebp
		mov		[edi+4*ebp-4],eax
		jnz		_AP88to32miniNextX
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jne		_AP88to32miniNextY
		pop		esi
		pop		edi
		ret

_AP88to32_withpalmap:
_AP88to32_bY4NextY_withpalmap:
		push	ebp
		push	esi
		push	edi
_AP88to32_bY4NextX_withpalmap:
		movq	mm0,[esi+4*ebp]	;	;a3p3a2p2a1p1a0p0
		movq	mm1,mm0		;
		psrlq   mm0,32
		movd	eax,mm1		;
		movq	mm4,mm1
		mov		dl,al		;
_AP88to32_AlphaShift1_withpalmap:
		psrlq	mm4,0h			;a0 shl/shr
		movd	mm3, DWORD PTR [ebx+4*edx]	;	;p0
		pand	mm4,mm7			;a0 mask
		shr		eax,16		;
		mov		BYTE PTR [ecx+edx],1h
		por		mm3,mm4		;
		mov		dl,al
		movd	mm2, DWORD PTR [ebx+4*edx]	;	;p1
		movq	mm4,mm1
_AP88to32_AlphaShift2_withpalmap:
		psrlq	mm4,0h		;	;a1 shr
		mov		BYTE PTR [ecx+edx],1h
		movd	eax,mm0		;
		pand	mm4,mm7			;a1 mask
		mov		dl,al		;
		por		mm2,mm4
		shr		eax,16		;
		psllq	mm2,32
		movd	mm5, DWORD PTR [ebx+4*edx]	;	;p2
		por		mm3,mm2
		movq	mm4,mm1		;
_AP88to32_AlphaShift3_withpalmap:
		psrlq	mm1,0h			;a2 shr
		movq	[edi+8*ebp],mm3	;
_AP88to32_AlphaShift4_withpalmap:
		psrlq	mm4,0h			;a3 shr
		pand	mm4,mm7		;	;a3 mask
		mov		BYTE PTR [ecx+edx],1h
		mov		dl,al		;
		pand	mm1,mm7			;a2 mask
		movd	mm2, DWORD PTR [ebx+4*edx] ;	;p3
		por		mm5,mm1
		por		mm2,mm4		;
		mov		BYTE PTR [ecx+edx],1h
		psllq	mm2,32		;
		add		ebp,2h
		por		mm2,mm5		;
		movq	[edi+8*ebp-8],mm2	;
		jnz		_AP88to32_bY4NextX_withpalmap
		pop		edi
		pop		esi
		pop		ebp
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to32_bY4NextY_withpalmap
		pop		eax
		pop		edi
		pop		esi
		pop		ebp
		ret

_AP88to32mini_withpalmap:
		push	edx
		xor		edx,edx
_AP88to32miniNextY_withpalmap:
		push	edi
		push	esi
		push	ebp
_AP88to32miniNextX_withpalmap:
		mov		ax,[esi+2*ebp]		;
		mov		dl,al			;
_AP88to32_AlphaShiftmini_withpalmap:
		ror		eax,0h			;
		mov		BYTE PTR [ecx+edx],1h
_AP88to32mini_AlphaMask_withpalmap:
		and		eax,12345678h		;
		or		eax,[ebx+4*edx]
		inc		ebp
		mov		[edi+4*ebp-4],eax
		jnz		_AP88to32miniNextX_withpalmap
		pop		ebp
		pop		esi
		pop		edi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		DWORD PTR [esp]
		jne		_AP88to32miniNextY_withpalmap
		pop		eax
		pop		esi
		pop		edi
		ret

EndOf8toXXConvs		LABEL	NEAR

;-------------------------------------------------------------------
;Init kód a palettakonverterhez

_8toXXInitConvCode:
		CheckInit8ToXX
		push	OFFSET _8toXXlastUsedPixFmt
		push	ebx
		call	CopyPixFmt
		mov		eax,(_pixfmt PTR [ebx]).RBitCount
		mov		al,_masktable_up[eax]
		shl		eax,16
		mov		DWORD PTR _8toXXARGBEntry_RedMask[0],eax	
		mov		DWORD PTR _8toXXARGBEntry_RedMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).GBitCount
		mov		al,_masktable_up[eax]
		shl		eax,8
		mov		DWORD PTR _8toXXARGBEntry_GreenMask[0],eax	
		mov		DWORD PTR _8toXXARGBEntry_GreenMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).BBitCount
		mov		al,_masktable_up[eax]
		mov		DWORD PTR _8toXXARGBEntry_BlueMask[0],eax
		mov		DWORD PTR _8toXXARGBEntry_BlueMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).ABitCount
		mov		al,_masktable_up[eax]
		shl		eax,24
		mov		DWORD PTR _8toXXARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _8toXXARGBEntry_AlphaMask[4],eax

		GetShiftCode_8xx	16+8, (_pixfmt PTR [ebx]).RPos, (_pixfmt PTR [ebx]).RBitCount
		push	eax
		GetShiftCode_8xx	8+8, (_pixfmt PTR [ebx]).GPos, (_pixfmt PTR [ebx]).GBitCount
		push	eax
		GetShiftCode_8xx	0+8, (_pixfmt PTR [ebx]).BPos, (_pixfmt PTR [ebx]).BBitCount
		push	eax

		or		(_pixfmt PTR [ebx]).ABitCount,0h
		jne		_8toXXInitConvCode_Alpha

		pop		eax
		and		WORD PTR _8toXX_BlueShift1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_BlueShift1+2, ax
		and		WORD PTR _8toXX_BlueShift2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_BlueShift2+2, ax

		pop		eax
		and		WORD PTR _8toXX_GreenShift1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_GreenShift1+2, ax
		and		WORD PTR _8toXX_GreenShift2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_GreenShift2+2, ax

		pop		eax
		and		WORD PTR _8toXX_RedShift1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_RedShift1+2, ax
		and		WORD PTR _8toXX_RedShift2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_RedShift2+2, ax
		ret

_8toXXInitConvCode_Alpha:
		pop		eax
		and		WORD PTR _8toXX_BlueShiftWA1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_BlueShiftWA1+2, ax
		and		WORD PTR _8toXX_BlueShiftWA2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_BlueShiftWA2+2, ax
		
		pop		eax
		and		WORD PTR _8toXX_GreenShiftWA1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_GreenShiftWA1+2, ax
		and		WORD PTR _8toXX_GreenShiftWA2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_GreenShiftWA2+2, ax

		pop		eax
		and		WORD PTR _8toXX_RedShiftWA1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_RedShiftWA1+2, ax
		and		WORD PTR _8toXX_RedShiftWA2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_RedShiftWA2+2, ax

		GetShiftCode_8xx	24+8, (_pixfmt PTR [ebx]).APos, (_pixfmt PTR [ebx]).ABitCount
		and		WORD PTR _8toXX_AlphaShiftWA1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_AlphaShiftWA1+2, ax
		and		WORD PTR _8toXX_AlphaShiftWA2+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _8toXX_AlphaShiftWA2+2, ax
		ret

;-------------------------------------------------------------------
;Init kód az AP88->16 konverterhez
_AP88to16InitConvCode:
		CheckInit8ToXX
		push	OFFSET _AP88to16lastUsedPixFmt
		push	edx
		call	CopyPixFmt

		mov		eax,(_pixfmt PTR [edx]).ABitCount
		mov		ecx,(_pixfmt PTR [edx]).APos
		mov		al,_masktable[eax]
		shl		eax,cl
		mov		DWORD PTR _AP88to16AlphaMask,eax

		mov		DWORD PTR _AP88to16mini_AlphaMask+1,eax
		mov		DWORD PTR _AP88to16mini_AlphaMask_withpalmap+1,eax

		GetSingleShiftCode_8xx	16, (_pixfmt PTR [edx]).APos, (_pixfmt PTR [edx]).ABitCount
		mov		BYTE PTR _AP88to16_AlphaShift1+3,al
		mov		BYTE PTR _AP88to16_AlphaShiftmini+2,al
		mov		BYTE PTR _AP88to16_AlphaShift1_withpalmap+3,al
		mov		BYTE PTR _AP88to16_AlphaShiftmini_withpalmap+2,al
		add		al,16
		mov		BYTE PTR _AP88to16_AlphaShift2+3,al
		mov		BYTE PTR _AP88to16_AlphaShift2_withpalmap+3,al
		add		al,16
		mov		BYTE PTR _AP88to16_AlphaShift3+3,al
		mov		BYTE PTR _AP88to16_AlphaShift3_withpalmap+3,al
		add		al,16
		mov		BYTE PTR _AP88to16_AlphaShift4+3,al
		mov		BYTE PTR _AP88to16_AlphaShift4_withpalmap+3,al
		ret

;-------------------------------------------------------------------
;Init kód az AP88->32 konverterhez
_AP88to32InitConvCode:

		CheckInit8ToXX
		push	OFFSET _AP88to32lastUsedPixFmt
		push	edx
		call	CopyPixFmt

		mov		eax,(_pixfmt PTR [edx]).ABitCount
		mov		ecx,(_pixfmt PTR [edx]).APos
		mov		al,_masktable[eax]
		shl		eax,cl
		mov		DWORD PTR _AP88to32AlphaMask,eax

		mov     DWORD PTR _AP88to32mini_AlphaMask+1,eax
		mov     DWORD PTR _AP88to32mini_AlphaMask_withpalmap+1,eax

		GetShiftCode_8xx	16, (_pixfmt PTR [edx]).APos, (_pixfmt PTR [edx]).ABitCount
		and		WORD PTR _AP88to32_AlphaShift1+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _AP88to32_AlphaShift1+2,ax
		and		WORD PTR _AP88to32_AlphaShift1_withpalmap+2, NOT MMXCODESHIFTMASK
		or		WORD PTR _AP88to32_AlphaShift1_withpalmap+2,ax
		GetSingleShiftCode_8xx	16, (_pixfmt PTR [edx]).APos, (_pixfmt PTR [edx]).ABitCount
		mov		BYTE PTR _AP88to32_AlphaShiftmini+2,al
		mov		BYTE PTR _AP88to32_AlphaShiftmini_withpalmap+2,al
		
		add		al,16
		mov		BYTE PTR _AP88to32_AlphaShift2+3,al
		mov		BYTE PTR _AP88to32_AlphaShift2_withpalmap+3,al
		add		al,16
		mov		BYTE PTR _AP88to32_AlphaShift3+3,al
		mov		BYTE PTR _AP88to32_AlphaShift3_withpalmap+3,al
		add		al,16
		mov		BYTE PTR _AP88to32_AlphaShift4+3,al
		mov		BYTE PTR _AP88to32_AlphaShift4_withpalmap+3,al
		ret

;-------------------------------------------------------------------
;A palettakonverter lapja írhatóvá tétele

_8toXX_doinit:
        push    ecx
        push	edx
        push    eax
        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE (EndOf8toXXConvs - BeginOf8toXXConvs)
        push    OFFSET BeginOf8toXXConvs
        W32     VirtualProtect,4
        mov     _8toXXInit,0FFh

        pop     eax
        pop		edx
        pop		ecx
        ret


;-------------------------------------------------------------------
;............................ Adatok ...............................

.data

_8toXXARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_8toXXARGBEntry_RedMask		DQ	000FF000000FF0000h
_8toXXARGBEntry_GreenMask	DQ	00000FF000000FF00h
_8toXXARGBEntry_BlueMask	DQ	0000000FF000000FFh
_AP88to16AlphaMask			DD	00000000h
_AP88to32AlphaMask			DD	00000000h
_8toXXlastUsedPixFmt		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_AP88to16lastUsedPixFmt		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_AP88to32lastUsedPixFmt		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_8toXXConverter				DD	?
_8toXXMiniConverter			DD	?
_8toXXInit					DB	0h
