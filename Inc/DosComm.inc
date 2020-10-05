;-----------------------------------------------------------------------------
; DOSCOMM.INC - Assembly include for communication between Dos and Windows
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

;---------------------------------------------------------------------
; dgVoodoo: DosComm.inc
;			Assembly include for communication between Dos and Windows
;---------------------------------------------------------------------

;A Windows-os DLL-lel való kapcsolattartáshoz
;(RegisterModule, UnregisterModule, DispatchCall)

OPCODE_REGISTERMODULE		EQU     0058C4C4h
OPCODE_DISPATCHCALL			EQU     0258C4C4h
OPCODE_UNREGISTERMODULE		EQU     0158C4C4h


;Makrók a kapcsolattartó függvények közvetlen hívásához

RegisterModule				MACRO
		DD			OPCODE_REGISTERMODULE
ENDM


DispatchCall				MACRO
		DD			OPCODE_DISPATCHCALL	
ENDM


UnregisterModule			MACRO
		DD			OPCODE_UNREGISTERMODULE
ENDM


;---------------------------------------------------------------------

;A szerver szolgáltatásai a DOS-os oldal felé

DGCLIENT_BEGINGLIDE			EQU     01h
DGCLIENT_ENDGLIDE			EQU     02h
DGCLIENT_CALLSERVER			EQU     03h
DGCLIENT_GETPLATFORM		EQU     04h
DGCLIENT_RELEASETIMESLICE   EQU     05h
DGCLIENT_RELEASEMUTEX		EQU		06h
DGCLIENT_VESA				EQU		07h
DGCLIENT_BEGINVESA			EQU		08h
DGCLIENT_ENDVESA			EQU		09h
DGCLIENT_INSTALLMOUSE		EQU		0Ah
DGCLIENT_UNINSTALLMOUSE		EQU		0Bh
DGCLIENT_GETVERSION			EQU		0Ch
