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

;Ported functions to Win32 from my good old DOS-extender, DGPM:
;       - Memory management
;(that's all)
;----------------------------------------------------------------------------

INCLUDE MACROS.INC

;Memory management: allocating memory from heaps, committing physical memory
;only to reserved memory blocks in the address range of heaps
;----------------------------------------------------------------------------

HeapMaxSize     EQU     8192            ;Max heap size
HeapNum         EQU     8               ;Max num of heaps

ALIGNMENT       EQU     16              ;Aligning memory blocks to 16 byte

;----------------------------------------------------------------------------
;A heapek blokkjait leÌrÛ strukt˙ra

dgwBlockHeader  STRUC

dgwState        DD      ?				;DGxy †llapot: foglalt - szabad, utols¢ - nem az
dgwLength       DD      ?               ;blokk mÇrete
dgwBlockID      DD      ?               ;nem egyedi blokkazonos°t¢
dgwBackLink     DD      ?               ;pointer a megelìzì blokkra

dgwBlockHeader  ENDS

.386
.model  FLAT

;UNICODE EQU 0
INCLUDE WIN.INC

.code

;----------------------------------------------------------------------------
;Allocating memory
;In:     EAX     -       block size in bytes
;Out:    Carry=1 -       Error
;        EAX     -       address of allocated block

PUBLIC  _GetMem
_GetMem:pushad
        mov     ecx,dgwSysInfoAVL
        jecxz   dgwGMSysInfoOK

        push    eax
        push    LARGE OFFSET dgwSysInfo
        W32     GetSystemInfo,1                ;Sysinfo lekÈrdez
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
        jne     memHeapAllocated                ;Heap m·r allok·lva

		push    eax
		push	ebx
		push	ecx
        mov     ebp,HeapMaxSize*1024
        cmp     eax,ebp
        jbe     dgwHeapMaxSizeOK
        mov     ebp,eax
dgwHeapMaxSizeOK:
        push    LARGE PAGE_READWRITE
        push    LARGE MEM_RESERVE
        push    ebp ;LARGE HeapMaxSize*1024
        push    LARGE NULL
        W32     VirtualAlloc,4                  ;⁄j heap allok·l·sa
        pop     ecx
        pop		ebx
        or      [ebx],eax
        pop     eax
        je      memHeapNotAllocated
        push    DWORD PTR [ebx]
        push    eax
        push	ebx
        push	ecx

        push    LARGE PAGE_READWRITE
        push    LARGE MEM_COMMIT
        push    LARGE dgwSysInfo.dwPageSize
        push    DWORD PTR [ebx]
        W32     VirtualAlloc,4                  ;Elsı lap COMITTED
        pop     ecx
        pop		ebx
        pop		eax
        pop		edx
        cmp     eax,NULL
        jne     memHeapSetFirstMCB

;----- ha nem bÌr az elsı laphoz sem fizikai t·rolÛt rendelni, akkor a
;      cÌmter¸letet eldobjuk, Ès hiba

        push    LARGE MEM_RELEASE
        push    LARGE 0h
        push    DWORD PTR edx
        W32     VirtualFree,3                   ;cÌmter¸let eldob·sa
        jmp     memHeapNotAllocated

memHeapSetFirstMCB:
        mov     (dgwBlockHeader PTR [edx]).dgwState,'DGLF'
        mov     (dgwBlockHeader PTR [edx]).dgwLength,ebp ;(HeapMaxSize*1024)-SIZE dgwBlockHeader
        mov     (dgwBlockHeader PTR [edx]).dgwBlockHeader.dgwBackLink,0h
                                                ;Elsı MCB kitˆltÈse
memHeapAllocated:
        inc     ecx
        mov     ebx,[ebx]
        xor     ebp,ebp
_CheckNextHiBlock_:
        mov     edx,(dgwBlockHeader PTR [ebx]).dgwState
        cmp     dl,'F'
        jne     SHORT HiMBlockGot
        cmp     eax,(dgwBlockHeader PTR [ebx]).dgwLength
        jbe     SHORT _ResHiMem_
HiMBlockGot:
        cmp     dh,'L'
        je      dgwLookAtNextHeap
        mov     ebp,ebx
        add     ebx,(dgwBlockHeader PTR [ebx]).dgwLength
        jmp     SHORT _CheckNextHiBlock_
_ResHiMem_:
        xor     edi,edi
        cmp     dh,'L'
        je      dgwGMNoNextBlock
        mov     edi,ebx
        add     edi,(dgwBlockHeader PTR [ebx]).dgwLength
dgwGMNoNextBlock:
        mov     ecx,(dgwBlockHeader PTR [ebx]).dgwLength
        push    edx
        sub     ecx,eax
        cmp     ecx, SIZE dgwBlockHeader
        ja      dgwGMCreateNewBlock
        add     eax,ecx
        xor     ecx,ecx
dgwGMCreateNewBlock:
        mov     (dgwBlockHeader PTR [ebx]).dgwBackLink,ebp
        mov     (dgwBlockHeader PTR [ebx]).dgwLength,eax

        pushad
        lea     eax,[eax+SIZE dgwBlockHeader]   ;kˆv. MCB is COMITTED lapon
                                                ;legyen
        push    LARGE PAGE_READWRITE
        push    LARGE MEM_COMMIT
        push    eax
        push    ebx
        W32     VirtualAlloc,4
        popad

        mov     ebp,BlockMarker
        mov     (dgwBlockHeader PTR [ebx]).dgwBlockID,ebp
        cmp     dh,'N'
        mov     dx,'NR'
        je      _ItsLastHiBlock_
        or      ecx,ecx
        jne     SHORT _ItsLastHiBlock_
        mov     dh,'L'
_ItsLastHiBlock_:
        mov     (dgwBlockHeader PTR [ebx]).dgwState,edx
        pop     edx
        jecxz   SHORT _HiMemGet_
        mov     (dgwBlockHeader PTR [ebx+eax]).dgwLength,ecx
        mov     (dgwBlockHeader PTR [ebx+eax]).dgwState,edx
        mov     (dgwBlockHeader PTR [ebx+eax]).dgwBackLink,ebx
        or      edi,edi
        je      _HiMemGet_
        lea     edx,[eax+ebx]
        mov     (dgwBlockHeader PTR [edi]).dgwBackLink,edx
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
;Releasing memory block
;In:     EAX     -       address of memory block
;Out:    Carry=1 -       Error
        
PUBLIC  _FreeMem
_FreeMem:
        pushad
        lea     ebx,[eax-SIZE dgwBlockHeader]
		xor     edi,edi
        mov     eax,(dgwBlockHeader PTR [ebx]).dgwLength
        mov     edx,(dgwBlockHeader PTR [ebx]).dgwState
		cmp     dl,'F'
        je      _HiBlockFree
		cmp     dh,'L'
		je      _NoNextHiBlock
        lea     esi,[ebx+eax]
        mov     edi,esi  
        mov     ecx,(dgwBlockHeader PTR [esi]).dgwState
		cmp     cl,'F'
		jne     _NoNextHiBlock
		mov     edx,ecx
        add     edi,(dgwBlockHeader PTR [esi]).dgwLength
		cmp     ch,'L'
		jne     _NoLastFreeHiBlock
		xor     edi,edi
_NoLastFreeHiBlock:
        add     eax,(dgwBlockHeader PTR [esi]).dgwLength
_NoNextHiBlock:
        mov     ecx,(dgwBlockHeader PTR [ebx]).dgwBackLink
		jecxz   _NoPrevHiBlock
        cmp     BYTE PTR (dgwBlockHeader PTR [ecx]).dgwState,'F'
		jne     _NoPrevHiBlock
        add     eax,(dgwBlockHeader PTR [ecx]).dgwLength
		mov     ebx,ecx
_NoPrevHiBlock:
        mov     (dgwBlockHeader PTR [ebx]).dgwLength,eax
		mov     dl,'F'
        mov     (dgwBlockHeader PTR [ebx]).dgwState,edx

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
        push    edi
        push	ebx
        push    LARGE MEM_DECOMMIT
        push    edx
        push    ebp
        W32     VirtualFree,3
        pop     ebx
        pop		edi
dgwFMnoPagesToDeCommit:
		or      edi,edi
        je      _HiBlockFree
        mov     (dgwBlockHeader PTR [edi]).dgwBackLink,ebx
		clc
		db 0B0h
		stc
		popad
        ret

;----------------------------------------------------------------------------
;EAX    -       identifier

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
        cmp     eax,(dgwBlockHeader PTR [ebx]).dgwBlockID
        jne     dgwSetToNextHiBlock
        push    eax
        lea     eax,[ebx+SIZE dgwBlockHeader]
        call    _FreeMem
        pop     eax
        jmp     dgwLookAtAllHeaps
dgwSetToNextHiBlock:
        add     ebx,(dgwBlockHeader PTR [ebx]).dgwLength
        cmp     dh,'L'
        jne     dgwCheckNextBlock
dgwEmptyHeap:
        loopd   dgwLookAtAllHeaps
        popad
        clc
        ret

;----------------------------------------------------------------------------
;Setting global block identifier (called 'Marker' here...) inherited in
;subsequent memory allocating calls
;In:    EAX     -       New identifier

PUBLIC  _SetBlockMarker
_SetBlockMarker:
        mov     BlockMarker,eax
        ret
;----------------------------------------------------------------------------
;Setting the ID of a particular, allocated memory block
;In:     EAX     -       address of memory block
;        EBX     -       new ID
;Out:    Carry=0 -       OK
;             =1 -       Invalid address

PUBLIC  _SetBlockID
_SetBlockID:
        cmp     WORD PTR (dgwBlockHeader PTR [eax+2-SIZE dgwBlockHeader]).dgwState,'DG'
        stc
        jne     memSBIDinvalid
        mov     (dgwBlockHeader PTR [eax-SIZE dgwBlockHeader]).dgwBlockID,ebx
        clc
memSBIDinvalid:
        ret

;----------------------------------------------------------------------------
;Releasing all of the reserved memory blocks
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
;C-interface functions

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

.data

BlockMarker     DD      0h
HeapHandles     DD      HeapNum DUP( NULL )
dgwSysInfoAVL   DD      -1h

.data?

dgwSysInfo      SYSTEM_INFO     <>

END
