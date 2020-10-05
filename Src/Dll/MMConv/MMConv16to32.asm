;-----------------------------------------------------------------------------
; MMCONV16TO32.ASM - MultiMedia Converter lib, convering from 16 bit format to
;                    32 bit format
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
; MMConv:	MMConv16to32.ASM
;		16 bites formátumról 32 bitesre konvertáló rész
;
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;........................... Makrók ................................

CheckInit16To32	MACRO
LOCAL   InitOk

        or      _16to32Init,0h
        jne     InitOk
        call    _16to32_doinit
InitOk:

ENDM

GetShiftCode_16to32	MACRO	SrcPos, SrcBitCount, DstPos, DstBitCount
LOCAL	MMXShiftCodeOk
LOCAL	MMXUpperShiftcodeOk

		mov		eax,DstPos
		add		eax,DstBitCount
		sub		eax,SrcPos
		mov		ecx,SrcBitCount
		sub		eax,ecx
		push	eax
		or		ah,20h
		jns		MMXShiftCodeOk
		neg		eax
MMXShiftCodeOk:
		xchg	al,ah
		xchg	eax,[esp]
		sub		eax,ecx
		or		ah,20h
		jns		MMXUpperShiftcodeOk
		neg		eax
MMXUpperShiftcodeOk:
		xchg	al,ah
		push	eax

ENDM


;-------------------------------------------------------------------
;........................... Függvények ............................

.code

BeginOf16to32Convs	LABEL

;-------------------------------------------------------------------
; Ált. 16->32 konverter

_general16to32converter:
		lea		eax,_16to32lastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_16to32CP_newformat
		lea		eax,_16to32lastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_16to32CP_format_cached
_16to32CP_newformat:
		call	_16to32InitConvCode
_16to32CP_format_cached:

		movq	mm4,_16to32ARGBEntry_AlphaMask
		movq	mm5,_16to32ARGBEntry_RedMask
		movq	mm6,_16to32ARGBEntry_GreenMask
		movq	mm7,_16to32ARGBEntry_BlueMask
		mov		ecx,x
		cmp		ecx,2h
		jb		_16to32mini_
		je		_16to32
		test	edi,111b		;cél 8-cal osztható címen?
		je		_16to32
		call	_16to32mini_
_16to32:call	_16to32_
		or		ecx,ecx
		jnz		_16to32mini_
		ret

_16to32_:
        mov     edx,y
		push	ecx
		shr		ecx,1h
		lea		esi,[esi+4*ecx]
		lea		edi,[edi+8*ecx]
		push	esi edi
		neg		ecx
		or		_16to32ConvertWithAlpha,0h
		jne		_g16to32_nexty_with_alpha
_g16to32_nexty_without_alpha:
		push	esi edi ecx
_g16to32_nextx_without_alpha:
		mov		eax,[esi+4*ecx] ;
		movd	mm0,eax		;
		punpcklwd mm0,mm0  	;
		movq	mm3,mm4
		movq	mm1,mm0		;
		nop
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to32_redshift1:
		pslld	mm0,0h		;
		pand	mm1,mm6
		pand	mm2,mm7		;
_g16to32_greenshift1:
		pslld	mm1,0h
		por		mm3,mm0		;
_g16to32_blueshift1:
		pslld	mm2,0h
		por		mm3,mm1		;
_g16to32_higherbitmask:
		and		eax,12345678h	
		por		mm3,mm2		;
		movd	mm0,eax
		punpcklwd mm0,mm0	;
		inc		ecx
		movq	mm1,mm0		;
		movq	mm2,mm0
		pand	mm0,mm5		;
		pand	mm1,mm6
_g16to32_redshift2:
		pslld	mm0,0h		;
		pand	mm2,mm7
_g16to32_greenshift2:
		pslld	mm1,0h		;
		por		mm3,mm0
_g16to32_blueshift2:
		pslld	mm2,0h		;
		por		mm3,mm1
		por		mm3,mm2		;
        movq    [edi+8*ecx-8],mm3 ;
        jnz     _g16to32_nextx_without_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to32_nexty_without_alpha
		pop		edi esi ecx
		and		ecx,1h
		ret

_16to32mini_:
        mov     edx,y
		push	esi edi
		or		_16to32ConvertWithAlpha,0h
		jne		_g16to32mini_nexty_with_alpha
_g16to32mini_nexty_without_alpha:
		push	esi edi
		mov		ax,[esi] 	;
		movd	mm0,eax		;
		punpcklwd mm0,mm0  	;
		movq	mm3,mm4
		movq	mm1,mm0		;
		pand	mm0,mm5
		movq	mm2,mm1		;
_g16to32_redshift1_mini:
		pslld	mm0,0h
		pand	mm1,mm6		;
		pand	mm2,mm7
_g16to32_greenshift1_mini:
		pslld	mm1,0h		;
		por		mm3,mm0
_g16to32_blueshift1_mini:
		pslld	mm2,0h		;
		por		mm3,mm1
_g16to32_higherbitmask_mini:
		and		eax,12345678h	;
		por		mm3,mm2
		movd	mm0,eax		;
		punpcklwd mm0,mm0	;
		movq	mm1,mm0		;
		movq	mm2,mm0
		pand	mm0,mm5		;
		pand	mm1,mm6
_g16to32_redshift2_mini:
		pslld	mm0,0h		;
		pand	mm2,mm7
_g16to32_greenshift2_mini:
		pslld	mm1,0h		;
		por		mm3,mm0
_g16to32_blueshift2_mini:
		pslld	mm2,0h		;
		por		mm3,mm1
		por		mm3,mm2		;
        movd    [edi],mm3 	;
		pop		edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to32mini_nexty_without_alpha
_g16to32miniexit:
		pop		edi esi
		dec		ecx
		add		esi,2h
		add		edi,4h
		ret


_g16to32_nexty_with_alpha:
		push	esi edi ecx
_g16to32_nextx_with_alpha:
		mov		eax,[esi+4*ecx] ;
		movd	mm0,eax		;
		punpcklwd mm0,mm0  	;
		movq	mm1,mm0		;
		movq	mm3,mm0
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to32_redshift1_wa:
		pslld	mm0,0h		;
		pand	mm3,mm4
		pand	mm1,mm6		;
_g16to32_alphashift1_wa:
		pslld	mm3,0h				;alpha feltolása max pozicioba (1 bit)
						;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7		;
_g16to32_greenshift1_wa:
		pslld	mm1,0h
		psrad	mm3,7h		;		;alpha felsõ bitjének másolása
		por		mm0,mm1
_g16to32_blueshift1_wa:
		pslld	mm2,0h		;
_g16to32_higherbitmask_wa:
		and		eax,12345678h	
		por		mm0,mm2		;
_g16to32_alphashift2_wa:
		pslld	mm3,0h				;alpha betolása a végsõ helyre
		por		mm0,mm3		;
		movd	mm1,eax
		punpcklwd mm1,mm1	;
		movd	mm2,eax
		pand	mm1,mm5		;
		movd	mm3,eax
_g16to32_redshift2_wa:
		pslld	mm1,0h		;		;red shr/shl
		punpcklwd mm2,mm2
		por		mm0,mm1		;
		punpcklwd mm3,mm3
		pand	mm2,mm6		;
		movq	mm1,mm3
_g16to32_greenshift2_wa:
		pslld	mm2,0h		;		;green shr/shl
		pand	mm3,mm7
		por		mm0,mm2		;
_g16to32_blueshift2_wa:
		pslld	mm3,0h				;blue shr/shl
		pand	mm1,mm4		;
		por		mm0,mm3
_g16to32_alphashift3_wa:
		pslld	mm1,0h		;		;alpha shr/shl
		inc		ecx
		por		mm0,mm1		;
        movq	[edi+8*ecx-8],mm0 ;
        jnz     _g16to32_nextx_with_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to32_nexty_with_alpha
		pop		edi esi ecx
		and		ecx,1h
		ret


_g16to32mini_nexty_with_alpha:
		push	esi edi
		mov		ax,[esi]  	;
		movd	mm0,eax		;
		punpcklwd mm0,mm0  	;
		movq	mm1,mm0		;
		movq	mm3,mm0
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to32_redshift1_wa_mini:
		pslld	mm0,0h		;
		pand	mm3,mm4
		pand	mm1,mm6		;
_g16to32_alphashift1_wa_mini:
		pslld	mm3,0h				;alpha feltolása max pozicioba (1 bit)
							;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7		;
		psrad	mm3,7h				;alpha felsõ bitjének másolása
_g16to32_greenshift1_wa_mini:
		pslld	mm1,0h		;
_g16to32_blueshift1_wa_mini:
		pslld	mm2,0h
		por		mm0,mm1		;
_g16to32_higherbitmask_wa_mini:
		and		eax,12345678h	
		por		mm0,mm2		;
_g16to32_alphashift2_wa_mini:
		pslld	mm3,0h				;alpha betolása a végsõ helyre
		por		mm0,mm3		;
		movd	mm1,eax
		punpcklwd mm1,mm1	;
		movd	mm2,eax
		pand	mm1,mm5		;
		movd	mm3,eax
_g16to32_redshift2_wa_mini:
		pslld	mm1,0h		;		;red shr/shl
		punpcklwd mm2,mm2
		por		mm0,mm1		;
		punpcklwd mm3,mm3
		pand	mm2,mm6		;
		movq	mm1,mm3
_g16to32_greenshift2_wa_mini:
		pslld	mm2,0h		;		;green shr/shl
		pand	mm3,mm7
		por		mm0,mm2		;
_g16to32_blueshift2_wa_mini:
		pslld	mm3,0h				;blue shr/shl
		pand	mm1,mm4		;
		por		mm0,mm3
_g16to32_alphashift3_wa_mini:
		pslld	mm1,0h		;		;alpha shr/shl
		por		mm0,mm1		;
        movq    [edi],mm0 	;
		pop		edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to32mini_nexty_with_alpha
		jmp		_g16to32miniexit

EndOf16to32Convs	LABEL

;-------------------------------------------------------------------
;Init kód a 16->32 konverterhez

_16to32InitConvCode:
		CheckInit16To32
		push	OFFSET _16to32lastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _16to32lastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_16to32ConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,[edx].ABitCount		;dst alpha bitcount
		mov		ebp,[ebx].ABitCount		;src alpha bitcount
		jecxz	_16to32_noalpha
		mov		eax,constalpha
		mov		ecx,[edx].APos
		or		ebp,ebp
		je		_16to32_alphaok
		mov		_16to32ConvertWithAlpha,1h
		mov		al,_masktable[ebp]
		mov		ecx,[ebx].APos
_16to32_alphaok:
		shl	eax,cl
_16to32_noalpha:
		push	edx
		mov		DWORD PTR _16to32ARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _16to32ARGBEntry_AlphaMask[4],eax
		mov		eax,[ebx].RBitCount
		movzx	ebp,_masktable_magic[eax]
		mov		al,_masktable[eax]
		mov		ecx,[ebx].RPos
		shl		eax,cl
		shl		ebp,cl
		mov		DWORD PTR _16to32ARGBEntry_RedMask[0],eax
		mov		DWORD PTR _16to32ARGBEntry_RedMask[4],eax
		mov		eax,[ebx].GBitCount
		movzx	edx,_masktable_magic[eax]
		mov		al,_masktable[eax]
		mov		ecx,[ebx].GPos
		shl		eax,cl
		shl		edx,cl
		or		ebp,edx
		mov		DWORD PTR _16to32ARGBEntry_GreenMask[0],eax
		mov		DWORD PTR _16to32ARGBEntry_GreenMask[4],eax
		mov		eax,[ebx].BBitCount
		movzx	edx,_masktable_magic[eax]
		mov		al,_masktable[eax]
		mov		ecx,[ebx].BPos
		shl		eax,cl
		shl		edx,cl
		or		edx,ebp
		mov		DWORD PTR _16to32ARGBEntry_BlueMask[0],eax
		mov		DWORD PTR _16to32ARGBEntry_BlueMask[4],eax
		
		push	dx
		shl		edx,10h
		pop		dx
		
		mov		ebp,edx
		pop		edx

		GetShiftCode_16to32	[ebx].RPos, [ebx].RBitCount, [edx].RPos, [edx].RBitCount
		GetShiftCode_16to32	[ebx].GPos, [ebx].GBitCount, [edx].GPos, [edx].GBitCount
		GetShiftCode_16to32	[ebx].BPos, [ebx].BBitCount, [edx].BPos, [edx].BBitCount

		or	_16to32ConvertWithAlpha,0h
		jne	_16to32InitConverterWithAlpha

        mov     DWORD PTR _g16to32_higherbitmask+1,ebp
        mov     DWORD PTR _g16to32_higherbitmask_mini+1,ebp

		pop		eax
        and     WORD PTR _g16to32_blueshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift2+2,ax
        and     WORD PTR _g16to32_blueshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift2_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_blueshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift1+2,ax
        and     WORD PTR _g16to32_blueshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift1_mini+2,ax

		pop		eax
        and     WORD PTR _g16to32_greenshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift2+2,ax
        and     WORD PTR _g16to32_greenshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift2_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_greenshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift1+2,ax
        and     WORD PTR _g16to32_greenshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift1_mini+2,ax

		pop		eax
        and     WORD PTR _g16to32_redshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift2+2,ax
        and     WORD PTR _g16to32_redshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift2_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_redshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift1+2,ax
        and     WORD PTR _g16to32_redshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift1_mini+2,ax

		ret	

_16to32InitConverterWithAlpha:
		push	edx
		push	LARGE 32
		pop		eax
		mov		ecx,[ebx].ABitCount
		movzx	edx,_masktable_magic[ecx]
		cmp		ecx,1h
		je		_16to32InitConvWithAlphaMaxPos1
		dec		eax
_16to32InitConvWithAlphaMaxPos1:
		push	eax ecx
		mov		ecx,[ebx].APos
		shl		edx,cl
		sub		eax,ecx
		pop		ecx
		sub		eax,ecx
		or		ah,20h
		jns		_16to32InitConvWithAlphaMMXShiftCodeOk1
		neg		eax
_16to32InitConvWithAlphaMMXShiftCodeOk1:
		xchg	al,ah
        and     WORD PTR _g16to32_alphashift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift1_wa+2,ax
        and     WORD PTR _g16to32_alphashift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift1_wa_mini+2,ax
		push	dx
		shl		edx,10h
		pop		dx
		or		edx,ebp
        mov     DWORD PTR _g16to32_higherbitmask_wa+1,edx
        mov     DWORD PTR _g16to32_higherbitmask_wa_mini+1,edx
		pop		ecx
		pop		edx
		mov		eax,[edx].APos
		add		eax,[edx].ABitCount
		cmp		cl,32
		je		_16to32InitConvWithAlphaMaxPos2
		sub		cl,7h
_16to32InitConvWithAlphaMaxPos2:	
		sub		eax,ecx
		or		ah,20h
		jns		_16to32InitConvWithAlphaMMXShiftCodeOk2
		neg		eax
_16to32InitConvWithAlphaMMXShiftCodeOk2:
		xchg	al,ah
        and     WORD PTR _g16to32_alphashift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift2_wa+2,ax
        and     WORD PTR _g16to32_alphashift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift2_wa_mini+2,ax

		mov		eax,[edx].APos
		add		eax,[edx].ABitCount
		sub		eax,[ebx].ABitCount
		sub		eax,[ebx].APos
		sub		eax,[ebx].ABitCount
		or		ah,20h
		jns		_16to32InitConvWithAlphaMMXShiftCodeOk3
		neg		eax
_16to32InitConvWithAlphaMMXShiftCodeOk3:
		xchg	al,ah
        and     WORD PTR _g16to32_alphashift3_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift3_wa+2,ax
        and     WORD PTR _g16to32_alphashift3_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_alphashift3_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g16to32_blueshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift2_wa+2,ax
        and     WORD PTR _g16to32_blueshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_blueshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift1_wa+2,ax
        and     WORD PTR _g16to32_blueshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_blueshift1_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g16to32_greenshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift2_wa+2,ax
        and     WORD PTR _g16to32_greenshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_greenshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift1_wa+2,ax
        and     WORD PTR _g16to32_greenshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_greenshift1_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g16to32_redshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift2_wa+2,ax
        and     WORD PTR _g16to32_redshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to32_redshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift1_wa+2,ax
        and     WORD PTR _g16to32_redshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to32_redshift1_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 16->32 konverter írhatóvá tétele

_16to32_doinit:
        push    ecx edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE EndOf16to32Convs - BeginOf16to32Convs
        push    OFFSET BeginOf16to32Convs
        W32     VirtualProtect,4
        mov     _16to32Init,0FFh

        pop     eax edx ecx
        ret
	

.data

_16to32ARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_16to32ARGBEntry_RedMask	DQ	000FF000000FF0000h
_16to32ARGBEntry_GreenMask	DQ	00000FF000000FF00h
_16to32ARGBEntry_BlueMask	DQ	0000000FF000000FFh
_16to32lastUsedPixFmt_Src	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to32lastUsedPixFmt_Dst	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to32Init					DB	0h
_16to32ConvertWithAlpha		DB	0h