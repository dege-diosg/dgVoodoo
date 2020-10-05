;-----------------------------------------------------------------------------
; MOUSEDRV.ASM - DOS Real mode part of mouse emulation
;                This resident stub (partly) implements the standard
;                INT 0x33 (services 0x0 - 0x24) Mouse interface
;                Also, it applies an INT4 hw-interrupt handler
;                which gets called by the serverproc (or VDD) simulating
;                virtual mouse by DirectInput
; (duplicated file)
;
; Copyright (C) 2003 Dege
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

;egérmeghajtó
;Ez a valós módú meghajtó egy dll-ként viselkedik:
; a védett módú overlay tölti be a DOS-memóriába, és szedi ki onnan,
; installáláskor és eltávolításkor meghívja a belépési pontját, hogy
; a driver a szükséges initet és cleanup-ot el tudja végezni
.386

MOUSE_IRQ       EQU     4h +70h         ;irq 12
MOUSE_IRQMASK   EQU     10h
MOUSE_PIC       EQU     0A1h

MOUSE_MOVEMENT  EQU     1h
LEFT_PRESSED    EQU     2h
LEFT_RELEASED   EQU     4h
RIGHT_PRESSED   EQU     8h
RIGHT_RELEASED  EQU     10h
CENTER_PRESSED  EQU     20h
CENTER_RELEASED EQU     40h

ALLEVENTMASK    EQU     7Fh
ALLEVENTMASK_EX EQU     1Fh

BSTATE_LEFT     EQU     1
BSTATE_RIGHT    EQU     2
BSTATE_CENTER   EQU     4

_mouse_driver_info      STRUC

irqevent			DW      ?               ;eseménymaszk, ami miatt az IRQ 12 bekövetkezett
irqmove_x			DW      ?               ;egér x irányú elmozdulása (IRQ 12)
irqmove_y			DW      ?               ;egér y irányú elmozdulása (IRQ 12)
res_x				DW      ?
res_y				DW      ?

clientarea_x_l		DW      ?               ;intervallum (x0)
clientarea_x_r		DW      ?               ;intervallum (x1)
clientarea_y_l		DW      ?               ;intervallum (y0)
clientarea_y_r		DW      ?               ;intervallum (y1)

pointer_x			DW      ?               ;pointer x pozíciója
pointer_x_mickey	DW      ?
pointer_y			DW      ?               ;pointer y pozíciója
pointer_y_mickey	DW      ?
sumofmove_x			DW      ?               ;x elmozdul sok összege (mickey)
sumofmove_y			DW      ?               ;y elmozdul sok összege (mickey)
sumofmove_x2		DW      ?               ;x elmozdul sok összege (mickey)
sumofmove_y2		DW      ?               ;y elmozdul sok összege (mickey)

buttonstate			DW      ?               ;bit 0 - left pressed
											;bit 1 - right pressed
											;bit 2 - center pressed
ratio_x				DW      ?               ;mickey / 8 pixel ar ny (x)
ratio_y				DW      ?               ;mickey / 8 pixel ar ny (y)
ds_threshold		DW      ?               ;double speed threshold

presscount			DW      3 DUP(?)        ;az egyes gombok lenyomásainak száma
releasecount		DW      3 DUP(?)        ;az egyes gombok elengedéseinek száma
lastpressx			DW      3 DUP(?)        ;az utolsó lenyomási pozíciók (x)
lastpressy			DW      3 DUP(?)        ;az utolsó lenyomási pozíciók (y)
lastreleasex		DW      3 DUP(?)        ;az utolsó elengedési pozíciók (x)
lastreleasey		DW      3 DUP(?)        ;az utolsó elengedési pozíciók (y)

eventmask			DW      ?
eventhandler		DD      ?

eventmask_ex		DW      3 DUP(?)
eventhandler_ex		DW      3 DUP(?)

_mouse_driver_info      ENDS

_header    STRUC

driverId	DD	?
entrypoint      DW      ?               ;A belépési pont offszetje
dllsize         DW      ?
dllextramemory  DW      ?               ;az igényelt többletmemória
dllselector     DW      ?
driverinfostruc DW      ?               ;A driverinfo struktúra offszetje
origint33handler DD     ?               ;Az eredeti kezelõ címét tárolja
origirq3handler DD      ?

_header    ENDS

code segment para public use16
        assume cs:code, ds:code

org 100h
_start:

EXTRA_MEMORY    EQU     SIZE _mouse_driver_info

_dllheader      _header<'EGED', OFFSET _mouse_doinit, MODULE_END-100h, EXTRA_MEMORY, 0, OFFSET driverinfo >
irqmask         DB      ?

;belépési pont: ECX - x felbontás, EDX - y felbontás
;BP=0: init, BP!=0: cleanup
_mouse_doinit:
        pusha
        push    ds
        push    cs
        pop     ds
        push    WORD PTR 0h
        pop     es
        or      bp,bp
        jne     _mouse_reason_uninstall
        lea     di,driverinfo
        push    es cs
        pop     es
        push    cx
        mov     cx,SIZE _mouse_driver_info
        xor     al,al
        cld
        rep     stosb
        pop     cx es
        mov     driverinfo.res_x,cx
        mov     driverinfo.res_y,dx

        call    _mouse_reset
        mov     ax,1bh          ;get sensitivity
        int     33h
        call    _mouse_setsensivity
        mov     ax,3h           ;get position
        int     33h
        push    cx dx
        mov     ax,4h
        mov     cx,7FFFh
        mov     dx,7FFFh
        int     33h
        mov     ax,3h           ;get position
        int     33h
        mov     driverinfo.clientarea_x_r,cx
        mov     driverinfo.clientarea_y_r,dx
        mov     cx,-32768
        mov     dx,-32768
        mov     ax,4h
        int     33h
        mov     ax,3h           ;get position
        int     33h
        mov     driverinfo.clientarea_x_l,cx
        mov     driverinfo.clientarea_y_l,dx
        pop     dx cx
        call    _mouse_set_pos
        mov     ax,4h
        int     33h

        mov     ax,14h
        mov     cx,1h
        xor     dx,dx
        dec     dx
;        mov     es,dx
        int     33h
        call    _mouse_set_eventhandler
        mov     ax,14h
        int     33h
        xor     cx,cx
_minit_studynextmask_ex:
        mov     ax,19h
        push    cx
        int     33h
        jecxz   _minit_nosuchhandler
        call    _mouse_set_eventhandler_ex
_minit_nosuchhandler:
        pop     cx
        inc     cx
        cmp     cx,ALLEVENTMASK_EX+1
        jne     _minit_studynextmask_ex

        mov     eax,DWORD PTR es:[33h*4]
        mov     DWORD PTR _dllheader.origint33handler,eax
        mov     eax,DWORD PTR es:[MOUSE_IRQ*4]
        mov     DWORD PTR _dllheader.origirq3handler,eax
        mov     ax,ds
        shl     eax,10h
        lea     ax,_int33
        mov     es:[33h*4],eax
        lea     ax,_irq3
        mov     es:[MOUSE_IRQ*4],eax
        in      al,MOUSE_PIC
        mov     irqmask,al
        and     al,NOT MOUSE_IRQMASK
        out     MOUSE_PIC,al			;enable IRQ12 in NTVDM
        jmp     _mouse_init_end

_mouse_reason_uninstall:
        in      al,MOUSE_PIC
        mov     ah,irqmask
        and     ah,MOUSE_IRQMASK
        or      al,ah
        out     MOUSE_PIC,al
        mov     eax,DWORD PTR _dllheader.origirq3handler
        mov     es:[MOUSE_IRQ*4],eax
        mov     eax,DWORD PTR _dllheader.origint33handler
        mov     es:[33h*4],eax

        call    _mouse_getsensivity
        mov     ax,1ah                  ;set sensitivity
        int     33h
        mov     cx,driverinfo.clientarea_x_l
        mov     dx,driverinfo.clientarea_x_r
        mov     ax,07h                  ;set x range
        int     33h
        mov     cx,driverinfo.clientarea_y_l
        mov     dx,driverinfo.clientarea_y_r
        mov     ax,08h                  ;set y range
        int     33h
        call    _mouse_query_pos_button
        mov     ax,04h                  ;set pointer pos
        int     33h
        mov     cx,driverinfo.eventmask
        mov     dx,WORD PTR driverinfo.eventhandler[0]
        mov     es,WORD PTR driverinfo.eventhandler[2]
        mov     ax,14h                  ;set event handler
        int     33h

        xor     cx,cx
_mcleanup_studynextmask_ex:
        push    cx
        mov     ax,19h
        int     33h
        jecxz   _mcleanup_nosuchhandler
        xor     cx,cx
        mov     ax,18h
        int     33h
_mcleanup_nosuchhandler:
        pop     cx
        inc     cx
        cmp     cx,ALLEVENTMASK+1
        jne     _mcleanup_studynextmask_ex

        mov     si,2*2h
        mov     di,4*2h
_mcleanup_instnexthandler_ex:
        mov     cx,driverinfo.eventmask_ex[si]
        mov     dx,WORD PTR driverinfo.eventhandler_ex[di]
        mov     es,WORD PTR driverinfo.eventhandler_ex[di]
        mov     ax,18h                  ;set event handler ex
        int     33h
        sub     si,2h
        sub     di,4h
        jne     _mcleanup_instnexthandler_ex
_mouse_init_end:
        pop     ds
        popa
        retf

_int33:
        cmp     ax,24h
        ja      _mouse_iret
        push    bp
        push    ds
        push    cs
        pop     ds
        movzx   bp,al
        shl     bp,1h
        call    ds:[mouse_functions+bp]
        pop     ds
        pop     bp
_mouse_iret:
        iret


_mouse_reset:
        xor     ax,ax
        mov     driverinfo.pointer_x_mickey,ax
        mov     driverinfo.pointer_y_mickey,ax
        mov     driverinfo.clientarea_x_l,ax
        mov     driverinfo.clientarea_y_l,ax
        mov     driverinfo.sumofmove_x,ax
        mov     driverinfo.sumofmove_y,ax
        mov     driverinfo.sumofmove_x2,ax
        mov     driverinfo.sumofmove_y2,ax
        mov     ax,driverinfo.res_x
        mov     driverinfo.clientarea_x_r,ax
        shr     ax,1h
        mov     driverinfo.pointer_x,ax
        mov     ax,driverinfo.res_y
        mov     driverinfo.clientarea_y_r,ax
        shr     ax,1h
        mov     driverinfo.pointer_y,ax
        mov     driverinfo.ratio_x,50
        mov     driverinfo.ratio_y,50
        mov     driverinfo.ds_threshold,64

        xor     ax,ax
        mov     driverinfo.presscount[0],ax
        mov     driverinfo.presscount[2],ax
        mov     driverinfo.presscount[4],ax
        mov     driverinfo.releasecount[0],ax
        mov     driverinfo.releasecount[2],ax
        mov     driverinfo.releasecount[4],ax
        mov     ax,driverinfo.pointer_x
        mov     driverinfo.lastpressx[0],ax
        mov     driverinfo.lastpressx[2],ax
        mov     driverinfo.lastpressx[4],ax
        mov     driverinfo.lastreleasex[0],ax
        mov     driverinfo.lastreleasex[2],ax
        mov     driverinfo.lastreleasex[4],ax
        mov     ax,driverinfo.pointer_y
        mov     driverinfo.lastpressy[0],ax
        mov     driverinfo.lastpressy[2],ax
        mov     driverinfo.lastpressy[4],ax
        mov     driverinfo.lastreleasey[0],ax
        mov     driverinfo.lastreleasey[2],ax
        mov     driverinfo.lastreleasey[4],ax

        mov     bx,3h
        mov     ax,0FFFFh
_mouse_ret:
        ret

_mouse_query_pos_button:
        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        mov     bx,driverinfo.buttonstate
        ret

_mouse_set_pos:
        mov     driverinfo.pointer_x_mickey,0h
        mov     driverinfo.pointer_y_mickey,0h
_mouse_set_pos_:
        cmp     cx,driverinfo.clientarea_x_l
        jge     _msp_ok1
        mov     cx,driverinfo.clientarea_x_l
_msp_ok1:
        cmp     cx,driverinfo.clientarea_x_r
        jle     _msp_ok2
        mov     cx,driverinfo.clientarea_x_r
_msp_ok2:
        cmp     dx,driverinfo.clientarea_y_l
        jge     _msp_ok3
        mov     dx,driverinfo.clientarea_y_l
_msp_ok3:
        cmp     dx,driverinfo.clientarea_y_r
        jle     _msp_ok4
        mov     dx,driverinfo.clientarea_y_r
_msp_ok4:
        mov     driverinfo.pointer_x,cx
        mov     driverinfo.pointer_y,dx
        ret

_mouse_query_buttonpresscount:
        shl     bx,1h
        mov     cx,driverinfo.lastpressx[bx]
        mov     dx,driverinfo.lastpressy[bx]
        xor     ax,ax
        xchg    ax,driverinfo.presscount[bx]
        xchg    ax,bx
        mov     ax,driverinfo.buttonstate
        ret

_mouse_query_buttonreleasecount:
        shl     bx,1h
        mov     cx,driverinfo.lastreleasex[bx]
        mov     dx,driverinfo.lastreleasey[bx]
        xor     ax,ax
        xchg    ax,driverinfo.releasecount[bx]
        xchg    ax,bx
        mov     ax,driverinfo.buttonstate
        ret

_mouse_setxrange:
        mov     driverinfo.clientarea_x_l,cx
        mov     driverinfo.clientarea_x_r,dx
        push    cx dx
        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        call    _mouse_set_pos_
        pop     dx cx
        cmp     cx,driverinfo.pointer_x
        je      _mouse_setxrange_ok
        mov     driverinfo.pointer_x_mickey,0h
        mov     driverinfo.pointer_y_mickey,0h
_mouse_setxrange_ok:
        ret

_mouse_setyrange:
        mov     driverinfo.clientarea_y_l,cx
        mov     driverinfo.clientarea_y_r,dx
        push    cx dx
        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        call    _mouse_set_pos_
        pop     dx cx
        cmp     dx,driverinfo.pointer_y
        je      _mouse_setyrange_ok
        mov     driverinfo.pointer_x_mickey,0h
        mov     driverinfo.pointer_y_mickey,0h
_mouse_setyrange_ok:
        ret

_mouse_getlastmotion:
        xor     cx,cx
        xor     dx,dx
        xchg    cx,driverinfo.sumofmove_x
        xchg    dx,driverinfo.sumofmove_y
        ret

_mouse_setspeed:
        mov     driverinfo.ratio_x,cx
        mov     driverinfo.ratio_y,dx
        ret

_mouse_getstatebuffsize:
        mov     bx,SIZE _mouse_driver_info
        ret

_mouse_savestatebuff:
        push    di si cx
        mov     di,dx
        cld
        lea     si,driverinfo
        mov     cx,SIZE _mouse_driver_info
        rep     movsb
        pop     cx si di
        ret

_mouse_restorestatebuff:
        push    es ds si di cx

        push    ds es
        pop     ds es
        mov     si,dx
        lea     di,driverinfo
        cld
        mov     cx,SIZE _mouse_driver_info
        rep     movsb
        mov     driverinfo.irqevent,0h
        pop     cx di si ds es
        ret

_mouse_setsensivity:
        mov     driverinfo.ratio_x,cx
        mov     driverinfo.ratio_y,dx
        mov     driverinfo.ds_threshold,bx
        ret

_mouse_getsensivity:
        mov     cx,driverinfo.ratio_x
        mov     dx,driverinfo.ratio_y
        mov     bx,driverinfo.ds_threshold
        ret

_mouse_deactivate:
        mov     ax,0FFFFh
        ret

_mouse_sw_reset:
        call    _mouse_reset
        mov     ax,21h
        ret

_mouse_gettype:
        mov     bx,0610h
        mov     cx,0203h
        ret

_mouse_set_eventhandler:
        and     cx,ALLEVENTMASK
        mov     driverinfo.eventmask,cx
        mov     WORD PTR driverinfo.eventhandler[0],dx
        mov     WORD PTR driverinfo.eventhandler[2],es
        jecxz   _mse_nullhandler
        ret
_mse_nullhandler:
        mov     WORD PTR driverinfo.eventhandler[0],cx
        mov     WORD PTR driverinfo.eventhandler[2],cx
        ret

_mouse_exchange_eventhandler:
        and     cx,ALLEVENTMASK
        xchg    driverinfo.eventmask,cx
        xchg    WORD PTR driverinfo.eventhandler[0],dx
        push    ax
        mov     ax,es
        mov     es,WORD PTR driverinfo.eventhandler[2]
        mov     WORD PTR driverinfo.eventhandler[2],ax
        pop     ax
;        or      driverinfo.eventmask
        ret

_mouse_set_eventhandler_ex:
        push    bx dx bp
        and     cx,ALLEVENTMASK_EX
        push    WORD PTR 3h
        mov     bp,cx
        xor     bx,bx
        jecxz   _msee_deinstall
        pop     cx
        mov     ax,0FFFFh
_msee_next:
        cmp     bp,driverinfo.eventmask[bx]
        je      _msee_ret
        or      WORD PTR driverinfo.eventmask[bx],0h
        je      _msee_installhandler
        add     bx,2h
        loop    _msee_next
_msee_ret:
        pop     bp dx bx
        ret
_msee_installhandler:
        mov     driverinfo.eventmask_ex[bx],bp
        shl     bx,1h
        mov     WORD PTR driverinfo.eventhandler_ex[bx],dx
        mov     WORD PTR driverinfo.eventhandler_ex[bx+2],es
        mov     ax,18h
        jmp     _msee_ret
_msee_deinstall:
        pop     cx
        mov     bp,es
_msee_nextdeinst_:
        cmp     dx,driverinfo.eventhandler_ex[bx]
        jne     _msee_nextdeinst
        cmp     bp,driverinfo.eventhandler_ex[bx+2]
        jne     _msee_nextdeinst
        xor     bp,bp
        jmp     _msee_installhandler
_msee_nextdeinst:
        add     bx,2h
        loop    _msee_nextdeinst_
        jmp     _msee_ret

_mouse_get_eventhandler_ex:
        push    bx dx
        and     cx,ALLEVENTMASK_EX
        mov     dx,cx
        xor     bx,bx
        mov     cx,3h
_mgee_next_:
        cmp     dx,driverinfo.eventmask_ex[bx]
        jne     _mgee_next
        shl     bx,1h
        mov     cx,dx
        mov     dx,WORD PTR driverinfo.eventhandler_ex[bx]
        mov     es,WORD PTR driverinfo.eventhandler_ex[bx+2]
        jmp     _mgee_ret
_mgee_next:
        add     bx,2h
        loop    _mgee_next_
        mov     ax,0FFFFh
_mgee_ret:
        pop     dx bx
        ret

_irq3:
        push    es
        push    ds
        push    cs
        pop     ds
        pushad
        mov     bx,driverinfo.irqevent
        test    bl,MOUSE_MOVEMENT
        je      _int3_moveok
        mov     ax,driverinfo.irqmove_x
        add     driverinfo.sumofmove_x,ax
        add     driverinfo.sumofmove_x2,ax
;        add     ax,driverinfo.pointer_x_mickey
        movsx   eax,ax
        movsx   ecx,driverinfo.pointer_x_mickey
        shl     eax,3h                  ;x elmozdul s * 8
        add     eax,ecx
        cdq
        movzx   ecx,driverinfo.ratio_x
        idiv    ecx                     ; / x ratio
        add     ax,driverinfo.pointer_x
        cmp     ax,driverinfo.clientarea_x_l
        jge     _int3_movleftok
        mov     ax,driverinfo.clientarea_x_l
        xor     dx,dx
_int3_movleftok:
        cmp     ax,driverinfo.clientarea_x_r
        jle     _int3_movrightok
        mov     ax,driverinfo.clientarea_x_r
        xor     dx,dx
_int3_movrightok:
        mov     driverinfo.pointer_x,ax
        mov     driverinfo.pointer_x_mickey,dx

        mov     ax,driverinfo.irqmove_y
        add     driverinfo.sumofmove_y,ax
        add     driverinfo.sumofmove_y2,ax
;        add     ax,driverinfo.pointer_y_mickey
        movsx   eax,ax
        movsx   ecx,driverinfo.pointer_y_mickey
        shl     eax,3h                  ;y elmozdul s * 8
        add     eax,ecx
        cdq
        movzx   ecx,driverinfo.ratio_y
        idiv    ecx                     ; / y ratio
        add     ax,driverinfo.pointer_y
        cmp     ax,driverinfo.clientarea_y_l
        jge     _int3_movupok
        mov     ax,driverinfo.clientarea_y_l
        xor     dx,dx
_int3_movupok:
        cmp     ax,driverinfo.clientarea_y_r
        jle     _int3_movbottomok
        mov     ax,driverinfo.clientarea_y_r
        xor     dx,dx
_int3_movbottomok:
        mov     driverinfo.pointer_y,ax
        mov     driverinfo.pointer_y_mickey,dx
_int3_moveok:

        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        test    bl,LEFT_PRESSED
        je      _int3_leftpressedok
        inc     driverinfo.presscount[0]
        mov     driverinfo.lastpressx[0],cx
        mov     driverinfo.lastpressy[0],dx
        or      driverinfo.buttonstate,BSTATE_LEFT
_int3_leftpressedok:
        test    bl,LEFT_RELEASED
        je      _int3_leftreleasedok
        inc     driverinfo.releasecount[0]
        mov     driverinfo.lastreleasex[0],cx
        mov     driverinfo.lastreleasey[0],dx
        and     driverinfo.buttonstate,NOT BSTATE_LEFT
_int3_leftreleasedok:
        test    bl,RIGHT_PRESSED
        je      _int3_rightpressedok
        inc     driverinfo.presscount[2]
        mov     driverinfo.lastpressx[2],cx
        mov     driverinfo.lastpressy[2],dx
        or      driverinfo.buttonstate,BSTATE_RIGHT
_int3_rightpressedok:
        test    bl,RIGHT_RELEASED
        je      _int3_rightreleasedok
        inc     driverinfo.releasecount[2]
        mov     driverinfo.lastreleasex[2],cx
        mov     driverinfo.lastreleasey[2],dx
        and     driverinfo.buttonstate,NOT BSTATE_RIGHT
_int3_rightreleasedok:
        test    bx,CENTER_PRESSED
        je      _int3_centerpressedok
        inc     driverinfo.presscount[4]
        mov     driverinfo.lastpressx[4],cx
        mov     driverinfo.lastpressy[4],dx
        or      driverinfo.buttonstate,BSTATE_CENTER
_int3_centerpressedok:
        test    bx,CENTER_RELEASED
        je      _int3_centerreleasedok
        inc     driverinfo.releasecount[4]
        mov     driverinfo.lastreleasex[4],cx
        mov     driverinfo.lastreleasey[4],dx
        and     driverinfo.buttonstate,NOT BSTATE_CENTER
_int3_centerreleasedok:
        mov     ax,bx
        and     ax,driverinfo.eventmask
        je      _int3_noeventhandler
        push    bx
        mov     bx,driverinfo.buttonstate
        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        mov     si,driverinfo.sumofmove_x2;irqmove_x
        mov     di,driverinfo.sumofmove_y2;irqmove_y
        call    DWORD PTR [driverinfo.eventhandler]
        pop     bx
_int3_noeventhandler:
        and     bl,1Fh
        mov     si,2*2h
_int3_nexteventhandler_ex:
        mov     ax,driverinfo.eventmask_ex[si]
        and     ax,bx
        je      _int3_noeventhandler_ex
        push    bx si
        shl     si,1h
        push    WORD PTR [driverinfo.eventhandler_ex][si][0]
        push    WORD PTR [driverinfo.eventhandler_ex][si][2]
        mov     bp,sp
        mov     bx,driverinfo.buttonstate
        mov     cx,driverinfo.pointer_x
        mov     dx,driverinfo.pointer_y
        mov     si,driverinfo.sumofmove_x2 ;irqmove_x
        mov     di,driverinfo.sumofmove_y2 ;irqmove_y
        call    DWORD PTR [bp]
        add     sp,4h
        pop     si bx
_int3_noeventhandler_ex:
        sub     si,2h
        jne     _int3_nexteventhandler_ex
        mov     al,20h
        out     20h,al
        out     0A0h,al
        mov     driverinfo.irqevent,0h
        popad
        pop     ds
        pop     es
        iret
        

;driverinfo      _mouse_driver_info <0>

mouse_functions LABEL WORD
        DW      _mouse_reset                    ;0
        DW      _mouse_ret                      ;1
        DW      _mouse_ret                      ;2
        DW      _mouse_query_pos_button         ;3
        DW      _mouse_set_pos                  ;4
        DW      _mouse_query_buttonpresscount   ;5
        DW      _mouse_query_buttonreleasecount ;6
        DW      _mouse_setxrange                ;7
        DW      _mouse_setyrange                ;8
        DW      _mouse_ret                      ;9
        DW      _mouse_ret                      ;a
        DW      _mouse_getlastmotion            ;b
        DW      _mouse_set_eventhandler         ;c
        DW      _mouse_ret                      ;d
        DW      _mouse_ret                      ;e
        DW      _mouse_setspeed                 ;f
        DW      _mouse_ret                      ;10
        DW      _mouse_ret                      ;11
        DW      _mouse_ret                      ;12
        DW      _mouse_ret      ;double speed ! ;13
        DW      _mouse_exchange_eventhandler    ;14
        DW      _mouse_getstatebuffsize         ;15
        DW      _mouse_savestatebuff            ;16
        DW      _mouse_restorestatebuff         ;17
        DW      _mouse_set_eventhandler_ex      ;18
        DW      _mouse_get_eventhandler_ex      ;19
        DW      _mouse_setsensivity             ;1a
        DW      _mouse_getsensivity             ;1b
        DW      _mouse_ret                      ;1c
        DW      _mouse_ret                      ;1d
        DW      _mouse_ret                      ;1e
        DW      _mouse_deactivate               ;1f
        DW      _mouse_ret                      ;20
        DW      _mouse_sw_reset                 ;21
        DW      _mouse_ret                      ;22
        DW      _mouse_ret                      ;23
        DW      _mouse_gettype                  ;24

driverinfo      LABEL _mouse_driver_info

MODULE_END LABEL

code ends
end _start
