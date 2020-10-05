;-----------------------------------------------------------------------------
; DLLSTART.ASM - dgVoodoo DOS Glide2x OVL implementation, basic things
;                OVL startup function creating 16 bit code segments
;                in order to make (NTVDM-like) server calls from there.
;                Also, some more extra stuff like flushing communication buffer,
;                handling memory allocation and real mode drivers are included
;                here.
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


        name    dllstart
.386p

INCLUDE ..\..\inc\Dos.inc
INCLUDE	..\..\inc\DosComm.inc
INCLUDE ..\..\inc\RMDriver.inc

;----------------------------------------------------------------------------
;Regiszterstruktúra DPMI-híváshoz
DPMIREGSTRUC    STRUC

_edi            DD      ?
_esi            DD      ?
_ebp            DD      ?
                DD      ?
_ebx            DD      ?
_edx            DD      ?
_ecx            DD      ?
_eax            DD      ?
_flags          DW      ?
_es             DW      ?
_ds             DW      ?
_fs             DW      ?
_gs             DW      ?
_ip             DW      ?
_cs             DW      ?
_sp             DW      ?
_ss             DW      ?
                DW      ?
                DW      ?
                DW      ?

DPMIREGSTRUC    ENDS


PStop           MACRO
        jmp     $
ENDM

;----------------------------------------------------------------------------
;Makrók az NT Third Party DLL mechanizmushoz
;Win9x alatt ezt a kernelmodul valósítja meg

;A három híváshoz makrók: ezek a 32 bites flat kódszegmensbõl hívják
;a 16 bites kódszegmensbeli tényleges hívásokat
;(a DLL-hívások csak 16 bites szegmensbõl mûködnek (NT-spec.))

; call FAR cs:[_RegisterModule]
RegisterModuleCall		MACRO
        db 2Eh, 0FFh, 01Dh
        dd OFFSET _RegisterModule
ENDM


; call FAR [_UnregisterModule]
UnregisterModuleCall    MACRO
        db 0FFh, 01Dh
        dd OFFSET _UnregisterModule
ENDM


; mov     eax,ServerHandle
; call FAR [_DispatchCall]
DispatchCallCall        MACRO
        mov     eax,ServerHandle
        db 0FFh, 01Dh
        dd OFFSET _DispatchCall
ENDM

;----------------------------------------------------------------------------

GetFDPtr        MACRO
        mov     edx,_ca
        mov     edx,[edx].FDPtr
        sub     edx,_ca
        add     edx,_ca_server
ENDM

;----------------------------------------------------------------------------

_TEXT   segment use32 word public 'CODE'
        assume  cs:_TEXT, ds:_TEXT

;A köv. _TEXT16-os részhez gyártunk majd egy 16 bites kódszegmenst
_TEXT16_start   LABEL

_ntdllnamebs    DB      '\'
IF GLIDE1 EQ 1
_ntdllname      DB      'glide.dll',0h
ELSE
_ntdllname      DB      'glide2x.dll',0h
ENDIF
_ntdllinitproc  DB      'InitDosNT',0h
_ntdlldpproc    DB      'DispatchDosNT',0h
_ntdllloc       DB      64+16 DUP(?)
__RegisterModule:
        DB      0BEh
        DW      _ntdllloc - _TEXT16_start       ;mov si,OFFSET _ntdllloc
        DB      0BFh
        DW      _ntdllinitproc - _TEXT16_start  ;mov di,OFFSET _ntdllinitproc
        DB      0BBh
        DW      _ntdlldpproc - _TEXT16_start    ;mov bx,OFFSET _ntdlldpproc
        RegisterModule
        DB      66h, 0CBh                       ;retf
__DispatchCall:
        DispatchCall
        DB      66h, 0CBh                       ;retf
__UnregisterModule:
        UnregisterModule
        DB      66h, 0CBh                       ;retf

_TEXT16_end      LABEL

INCLUDE FakeVesa.mod
INCLUDE MouseDrv.mod

;A Dos-os DLL init függvénye

PUBLIC  GlideDLLStartUp
GlideDLLStartUp:
        pushad
        push    es
        push    ds
;        call    StartServerProc
;        PStop
;        or      _canrun,0h
;        je      StartUpError

        lea     edi,_regs
        mov     ecx,SIZE DPMIREGSTRUC
        xor     al,al
        cld
        rep     stosb
		mov     _canrun,al
        
		call    GetDllName

		call    GlideGetProgName

        pop     ds
        pop     es
        mov     DWORD PTR [esp+7*4],1h
        popad
        ret     ;4


PUBLIC  GlideDLLInit_
GlideDLLInit_:
		push	es
		pushad
		mov     _canrun,0h

		mov		ax,0
		mov		cx,1
        int     31h                             ;új szegmens létrehozása
		movzx	eax,ax
        mov     _RegisterModule[4],eax
        mov     _DispatchCall[4],eax
        mov     _UnregisterModule[4],eax        ;a 3 hívási címhez eltároljuk

        lea     ecx,_TEXT16_start
		movzx	edx,cx
		shr		ecx,10h
		xchg	eax,ebx
		mov		ax,7h
        int     31h                             ;bázis beáll.
		xor		ecx,ecx
        mov     edx,_TEXT16_end - _TEXT16_start
		mov		ax,8h
        int     31h                             ;határ beáll.

        mov     ecx,cs
        lar     ecx,ecx
		mov		cl,ch
		mov		ch,10h
		mov		ax,9h
        int     31h                             ;típus és jogok beáll.

        mov     ax,0Ah
        int     31h
        mov     es,ax                           ;adatszegmens alias létrehoz.

        mov     ax,202h
        mov     bl,06h                          ;6-os kivétel kezelõje címének
        int     31h                             ;lekérdezése
        push    ecx
        push    edx
        mov     DWORD PTR FTPDH_OriginalAddress[0],edx
        mov     DWORD PTR FTPDH_OriginalAddress[4],ecx
        mov     ax,203h
        mov     ecx,cs
        lea     edx,FakeThirdPartyDLLHandler
        int     31h                             ;A hamis beiktatása
        push    ds
        push    es                              ;DS ráállítása a 16 bites
        pop     ds                              ;szegmensre
        RegisterModuleCall
        pop     ds
        mov     ServerHandle,eax
        pop     edx
        pop     ecx

        pushfd
		mov		ax,1
        mov     bx,es
        int     31h                             ;DS alias eldob

        mov     ax,203h
        mov     bl,6h
        int     31h                             ;kiv. kezelõ visszaállítása
        popfd
        jc      InitDontUnregisterDLL			;CF=1: DLL sikertelen betöltése

        mov     ch,DGCLIENT_BEGINGLIDE          ;szólunk a szervernek, hogy
        DispatchCallCall                        ;regisztráljon minket mint
                                                ;klienst
                                                ;EDX = kommunikációs terület
                                                ;ECX = komm. terület a
                                                ;      szerver címterében
                                                ; (VDD-módban EDX=ECX)
                                                ;EAX = osztott memóriaterület
                                                ;      kezdõcíme VDD-módhoz
                                                ;      (NT alatt=0)
                                                ;      (9x/Me alatt=0x80000000)
        jc      InitError						;CF=1  hiba

        mov     _sap,eax                        ;shared address space
        mov     _ca,edx                         ;communication area
        mov     _ca_server,ecx                  ;communication area in server
                                                ;addr space
        lea     eax,[edx].FuncData+FUNCDATASIZE
        mov     _databuffend,eax

        mov     ch,DGCLIENT_GETPLATFORM
        DispatchCallCall
        mov     _platform,eax
        
        GetFDPtr
        mov     eax,_ca
        push    DWORD PTR [eax].FDPtr
        mov     [eax].ExeCodes[0*4],DGVOODOOGETCONFIG
        mov     [eax].ExeCodes[1*4],edx
        mov     [eax].ExeCodeIndex,2h           ;Az szerveren meghívjuk a
        call    FlushExecBuffer_                ;dgVoodooGetConfig függvényt,
                                                ;mert a konfigurációra nekünk
                                                ;is szükségünk lesz
        pop     esi
        push    ds
        pop     es
        lea     edi,_config
        mov     ecx,SIZE dgVoodooConfig
        cld
        rep     movsb

		lea		edi,[eax].progname
		lea		esi,_progname
InitCopyProgName:
		lodsb
		or		al,al
		stosb
		jne		InitCopyProgName

InitOk:
        mov     _canrun,1h
        jmp     InitDontUnregisterDLL
InitError:
        call    GlideDLLUnregister
InitDontUnregisterDLL:
		popad
		pop		es
		ret


;Ez a rutin egy kezelõ a 6-os kivételhez, amit ideiglenesen installálunk
;fel a DPMI-n keresztül. Végrehajtuk a RegisterModule hívást, ami egy
;6-os kivételt generál. NT alatt ez a rutin soha nem hívódhat meg. Win9x/Me
;alatt pedig attól függ, hogy fut-e a szerver: ha igen, akkor az kernel
;szinten lekezeli a kivételt, szimulálva, hogy a DLL-t sikerült betölteni,
;ha nem fut, akkor hívódik meg ez, ami hibával tér vissza.

FakeThirdPartyDLLHandler:
        pushad
        mov     ax,000Ah
        mov     ebx,[esp+16+ 8*4]
        int     31h
        jc      FTPDH_ChainToOriginalHandler
        mov     ecx,[esp+12+ 8*4]
        push    ds
        mov     ds,ax
        cmp     DWORD PTR [ecx],OPCODE_REGISTERMODULE
        pop     ds
        pushfd
        xchg    eax,ebx
        mov     ax,0001h
        int     31h
        popfd
        jne     FTPDH_ChainToOriginalHandler

        mov     WORD PTR [esp+7*4],1h           ;AX = 1: DLL not found
        or      BYTE PTR [esp+20 +8*4],1h       ;CF = 1
        add     DWORD PTR [esp+12 +8*4],4h      ;EIP = EIP+4
        popad
        retf
FTPDH_ChainToOriginalHandler:
        popad
        DB      0EAh
FTPDH_OriginalAddress:
        DD      ?,?

PUBLIC  GlideDLLCleanUp_
GlideDLLCleanUp_:
        mov     ch,DGCLIENT_ENDGLIDE
        DispatchCallCall
GlideDLLUnregister:
        mov     eax,ServerHandle
        UnregisterModuleCall
        mov     ebx,_RegisterModule[4]
        mov     ax,1h
        int     31h
        ret

;Ez a függvény üríti ki a parancs- és adatpuffert a kommunikációs területen:
;meghívja a szervert. Amíg a szerver a megadott függvényeken dolgozik, két
;eset lehetséges: Win9x/Me alatt a hívó szál blokkolva van, WinXP alatt nincs.
;A CALL_SERVER szolgáltatás az állapotot az EAX regiszterben adja vissza
;(EAX=1: a szál blokkolva volt, és a szerver végzett,
; EAX=0: szerver elkezdett dolgozni, de a szál futása folytatódik)
;Ha a szál nincs blokkolva, akkor meg kell várnia, amíg a szerver elvégzi
;a feldolgozást: ehhez a kernelflag mezõ megfelelõ bitjét monitorozza,
;szükség esetén lemondva az adott idõszeletérõl. (Ezzel egy szemaforon
;való várakozást valósít meg.)
;Erre azért van szükség, mert XP alatt az emulált megszakítások csak
;akkor hajtódnak végre, amikor a VDM ténylegesen DOS-os kódot hajt végre,
;azaz egy VDD-bõl nem szabad blokkolni. Win9x/Me alatt a VM event-ekként
;beütemezett megszakítások a VM blokkolt állapotában is végrehajthatóak,
;ott nincs ilyen probléma.

PUBLIC  FlushExecBuffer_
FlushExecBuffer_:
        push    eax
        push    ecx
        mov     eax,_ca
        mov     ecx,[eax].ExeCodeIndex
        mov     [eax].ExeCodeIndex,0h
        mov     [eax].ExeCodes[4*ecx],0FFFFFFFFh
        mov     ch,DGCLIENT_CALLSERVER
        DispatchCallCall
        or      eax,eax
        jne     _execbflushed
_waitflush:
        mov     eax,_ca
        test    [eax].kernelflag,KFLAG_SERVERWORKING
        je      _execbflushed_
        mov     ch,DGCLIENT_RELEASETIMESLICE
        DispatchCallCall
        nop
		nop
        jmp     _waitflush
_execbflushed_:
        mov     ch,DGCLIENT_RELEASEMUTEX
        DispatchCallCall
_execbflushed:
        mov     eax,_ca
        lea     ecx,[eax].FuncData
        mov     [eax].FDPtr,ecx
        mov     _exebuff_free,EXEBUFFSIZE
        pop     ecx
        pop     eax
        ret


;Ez a fgv. egy adott méretû területet a következõ 4-gyel osztható címre másol
;a kommunikációs területen belül az adatterületre
PUBLIC  _CopyData@8
PUBLIC  CopyData_
CopyData_:
_CopyData@8:
        push    esi
        mov     esi,_ca
        push    edi
        push    ecx
        mov     ecx,[esp+8 + 3*4]
        mov     edi,[esi].FDPtr
        lea     eax,[edi+ecx+3]
        and     al, NOT 3h
        mov     [esi].FDPtr,eax
        mov     esi,[esp+4 + 3*4]
        mov     eax,ecx
        shr     ecx,2h
        cld
        rep     movsd
        mov     ecx,eax
        and     ecx,3h
        rep     movsb
        pop     ecx
        pop     edi
        pop     esi
        ret     8


PUBLIC  _CopyMem@12
PUBLIC  _CopyMem
_CopyMem:
_CopyMem@12:
        push    esi
        push    edi
        mov     edi,[esp+4 +2*4]
        mov     esi,[esp+8 +2*4]
        mov     ecx,[esp+12 +2*4]
        mov     eax,ecx
        shr     ecx,2h
        cld
        rep     movsd
        and     eax,3h
        mov     ecx,eax
        rep     movsb
        pop     edi
        pop     esi
        ret     12


PUBLIC	_IsRealModeDriverPresent@4
PUBLIC	_IsRealModeDriverPresent
_IsRealModeDriverPresent@4:
_IsRealModeDriverPresent:
		mov		eax,[esp+4 +0*4]
		push	edx
		mov		eax,[4*eax]
		movzx	edx,ax
		shr		eax,16-4
		and		al,0F0h
		mov		dx,[eax+edx-2]
		cmp		DWORD PTR [eax+edx].driverId, _DRIVERID
		sete	al
		pop		edx
		movzx	eax,al
		ret	4


;Ez a fgv. a DOS-os program nevét másolja be a komm. területre
;Csak init közben használatos
GlideGetProgName:
        mov     ah,62h
        int     21h
        push    es
        mov     es,bx
        mov     es,WORD PTR es:[2Ch]
        mov     ecx,es
        xor     edi,edi
        lsl     ecx,ecx
        inc     ecx
        xor     eax,eax
		lea		edx,_progname
        mov     [edx][0],al
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
        push    ds
        push    ds es
        pop     ds es
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
        mov     al,[esi]
		cmp		BYTE PTR [esi+1],':'
		je		DriveFound
		mov     ah,19h
        int     21h
        add     al,'A'
DriveFound:
        sub     ecx,edi
        mov     esi,edi
		mov		edi,edx
		stosb
        rep     movsb
        stosb
        xor     al,al
        stosb
        pop     ds
CantFindName:
        pop     es
        ret


;ESI: forr s
;EBX: c‚lstring
_strcat:push    esi ebx
_strcat_seend:
        or      BYTE PTR [ebx],0h
        lea     ebx,[ebx+1]
        jne     _strcat_seend
        dec     ebx
_strcat_copy:
        mov     al,[esi]
        mov     [ebx],al
        inc     esi
        inc     ebx
        or      al,al
        jne     _strcat_copy
        pop     ebx esi
        ret


GetDllName:
        mov     BYTE PTR _ntdllloc[0],0h
        lea     edi,_regs
        xor     ecx,ecx
        mov     [edi]._edx,ecx
        mov     ax,100h
        mov     bx,128/16
        xor     edx,edx
        int     31h
        push    edx
        jc      _gdllname_notfound
        mov     [edi]._ds,ax
        shl     eax,4h
        push    eax
        xchg    eax,ebx
        mov     ah,19h
        int     21h
        add     al,'A'
        mov     [ebx][0],al
        mov     DWORD PTR [ebx][1],'\:'

        mov     [edi]._esi,3h
        mov     [edi]._eax,4700h
        mov     ax,300h
        push    LARGE 21h
        pop     ebx
        int     31h
        test    [edi]._flags,1h
        pop     ebx
        jne     _gdllname_notfound

        lea     esi,_ntdllnamebs
        call    _strcat
        push    ebx
        mov     [edi]._eax,3D00h
        mov     ax,300h
        push    LARGE 21h
        pop     ebx
        int     31h
        test    [edi]._flags,1h
        pop     esi
        jne     _gdllname_notfound
        mov     ebx,[edi]._eax
        mov     ah,3Eh
        int     21h
        jmp     _gdllname_found
_gdllname_notfound:
        lea     esi,_ntdllname
_gdllname_found:
        lea     ebx,_ntdllloc
        call    _strcat
        pop     edx
        or      edx,edx
        je      _gdllname_exit
        mov     ax,101h
        int     31h
_gdllname_exit:
        ret

COMMENT \

StartServerProc:
        lea     edi,_regs
        mov     ecx,SIZE DPMIREGSTRUC
        xor     al,al
        cld
        rep     stosb
        mov     _canrun,al
        mov     ax,100h
        mov     bx,512/16
        int     31h
        push    edx
        movzx   edx,ax
        movzx   ebx,ax
        shl     edx,4h
        mov     edi,edx
        mov     ecx,64
        lea     esi,_servername
        rep     movsb

        lea     edi,[edx+80h]
        xor     al,al
        mov     ecx,7*2
        rep     stosb
        mov     WORD PTR [edx+82h],100h
        mov     [edx+84h],bx
        mov     _regs._ds,bx
        mov     _regs._es,bx

        xor     ecx,ecx
        lea     ebx,_scheck
        lea     esi,_argument
SSP_nextdigit:
        xor     eax,eax
        shld    eax,ebx,4h
        shl     ebx,4h
        add     al,'0'
        mov     [esi+ecx+13],al
        inc     cl
        cmp     cl,8h
        jne     SSP_nextdigit
        lea     edi,[edx+100h]
        mov     ecx,32
        rep     movsb

        mov     ax,300h
        mov     bl,21h
        xor     ecx,ecx
        lea     edi,_regs
        mov     _regs._eax,4B00h
        mov     _regs._edx,0h
        mov     _regs._ebx,80h
        int     31h        
        pop     edx
        mov     ax,101h
        int     31h
        test    _regs._flags,1h         ;test cf
        jne     SSP_error

SSP_waitserver:
        or      _scheck,0h
        je      SSP_waitserver
        cmp     DWORD PTR _scheck, OFFSET _scheck
        jne     SSP_error
        mov     _canrun,1h

SSP_error:
        mov     _canrun,1h
        ret

        \

;Ez a fgv. egy filen‚v teljes el‚r‚si £tj t adja vissza
;(a text£r t f jlb¢l bet”lt“ rutinok haszn lj k)
PUBLIC  _GetCanonicFileName@4
_GetCanonicFileName@4:
        pushad

        mov     ax,100h
        mov     bx,1024/16
        int     31h
        mov     esi,[esp+4+ 8*4]
        push    edx
        mov     _regs._ds,ax
        mov     _regs._es,ax
        movzx   edi,ax
        shl     edi,4h
        xor     ecx,ecx
        mov     eax,esi
GCFN_countlen:
        cmp     BYTE PTR [eax],0h
        lea     ecx,[ecx+1]
        lea     eax,[eax+1]
        jne     GCFN_countlen
        cld
        push    edi
        rep     movsb
        lea     edi,_regs
        mov     [edi]._eax,6000h        
        mov     [edi]._esi,0h
        mov     [edi]._edi,512
        mov     ax,300h
        mov     bl,21h
        xor     ecx,ecx
        int     31h
        pop     edi
        pop     edx
        mov     ax,101h
        int     31h
        xor     eax,eax
        test    _regs._flags,1h
        jne     GCFN_error
        lea     esi,[edi+512]
        lea     edi,_filename
        mov     ecx,256
        rep     movsb
        inc     eax
GCFN_error:
        popad
        ret     4


PUBLIC  _InstallFakeVesaDrv@0
_InstallFakeVesaDrv@0:
        pushad
        lea     esi,VESADRV_START
        call    _LoadRealModeModule
        mov     [esp+7*4],eax
        popad
        ret


PUBLIC  _UnInstallFakeVesaDrv@4
_UnInstallFakeVesaDrv@4:
        pushad
        mov     eax,[esp+4 +8*4]
        call    _ReleaseRealModeModule
        popad
        ret     4


PUBLIC  _InstallMouseDrv@8
_InstallMouseDrv@8:
        pushad
        mov     eax,[esp+4 +8*4]
        mov     _regs._ecx,eax
        mov     eax,[esp+8 +8*4]
        mov     _regs._edx,eax
        lea     esi,MOUSEDRV_START
        call    _LoadRealModeModule
        mov     [esp+7*4],eax
        popad
        ret     8


PUBLIC  _UnInstallMouseDrv@4
_UnInstallMouseDrv@4:
        pushad
        mov     eax,[esp+4 +8*4]
        call    _ReleaseRealModeModule
        popad
        ret     4


_LoadRealModeModule:
        pushad
        mov     DWORD PTR [esp+7*4],0h
        movzx   ebx,[esi].dllsize
        movzx   eax,[esi].dllextramemory
        add     ebx,eax
        add     ebx,0Fh
        shr     ebx,4h
        mov     ax,100h
        int     31h
        jc      _LRMD_error
        movzx   edi,ax
        push    edi
        shl     edi,4h
        push    edi
        movzx   ecx,[esi].dllsize
        cld
        rep     movsb        
        pop     edi
        mov     [edi].dllselector,dx

        movzx   eax,WORD PTR [edi].entrypoint
        pop     edx
        mov     [esp+7*4],edi
        sub     edx,10h
        lea     edi,_regs
        mov     [edi]._cs,dx
        mov     [edi]._ip,ax
        mov     [edi]._ebp,0h
        mov     ax,301h
        xor     bh,bh
        xor     ecx,ecx
        int     31h
_LRMD_error:
        popad
        ret


_ReleaseRealModeModule:
        pushad
        xchg    eax,edx
        mov     ecx,edx
        jecxz   _RRMD_end
        shr     ecx,4h
        sub     ecx,10h
        lea     edi,_regs
        movzx   eax,WORD PTR [edx].entrypoint
        mov     [edi]._cs,cx
        mov     [edi]._ip,ax
        mov     [edi]._ebp,1h
        mov     ax,301h
        xor     bh,bh
        xor     ecx,ecx
        int     31h
        mov     ax,101h
        movzx   edx,WORD PTR [edx].dllselector
        int     31h
_RRMD_end:
        popad
        ret

        
PUBLIC  _DPMIGetMem@4
_DPMIGetMem@4:
        pushad
        mov     ecx,[esp+4 +8*4]
        mov     ebx,ecx
        shr     ebx,10h
        mov     ax,501h
        int     31h
        mov     DWORD PTR [esp+7*4],0h
        jc      _DPMIGMEnd

        shl     ebx,10h
        mov     bx,cx
        mov     [esp+7*4],ebx
_DPMIGMEnd:
        popad
        ret     4


PUBLIC  _DPMIFreeMem@4
_DPMIFreeMem@4:
        pushad
        mov     esi,[esp+4 +8*4]
        mov     edi,esi
        shr     esi,10h
        mov     ax,502h
        int     31h
        popad
        ret     4


PUBLIC  _platform
PUBLIC  _ca
PUBLIC  _ca_server
PUBLIC  _sap
PUBLIC  _databuffend
PUBLIC  _canrun
PUBLIC  _filename
PUBLIC  _config
PUBLIC  _exebuff_free

;a három hívás szelektor:offset címe
_RegisterModule		DD	__RegisterModule - _TEXT16_start
					DD	0
_UnregisterModule	DD  __UnregisterModule - _TEXT16_start
					DD	0
_DispatchCall		DD  __DispatchCall - _TEXT16_start
					DD	0

_platform       DD      ?
ServerHandle    DD      ?
_ca             DD      ?               ;kommunikációs terület címe
_ca_server      DD      ?
_databuffend    DD      ?
_sap            DD      ?
_exebuff_free   DD      ?
_config         dgVoodooModeInfo <>
_regs           DPMIREGSTRUC <>
_scheck         DD      0h
_canrun         DB      0h
_filename       DB      256 DUP(?)
_progname		DB		128 DUP(?)

_TEXT   ends

end
