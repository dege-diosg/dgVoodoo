;-----------------------------------------------------------------------------
; TEXMMGEN.ASM - Texture mipmap generating functions
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

.586p
.model FLAT
.mmx

.code

PUBLIC _GlideGenerateMipMap@20
PUBLIC _GlideGenerateMipMap@
_GlideGenerateMipMap@20:
_GlideGenerateMipMap@:
        push    ebx
        push    esi
        push    edi
        push    ebp
        mov     esi,[esp+8 +4*4]                ;src
        mov     edi,[esp+12 +4*4]               ;dst
        mov     ecx,[esp+16 +4*4]
        mov     edx,[esp+20 +4*4]
        mov     eax,[esp+4 +4*4]
        shr     ecx,1h
        shr     edx,1h
;        mov     x,ecx
        call    [_funcs+4*eax]
        emms
        pop     ebp
        pop     edi
        pop     esi
        pop     ebx
        ret     20

_create_mipmap_rgb332:
_cm_rgb332y:
        push    edx
        lea     edx,[ecx+ecx]
        push    esi
        push    ecx
        push    ecx
        cmp     ecx,4h
        jb      _cm_rgb332xmini
_cm_rgb332x:
        mov     eax,[esi+4]
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        shld    ebp,eax,5h+2
        shl     eax,3h+2
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        and     ebp,11100111001100111001110011b
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        mov     eax,[esi+edx+4]
        and     ebx,11100111001100111001110011b
        add     edi,4h
        add     ebp,ebx
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,4h
        ror     eax,16-2

        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h
        and     ebx,11100111001100111001110011b
        add     ebp,ebx

        shld    ebx,eax,5h+2+8
        shl     eax,3h+2+8
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h
        and     ebx,11100111001100111001110011b
        add     ebp,ebx
        shld    ecx,ebp,3h+4
        shl     ebp,3h+4+2
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,2h
        shl     ebp,4h
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,2h

        mov     eax,[esi]
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        shld    ebp,eax,5h+2
        shl     eax,3h+2
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        and     ebp,11100111001100111001110011b
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        mov     eax,[esi+edx]
        and     ebx,11100111001100111001110011b
        add     ebp,ebx
        add     esi,8h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,4h
        ror     eax,16-2

        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h
        and     ebx,11100111001100111001110011b
        add     ebp,ebx

        shld    ebx,eax,5h+2+8
        shl     eax,3h+2+8
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h

        shld    ebx,eax,5h+2
        shl     eax,3h+2
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h
        and     ebx,11100111001100111001110011b
        add     ebp,ebx
        shl     ebp,4h
        shld    ecx,ebp,3h
        shl     ebp,3h+2
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,2h
        shl     ebp,4h
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,3h
        shl     ebp,5h
        shld    ecx,ebp,2h

        mov     [edi-4],ecx
        sub     DWORD PTR [esp],4h
        jne     _cm_rgb332x
_cm_rgb332xend:
        pop     eax
        pop     ecx
        pop     esi
        pop     edx
        lea     esi,[esi+4*ecx]
        dec     edx
        jne     _cm_rgb332y
        ret

_cm_rgb332xmini:
        mov     ax,[esi]
        shl     eax,10h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,2h+20
        shld    ebp,eax,5h+2
        shl     eax,5h
        shld    ebp,eax,5h
        shl     eax,3h
        shld    ebp,eax,2h+20
        and     ebp,11100111001100000000000000000000b
        and     ebx,11100111001100000000000000000000b
        mov     ax,[esi+edx]
        add     ebp,ebx
        shl     eax,10h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,4h
        and     ebx,11100111001100000000000000000000b
        add     ebp,ebx
        shld    ebx,eax,5h+2
        shl     eax,5h
        shld    ebx,eax,5h
        shl     eax,3h
        shld    ebx,eax,4h
        and     ebx,11100111001100000000000000000000b
        add     ebp,ebx
        shl     ebp,10h
        shld    eax,ebp,3h
        shl     ebp,3h
        shld    eax,ebp,3h
        shl     ebp,3h
        inc     edi
        shld    eax,ebp,2h
        mov     [edi-1],al
        add     esi,2h
        dec     ecx
        jne     _cm_rgb332xmini
        jmp     _cm_rgb332xend

_create_mipmap_p8:
        mov     ebp,ecx
        cmp     ecx,4h
        jb      _cm_p8y_mini
        mov     eax,00FF00FFh
        movd    mm1,eax
        punpckldq mm1,mm1
        cmp     ecx,4h
        je      _cm_p8y
_cm_p8x_b:
        movq    mm0,[esi+2*ecx-8]
        movq    mm2,[esi+2*ecx-8-8]
        pand    mm0,mm1
        pand    mm2,mm1
        packuswb mm2,mm0
        sub     ecx,8h
        movq    [edi+ecx-8+8],mm2
        jne     _cm_p8x_b
        lea     esi,[esi+4*ebp]
        mov     ecx,ebp
        add     edi,ebp
        dec     edx
        jne     _cm_p8x_b
        ret
_cm_p8y:
_cm_p8x:
        movq    mm0,[esi+2*ecx-8]
        pand    mm0,mm1
        packuswb mm0,mm0
        movd    [edi+ecx-4],mm0
        sub     ecx,4h
        jne     _cm_p8x
        lea     esi,[esi+4*ebp]
        mov     ecx,ebp
        add     edi,ebp
        dec     edx
        jne     _cm_p8y
        ret
_cm_p8y_mini:
_cm_p8x_mini:
        mov     al,[esi+2*ecx-2]
        mov     [edi+ecx-1],al
        dec     ecx
        jne     _cm_p8x_mini
        lea     esi,[esi+4*ebp]
        mov     ecx,ebp
        add     edi,ebp
        dec     edx
        jne     _cm_p8y_mini
        ret

_create_mipmap_a8:
_cm_a8y:
        push    edx
        push    esi
        push    ecx
        lea     edx,[ecx+ecx]
        xor     eax,eax
        xor     ebx,ebx
_cm_a8x:
        mov     bl,[esi]
        mov     al,[esi+1]
        add     ebx,eax
        mov     al,[esi+edx]
        add     ebx,eax
        mov     al,[esi+edx+1]
        add     ebx,eax
        shr     ebx,2h
        mov     [edi],bl
        add     esi,2h
        inc     edi
        dec     ecx
        jne     _cm_a8x
        pop     ecx
        pop     esi
        lea     esi,[esi+4*ecx]
        pop     edx
        dec     edx
        jne     _cm_a8y
        ret

_create_mipmap_ai44:
_cm_ai44y:
        push    edx
        push    esi
        push    ecx
        lea     edx,[ecx+ecx]
_cm_ai44x:
        xor     eax,eax
        xor     ebx,ebx
        mov     bl,[esi]
        mov     ebp,ebx
        and     ebp,0F0h
        shl     ebp,2h
        and     bl,0Fh
        or      ebx,ebp
        mov     al,[esi+1]
        mov     ebp,eax
        and     ebp,0F0h
        shl     ebp,2h
        and     al,0Fh
        or      eax,ebp
        add     ebx,eax
        mov     al,[esi+edx]
        mov     ebp,eax
        and     ebp,0F0h
        shl     ebp,2h
        and     al,0Fh
        or      eax,ebp
        add     ebx,eax
        mov     al,[esi+edx+1]
        mov     ebp,eax
        and     ebp,0F0h
        shl     ebp,2h
        and     al,0Fh
        or      eax,ebp
        add     ebx,eax
        mov     eax,ebx
        shr     ebx,2h
        and     bl,0Fh
        shr     eax,4h
        and     al,0F0h
        or      al,bl
        mov     [edi],al
        add     esi,2h
        inc     edi
        dec     ecx
        jne     _cm_ai44x
        pop     ecx
        pop     esi
        lea     esi,[esi+4*ecx]
        pop     edx
        dec     edx
        jne     _cm_ai44y
        ret

_create_mipmap_rgb565:
        mov     eax,11111000000000000000b
        movd    mm5,eax
        mov     eax,1111110000000b
        movd    mm6,eax
        mov     eax,11111b
        movd    mm7,eax
        punpckldq mm5,mm5
        punpckldq mm6,mm6
        punpckldq mm7,mm7

        mov     ebp,edx
        mov     eax,ecx
        cmp     ecx,2h
        jb      _cm_rgb565x_mini

_cm_rgb565y:
        mov     ecx,eax
        lea     edx,[esi+4*eax]
_cm_rgb565x:
        movq    mm0,[esi+4*ecx-8]  ;
        movq    mm1,mm0            ;
        movq    mm4,[edx+4*ecx-8]
        punpcklwd mm1,mm1          ;
        movq    mm2,mm1            ;
        pslld   mm1,4h  ;r
        movq    mm3,mm2            ;
        pslld   mm2,2h
        pand    mm1,mm5 ;rm        ;
        pand    mm2,mm6 ;gm
        por     mm1,mm2            ;
        movq    mm2,mm4
        pand    mm3,mm7 ;bm        ;
        punpcklwd mm2,mm2
        pslld   mm2,4h  ;r         ;
        por     mm1,mm3
        movq    mm3,mm2            ;
        pand    mm2,mm5 ;rm
        psrld   mm3,2h  ;g         ;
        paddd   mm1,mm2 ;r+r
        pand    mm3,mm6 ;gm        ;
        movq    mm2,mm4
        punpckhwd mm4,mm4          ;
        paddd   mm1,mm3 ;g+g
        punpcklwd mm2,mm2          ;
        pand    mm2,mm7 ;bm        ;
        punpckhwd mm0,mm0
        paddd   mm1,mm2 ;b+b       ;
        movq    mm2,mm0
        movq    mm3,mm1            ;
        psrlq   mm3,32             ;
        pslld   mm2,4   ;r         ;
        paddd   mm1,mm3
        movq    mm3,mm0            ;
        pslld   mm3,2h  ;g         ;
        pand    mm0,mm7 ;bm
        pand    mm2,mm5 ;rm        ;
        pand    mm3,mm6 ;gm
        por     mm0,mm2            ;
        movq    mm2,mm4
        por     mm0,mm3            ;
        movq    mm3,mm4
        pslld   mm2,4h  ;r         ;
        pand    mm3,mm7 ;bm
        pslld   mm4,2h  ;g         ;
        paddd   mm0,mm3
        pand    mm2,mm5 ;rm        ;
        pand    mm4,mm6 ;gm
        paddd   mm0,mm2            ;
        paddd   mm0,mm4            ;
        movq    mm2,mm0            ;
        psrlq   mm2,32             ;
        paddd   mm0,mm2            ;

        punpckldq mm1,mm0          ;
        psrld   mm1,2h  ; /4       ;
        movq    mm0,mm1            ;
        movq    mm2,mm1
        pand    mm0,mm5 ;rm        ;
        pand    mm1,mm6 ;gm
        psrld   mm0,4h             ;
        pand    mm2,mm7 ;bm
        psrld   mm1,2h             ;
        por     mm0,mm2
        por     mm0,mm1            ;
        pslld   mm0,16             ;
        psrad   mm0,16             ;
        sub     ecx,2
        packssdw mm0,mm0           ;
        movd    [edi+2*ecx],mm0    ;
        jne     _cm_rgb565x
        lea     esi,[esi+8*eax]
        lea     edi,[edi+2*eax]
        dec     ebp
        jne     _cm_rgb565y
        ret

_cm_rgb565x_mini:
        movq    mm0,[esi]          ;
        movq    mm4,mm0
        punpcklwd mm0,mm0          ;
        movq    mm1,mm0            ;
        pslld   mm0,4h  ;r
        movq    mm2,mm1            ;
        pslld   mm1,2h
        pand    mm0,mm5 ;rm        ;
        pand    mm1,mm6 ;gm
        pand    mm2,mm7 ;bm        ;
        por     mm0,mm1
        punpckhwd mm4,mm4          ;
        por     mm0,mm2

        movq    mm1,mm4            ;
        pslld   mm4,4h  ;r
        movq    mm2,mm1            ;
        pslld   mm1,2h
        pand    mm4,mm5 ;rm        ;
        pand    mm1,mm6 ;gm
        por     mm4,mm1            ;
        add     esi,8h
        pand    mm2,mm7 ;bm        ;
        add     edi,2h
        por     mm4,mm2            ;
        paddd   mm0,mm4
        movq    mm1,mm0
        psrlq   mm1,32
        paddd   mm0,mm1
        psrld   mm0,2h  ; /4       ;
        movq    mm1,mm0            ;
        movq    mm2,mm0
        pand    mm0,mm5 ;rm        ;
        pand    mm1,mm6 ;gm
        psrld   mm0,4h             ;
        pand    mm2,mm7 ;bm
        psrld   mm1,2h             ;
        por     mm0,mm2
        por     mm0,mm1            ;
        movd    eax,mm0
        mov     [edi-2],ax
        dec     ebp
        jne     _cm_rgb565x_mini
_ggmm_ret:
        ret

_create_mipmap_argb4444:
_cm_argb4444y:
        push    edx
        push    esi
        lea     edx,[4*ecx]
        push    ecx
_cm_argb4444x:
        mov     eax,[esi]
        xor     ebx,ebx
        shld    ebx,eax,4
        shl     eax,4h
        shl     ebx,2h
        shld    ebx,eax,4
        shl     eax,4h
        shl     ebx,2h
        shld    ebx,eax,4
        shl     eax,4h
        shl     ebx,2h
        shld    ebx,eax,4
        shl     eax,4h
        xor     ebp,ebp
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        add     ebx,ebp
        mov     eax,[esi+edx]
        xor     ebp,ebp
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        add     ebx,ebp
        xor     ebp,ebp
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        shl     eax,4h
        shl     ebp,2h
        shld    ebp,eax,4
        add     ebp,ebx

        mov     ebx,ebp
        shr     ebx,2h
        and     ebx,0Fh
        mov     eax,ebp
        shr     eax,4h
        and     al,0F0h
        or      bl,al
        mov     eax,ebp
        shr     eax,6h
        and     ah,0Fh
        or      bh,ah
        shr     ebp,8h
        and     ebp,0F000h
        or      ebx,ebp
        mov     [edi],bx
        add     edi,2h
        add     esi,4h
        dec     ecx
        jne     _cm_argb4444x
        
        pop     ecx
        pop     esi
        lea     esi,[esi+8*ecx]
        pop     edx
        dec     edx
        jne     _cm_argb4444y
        ret

_create_mipmap_argb1555:
_cm_argb1555y:
        push    edx
        push    esi
        lea     edx,[4*ecx]
        push    ecx
_cm_argb1555x:
        mov     eax,[esi]
        xor     ebx,ebx
        shld    ebx,eax,1
        shl     eax,1h
        shl     ebx,2h
        shld    ebx,eax,5
        shl     eax,5h
        shl     ebx,2h
        shld    ebx,eax,5
        shl     eax,5h
        shl     ebx,2h
        shld    ebx,eax,5
        shl     eax,5h
        xor     ebp,ebp
        shld    ebp,eax,1
        shl     eax,1h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        add     ebx,ebp
        mov     eax,[esi+edx]
        xor     ebp,ebp
        shld    ebp,eax,1
        shl     eax,1h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        add     ebx,ebp
        xor     ebp,ebp
        shld    ebp,eax,1
        shl     eax,1h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        shl     eax,5h
        shl     ebp,2h
        shld    ebp,eax,5
        add     ebp,ebx

        mov     ebx,ebp
        shr     ebx,2h
        and     ebx,011111b
        mov     eax,ebp
        shr     eax,4h
        and     eax,01111100000b
        or      ebx,eax
        mov     eax,ebp
        shr     eax,6h
        and     ah,01111100b
        or      bh,ah
        shr     ebp,8h
        and     ebp,08000h
        or      ebx,ebp
        mov     [edi],bx
        add     edi,2h
        add     esi,4h
        dec     ecx
        jne     _cm_argb1555x
        
        pop     ecx
        pop     esi
        lea     esi,[esi+8*ecx]
        pop     edx
        dec     edx
        jne     _cm_argb1555y
        ret

_create_mipmap_argb8332:
_cm_argb8332y:
        push    edx
        push    esi
        lea     edx,[4*ecx]
        push    ecx
_cm_argb8332x:
        mov     eax,[esi]
        xor     ebx,ebx
        shld    ebx,eax,8
        shl     eax,8h
        shl     ebx,2h
        shld    ebx,eax,3
        shl     eax,3h
        shl     ebx,2h
        shld    ebx,eax,3
        shl     eax,3h
        shl     ebx,2h
        shld    ebx,eax,2
        shl     eax,2h
        xor     ebp,ebp
        shld    ebp,eax,8
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,2
        add     ebx,ebp
        mov     eax,[esi+edx]
        xor     ebp,ebp
        shld    ebp,eax,8
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,2
        shl     eax,2h
        add     ebx,ebp
        xor     ebp,ebp
        shld    ebp,eax,8
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,3
        shl     eax,3h
        shl     ebp,2h
        shld    ebp,eax,2
        add     ebp,ebx

        mov     ebx,ebp
        shr     ebx,2h
        and     ebx,011b
        mov     eax,ebp
        shr     eax,4h
        and     al,11100b
        or      bl,al
        mov     eax,ebp
        shr     eax,6h
        and     al,11100000b
        or      bl,al
        shr     ebp,8h
        and     ebp,0FF00h
        or      ebx,ebp
        mov     [edi],bx
        add     edi,2h
        add     esi,4h
        dec     ecx
        jne     _cm_argb8332x
        
        pop     ecx
        pop     esi
        lea     esi,[esi+8*ecx]
        pop     edx
        dec     edx
        jne     _cm_argb8332y
        ret

_create_mipmap_ai88:
_cm_ai88y:
        push    edx
        push    esi
        lea     edx,[4*ecx]
        push    ecx
_cm_ai88x:
        mov     eax,[esi]
        xor     ebx,ebx
        shld    ebx,eax,8h
        shl     eax,8h
        shl     ebx,2h
        shld    ebx,eax,8h
        shl     eax,8h
        xor     ebp,ebp
        shld    ebp,eax,8h
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,8h
        add     ebx,ebp
        mov     eax,[esi+edx]
        xor     ebp,ebp
        shld    ebp,eax,8h
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,8h
        add     ebx,ebp
        xor     ebp,ebp
        shld    ebp,eax,8h
        shl     eax,8h
        shl     ebp,2h
        shld    ebp,eax,8h
        add     ebx,ebp
        mov     eax,ebx
        shr     ebx,2h
        and     ebx,0FFh
        shr     eax,4h
        mov     bh,ah
        mov     [edi],bx
        add     edi,2h
        add     esi,4h
        dec     ecx
        jne     _cm_ai88x
        
        pop     ecx
        pop     esi
        lea     esi,[esi+8*ecx]
        pop     edx
        dec     edx
        jne     _cm_ai88y
        ret


_create_mipmap_ap88:
_cm_ap88y:
        push    edx
        push    esi
        push    ecx
_cm_ap88x:
        mov     eax,[esi]
        mov     [edi],ax
        add     esi,4h
        add     edi,2h
        dec     ecx
        jne     _cm_ap88x
        pop     ecx
        pop     esi
        lea     esi,[esi+8*ecx]
        pop     edx
        dec     edx
        jne     _cm_ap88y
        ret        

.data

_funcs  DD      _create_mipmap_rgb332   ;argb332
        DD      _create_mipmap_p8       ;yiq422
        DD      _create_mipmap_a8       ;a8
        DD      _create_mipmap_a8       ;i8
        DD      _create_mipmap_ai44     ;ai44
        DD      _create_mipmap_p8       ;p8
        DD      _ggmm_ret               ;rsvd0
        DD      _ggmm_ret               ;rsvd1
        DD      _create_mipmap_argb8332 ;argb8332
        DD      _create_mipmap_ap88     ;ayiq8422
        DD      _create_mipmap_rgb565   ;rgb565
        DD      _create_mipmap_argb1555 ;argb1555
        DD      _create_mipmap_argb4444 ;argb4444
        DD      _create_mipmap_ai88     ;ai88
        DD      _create_mipmap_ap88     ;ap88
        DD      _ggmm_ret               ;rsvd2

x       DD      ?
END
