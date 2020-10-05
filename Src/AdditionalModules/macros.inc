;-----------------------------------------------------------------------------
; MACROS.INC - General macro defs for ASM
;
; Copyright (C) 2001 Dege
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

;----------------------------------------------------------------------------
;Macro include

;IFNDEF  DGMACROS

DGMACROS        EQU     1

LARGE			EQU 	<DWORD PTR>
NULL			EQU		0

M_TRUE          EQU     1
M_FALSE         EQU     0

VISUALC         EQU     M_TRUE ;M_FALSE          ;Visual C function naming convention

;-------------- Macro for Windows API function calling and naming convention

IF      VISUALC EQ M_TRUE

        W32     MACRO   APIfunc,ParamNum

                IFB     <ParamNum>

                        EXTRN   APIfunc:PROC
                        call    APIfunc

                ELSE
	
                        PARAMBYTES      =     ParamNum*4
			APINAME		CATSTR	<_>,<APIfunc>,<@>,%PARAMBYTES
                        EXTRN   APINAME :PROC
                        call    APINAME
                ENDIF

        ENDM

ELSE

        W32     MACRO   APIfunc,ParamNum

                EXTRN   APIfunc:PROC
                call    APIfunc
        ENDM

ENDIF

EXT     MACRO   APIfunc

        EXTRN   APIfunc:PROC

ENDM

;ENDIF
