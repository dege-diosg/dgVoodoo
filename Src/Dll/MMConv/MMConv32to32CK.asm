;-----------------------------------------------------------------------------
; MMCONV32TO32CK.ASM - MultiMedia Converter lib, convering from 32 bit format to
;                      another 32 bit format with colorkeying capability
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
; MMConv:	MMConv32to32.ASM
;		32 bites formátumról 32 bitesre konvertáló rész
;
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;........................... Makrók ................................

CheckInit32To32CK	MACRO
LOCAL   InitOk

        or      _32to32ckInit,0h
        jne     InitOk
        call    _32to32ck_doinit
InitOk:

ENDM

GetShiftCode_32to32CK	MACRO	SrcPos, DstPos
LOCAL	MMXShiftCodeOk

		mov		eax,DstPos
		sub		eax,SrcPos
		or		ah,20h
		jns		MMXShiftCodeOk
		neg		eax
MMXShiftCodeOk:
		xchg	al,ah
		push	eax

ENDM


;-------------------------------------------------------------------
;........................... Függvények ............................

.code

BeginOf32to32ckConvs	LABEL	NEAR

;-------------------------------------------------------------------
; Általános 32->32 konverter colorkey-jel

_ckgeneral32to32converter:
		lea		eax,_32to32cklastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_32to32ckCP_newformat
		lea		eax,_32to32cklastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_32to32ckCP_format_cached
_32to32ckCP_newformat:
		call	_32to32ckInitConvCode
_32to32ckCP_format_cached:

		movq	mm4,_32to32ckARGBEntry_AlphaMask
		movq	mm5,_32to32ckARGBEntry_RedMask
		movq	mm6,_32to32ckARGBEntry_GreenMask
		movq	mm7,_32to32ckARGBEntry_BlueMask
		mov		ebp,colorKeyMask
		mov		ebx,colorkey
		mov		ecx,x
		cmp		ecx,2h
		jb		_32to32ckmini_
		je		_32to32ck

;mivel a 32->32 konverter jelenleg mindig csak videómemória -> systemmem irányban használatos,
;inkább a forrást igazítjuk 8 byte-ra
		test	esi,111b		;forrás 8-cal osztható címen?
        je      _32to32ck
		call	_32to32ckmini_
_32to32ck:
		call	_32to32ck_
		or		ecx,ecx
		jnz		_32to32ckmini_
		ret

_32to32ck_:
        mov     edx,y
		push	ecx
		shr		ecx,1h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+8*ecx]
		push	esi
		push	edi
		neg		ecx
		push	ebp
		push	ebp
		movd	mm3,ebx
		punpckldq mm3,mm3
		or		_32to32ckConvertWithAlpha,0h
		jne	_g32to32ck_nexty_with_alpha
_g32to32ck_nexty_without_alpha:
		push	esi
		push	edi
		push	ecx
_g32to32ck_nextx_without_alpha:
 		movq    mm0,[esi+8*ecx] ;
        movq    mm1,mm0 	;
		pand	mm0,[esp + 3*4]
		pcmpeqd	mm0,mm3		;
        movq    mm2,mm1
		packssdw mm0,mm0	;
        pand    mm1,mm6
		movd	ebx,mm0		;
		movq	mm0,mm2
		cmp		ebx,0FFFFFFFFh	;
		je		_g32to32ck_no_conv_without_alpha
        pand    mm0,mm5		;
		cmp	ebx,0FFFFh
_g32to32ck_redshift:
        pslld   mm0,0h 		;
        pand    mm2,mm7
_g32to32ck_greenshift:
        pslld   mm1,0h  	;
_g32to32ck_blueshift:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
		ja		_g32to32ck_lowerpixel
		je		_g32to16ck_higherpixel
        movq    [edi+8*ecx],mm0 ;
_g32to32ck_no_conv_without_alpha:
		inc		ecx
        jnz     _g32to32ck_nextx_without_alpha
_g32to32ck_endofrow:
		pop		ecx
		pop		edi
		pop		esi
        add     esi,srcpitch
        add     edi,dstpitch
        dec     edx
        jne     _g32to32ck_nexty_without_alpha
_g32to32ckexit:
		pop		edi
		pop		esi
		pop		ecx
		pop		eax
		pop		eax
		and		ecx,1h
		ret

_g32to32ck_lowerpixel:
		movd	eax,mm0
		mov		[edi+8*ecx],eax
		inc		ecx
        jnz     _g32to32ck_nextx_without_alpha
		jmp		_g32to32ck_endofrow

_g32to32ck_higherpixel:
		psrlq	mm0,32
		movd	eax,mm0
		mov		[edi+8*ecx+4],eax
		inc		ecx
        jnz     _g32to32ck_nextx_without_alpha
		jmp		_g32to32ck_endofrow


_32to32ckmini_:
        mov     edx,y
		push	esi
		push	edi
        or      _32to32ckConvertWithAlpha,0h
        jne     _g32to32ck_nexty_with_alpha
_g32to32ckmini_nexty_without_alpha:
		movd    mm0,DWORD PTR [esi]	;
        movq    mm1,mm0 	;
		movd	eax,mm0
		and		eax,ebp
		cmp		eax,ebx
		je		_g32to32ckmini_no_conv_without_alpha
        movq    mm2,mm0
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to32ck_redshift_mini:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to32ck_greenshift_mini:
        pslld   mm1,0h  	;
_g32to32ck_blueshift_mini:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
        movd    DWORD PTR [edi],mm0 	;
_g32to32ckmini_no_conv_without_alpha:
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to32ckmini_nexty_without_alpha
_g32to32ckminiexit:
		dec		ecx
		pop		edi
		pop		esi
		add		esi,4h
		add		edi,4h
		ret


_g32to32ck_nexty_with_alpha:
		push	esi
		push	edi
		push	ecx
_g32to32ck_nextx_with_alpha:
		movq    mm0,[esi+8*ecx] ;
        movq    mm1,mm0 	;
		pand	mm0,[esp + 3*4]
		pcmpeqd	mm0,mm3		;
        movq    mm2,mm1
		packssdw mm0,mm0	;
        pand    mm1,mm6
		movd	ebx,mm0		;
		movq	mm0,mm2
		cmp		ebx,0FFFFFFFFh	;
		je		_g32to32ck_no_conv_without_alpha
        pand    mm0,mm5		;
		cmp		ebx,0FFFFh
_g32to32ck_redshift_wa:
        pslld   mm0,0h  	;
        pand    mm1,mm6
_g32to32ck_greenshift_wa:
        pslld   mm1,0h  	;
        por     mm0,mm1 	;
		movq	mm1,mm2
        pand    mm2,mm7		;
        pand    mm1,mm4
_g32to32ck_blueshift_wa:
        pslld   mm2,0h		;
_g32to32ck_alphashift_wa:
        pslld   mm1,0h		;
        por     mm0,mm2
        por     mm0,mm1 	;
		ja		_g32to32ck_lowerpixel_with_alpha
		je		_g32to16ck_higherpixel_with_alpha
        movq    [edi+8*ecx],mm0 ;
        inc	ecx
        jnz     _g32to32ck_nextx_with_alpha
		pop		ecx
		pop		edi
		pop		esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g32to32ck_nexty_with_alpha
		jmp		_g32to32ckexit	

_g32to32ck_lowerpixel_with_alpha:
		movd	eax,mm0
		mov		[edi+8*ecx],eax
		inc		ecx
        jnz     _g32to32ck_nextx_with_alpha
		jmp		_g32to32ck_endofrow

_g32to32ck_higherpixel_with_alpha:
		psrlq	mm0,32
		movd	eax,mm0
		mov		[edi+8*ecx+4],eax
		inc		ecx
		jnz     _g32to32ck_nextx_with_alpha
		jmp		_g32to32ck_endofrow


_g32to32ckmini_nexty_with_alpha:
		movd    mm0,DWORD PTR [esi]
        movq    mm1,mm0 ;
		movd	eax,mm0
        movq    mm2,mm0
		and		eax,ebp
		cmp		eax,ebx
		je		_g32to32ckmini_no_conv_with_alpha
        pand    mm0,mm5 ;
        movq    mm3,mm0
_g32to32ck_redshift_wa_mini:
        pslld   mm0,0h  ;
        pand    mm1,mm6
_g32to32ck_greenshift_wa_mini:
        pslld   mm1,0h  ;
        pand    mm2,mm7
        pand    mm3,mm4 ;
_g32to32ck_blueshift_wa_mini:
        pslld   mm2,0h
        por     mm0,mm1 ;
_g32to32ck_alphashift_wa_mini:
        pslld   mm3,0h
        por     mm0,mm2 ;
        por     mm0,mm3 ;
        movd    DWORD PTR [edi],mm0 ;
_g32to32ckmini_no_conv_with_alpha:
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to32ckmini_nexty_with_alpha
		jmp		_g32to32ckminiexit

EndOf32to32ckConvs	LABEL	NEAR

;-------------------------------------------------------------------
;Init kód a 32->32 ck konverterhez

_32to32ckInitConvCode:
		CheckInit32To32CK
		push	OFFSET _32to32cklastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _32to32cklastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_32to32ckConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,(_pixfmt PTR [edx]).ABitCount		;dst alpha bitcount
		mov		ebp,(_pixfmt PTR [ebx]).ABitCount		;src alpha bitcount
		jecxz	_32to32ck_noalpha
		mov		ecx,(_pixfmt PTR [edx]).APos
		mov		eax,constalpha
		or		ebp,ebp
        je      _32to32ck_alphaok
        mov     _32to32ckConvertWithAlpha,1h
		mov		al,0FFh
		mov		ecx,(_pixfmt PTR [ebx]).APos
_32to32ck_alphaok:
		shl		eax,cl
_32to32ck_noalpha:
		mov		DWORD PTR _32to32ckARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _32to32ckARGBEntry_AlphaMask[4],eax
		mov		eax,0FFh
		mov		ecx,(_pixfmt PTR [ebx]).RPos
		shl		eax,cl
        mov     DWORD PTR _32to32ckARGBEntry_RedMask[0],eax
        mov     DWORD PTR _32to32ckARGBEntry_RedMask[4],eax
		mov		eax,0FFh
		mov		ecx,(_pixfmt PTR [ebx]).GPos
		shl		eax,cl
        mov     DWORD PTR _32to32ckARGBEntry_GreenMask[0],eax
        mov     DWORD PTR _32to32ckARGBEntry_GreenMask[4],eax
		mov		eax,0FFh
		mov		ecx,(_pixfmt PTR [ebx]).BPos
		shl		eax,cl
        mov     DWORD PTR _32to32ckARGBEntry_BlueMask[0],eax
        mov     DWORD PTR _32to32ckARGBEntry_BlueMask[4],eax

		GetShiftCode_32to32CK	(_pixfmt PTR [ebx]).RPos, (_pixfmt PTR [edx]).RPos
		GetShiftCode_32to32CK	(_pixfmt PTR [ebx]).GPos, (_pixfmt PTR [edx]).GPos
		GetShiftCode_32to32CK	(_pixfmt PTR [ebx]).BPos, (_pixfmt PTR [edx]).BPos

		or		_32to32ckConvertWithAlpha,0h
		jne		_32to32ckInitConverterWithAlpha

		pop		eax
        and     WORD PTR _g32to32ck_blueshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_blueshift+2,ax
        and     WORD PTR _g32to32ck_blueshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_blueshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32ck_greenshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_greenshift+2,ax
        and     WORD PTR _g32to32ck_greenshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_greenshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32ck_redshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_redshift+2,ax
        and     WORD PTR _g32to32ck_redshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_redshift_mini+2,ax

		ret	

_32to32ckInitConverterWithAlpha:
		pop		eax
        and     WORD PTR _g32to32ck_blueshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_blueshift_wa+2,ax
        and     WORD PTR _g32to32ck_blueshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_blueshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32ck_greenshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_greenshift_wa+2,ax
        and     WORD PTR _g32to32ck_greenshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_greenshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32ck_redshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_redshift_wa+2,ax
        and     WORD PTR _g32to32ck_redshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_redshift_wa_mini+2,ax

		GetShiftCode_32to32CK	(_pixfmt PTR [ebx]).APos, (_pixfmt PTR [edx]).APos
		pop		eax
        and     WORD PTR _g32to32ck_alphashift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_alphashift_wa+2,ax
        and     WORD PTR _g32to32ck_alphashift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32ck_alphashift_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 32->32 konverter írhatóvá tétele

_32to32ck_doinit:
        push    ecx
        push	edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE (EndOf32to32ckConvs - BeginOf32to32ckConvs)
        push    OFFSET BeginOf32to32ckConvs
        W32     VirtualProtect,4
        mov     _32to32ckInit,0FFh

        pop     eax
        pop		edx
        pop		ecx
        ret
	

.data

_32to32ckARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_32to32ckARGBEntry_RedMask		DQ	000FF000000FF0000h
_32to32ckARGBEntry_GreenMask	DQ	00000FF000000FF00h
_32to32ckARGBEntry_BlueMask		DQ	0000000FF000000FFh
_32to32cklastUsedPixFmt_Src		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to32cklastUsedPixFmt_Dst		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to32ckInit					DB	0h
_32to32ckConvertWithAlpha		DB	0h
