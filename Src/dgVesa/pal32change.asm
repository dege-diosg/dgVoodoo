;-----------------------------------------------------------------------------
; PAL32CHANGE.ASM - Don't know what this is, must be some work-garbage
;                   or test program for dgVesa
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

.386
.model flat

code segment use32
assume cs:code

         pushad
         or     dh,dh
         je     tovabb
         mov    ax,014Fh
         jmp    kilep
tovabb:  mov    bh,dl
         mov    esi,es
         mov    edx,edi
         shr    edx,16
         mov    ax,4f0Bh
         int    10
kilep:
         popad
         ret

code ends

end
