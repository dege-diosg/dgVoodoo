;-----------------------------------------------------------------------------
; CUT.ASM - AFAIR, it is a tool cutting out unwanted parts from a DOS executable
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

.model small

cut  segment byte
  assume cs:cut, ds:cut

org 100h

start:  cld
        mov   sp,offset stack_+768
        mov   si,82h
_st1:   mov   al,[si]
        inc   si
        cmp   al,20h
        je    _st2
        cmp   al,0Dh
        jne   _st1
_st2:   mov   byte ptr [si-1],0h
        mov   dx,82h
        mov   ax,3d00h
        int   21h
        mov   bx,ax
        lea   ax,end_of_prg
        mov   cl,4h
        shr   ax,cl
        push  ds
        pop   cx
        add   cx,ax
        inc   cx
        push  cx
        xor   cx,cx
        xor   dx,dx
        mov   ax,4202h
        int   21h
        push  ax
        push  dx
        xor   dx,dx
        xor   cx,cx
        mov   ax,4200h
        int   21h
        pop   dx
        pop   ax
        pop   cx
        call  read_data
        push  cx
        pop   ds
        
        mov   byte ptr ds:[08h],4h
        dec   word ptr ds:[4h]
        mov   es:szegmensek,dx
        mov   es:byteok,ax
        mov   ah,3Eh
        int   21h
        push  ds
        push  cs
        pop   ds
        mov   dx,82h
        xor   cl,cl
        mov   ah,3ch
        int   21h
        pop   ds
        mov   bx,ax
        xor   dx,dx
        mov   cx,40h
        mov   ah,40h
        int   21h
        push  ds
        pop   cx
        add   cx,32
        mov   ax,es:byteok
        mov   dx,es:szegmensek
        sub   ax,512
        sbb   dx,0h
        call  write_data
        mov   ah,3eh
        int   21h
        mov   ax,4c00h
        int   21h

read_data  proc

        push   cx
        push   ds
        push   bp
        push   cx
        pop    ds
        xor    cx,cx
        push   cx
        push   cx
        push   dx
        push   ax
read_a_new_part___:
        pop    ax
        pop    dx
        mov    cx,0E800h
        cmp    dx,0h
        jne    full_segm
        cmp    ax,0E800h
        ja     full_segm
        mov    cx,ax
full_segm:
        push   dx
        push   ax
        mov    ah,3Fh
        xor    dx,dx
        int    21h
        cmp    ax,0h
        je     rdend
        mov    bp,sp
        add    [bp+6],ax
        adc    [bp+4],word ptr 0h
        push   ax
        pop    cx
        pop    ax
        pop    dx
        sub    ax,cx
        sbb    dx,0h
        push   dx
        push   ax
        push   ds
        pop    ax
        add    ax,3712
        push   ax
        pop    ds
        jmp    short read_a_new_part___
rdend:  pop    cx
        pop    cx
        pop    dx
        pop    ax
        pop    bp
        pop    ds
        pop    cx
        ret

read_data  endp

write_data  proc

        push    cx
        push    ds
        push    bp
        push    cx
        pop     ds
        xor     cx,cx
        push    cx
        push    cx
        push    dx
        push    ax
write_a_new_part:
        pop     ax
        pop     dx
        mov     cx,0E800h
        cmp     dx,0h
        jne     full_segm2
        cmp     ax,0E800h
        ja      full_segm2
        mov     cx,ax
full_segm2:
        push    dx
        push    ax
        mov     ah,40h
        xor     dx,dx
        int     21h
        cmp     ax,cx
        jb      wdend
        mov     bp,sp
        add     [bp+6],ax
        adc     [bp+4],word ptr 0h
        push    ax
        pop     cx
        pop     ax
        pop     dx
        sub     ax,cx
        sbb     dx,0h
        push    dx
        push    ax
        cmp     dx,0h
        jne     cont_write
        cmp     ax,0h
        je      wdend
cont_write:
        push    ds
        pop     ax
        add     ax,3712
        push    ax
        pop     ds
        jmp     write_a_new_part
wdend:  pop     cx
        pop     cx
        pop     dx
        pop     ax
        pop     bp
        pop     ds
        pop     cx
        ret

write_data  endp



byteok  dw  ?
szegmensek  dw ?

cut  ends

.data?

stack_  db  768 dup(?)
end_of_prg  label byte

end  start
