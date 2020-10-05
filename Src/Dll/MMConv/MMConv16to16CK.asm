;-----------------------------------------------------------------------------
; MMCONV16TO16CK.ASM - MultiMedia Converter lib, convering from 16 bit format
;                      to another 16 bit format with colorkeying capability
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
;		 colorkey használata
;
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;........................... Makrók ................................

CheckInit16To16CK	MACRO
LOCAL   InitOk

        or      _16to16ckInit,0h
        jne     InitOk
        call    _16to16ck_doinit
InitOk:

ENDM

GetShiftCode_16to16CK	MACRO	SrcPos, SrcBitCount, DstPos, DstBitCount
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

BeginOf16to16ckConvs	LABEL

;-------------------------------------------------------------------
; Ált. 16->16 konverter ColorKey-jel

_ckgeneral16to16converter:
		lea		eax,_16to16cklastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_16to16ckCP_newformat
		lea		eax,_16to16cklastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_16to16ckCP_format_cached
_16to16ckCP_newformat:
		call	_16to16ckInitConvCode
_16to16ckCP_format_cached:

		movq	mm4,_16to16ckARGBEntry_AlphaMask
		movq	mm5,_16to16ckARGBEntry_RedMask
		movq	mm6,_16to16ckARGBEntry_GreenMask
		movq	mm7,_16to16ckARGBEntry_BlueMask
		mov		ebp,colorkey
		mov		eax,colorKeyMask
		movd	mm0,eax
		punpckldq mm0,mm0
		movq	_16to16ckColorKeyMask,mm0
		mov		ecx,x
		cmp		ecx,4h
		jb		_16to16ckmini_
		je		_16to16ck
		mov		eax,edi
		and		eax,111b		;cél 8-cal osztható címen?
		je		_16to16ck
		sub		eax,8h
		neg		eax
		sub		ecx,eax
		push	ecx
		mov		ecx,eax
		call	_16to16ckmini_
		pop		ecx
_16to16ck:
		call	_16to16ck_
		or		ecx,ecx
		jnz		_16to16ckmini_
		ret

_16to16ck_:
        mov     edx,y
		push	ecx
		shr		ecx,2h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+8*ecx]
		push	esi edi
		neg		ecx
		push	bp
		push	bp
		push	bp
		push	bp
		movq	mm3,[esp]
		add		esp,8h
		or		_16to16ckConvertWithAlpha,0h
		jne		_g16to16ck_nexty_with_alpha
_g16to16ck_nexty_without_alpha:
		push	esi edi ecx edx
_g16to16ck_nextx_without_alpha:
		movq	mm1,[esi+8*ecx] ;
		movq	mm2,mm3			;ck
		movd	eax,mm1		;
		movq	mm0,mm1
		pand	mm0,_16to16ckColorKeyMask ;
		pcmpeqw	mm2,mm0		;
		packsswb mm2,mm2	;
		movq	mm0,mm1
		movd	ebp,mm2		;
		psrlq	mm1,32
		cmp		ebp,0FFFFFFFFh	;
		je		_g16to16ck_reject_without_alpha
		movd	ebx,mm1		;
		movq	mm1,mm0
		pand	mm0,mm5		;	;red mask
		movq	mm2,mm1
		pand	mm1,mm6		;	;green mask
_g16to16ck_redshift1:
		psllw	mm0,0h
_g16to16ck_higherbitmask1:
		and		ebx,12345678h	;
		por		mm0,mm4
_g16to16ck_greenshift1:
		psllw	mm1,0h		;
		pand	mm2,mm7			;blue mask
		mov		edx,ebp		;
		por		mm0,mm1
		movd	mm1,ebx		;
_g16to16ck_blueshift1:
		psllw	mm2,0h
		and		ebp,0FFFF0000h	;
		and		edx,00000FFFFh
		por		mm0,mm2		;
_g16to16ck_higherbitmask2:
		and		eax,12345678h
		movd	mm2,eax		;
		psllq	mm1,32
		por		mm1,mm2		;
		cmp		edx,00000FFFFh
		movq	mm2,mm1		;
		pand	mm1,mm5			;red mask
_g16to16ck_redshift2:
		psllw	mm1,0h		;
		por		mm1,mm0		;
		movq	mm0,mm2
		pand	mm0,mm6		;	;green mask
		pand	mm2,mm7			;blue mask
_g16to16ck_greenshift2:
		psllw	mm0,0h		;
		nop
_g16to16ck_blueshift2:
		psllw	mm2,0h		;
		por		mm1,mm0
		por		mm1,mm2		;
		je		_g16to16cklowerhalf_without_alpha
		cmp		edx,00FFh
		ja		_g16to16ck_lowerhalf_lowerwrite
		je		_g16to16ck_lowerhalf_higherwrite
		movd	[edi+8*ecx],mm1
_g16to16cklowerhalf_without_alpha:
		cmp		ebp,0FFFF0000h
		je		_g16to16ck_reject_without_alpha

		psrlq	mm1,32
		cmp		ebp,00FF0000h
		ja		_g16to16ck_higherhalf_lowerwrite
		je		_g16to16ck_higherhalf_higherwrite
		movd    [edi+8*ecx+4],mm1
		nop
_g16to16ck_reject_without_alpha:
		inc		ecx
        jnz     _g16to16ck_nextx_without_alpha
_g16to16ck_x_end_without_alpha:
		pop		edx ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16ck_nexty_without_alpha
		pop		edi esi ecx
		and		ecx,3h
		ret

_g16to16ck_lowerhalf_lowerwrite:
		movd	eax,mm1
		mov		[edi+8*ecx],ax
		jmp		_g16to16cklowerhalf_without_alpha
_g16to16ck_lowerhalf_higherwrite:
		movd	eax,mm1
		shr		eax,16
		mov		[edi+8*ecx+2],ax
		jmp		_g16to16cklowerhalf_without_alpha
_g16to16ck_higherhalf_lowerwrite:
		movd	eax,mm1
		mov		[edi+8*ecx+4],ax
		inc		ecx
		jnz		_g16to16ck_nextx_without_alpha
		jmp		_g16to16ck_x_end_without_alpha
_g16to16ck_higherhalf_higherwrite:
		movd	eax,mm1
		shr		eax,16
		inc		ecx
		mov		[edi+8*ecx+6-8],ax
		jnz		_g16to16ck_nextx_without_alpha
		jmp		_g16to16ck_x_end_without_alpha
	

_16to16ckmini_:
        mov     edx,y
		lea		esi,[esi+2*ecx]
		lea		edi,[edi+2*ecx]
		push	esi edi
		neg		ecx
		or		_16to16ckConvertWithAlpha,0h
		jne		_g16to16ck_nexty_with_alpha
_g16to16ckmini_nexty_without_alpha:
		push	esi edi ecx
_g16to16ckmini_nextx_without_alpha:
		mov		ax,[esi+2*ecx] 	;
		and		eax,ebx
		cmp		ax,bp
		je		_g16to16ckmini_reject
		movd	mm0,eax		;
		movq	mm3,mm4
		movq	mm1,mm0		;
		pand	mm0,mm5
		movq	mm2,mm1		;
_g16to16ck_redshift1_mini:
		psllw	mm0,0h
		pand	mm1,mm6		;
		pand	mm2,mm7
_g16to16ck_greenshift1_mini:
		psllw	mm1,0h		;
		por		mm3,mm0
_g16to16ck_blueshift1_mini:
		psllw	mm2,0h		;
		por		mm3,mm1
_g16to16ck_higherbitmask_mini:
		and		eax,12345678h	;
		por		mm3,mm2
		movd	mm0,eax		;
		movq	mm1,mm0		;
		movq	mm2,mm0
		pand	mm0,mm5		;
		pand	mm1,mm6
_g16to16ck_redshift2_mini:
		psllw	mm0,0h		;
		pand	mm2,mm7
_g16to16ck_greenshift2_mini:
		psllw	mm1,0h		;
		por		mm3,mm0
_g16to16ck_blueshift2_mini:
		psllw	mm2,0h		;
		por		mm3,mm1
		por		mm3,mm2		;
		movd	eax,mm3
        mov     [edi+2*ecx-2],ax 	;
_g16to16ckmini_reject:
		inc		ecx
		jnz		_g16to16ckmini_nextx_without_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16ckmini_nexty_without_alpha
_g16to16ckminiexit:
		pop		edi esi
		ret


_g16to16ck_nexty_with_alpha:
		push	esi edi ecx edx
_g16to16ck_nextx_with_alpha:
		movq	mm1,[esi+8*ecx]	;
		movq	mm2,mm3			;ck	
		movd	eax,mm1		;
		movq	mm0,mm1
		pand	mm0,_16to16ckColorKeyMask ;
		pcmpeqw	mm2,mm0		;
		movd	ebp,mm2		;
		psrlq	mm2,32
		movq	mm0,mm1		;
		psrlq	mm1,32
		movd	ebx,mm1		;
		movq	mm1,mm0
		movd	edx,mm2		;
		pand	mm0,mm5				;red mask
		cmp		ebp,0FFFFFFFFh	;
		jne		_g16to16ck_notck_with_alpha
		cmp		edx,0FFFFFFFFh	;
		je		_g16to16ck_reject_with_alpha
_g16to16ck_notck_with_alpha:
_g16to16ck_redshift1_wa:
		psllw	mm0,0h		;
		movq	mm2,mm1
		pand	mm1,mm6		;		;green mask
_g16to16ck_greenshift1_wa:
		psllw	mm1,0h		;
		por		mm0,mm1		;
		movq	mm1,mm2
		pand	mm1,mm4		;		;alpha mask
_g16to16ck_alphashift1_wa:
		psllw	mm1,0h		;		;alpha feltolása max pozicioba (1 bit)
							;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7				;blue mask
		psraw	mm1,3h		;		;alpha felsõ bitjének másolása
_g16to16ck_higherbitmask_wa1:
		and		ebx,12345678h
_g16to16ck_blueshift1_wa:
		pslld	mm2,0h		;
_g16to16ck_higherbitmask_wa2:
		and		eax,12345678h
		por		mm0,mm2		;
_g16to16ck_alphashift2_wa:
		psllw	mm1,0h				;alpha betolása a végsõ helyre	
		movd	mm2,ebx		;
		por		mm0,mm1
		movd	mm1,eax		;
		psllq	mm2,32
		por		mm1,mm2		;
		movq	mm2,mm1		;
		pand	mm1,mm5				;red mask
_g16to16ck_redshift2_wa:
		psllw	mm1,0h		;		;red shr/shl
		por		mm0,mm1		;
		movq	mm1,mm2
		pand	mm1,mm6		;		;green mask
		pand	mm2,mm7				;blue mask
_g16to16ck_greenshift2_wa:
		psllw	mm1,0h		;		;green shr/shl

_g16to16ck_blueshift2_wa:
		psllw	mm2,0h		;		;blue shr/shl
		cmp		ebp,0FFFFFFFFh
		por		mm0,mm2		;
		por		mm1,mm0		;
		je		_g16to16cklowerhalf_with_alpha
		cmp		ebp,0FFFFh
		ja		_g16to16ck_lowerhalf_lowerwrite_with_alpha
		je		_g16to16ck_lowerhalf_higherwrite_with_alpha
		movd	[edi+8*ecx],mm1
_g16to16cklowerhalf_with_alpha:
		cmp		edx,0FFFFFFFFh
		je		_g16to16ck_reject_with_alpha

		psrlq	mm1,32
		cmp		edx,0FFFFh
		ja		_g16to16ck_higherhalf_lowerwrite_with_alpha
		je		_g16to16ck_higherhalf_higherwrite_with_alpha
        movd    [edi+8*ecx+4],mm1
		nop
_g16to16ck_reject_with_alpha:
		inc		ecx
        jnz     _g16to16ck_nextx_with_alpha
_g16to16ck_x_end_with_alpha:
		pop		edx ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16ck_nexty_with_alpha
		pop		edi esi ecx
		and		ecx,3h
		ret

_g16to16ck_lowerhalf_lowerwrite_with_alpha:
		movd	eax,mm1
		mov		[edi+8*ecx],ax
        jmp     _g16to16cklowerhalf_with_alpha
_g16to16ck_lowerhalf_higherwrite_with_alpha:
		movd	eax,mm1
		shr		eax,16
		mov		[edi+8*ecx+2],ax
        jmp     _g16to16cklowerhalf_with_alpha
_g16to16ck_higherhalf_lowerwrite_with_alpha:
		movd	eax,mm1
		mov		[edi+8*ecx+4],ax
		inc		ecx
		jnz		_g16to16ck_nextx_with_alpha
		jmp		_g16to16ck_x_end_with_alpha
_g16to16ck_higherhalf_higherwrite_with_alpha:
		movd	eax,mm1
		shr		eax,16
		inc		ecx
		mov		[edi+8*ecx+6 -8],ax
		jnz		_g16to16ck_nextx_with_alpha
		jmp		_g16to16ck_x_end_with_alpha


_g16to16ckmini_nexty_with_alpha:
		push	esi edi ecx
_g16to16ckmini_nextx_with_alpha:
		mov		ax,[esi+2*ecx] 	;
		and		eax,ebx
		cmp		ax,bp
		je		_g16to16ckmini_reject_with_alpha
		movd	mm0,eax		;
		movq	mm1,mm0		;
		movq	mm3,mm0
		pand	mm0,mm5		;
		movq	mm2,mm1
_g16to16ck_redshift1_wa_mini:
		psllw	mm0,0h		;
		pand	mm3,mm4
		pand	mm1,mm6		;
_g16to16ck_alphashift1_wa_mini:
		psllw	mm3,0h				;alpha feltolása max pozicioba (1 bit)
							;vagy max-1 pozicioba (4 bit)
		pand	mm2,mm7		;
		psraw	mm3,3h				;alpha felsõ bitjének másolása
_g16to16ck_greenshift1_wa_mini:
		psllw	mm1,0h		;
_g16to16ck_blueshift1_wa_mini:
		psllw	mm2,0h
		por		mm0,mm1		;
_g16to16ck_higherbitmask_wa_mini:
		and		eax,12345678h	
		por		mm0,mm2		;
_g16to16ck_alphashift2_wa_mini:
		psllw	mm3,0h				;alpha betolása a végsõ helyre
		movd	mm1,eax		;
		por		mm0,mm3
		movq	mm2,mm1		;
		pand	mm1,mm5
		movq	mm3,mm2		;
_g16to16ck_redshift2_wa_mini:
		psllw	mm1,0h		;		;red shr/shl
		pand	mm2,mm6
		por		mm0,mm1		;
_g16to16ck_greenshift2_wa_mini:
		psllw	mm2,0h		;		;green shr/shl
		pand	mm3,mm7
		por		mm0,mm2		;
_g16to16ck_blueshift2_wa_mini:
		psllw	mm3,0h		;		;blue shr/shl
		por		mm0,mm3		;
		por		mm0,mm1		;
		movd	eax,mm0		;
        mov     [edi+2*ecx-2],ax 	;
_g16to16ckmini_reject_with_alpha:
		inc		ecx
		jnz		_g16to16ckmini_nextx_with_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g16to16ckmini_nexty_with_alpha
		jmp		_g16to16ckminiexit

EndOf16to16ckConvs	LABEL

;-------------------------------------------------------------------
;Init kód a 16->16 konverterhez (colorkey-jel)

_16to16ckInitConvCode:
        CheckInit16To16CK
		push	OFFSET _16to16cklastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _16to16cklastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_16to16ckConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,[edx].ABitCount		;dst alpha bitcount
		mov		ebp,[ebx].ABitCount		;src alpha bitcount
		jecxz	_16to16ck_noalpha
		mov		eax,constalpha
		mov		ecx,[edx].APos
		or		ebp,ebp
		je		_16to16ck_alphaok
		mov		_16to16ckConvertWithAlpha,1h
		mov		eax,[edx].ABitCount		;dst alpha bitcount
		mov		al,_masktable16[8*eax+ebp-1*8-1]
		mov		ecx,[ebx].APos
_16to16ck_alphaok:
		shl		eax,cl
_16to16ck_noalpha:
		push	edx esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ckARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _16to16ckARGBEntry_AlphaMask[4],eax
		mov		eax,[ebx].RBitCount
		mov		ecx,[edx].RBitCount
		movzx	ebp,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,[ebx].RPos
		shl		eax,cl
		shl		ebp,cl
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ckARGBEntry_RedMask[0],eax
		mov		DWORD PTR _16to16ckARGBEntry_RedMask[4],eax
		mov		eax,[ebx].GBitCount
		mov		ecx,[edx].GBitCount
		movzx	esi,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,[ebx].GPos
		shl		eax,cl
		shl		esi,cl
		or		ebp,esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ckARGBEntry_GreenMask[0],eax
		mov		DWORD PTR _16to16ckARGBEntry_GreenMask[4],eax
		mov		eax,[ebx].BBitCount
		mov		ecx,[edx].BBitCount
		movzx	esi,_masktable_magicfor16[8*ecx+eax-1*8-1]
		mov		al,_masktable16[8*ecx+eax-1*8-1]
		mov		ecx,[ebx].BPos
		shl		eax,cl
		shl		esi,cl
		or		ebp,esi
		push	ax
		shl		eax,10h
		pop		ax
		mov		DWORD PTR _16to16ckARGBEntry_BlueMask[0],eax
		mov		DWORD PTR _16to16ckARGBEntry_BlueMask[4],eax
	
		push	bp
		shl		ebp,10h
		pop		bp
		
		pop		esi edx

		GetShiftCode_16to16CK	[ebx].RPos, [ebx].RBitCount, [edx].RPos, [edx].RBitCount
		GetShiftCode_16to16CK	[ebx].GPos, [ebx].GBitCount, [edx].GPos, [edx].GBitCount
		GetShiftCode_16to16CK	[ebx].BPos, [ebx].BBitCount, [edx].BPos, [edx].BBitCount

		or		_16to16ckConvertWithAlpha,0h
		jne		_16to16ckInitConverterWithAlpha

        mov     DWORD PTR _g16to16ck_higherbitmask1+2,ebp
        mov     DWORD PTR _g16to16ck_higherbitmask2+1,ebp
        mov     DWORD PTR _g16to16ck_higherbitmask_mini+1,ebp

		pop	eax
        and     WORD PTR _g16to16ck_blueshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift2+2,ax
        and     WORD PTR _g16to16ck_blueshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16ck_blueshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift1+2,ax
        and     WORD PTR _g16to16ck_blueshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift1_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16ck_greenshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift2+2,ax
        and     WORD PTR _g16to16ck_greenshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16ck_greenshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift1+2,ax
        and     WORD PTR _g16to16ck_greenshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift1_mini+2,ax

		pop	eax
        and     WORD PTR _g16to16ck_redshift2+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift2+2,ax
        and     WORD PTR _g16to16ck_redshift2_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift2_mini+2,ax
		pop	eax
        and     WORD PTR _g16to16ck_redshift1+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift1+2,ax
        and     WORD PTR _g16to16ck_redshift1_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift1_mini+2,ax

		ret	

_16to16ckInitConverterWithAlpha:
		push	edx
		mov		ecx,[ebx].ABitCount
		mov		eax,[edx].ABitCount
		movzx	edx,_masktable_magicfor16[8*eax+ecx-1*8h-1]
		push	LARGE 16
		cmp		ecx,eax
		pop		eax
		jb		_16to16ckInitConvWithAlphaMaxPos1
		dec		eax
_16to16ckInitConvWithAlphaMaxPos1:
		push	eax ecx
		mov		ecx,[ebx].APos
		shl		edx,cl
		sub		eax,ecx
		pop		ecx
		sub		eax,ecx
		or		ah,20h
		jns		_16to16ckInitConvWithAlphaMMXShiftCodeOk1
		neg		eax
_16to16ckInitConvWithAlphaMMXShiftCodeOk1:
		xchg	al,ah
        and     WORD PTR _g16to16ck_alphashift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_alphashift1_wa+2,ax
        and     WORD PTR _g16to16ck_alphashift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_alphashift1_wa_mini+2,ax
		push	dx
		shl		edx,10h
		pop		dx
		or		edx,ebp
        mov     DWORD PTR _g16to16ck_higherbitmask_wa1+2,edx
        mov     DWORD PTR _g16to16ck_higherbitmask_wa2+1,edx
        mov     DWORD PTR _g16to16ck_higherbitmask_wa_mini+1,edx
		pop		ecx
		pop		edx
		mov		eax,[edx].APos
		add		eax,[edx].ABitCount
		cmp		cl,16
		je		_16to16ckInitConvWithAlphaMaxPos2
		sub		cl,3h
_16to16ckInitConvWithAlphaMaxPos2:	
		sub		eax,ecx
		or		ah,20h
		jns		_16to16ckInitConvWithAlphaMMXShiftCodeOk2
		neg		eax
_16to16ckInitConvWithAlphaMMXShiftCodeOk2:
		xchg	al,ah
        and     WORD PTR _g16to16ck_alphashift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_alphashift2_wa+2,ax
        and     WORD PTR _g16to16ck_alphashift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_alphashift2_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g16to16ck_blueshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift2_wa+2,ax
        and     WORD PTR _g16to16ck_blueshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to16ck_blueshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift1_wa+2,ax
        and     WORD PTR _g16to16ck_blueshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_blueshift1_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g16to16ck_greenshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift2_wa+2,ax
        and     WORD PTR _g16to16ck_greenshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to16ck_greenshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift1_wa+2,ax
        and     WORD PTR _g16to16ck_greenshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_greenshift1_wa_mini+2,ax

		pop		eax
	    and     WORD PTR _g16to16ck_redshift2_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift2_wa+2,ax
        and     WORD PTR _g16to16ck_redshift2_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift2_wa_mini+2,ax
		pop		eax
        and     WORD PTR _g16to16ck_redshift1_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift1_wa+2,ax
        and     WORD PTR _g16to16ck_redshift1_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g16to16ck_redshift1_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 16->16 konverter írhatóvá tétele

_16to16ck_doinit:
        push    ecx edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE EndOf16to16ckConvs - BeginOf16to16ckConvs
        push    OFFSET BeginOf16to16ckConvs
        W32     VirtualProtect,4
        mov     _16to16ckInit,0FFh

        pop     eax edx ecx
        ret
	

.data

ALIGN 4
_16to16ckARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_16to16ckARGBEntry_RedMask		DQ	000FF000000FF0000h
_16to16ckARGBEntry_GreenMask	DQ	00000FF000000FF00h
_16to16ckARGBEntry_BlueMask		DQ	0000000FF000000FFh
_16to16ckColorKeyMask			DQ	?
_16to16cklastUsedPixFmt_Src		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to16cklastUsedPixFmt_Dst		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_16to16ckInit					DB	0h
_16to16ckConvertWithAlpha		DB	0h
