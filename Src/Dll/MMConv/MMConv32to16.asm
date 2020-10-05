;-----------------------------------------------------------------------------
; MMCONV32TO16.ASM - MultiMedia Converter lib, convering from 32 bit format to
;                    16 bit format
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
; MMConv:	MMConv32to16.ASM
;		32 bites form�tumr�l 16 bitesre konvert�l� r�sz
;
;-------------------------------------------------------------------

;-------------------------------------------------------------------
;............................. Makr�k ..............................

CheckInit32To16	MACRO
LOCAL   InitOk

        or      _32to16Init,0h
        jne     InitOk
        call    _32to16_doinit
InitOk:

ENDM


GetSrcComponentMask_32to16	MACRO	DstComponentSize, SrcComponentPos

		mov		eax,DstComponentSize
		mov		al,_masktable_up[eax]
		mov		ecx,SrcComponentPos
		shl		eax,cl

ENDM


GetShiftCode_32to16	MACRO	SrcPos, DstPos, DstBitCount
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
;........................... F�ggv�nyek ............................

.code

BeginOf32to16Convs	LABEL

;-------------------------------------------------------------------
; �ltal�nos 32->16 konverter

_general32to16converter:
		lea		eax,_32to16lastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_32to16CP_newformat
		lea		eax,_32to16lastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_32to16CP_format_cached
_32to16CP_newformat:
		call	_32to16InitConvCode
_32to16CP_format_cached:

		movq	mm4,_32to16ARGBEntry_AlphaMask
		movq	mm5,_32to16ARGBEntry_RedMask
		movq	mm6,_32to16ARGBEntry_GreenMask
		movq	mm7,_32to16ARGBEntry_BlueMask
		mov		ecx,x
		cmp		ecx,2h
		jb		_32to16mini_
		je		_32to16

;mivel a 32->16 konverter jelenleg mindig csak vide�mem�ria -> systemmem ir�nyban haszn�latos,
;ink�bb a forr�st igaz�tjuk 8 byte-ra
		test	esi,111b		;forr�s 8-cal oszthat� c�men?
        je      _32to16
		call	_32to16mini_
_32to16:call	_32to16_
		or		ecx,ecx
		jnz		_32to16mini_
		ret

_32to16_:
        mov     edx,y
		push	ecx
		shr		ecx,1h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+4*ecx]
		push	esi edi
		neg		ecx
		or		_32to16ConvertWithAlpha,0h
		jne		_g32to16_nexty_with_alpha
_g32to16_nexty_without_alpha:
		push	esi edi ecx
_g32to16_nextx_without_alpha:
 		movq    mm0,[esi+8*ecx] ;
        movq    mm1,mm0 	;
        movq    mm2,mm0
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to16_redshift:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to16_greenshift:
        pslld   mm1,0h  	;
	inc	ecx
_g32to16_blueshift:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
		psrad	mm0,16		;
		packssdw mm0,mm0	;
        movd    [edi+4*ecx-4],mm0 ;
        jnz     _g32to16_nextx_without_alpha
	pop	ecx edi esi
        add     esi,srcpitch
        add     edi,dstpitch
        dec     edx
        jne     _g32to16_nexty_without_alpha
_g32to16exit:
		pop		edi esi ecx
		and		ecx,1h
		ret

_32to16mini_:
        mov     edx,y
		push	esi edi
        or      _32to16ConvertWithAlpha,0h
        jne     _g32to16_nexty_with_alpha
_g32to16mini_nexty_without_alpha:
		movd    mm0,[esi]	;
        movq    mm1,mm0 	;
        movq    mm2,mm0
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to16_redshift_mini:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to16_greenshift_mini:
        pslld   mm1,0h  	;
_g32to16_blueshift_mini:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
		movd	eax,mm0		;
        mov     [edi],ax 	;
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to16mini_nexty_without_alpha
_g32to16miniexit:
		dec		ecx
		pop		edi esi
		add		esi,4h
		add		edi,2h
		ret


_g32to16_nexty_with_alpha:
		push	esi edi ecx
_g32to16_nextx_with_alpha:
		movq    mm0,[esi+8*ecx]	;
        movq    mm1,mm0 	;
        movq    mm2,mm0
        pand    mm0,mm5 	;
        movq    mm3,mm1
_g32to16_redshift_wa:
        pslld   mm0,0h  	;
        pand    mm1,mm6
_g32to16_greenshift_wa:
        pslld   mm1,0h  	;
        pand    mm2,mm7
        pand    mm3,mm4 	;
_g32to16_blueshift_wa:
        pslld   mm2,0h
        por     mm0,mm1 	;
_g32to16_alphashift_wa:
        pslld   mm3,0h
        por     mm0,mm2 	;
        inc		ecx
        por     mm0,mm3 	;
		psrad	mm0,16		;
		packssdw mm0,mm0	;
        movd    [edi+4*ecx-4],mm0 ;
        jnz     _g32to16_nextx_with_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g32to16_nexty_with_alpha
		jmp		_g32to16exit


_g32to16mini_nexty_with_alpha:
		push	esi edi
		movd    mm0,[esi]
        movq    mm1,mm0 ;
        movq    mm2,mm0
        pand    mm0,mm5 ;
        movq    mm3,mm0
_g32to16_redshift_wa_mini:
        pslld   mm0,0h  ;
        pand    mm1,mm6
_g32to16_greenshift_wa_mini:
        pslld   mm1,0h  ;
        pand    mm2,mm7
        pand    mm3,mm4 ;
_g32to16_blueshift_wa_mini:
        pslld   mm2,0h
        por     mm0,mm1 ;
_g32to16_alphashift_wa_mini:
        pslld   mm3,0h
        por     mm0,mm2 ;
        por     mm0,mm3 ;
		movd	eax,mm0
        mov     [edi],ax ;
		pop		edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to16mini_nexty_with_alpha
		jmp		_g32to16miniexit

EndOf32to16Convs	LABEL

;-------------------------------------------------------------------
;Init k�d a 32->16 konverterhez

_32to16InitConvCode:
		CheckInit32To16
		push	OFFSET _32to16lastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _32to16lastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_32to16ConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,[edx].ABitCount		;dst alpha bitcount
		mov		ebp,[ebx].ABitCount		;src alpha bitcount
		jecxz	_32to16_noalpha
		mov		eax,constalpha
		mov		cl,_invshrcnt[ecx]
		shr		eax,cl
		mov		ecx,[edx].APos
		or		ebp,ebp
        je      _32to16_alphaok
        mov     _32to16ConvertWithAlpha,1h
		mov		al,_masktable_up[ebp]
		mov		ecx,[ebx].APos
_32to16_alphaok:
		shl		eax,cl
_32to16_noalpha:
		mov		DWORD PTR _32to16ARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _32to16ARGBEntry_AlphaMask[4],eax

		GetSrcComponentMask_32to16	[edx].RBitCount, [ebx].RPos
        mov     DWORD PTR _32to16ARGBEntry_RedMask[0],eax
        mov     DWORD PTR _32to16ARGBEntry_RedMask[4],eax
		GetSrcComponentMask_32to16	[edx].GBitCount, [ebx].GPos
        mov     DWORD PTR _32to16ARGBEntry_GreenMask[0],eax
        mov     DWORD PTR _32to16ARGBEntry_GreenMask[4],eax
		GetSrcComponentMask_32to16	[edx].BBitCount, [ebx].BPos
        mov     DWORD PTR _32to16ARGBEntry_BlueMask[0],eax
        mov     DWORD PTR _32to16ARGBEntry_BlueMask[4],eax

		GetShiftCode_32to16	[ebx].RPos, [edx].RPos, [edx].RBitCount
		GetShiftCode_32to16	[ebx].GPos, [edx].GPos, [edx].GBitCount
		GetShiftCode_32to16	[ebx].BPos, [edx].BPos, [edx].BBitCount

		or		_32to16ConvertWithAlpha,0h
		jne		_32to16InitConverterWithAlpha

		pop		eax
        and     WORD PTR _g32to16_blueshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_blueshift+2,ax
		pop		eax
        and     WORD PTR _g32to16_blueshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_blueshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16_greenshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_greenshift+2,ax
		pop		eax
        and     WORD PTR _g32to16_greenshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_greenshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16_redshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_redshift+2,ax
		pop		eax
        and     WORD PTR _g32to16_redshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_redshift_mini+2,ax

		ret	

_32to16InitConverterWithAlpha:
		pop		eax
        and     WORD PTR _g32to16_blueshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_blueshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16_blueshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_blueshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16_greenshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_greenshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16_greenshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_greenshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to16_redshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_redshift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16_redshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_redshift_wa_mini+2,ax

		GetShiftCode_32to16	[ebx].APos, [edx].APos, [edx].ABitCount
		pop		eax
        and     WORD PTR _g32to16_alphashift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_alphashift_wa+2,ax
		pop		eax
        and     WORD PTR _g32to16_alphashift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to16_alphashift_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 32->16 konverter �rhat�v� t�tele

_32to16_doinit:
        push    ecx edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE EndOf32to16Convs - BeginOf32to16Convs
        push    OFFSET BeginOf32to16Convs
        W32     VirtualProtect,4
        mov     _32to16Init,0FFh

        pop     eax edx ecx
        ret
	

.data

_32to16ARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_32to16ARGBEntry_RedMask	DQ	000FF000000FF0000h
_32to16ARGBEntry_GreenMask	DQ	00000FF000000FF00h
_32to16ARGBEntry_BlueMask	DQ	0000000FF000000FFh
_32to16lastUsedPixFmt_Src	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to16lastUsedPixFmt_Dst	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to16Init					DB	0h
_32to16ConvertWithAlpha		DB	0h
