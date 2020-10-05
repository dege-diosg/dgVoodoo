;-----------------------------------------------------------------------------
; MMCONV32TO32.ASM - MultiMedia Converter lib, convering from 32 bit format to
;                    another 32 bit format
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

CheckInit32To32	MACRO
LOCAL   InitOk

        or      _32to32Init,0h
        jne     InitOk
        call    _32to32_doinit
InitOk:

ENDM

GetShiftCode_32to32	MACRO	SrcPos, DstPos
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

BeginOf32to32Convs	LABEL

;-------------------------------------------------------------------
; Általános 32->32 konverter

_general32to32converter:
		lea		eax,_32to32lastUsedPixFmt_Src
		IsPixFmtTheSame	ebx, eax
		jne		_32to32CP_newformat
		lea		eax,_32to32lastUsedPixFmt_Dst
		IsPixFmtTheSame	edx, eax
		je		_32to32CP_format_cached
_32to32CP_newformat:
		call	_32to32InitConvCode
_32to32CP_format_cached:

		movq	mm4,_32to32ARGBEntry_AlphaMask
		movq	mm5,_32to32ARGBEntry_RedMask
		movq	mm6,_32to32ARGBEntry_GreenMask
		movq	mm7,_32to32ARGBEntry_BlueMask
		mov		ecx,x
		cmp		ecx,2h
		jb		_32to32mini_
		je		_32to32

;mivel a 32->32 konverter jelenleg mindig csak videómemória -> systemmem irányban használatos,
;inkább a forrást igazítjuk 8 byte-ra
		test	esi,111b		;forrás 8-cal osztható címen?
		je      _32to32
		call	_32to32mini_
_32to32:call	_32to32_
		or		ecx,ecx
		jnz		_32to32mini_
		ret

_32to32_:
        mov     edx,y
		push	ecx
		shr		ecx,1h
		lea		esi,[esi+8*ecx]
		lea		edi,[edi+8*ecx]
		push	esi edi
		neg		ecx
		or		_32to32ConvertWithAlpha,0h
		jne		_g32to32_nexty_with_alpha
_g32to32_nexty_without_alpha:
		push	esi edi ecx
_g32to32_nextx_without_alpha:
 		movq    mm0,[esi+8*ecx] ;
        movq    mm1,mm0 ;
        movq    mm2,mm0
        pand    mm0,mm5 ;
        pand    mm1,mm6
_g32to32_redshift:
        pslld   mm0,0h  ;
        pand    mm2,mm7
_g32to32_greenshift:
        pslld   mm1,0h  ;
		inc		ecx
_g32to32_blueshift:
        pslld   mm2,0h  ;
        por     mm0,mm1
        por     mm0,mm2 ;
        por     mm0,mm4 ;
        movq    [edi+8*ecx-8],mm0 ;
        jnz     _g32to32_nextx_without_alpha
		pop		ecx edi esi
        add     esi,srcpitch
        add     edi,dstpitch
        dec     edx
        jne     _g32to32_nexty_without_alpha
_g32to32exit:
		pop		edi esi ecx
		and		ecx,1h
		ret

_32to32mini_:
        mov     edx,y
		push	esi edi
        or      _32to32ConvertWithAlpha,0h
        jne     _g32to32_nexty_with_alpha
_g32to32mini_nexty_without_alpha:
		movd    mm0,[esi]
        movq    mm1,mm0 	;
        movq    mm2,mm0
        pand    mm0,mm5 	;
        pand    mm1,mm6
_g32to32_redshift_mini:
        pslld   mm0,0h  	;
        pand    mm2,mm7
_g32to32_greenshift_mini:
        pslld   mm1,0h  	;
_g32to32_blueshift_mini:
        pslld   mm2,0h  	;
        por     mm0,mm1
        por     mm0,mm2 	;
        por     mm0,mm4 	;
        movd    [edi],mm0 	;
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to32mini_nexty_without_alpha
_g32to32miniexit:
		dec		ecx
		pop		edi esi
		add		esi,4h
		add		edi,4h
		ret


_g32to32_nexty_with_alpha:
		push	esi edi ecx
_g32to32_nextx_with_alpha:
		movq    mm0,[esi+8*ecx]
        movq    mm1,mm0 ;
        movq    mm2,mm0
        pand    mm0,mm5 ;
        movq    mm3,mm0
_g32to32_redshift_wa:
        pslld   mm0,0h  ;
        pand    mm1,mm6
_g32to32_greenshift_wa:
        pslld   mm1,0h  ;
        pand    mm2,mm7
        pand    mm3,mm4 ;
_g32to32_blueshift_wa:
        pslld   mm2,0h
        por     mm0,mm1 ;
_g32to32_alphashift_wa:
        pslld   mm3,0h
        por     mm0,mm2 ;
        inc		ecx
        por     mm0,mm3 ;
        movq    [edi+8*ecx-8],mm0 ;
        jnz     _g32to32_nextx_with_alpha
		pop		ecx edi esi
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
		jnz		_g32to32_nexty_with_alpha
		jmp		_g32to32exit


_g32to32mini_nexty_with_alpha:
		movd    mm0,[esi]
        movq    mm1,mm0 ;
        movq    mm2,mm0
        pand    mm0,mm5 ;
        movq    mm3,mm0
_g32to32_redshift_wa_mini:
        pslld   mm0,0h  ;
        pand    mm1,mm6
_g32to32_greenshift_wa_mini:
        pslld   mm1,0h  ;
        pand    mm2,mm7
        pand    mm3,mm4 ;
_g32to32_blueshift_wa_mini:
        pslld   mm2,0h
        por     mm0,mm1 ;
_g32to32_alphashift_wa_mini:
        pslld   mm3,0h
        por     mm0,mm2 ;
        por     mm0,mm3 ;
        movd    [edi],mm0 ;
		add		esi,srcpitch
		add		edi,dstpitch
		dec		edx
        jnz     _g32to32mini_nexty_with_alpha
		jmp		_g32to32miniexit

EndOf32to32Convs	LABEL

;-------------------------------------------------------------------
;Init kód a 32->32 konverterhez

_32to32InitConvCode:
		CheckInit32To32
		push	OFFSET _32to32lastUsedPixFmt_Src
		push	ebx
		call	CopyPixFmt
		push	OFFSET _32to32lastUsedPixFmt_Dst
		push	edx
		call	CopyPixFmt

		mov		_32to32ConvertWithAlpha,0h
		xor		eax,eax
		mov		ecx,[edx].ABitCount		;dst alpha bitcount
		mov		ebp,[ebx].ABitCount		;src alpha bitcount
		jecxz	_32to32_noalpha
		mov		ecx,[edx].APos
		mov		eax,constalpha
		or		ebp,ebp
        je      _32to32_alphaok
        mov     _32to32ConvertWithAlpha,1h
		mov		al,0FFh
		mov		ecx,[ebx].APos
_32to32_alphaok:
		shl		eax,cl
_32to32_noalpha:
		mov		DWORD PTR _32to32ARGBEntry_AlphaMask[0],eax
		mov		DWORD PTR _32to32ARGBEntry_AlphaMask[4],eax
		mov		eax,0FFh
		mov		ecx,[ebx].RPos
		shl		eax,cl
        mov     DWORD PTR _32to32ARGBEntry_RedMask[0],eax
        mov     DWORD PTR _32to32ARGBEntry_RedMask[4],eax
		mov		eax,0FFh
		mov		ecx,[ebx].GPos
		shl		eax,cl
        mov     DWORD PTR _32to32ARGBEntry_GreenMask[0],eax
        mov     DWORD PTR _32to32ARGBEntry_GreenMask[4],eax
		mov		eax,0FFh
		mov		ecx,[ebx].BPos
		shl		eax,cl
        mov     DWORD PTR _32to32ARGBEntry_BlueMask[0],eax
        mov     DWORD PTR _32to32ARGBEntry_BlueMask[4],eax

		GetShiftCode_32to32	[ebx].RPos, [edx].RPos
		GetShiftCode_32to32	[ebx].GPos, [edx].GPos
		GetShiftCode_32to32	[ebx].BPos, [edx].BPos

		or		_32to32ConvertWithAlpha,0h
		jne		_32to32InitConverterWithAlpha

		pop		eax
        and     WORD PTR _g32to32_blueshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_blueshift+2,ax
        and     WORD PTR _g32to32_blueshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_blueshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32_greenshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_greenshift+2,ax
        and     WORD PTR _g32to32_greenshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_greenshift_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32_redshift+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_redshift+2,ax
        and     WORD PTR _g32to32_redshift_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_redshift_mini+2,ax

		ret	

_32to32InitConverterWithAlpha:
		pop		eax
        and     WORD PTR _g32to32_blueshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_blueshift_wa+2,ax
        and     WORD PTR _g32to32_blueshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_blueshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32_greenshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_greenshift_wa+2,ax
        and     WORD PTR _g32to32_greenshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_greenshift_wa_mini+2,ax

		pop		eax
        and     WORD PTR _g32to32_redshift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_redshift_wa+2,ax
        and     WORD PTR _g32to32_redshift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_redshift_wa_mini+2,ax

		GetShiftCode_32to32	[ebx].APos, [edx].APos
		pop		eax
        and     WORD PTR _g32to32_alphashift_wa+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_alphashift_wa+2,ax
        and     WORD PTR _g32to32_alphashift_wa_mini+2, NOT MMXCODESHIFTMASK
        or      WORD PTR _g32to32_alphashift_wa_mini+2,ax

		ret

;-------------------------------------------------------------------
;A 32->32 konverter írhatóvá tétele

_32to32_doinit:
        push    ecx edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE EndOf32to32Convs - BeginOf32to32Convs
        push    OFFSET BeginOf32to32Convs
        W32     VirtualProtect,4
        mov     _32to32Init,0FFh

        pop     eax edx ecx
        ret
	

.data

_32to32ARGBEntry_AlphaMask	DQ	0FF000000FF000000h
_32to32ARGBEntry_RedMask	DQ	000FF000000FF0000h
_32to32ARGBEntry_GreenMask	DQ	00000FF000000FF00h
_32to32ARGBEntry_BlueMask	DQ	0000000FF000000FFh
_32to32lastUsedPixFmt_Src	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to32lastUsedPixFmt_Dst	_pixfmt	<0, 0, 0, 0, 0, 0, 0, 0, 0>
_32to32Init					DB	0h
_32to32ConvertWithAlpha		DB	0h
