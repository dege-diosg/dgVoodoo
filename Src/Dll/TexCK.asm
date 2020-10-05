;-----------------------------------------------------------------------------
; TEXCK.ASM - Colorkeying functions for texturing
;             It is mainly for creating 'alpha mask' textures for non-native
;             glide colorkeying
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


;-------------------------------------------------------------------------------
;...............................................................................


.code

PUBLIC _TexGetTexelValueFromARGB@16
PUBLIC _TexGetTexelValueFromARGB
_TexGetTexelValueFromARGB@16:
_TexGetTexelValueFromARGB:
        push    ebx
        xor     eax,eax
        mov     ecx,[esp+8 +1*4]        ;form tum
        cmp     ecx,16
        jae     GGTVF_End

        mov     edx,[esp+4 +1*4]        ;ARGB sz¡n
        and     edx,0FFFFFFh
        mov     ebx,[esp+12 +1*4]       ;paletta
        mov     eax,[esp+16 +1*4]       ;ncc t bla
        call    [ARGBToTexel+4*ecx]
GGTVF_End:
        pop     ebx
        ret     16

ARGBTo_RGB332:
ARGBTo_ARGB8332:
        mov     ecx,edx
        shr     cx,5h
        shr     cl,1h
        and     cl,3h
        mov     al,cl
        shl     ch,2h
        or      al,ch
        rol     ecx,10h
        and     cl,011100000b
        or      cl,al                   ;cl=texel format

        push    ecx
        xor     eax,eax
        shl     ecx,24
        shld    eax,ecx,3h
        shld    eax,ecx,3h
        shld    eax,ecx,2h
        shl     ecx,3h
        shld    eax,ecx,3h
        shld    eax,ecx,3h
        shld    eax,ecx,2h
        shl     ecx,3h
        shld    eax,ecx,2h
        shld    eax,ecx,2h
        shld    eax,ecx,2h
        shld    eax,ecx,2h
        pop     ecx
        cmp     eax,edx
        push    LARGE 0h
        pop     eax
        jne     ARGBTo_RGB332_end
        movzx   eax,cl
        or      eax,0FF0000h
ARGBTo_RGB332_end:
        ret

ARGBTo_Invalid:
        xor     eax,eax
        ret

ARGBTo_A8:
ARGBTo_I8:
ARGBTo_AI88:
        mov     eax,edx
        cmp     al,ah
        jne     ARGBTo_Invalid
        mov     cl,al
        rol     eax,10h
        cmp     al,cl
        jne     ARGBTo_Invalid
        movzx   eax,al
        or      eax,0FF0000h
        ret

ARGBTo_AI44:
        mov     eax,edx
        cmp     al,ah
        jne     ARGBTo_Invalid
        mov     cl,al
        rol     eax,10h
        cmp     al,cl
        jne     ARGBTo_Invalid
        mov     ch,cl
        shr     cl,4h
        and     ch,0Fh
        cmp     cl,ch
        jne     ARGBTo_Invalid
        movzx   eax,cl
        or      eax,0F0000h
        ret

ARGBTo_RGB565:
        mov     ecx,edx
        shl     ecx,8h
        xor     eax,eax
        shld    eax,ecx,5h
        shl     ecx,8h
        shld    eax,ecx,6h
        shl     ecx,8h
        shld    eax,ecx,5h
        push    eax
        shl     eax,10h
        xor     ecx,ecx
        shld    ecx,eax,5h
        shld    ecx,eax,3h
        shl     eax,5h
        shld    ecx,eax,6h
        shld    ecx,eax,2h
        shl     eax,6h
        shld    ecx,eax,5h
        shld    ecx,eax,3h
        pop     eax
        or      eax,0FFFF0000h
        cmp     ecx,edx
        je      ARGBTo_RGB565_end
        xor     eax,eax
ARGBTo_RGB565_end:
        ret

ARGBTo_ARGB1555:
        mov     ecx,edx
        shl     ecx,8h
        xor     eax,eax
        shld    eax,ecx,5h
        shl     ecx,8h
        shld    eax,ecx,5h
        shl     ecx,8h
        shld    eax,ecx,5h
        push    eax
        shl     eax,11h
        xor     ecx,ecx
        shld    ecx,eax,5h
        shld    ecx,eax,3h
        shl     eax,5h
        shld    ecx,eax,5h
        shld    ecx,eax,3h
        shl     eax,5h
        shld    ecx,eax,5h
        shld    ecx,eax,3h
        pop     eax
        or      eax,07FFF0000h
        cmp     ecx,edx
        je      ARGBTo_ARGB1555_end
        xor     eax,eax
ARGBTo_ARGB1555_end:
        ret

ARGBTo_ARGB4444:
        mov     ecx,edx
        shl     ecx,8h
        xor     eax,eax
        shld    eax,ecx,4h
        shl     ecx,8h
        shld    eax,ecx,4h
        shl     ecx,8h
        shld    eax,ecx,4h
        push    eax
        shl     eax,14h
        xor     ecx,ecx
        shld    ecx,eax,4h
        shld    ecx,eax,4h
        shl     eax,4h
        shld    ecx,eax,4h
        shld    ecx,eax,4h
        shl     eax,4h
        shld    ecx,eax,4h
        shld    ecx,eax,4h
        pop     eax
        or      eax,0FFF0000h
        cmp     ecx,edx
        je      ARGBTo_ARGB4444_end
        xor     eax,eax
ARGBTo_ARGB4444_end:
        ret

ARGBTo_P8:
ARGBTo_AP88:
        xor     eax,eax
ARGBTo_P8search:
        cmp     edx,[ebx+4*eax]
        je      ARGBTo_P8find
        inc     al
        jne     ARGBTo_P8search
        ret
ARGBTo_P8find:
        or      eax,0FF0000h
        ret

ARGBTo_AYIQ8422:
ARGBTo_YIQ422:
        push    esi
        push    ebp
        push    edx
        mov     esi,eax
        xor     eax,eax
ARGBTo_YIQNext:
        mov     ecx,eax
        shr     ecx,4h
        movzx   edx,(_ncctable PTR [esi]).yRGB[ecx]
        mov     ebp,eax
        shr     ebp,2h
        mov     ebx,edx
        and     ebp,3h        
        imul    ebp,3*2
        add     bx,(_ncctable PTR [esi]).iRGB[ebp+0*2]
        mov     ecx,eax
        and     ecx,3h
        imul    ecx,3*2h
        add     bx,(_ncctable PTR [esi]).qRGB[ecx+0*2]
        or      bh,bh
        je      _ARGBTOYIQ_ClampOk1
        jns     _ARGBTOYIQ_ClampUp1
        xor     ebx,ebx
        jmp     _ARGBTOYIQ_ClampOk1
_ARGBTOYIQ_ClampUp1:
        mov     bl,0FFh
_ARGBTOYIQ_ClampOk1:
        shl     ebx,10h
        mov     bx,dx
        add     bx,(_ncctable PTR [esi]).iRGB[ebp+1*2]
        add     bx,(_ncctable PTR [esi]).qRGB[ecx+1*2]
        or      bh,bh
        je      _ARGBTOYIQ_ClampOk2
        jns     _ARGBTOYIQ_ClampUp2
        mov     bl,0h
        jmp     _ARGBTOYIQ_ClampOk2
_ARGBTOYIQ_ClampUp2:
        mov     bl,0FFh
_ARGBTOYIQ_ClampOk2:
        mov     bh,bl
        add     dx,(_ncctable PTR [esi]).iRGB[ebp+2*2]
        add     dx,(_ncctable PTR [esi]).qRGB[ecx+2*2]
        or      dh,dh
        je      _ARGBTOYIQ_ClampOk3
        jns     _ARGBTOYIQ_ClampUp3
        xor     edx,edx
        jmp     _ARGBTOYIQ_ClampOk3
_ARGBTOYIQ_ClampUp3:
        mov     dl,0FFh
_ARGBTOYIQ_ClampOk3:
        mov     bl,dl
        cmp     ebx,[esp]
        je      _ARGBTOYIQ_IndexFound
        inc     al
        jne     ARGBTo_YIQNext
        xor     eax,eax
        jmp     _ARGBTOYIQ_End
_ARGBTOYIQ_IndexFound:
        or      eax,0FF0000h
_ARGBTOYIQ_End:
        pop     edx
        pop     ebp
        pop     esi
        ret

;---------------------------------------------------------------------------------


PUBLIC  _TexCreateAlphaMask@36
PUBLIC  _TexCreateAlphaMask
_TexCreateAlphaMask@36:
_TexCreateAlphaMask:
        push    esi
        push	edi
        push	ebx
        push	ebp
        mov     esi,[esp+4+4*4]             ;esi=src
        mov     edi,[esp+8+4*4]             ;edi=dst
        mov     ebx,[esp+12+4*4]
        mov     eax,[esp+24+4*4]
        mov     x,eax
        mov     eax,[esp+28+4*4]
        mov     y,eax

        mov     ebp,[esp+36+4*4]            ;DX alphamask
        mov     edx,[esp+32+4*4]            ;felsõ 16 bit: ck mask
                                            ;alsó 16 bit: ck 3Dfx texel form.
        mov     eax,[esp+16+4*4]            ;forrás bytepp (1 vagy 2)
        dec     eax
        mov     ecx,[ckalphacreate_src_table+4*eax]
        mov     eax,[esp+20+4*4]            ;cél bytepp (2 vagy 4)

		jnz	ckGenMaskTexFrom16
		mov	dh,dl

ckGenMaskTexFrom16:
		push	dx
		push	dx
		push	dx
		push	dx
		movq	mm6,[esp]
		
		pushfd
		shr	edx,10h
		popfd
		jnz	ckGenMaskTexFrom16_
		mov	dh,dl
ckGenMaskTexFrom16_:
		push	dx
		push	dx
		push	dx
		push	dx
		movq	mm5,[esp]

        shr     eax,1h
        dec     eax

		jnz	ckGenMaskTexTo32
		push	bp
		push	bp
		push	bp
		push	bp
		jmp	ckGenMaskTexAlphaMaskOK
ckGenMaskTexTo32:
		push	ebp
		push	ebp
ckGenMaskTexAlphaMaskOK:
		movq	mm7,[esp]
		add	esp,3*8

		mov	edx,y
        call    DWORD PTR [ecx+4*eax]
		emms

		xchg	eax,esi
        pop     ebp
        pop		ebx
        pop		edi
        pop		esi
        ret     9*4

ckAlphaCreate_8to16:
		mov		ecx,x
		lea		edi,[edi+2*ecx]
		cmp		ecx,8h
		jb		ckAlphaCreate_8to16_mini
		shr		ecx,2h
ckAlphaCreate_8to16_nexty:
		lea		esi,[esi+4*ecx]
		push	ecx
		push	edi
		neg		ecx
ckAlphaCreate_8to16_nextx:
		movq	mm0,[esi+4*ecx]	;
		pand	mm0,mm5		;
		pcmpeqb	mm0,mm6		;
		movq	mm3,mm0		;
		punpcklbw mm0,mm0
		pandn	mm0,mm7		;
		punpckhbw mm3,mm3
		movq	[edi+8*ecx],mm0	;
		pandn	mm3,mm7
		movq	[edi+8*ecx+8],mm3 ;
		nop
		add		ecx,2h		;
		jnz		ckAlphaCreate_8to16_nextx
		pop		edi
		pop		ecx
        add     edi,ebx
		dec		edx
		jnz		ckAlphaCreate_8to16_nexty
		ret
ckAlphaCreate_8to16_mini:
ckAlphaCreate_8to16_mini_nexty:
		add		esi,ecx
		push	ecx
		push	edi
		neg		ecx
ckAlphaCreate_8to16_mini_nextx:
		mov		al,[esi+ecx]
		movd	mm0,eax
		pand	mm0,mm5
		pcmpeqb mm0,mm6
		punpcklbw mm0,mm0
		pandn	mm0,mm7
		movd	eax,mm0
		mov		[edi+2*ecx],ax
		inc		ecx
		jnz		ckAlphaCreate_8to16_mini_nextx
		pop		edi
		pop		ecx
        add     edi,ebx
		dec		edx
		jnz		ckAlphaCreate_8to16_mini_nexty
		ret


ckAlphaCreate_8to32:
		mov			ecx,x
		lea			edi,[edi+4*ecx]
		cmp			ecx,8h
		jb			ckAlphaCreate_8to32_mini
		shr			ecx,1h
ckAlphaCreate_8to32_nexty:
		lea			esi,[esi+2*ecx]
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_8to32_nextx:
		movq		mm0,[esi+2*ecx]	;
		pand		mm0,mm5		;
		pcmpeqb		mm0,mm6		;
		movq		mm1,mm0		;
		punpcklbw	mm0,mm0
		punpcklwd	mm0,mm0	;
		movq		mm2,mm1
		pandn		mm0,mm7		;
		punpcklbw	mm1,mm1
		movq		[edi+8*ecx],mm0 ;
		punpckhwd	mm1,mm1
		movq		mm3,mm2		;
		pandn		mm1,mm7
		movq		[edi+8*ecx+8],mm1 ;
		punpckhbw	mm2,mm2
		punpcklwd	mm2,mm2	;
		pandn		mm2,mm7		;
		punpckhbw	mm3,mm3
		movq		[edi+8*ecx+16],mm2 ;
		punpckhwd	mm3,mm3
		pandn		mm3,mm7		;
		add			ecx,4h
		movq		[edi+8*ecx+24-4*8],mm3 ;
		jnz			ckAlphaCreate_8to32_nextx
		pop			edi
		pop			ecx
        add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_8to32_nexty
		ret
ckAlphaCreate_8to32_mini:
ckAlphaCreate_8to32_mini_nexty:
		add			esi,ecx
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_8to32_mini_nextx:
		mov			al,[esi+ecx]
		movd		mm0,eax
		pand		mm0,mm5
		pcmpeqb		mm0,mm6
		punpcklbw	mm0,mm0
		punpcklwd	mm0,mm0
		pandn		mm0,mm7
		movd		eax,mm0
		mov			[edi+4*ecx],eax
		inc			ecx
		jnz			ckAlphaCreate_8to32_mini_nextx
		pop			edi
		pop			ecx
		add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_8to32_mini_nexty
		ret

ckAlphaCreate_16to16:
 		mov			ecx,x
		lea			edi,[edi+2*ecx]
		cmp			ecx,4h
		jb			ckAlphaCreate_16to16_mini
		shr			ecx,2h
ckAlphaCreate_16to16_nexty:
		lea			esi,[esi+8*ecx]
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_16to16_nextx:
		movq		mm0,[esi+8*ecx]	;
		pand		mm0,mm5		;
		pcmpeqw		mm0,mm6		;
		pandn		mm0,mm7		;
		inc			ecx
		movq		[edi+8*ecx-8],mm0 ;
		jnz			ckAlphaCreate_16to16_nextx
		pop			edi
		pop			ecx
		add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_16to16_nexty
		ret
ckAlphaCreate_16to16_mini:
ckAlphaCreate_16to16_mini_nexty:
		lea			esi,[esi+2*ecx]
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_16to16_mini_nextx:
		mov			ax,[esi+2*ecx]
		movd		mm0,eax
		pand		mm0,mm5
		pcmpeqw		mm0,mm6
		pandn		mm0,mm7
		movd		eax,mm0
		mov			[edi+2*ecx],ax
		inc			ecx
		jnz			ckAlphaCreate_16to16_mini_nextx
		pop			edi
		pop			ecx
		add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_16to16_mini_nexty
		ret
	

ckAlphaCreate_16to32:
 		mov			ecx,x
		lea			edi,[edi+4*ecx]
		cmp			ecx,4h
		jb			ckAlphaCreate_16to32_mini
		shr			ecx,1h
ckAlphaCreate_16to32_nexty:
		lea			esi,[esi+4*ecx]
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_16to32_nextx:
		movq		mm0,[esi+4*ecx]	;
		pand		mm0,mm5		;
		pcmpeqw		mm0,mm6		;
		movq		mm1,mm0		;
		punpcklwd	mm0,mm0
		pandn		mm0,mm7		;
		punpckhwd	mm1,mm1
		movq		[edi+8*ecx],mm0	;
		pandn		mm1,mm7
		movq		[edi+8*ecx+8],mm1	;
		add			ecx,2h
		jnz			ckAlphaCreate_16to32_nextx
		pop			edi
		pop			ecx
		add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_16to32_nexty
		ret
ckAlphaCreate_16to32_mini:
ckAlphaCreate_16to32_mini_nexty:
		lea			esi,[esi+2*ecx]
		push		ecx
		push		edi
		neg			ecx
ckAlphaCreate_16to32_mini_nextx:
		mov			ax,[esi+2*ecx]
		movd		mm0,eax
		pand		mm0,mm5
		pcmpeqw		mm0,mm6
		punpcklwd	mm0,mm0
		pandn		mm0,mm7
		movd		DWORD PTR [edi+4*ecx],mm0
		inc			ecx
		jnz			ckAlphaCreate_16to32_mini_nextx
		pop			edi
		pop			ecx
		add			edi,ebx
		dec			edx
		jnz			ckAlphaCreate_16to32_mini_nexty
		ret

;-------------------------------------------------------------------------------
;...............................................................................

.data

ARGBToTexel     LABEL DWORD
        DD      ARGBTo_RGB332
        DD      ARGBTo_YIQ422
        DD      ARGBTo_A8
        DD      ARGBTo_I8
        DD      ARGBTo_AI44
        DD      ARGBTo_P8
        DD      ARGBTo_Invalid
        DD      ARGBTo_Invalid
        DD      ARGBTo_ARGB8332
        DD      ARGBTo_AYIQ8422
        DD      ARGBTo_RGB565
        DD      ARGBTo_ARGB1555
        DD      ARGBTo_ARGB4444
        DD      ARGBTo_AI88
        DD      ARGBTo_AP88
        DD      ARGBTo_Invalid

ckalphacreate_src_table LABEL DWORD
        DD      ckalphacreate_dst_tablewithsrc8
        DD      ckalphacreate_dst_tablewithsrc16

ckalphacreate_dst_tablewithsrc8 LABEL DWORD
        DD      ckAlphaCreate_8to16
        DD      ckAlphaCreate_8to32

ckalphacreate_dst_tablewithsrc16 LABEL DWORD
        DD      ckAlphaCreate_16to16
        DD      ckAlphaCreate_16to32