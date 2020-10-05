;-----------------------------------------------------------------------------
; DGVESA.ASM - DOS Real mode part of VESA emulation
;              This resident stub communicates to the server proc
;              via kernel module (dgVoodoo.vxd) (Win9x)
;              or standard NTVDM calls          (WinNT)
;              by delegating all 4Fxx (INT 0x10) VESA interrupt calls
;              to the server proc (or server dll, if VDD mode is used)
;
;              VDD-calls are unified both on Win9x and WinNT using the
;              standard NTVDM call-sequences
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

INCLUDE	..\..\Inc\RMDriver.inc
INCLUDE	..\..\Inc\DosComm.inc
INCLUDE	..\..\Inc\Version.inc

LAST_VALIDERROR		EQU		9
UNKNOWN_ERROR		EQU		10

GROUP	code	rcode, icode
ASSUME	cs:code, ds:code

rcode	segment	para public use16

org		100h

EXTRN	_mousedllheader:_MouseDriverHeader

entry:	jmp		Init

vesaDrvHeader	LABEL _VesaDriverHeader
		DD		_DRIVERID
		DW		OFFSET vDrvVideoHandler
		DW		0
		DW		0
		DW		0

		DW		0
		DD		0
		DW		0
		DW		0
		DW		OFFSET vesaDriverProttable
		DW		vesaDriverProtTableEnd - vesaDriverProttable
		DB		0


vesaDriverProttable     LABEL
        DW      8
        DW      19
        DW      28
        DW      0
        DB      60h,33h,0DBh,66h,0B8h,05h,4Fh,0CDh,10h,61h,0C3h
        DB      60h,66h,0B8h,07h,4Fh,0CDh,10h,61h,0C3h
        
		DB      60h,0Ah,0F6h,74h,0Dh,90h,90h,90h,90h,66h,0B8h,01h,4Fh
		DB			0EBh,12h,90h,90h,90h,8Ah,0FAh,8Ch,0C6h,8Bh,0D7h,0C1h,0EAh,10h,66h,0B8h,0Bh,4Fh,0CDh,10h,61h,0C3h

vesaDriverProtTableEnd	LABEL


vDrvHeaderPointer	DW	OFFSET	vesaDrvHeader
vDrvVideoHandler:
		or		cs:w98,0h
		je		vDrv_NT

vDrv_98:or		cs:vesaDrvHeader.semafor,0h
		mov		cs:vesaDrvHeader.semafor,0h
		jne		vDrvOriginalHandler
		mov		cs:vesaDrvHeader.semafor,1h
		int		10h
		mov		cs:vesaDrvHeader.semafor,0h
		iret		

vDrv_NT:push	es bx
		xor		bx,bx
		mov		es,bx
		les		bx,es:[10h * 4]
		mov		bx,es:[bx - 2]
		cmp		DWORD PTR es:[bx].driverId,_DRIVERID
		pop		bx es
		jne		vDrvOriginalHandler
		cmp		ax,0013h
		jbe		vDrv_callDriver
		cmp		ax,4F00h
		jb		vDrvOriginalHandler
		cmp		ax,4F0Bh
		ja		vDrvOriginalHandler
		cmp		ax,4F02h
		jne		vDrv_callDriver
		pusha
		mov		ah,0Fh
		int		10h
		cmp		al,3h
		popa
		je		vDrv_callDriver
		pusha
		mov		ax,3h
		int		10h
		popa
vDrv_callDriver:
		mov		cs:vesaService,ax
		push	ebp
		xor		bp,bp
		rol		eax,16
		rol		ecx,16
		push	cx
		push	ax
		mov		ax,cs:vesaDrvHeader.vddHandle
		mov		ch,DGCLIENT_VESA
		DispatchCall
		pop		ax
		pop		cx
		rol		eax,16
		rol		ecx,16
		pusha
		cmp		ebp,320 * 65536 + 200
		jne		vDrv320x200Handled
		mov		cs:vesaService,4F02h
vDrv320x200Handled:
		cmp		cs:vesaService,13h
		jbe		vDrvUninstallMouse
		cmp		cs:vesaService,4F02h
		je		vDrvInstallMouse
vDrvIret:
		or		bp,bp
		popa
		je		vDrvToOriginalHandler
		pop		ebp
		iret
vDrvInstallMouse:
		push	bp
		push	edx
		xor		edx,edx
		mov		ax,cs:vesaDrvHeader.vddHandle
		mov		dx,cs:_mousedllheader.driverinfostruc
		mov		ch,DGCLIENT_INSTALLMOUSE
		DispatchCall
		pop		edx
		jc		vDrvMouseUnsuccess

		mov		dx,bp
		shr		ebp,10h
		mov		cx,bp
		xor		bp,bp
		push	cs
		call	cs:_mousedllheader.entrypoint
vDrvMouseUnsuccess:
		pop		bp
		jmp		vDrvIret
		
vDrvUninstallMouse:
		push	bp
		mov		ax,cs:vesaDrvHeader.vddHandle
		mov		ch,DGCLIENT_UNINSTALLMOUSE
		DispatchCall
		jc		vDrvMouseUnsuccess
		
		mov		bp,1h
		push	cs
		call	cs:_mousedllheader.entrypoint
		pop		bp
		jmp		vDrvIret

vDrvToOriginalHandler:
		pop		ebp
vDrvOriginalHandler:
		DB		0EAh
		DD		0h

w98				DB	?
vesaService		DW	?

rcode	ends

icode	segment	use16

EndOfResidentPart	LABEL

Init:	push	cs
		pop		ds
		lea		dx,drvTitleText
		mov		ah,9
		int		21h

		mov		ax,3306h
		int		21h
		cmp		bx,3205h
		setne	w98

		mov		si,81h
vDrvCheckNextOption:
		cmp		BYTE PTR [si],0Dh
		je		vDrvOptionsOk
		mov		ax,[si]
		cmp		ax,'u-'
		je		vDrvUninstFound
		cmp		ax,'U-'
		je		vDrvUninstFound
		cmp		ax,'u/'
		je		vDrvUninstFound
		cmp		ax,'U/'
		jne		vDrvUninstNotFound
vDrvUninstFound:
		mov		drvUninstallNeeded,1h
		add		si,2h
		jmp		vDrvCheckNextOption
vDrvUninstNotFound:
		cmp		ax,'?-'
		je		vDrvHelpFound
		cmp		ax,'?/'
		jne		vDrvHelpNotFound
vDrvHelpFound:
		mov		drvHelpNeeded,1h
		add		si,2h
		jmp		vDrvCheckNextOption
vDrvHelpNotFound:
		inc		si
		jmp		vDrvCheckNextOption

vDrvOptionsOk:
		or		drvHelpNeeded,0h
		je		vDrvNoHelp
		lea		dx,drvUsageText
		mov		ah,9
		int		21h
		jmp		vDrv_NormalExit
vDrvNoHelp:

		push	0h
		pop		es
		les		bx,es:[10h*4]
		mov		bx,es:[bx - 2]
		or		drvUninstallNeeded,0h
		je		vDrvInstall

;---------------
;Uninstall

		cmp		DWORD PTR es:[bx].driverHeader.driverId,_DRIVERID
		lea		dx,drvNoInstalledDriver
		jne		vDrvAlreadyLoaded
		push	bx
		mov		ah,0Fh
		int		10h
		cmp		al,3h
		je		vDrvInTextMode
		mov		ax,3h
		int		10h
vDrvInTextMode:
		pop		bx
		or		w98,0h
		jne		vDrvNoUninstAPICall
		mov		ax,es:[bx].vddHandle
		mov		ch,DGCLIENT_ENDVESA
		DispatchCall
		mov		ax,es:[bx].vddHandle
		UnregisterModule

vDrvNoUninstAPICall:
		mov		eax,DWORD PTR es:[bx].origInt10Handler
		mov		es,es:[bx].dllselector
		push	es
		push	0h
		pop		es
		mov		es:[10h*4],eax
		pop		es
		mov		ah,49h
		int		21h
		lea		dx,drvUninstalled
		mov		ah,9h
		int		21h
		jmp		vDrv_NormalExit

;---------------
;Install

		
vDrvInstall:
		cmp		DWORD PTR es:[bx].driverHeader.driverId,_DRIVERID
		lea		dx,drvAlreadyLoadedMsg
		mov		ah,9h
		je		vDrvAlreadyLoaded

vDrvNoDriverDetected:
		or		w98,0h
		jne		vDrvInitVesaSucceeded

		;mov		w98,0h
		push	cs
		pop		es

		lea		bp,drvDLLName
		mov		ah,19h
		int		21h
		mov		dl,al
		inc		dx
		add		al,'A'
		mov		fileNameBuffer[0],al
		mov		WORD PTR fileNameBuffer[1],'\:'
		mov		ah,47h
		lea		si,fileNameBuffer[3]
		int		21h
		jc		vDrvTryDefaultPath
vDrvSearchTermZero:
		cmp		BYTE PTR [esi],0h
		lea		si,[si + 1]
		jne		vDrvSearchTermZero
		mov		BYTE PTR [si - 1],'\'
		mov		di,si
		lea		si,drvDLLName
		cld
vDrvCopyDLLName:
		movsb
		cmp		BYTE PTR [si - 1],0h
		jne		vDrvCopyDLLName
		mov		ax,3D00h
		lea		dx,fileNameBuffer
		int		21h
		jc		vDrvTryDefaultPath
		xchg	ax,bx
		mov		ah,3Eh
		int		21h
		lea		bp,fileNameBuffer

vDrvTryDefaultPath:
		mov		si,bp
		lea		di,drvDLLInitRoutine
		lea		bx,drvDLLDispatchRoutine
		RegisterModule
		jnc		vDrvDLLLoadOK
vDrvError:
		cmp		ax,LAST_VALIDERROR
		jbe		vDrvErrorOk
		mov		ax,UNKNOWN_ERROR
vDrvErrorOk:
		push	ax
		mov		ah,9h
		lea		dx,drvInitErrorMsg
		int		21h
		pop		bx
		shl		bx,1h
		mov		dx,initErrorMsgTable[bx - 2]
vDrvAlreadyLoaded:
		mov		ah,9h
		int		21h
		jmp		vDrv_AbnormalExit

vDrvDLLLoadOK:
		mov		vesaDrvHeader.vddHandle,ax

		mov		ch,DGCLIENT_GETVERSION
		DispatchCall
		cmp		ax,DGVOODOOVERSION
		mov		ax,9
		jne		vDrvError

		mov		ax,vesaDrvHeader.vddHandle
		push	ax
		mov		dx,cs
		shl		edx,10h
		mov		dx,OFFSET vesaDrvHeader
		mov		ch,DGCLIENT_BEGINVESA
		DispatchCall
		pop		ax
		jcxz	vDrvInitVesaSucceeded
		dec		cx
		;mov		ax,vesaDrvHeader.vddHandle
		UnregisterModule
		xchg	ax,cx
		add		ax,5
		cmp		ax,9
		jb		vDrvError
		mov		ax,9
		jmp		vDrvError

vDrvInitVesaSucceeded:
		push	0h
		pop		es
		mov		eax,es:[10h*4]
		mov		vesaDrvHeader.origInt10Handler,eax
		mov		DWORD PTR vDrvOriginalHandler+1,eax
		mov		vesaDrvHeader.dllselector,cs

		mov		ax,cs
		shl		eax,10h
		lea		ax,vDrvVideoHandler
		mov		es:[10h*4],eax

		lea		dx,drvSuccessfulInit
		mov		ah,9h
		int		21h

		;mov		dx,SIZE RCODE
		lea		dx,EndOfResidentPart
		int		27h
		
vDrv_AbnormalExit:
		mov		ax,4c01h
		int		21h
vDrv_NormalExit:
		mov		ax,4c00h
		int		21h

drvTitleText			DB	0Dh,0Ah,'DOS VESA 2.0 driver of dgVoodoo 1.40, by Dege, 2005'
						DB	0Dh,0Ah,'Copyright and so on blabla...',0Dh,0Ah,'$'
drvUsageText			DB  0Dh,0Ah,'Options:',0Dh,0Ah,'	/u   Uninstall dgVesa driver',0Dh,0Ah,'$'
drvInitErrorMsg			DB	0Dh,0Ah,'Init has failed: $'
drvInitErrorMsg1		DB	'glide2x.dll not found$'
drvInitErrorMsg2		DB	'Dispatch routine not found, probably corrupted Glide2x.dll$'
drvInitErrorMsg3		DB	'Init routine not found, probably corrupted Glide2x.dll$'
drvInitErrorMsg4		DB	'Insufficient memory$'
drvInitErrorMsg5		DB	'InitVESA has failed in glide2x.dll$'
drvInitErrorMsg6		DB	'VESA is disabled, use dgVoodooSetup to enable it!$'
drvInitErrorMsg7		DB	'InitVESA has failed, it seems server is not started!$'
drvInitErrorMsg8		DB	'InitVESA has failed, it seems server is being attached to another DOS Box!$'
drvInitErrorMsg9		DB	'Fatal error: glide2x.dll is not versioned as ', '1.40', '!$'
drvInitErrorMsg10		DB	'Fatal: unknown error, what the fuck is it?$'
drvSuccessfulInit		DB	0Dh,0Ah,'Driver is successfully installed.$'
drvAlreadyLoadedMsg		DB	0Dh,0Ah,'Driver is already loaded, dgVoodoo VESA support is available.$'
drvNoInstalledDriver	DB	0Dh,0Ah,'Cannot uninstall, no driver installed.$'
drvUninstalled			DB	0Dh,0Ah,'Driver is successfully uninstalled.$'
drvDLLName				DB	'Glide2x.dll',0h
drvDLLInitRoutine		DB	'InitDosNT',0h
drvDLLDispatchRoutine	DB	'DispatchDosNT',0h
drvHelpNeeded			DB	0h
drvUninstallNeeded		DB	0h

fileNameBuffer			DB	128 DUP(0)

initErrorMsgTable		DW	OFFSET code:drvInitErrorMsg1, OFFSET code:drvInitErrorMsg2
						DW  OFFSET code:drvInitErrorMsg3, OFFSET code:drvInitErrorMsg4
						DW  OFFSET code:drvInitErrorMsg5, OFFSET code:drvInitErrorMsg6
						DW  OFFSET code:drvInitErrorMsg7, OFFSET code:drvInitErrorMsg8
						DW	OFFSET code:drvInitErrorMsg9, OFFSET code:drvInitErrorMsg10

icode	ends
end		entry

