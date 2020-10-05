;-----------------------------------------------------------------------------
; MMCONVCOPY.ASM - MultiMedia Converter lib, copying between same formats
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
; MMConv:	MMConvCopy.ASM
;		Konvertálást nem, csak másolást végrehajtó rutinok
;
;-------------------------------------------------------------------


CheckInitCopy	MACRO
LOCAL   InitOk

		or      _copyInitOk,0h
		jne     InitOk
		call    _copy_doinit
InitOk:

ENDM

;---------------------------------------------------------------------------
.code

BeginOfCopyConvs	LABEL	NEAR

_general8to8copy:
		mov     ecx,x
		cmp		DWORD PTR (_pixfmt PTR [edx]).ABitCount,8h
		jne		_gen8copy_notspecap88
		lea		ecx,[ecx+ecx]
_gen8copy_notspecap88:
		test	esi,11b
		je		_gen8copy_alignok
		mov		eax,esi
		add		eax,4h
		and		al,NOT 3h
		sub		eax,esi
		sub		ecx,eax	
		lea		ebx,[esi+eax]
		push	ebx
		lea		ebx,[edi+eax]
		push	ebx
		call	_gen8copy_mini
		pop		edi
		pop		esi
_gen8copy_alignok:
		mov		eax,ecx
		shr     ecx,2h
		and		eax,11b
		je		_dogeneralcopy
		push	esi
		push	edi
		call	_dogeneralcopy
		pop		edi
		pop		esi
		lea     esi,[esi+4*ecx]
		lea     edi,[edi+4*ecx]
_gen8copy_mini:
		mov		edx,y
_gen8copy_mini_nexty:
		push	eax
		push	esi
		push	edi
_gen8copy_mini_nextx:
		mov		bl,[esi+eax-1]
		dec		eax
		mov		[edi+eax],bl
		jne		_gen8copy_mini_nextx
		pop		edi
		pop		esi
		pop		eax
		add     esi,srcpitch
		add     edi,dstpitch
		dec		edx
		jne		_gen8copy_mini_nexty
		ret

_dogeneralcopy:
		jecxz	_dogencopy_end
		mov     edx,y
		cld
_RC_next:
		push    esi
		push	edi
		push	ecx
		rep     movsd
		pop     ecx
		pop		edi
		pop		esi
		add     esi,srcpitch
		add     edi,dstpitch
		dec     edx
		jne     _RC_next
_dogencopy_end:
		ret


_general16to16copy:
		shl		x,1h
		jmp		_general8to8copy

_general32to32copy:
		shl		x,2h
		jmp		_general8to8copy

;---------------------------------------------------------------------------
_ckgeneralinit:
		mov     eax,colorkey
		mov     DWORD PTR _CKRC_compcolorkeycmp+1,eax
		movzx	eax,ax
		mov     DWORD PTR _CKRC_colorkeycmp1+2,eax
		mov     DWORD PTR _CKRC_colorkeycmp2+1,eax

		mov     ebx,colorKeyMask
		mov		edx,y
		mov		ecx,x
		shr		ecx,1h
		lea		esi,[esi+4*ecx]
		lea		edi,[edi+4*ecx]
		neg		ecx
		ret


_ckgeneral16to16copy:
		CheckInitCopy
		call	_ckgeneralinit
		push	edx
_CKRC_next_y:
		push    esi
		push	edi
		push	ecx
_CKRC_next_x:
		mov     eax,[esi+4*ecx]		;
		mov     ebp,eax				;
		and		eax,ebx
_CKRC_compcolorkeycmp:
		cmp     eax,12345678h		;
		je      _CKRC_nocopy
		mov     edx,eax				;
		and     eax,0FFFFh
		shr     edx,10h				;
_CKRC_colorkeycmp1:
		cmp     edx,12345678h		;
		je      _CKRC_highck
_CKRC_colorkeycmp2:
		cmp     eax,12345678h		;
		je      _CKRC_lowck
		mov     [edi+4*ecx],ebp		;
_CKRC_nocopy:
		inc		ecx
		jnz     _CKRC_next_x
_CKRC_exit:
		pop     ecx
		pop		edi
		pop		esi
		add     esi,srcpitch
		add     edi,dstpitch
		dec		DWORD PTR [esp]
		jne     _CKRC_next_y
		pop		eax
		ret
_CKRC_highck:
		mov     [edi+4*ecx],bp
		inc		ecx
		jnz     _CKRC_next_x
		jmp		_CKRC_exit
_CKRC_lowck:
		shr		ebp,10h
		mov     [edi+4*ecx+2],bp
		inc     ecx
		jnz     _CKRC_next_x
		jmp		_CKRC_exit


_ckgeneral32to32copy:
		call	_ckgeneralinit
_CKRC32to32_next_y:
		push    esi
		push	edi
		push	ecx
		push	edx
_CKRC32to32_next_x:
		mov     ebp,[esi+4*ecx]		;
		mov		edx,ebp				;
		and		ebp,ebx
		cmp     eax,ebp				;
		je		_CKRC32to32_nocopy
		mov     [edi+4*ecx],edx		;
_CKRC32to32_nocopy:
		inc		ecx
		jnz     _CKRC32to32_next_x
		pop     edx
		pop		ecx
		pop		edi
		pop		esi
		add     esi,srcpitch
		add     edi,dstpitch
		dec		edx
		jne     _CKRC32to32_next_y
		ret
	

EndOfCopyConvs	LABEL	NEAR
;---------------------------------------------------------------------------

_copy_doinit:
		push    ecx
		push	edx
		push    eax

		push    esp
		push    LARGE PAGE_EXECUTE_READWRITE
		push    LARGE (EndOfCopyConvs - BeginOfCopyConvs)
		push    OFFSET BeginOfCopyConvs
		W32     VirtualProtect,4
		mov     _copyInitOk,0FFh

		pop     eax
		pop		edx
		pop		ecx
		ret

.data

_copyInitOk	DB	0h
