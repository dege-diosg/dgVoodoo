;-----------------------------------------------------------------------------
; MMCONV16TO16.ASM - MultiMedia Converter lib, convering from 16 bit format to
;                    another 16 bit format
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
; MMConv:	MMConv16to16.ASM
;		16 bites formátumról 16 bitesre konvertáló rész
;
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;........................... Makrók ................................

CheckInit16To16	MACRO
LOCAL   InitOk

        or      _16to16Init,0h
        jne     InitOk
        call    _16to16_doinit
InitOk:

ENDM

GetShiftCode_16to16	MACRO	SrcPos, SrcBitCount, DstPos, DstBitCount
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

BeginOf16to16Convs	LABEL	NEAR

;-------------------------------------------------------------------
; Ált. 16->16 konverter

_general16to16converter:
		lea		eax,_16to16lastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_16to16CP_newformat
		lea		eax,_16to16lastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_16to16CP_format_cached
_16to16CP_newformat:
		call	_16to16InitConvCode
_16to16CP_format_cached:

		movq	mm4,_16to16ARGBEntry_AlphaMask
		movq	mm5,_16to16ARGBEntry_RedMask
		movq	mm6,_16to16ARGBEntry_GreenMask
		movq	mm7,_16to16ARGBEntry_BlueMask
		mov		ecx,x
		cmp		ecx,4h
		jb		_16to16mini_
		je		_16to16
		mov		eax,edi
		and		eax,111b		;cél 8-cal osztható címen?
		je		_16to16
		sub		eax,8h
		neg		eax
		sub		ecx,eax
		push	ecx
		mov		ecx,eax
		call	_16to16mini_
		pop		ecx
_16to16:call	_16to16_
		or		ecx,ecx
		jnz		_16to16mini_
		ret

_16to16_:
        mov     edx,y
		push	ecx
		shr		ecx,2h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+8*ecx]
		push	esi
		push	edi
		neg		ecx
		or		_16to16ConvertWithAlpha,0h
		jne		_g16to16_nexty_with_alpha
_g16to16_nexty_without_alpha:
		push	esi
		push	edi
		push	ecx
_g16to16_nextx_without_alpha:
		movq	mm2,[esi+8*ecx] ;
		movq	mm3,mm4
		movd	eax,mm2		;
		movq	mm0,mm2
		psrlq	mm2,32		;
		movq	mm1,mm0
		movd	ebx,mm2		;
		pand	mm0,mm5
_g16to16_redshift1:
		psllw	mm0,0h		;
		movq	mm2,mm1
		pand	mm1,mm6		;
_g16to16_higherbitmask1:
		and		ebx,12345678h
		pand	mm2,mm7		;
_g16to16_greenshift1:
		psllw	mm1,0h
		por		mm3,mm0		;
_g16to16_blueshift1:
		psllw	mm2,0h
		movd	mm0,ebx		;
		por		mm3,mm1
		psllq	mm0,32		;
_g16to16_higherbitmask2:
		and		eax,12345678h
		movd	mm1,eax		;
		por		mm3,mm2
		por		mm0,mm1		;
		inc		ecx
		movq	mm1,mm0		;
		movq	mm2,mm0
		pand	mm0,mm5		;
		pand	mm1,mm6
_g16to16_redshift2:
		psllw	mm0,0h		;
		pand	mm2,mm7
_g16to16_greenshift2:
		psllw	mm1,0h		;
		por		mm3,mm0
_g16to16_blueshift2:
		psllw	mm2,0h		;
		por		mm3,mm1
		por		mm3,mm2		;
        movq    [edi+8*ecx-8],mm3 ;
        jnz     _g16to16_nextx_without_alpha
		pop		ecx
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16_nexty_without_alpha
		pop		edi
		pop		esi
		pop		ecx
		and		ecx,3h
		ret

_16to16mini_:
        mov     edx,y
		lea		esi,[esi+2*ecx]
		lea		edi,[edi+2*ecx]
		push	esi
		push	edi
		neg		ecx
		or		_16to16ConvertWithAlpha,0h
		jne		_g16to16_nexty_with_alpha
_g16to16mini_nexty_without_alpha:
		push	esi
		push	edi
		push	ecx
_g16to16mini_nextx_without_alpha:
		mov		ax,[esi+2*ecx] 	;
		movd	mm0,eax		;
		movq	mm3,mm4
		movq	mm1,mm0		;
		pand	mm0,mm5
		movq	mm2,mm1		;
_g16to16_redshift1_mini:
		psllw	mm0,0h
		pand	mm1,mm6		;
		pand	mm2,mm7
_g16to16_greenshift1_mini:
		psllw	mm1,0h		;
		por		mm3,mm0
_g16to16_blueshift1_mini:
		psllw	mm2,0h		;
		por		mm3,mm1
_g16to16_higherbitmask_mini:
		and		eax,12345678h	;
		por		mm3,mm2
		movd	mm0,eax		;
		inc		ecx
		movq	mm1,mm0		;
		movq	mm2,mm0
		pand	mm0,mm5		;
		pand	mm1,mm6
_g16to16_redshift2_mini:
		psllw	mm0,0h		;
		pand	mm2,mm7
_g16to16_greenshift2_mini:
		psllw	mm1,0h		;
		por		mm3,mm0
_g16to16_blueshift2_mini:
		psllw	mm2,0h		;
		por		mm3,mm1
		por		mm3,mm2		;
		movd	eax,mm3
        mov     [edi+2*ecx-2],ax 	;
		jnz		_g16to16mini_nextx_without_alpha
		pop		ecx
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16mini_nexty_without_alpha
_g16to16miniexit:
		pop		edi
		pop		esi
		ret


_g16to16_nexty_with_alpha:
		push	esi
		push	edi
		push	ecx
_g16to16_nextx_with_alpha:
		movq	mm2,[esi+8*ecx]	;
		movd	eax,mm2		;
		movq	mm1,mm2
		movq	mm0,mm2		;
		psrlq	mm2,32
		movd	ebx,mm2		;
		movq	mm3,mm1
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to16_redshift1_wa:
		psllw	mm0,0h		;
		pand	mm3,mm4
		pand	mm1,mm6		;
_g16to16_alphashift1_wa:
		psllw	mm3,0h				;alpha feltolása max pozicioba (1 bit)
							;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7		;
_g16to16_greenshift1_wa:
		psllw	mm1,0h
		psraw	mm3,3h		;		;alpha felsõ bitjének másolása
		por		mm0,mm1
_g16to16_blueshift1_wa:
		pslld	mm2,0h		;
_g16to16_higherbitmask_wa1:
		and		ebx,12345678h	
		por		mm0,mm2		;
_g16to16_higherbitmask_wa2:
		and		eax,12345678h
		movd	mm2,ebx		;
_g16to16_alphashift2_wa:
		pslld	mm3,0h				;alpha betolása a végsõ helyre
		movd	mm1,eax		;
		psllq	mm2,32
		por		mm0,mm3		;
		por		mm1,mm2
		movq	mm2,mm1		;
		pand	mm1,mm5
_g16to16_redshift2_wa:
		pslld	mm1,0h		;		;red shr/shl
		movq	mm3,mm2
		por		mm0,mm1		;
		pand	mm2,mm6
_g16to16_greenshift2_wa:
		pslld	mm2,0h		;		;green shr/shl
		pand	mm3,mm7
		por		mm0,mm2		;
_g16to16_blueshift2_wa:
		pslld	mm3,0h				;blue shr/shl
		por		mm0,mm3		;
		inc		ecx
        movq    [edi+8*ecx-8],mm0 ;
        jnz     _g16to16_nextx_with_alpha
		pop		ecx
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16_nexty_with_alpha
		pop		edi
		pop		esi
		pop		ecx
		and		ecx,3h
		ret


_g16to16mini_nexty_with_alpha:
		push	esi
		push	edi
		push	ecx
_g16to16mini_nextx_with_alpha:
		mov		ax,[esi+2*ecx] 	;
		movd	mm0,eax		;
		movq	mm1,mm0		;
		movq	mm3,mm0
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to16_redshift1_wa_mini:
		psllw	mm0,0h		;
		pand	mm3,mm4
		pand	mm1,mm6		;
_g16to16_alphashift1_wa_mini:
		psllw	mm3,0h				;alpha feltolása max pozicioba (1 bit)
							;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7		;
		psraw	mm3,3h				;alpha felsõ bitjének másolása
_g16to16_greenshift1_wa_mini:
		psllw	mm1,0h		;
_g16to16_blueshift1_wa_mini:
		psllw	mm2,0h
		por		mm0,mm1		;
_g16to16_higherbitmask_wa_mini:
		and		eax,12345678h	
		por		mm0,mm2		;
_g16to16_alphashift2_wa_mini:
		psllw	mm3,0h				;alpha betolása a végsõ helyre
		movd	mm1,eax		;
		por		mm0,mm3
		movq	mm2,mm1		;
		pand	mm1,mm5
		movq	mm3,mm2		;
_g16to16_redshift2_wa_mini:
		psllw	mm1,0h		;		;red shr/shl
		pand	mm2,mm6
		por		mm0,mm1		;
_g16to16_greenshift2_wa_mini:
		psllw	mm2,0h		;		;green shr/shl
		pand	mm3,mm7
		por		mm0,mm2		;
_g16to16_blueshift2_wa_mini:
		psllw	mm3,0h		;		;blue shr/shl
		por		mm0,mm3		;
		inc		ecx
		movd	eax,mm0		;
        mov     [edi+2*ecx-2],ax 	;
		jnz		_g16to16mini_nextx_with_alpha
		pop		ecx
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16mini_nexty_with_alpha
		jmp		_g16to16miniexit

EndOf16to16Convs	LABEL	NEAR

;-------------------------------------------------------------------
;Init kód a 16->16 konverterhez

_16to16InitConvCode:
		CheckInit16To16
		push	OFFSET _16to16lastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _16to16lastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_16to16ConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,(_pixfmt PTR [edx]).ABitCount		;dst alpha bitcount
		mov		ebp,(_pixfmt PTR [ebx]).ABitCount		;src alpha bitcount
		jecxz	_16to16_noalpha
		mov		eax,constalpha
		mov		ecx,(_pixfmt PTR [edx]).APos
		or		ebp,ebp
		je		_16to16_alphaok
		mov		_16to16ConvertWithAlpha,1h
		mov		eax,(_pixfmt PTR [edx]).ABitCount		;dst alpha bitcount
		mov		al,_masktable16[8*eax+ebp-1*8-1]
		mov		ecx,(_pixfmt PTR [ebx]).APos
_16to16_alphaok:
		shl		eax,cl
_16to16_noalpha:
		push	edx
		push	esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _16to16ARGBEntry_AlphaMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).RBitCount
		mov		ecx,(_pixfmt PTR [edx]).RBitCount
		movzx	ebp,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,(_pixfmt PTR [ebx]).RPos
		shl		eax,cl
		shl		ebp,cl
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ARGBEntry_RedMask[0],eax
		mov		DWORD PTR _16to16ARGBEntry_RedMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).GBitCount
		mov		ecx,(_pixfmt PTR [edx]).GBitCount
		movzx	esi,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,(_pixfmt PTR [ebx]).GPos
		shl		eax,cl
		shl		esi,cl
		or		ebp,esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ARGBEntry_GreenMask[0],eax
		mov		DWORD PTR _16to16ARGBEntry_GreenMask[4],eax
		mov		eax,(_pixfmt PTR [ebx]).BBitCount
		mov		ecx,(_pixfmt PTR [edx]).BBitCount
		movzx	esi,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,(_pixfmt PTR [ebx]).BPos
		shl		eax,cl
		shl		esi,cl
		or		ebp,esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ARGBEntry_BlueMask[0],eax
		mov		DWORD PTR _16to16ARGBEntry_BlueMask[4],eax
	
		push	bp
		shl		ebp,10h
		pop		bp
		
		pop		esi
		pop		edx

		GetShiftCode_16to16	(_pixfmt PTR [ebx]).RPos, (_pixfmt PTR [ebx]).RBitCount, (_pixfmt PTR [edx]).RPos, (_pixfmt PTR [edx]).RBitCount
		GetShiftCode_16to16	(_pixfmt PTR [ebx]).GPos, (_pixfmt PTR [ebx]).GBitCount, (_pixfmt PTR [edx]).GPos, (_pixfmt PTR [edx]).GBitCount
		GetShiftCode_16to16	(_pixfmt PTR [ebx]).BPos, (_pixfmt PTR [ebx]).BBitCount, (_pixfmt PTR [edx]).BPos, (_pixfmt PTR [edx]).BBitCount

		or		_16to16ConvertWithAlpha,0h
		jne		_16to16InitConverterWithAlpha

        mov     DWORD PTR _g16to16_higherbitmask1+2,ebp
        mov     DWORD PTR _g16to16_higherbitmask2+1,ebp
        mov     DWORD PTR _g16to16_higherbitmask_mini+1,ebp

		pop	eax
        and     WORD PTR _g16to16_blueshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift2+2,ax
        and     WORD PTR _g16to16_blueshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_blueshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift1+2,ax
        and     WORD PTR _g16to16_blueshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift1_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16_greenshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift2+2,ax
        and     WORD PTR _g16to16_greenshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_greenshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift1+2,ax
        and     WORD PTR _g16to16_greenshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift1_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16_redshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift2+2,ax
        and     WORD PTR _g16to16_redshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_redshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift1+2,ax
        and     WORD PTR _g16to16_redshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift1_mini+2,ax

		ret	

_16to16InitConverterWithAlpha:
		push	edx
		mov		ecx,(_pixfmt PTR [ebx]).ABitCount
		mov		eax,(_pixfmt PTR [edx]).ABitCount
		movzx	edx,_masktable_magicfor16[8*eax+ecx-1*8h-1]
		push	LARGE 16
		cmp		ecx,eax
		pop		eax
		jb		_16to16InitConvWithAlphaMaxPos1
		dec		eax
_16to16InitConvWithAlphaMaxPos1:
		push	eax
		push	ecx
		mov		ecx,(_pixfmt PTR [ebx]).APos
		shl		edx,cl
		sub		eax,ecx
		pop		ecx
		sub		eax,ecx
		or		ah,20h
		jns		_16to16InitConvWithAlphaMMXShiftCodeOk1
		neg		eax
_16to16InitConvWithAlphaMMXShiftCodeOk1:
		xchg	al,ah
        and     WORD PTR _g16to16_alphashift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_alphashift1_wa+2,ax
        and     WORD PTR _g16to16_alphashift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_alphashift1_wa_mini+2,ax
		push	dx
		shl		edx,10h
		pop		dx
		or		edx,ebp
        mov     DWORD PTR _g16to16_higherbitmask_wa1+2,edx
        mov     DWORD PTR _g16to16_higherbitmask_wa2+1,edx
        mov     DWORD PTR _g16to16_higherbitmask_wa_mini+1,edx
		pop		ecx
		pop		edx
		mov		eax,(_pixfmt PTR [edx]).APos
		add		eax,(_pixfmt PTR [edx]).ABitCount
		cmp		cl,16
		je		_16to16InitConvWithAlphaMaxPos2
		sub		cl,3h
_16to16InitConvWithAlphaMaxPos2:	
		sub		eax,ecx
		or		ah,20h
		jns		_16to16InitConvWithAlphaMMXShiftCodeOk2
		neg		eax
_16to16InitConvWithAlphaMMXShiftCodeOk2:
		xchg	al,ah
        and     WORD PTR _g16to16_alphashift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_alphashift2_wa+2,ax
        and     WORD PTR _g16to16_alphashift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_alphashift2_wa_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16_blueshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift2_wa+2,ax
        and     WORD PTR _g16to16_blueshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift2_wa_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_blueshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift1_wa+2,ax
        and     WORD PTR _g16to16_blueshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_blueshift1_wa_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16_greenshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift2_wa+2,ax
        and     WORD PTR _g16to16_greenshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift2_wa_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_greenshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift1_wa+2,ax
        and     WORD PTR _g16to16_greenshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_greenshift1_wa_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16_redshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift2_wa+2,ax
        and     WORD PTR _g16to16_redshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift2_wa_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16_redshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift1_wa+2,ax
        and     WORD PTR _g16to16_redshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16_redshift1_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 16->16 konverter írhatóvá tétele

_16to16_doinit:
        push    ecx
        push	edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE (EndOf16to16Convs - BeginOf16to16Convs)
        push    OFFSET BeginOf16to16Convs
        W32     VirtualProtect,4
        mov     _16to16Init,0FFh

        pop     eax
        pop		edx
        pop		ecx
        ret
	

.data


_masktable16	LABEL BYTE
	DB	1h, 0h, 0h, 08h, 10h, 20h, 0h, 0h 	;if dst bitcount = 1
	DB	0h, 0h, 0h,  0h,  0h,  0h, 0h, 0h 	;if dst bitcount = 2 (unused, invalid)
	DB	0h, 0h, 0h,  0h,  0h,  0h, 0h, 0h 	;if dst bitcount = 3 (unused, invalid)
	DB	1h, 0h, 0h, 0Fh, 1Eh, 3Ch, 0h, 0h 	;if dst bitcount = 4
	DB	1h, 0h, 0h, 0Fh, 1Fh, 3Eh, 0h, 0h 	;if dst bitcount = 5
	DB	1h, 0h, 0h, 0Fh, 1Fh, 3Fh, 0h, 0h 	;if dst bitcount = 6

_masktable_magicfor16	LABEL BYTE
;az alsó i-(1-i) = 2*i-1 bit kimaszkolva (tábla 1-8-ig)
_masktable_magic1	DB	0h, 0h, 0h,  0h,  0h,  0h,  0h, 0h
_masktable_magic2	DB	0h, 0h, 0h,  0h,  0h,  0h,  0h, 0h	;(unused, invalid)
_masktable_magic3	DB	0h, 0h, 0h,  0h,  0h,  0h,  0h, 0h	;(unused, invalid)
;az alsó i-(4-i) = 2*i-4 bit kimaszkolva (tábla 1-8-ig)
_masktable_magic4	DB	0h, 0h, 0h,  0h,  0h,  0h,  0h, 0h

;az alsó i-(5-i) = 2*i-5 bit kimaszkolva (tábla 1-8-ig)
_masktable_magic5	DB	0h, 0h, 0h, 08h,  0h,  0h,  0h, 0h

;az alsó i-(6-i) = 2*i-6 bit kimaszkolva (tábla 1-8-ig)
_masktable_magic6	DB	0h, 0h, 0h, 0Ch, 10h,  0h,  0h, 0h


_16to16ARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_16to16ARGBEntry_RedMask	DQ	000FF000000FF0000h
_16to16ARGBEntry_GreenMask	DQ	00000FF000000FF00h
_16to16ARGBEntry_BlueMask	DQ	0000000FF000000FFh
_16to16lastUsedPixFmt_Src	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to16lastUsedPixFmt_Dst	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to16Init					DB	0h
_16to16ConvertWithAlpha		DB	0h