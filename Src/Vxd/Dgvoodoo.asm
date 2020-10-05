;-----------------------------------------------------------------------------
; DGVOODOO.ASM - dgVoodoo Virtual Device (VxD) implementation, only for Win9x
;                Communicates with VESA/Glide client present in a DOS VM and
;                the 32 bit server proc; It's a bridge between them
;                through the kernel.
;                Also, it contains VESA 2.0 impl, except for the rendering.
;                VESA rendering is still done in the server proc, of course.
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


.386p

MINIVDD EQU 1

.xlist
INCLUDE vmm.inc
INCLUDE vwin32.inc
INCLUDE vkd.inc
INCLUDE vmd.inc

INCLUDE minivdd.inc
INCLUDE shell.inc
INCLUDE dosmgr.inc
.list

INCLUDE	..\..\Inc\RMDriver.inc
INCLUDE ..\..\Inc\Dos.inc
INCLUDE ..\..\Inc\DosComm.inc
INCLUDE ..\..\Inc\VxdComm.inc

SHARED_ADDRESS_SPACE    EQU     80000000h

;----------------------------------------------------------------------------

DgVoodooDeviceID      	EQU     Undefined_Device_ID
DgVoodooInitOrder       EQU     Undefined_Init_Order

Declare_Virtual_Device DGVOODOO, 1,0, DgVoodooControl, \
                       DgVoodooDeviceID, DgVoodooInitOrder

INCLUDE vesa.asm
;----------------------------------------------------------------------------
;DgVoodoo VXD control procedure:

VxD_LOCKED_CODE_SEG

BeginProc       DgVoodooControl

        Control_Dispatch     Device_Init,				VXD_Init_Static
        Control_Dispatch     Sys_Dynamic_Device_Init,	VXD_Init_Dynamic
        Control_Dispatch     Sys_Dynamic_Device_Exit,	VXD_Exit
        Control_Dispatch     W32_DEVICEIOCONTROL,		GlideServerSupp
        Control_Dispatch     VM_Init,					VESACreateVMInt10Stub
        Control_Dispatch     Destroy_VM,				GlideDestroyVM
        Control_Dispatch     Terminate_Thread,			GlideDestroyThread
        clc
        ret

EndProc         DgVoodooControl

;----------------------------------------------------------------------------

InitCommArea:
        mov     edi,GlideCommArea
        push    edi
        mov     ecx,ExeCodes-kernelflag
        cld
        xor     al,al
        rep     stosb
        pop     eax
        lea     edx,[eax].FuncData
        mov     [eax].FDPtr,edx
        mov     [eax].areaId,AREAID_COMMAREA
        ret

;VXD inicializ·l·s·t vÈgzı rutin (dinamikus esetben, ez az ·ltal·nosabb)
BeginProc       VXD_Init_Dynamic

        xor     ecx,ecx
        VMMCall Create_Semaphore
        mov     semhandle,eax

        mov     eax,SIZE CommArea
        call    _GetMem
        mov     GlideCommArea,eax

        call    InitCommArea

        push    DWORD PTR 6h
        pop     eax
        lea     esi,ServerCaller
        VMMCall Hook_PM_Fault
        mov     OrigFault6Handler,esi

        GetVxDServiceOrdinal eax, DOSMGR_End_V86_App
        mov     esi,OFFSET32 ServerHookEndV86App
        VMMCall Hook_Device_Service       

        GetVxDServiceOrdinal eax, DOSMGR_Begin_V86_App
        mov     esi,OFFSET32 ServerHookBeginV86App
        VMMCall Hook_Device_Service

;        mov     al,56
;        xor     ah,ah
;        mov     ebx, ((NOT (SS_Alt OR SS_Ctrl OR SS_Shift)) SHL 16) + (SS_Alt OR SS_Ctrl)
;        mov     ebx, ((NOT (SS_Alt)) SHL 16) + (SS_Alt)
;        xor ebx,ebx
;        mov     cl,CallOnPress
;        lea     esi,Hotkeyproc
;        xor     edi,edi
;        VxDCall VKD_Define_Hot_Key
;        mov     hotkeyh,eax

        mov     VXDIsStatic,0h
        clc
        ret        

EndProc         VXD_Init_Dynamic

;VXD inicializ·l·s·t vÈgzı rutin (statikus esetben)
BeginProc       VXD_Init_Static

        xor     ecx,ecx
        VMMCall Create_Semaphore
        mov     semhandle,eax

        mov     eax,SIZE CommArea
        call    _GetMem
        mov     GlideCommArea,eax

        call    InitVideoMemoryStatic

        call    InitCommArea

        push    DWORD PTR 6h
        pop     eax
        lea     esi,ServerCaller
        VMMCall Hook_PM_Fault
        mov     OrigFault6Handler,esi

        GetVxDServiceOrdinal eax, DOSMGR_End_V86_App
        mov     esi,OFFSET32 ServerHookEndV86App
        VMMCall Hook_Device_Service       

        GetVxDServiceOrdinal eax, DOSMGR_Begin_V86_App
        mov     esi,OFFSET32 ServerHookBeginV86App
        VMMCall Hook_Device_Service

        mov     VXDIsStatic,1h
        clc
        ret        

EndProc         VXD_Init_Static

Hotkeyproc:
;        VxDCall VKD_Cancel_Hot_Key_State
        nop
        ret

;----------------------------------------------------------------------------
;VXD cleanup-j·t vÈgzı rutin

BeginProc       VXD_Exit

;        mov     eax,hotkeyh
;        VxDCall VKD_Remove_Hot_Key

;        call    DestroyVESA
        GetVxDServiceOrdinal eax, DOSMGR_Begin_V86_App
        mov     esi,OFFSET32 ServerHookBeginV86App
        VMMCall Unhook_Device_Service

        GetVxDServiceOrdinal eax, DOSMGR_End_V86_App
        mov     esi,OFFSET32 ServerHookEndV86App
        VMMCall Unhook_Device_Service

        push    DWORD PTR 6h
        pop     eax
        lea     esi,ServerCaller
        VMMCall Unhook_PM_Fault

        call    DestroyVideoMemoryStatic

        mov     eax,GlideCommArea
        call    _FreeMem

        mov     eax,semhandle
        VMMCall Destroy_Semaphore
        clc
        ret

EndProc         VXD_Exit

;----------------------------------------------------------------------------

;6-os kivÈtel kezelıje a l·ncban
BeginProc ServerCaller, HOOK_PROC, OrigFault6Handler, LOCKED

        pushad
        movzx   eax,[ebp].Client_CS
        VMMCall _SelectorMapFlat, <ebx, eax, 0>
        add     eax,[ebp].Client_EIP

        or      GlideRegServerThread,0h
        je      SC_RealFault
        cmp     DWORD PTR [eax],OPCODE_DISPATCHCALL
        je      SC_DispatchCall
        cmp     DWORD PTR [eax],OPCODE_REGISTERMODULE
        je      SC_RegisterModule
        cmp     DWORD PTR [eax],OPCODE_UNREGISTERMODULE
        jne     SC_RealFault

SC_UnregisterModule:
        jmp     SC_Exit        

SC_RegisterModule:
        movzx   eax,[ebp].Client_DS
        VMMCall _SelectorMapFlat, <ebx, eax, 0>
        cmp     eax,0FFFFFFFFh
        stc
        je      SC_notfound_
        xchg    eax,esi
        mov     ecx,esi
        movzx   eax,[ebp].Client_SI
        add     esi,eax
        mov     eax,esi
SC_searchstrnull:
        cmp     BYTE PTR [esi],0h
        lea     esi,[esi+1]
        jne     SC_searchstrnull
        sub     esi,2h
SC_searchstrseparator:
        cmp     eax,esi
        je      SC_srcfound_
        cmp     BYTE PTR [esi],'\'
        je      SC_srcfound
        cmp     BYTE PTR [esi],':'
        je      SC_srcfound
        dec     esi
        jmp     SC_searchstrseparator
SC_srcfound:
        inc     esi
SC_srcfound_:
        lea     edi,_ntdllname211
        cmp     areaIdRegistered,AREAID_REGSTRUCTURE_GLIDE211
        je      SC_nameok
        lea     edi,_ntdllname243
SC_nameok:
        call    _strcmp
SC_notfound_:
        push    DWORD PTR 1h
        pop     eax
        jc      SC_notfound

        movzx   esi,[ebp].Client_BX
        add     esi,ecx
        lea     edi,_ntdispatchname
        call    _strcmp
        push    DWORD PTR 2h
        pop     eax
        jc      SC_notfound

        movzx   eax,[ebp].Client_ES
        VMMCall _SelectorMapFlat, <ebx, eax, 0>
        cmp     eax,0FFFFFFFFh
        stc
        je      SC_initnotfound        
        movzx   esi,[ebp].Client_DI
        add     esi,eax
        lea     edi,_ntinitname
        call    _strcmp
SC_initnotfound:
        push    DWORD PTR 3h
        pop     eax

SC_notfound:
        push    DWORD PTR 1h
        pop     ecx
        jc      SC_DLLNotFound
        mov     eax,0EAC8h
        xor     ecx,ecx

SC_DLLNotFound:
        mov     [ebp].Client_EAX,eax
        and     [ebp].Client_Flags,NOT CF_Mask
        or      [ebp].Client_Flags,cx
        jmp     SC_Exit

SC_DispatchCall:
        call    ProcessCall

SC_Exit:
        add     [ebp].Client_EIP,4h
        popad
        ret
SC_RealFault:
        popad
        jmp     [OrigFault6Handler]

EndProc ServerCaller

;Ez a fgv. dolgozza fel a DispatchCall-h°v†sokat
BeginProc       ProcessCall

        lea     esi,ClientProcessFuncs
        movzx   eax,[ebp].client_CH
GPMAPInextfunc:
        cmp     DWORD PTR [esi],0FFFFFFFFh
        je      GPMAPIunsuppfunc
        cmp     eax,[esi]
        lea     esi,[esi+8]
        jne     GPMAPInextfunc
        call    [esi-4]
        mov     [ebp].client_EAX,eax
GPMAPIunsuppfunc:
        clc
        ret

EndProc         ProcessCall

;----------------------------------------------------------------------------
;DOS-os program †ltal h°vhat¢ szerver-szolg†ltat†sok
;----------------------------------------------------------------------------

;Ezzel a szolg·ltat·ssal kÈrdezi le a dos driver a platform tÌpus·t
ClientGetPlatformType:
        mov     eax,PLATFORM_DOSWIN9X        
        ret

;----------------------------------------------------------------------------
;Ezzel jelzi a DOS-os virtu†lis gÇp, hogy el szeretnÇ kezdeni a
;Glide session-t (register). Ha m†r egy m†sik is elkezdte, akkor hiba.
;Be:    EBX             -       VM handle
;Ki:    client EDX      -       kîzîs puffer (kommunik†ci¢s terÅlet) c°me

ClientBeginGlide:
        cmp     ebx,GlideRegVMHandle
        je      CBGincref
        or      [ebp].client_flags,1h
        or      GlideRegVMHandle,0h
        jne     CBGalreadybegun
        call    InitCommArea
        mov     GlideRegVMHandle,ebx
        cmp     ebx,GlideRegDOSProgVMHandle
        mov     GlideRegDOSProgVMHandle,ebx
        je      CBGNotNewClient
        push    DWORD PTR DGVOODOONEWCLIENTREGISTERING
        pop     eax
        xor     ecx,ecx
        call    _CallServerProc
        mov     GlideRegDOSStartCount,1h
        call    VESAGetProgName
;        call    _GetDOSProgPSP
;        mov     GlideRegDOSProgPSP,eax
CBGNotNewClient:
        VMMCall Get_Time_Slice_Priority
        push    eax
        and     eax,VMStat_Background OR VMStat_High_Pri_Back
        mov     GlideRegVMBackgroundBit,eax
        pop     eax
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
        VMMCall Set_Time_Slice_Priority
        VxDCall SHELL_GetVMInfo
        and     eax,SGVMI_Windowed
        movzx   eax,dx
        mov     edx,GlideCommArea
        mov     [edx].WinOldApHWND,eax
        je      CBGInFullScr
        or      [edx].kernelflag,2h
CBGInFullScr:

CBGincref:
        inc     [GlideRegRefCount]
        mov     eax,GlideCommArea
        mov     [ebp].client_EDX,eax
        mov     [ebp].client_ECX,eax
        and     [ebp].client_flags,NOT 1h
        mov     eax,SHARED_ADDRESS_SPACE
;        xor     eax,eax
CBGalreadybegun:
        ret

;----------------------------------------------------------------------------
;Ezzel jelzi a DOS-os gÇp a Glide session vÇgÇt (unregister).
;Csak az vÇgezheti, aki elkezdte, kÅlînben hiba.

ClientEndGlide:
        push    DWORD PTR 1h
        pop     eax
        cmp     ebx,GlideRegVMHandle
        jne     CEGnotsameVM
        dec     [GlideRegRefCount]
        jne     CEGnotsameVM
        xor     eax,eax
        mov     GlideRegVMHandle,eax
        mov     GlideRegVMBackgroundBit,0h
CEGnotsameVM:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
;A DOS-os gÇp megh°vja a szervert, azaz: a DOS-os gÇpet leblokkoljuk egy
;szemaforral (de azÇrt a megszak°t†si rutinok legyenek benne vÇgrehajtva!).
;Ha a szerverproc vÇgzett az adott munk†val, akkor feloldja a blokkol†st.

ClientCallServer:
        mov     eax,GlideCommArea
        or      DWORD PTR [eax].kernelflag,1h
        call    ServerWakeUpWT
        mov     eax,semhandle
        mov     ecx,Block_Svc_Ints
        VMMCall Wait_Semaphore

        mov     eax,GlideCommArea
        test    DWORD PTR [eax].kernelflag,80h
        je      CCS_End
        and     DWORD PTR [eax].kernelflag,NOT 80h
        call    KbdRestrictedCall
CCS_End:
        push    DWORD PTR 1h
        pop     eax
        ret

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;Glide VXD szolg†ltat†sai a szerverprocessz sz†m†ra
;(DeviceIoControl-on keresztÅl)
;ebx: vmhandle, eax: w32_deviceiocontrol, esi: offset diocparams

GlideServerSupp:
        
        lea     edi,ServerProcessFuncs
        mov     eax,[esi].dwIoControlCode
GSSnextfunc:
        cmp     DWORD PTR [edi],0FFFFFFFFh
        je      GSSunsuppfunc
        cmp     eax,[edi]
        lea     edi,[edi+8]
        jne     GSSnextfunc
        call    [edi-4]
        ret
GSSunsuppfunc:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
InitVXDOpen:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
;A szerverproc ezzel tudja mag†t bejegyeztetni, Çs a tov†bbiakban
;kommunik†lni

ServerRegister:
        mov     ecx,[esi].lpvInBuffer
        mov     (VxdRegInfo PTR [ecx]).errorCode,DGSERVERR_ALREADYREGISTERED
        or      GlideRegServerThread,0h
        jne     SRdone_
        mov     eax,(VxdRegInfo PTR [ecx]).workerThread_
        mov     WorkerThread,eax
        mov     eax,[ecx].areaId_regs
        mov     areaIdRegistered,eax
        or      VXDIsStatic,0h
        jne     SRVideoMemReserved
        mov     eax,(VxdRegInfo PTR [ecx]).videoMemSize
        mov     VideoMemorySize,eax
SRVideoMemReserved:
        mov     (VxdRegInfo PTR [ecx]).errorCode,DGSERVERR_INITVESAFAILED
        push    ecx
        mov     ecx,(VxdRegInfo PTR [ecx]).commAreaPtr
        mov     eax,GlideCommArea
        mov     [ecx],eax
        call    InitVESA
        pop     ecx
        jc      SRdone_
        mov     (VxdRegInfo PTR [ecx]).errorCode,DGSERVERR_NONE
        VMMCall Get_Cur_Thread_Handle
        mov     GlideRegServerThread,edi
        mov     esi,(VxdRegInfo PTR [ecx]).configPtr
        lea     edi,config
        mov     ecx,SIZE dgVoodooConfig
        cld
        rep     movsb
;Ha az egÇrf¢kusz a VM-ben van, Çs a Ctrl-Alt engedÇlyezve, vagy
;kÇplop†s engedÇlyezve, akkor hookoljuk a billentyñzetet
        test    DWORD PTR config.Flags,CFG_SETMOUSEFOCUS
        je      SRCheckGrabbing
        test    DWORD PTR config.Flags,CFG_CTRLALT
        jne     SRHookKbd
SRCheckGrabbing:
        test    DWORD PTR config.Flags,CFG_GRABENABLE
        je      SRdone_
;        mov     al,1Dh
;        mov     ah,0h
;        mov     cl,CallOnPress
;        ShiftState 0h,0h
;;        mov     ebx,0FFFF0000h
;        xor     edi,edi
;        lea     esi,GrabKeyFunc
;        VxDCall VKD_Define_Hot_Key
;        mov     HotKeyHandle,eax
SRHookKbd:
        GetVxDServiceOrdinal eax, VKD_Filter_Keyboard_Input
        mov     esi,OFFSET32 ServerHookFKI
        VMMCall Hook_Device_Service
SRdone_:xor     eax,eax
        ret

;----------------------------------------------------------------------------
;A szerverproc ezzel tudja mag†t kijelenteni (unregister)

ServerUnregister:
        call    DestroyVESA
        test    DWORD PTR config.Flags,CFG_GRABENABLE
        je      SURdone_
;        mov     eax,HotKeyHandle
;        VxDCall VKD_Remove_Hot_Key
        GetVxDServiceOrdinal eax, VKD_Filter_Keyboard_Input
        mov     esi,OFFSET32 ServerHookFKI
        VMMCall Unhook_Device_Service
SURdone_:
        mov     GlideRegServerThread,0h
        ret

;----------------------------------------------------------------------------
;Ezzel a szolg†ltat†ssal a szerverprocessz a DOS-os gÇpre †ll°tja a
;billentyñzet-f¢kuszt

ServerSetFocusOnVM:
        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SSFVMnoVM
        mov     KeyboardFocusonVM,1h
        mov     edx,VKD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control

        test    config.Flags,CFG_SETMOUSEFOCUS
        je      SSFoVM_NoMouseFocus
        mov     MouseFocusonVM,1h
        mov     edx,VMD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control
SSFoVM_NoMouseFocus:

        VMMCall Get_Time_Slice_Priority
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
        VMMCall Set_Time_Slice_Priority
        VMMCall Set_Execution_Focus
SSFVMnoVM:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
;Ezzel a szolg†ltat†ssal a szerverprocessz a DOS-os gÇpre †ll°t minden f¢kuszt

ServerSetOrigFocusOnVM:
ServerSetAllFocusOnVM:
        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SSFVMnoVM
        mov     KeyboardFocusonVM,1h
        mov     MouseFocusonVM,1h
        xor     edx,edx
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control
        VMMCall Get_Time_Slice_Priority
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
        VMMCall Set_Time_Slice_Priority
        VMMCall Set_Execution_Focus
        xor     eax,eax
        ret

;----------------------------------------------------------------------------

;ServerSetOrigFocusOnVM:
;        mov     ebx,GlideRegVMHandle
;        or      ebx,ebx
;        je      SSFVMnoVM
;        VMMCall Set_Execution_Focus
;        xor     edx,edx
;        xor     esi,esi
;        mov     eax,Set_Device_Focus
;        VMMCall System_Control
;        mov     ebx,GlideCRTCowner
;        mov     edx,VDD_DEVICE_ID
;        xor     esi,esi
;        mov     eax,Set_Device_Focus
;        VMMCall System_Control
;        xor     eax,eax
;        ret

;----------------------------------------------------------------------------
;Ezzel a szolg†ltat†ssal a szerverprocessz a System VM-re †ll°tja a
;billentyñzet-f¢kuszt

ServerSetFocusOnSysVM:
        VMMCall Get_Sys_VM_Handle
        VMMCall Set_Execution_Focus
        mov     KeyboardFocusonVM,0h
        mov     edx,VKD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control

        test    config.Flags,CFG_SETMOUSEFOCUS
        je      SSFoSVM_NoMouseFocus
        mov     MouseFocusonVM,0h
        mov     edx,VMD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control
SSFoSVM_NoMouseFocus:

        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SSFSVMnoVM
        VMMCall Get_Time_Slice_Priority
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
;        and     eax,NOT VMStat_Background
;        or      eax,GlideRegVMBackgroundBit
        VMMCall Set_Time_Slice_Priority
SSFSVMnoVM:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
ServerReleaseFocusOnVM:
        or      GlideRegVMHandle,0h
        je      SSFoSVM_End

;        mov     KeyboardFocusonVM,0h
;        VMMCall Get_Sys_VM_Handle

;        mov     edx,VKD_DEVICE_ID
;        xor     esi,esi
;        mov     eax,Set_Device_Focus
;        VMMCall System_Control

;        mov     ebx,GlideRegVMHandle
;        mov     edx,VTD_DEVICE_ID
;        xor     esi,esi
;        mov     eax,Set_Device_Focus
;        VMMCall System_Control

        test    config.Flags,CFG_SETMOUSEFOCUS
        je      SSFoSVM_NoMouseFocus_
        mov     MouseFocusonVM,0h
        VMMCall Get_Sys_VM_Handle
        mov     edx,VMD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control

        mov     ebx,GlideRegVMHandle
        mov     edx,VTD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control

SSFoSVM_NoMouseFocus_:
SSFoSVM_End:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
;Ezzel a szolg†ltat†ssal a szerverprocessz a System VM-re †ll°t minden f¢kuszt

ServerSetAllFocusOnSysVM:
        VMMCall Get_Sys_VM_Handle
        VMMCall Set_Execution_Focus
        mov     KeyboardFocusonVM,0h
        mov     MouseFocusonVM,0h
        xor     edx,edx
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control
        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SSAFSVMnoVM
        VMMCall Get_Time_Slice_Priority
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
;        and     eax,NOT VMStat_Background
;        or      eax,GlideRegVMBackgroundBit
        VMMCall Set_Time_Slice_Priority
SSAFSVMnoVM:
        xor     eax,eax
        ret

;----------------------------------------------------------------------------
;ezzel a szolg†ltat†ssal Çbreszti fel (feloldja a blokkol†st) a szerverproc
;a DOS-os virtu†lis gÇpet

ServerReturnVM:
        lea     esi,ServerWakeUpVM
        VMMCall Schedule_Global_Event
        call    ServerBlockWT
        ret
ServerWakeUpVM:
        mov     eax,GlideCommArea
        and     DWORD PTR [eax].kernelflag,NOT 1h
        mov     eax,semhandle
        VMMCall Signal_Semaphore; //_No_Switch
        xor     eax,eax
        ret

ServerBlockWT:
        mov     WorkerThreadAsleep,1h
        VMMCall _BlockOnID,<WorkerThread,0>
        or      WorkerThreadAsleep,0h
        jne     ServerBlockWT
        ret

ServerWakeUpWT:
        or      WorkerThreadAsleep,0h           ;Ennek nem szabadna soha
        je      SWUWT_AlreadyAwaken             ;elìfordulnia
        mov     WorkerThreadAsleep,0h
        VMMCall _SignalID,<WorkerThread>
SWUWT_AlreadyAwaken:
        ret

;----------------------------------------------------------------------------
ServerMouseEventInVM:
        mov     esi,[esi].lpvInBuffer
        push    esi
;        lea     edi,mouseinfo
;        cld
;        push    DWORD PTR SIZE MouseInfo
;        pop     ecx
;        rep     movsb

        mov     ebx,GlideRegVMHandle
        mov     edx,VMD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control
        
        pop     esi
        mov     edi,[esi].mi_x
        mov     esi,[esi].mi_y
        xor     al,al
;        VxDCall VMD_Post_Pointer_Message
        VxDCall VMD_Post_Absolute_Pointer_Message

        VMMCall Get_Sys_VM_Handle
        mov     edx,VMD_DEVICE_ID
        xor     esi,esi
        mov     eax,Set_Device_Focus
        VMMCall System_Control

        ret

;----------------------------------------------------------------------------
ServerSuspendVM:
        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SSVM_noServedVM
        mov     eax,Low_Pri_Device_Boost
        mov     ecx,PEF_Wait_For_STI OR PEF_Wait_Not_HW_Int OR PEF_Wait_Crit
        lea     esi,SuspendEvent
        VMMCall Call_Restricted_Event
;        VMMCall Get_Time_Slice_Priority
;        and     eax,NOT (VMStat_Background OR VMStat_High_Pri_Back)
;        VMMCall Set_Time_Slice_Priority        
;        VMMCall Suspend_VM
SSVM_noServedVM:
        ret

SuspendEvent:
        call    ServerSetFocusOnSysVM
        VMMCall Get_Time_Slice_Priority
;        and     eax,NOT (VMStat_Background OR VMStat_High_Pri_Back)
        VMMCall Set_Time_Slice_Priority        
        VMMCall Suspend_VM
        ret

;----------------------------------------------------------------------------
ServerResumeVM:
        mov     ebx,GlideRegVMHandle
        or      ebx,ebx
        je      SRVM_noServedVM
        VMMCall Resume_VM
        VMMCall Get_Time_Slice_Priority
        or      eax,VMStat_Background OR VMStat_High_Pri_Back
        VMMCall Set_Time_Slice_Priority
        call    ServerSetFocusOnVM
SRVM_noServedVM:
        ret

;----------------------------------------------------------------------------
;Ha a DOS-os VM tînkremegy, akkor arr¢l mi is tudni akarunk, Çs
;unregisztr†ljuk

GlideDestroyVM:
        cmp     ebx,GlideRegVMHandle
        jne     GDVMdone

        call    GlideUnregClientByCrash
;        call    VESADestroyVM
;        push    DWORD PTR DGVOODOORESET
;        pop     eax
;        xor     ecx,ecx
;        call    VESACallServer
;        xor     eax,eax
;        mov     GlideRegRefCount,eax
;        mov     GlideRegVMHandle,eax
GDVMdone:
        clc
        ret

;----------------------------------------------------------------------------
GlideUnregClientByCrash:

        call    VESADestroyVM
        push    DWORD PTR DGVOODOOCLIENTCRASHED
        pop     eax
        xor     ecx,ecx
        call    _CallServerProc
GlideUnregClientByCrashZEROonly:
        xor     eax,eax
        mov     GlideRegRefCount,eax
        mov     GlideRegVMHandle,eax
        mov     GlideRegDOSProgVMHandle,eax
        mov     GlideRegDOSStartCount,eax
        ret

;----------------------------------------------------------------------------
;Ha a szerverproc tînkremegy, akkor arr¢l mi is tudni akarunk, Çs
;unregisztr†ljuk

GlideDestroyThread:
        cmp     edi,GlideRegServerThread
        jne     GDVMdone
        call    ServerUnregister
        clc
        ret

;----------------------------------------------------------------------------
;Ha vÇge az alkalmaz†snak a VM-ben, akkor szÅksÇg szerint megjelen°tjÅk a
;konzolt

BeginProc ServerHookEndV86App, HOOK_PROC, OrigEndV86App, LOCKED

        pushad
;        mov ax,0
;        mov gs,ax
;        mov ax,gs:[0]
        call    VESAHookEndApp

        cmp     ebx,GlideRegDOSProgVMHandle
        jne     SHEV_DontCallServer

        dec     GlideRegDOSStartCount
        jne     SHEV_DontCallServer

        cmp     ebx,GlideRegVMHandle
        jne     SHEV_DontCallServerUnreg

        call    GlideUnregClientByCrash
SHEV_DontCallServerUnreg:
        call    GlideUnregClientByCrashZEROonly
        VMMCall Get_Time_Slice_Priority
        and     eax,NOT (VMStat_Background OR VMStat_High_Pri_Back)
        or      eax,GlideRegVMBackgroundBit
        VMMCall Set_Time_Slice_Priority

SHEV_DontCallServer:
        popad
        jmp     [OrigEndV86App]

EndProc ServerHookEndV86App

BeginProc ServerHookBeginV86App, HOOK_PROC, OrigBeginV86App, LOCKED

        pushad

        cmp     ebx,VMToInit
        jne     SHBV_DontInitVM

        mov     VMToInit,0h
        call    VESAPlaceVMStubIntoVM
        jmp     SHBV_CallOrigSHBV

SHBV_DontInitVM:
        cmp     ebx,GlideRegDOSProgVMHandle
        jne     SHBV_CallOrigSHBV
        inc     GlideRegDOSStartCount

SHBV_CallOrigSHBV:
        call    VESAHookBeginApp
        popad
        jmp     [OrigBeginV86App]
        
EndProc ServerHookBeginV86App

BeginProc ServerHookFKI, HOOK_PROC, OrigFKI, LOCKED

        or      GlideRegVMHandle,0h
        je      SHFKI_NoRegisteredVM

        or      MouseFocusOnVM,0h
        je      SHFKI_CheckForGrabbing
        test    DWORD PTR config.Flags,CFG_CTRLALT
        je      SHFKI_CheckForGrabbing

        cmp     cl,56           ;ALT
        jne     SHFKI_CheckForGrabbing
        cmp     PrevScanCode,29 ;CTRL
        jne     SHFKI_CheckForGrabbing

        pushad
        xor     eax,eax
        mov     ebx,GlideRegVMHandle
        mov     ecx,PEF_Wait_Not_HW_Int
        xor     edi,edi
        lea     esi,KbdRestrictedCall
        VMMCall Call_Restricted_Event
        popad
        mov     cl,29+80h
;        jmp     SHFKI_NoRegisteredVM
        
SHFKI_CheckForGrabbing:
        mov     PrevScanCode,cl
        test    DWORD PTR config.Flags,CFG_GRABENABLE
        je      SHFKI_NoRegisteredVM
        or      KeyboardFocusOnVM,0h
        je      SHFKI_NoRegisteredVM

        cmp     cl,80h
        ja      SHFKI_NoRegisteredVM
        cmp     cl,70+128
        je      SHFKI_ScrollLockReleased

        cmp     cl,70
        jne     SHFKI_NoRegisteredVM
        mov     eax,GlideCommArea
        or      [eax].kernelflag,4h
SHFKI_ScrollLockReleased:
        stc
        ret
SHFKI_NoRegisteredVM:
        clc
        ret
        
EndProc ServerHookFKI

KbdRestrictedCall:
        mov     eax,GlideCommArea
        test    DWORD PTR [eax].kernelflag,1h
        je      KbdCallFunc
        or      DWORD PTR [eax].kernelflag,80h
        ret
;        mov     eax,semhandle
;        xor     ecx,ecx
;        VMMCall Wait_Semaphore
KbdCallFunc:
        mov     eax,DGVOODOORELEASEFOCUS
        xor     ecx,ecx
        jmp     _CallServerProc

;----------------------------------------------------------------------------
_GetMem:add     eax,0FFFh
        shr     eax,12
        push    eax
        VMMCall _PageReserve,<PR_SHARED, eax, 0>
        cmp     eax,0FFFFFFFFh
        pop     ecx
        je      _GetMemFailed
        push    eax
        shr     eax,12
        VMMCall _PageCommit,<eax, ecx, PD_NOINIT, 0, PC_USER OR PC_WRITEABLE>
        or      eax,eax
        pop     eax
        je      _GetMemFailed
        clc
        ret
_GetMemFailed:
        stc
        ret

;----------------------------------------------------------------------------
_FreeMem:        
        VMMCall _PageFree,<eax,0>
        ret

;----------------------------------------------------------------------------
;Ez az rutin megh°v egy fÅggvÇnyt a szerverben
;Ez csak akkor h°vhat¢ meg, ha az aktu†lis VM a kiszolg†lt VM

_CallServerProc:
        push    eax
        push    ecx
        mov     edx,GlideCommArea
        mov     ecx,[edx].ExeCodeIndex
        jecxz   _csNoDataToFlush
        mov     [edx].ExeCodeIndex,0h
        mov     [edx].ExeCodes[4*ecx],0FFFFFFFFh
        call    ClientCallServer
        mov     eax,GlideCommArea
        lea     ecx,[eax].FuncData
        mov     [eax].FDPtr,ecx
_csNoDataToFlush:
        pop     ecx
        pop     eax

        mov     [edx].ExeCodes[0],eax
        movzx   ecx,cl
        lea     esi,VESAParams
        cld
        lea     edi,[edx].ExeCodes[1*4]
        rep     movsd
        mov     DWORD PTR [edi],0FFFFFFFFh
        mov     [edx].ExeCodeIndex,0h
        or      [edx].kernelflag,1h
        mov     ebx,GlideRegVMHandle
        call    ClientCallServer
        mov     edx,GlideCommArea
        mov     eax,[edx].ExeCodes[0]
        ret

;----------------------------------------------------------------------------
_GetDOSProgPSP:
        lea     edi,ClientSaveRegs
        VMMCall Save_Client_State

        VMMCall Begin_Nest_V86_Exec
        mov     [ebp].Client_AH,51h
;        mov     eax,21h
;        VMMCall Exec_Int
;        movzx   eax,[ebp].Client_BX

        VMMCall End_Nest_Exec
        
        lea     esi,ClientSaveRegs
        VMMCall Restore_Client_State
        ret

;----------------------------------------------------------------------------
_strcmp:push    ecx
        push    DWORD PTR 20h
        pop     ecx
        mov     dl,1h
_strcmpnext:
        mov     al,[esi]
        mov     ah,[edi]
        inc     esi
        inc     edi
        cmp     al,ah
        jne     _strcmpend
        dec     ecx
        je      _strcmpend
        or      al,al
        jne     _strcmpnext
        xor     dl,dl
_strcmpend:
        shr     dl,1h
        pop     ecx
        ret

VxD_LOCKED_CODE_ENDS

;----------------------------------------------------------------------------

VxD_LOCKED_DATA_SEG

ClientProcessFuncs      DD      DGCLIENT_BEGINGLIDE, OFFSET ClientBeginGlide
                        DD      DGCLIENT_ENDGLIDE, OFFSET ClientEndGlide
                        DD      DGCLIENT_CALLSERVER, OFFSET ClientCallServer
                        DD      DGCLIENT_GETPLATFORM, OFFSET ClientGetPlatformType
                        DD      0FFFFFFFFh

ServerProcessFuncs      DD      DGSERVER_SETFOCUSONVM, OFFSET ServerSetFocusOnVM
                        DD      DGSERVER_REGSERVER, OFFSET ServerRegister
                        DD      DGSERVER_UNREGSERVER, OFFSET ServerUnregister
                        DD      DGSERVER_RETURNVM, OFFSET ServerReturnVM
                        DD      DGSERVER_SETALLFOCUSONSYSVM, OFFSET ServerSetAllFocusOnSysVM
                        DD      DGSERVER_SETFOCUSONSYSVM, OFFSET ServerSetFocusOnSysVM
                        DD      DGSERVER_SETALLFOCUSONVM, OFFSET ServerSetAllFocusOnVM
                        DD      DGSERVER_RELEASEKBDMOUSEFOCUS, OFFSET ServerReleaseFocusOnVM
                        DD      DGSERVER_SETORIGFOCUSONVM, OFFSET ServerSetOrigFocusOnVM
                        DD      DGSERVER_BLOCKWORKERTHREAD, OFFSET ServerBlockWT
                        DD      DGSERVER_WAKEUPWORKERTHREAD, OFFSET ServerWakeUpWT
                        DD      DGSERVER_REGISTERMODE, OFFSET VESARegisterMode
                        DD      DGSERVER_REGISTERACTMODE, OFFSET VESARegisterActMode
                        DD      DGSERVER_MOUSEEVENTINVM, OFFSET ServerMouseEventInVM
                        DD      DGSERVER_SUSPENDVM, OFFSET ServerSuspendVM
                        DD      DGSERVER_RESUMEVM, OFFSET ServerResumeVM
                        DD      DIOC_OPEN, OFFSET InitVXDOpen
                        DD      0FFFFFFFFh

GlideRegVMHandle        DD      0h      ;Kliens VM handle
GlideRegDOSProgVMHandle DD      0h      ;Kliens VM handle mÇgegy pÇld†nyban,
                                        ;hogy figyelni tudjuk a DOS program
                                        ;vÇgetÇrÇsÇt
GlideRegDOSProgPSP      DD      ?
GlideRegDOSStartCount   DD      0h
GlideRegRefCount        DD      0h      ;register refence counter
GlideCommArea           DD      ?       ;Kîzîs puffer c°me
GlideRegServerThread    DD      0h      ;Szerver sz†l handle
GlideCRTCowner          DD      ?
GlideRegVMBackgroundBit DD      0h
VMToInit                DD      0h
semhandle               DD      ?
WorkerThread            DD      ?
areaIdRegistered        DD      ?
OrigFault6Handler       DD      ?
OrigEndV86App           DD      ?
OrigBeginV86App         DD      ?
OrigFKI                 DD      ?
;HotKeyHandle            DD      ?
config                  dgVoodooConfig <>
VesaInitErrPoint        DB      ?
KeyboardFocusonVM       DB      0h
MouseFocusOnVM          DB      0h
PrevScanCode            DB      0h
VXDIsStatic             DB      0h
WorkerThreadAsleep      DB      0h
_ntdllname211           DB      'glide.dll',0h
_ntdllname243           DB      'glide2x.dll',0h
_ntinitname             DB      'InitDosNT',0h
_ntdispatchname         DB      'DispatchDosNT',0h
hotkeyh dd ?

VxD_LOCKED_DATA_ENDS

;----------------------------------------------------------------------------

END
