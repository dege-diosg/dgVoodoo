;-----------------------------------------------------------------------------
; RMDRIVER.INC - Assembly header for Real Mode Driver definitions (used by me)
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

;--------------------------------------------------------------------
; dgVoodoo: RMDriver.inc
;			Assembly include for real mode parts/drivers of dgVoodoo
;--------------------------------------------------------------------

_DRIVERID			EQU		'EGED'


;General realmode driver header

_driverHeader   	STRUC

driverId			DD		?
entrypoint      	DW      ?               ;A belépési pont offszetje
dllsize         	DW      ?
dllextramemory  	DW      ?               ;az igényelt többletmemória
dllselector     	DW      ?

_driverHeader   	ENDS


; Header of VESA drivers (Fake, and dgVesa)

_VesaDriverHeader	STRUC

driverHeader		_driverHeader<>
vddHandle			DW	?
origInt10Handler	DD	?
PSP					DW	?
Env					DW	?
protTable			DW	?
protTableLen		DW	?
semafor				DB	?

_VesaDriverHeader	ENDS


; Header of mouse driver

_MouseDriverHeader	STRUC

driverHeader		_driverHeader<>
driverinfostruc 	DW      ?               ;A driverinfo struktúra offszetje
origint33handler 	DD     	?               ;Az eredeti kezelõ címét tárolja
origirq3handler 	DD      ?

_MouseDriverHeader	ENDS