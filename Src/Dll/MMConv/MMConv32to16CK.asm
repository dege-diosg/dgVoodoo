;-----------------------------------------------------------------------------
; MMCONV32TO16.ASM - MultiMedia Converter lib, convering from 32 bit format to
;                    16 bit format with colorkeying capability
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
; MMConv:	MMConv32to16CK.ASM
;		32 bites formátumról 16 bitesre konvertáló rész
;		  colorkey használata
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;............................. Makrók ..............................

CheckInit32To16CK	MACRO
LOCAL   InitOk

        or      _32to16ckInit,0h
        jne     InitOk
        call    _32to16ck_doinit
InitOk:

ENDM


GetSrcComponentMask_32to16CK	MACRO	DstComponentSize, SrcComponentPos

		mov		eax,DstComponentSize
		mov		al,_masktable_up[eax]
		mov		ecx,SrcComponentPos
		shl		eax,cl

ENDM


GetShiftCode_32to16CK	MACRO	SrcPos, DstPos, DstBitCount
LOCAL	MMXShiftCodeOk
LOCAL	MMXUpperShiftcodeOk

		mov		eax,DstPos
		add		eax,DstBitCount
		sub		eax,SrcPos
		sub		eax,8			;Src bitcount
		push	eax
		or		ah,20h
		jns		MMXShiftCodeOk
		neg		eax
MMXShiftCodeOk:
		xchg	al,ah
		pop		ecx
		push	eax
		add		ecx,16
		or		ch,20h
		jns		MMXUpperShiftcodeOk
		neg		ecx
MMXUpperShiftcodeOk:
		xchg	cl,ch
		push	ecx

ENDM


;-------------------------------------------------------------------
;........................... Függvények ............................

.code

BeginOf32to16ckConvs	LABEL

;-------------------------------------------------------------------
; Általános 32->16 konverter

_ckgeneral32to16converter:
		lea		eax,_32to16cklastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_32to16ckCP_newformat
		lea		eax,_32to16cklastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_32to16ckCP_format_cached
_32to16ckCP_newformat:
		call	_32to16ckInitConvCode
_32to16ckCP_format_cached:

		movq	mm4,_32to16ckARGBEntry_AlphaMask
		movq	mm5,_32to16ckARGBEntry_RedMask
		movq	mm6,_32to16ckARGBEntry_GreenMask
		movq	mm7,_32to16ckARGBEntry_BlueMask
		mov		ebx,colorkey
		mov		ebp,colorKeyMask
		mov		ecx,x
		cmp		ecx,2h
		jb		_32to16ckmini_
		je		_32to16ck

;mivel a 32->16 konverter jelenleg mindig csak videómemória -> systemmem irányban használatos,
;inkább a forrást igazítjuk 8 byte-ra
		test	esi,111b		;forrás 8-cal osztható címen?
        je      _32to16ck
		call	_32to16ckmini_
_32to16ck:
		call	_32to16ck_
		or		ecx,ecx
		jnz		_32to16ckmini_
		ret

_32to16ck_:
		movd	mm3,ebx
		punpckldq mm3,mm3
		xchg	eax,ebp
		movd	ebp,mm4
		movd	mm4,eax
		punpckldq mm4,mm4
		movq	_32to16ckColorKeyMask,mm4
        mov     edx,y
		push	ecx
		shr		ecx,1h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+4*ecx]
		push	esi edi
		neg		ecx
		push	eax eax
		or		_32to16ckConvertWithAlpha,0h
		jne		_g32to16ck_nexty_with_alpha
_g32to16ck_nexty_without_alpha:
		push	esi edi ecx
_g32to16ck_nextx_without_alpha:
		movq    mm0,[esi+8*ecx]		;? az eredeti pr alapján nem párosítható	
		movq    mm1,mm0			;
		pand	mm0,mm4
		pcmpeqd	mm0,mm3			;
		movq	mm2,mm1
		packssdw mm0,mm0		;
		pand    mm1,mm6
		movd	ebx,mm0			;
		movq	mm0,mm2
		cmp		ebx,0FFFFFFFFh		;
		je		_g32to16ck_no_conv_without_alpha
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to16ck_redshift:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to16ck_greenshift:
        pslld   mm1,0h  	;
		cmp		ebx,0FFFFh
_g32to16ck_blueshift:
        pslld   mm2,0h  	;
        por     mm0,mm1
		movd	mm1,ebp		;
        por     mm0,mm2
		punpckldq mm1,mm1	;
        por     mm0,mm1 	;
		psrad	mm0,16		;
		packssdw mm0,mm0	;
		ja		_g32to16ck_lowerpixel
		je		_g32to16ck_higherpixel
        movd    [edi+4*ecx],mm0 ;
_g32to16ck_no_conv_without_alpha:
		inc		ecx
        jnz     _g32to16ck_nextx_without_alpha
_g32to16ck_endofrow:
		pop		ecx edi esi
        add     esi,srcpitch
        add     edi,dstpitch
        dec     edx
        jne     _g32to16ck_nexty_without_alpha
_g32to16ckexit:
		pop		edi esi ecx eax eax
		and		ecx,1h
		ret

_g32to16ck_lowerpixel:
		movd	eax,mm0
		mov		[edi+4*ecx],ax
		inc		ecx
		jnz		_g32to16ck_nextx_without_alpha
		jmp		_g32to16ck_endofrow

_g32to16ck_higherpixel:
		movd	eax,mm0
		shr		eax,16
		mov		[edi+4*ecx+2],ax
		inc		ecx
		jnz		_g32to16ck_nextx_without_alpha
		jmp		_g32to16ck_endofrow
	

_32to16ckmini_:
        mov     edx,y
		push	esi edi
        or      _32to16ckConvertWithAlpha,0h
        jne     _g32to16ck_nexty_with_alpha
_g32to16ckmini_nexty_without_alpha:
		movd    mm0,[esi]	;
		movd	eax,mm0		;
        movq    mm1,mm0
		and		eax,ebp		;
		cmp		eax,ebx
		je		_g32to16ckmini_no_conv_without_alpha
        movq    mm2,mm0
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to16ck_redshift_mini:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to16ck_greenshift_mini:
        pslld   mm1,0h  	;
_g32to16ck_blueshift_mini:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
		movd	eax,mm0		;
        mov     [edi],ax 	;
_g32to16ckmini_no_conv_without_alpha:
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to16ckmini_nexty_without_alpha
_g32to16ckminiexit:
		dec		ecx
		pop		edi esi
		add		esi,4h
		add		edi,2h
		ret


_g32to16ck_nexty_with_alpha:
		push	esi edi ecx
_g32to16ck_nextx_with_alpha:
		movq    mm0,[esi+8*ecx]		;? az eredeti pr alapján nem párosítható
		movq    mm1,mm0			;
		pand	mm0,[esp + 3*4]
		pcmpeqd	mm0,mm3			;
		movq	mm2,mm1
		packssdw mm0,mm0		;
		pand    mm1,mm6
		movd	ebx,mm0			;
		movq	mm0,mm2
		cmp		ebx,0FFFFFFFFh		;
		je		_g32to16ck_no_conv_with_alpha
        pand    mm0,mm5 	;
        movq    mm4,mm0
_g32to16ck_redshift_wa:
        pslld   mm0,0h  	;
        pand    mm1,mm6
_g32to16ck_greenshift_wa:
        pslld   mm1,0h  	;
        pand    mm2,mm7
		por     mm0,mm1 	;
_g32to16ck_blueshift_wa:
        pslld   mm2,0h
		movd	mm1,ebp		;
        por     mm0,mm2
		punpckldq mm1,mm1	;
		cmp		ebx,0FFFFh
        pand    mm4,mm1		;
_g32to16ck_alphashift_wa:
        pslld   mm4,0h		;
        por     mm0,mm4 	;
		psrad	mm0,16		;
		packssdw mm0,mm0	;
		ja		_g32to16ck_lowerpixel_with_alpha
		je		_g32to16ck_higherpixel_with_alpha
        movd    [edi+4*ecx],mm0 ;
_g32to16ck_no_conv_with_alpha:
		inc		ecx
        jnz     _g32to16ck_nextx_with_alpha
_g32to16ck_endofrow_with_alpha:
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g32to16ck_nexty_with_alpha
		jmp		_g32to16ckexit

_g32to16ck_lowerpixel_with_alpha:
		movd	eax,mm0
		mov		[edi+4*ecx],ax
		inc		ecx
		jnz		_g32to16ck_nextx_with_alpha
		jmp		_g32to16ck_endofrow_with_alpha

_g32to16ck_higherpixel_with_alpha:
		movd	eax,mm0
		shr		eax,16
		mov		[edi+4*ecx+2],ax
		inc		ecx
		jnz		_g32to16ck_nextx_with_alpha
		jmp		_g32to16ck_endofrow_with_alpha
	

_g32to16ckmini_nexty_with_alpha:
		movd    mm0,[esi]
		movd	eax,mm0		;
        movq    mm1,mm0
		and		eax,ebp		;
		cmp		eax,ebx
		je		_g32to16ckmini_no_conv_with_alpha
        movq    mm2,mm0
        pand    mm0,mm5 ;
        movq    mm3,mm0
_g32to16ck_redshift_wa_mini:
        pslld   mm0,0h  ;
        pand    mm1,mm6
_g32to16ck_greenshift_wa_mini:
        pslld   mm1,0h  ;
        pand    mm2,mm7
        pand    mm3,mm4 ;
_g32to16ck_blueshift_wa_mini:
        pslld   mm2,0h
        por     mm0,mm1 ;
_g32to16ck_alphashift_wa_mini:
        pslld   mm3,0h
        por     mm0,mm2 ;
        por     mm0,mm3 ;
		movd	eax,mm0
        mov     [edi],ax ;
_g32to16ckmini_no_conv_with_alpha:
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz     _g32to16ckmini_nexty_with_alpha
		jmp		_g32to16ckminiexit

EndOf32to16ckConvs	LABEL

;-------------------------------------------------------------------
;Init kód a 32->16 konverterhez

_32to16ckInitConvCode:
		CheckInit32To16CK
		push	OFFSET _32to16cklastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _32to16cklastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_32to16ckConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,[edx].ABitCount		;dst alpha bitcount
		mov		ebp,[ebx].ABitCount		;src alpha bitcount
		jecxz	_32to16ck_noalpha
		mov		eax,constalpha
		mov		cl,_invshrcnt[ecx]
		shr		eax,cl
		mov		ecx,[edx].APos
		or		ebp,ebp
        je      _32to16ck_alphaok
        mov     _32to16ckConvertWithAlpha,1h
		mov		al,_masktable_up[ebp]
		mov		ecx,[ebx].APos
_32to16ck_alphaok:
		shl		eax,cl
_32to16ck_noalpha:
		mov		DWORD PTR _32to16ckARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _32to16ckARGBEntry_AlphaMask[4],eax

	GetSrcComponentMask_32to16CK	[edx].RBitCount, [ebx].RPos
        mov     DWORD PTR _32to16ckARGBEntry_RedMask[0],eax
        mov     DWORD PTR _32to16ckARGBEntry_RedMask[4],eax
	GetSrcComponentMask_32to16CK	[edx].GBitCount, [ebx].GPos
        mov     DWORD PTR _32to16ckARGBEntry_GreenMask[0],eax
        mov     DWORD PTR _32to16ckARGBEntry_GreenMask[4],eax
	GetSrcComponentMask_32to16CK	[edx].BBitCount, [ebx].BPos
        mov     DWORD PTR _32to16ckARGBEntry_BlueMask[0],eax
        mov     DWORD PTR _32to16ckARGBEntry_BlueMask[4],eax

		GetShiftCode_32to16CK	[ebx].RPos, [edx].RPos, [edx].RBitCount
		GetShiftCode_32to16CK	[ebx].GPos, [edx].GPos, [edx].GBitCount
		GetShiftCode_32to16CK	[ebx].BPos, [edx].BPos, [edx].BBitCount

		or		_32to16ckConvertWithAlpha,0h
		jne		_32to16ckInitConverterWithAlpha

		pop		eax
        and     WORD PTR _g32to16ck_blueshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_blueshift+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_blueshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_blueshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16ck_greenshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_greenshift+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_greenshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_greenshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16ck_redshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_redshift+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_redshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_redshift_mini+2,ax

		ret	

_32to16ckInitConverterWithAlpha:
		pop		eax
        and     WORD PTR _g32to16ck_blueshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_blueshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_blueshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_blueshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16ck_greenshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_greenshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_greenshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_greenshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16ck_redshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_redshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_redshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_redshift_wa_mini+2,ax

		GetShiftCode_32to16CK	[ebx].APos, [edx].APos, [edx].ABitCount
		pop		eax
        and     WORD PTR _g32to16ck_alphashift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_alphashift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16ck_alphashift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16ck_alphashift_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 32->16 ck konverter írhatóvá tétele

_32to16ck_doinit:
        push    ecx edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE EndOf32to16ckConvs - BeginOf32to16ckConvs
        push    OFFSET BeginOf32to16ckConvs
        W32     VirtualProtect,4
        mov     _32to16ckInit,0FFh

        pop     eax edx ecx
        ret
	

.data

_32to16ckARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_32to16ckARGBEntry_RedMask		DQ	000FF000000FF0000h
_32to16ckARGBEntry_GreenMask	DQ	00000FF000000FF00h
_32to16ckARGBEntry_BlueMask		DQ	0000000FF000000FFh
_32to16ckColorKeyMask			DQ	?
_32to16cklastUsedPixFmt_Src		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to16cklastUsedPixFmt_Dst		_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to16ckInit					DB	0h
_32to16ckConvertWithAlpha		DB	0h
