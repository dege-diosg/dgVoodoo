;-----------------------------------------------------------------------------
; VESA.ASM - Part of dgVoodoo Virtual Device (VxD) implementation, only for Win9x
;            VESA 2.0 interface impl (server side), except for the rendering.
;            Why is it here and why is it duplicated?
;            It is the superior implementation to the one in the glide dll.
;            Only services 4Fxx are duplicated, the others are all related to
;            Win9x kernel driving which couldn't be moved to a user mode dll.
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


INCLUDE	..\..\Inc\VesaDefs.inc

DEFAULT_STATIC_VIDEOMEMORY      EQU     2048*1024
VESA_INTERRUPT                  EQU     10h

PORTLOG MACRO   port
LOCAL   plOK

;        pushad
;        pushfd
;        mov     esi,GlideCommArea
;        test    [esi].kernelflag,8h
;        je      plOK
;        mov     edx,port
;        call    DOPORTLOG
;plOK:
;        popfd
;        popad

ENDM

;----------------------------------------------------------------------------
VxD_LOCKED_CODE_SEG

InitVESA:
;        ret
        mov     ecx,GlideCommArea
        mov     [ecx].VideoMemory,0FFFFFFFFh
                                        ;Kîzîs buffer: VideoMemory=0xFFFFFFFF
                                        ;ez azt jelenti, hogy nincs vesa emu
        xor     eax,eax
        or      VideoMemorySize,eax
        je      NoVESASupport

;Hook a 10h megszak°t†sra
        mov     VesaInitErrPoint,al
        mov     [ecx].VideoMemory,eax   ;Kîzîs buffer: VideoMemory!=0xFFFFFFFF
                                        ;ez azt jelenti, hogy van vesa emu

        mov     eax,VESA_INTERRUPT
        lea     esi,Server_VESA_Support
        VMMCall Hook_V86_Int_Chain
        jc      InitVESAfailed
        mov     VesaInitErrPoint,1h

        or      VXDIsStatic,0h
        jne     IV_VideoMemoryStatic

;Line†ris c°mtartom†ny foglal†sa a videomem¢ri†nak a SHARED poolban
        mov     ecx,VideoMemorySize
        shr     ecx,0Ch
        VMMCall _PageReserve,<PR_SHARED, ecx, 0>
        mov     VideoMemoryLin,eax
        cmp     eax,0FFFFFFFFh
        je      InitVESAfailed
        mov     VesaInitErrPoint,2h

;Line†ris c°mtartom†ny committ†l†sa folytonos fizikai lapokkal
        shr     eax,0Ch
        mov     ecx,VideoMemorySize
        shr     ecx,0Ch
        VMMCall _PageCommitContig,<eax, ecx, PC_USER OR PC_WRITEABLE,0, 0, -1>
        mov     VideoMemoryPhys,eax
        cmp     eax,0FFFFFFFFh
        je      InitVESAfailed

IV_VideoMemoryStatic:
        mov     VesaInitErrPoint,3h

        mov     ebx,3C0h
IV_GetControlOfNextVGAPort:
        mov     edx,ebx
        mov     ecx,VGAPortNewHandler[4*ebx-4*3C0h]
        VxDCall VDD_Takeover_VGA_Port
        mov     VGAPortOldHandler[4*ebx-4*3C0h],ecx
        inc     ebx
        cmp     bl,0E0h
        jne     IV_GetControlOfNextVGAPort

        GetVxDServiceOrdinal eax, Enable_Local_Trapping
        mov     esi,OFFSET32 ServerHookEnableLocalTrapping
        VMMCall Hook_Device_Service

        GetVxDServiceOrdinal eax, Disable_Local_Trapping
        mov     esi,OFFSET32 ServerHookDisableLocalTrapping
        VMMCall Hook_Device_Service

NoVESASupport:
        clc
        ret

InitVESAfailed:
        call    DestroyVesa
        stc
        ret

InitVideoMemoryStatic:

;1M line†ris c°mtartom†ny foglal†sa a videomem¢ri†nak a SHARED poolban
        mov     ecx,DEFAULT_STATIC_VIDEOMEMORY
        mov     VideoMemorySize,ecx
        shr     ecx,0Ch
        VMMCall _PageReserve,<PR_SHARED, ecx, 0>
        mov     VideoMemoryLin,eax
        cmp     eax,0FFFFFFFFh
        je      InitVESAfailedStatic

;Line†ris c°mtartom†ny committ†l†sa folytonos fizikai lapokkal
        shr     eax,0Ch
        mov     ecx,(DEFAULT_STATIC_VIDEOMEMORY) / 4096
        VMMCall _PageCommitContig,<eax, ecx, PC_USER OR PC_WRITEABLE,0, 0, -1>
        mov     VideoMemoryPhys,eax
        cmp     eax,0FFFFFFFFh
        jne     InitVESAsucceed

InitVESAfailedStatic:
        VMMCall _PageFree, <VideoMemoryLin, 0>
        mov     VideoMemorySize,0h
        stc
        ret

InitVESAsucceed:
        clc
        ret

;-----------------------------------

DestroyVesa:
;        ret
        or      VideoMemorySize,0h
        je      DestroyVESAEnd
        cmp     VesaInitErrPoint,0h
        je      DestroyVESAEnd
        mov     eax,VESA_INTERRUPT
        lea     esi,Server_VESA_Support
        VMMCall Unhook_V86_Int_Chain
        jc      DestroyVESAfailed

        cmp     VesaInitErrPoint,2h
        jb      DestroyVESAEnd

        or      VXDIsStatic,0h
        jne     DV_VideoMemoryStatic

        VMMCall _PageFree, <VideoMemoryLin, 0>

DV_VideoMemoryStatic:
        cmp     VesaInitErrPoint,3h
        jb      DestroyVESAEnd

        mov     ebx,3C0h
DV_ThrowControlOfNextVGAPort:
        mov     edx,ebx
        mov     ecx,VGAPortOldHandler[4*ebx-4*3C0h]
        VxDCall VDD_Takeover_VGA_Port
        inc     ebx
        cmp     bl,0E0h
        jne     DV_ThrowControlOfNextVGAPort

        GetVxDServiceOrdinal eax, Enable_Local_Trapping
        mov     esi,OFFSET32 ServerHookEnableLocalTrapping
        VMMCall Unhook_Device_Service

        GetVxDServiceOrdinal eax, Disable_Local_Trapping
        mov     esi,OFFSET32 ServerHookDisableLocalTrapping
        VMMCall Unhook_Device_Service
                
DestroyVESAEnd:
        clc
        ret
DestroyVESAfailed:
        stc
        ret

DestroyVideoMemoryStatic:
        VMMCall _PageFree, <VideoMemoryLin, 0>
        mov     VideoMemorySize,0h
        ret

Server_VESA_Support:
        or      VideoMemorySize,0h
        je      SVS_notserved_
        pushad
        call    VESATestVMSupport
        jne     SVS_notserved
		mov		[ecx].semafor,1h
        mov     V86Handler,ecx

        or      GlideRegVMHandle,0h
        je      SVS_StudyCall
        cmp     ebx,GlideRegVMHandle
        jne     SVS_notserved

SVS_StudyCall:

        mov     eax,[ebp].Client_EAX
        or      ah,ah
        jne     ServerOtherFunc
        and     al,7Fh
        mov     cl,13h
        test    config.Flags,CFG_SUPPORTMODE13H
        je      SVS_CompareMode
        mov     cl,12h
SVS_CompareMode:
        cmp     al,cl
        ja      ServerOtherFunc
        
        mov     eax,GlideCommArea
        or      [eax].VESASession,0h
        je      SVS_notserved

        or      VESALinearMode,0h
        jne     SVS_DontBotherA000
        call    SetVDDVideoMemMapping
SVS_DontBotherA000:
        call    RestoreIOTrapping
        call    UnsetVBEMode
        call    ClientEndGlide
        mov     reg__,0
        jmp     SVS_notserved

;ServerOtherFunc:
        or      ah,ah
        jne     SOF_notstandardBIOSsetmode      ;M¢dbe†ll°t†s
        mov     ah,al
        and     ah,80h
        and     al,7Fh
        cmp     al,50h                          ;ha 50h-nÇl kisebb m¢dsz†m,
                                                ;tov†bbengedjÅk
        jb      SVS_notserved
        sub     al,50h
        add     ax,100h
        xchg    eax,ecx
        push    [ebp].Client_EAX
        call    SetVBEMode_
        pop     [ebp].Client_EAX
        jmp     ServerVSEnd

SOF_notstandardBIOSsetmode:
        cmp     ah,0Fh
        jne     SOF_notstandardBIOSgetmode
        mov     eax,GlideCommArea
        or      [eax].VESASession,0h
        je      SVS_notserved
        mov     cx,[eax].actmodeinfo.ModeNumber
        mov     al,ch
        and     al,80h
        add     al,cl
        add     al,50h
        mov     ah,80
        mov     [ebp].Client_AX,ax
        mov     [ebp].Client_BH,0h
        jmp     ServerVSEnd

SOF_notstandardBIOSgetmode:
ServerOtherFunc:        
        cmp     ax,0013h
        jne     ServerVS_DontConvCall
        mov     al,0Ch
        jmp     ServerVS_CallConvCall
ServerVS_DontConvCall:
        cmp     ax,4F00h
        jb      SVS_notserved
        cmp     ax,4F0Ch
        ja      SVS_notserved

        VMMCall Test_Sys_VM_Handle
        je      SVS_notserved
        
ServerVS_CallConvCall:
        mov     ecx,GlideCommArea
        or      [ecx].VESASession,0h
        jne     ServerVS_AlreadyReg
        cmp     al,0Ah
        je      ServerVS_regifneeded
        cmp     al,0Ch
        je      ServerVS_regifneeded
        cmp     al,02h
        ja      SVS_notserved

ServerVS_regifneeded:
        push    eax
        call    _callreg
        pop     eax

ServerVS_AlreadyReg:

        movzx   eax,al
;	jmp $
        call    [VESAservices+4*eax]
ServerVSEnd:
        mov     eax,V86Handler
		mov		[eax].semafor,0h
        clc
        popad
        ret

SVS_notserved:
        popad
SVS_notserved_:
        stc
        ret

VESATestVMSupport:
        movzx   eax,WORD PTR ds:[VESA_INTERRUPT*4+2]
        movzx   ecx,WORD PTR ds:[VESA_INTERRUPT*4]
        shl     eax,4h
		movzx	ecx,WORD PTR ds:[eax+ecx-2]
		add		ecx,eax
        cmp     [ecx]._driverHeader.driverId,_DRIVERID
        ret

VESADestroyVM:
        mov     eax,GlideCommArea
        or      [eax].VESASession,0h
        je      VDVM_End        
        call    RestoreIOTrapping
        call    ClientEndGlide
        mov     reg__,0
VDVM_End:
        ret

;----------------------------------------------------------------------------
;VESA 2.0 functions implementation

;Function:      4F00    -       Return VBE Controller Information

ReturnVBEControllerInfo:
;        mov     GlideRegVMHandle,ebx

        movzx   edx,[ebp].Client_ES
        shl     edx,4h
        movzx   eax,[ebp].Client_DI
        add     edx,eax
        mov     ebx,[edx]
        cmp     ebx,'2EBV'
        jne     RVBECInfo_dontzero

        mov     edi,edx
        cld
        mov     ecx, SIZE VBEInfoBlock
        xor     al,al
        rep     stosb

        mov     ecx,ENDOEMSTRINGS - BEGINOEMSTRINGS
        lea     edi,[edx].OemData
        lea     esi,BEGINOEMSTRINGS
        rep     movsb

RVBECInfo_dontzero:
        mov     [edx].VBESignature,'ASEV'
        mov     [edx].VBEVersion,0200h
        mov     [edx].Capabilities,1h
        mov     eax,VideoMemorySize
        shr     eax,10h
        mov     [edx].TotalMemory,ax

        lea     esi,VESAModeListWindowed
        lea     edi,[edx].Reserved
        push    edi
RVBEInfo_NextModeToList:
        lea     edi,[edi+2]
RVBEInfo_NextModeToListSkip:
        mov     ax,(dgVoodooModeInfo PTR [esi]).ModeNumber
        mov     [edi-2],ax
        cmp     ax,0FFFFh
        je      RVBEInfo_SuppModeListEnd
        movzx   eax,(dgVoodooModeInfo PTR [esi]).XRes
        movzx   ecx,(dgVoodooModeInfo PTR [esi]).YRes
        imul    eax,ecx
        movzx   ecx,(dgVoodooModeInfo PTR [esi]).BitPP
        imul    eax,ecx
        shr     eax,3h
        cmp     eax,VideoMemorySize
        mov     eax,esi
        lea     esi,[esi+SIZE dgVoodooModeInfo]
        jbe     RVBEInfo_NextModeToList
        or      BYTE PTR (dgVoodooModeInfo PTR [eax]).ModeNumber+1,80h
        jmp     RVBEInfo_NextModeToListSkip
RVBEInfo_SuppModeListEnd:
        pop     eax
        call    GetRealFarPointer
        mov     [edx].VideoModePtr,eax

        lea     eax,OEMNameStr
        cmp     ebx,'2EBV'
        je      RVBECInfo_VESA2Required
        xchg    eax,esi
        mov     eax,edi
        mov     ecx,9
        rep     movsb
        call    GetRealFarPointer
        mov     [edx].OemStringPtr,eax
        jmp     RVBECInfo_OK

RVBECInfo_VESA2Required:

        call    GetRealFarPointerOEMSTRING
        mov     [edx].OemStringPtr,eax
        mov     [edx].OemVendorNamePtr,eax
        lea     eax,ProductNameStr
        call    GetRealFarPointerOEMSTRING
        mov     [edx].OemProductNamePtr,eax
        lea     eax,RevisionStr
        call    GetRealFarPointerOEMSTRING
        mov     [edx].OemProductRevPtr,eax
        mov     [edx].OemSoftwareRev,0100h

RVBECInfo_OK:
        mov     [ebp].Client_AX,004Fh
        ret

GetRealFarPointerOEMSTRING:
        sub     eax,OFFSET BEGINOEMSTRINGS
        lea     eax,[edx+eax].OemData
GetRealFarPointer:
        mov     ecx,eax
        and     ecx,0Fh
        shr     eax,4h
        shl     eax,10h
        or      eax,ecx
        ret

VESARegisterMode:
        mov     esi,[esi].lpvInBuffer
        lea     edi,VESAModeListWindowed
VRM_Compare:
        cmp     WORD PTR [edi],0FFFFh
        je      VRM_NoMoreItem
        mov     ax,(dgVoodooModeInfo PTR [esi]).XRes
        cmp     ax,(dgVoodooModeInfo PTR [edi]).XRes
        jne     VRM_NextItem
        mov     ax,(dgVoodooModeInfo PTR [esi]).YRes
        cmp     ax,(dgVoodooModeInfo PTR [edi]).YRes
        jne     VRM_NextItem
        mov     al,(dgVoodooModeInfo PTR [esi]).BitPP
        cmp     al,(dgVoodooModeInfo PTR [edi]).BitPP
        jne     VRM_NextItem
        mov     ax,(dgVoodooModeInfo PTR [edi]).ModeNumber
        mov     (dgVoodooModeInfo PTR [esi]).ModeNumber,ax
        cld
        mov     ecx,SIZE dgVoodooModeInfo
        rep     movsb
VRM_NoMoreItem:
        ret
VRM_NextItem:
        add     edi,SIZE dgVoodooModeInfo
        jmp     VRM_Compare

VESARegisterActMode:
        mov     esi,[esi].lpvInBuffer
        lea     edi,ActVideoMode
        cld
        mov     ecx,SIZE dgVoodooModeInfo
        rep     movsb
        ret

;----------------------------------------------------------------------------
;Function:      4F01    -       Return VBE Mode Information

ReturnVBEModeInfo:
        mov     bx,[ebp].Client_CX
        mov     ax,014Fh
        call    RVBESearchInModeList
        jc      RVBEMInfo_End

        movzx   eax,[ebp].Client_ES
        shl     eax,4h
        movzx   edi,[ebp].Client_DI
        add     edi,eax

		mov		ax,10011011b
		test	config.Flags, CFG_DISABLEVESAFLATLFB
		je		RVBEMInfo_FlatLFBEnabled
		and		al,7Fh
RVBEMInfo_FlatLFBEnabled:
        mov     [edi].ModeAttributes,ax
        mov     al,111b
        mov     [edi].WinAAttributes,al
        mov     [edi].WinBAttributes,0h
        mov     [edi].WinGranularity,64;4h         ;4K-nkÇnt †ll°tatjuk az ablakot
        mov     [edi].WinSize,40h               ;64K-s egy ablak
        mov     [edi].WinASegment,0A000h
        mov     [edi].WinBSegment,0000h
        mov     [edi].WinFuncPtr,0h
        movzx   eax,(dgVoodooModeInfo PTR [edx]).XRes
        mov     [edi].XResolution,ax
        movzx   ecx,(dgVoodooModeInfo PTR [edx]).BitPP
        mov     [edi].BitsPerPixel,cl
        push    ecx
        imul    eax,ecx
        shr     eax,3h
        mov     [edi].BytesPerScanLineVESA,ax
        movzx   ecx,(dgVoodooModeInfo PTR [edx]).YRes
        mov     [edi].YResolution,cx
        imul    ecx,eax
        mov     eax,VideoMemorySize
        push    edx
        xor     edx,edx
        div     ecx
        pop     edx
        mov     [edi].NumberOfImagePages,al
        mov     [edi].XCharSize,0h
        mov     [edi].YCharSize,0h
        mov     [edi].NumberOfPlanes,1h
        pop     ecx
        mov     ah,6h
        cmp     cl,8
        jne     RVBEMInfo_Not256C
        mov     ah,4h
RVBEMInfo_Not256C:
        mov     [edi].MemoryModel,ah
        mov     [edi].NumberOfBanks,1h
        mov     [edi].BankSize,0h
        mov     [edi].MIReserved,1h

		call	RVBESetModeInfo
		mov     [edi].RedMaskSize,bl
        mov     [edi].RedFieldPosition,bh
        shr     ebx,10h
        mov     [edi].GreenMaskSize,bl
        mov     [edi].GreenFieldPosition,bh
        mov     [edi].BlueMaskSize,al
        mov     [edi].BlueFieldPosition,ah
        shr     eax,10h
        mov     [edi].RsvdMaskSize,al
        mov     [edi].RsvdFieldPosition,ah

        mov     [edi].DirectColorModeInfo,10b
        mov     eax,VideoMemoryPhys
        mov     [edi].PhysBasePtr,eax
        xor     eax,eax
        mov     [edi].OffScreenMemOffset,eax
        mov     [edi].OffScreenMemSize,ax
        mov     ax,004Fh
RVBEMInfo_End:
        mov     [ebp].Client_AX,ax
        ret

RVBESearchInModeList:
        lea     esi,VESAModeListWindowed
RVBEMInfo_SearchModeInList:
        cmp     (dgVoodooModeInfo PTR [esi]).ModeNumber,0FFFFh
        je      RVBESIML_End
        cmp     bx,(dgVoodooModeInfo PTR [esi]).ModeNumber
        mov     edx,esi
        lea     esi,[esi+SIZE dgVoodooModeInfo]
        jne     RVBEMInfo_SearchModeInList
        clc
        ret
RVBESIML_End:
        stc
        ret


RVBESetModeInfo:
;Ha teljes kÇpernyìn futunk, akkor a list†b¢l szedjÅk a pixelform†tumot,
;egyÇbkÇnt a konstans form†tumokat adjuk vissza
		test    config.Flags,CFG_WINDOWED
		jne		RVBE_WindowedMode
        ;cmp     (dgVoodooModeInfo PTR ActVideoMode).ModeNumber,0EEEEh
        ;jne     RVBE_WindowedMode
RVBE_FullScreenMode:
        mov     bl,(dgVoodooModeInfo PTR [edx]).GreenSize
        mov     bh,(dgVoodooModeInfo PTR [edx]).GreenPos
        shl     ebx,10h
        mov     bl,(dgVoodooModeInfo PTR [edx]).RedSize
        mov     bh,(dgVoodooModeInfo PTR [edx]).RedPos
        mov     al,(dgVoodooModeInfo PTR [edx]).RsvSize
        mov     ah,(dgVoodooModeInfo PTR [edx]).RsvPos
        shl     eax,10h
        mov     al,(dgVoodooModeInfo PTR [edx]).BlueSize
        mov     ah,(dgVoodooModeInfo PTR [edx]).BluePos
        jmp     RVBE_PixelFormatGot        

RVBE_WindowedMode:
        lea     edx,ActVideoMode
        cmp     cl,(dgVoodooModeInfo PTR [edx]).BitPP
        je      RVBE_FullScreenMode

        xor     ebx,ebx
        xor     eax,eax
        cmp     cl,16
        jne     RVBEMInfo_Not64Kmode
        mov     ebx,05060B05h
        mov     eax,00000005h
RVBEMInfo_Not64Kmode:
        cmp     cl,32
        jne     RVBEMInfo_Not16Mmode
        mov     ebx,10081808h
        mov     eax,00080808h           ;sorrend: RGBA
RVBEMInfo_Not16Mmode:

RVBE_PixelFormatGot:
		ret

;----------------------------------------------------------------------------
;Function:      4F02    -       Set VBE Mode

SetVBEMode13h:
        mov     cx,13h
        jmp     SetVBEMode_

SetVBEMode:
        mov     cx,[ebp].Client_BX
SetVBEMode_:
        push    ecx
        mov     eax,GlideCommArea
        or      [eax].VESASession,0h
        jne     SVBEM_VBEtoVBE
        lea     edi,ClientSaveRegs
        VMMCall Save_Client_State

        VMMCall Begin_Nest_V86_Exec
        mov     [ebp].Client_EAX,3h
        push    DWORD PTR 10h
        pop     eax
        VMMCall Exec_Int
        VMMCall End_Nest_Exec

        lea     esi,ClientSaveRegs
        VMMCall Restore_Client_State

SVBEM_VBEtoVBE:
        pop     ebx
        mov     ah,bh
        push    ebx
        shr     ah,6h
        and     ah,1h
        mov     VESALinearMode,ah
        and     bx,3FFFh
        mov     ax,014Fh
        call    RVBESearchInModeList
        pop     ebx
        jc      RVBEMInfo_End
        
        mov     esi,GlideCommArea
		or		[esi].kernelflag, KFLAG_VESAFRESHDISABLED
        
		mov     [esi].actmodeinfo.ModeNumber,bx
        movzx   eax,(dgVoodooModeInfo PTR [edx]).XRes
        mov     [esi].actmodeinfo.XRes,ax
        mov     ecx,eax
        movzx   eax,(dgVoodooModeInfo PTR [edx]).BitPP
        mov     [esi].actmodeinfo.BitPP,al
        imul    ecx,eax
        shr     ecx,3h
        mov     [esi].BytesPerScanLine,ecx
        mov     [esi].DisplayOffset,0h
        movzx   eax,(dgVoodooModeInfo PTR [edx]).YRes
        mov     [esi].actmodeinfo.YRes,ax
        imul    ecx,eax

		push	ebx
		push	ecx
		mov		cl,(dgVoodooModeInfo PTR [edx]).BitPP
		call	RVBESetModeInfo
		mov		[esi].actmodeinfo.RedSize,bl
		mov		[esi].actmodeinfo.RedPos,bh
        shr     ebx,10h
		mov		[esi].actmodeinfo.GreenSize,bl
		mov		[esi].actmodeinfo.GreenPos,bh
    	mov		[esi].actmodeinfo.BlueSize,al
		mov		[esi].actmodeinfo.BluePos,ah
        shr     eax,10h
		mov		[esi].actmodeinfo.RsvSize,al
		mov		[esi].actmodeinfo.RsvPos,ah
		pop		ecx
		pop		ebx

        mov     edi,VideoMemoryLin
        mov     [esi].VideoMemory,edi
        and     bh,80h
        jne     SVBM_DontClearVideoMem
        mov     eax,VideoMemorySize
        xor     edx,edx
        div     ecx
        imul    ecx,eax
        shr     ecx,2h
        xor     eax,eax
        cld
        rep     stosd
SVBM_DontClearVideoMem:

        push    DWORD PTR VESASETVBEMODE
        pop     eax
        xor     ecx,ecx
        call    _CallServerProc

        call    EnableVESAIOTrapping

        xor     eax,eax
        mov     VGAPalEntryIndexR,eax
        mov     VGAPalEntryIndexW,eax
        mov     VGAPalRGBIIndexW,eax
        mov     VGAPalRGBIIndexR,eax
        mov     VESAMapOffset,eax
        or      VESALinearMode,al
        jne     SVBM_NoA000Mapping
        push    eax
        call    GetVDDVideoMemMapping
        pop     eax
        call    MapVideoMemIntoA000
SVBM_NoA000Mapping:

;        push    eax
;        mov     eax,30
;        lea     esi,TimerProc
;        VMMCall Set_Global_Time_Out
;        pop     eax

;        mov     ax,004Fh
;        jmp     RVBEMInfo_End

tempVesaOK:
        mov     [ebp].Client_AX,004Fh
        clc
        ret

UnsetVBEMode:
        push    DWORD PTR VESAUNSETVBEMODE
        pop     eax
        xor     ecx,ecx
        jmp     _CallServerProc

;----------------------------------------------------------------------------
;Function:      4F03    -       Return Current VBE Mode

ReturnCurrentVBEMode:
        mov     esi,GlideCommArea
        mov     ax,[esi].actmodeinfo.ModeNumber
        mov     [ebp].Client_BX,ax
        mov     ax,004Fh
        mov     [ebp].Client_AX,ax
        clc
        ret

;----------------------------------------------------------------------------
;Function:      4F04    -       Save/Restore State

VBESaveRestoreState:
        mov     ebx,GlideCommArea
        movzx   edi,[ebp].Client_BX
        movzx   eax,[ebp].Client_ES
        shl     eax,4h
        add     edi,eax
        mov     ax,014Fh
        mov     dl,[ebp].Client_DL
        cmp     dl,2h
        ja      SRS_End
        je      SRS_RestoreState
        or      dl,dl
        jne     SRS_SaveState
        mov     [ebp].Client_BX,((1024+16)/64)+1
        mov     ax,004Fh
        jmp     SRS_End
SRS_SaveState:
        lea     esi,[ebx].paletteVesa
        cld
        mov     cx,1024/4
        rep     movsd
        mov     eax,[ebx].bytesPerScanline
        mov     [edi],eax
        mov     eax,[ebx].displayOffset
        mov     [edi+4],eax
        mov     al,[ebx].vgaRAMDACSize
        mov     [edi+8],al
        mov     ax,004Fh
        jmp     SRS_End
SRS_RestoreState:
        mov     cl,[ebp].Client_CL
        test    cl,1001b
        je      SRS_DontRestoreHardware
        mov     eax,[edi+768]
        mov     [ebx].bytesPerScanLine,eax
        mov     eax,[edi+768+4]
        mov     [ebx].displayOffset,eax
        mov     al,[edi+768+8]
        mov     [ebx].vgaRAMDACSize,al
SRS_DontRestoreHardware:
        and     cl,0100b
        je      SRS_DontRestoreDAC
        mov     esi,edi
        lea     edi,[ebx].paletteVesa
        cld
        mov     cx,1024/4
        rep     movsd
SRS_DontRestoreDAC:
        mov     ax,004Fh
SRS_End:mov     [ebp].Client_AX,ax
        clc
        ret

;----------------------------------------------------------------------------
;Function:      4F05    -       Display Window Control

DisplayWindowControl:
        mov     ax,014Fh
        mov     edx,[ebp].Client_EBX
        or      dl,dl
        jne     DWC_End
        or      dh,dh
        jne     DWC_GetMemoryWindow
        movzx   edx,[ebp].Client_DX
		push	edx
		shl		edx,16
		cmp		edx,VideoMemorySize
		pop		edx
		jae		DWC_End
		mov		eax,edx
        mov     VESAMapOffset,eax
        call    MapVideoMemIntoA000
        mov     ax,004Fh
        jmp     DWC_End
DWC_GetMemoryWindow:
        cmp     dh,1h
        jne     DWC_End
        mov     eax,VESAMapOffset
        mov     [ebp].Client_DX,ax
        mov     ax,004Fh
DWC_End:
        mov     [ebp].Client_AX,ax
        clc
        ret

;----------------------------------------------------------------------------
;Function:      4F06    -       Set/Get Logical Scan Line Length
SetGetLogicalScanLineLength:
        mov     ax,014Fh
        mov     ebx,[ebp].Client_EBX
        cmp     bl,3h
        ja      SGLSLL_End
        mov     esi,GlideCommArea
        movzx   edi,[ebp].Client_CX
        cmp     bl,1h
        je      SGLSLL_GetInfo
        or      bl,bl
        je      SGLSLL_SetSLByPixels
        cmp     bl,2h
        je      SGLSLL_SetSLByBytes
SGLSLL_GetInfo:
        mov     edi,[esi].BytesPerScanLine
        jmp     SGLSLL_SetSLByBytes
SGLSLL_GetMaxScanLineLength:
        mov     eax,VideoMemorySize
        xor     edx,edx
        movzx   ecx,[esi].actmodeinfo.YRes
        div     ecx
        and     al,NOT 3h
        mov     [ebp].Client_BX,ax      ;Bytes per scanline
        mov     [ebp].Client_DX,cx      ;max number of scanlines
        jmp     SGLSLL_GetPixPerSL
SGLSLL_SetSLByPixels:
        mov     ah,2h        
        cmp     di,[esi].actmodeinfo.XRes
        jb      SGLSLL_End
        movzx   eax,[esi].actmodeinfo.BitPP
        imul    edi,eax
        shr     edi,3h
SGLSLL_SetSLByBytes:
        lea     edi,[edi+3]
        and     edi,NOT 3h              ;scanline length 4-gyel oszthat¢ legyen
        mov     eax,VideoMemorySize
        xor     edx,edx
        div     edi
        cmp     ax,[esi].actmodeinfo.YRes
        xchg    eax,ebx
        mov     ax,024Fh
        jb      SGLSLL_End
        mov     [esi].BytesPerScanLine,edi
        mov     [ebp].Client_BX,di      ;Bytes per scanline
        mov     [ebp].Client_DX,bx      ;max number of scanlines
        xchg    eax,edi
SGLSLL_GetPixPerSL:
        shl     eax,3h
        xor     edx,edx
        movzx   ecx,[esi].actmodeinfo.BitPP
        div     ecx
        mov     [ebp].Client_CX,ax      ;pixels per scanline
        mov     ax,004Fh

SGLSLL_End:
        mov     [ebp].Client_AX,ax
        clc
        ret

;----------------------------------------------------------------------------
;Function:      4F07    -       Set/Get Display Start
SetGetDisplayStart:
        mov     edi,GlideCommArea
        mov     al,[ebp].Client_BL
        and     al,7Fh
        cmp     al,1h
        mov     ax,014Fh
        ja      SGDS_End
        jb      SGDS_SetDispStart
        mov     eax,[edi].DisplayOffset
        xor     edx,edx
        div     [edi].BytesPerScanLine
        mov     [ebp].Client_DX,ax
        xchg    eax,edx
        shl     eax,3h
        movzx   ecx,[esi].actmodeinfo.BitPP
        div     ecx
        mov     [ebp].Client_CX,ax
        mov     [ebp].Client_BH,0h
        mov     ax,004Fh
        jmp     SGDS_End
SGDS_SetDispStart:
        movzx   eax,[edi].actmodeinfo.XRes
        movzx   ecx,[edi].actmodeinfo.YRes
        imul    eax,ecx
        movzx   esi,[edi].actmodeinfo.BitPP
        shr     esi,3h
        imul    eax,esi
        movzx   ecx,[ebp].Client_DX
        imul    ecx,[edi].BytesPerScanLine
        movzx   ebx,[ebp].Client_CX
        imul    ebx,esi
        add     ecx,ebx
        add     eax,ecx
        cmp     eax,VideoMemorySize
        mov     ax,024Fh
        ja      SGDS_End
        mov     [edi].DisplayOffset,ecx
        mov     ax,004Fh
SGDS_End:
        mov     [ebp].Client_AX,ax
        clc
        ret
        
;----------------------------------------------------------------------------
;Function:      4F08    -       Set/Get DAC Palette Format
SetGetDACPalFormat:
        mov     ax,014Fh
        mov     ebx,[ebp].Client_EBX
        cmp     bl,01h
        ja      SGDPF_End
        mov     esi,GlideCommArea ;VESACommArea
        or      bl,bl
        jne     SGDPF_GetDACPalFormat
        cmp     bh,8h
        ja      SGDPF_End
        cmp     bh,6h
        jb      SGDPF_End
        cmp     bh,7h
        jne     SGDPF_DontRoundTo6bits
        mov     bh,6h
SGDPF_DontRoundTo6bits:
        mov     al,bh
        mov     [esi].VGARAMDACSize,al
        jmp     SGDPF_RetDACSize

SGDPF_GetDACPalFormat:
        mov     al,[esi].VGARAMDACSize
SGDPF_RetDACSize:
        mov     [ebp].Client_BH,al
        mov     ax,004Fh
SGDPF_End:
        mov     [ebp].Client_AX,ax        
        clc
        ret

;----------------------------------------------------------------------------
;Function:      4F09    -       Set/Get Palette Data

SetGetPaletteData:
        movzx   esi,[ebp].Client_ES
		shl		esi,4h
		movzx	eax,[ebp].Client_DI
		add		esi,eax

SGPD_UserPointerGot:
		mov     ax,014Fh
        mov     cl,[ebp].Client_BL
        and     cl,7Fh
		cmp		cl,3h
        ja      SGPD_End

		cmp		cl,1h
		jbe		SGPD_SetOrGet
		mov		ah,2h
		jmp		SGPD_End
SGPD_SetOrGet:		      
        mov     edi,ebx
        movzx   ebx,[ebp].Client_CX
        movzx   edx,[ebp].Client_DX
        cmp     bx,100h
        ja      SGPD_End
        or      dh,dh
        jne     SGPD_End
        push    ebx
        add     ebx,edx
        cmp     bx,100h
        pop     ebx
        ja      SGPD_End

		mov     edi,GlideCommArea
		mov		al,[edi].VGARAMDACSize
		lea		edi,[edi].VESAPalette
		lea		esi,[esi+4*edx]
		lea		edi,[edi+4*edx]
		mov		edx,ebx

		or		cl,cl
		je		SGPD_SetInit
		mov		cl,2						;Shift count
		mov		ebx,00FCFCFCh				;PalEntryMask
		xchg	esi,edi
		jmp		SGPD_BeginTransferPalData

SGPD_SetInit:
		mov		cl,32-2						;Shift count
		mov		ebx,003F3F3Fh				;PalEntryMask

SGPD_BeginTransferPalData:
		cmp		al,6
		je		SGPD_TransferPalData
		xor		cl,cl
		mov		ebx,00FFFFFFh

SGPD_TransferPalData:
		mov		eax,[esi+4*edx-4]
		and		eax,ebx
		ror		eax,cl
		dec		edx
		mov		[edi+4*edx],eax
		jne		SGPD_TransferPalData

SGPD_Success:
        mov     ax,004Fh

SGPD_End:
        mov     [ebp].Client_AX,ax
        ret

;----------------------------------------------------------------------------
;Function:      4F0A    -       Return VBE Protected Mode Interface

;Ez a fÅggvÇny csak akkor mñkîdik, ha a v86 rezidens rÇsz install†lva van
ReturnVBEProtectedModeInterface:
        mov     ax,014Fh
        mov     bl,[ebp].Client_BL
        or      bl,bl
        jne     RVPMI_InvalidSubFunc
        movzx   eax,WORD PTR ds:[VESA_INTERRUPT*4+2]
        mov     [ebp].Client_ES,ax
        shl     eax,4h
        movzx   ecx,WORD PTR ds:[VESA_INTERRUPT*4]
        add     ecx,eax
		movzx	ecx,WORD PTR [ecx-2]
		add		ecx,eax
		mov		ax,[ecx].protTable
        mov     [ebp].Client_DI,ax
		mov		ax,[ecx].protTableLen
        mov     [ebp].Client_CX,ax
        mov     ax,004Fh
RVPMI_InvalidSubFunc:
        mov     [ebp].Client_AX,ax
        clc
        ret

;Due to restrictions under WinXP
SetGetPaletteDataInProtMode:
		movzx   eax,[ebp].Client_SI
		VMMCall _SelectorMapFlat,<ebx,eax,0>
		movzx	esi,[ebp].Client_DX
		shl		esi,16
		mov		si,[ebp].Client_DI
		add		esi,eax
		movzx	eax,[ebp].Client_BH
		mov		[ebp].Client_DX,ax
		jmp		SGPD_UserPointerGot


reg__ db 0
_callreg:
        cmp reg__,1
        je  nemkell
        mov reg__,1
        push    [ebp].Client_EDX
		push    [ebp].Client_ECX
        call    ClientBeginGlide
		pop     [ebp].Client_ECX
        pop     [ebp].Client_EDX
nemkell:
        ret

CheckToProcessPort:
        cmp     ebx,GlideRegVMHandle
        jne     CRPP_DontProcess
        push    eax
        mov     eax,GlideCommArea ;VESACommArea
        or      [eax].VESASession,0h
        pop     eax
        je      CRPP_DontProcess
        clc
        ret
CRPP_DontProcess:
        stc        
        ret

VGAPortHandler_3C0:
        PORTLOG 3C0h
        push    esi
        mov     esi,VGAPortOldHandler[0]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C1:
        PORTLOG 3C1h
        push    esi
        mov     esi,VGAPortOldHandler[1*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C2:
        PORTLOG 3C2h
        push    esi
        mov     esi,VGAPortOldHandler[2*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C3:
        PORTLOG 3C3h
        push    esi
        mov     esi,VGAPortOldHandler[3*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C4:
        PORTLOG 3C4h
        push    esi
        mov     esi,VGAPortOldHandler[4*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C5:
        PORTLOG 3C5h
        push    esi
        mov     esi,VGAPortOldHandler[5*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C6:
        PORTLOG 3C6h
        push    esi
        mov     esi,VGAPortOldHandler[6*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3C7:
;        mov     ax,0
;        mov gs,ax
;        mov ax,gs:[0]
        push    esi
        pushfd
        mov     esi,VGAPortOldHandler[7*4]
        call    CheckToProcessPort
        jc      VGAPortDefaultHandler_

        test    cl,OUTPUT
        jne     VPH3C7_Output
        mov     eax,VGAPalEntryIndexR
        PORTLOG 3C7h
        jmp     VPH3C7_End
VPH3C7_Output:
        PORTLOG 3C7h
        movzx   eax,al
        mov     VGAPalEntryIndexR,eax
        mov     VGAPalRGBIIndexR,0h
VPH3C7_End:
        popfd
        pop     esi
        ret

VGAPortHandler_3C8:
        push    esi
        pushfd
        mov     esi,VGAPortOldHandler[8*4]
        call    CheckToProcessPort
        jc      VGAPortDefaultHandler_

        test    cl,OUTPUT
        jne     VPH3C8_Output
        mov     eax,VGAPalEntryIndexW
        PORTLOG 3C8h
        jmp     VPH3C8_End
VPH3C8_Output:
        PORTLOG 3C8h
        movzx   eax,al
        mov     VGAPalEntryIndexW,eax
        mov     VGAPalRGBIIndexW,0h
VPH3C8_End:
        popfd
        pop     esi
        ret

VGAPortHandler_3C9:
        push    esi
        pushfd
        mov     esi,VGAPortOldHandler[9*4]
        call    CheckToProcessPort
        jc      VGAPortDefaultHandler_

        mov     esi,GlideCommArea ;VESACommArea
        test    cl,OUTPUT
        je      VPH3C9_Input

        PORTLOG 3C9h
        mov     ebx,VGAPalEntryIndexW
        cmp     [esi].VGARAMDACSIZE,8h
        je      VPH3C9RAMDACOKW
        shl     al,2h
VPH3C9RAMDACOKW:
        mov     edx,VGAPalRGBIIndexW
        movzx   ecx,BYTE PTR RGBIndexTranslating[edx]
		or		[esi].kernelflag,KFLAG_PALETTECHANGED
        lea     esi,[esi].VESAPalette[4*ebx]
        mov     [esi+ecx],al
        inc     edx
        cmp     dl,3h
        jb      VPH3C9_NotNextEntryW
        xor     dl,dl
        inc     bl
        mov     VGAPalEntryIndexW,ebx
VPH3C9_NotNextEntryW:
        mov     VGAPalRGBIIndexW,edx
        jmp     VPH3C9_End

VPH3C9_Input:
        cmp     [esi].VGARAMDACSIZE,8h
        mov     ebx,VGAPalEntryIndexR
        mov     edx,VGAPalRGBIIndexR
                push    ecx
        movzx   ecx,BYTE PTR RGBIndexTranslating[edx]
        lea     esi,[esi].VESAPalette[4*ebx]
        mov     al,[esi+ecx]
                pop     ecx
        je      VPH3C9RAMDACOKR
        shr     al,2h
VPH3C9RAMDACOKR:
        inc     edx
        cmp     dl,3h
        jb      VPH3C9_NotNextEntryR
        xor     dl,dl
        inc     bl
;                mov al,0FFh
        mov     VGAPalEntryIndexR,ebx
VPH3C9_NotNextEntryR:
        mov     VGAPalRGBIIndexR,edx
        PORTLOG 3C9h

VPH3C9_End:
        popfd
        pop     esi
        ret

VGAPortHandler_3CA:
        PORTLOG 3CAh
        push    esi
        mov     esi,VGAPortOldHandler[10*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3CB:
        PORTLOG 3CBh
        push    esi
        mov     esi,VGAPortOldHandler[11*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3CC:
        PORTLOG 3CCh
        push    esi
        mov     esi,VGAPortOldHandler[12*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3CD:
        PORTLOG 3CDh
        push    esi
        mov     esi,VGAPortOldHandler[13*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3CE:
        PORTLOG 3CEh
        push    esi
        mov     esi,VGAPortOldHandler[14*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3CF:
        PORTLOG 3CFh
        push    esi
        mov     esi,VGAPortOldHandler[15*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D0:
        PORTLOG 3D0h
        push    esi
        mov     esi,VGAPortOldHandler[16*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D1:
        PORTLOG 3D1h
        push    esi
        mov     esi,VGAPortOldHandler[17*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D2:
        PORTLOG 3D2h
        push    esi
        mov     esi,VGAPortOldHandler[18*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D3:
        PORTLOG 3D3h
        push    esi
        mov     esi,VGAPortOldHandler[19*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D4:
        PORTLOG 3D4h
        push    esi
        mov     esi,VGAPortOldHandler[20*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D5:
        PORTLOG 3D5h
        push    esi
        mov     esi,VGAPortOldHandler[21*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D6:
        PORTLOG 3D6h
        push    esi
        mov     esi,VGAPortOldHandler[22*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D7:
        PORTLOG 3D7h
        push    esi
        mov     esi,VGAPortOldHandler[23*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D8:
        PORTLOG 3D8h
        push    esi
        mov     esi,VGAPortOldHandler[24*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3D9:
        PORTLOG 3D9h
        push    esi
        mov     esi,VGAPortOldHandler[25*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3DA:
        push    esi
        pushfd
        mov     esi,VGAPortOldHandler[26*4]
        call    CheckToProcessPort
        jc      VGAPortDefaultHandler_

        mov     esi,GlideCommArea ;VESACommArea
        mov     al,[esi].VGAStateRegister3DA
        mov     ah,al
        and     ah,NOT 8h
        xor     ah,1h
        mov     [esi].VGAStateRegister3DA,ah
        test    [ebp].Client_Flags,IF_Mask
        jne     VPH3DA_Ints_OK
        in      al,dx
VPH3DA_Ints_OK:
        PORTLOG 3DAh
        popfd
        pop     esi
        ret
VGAPortHandler_3DB:
        PORTLOG 3DBh
        push    esi
        mov     esi,VGAPortOldHandler[27*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3DC:
        PORTLOG 3DCh
        push    esi
        mov     esi,VGAPortOldHandler[28*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3DD:
        PORTLOG 3DDh
        push    esi
        mov     esi,VGAPortOldHandler[29*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3DE:
        PORTLOG 3DEh
        push    esi
        mov     esi,VGAPortOldHandler[30*4]
        jmp     VGAPortDefaultHandler
VGAPortHandler_3DF:
        PORTLOG 3DFh
        push    esi
        mov     esi,VGAPortOldHandler[31*4]
;        jmp     VGAPortDefaultHandler
VGAPortDefaultHandler:
        pushfd
VGAPortDefaultHandler_:
        push    eax
        mov     eax,GlideCommArea
        or      [eax].VESASession,0h
        pop     eax
        je      VGAPortOrigHandler
        cmp     ebx,GlideRegVMHandle
        jne     VGAPortOrigHandler
        popfd
        pop     esi
        ret
VGAPortOrigHandler:
        popfd
        xchg    esi,[esp]
        ret

DOPORTLOG:
;        mov     VESAParams[0],edx
;        movzx   eax,al
;        mov     VESAParams[4],eax
;        mov     eax,PORTLOG_IN
;        test    cl,OUTPUT
;        je      plOK
;        mov     eax,PORTLOG_OUT
;plOK:   
;        mov     ecx,2h
;        call    _CallServerProc
;DOPORTLOG_OK:
;        ret

BeginProc ServerHookEnableLocalTrapping, HOOK_PROC, OrigEnableLocalTrapping, LOCKED

        pushfd
        cmp     ebx,GlideRegVMHandle
        jne     CallOrigEnable
        cmp     edx,3C0h
        jb      CallOrigEnable
        cmp     edx,3DFh
        ja      CallOrigEnable
        push    eax
        mov     eax,GlideCommArea ;VESACommArea
        or      [eax].VESASession,0h
        pop     eax
        je      CallOrigEnable

        popfd
        ret

CallOrigEnable:
        popfd
        jmp     [OrigEnableLocalTrapping]

EndProc ServerHookEnableLocalTrapping

BeginProc ServerHookDisableLocalTrapping, HOOK_PROC, OrigDisableLocalTrapping, LOCKED

        pushfd
        cmp     ebx,GlideRegVMHandle
        jne     CallOrigDisable
        cmp     edx,3C0h
        jb      CallOrigDisable
        cmp     edx,3DFh
        ja      CallOrigDisable
        push    eax
        mov     eax,GlideCommArea ;VESACommArea
        or      [eax].VESASession,0h
        pop     eax
        je      CallOrigDisable

        popfd
        ret

CallOrigDisable:
        popfd
        jmp     [OrigDisableLocalTrapping]

EndProc ServerHookDisableLocalTrapping

EnableVESAIOTrapping:
        mov     edx,3C0h
EnableVESAIOTrappingNext:
        VMMCall Enable_Local_Trapping
        inc     edx
        cmp     dl,0E0h
        jne     EnableVESAIOTrappingNext
        ret

RestoreIOTrapping:
        VxDCall VDD_Get_VM_Info
        cmp     edi,ebx
        jne     EnableVESAIOTrapping
        mov     edx,3C0h
DisableVESAIOTrappingNext:
        VMMCall Disable_Local_Trapping
        inc     edx
        cmp     dl,0E0h
        jne     DisableVESAIOTrappingNext
        ret

MapVideoMemIntoA000:
        mov     eax,VideoMemoryPhys
        shr     eax,12
        mov     ecx,VESAMapOffset
        shl     ecx,4h
        add     eax,ecx
;        add     eax,VESAMapOffset
        VMMCall _PhysIntoV86, <eax,GlideRegVMHandle,0A0h,64/4,0>
        ret

GetVDDVideoMemMapping:
        VMMCall _CopyPageTable,<0A0h,16,OFFSET VDDMemMapping,0>
        ret

SetVDDVideoMemMapping:
;        VMMCall _GetNulPageHandle
;        VMMCall _MapIntoV86,<eax,GlideRegVMHandle,0A0h,16,0,0>
;        ret
        VMMCall _GetCurrentContext
        xor     ecx,ecx
SVVMM_NextVDDPage:
        mov     eax,[VDDMemMapping+4*ecx]
        push    ecx
        push    eax
        shr     eax,12
        add     cl,0A0h
        push    ecx
        VMMCall _PhysIntoV86, <eax,GlideRegVMHandle,ecx,1,0>
        pop     ecx
        pop     eax
        and     eax,7h
        push    DWORD PTR NOT 7
        pop     edx
        test    al,1h
        je      SVMM_Go
        and     al,NOT 1h
        or      dl,1h
SVMM_Go:
        VMMCall _ModifyPageBits, <GlideRegVMHandle,ecx,1,edx,eax,PG_HOOKED,0>
        pop     ecx
        inc     ecx
        cmp     cl,10h
        jne     SVVMM_NextVDDPage
        ret

VESACreateVMInt10Stub:
        mov     VMToInit,ebx

        clc
        ret

;INT 10h stub beillesztÇse a virtu†lis gÇpbe: ez a rutin akkor h°vodik meg,
;amikor a vm kÇsz, az elsì program (†ltal†ban COMMAND.COM) betîltve Çs
;relok†lva, de mÇg nincs elind°tva. A rutin a kîvetkezìt csin†lja:
;megkeresi a legutols¢ DOS-mem¢riablokkot (ez a betîltîtt programÇ), Çs
;lecs°p annak a vÇgÇbìl egy kis darabot, lÇtrehoz egy foglalt blokkot,
;amibe a stubot bem†solja.
VESAPlaceVMStubIntoVM:
        push    edx
        movzx   eax,dx
        shl     eax,4h
        lea     eax,[eax-10h]
        mov     dl,'M'
        xchg    [eax],dl
        mov     esi,(VMStubCodeEnd - VMStubCode + 1Fh) SHR 4
        movzx   ecx,WORD PTR [eax+3]
        sub     ecx,esi
;        dec     ecx
        mov     [eax+3],cx
        inc     ecx
        shl     ecx,4h
        add     eax,ecx
		push	eax
        mov     [eax],dl
        mov     WORD PTR [eax+1],8h
        dec     esi
        mov     WORD PTR [eax+3],si
        mov     DWORD PTR [eax+8],'OVGD'
        mov     DWORD PTR [eax+12],'OODO'
        lea     edi,[eax+10h]
        lea     esi,VMStubCode
        cld
        push    DWORD PTR VMStubCodeEnd - VMStubCode
        pop     ecx
        push    edi
        rep     movsb
        pop     edi
        mov     eax,ds:[VESA_INTERRUPT*4]
	mov	[edi]._VesaDriverHeader.origInt10Handler,eax
		pop		eax
		shr		eax,4h
		inc		eax
		mov		[edi]._driverHeader.dllselector,ax
	mov	ax,[edi]._driverHeader.entrypoint
        sub     edi,100h
        shl     edi,0Ch
	mov	di,ax
        mov     ds:[VESA_INTERRUPT*4],edi
        pop     edx
        ret

VESAHookBeginApp:
        call    VESATestVMSupport
        jne     VHBA_End
	mov	[ecx].PSP,dx
        ;mov     [ecx-4],dx
        movzx   eax,dx
        shl     eax,4h
        mov     ax,[eax+2Ch]
	mov	[ecx].Env,ax
        ;mov     [ecx-2],ax
VHBA_End:
        ret

VESAHookEndApp:
        call    VESATestVMSupport
        jne     VHEA_End
	movzx   eax,WORD PTR [ecx].PSP
        ;movzx   eax,WORD PTR [ecx-4]
        shl     eax,4h
        movzx   eax,WORD PTR [eax+16h]
	mov     WORD PTR [ecx].PSP,ax
        ;mov     [ecx-4],ax
        shl     eax,4h
        mov     ax,[eax+2Ch]
	mov     WORD PTR [ecx].Env,ax
        ;mov     [ecx-2],ax
VHEA_End:
        ret

VESAGetProgName:
        call    VESATestVMSupport
        ;movzx   edi,WORD PTR [ecx-2]
        movzx   edi,WORD PTR [ecx].Env
        mov     ecx,0FFFFh
        shl     edi,4h

        xor     eax,eax
        mov     edx,GlideCommArea
        mov     [edx].progname[0],al
        cld
SearchProgName:
        repnz   scasb
        jne     CantFindName
        scasb
        loopne  SearchProgName
        inc     eax
        scasw
        lea     ecx,[ecx-2]
        jne     CantFindName
        mov     esi,edi
        dec     eax
        repnz   scasb
        jne     CantFindName
        mov     ecx,edi
GetFileName:
GetFileNameBegin:
        cmp     edi,esi
        je      NameFound
        cmp     BYTE PTR [edi-1],'\'
        je      NameFound
        cmp     BYTE PTR [edi-1],':'
        je      NameFound
        dec     edi
        jmp     GetFileNameBegin
NameFound:
        sub     ecx,edi
        mov     esi,edi
        lea     edi,[edx].progname
        rep     movsb
CantFindName:
        ret

VxD_LOCKED_CODE_ENDS

;----------------------------------------------------------------------------

VxD_LOCKED_DATA_SEG

VideoMemoryLin  DD      ?
VideoMemoryPhys DD      ?
VideoMemorySize DD      ?
OrigEnableLocalTrapping         DD      ?
OrigDisableLocalTrapping        DD      ?
VESAMapOffset   DD      ?
VESALinearMode  DB      ?
ClientSaveRegs  Client_Reg_Struc <>

BEGINOEMSTRINGS LABEL BYTE
OEMNameStr      DB      'dgVoodoo',0h
ProductNameStr  DB      'dgVoodoo VESA Emulation',0h
RevisionStr     DB      'v1.1',0h
ENDOEMSTRINGS   LABEL BYTE

VESAModeListWindowed    LABEL BYTE
                dgvoodooModeInfo <013h, 320, 200, 8>
                dgVoodooModeInfo <100h, 640, 400, 8>
                dgVoodooModeInfo <101h, 640, 480, 8>
                dgVoodooModeInfo <103h, 800, 600, 8>
                dgVoodooModeInfo <105h, 1024, 768, 8>
                dgVoodooModeInfo <107h, 1280, 1024, 8>
                dgVoodooModeInfo <10Eh, 320, 200, 16>
                dgVoodooModeInfo <10Fh, 320, 200, 32>
                dgVoodooModeInfo <111h, 640, 480, 16>
                dgVoodooModeInfo <112h, 640, 480, 32>
                dgVoodooModeInfo <114h, 800, 600, 16>
                dgVoodooModeInfo <115h, 800, 600, 32>
                dgVoodooModeInfo <117h, 1024, 768, 16>
                dgVoodooModeInfo <118h, 1024, 768, 32>
                dgVoodooModeInfo <11Ah, 1280, 1024, 16>
                dgVoodooModeInfo <11Bh, 1280, 1024, 32>
                DW      0FFFFh
ActVideoMode    dgVoodooModeInfo <0EEEEh>

VESAservices    DD      ReturnVBEControllerInfo
                DD      ReturnVBEModeInfo
                DD      SetVBEMode
                DD      ReturnCurrentVBEMode
                DD      VBESaveRestoreState
                DD      DisplayWindowControl
                DD      SetGetLogicalScanLineLength
                DD      SetGetDisplayStart
                DD      SetGetDACPalFormat
                DD      SetGetPaletteData
                DD      ReturnVBEProtectedModeInterface
				DD		SetGetPaletteDataInProtMode
                DD      SetVBEMode13h

VESAParams      DD      8 DUP(?)
VDDMemMapping   DD      16 DUP(?)

VGAPalRGBIIndexW        DD      ?
VGAPalRGBIIndexR        DD      ?
VGAPalEntryIndexW       DD      ?
VGAPalEntryIndexR       DD      ?
V86Handler              DD      ?

VGAPortOldHandler       DD      32 DUP(?)
VGAPortNewHandler       DD      OFFSET VGAPortHandler_3C0
                        DD      OFFSET VGAPortHandler_3C1
                        DD      OFFSET VGAPortHandler_3C2
                        DD      OFFSET VGAPortHandler_3C3
                        DD      OFFSET VGAPortHandler_3C4
                        DD      OFFSET VGAPortHandler_3C5
                        DD      OFFSET VGAPortHandler_3C6
                        DD      OFFSET VGAPortHandler_3C7
                        DD      OFFSET VGAPortHandler_3C8
                        DD      OFFSET VGAPortHandler_3C9
                        DD      OFFSET VGAPortHandler_3CA
                        DD      OFFSET VGAPortHandler_3CB
                        DD      OFFSET VGAPortHandler_3CC
                        DD      OFFSET VGAPortHandler_3CD
                        DD      OFFSET VGAPortHandler_3CE
                        DD      OFFSET VGAPortHandler_3CF
                        DD      OFFSET VGAPortHandler_3D0
                        DD      OFFSET VGAPortHandler_3D1
                        DD      OFFSET VGAPortHandler_3D2
                        DD      OFFSET VGAPortHandler_3D3
                        DD      OFFSET VGAPortHandler_3D4
                        DD      OFFSET VGAPortHandler_3D5
                        DD      OFFSET VGAPortHandler_3D6
                        DD      OFFSET VGAPortHandler_3D7
                        DD      OFFSET VGAPortHandler_3D8
                        DD      OFFSET VGAPortHandler_3D9
                        DD      OFFSET VGAPortHandler_3DA
                        DD      OFFSET VGAPortHandler_3DB
                        DD      OFFSET VGAPortHandler_3DC
                        DD      OFFSET VGAPortHandler_3DD
                        DD      OFFSET VGAPortHandler_3DE
                        DD      OFFSET VGAPortHandler_3DF

RGBIndexTranslating	DB	2, 1, 0

VMStubCode      LABEL BYTE

 DB 044h,045h,047h,045h,01Dh,001h,000h,000h,000h,000h,000h,000h,000h,000h,000h
 DB 000h,000h,000h,000h,000h,000h,000h,041h,001h,03Fh,000h,000h,000h,001h,02Eh
 DB 080h,00Eh,01Ah,001h,000h,075h,011h,090h,090h,02Eh,0C6h,006h,01Ah,001h,001h
 DB 0CDh,010h,02Eh,0C6h,006h,01Ah,001h,000h,0CFh,02Eh,0C6h,006h,01Ah,001h,000h
 DB 02Eh,0FFh,02Eh,00Eh,001h,008h,000h,013h,000h,01Ch,000h,000h,000h,060h,033h
 DB 0DBh,066h,0B8h,005h,04Fh,0CDh,010h,061h,0C3h,060h,066h,0B8h,007h,04Fh,0CDh
 DB 010h,061h,0C3h,060h,00Ah,0F6h,074h,00Dh,090h,090h,090h,090h,066h,0B8h,001h
 DB 04Fh,0EBh,012h,090h,090h,090h,08Ah,0FAh,08Ch,0C6h,08Bh,0D7h,0C1h,0EAh,010h
 DB 066h,0B8h,00Bh,04Fh,0CDh,010h,061h,0C3h

VMStubCodeEnd   LABEL BYTE

VxD_LOCKED_DATA_ENDS
