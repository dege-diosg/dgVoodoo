;-----------------------------------------------------------------------------
; STUB.ASM - Replacement of real mode stub programs linked into PE Win executables
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

.386

CurrentProductVersion   EQU     140h

;Konstansok a konfig struktúra flagjeihez
CFG_XPSTYLE	EQU	1h

SETUPCONFIG 	STRUC

ProductVersion  DW      ?
Flags           DD      ?
		DD	8 DUP(0)

SETUPCONFIG 	ENDS


code segment byte public use16
        assume  cs:code, ds:code

config	SETUPCONFIG	<CurrentProductVersion, CFG_XPSTYLE>

;product id
        DB      "DEGE"

Start:  push    cs
        pop     ds
        lea     dx,text
        mov     ah,9h
        int     21h
        mov     ax,4C01h
        int     21h

text    DB      'This program cannot be run in DOS mode.',0Dh,0Ah,'$'

code ends
end Start
