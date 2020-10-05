;-----------------------------------------------------------------------------
; FILE.ASM - File related functions ported from my old DOS-Extender (DGPM)
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

;            File utility Windows Assemblyhez (WinAPI-n keresztÅl)
;----------------------------------------------------------------------------

INCLUDE MACROS.INC

FILEUTYPE       EQU     STANDALONE_OMF ;ASSEMBLY_INCLUDE

Yes_    EQU     1h
No_     EQU     0h
;----------------------------------------------------------------------------
;Az egyes rutinok beinkl£doltassanak-e vagy sem
CreateFileQ     EQU     Yes_
OpenFileQ       EQU     Yes_
CloseFileQ      EQU     Yes_
SetFilePosQ     EQU     Yes_
GetFilePosQ     EQU     Yes_
GetFileSizeQ    EQU     Yes_
ReadFileQ       EQU     Yes_
WriteFileQ      EQU     Yes_

;----------------------------------------------------------------------------

IF      (FILEUTYPE  EQ  STANDALONE_OMF) OR (FILEUTYPE  EQ  NEEDEDONLY_OMF)

.386
.model  FLAT

UNICODE EQU 0
INCLUDE W32MAIN.INC

ENDIF

.code

;----------------------------------------------------------------------------
;File lÇtrehoz†sa °r†shoz. Ha a file lÇtezik, akkor azt felÅl°rja.

;Be:    ESI             -       file nevÇnek c°me
;       EAX             -       file attrib£tuma
;Ki:    Carry=1         -       hiba
;       EAX             -       file handle

;C-szintaxis: HANDLE _CreateFile(char *name, dword attribs);
IF      CreateFileQ  EQ  Yes_
PUBLIC  __CreateFile@8
PUBLIC  __CreateFile
__CreateFile@8:
__CreateFile    PROC

        pushad
        mov     esi,[esp+4+8*4]
        mov     eax,[esp+8+8*4]
        push    LARGE 0h
        push    eax
        push    LARGE CREATE_ALWAYS
        push    LARGE NULL ;OFFSET fuSecAttribs
        push    LARGE 0h
        push    LARGE GENERIC_WRITE
        push    esi
        W32     CreateFileA,7
;        push    eax
;        W32     GetLastError,0
;        or      eax,eax
;        pop     eax
;        je      fuCFnoError
;        mov     eax,INVALID_HANDLE_VALUE
fuCFnoError:
        mov     [esp+7*4],eax
        popad
        ret     8

__CreateFile    ENDP
ENDIF

;----------------------------------------------------------------------------
;file megnyit†sa olvas†sra
;Be:    
;       ESI             -       file neve
;Ki:    Carry=1         -       hiba
;       EAX             -       file handle

;C-szintaxis: HANDLE _OpenFile(char *name);
IF      OpenFileQ  EQ  Yes_
PUBLIC  __OpenFile@4
PUBLIC  __OpenFile
__OpenFile@4:
__OpenFile      PROc

        pushad
        mov     esi,[esp+4+8*4]
        push    LARGE 0h
        push    LARGE 0h                ;attrib£tumok Çrdektelenek
        push    LARGE OPEN_EXISTING
        push    LARGE NULL ;OFFSET fuSecAttribs
        push    LARGE FILE_SHARE_READ
        push    LARGE GENERIC_READ
        push    esi
        W32     CreateFileA,7
;        push    eax
;        W32     GetLastError
        or      eax,eax
;        pop     eax
        jne     fuOFnoError
        mov     eax,INVALID_HANDLE_VALUE        
fuOFnoError:
        mov     [esp+7*4],eax
        popad
        ret     4

__OpenFile      ENDP
ENDIF

;----------------------------------------------------------------------------
;file lez†r†sa
;Be:    EAX     -       filehandle
;Ki     -

;C-szintaxis: void _CloseFile(HANDLE handle);
IF      CloseFileQ  EQ  Yes_
PUBLIC  __CloseFile@4
PUBLIC  __CloseFile
__CloseFile@4:
__CloseFile     PROC

        pushad
        push    DWORD PTR [esp+4+8*4]
        W32     CloseHandle,1
        popad
        ret     4

__CloseFile     ENDP
ENDIF

;----------------------------------------------------------------------------
;filepointer lekÇrdezÇse
;Be:    EDX     -       Filehandle
;Ki:    EAX     -       Poz°ci¢ (=0FFFFFFFFFh: hiba)
;       Carry=1 -       Hiba

;C-szintaxis: dword _GetFilePos(HANDLE handle);
IF      GetFilePosQ  EQ  Yes_
PUBLIC  __GetFilePos@4
PUBLIC  __GetFilePos
__GetFilePos@4:
__GetFilePos    PROC

        pushad
        mov     edx,[esp+4+8*4]
        push    LARGE FILE_CURRENT
        push    LARGE NULL
        push    LARGE 0h
        push    edx
        W32     SetFilePointer,4
;        cmp     eax,0FFFFFFFFh
;        stc
;        je      fuGFPError
;        clc
;fuGFPError:
        mov     [esp+7*4],eax
        popad
        ret     4


__GetFilePos    ENDP
ENDIF

;----------------------------------------------------------------------------
;File mÇretÇnek lekÇrdezÇse
;Be:    EAX     -       FileHandle
;Ki:    Carry=1 -       Hiba
;       EAX     -       file mÇrete

;C-szintaxis: dword _GetFileSize(HANDLE handle);
IF      GetFileSizeQ  EQ  Yes_
PUBLIC  __GetFileSize@4
PUBLIC  __GetFileSize
__GetFileSize@4:
__GetFileSize   PROC

        pushad
        mov     eax,[esp+4+8*4]
        push    LARGE NULL
        push    eax
        W32     GetFileSize,2
;        cmp     eax,0FFFFFFFFh
;        stc
;        je      fuGFSError
;        clc
;fuGFSError:
        mov     [esp+7*4],eax
        popad
        ret     4


__GetFileSize   ENDP
ENDIF

;----------------------------------------------------------------------------
;filepointer be†ll°t†sa
;Be:    EBX     -       Poz°ci¢ relat°v helye
;                       (FILE_BEGIN, FILE_CURRENT, FILE_END)
;       EDX     -       Filehandle
;       EAX     -       Poz°ci¢
;Ki:    Carry=1 -       Hiba
;       EAX     -       Akt. poz°ci¢

;C-szintaxis: dword _SetFilePos(HANDLE handle, dword pos, dword how);
IF      SetFilePosQ  EQ  Yes_
PUBLIC  __SetFilePos@12
PUBLIC  __SetFilePos
__SetFilePos@12:
__SetFilePos    PROC

        pushad
        mov     edx,[esp+4+8*4]
        mov     eax,[esp+8+8*4]
        mov     ebx,[esp+12+8*4]
        push    ebx
        push    LARGE NULL
        push    eax edx
        W32     SetFilePointer,4
;        cmp     eax,0FFFFFFFFh
;        stc
;        je      fuSFPError
;        clc
;fuSFPError:
        mov     [esp+7*4],eax
        popad
        ret     12


__SetFilePos    ENDP
ENDIF

;----------------------------------------------------------------------------
;Olvas†s fileb¢l
;Be:    EBX     -       line†ris kezdìc°m
;       ECX     -       olvasni k°v†nt byte-ok sz†ma
;       EAX     -       file handle
;Ki:    CF=1    -       nem sikerÅlt
;     CF=0,EAX  -       tÇnylegesen beolvasott byteok sz†ma
;       ZF=1,   ha a megadott Çs tÇnylegesen beolvasott adatok mennyisÇge
;               egyenlì
;

;C-szintaxis: dword _ReadFile(HANDLE handle, void *mem, dword num);
IF      ReadFileQ  EQ  Yes_
PUBLIC  __ReadFile@12
PUBLIC  __ReadFile
__ReadFile@12:
__ReadFile      PROC

        pushad
        mov     eax,[esp+4+8*4]
        mov     ebx,[esp+8+8*4]
        mov     ecx,[esp+12+8*4]
        push    ecx
        push    LARGE 0h
        push    LARGE OFFSET fuTempStore
        push    ecx ebx eax
        W32     ReadFile,5
        pop     ecx
        cmp     eax,TRUE
        mov     eax,fuTempStore
        je      fuRFNoError
        mov     eax,0FFFFFFFFh
fuRFNoError:
        cmp     eax,ecx
        mov     [esp+7*4],eax
        popad      
        ret     12

__ReadFile      ENDP
ENDIF

;----------------------------------------------------------------------------
;Ir†s fileba
;Be:    EBX     -       line†ris kezdìc°m
;       ECX     -       °rni k°v†nt byte-ok sz†ma
;       EAX     -       file handle
;Ki:    CF=1    -       nem sikerÅlt
;     CF=0,EAX  -       tÇnylegesen ki°rt byteok sz†ma
;       ZF=1, ha a megadott Çs tÇnylegesen ki°rt adatok mennyisÇge
;              egyenlì
;

;C-szintaxis: dword _WriteFile(HANDLE handle, void *mem, dword num);
IF      WriteFileQ  EQ  Yes_
PUBLIC  __WriteFile@12
PUBLIC  __WriteFile
__WriteFile@12:
__WriteFile     PROC

        pushad
        mov     eax,[esp+4+8*4]
        mov     ebx,[esp+8+8*4]
        mov     ecx,[esp+12+8*4]
        push    ecx
        push    LARGE 0h
        push    LARGE OFFSET fuTempStore
        push    ecx ebx eax
        W32     WriteFile,5
        pop     ecx
        cmp     eax,TRUE
        mov     eax,fuTempStore
        je      fuWFNoError
        mov     eax,0FFFFFFFFh
fuWFNoError:        
        mov     [esp+7*4],eax
        cmp     ecx,eax
        popad
        ret     12

__WriteFile     ENDP
ENDIF

.data

fuSecAttribs    DD      12
                DD      NULL
                DD      FALSE

.data?

fuTempStore     DD      ?

IF      (FILEUTYPE  EQ  STANDALONE_OMF) OR (FILEUTYPE  EQ  NEEDEDONLY_OMF)

END

ENDIF

