;-----------------------------------------------------------------------------
; VXDCOMM.INC - Include for communication between Server and VxD component
;               Used only with Win9x
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


;------------------------------------------------------------------------------------------
; dgVoodoo: VxdComm.inc
;			Assembly include for communication between Server and VxD component
;			(only for Win9x/Me)
;------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------
; Adatok, amelyeket a szerver átad a kernelmodulnak, amikor regisztrálja magát

VxdRegInfo		STRUC

workerThread_   DD      ?
commAreaPtr     DD      ?
videoMemSize    DD      ?
configPtr       DD      ?
errorCode       DD      ?

VxdRegInfo		ENDS


;------------------------------------------------------------------------------------------
; A dgVoodoo VXD által visszaadott hibakódok (regisztráláskor)

DGSERVERR_NONE					EQU		0
DGSERVERR_ALREADYREGISTERED		EQU		1
DGSERVERR_INITVESAFAILED		EQU		2


;------------------------------------------------------------------------------------------
; A szerver által hívott szolgáltatások (a VXD-hez, csak Win9x/Me alatt)

DGSERVER_SETFOCUSONVM			EQU		01h
DGSERVER_REGSERVER				EQU		02h
DGSERVER_UNREGSERVER			EQU		03h
DGSERVER_RETURNVM				EQU		04h
DGSERVER_SETFOCUSONSYSVM		EQU		05h
DGSERVER_SETALLFOCUSONSYSVM		EQU		06h
DGSERVER_SETALLFOCUSONVM		EQU		07h
DGSERVER_SETORIGFOCUSONVM		EQU		08h
DGSERVER_BLOCKWORKERTHREAD		EQU		09h
DGSERVER_WAKEUPWORKERTHREAD		EQU		0Ah
DGSERVER_REGISTERMODE			EQU		0Bh
DGSERVER_REGISTERACTMODE		EQU		0Ch
DGSERVER_MOUSEEVENTINVM			EQU		0Dh
DGSERVER_SUSPENDVM				EQU		0Eh
DGSERVER_RESUMEVM				EQU		0Fh
DGSERVER_RELEASEKBDMOUSEFOCUS   EQU     10h
