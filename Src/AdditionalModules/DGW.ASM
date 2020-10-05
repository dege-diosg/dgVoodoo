;-----------------------------------------------------------------------------
; DGW.ASM - Some ported functions from my old DOS-Extender (DGPM)
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

;A kiterjesztìbìl (DGPM) Windows32 al† port†lt fÅggvÇnyek:
;       - Mem¢riakezelÇs
;(egyelìre ennyi)
;----------------------------------------------------------------------------

INCLUDE MACROS.INC

DGWTYPE         EQU     STANDALONE_OMF

;Mem¢ri†t kezelì utility: az egyes heap-ekbìl allok†l mem¢ri†t, a heapeknek
;csak azon rÇszeihez rendelÅnk fizikai t†rol¢t, amelyek tÇnylegesen foglaltak
;----------------------------------------------------------------------------

HeapMaxSize     EQU     8192            ;Egy Heap max. mÇrete
HeapNum         EQU     8               ;Heapek max. sz†ma

ALIGNMENT       EQU     16              ;Mem¢riablokkok igaz°t†sa 16 byte-ra

;----------------------------------------------------------------------------
;A heapek blokkjait le°r¢ strukt£ra

dgwBlockHeader  STRUC

dgwState        DD      ?     ;DGxy †llapot: foglalt - szabad, utols¢ - nem az
dgwLength       DD      ?               ;blokk mÇrete
dgwBlockID      DD      ?               ;nem egyedi blokkazonos°t¢
dgwBackLink     DD      ?               ;pointer a megelìzì blokkra

dgwBlockHeader  ENDS

IF      (DGWTYPE  EQ  STANDALONE_OMF) OR (DGWTYPE  EQ  NEEDEDONLY_OMF)

.386
.model  FLAT

UNICODE EQU 0
INCLUDE W32MAIN.INC

ENDIF

.code

;----------------------------------------------------------------------------
;Mem¢ria foglal†sa
;Be:    EAX     -       mem¢ria b†jtokban
;Ki:    Carry=1 -       Hiba
;       EAX     -       terÅlet kezdìc°me

PUBLIC  _GetMem
_GetMem:pushad
        mov     ecx,dgwSysInfoAVL
        jecxz   dgwGMSysInfoOK

        push    eax
        push    LARGE OFFSET dgwSysInfo
        W32     GetSystemInfo,1                ;Sysinfo lekÇrdez
        pop     eax

dgwGMSysInfoOK:
        xor     ecx,ecx
        add     eax,SIZE dgwBlockHeader+ALIGNMENT-1
        jc      memHeapNotAllocated
        mov     dgwSysInfoAVL,ecx
        and     al,NOT (ALIGNMENT-1)
dgwLookAtNextHeap:
        cmp     ecx,HeapNum
        je      memHeapNotAllocated
        lea     ebx,[HeapHandles+4*ecx]
        or      DWORD PTR [ebx],0h
        jne     memHeapAllocated                ;Heap m†r allok†lva

        push    eax ebx ecx
        mov     ebp,HeapMaxSize*1024
        cmp     eax,ebp
        jbe     dgwHeapMaxSizeOK
        mov     ebp,eax
dgwHeapMaxSizeOK:
        push    LARGE PAGE_READWRITE
        push    LARGE MEM_RESERVE
        push    ebp ;LARGE HeapMaxSize*1024
        push    LARGE NULL
        W32     VirtualAlloc,4                  ;£j heap allok†l†sa
        pop     ecx ebx
        or      [ebx],eax
        pop     eax
        je      memHeapNotAllocated
        push    DWORD PTR [ebx]
        push    eax ebx ecx

        push    LARGE PAGE_READWRITE
        push    LARGE MEM_COMMIT
        push    LARGE dgwSysInfo.dwPageSize
        push    DWORD PTR [ebx]
        W32     VirtualAlloc,4                  ;Elsì lap COMITTED
        pop     ecx ebx eax edx
        cmp     eax,NULL
        jne     memHeapSetFirstMCB

;----- ha nem b°r az elsì laphoz sem fizikai t†rol¢t rendelni, akkor a
;      c°mterÅletet eldobjuk, Çs hiba

        push    LARGE MEM_RELEASE
        push    LARGE 0h
        push    DWORD PTR edx
        W32     VirtualFree,3                   ;c°mterÅlet eldob†sa
        jmp     memHeapNotAllocated

memHeapSetFirstMCB:
        mov     [edx.dgwState],'DGLF'
        mov     [edx.dgwLength],ebp ;(HeapMaxSize*1024)-SIZE dgwBlockHeader
        mov     [edx.dgwBackLink],0h
                                                ;Elsì MCB kitîltÇse
memHeapAllocated:
        inc     ecx
        mov     ebx,[ebx]
        xor     ebp,ebp
_CheckNextHiBlock_:
        mov     edx,[ebx.dgwState]
        cmp     dl,'F'
        jne     SHORT HiMBlockGot
        cmp     eax,[ebx.dgwLength]
        jbe     SHORT _ResHiMem_
HiMBlockGot:
        cmp     dh,'L'
        je      dgwLookAtNextHeap
        mov     ebp,ebx
        add     ebx,[ebx.dgwLength]
        jmp     SHORT _CheckNextHiBlock_
_ResHiMem_:
        xor     edi,edi
        cmp     dh,'L'
        je      dgwGMNoNextBlock
        mov     edi,ebx
        add     edi,[ebx].dgwLength
dgwGMNoNextBlock:
        mov     ecx,[ebx.dgwLength]
        push    edx
        sub     ecx,eax
        cmp     ecx, SIZE dgwBlockHeader
        ja      dgwGMCreateNewBlock
        add     eax,ecx
        xor     ecx,ecx
dgwGMCreateNewBlock:
        mov     [ebx.dgwBackLink],ebp
        mov     [ebx.dgwLength],eax

        pushad
        lea     eax,[eax+SIZE dgwBlockHeader]   ;kîv. MCB is COMITTED lapon
                                                ;legyen
        push    LARGE PAGE_READWRITE
        push    LARGE MEM_COMMIT
        push    eax
        push    ebx
        W32     VirtualAlloc,4
        popad

        mov     ebp,BlockMarker
        mov     [ebx.dgwBlockID],ebp
        cmp     dh,'N'
        mov     dx,'NR'
        je      _ItsLastHiBlock_
        or      ecx,ecx
        jne     SHORT _ItsLastHiBlock_
        mov     dh,'L'
_ItsLastHiBlock_:
        mov     [ebx.dgwState],edx
        pop     edx
        jecxz   SHORT _HiMemGet_
        mov     [ebx+eax.dgwLength],ecx
        mov     [ebx+eax.dgwState],edx
        mov     [ebx+eax.dgwBackLink],ebx
        or      edi,edi
        je      _HiMemGet_
        lea     edx,[eax+ebx]
        mov     [edi].dgwBackLink,edx
_HiMemGet_:
        lea     eax,[ebx+ SIZE dgwBlockHeader]
        mov     [esp+4*7],eax
_HiBlockFree:
	clc
	db 0B0h
memHeapNotAllocated:
	stc
	popad
_RET32: ret
PUBLIC  _RET32

;----------------------------------------------------------------------------
;Mem¢ria felszabad°t†sa
;Be:    EAX     -       terÅlet kezdìc°me
;Ki:    Carry=1 -       Hiba
        
PUBLIC  _FreeMem
_FreeMem:
        pushad
        lea     ebx,[eax-SIZE dgwBlockHeader]
	xor     edi,edi
        mov     eax,[ebx.dgwLength]
        mov     edx,[ebx.dgwState]
	cmp     dl,'F'
        je      _HiBlockFree
	cmp     dh,'L'
	je      _NoNextHiBlock
        lea     esi,[ebx+eax]
        mov     edi,esi  
        mov     ecx,[esi.dgwState]
	cmp     cl,'F'
	jne     _NoNextHiBlock
	mov     edx,ecx
        add     edi,[esi.dgwLength]
	cmp     ch,'L'
	jne     _NoLastFreeHiBlock
	xor     edi,edi
_NoLastFreeHiBlock:
        add     eax,[esi.dgwLength]
_NoNextHiBlock:
        mov     ecx,[ebx.dgwBackLink]
	jecxz   _NoPrevHiBlock
        cmp     BYTE PTR [ecx.dgwState],'F'
	jne     _NoPrevHiBlock
        add     eax,[ecx.dgwLength]
	mov     ebx,ecx
_NoPrevHiBlock:
        mov     [ebx.dgwLength],eax
	mov     dl,'F'
        mov     [ebx.dgwState],edx

        lea     edx,[ebx+eax]
        mov     eax,dgwSysInfo.dwPageSize
        dec     eax
        lea     ebp,[ebx+SIZE dgwBlockHeader]
        add     ebp,eax
        not     eax
        and     ebp,eax
        and     edx,eax
        sub     edx,ebp
        jbe     dgwFMnoPagesToDeCommit
        push    edi ebx
        push    LARGE MEM_DECOMMIT
        push    edx
        push    ebp
        W32     VirtualFree,3
        pop     ebx edi
dgwFMnoPagesToDeCommit:
	or      edi,edi
        je      _HiBlockFree
        mov     [edi.dgwBackLink],ebx
	clc
	db 0B0h
	stc
	popad
        ret

;----------------------------------------------------------------------------
;EAX    -       azonos°t¢

PUBLIC  _FreeMemWithMarker
_FreeMemWithMarker:
        pushad
        push    LARGE HeapNum
        pop     ecx
dgwLookAtAllHeaps:
        mov     ebx,[HeapHandles-4][4*ecx]
        or      ebx,ebx
        je      dgwEmptyHeap
dgwCheckNextBlock:
        mov     edx,[ebx]
        cmp     dl,'R'
        jne     dgwSetToNextHiBlock
        cmp     eax,[ebx.dgwBlockID]
        jne     dgwSetToNextHiBlock
        push    eax
        lea     eax,[ebx+SIZE dgwBlockHeader]
        call    _FreeMem
        pop     eax
        jmp     dgwLookAtAllHeaps
dgwSetToNextHiBlock:
        add     ebx,[ebx.dgwLength]
        cmp     dh,'L'
        jne     dgwCheckNextBlock
dgwEmptyHeap:
        loopd   dgwLookAtAllHeaps
        popad
        clc
        ret

;----------------------------------------------------------------------------
;Blokkazonos°t¢ be†ll°t†sa
;Be:    EAX     -       £j blokkazonos°t¢

PUBLIC  _SetBlockMarker
_SetBlockMarker:
        mov     BlockMarker,eax
        ret
;----------------------------------------------------------------------------
;Blokkazonos°t¢ be†ll°t†sa egy adott blokkban
;Be:    EAX     -       blokk c°me
;       EBX     -       £j azonos°t¢
;Ki:    Carry=0 -       OK
;            =1 -       ÇrvÇnytelen c°m

PUBLIC  _SetBlockID
_SetBlockID:
        cmp     WORD PTR [eax.dgwState+2-SIZE dgwBlockHeader],'DG'
        stc
        jne     memSBIDinvalid
        mov     [eax.dgwBlockID-SIZE dgwBlockHeader],ebx
        clc
memSBIDinvalid:
        ret

;----------------------------------------------------------------------------
;Az îsszes lefoglalt mem¢ria felszabad°t†sa
;Be:    -
;Ki:    -

PUBLIC  _FreeAllMem
_FreeAllMem:
        pushad
        push    LARGE HeapNum
        pop     ecx
        lea     eax,HeapHandles
memDestroyHeaps:
        or      DWORD PTR [eax],0h
        je      memNoMoreHeaps
        push    eax
        push    LARGE MEM_DECOMMIT
        push    LARGE HeapMaxSize*1024
        push    DWORD PTR [eax]
        W32     VirtualFree,3                   ;Az îsszes lap decomitt†l†sa
        pop     eax
        push    eax
        push    LARGE MEM_RELEASE
        push    LARGE 0h
        push    DWORD PTR [eax]
        W32     VirtualFree,3                   ;c°mterÅlet eldob†sa
        pop     eax
        lea     eax,[eax+4]
        loopd   memDestroyHeaps
memNoMoreHeaps:
        popad
        ret

;----------------------------------------------------------------------------
;KilÇpÇs a programb¢l
;Be:    EAX     -       KilÇpÇsi k¢d

IF      (DGWTYPE  EQ  ASSEMBLY_INCLUDE)
_ExitProtProgram:
        push    eax
        call    _FreeAllMem
        W32     ExitProcess,1
ENDIF

;----------------------------------------------------------------------------
;C-interfÇsz a fÅggvÇnyekhez

IF      (DGWTYPE  EQ  STANDALONE_OMF) OR (DGWTYPE  EQ  NEEDEDONLY_OMF)

;C-szintaxis:   int _GetMem(dword bytes);
PUBLIC  __GetMem@4
PUBLIC  __GetMem
__GetMem@4:
__GetMem:
        mov     eax,[esp+4]
        call    _GetMem
        jnc     dgwCIok1
        mov     eax,NULL
dgwCIok1:
        ret     4

;C-szintaxis:   int _FreeMem(void *block);
PUBLIC  __FreeMem@4
PUBLIC  __FreeMem
__FreeMem@4:
__FreeMem:
        mov     eax,[esp+4]
        call    _FreeMem
        mov     eax,TRUE
        jnc     dgwCIok2
        mov     eax,FALSE
dgwCIok2:
        ret     4

;C-szintaxis:   int _FreeMemWithMarker(dword id);
PUBLIC  __FreeMemWithMarker@4
PUBLIC  __FreeMemWithMarker
__FreeMemWithMarker@4:
__FreeMemWithMarker:
        mov     eax,[esp+4]
        call    _FreeMemWithMarker
        mov     eax,TRUE
        jnc     dgwCIok3
        mov     eax,FALSE
dgwCIok3:
        ret     4

;C-szintaxis:   void _SetBlockMarker(dword id);
PUBLIC  __SetBlockMarker@4
PUBLIC  __SetBlockMarker
__SetBlockMarker@4:
__SetBlockMarker:
        mov     eax,[esp+4]
        call    _SetBlockMarker
        ret     4

;C-szintaxis:   int _SetBlockID(void *block, dword id);
PUBLIC  __SetBlockID@8
PUBLIC  __SetBlockID
__SetBlockID@8:
__SetBlockID:
        mov     eax,[esp+4]
        mov     ebx,[esp+8]
        call    _SetBlockID
        mov     eax,TRUE
        jnc     dgwCIok4
        mov     eax,FALSE
dgwCIok4:
        ret     8

;C-szintaxis:   void _FreeAllMem();
PUBLIC  __FreeAllMem@0
PUBLIC  __FreeAllMem
__FreeAllMem@0:
__FreeAllMem:
        jmp     _FreeAllMem               

ENDIF

.data

BlockMarker     DD      0h
HeapHandles     DD      HeapNum DUP( NULL )
dgwSysInfoAVL   DD      -1h

.data?

dgwSysInfo      SYSTEM_INFO     <>

IF      (DGWTYPE  EQ  STANDALONE_OMF) OR (DGWTYPE  EQ  NEEDEDONLY_OMF)

END

ENDIF
