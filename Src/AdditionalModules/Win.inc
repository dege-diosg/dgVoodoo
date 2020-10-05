;-----------------------------------------------------------------------------
; WIN.INC - Some Windows memory and file related include for ASM
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

FALSE	EQU	0
TRUE	EQU	1

FILE_READ_DATA                       equ 1h
FILE_LIST_DIRECTORY                  equ 1h
FILE_WRITE_DATA                      equ 2h
FILE_ADD_FILE                        equ 2h
FILE_APPEND_DATA                     equ 4h
FILE_ADD_SUBDIRECTORY                equ 4h
FILE_CREATE_PIPE_INSTANCE            equ 4h
FILE_READ_EA                         equ 8h
FILE_READ_PROPERTIES                 equ FILE_READ_EA
FILE_WRITE_EA                        equ 10h
FILE_WRITE_PROPERTIES                equ FILE_WRITE_EA
FILE_EXECUTE                         equ 20h
FILE_TRAVERSE                        equ 20h
FILE_DELETE_CHILD                    equ 40h
FILE_READ_ATTRIBUTES                 equ 80h
FILE_WRITE_ATTRIBUTES                equ 100h
FILE_ALL_ACCESS                      equ STANDARD_RIGHTS_REQUIRED OR SYNCHRONIZE OR 1FFh
FILE_GENERIC_READ                    equ STANDARD_RIGHTS_READ OR FILE_READ_DATA OR FILE_READ_ATTRIBUTES OR FILE_READ_EA OR SYNCHRONIZE
FILE_GENERIC_WRITE                   equ STANDARD_RIGHTS_WRITE OR FILE_WRITE_DATA OR FILE_WRITE_ATTRIBUTES OR FILE_WRITE_EA OR FILE_APPEND_DATA OR SYNCHRONIZE
FILE_GENERIC_EXECUTE                 equ STANDARD_RIGHTS_EXECUTE OR FILE_READ_ATTRIBUTES OR FILE_EXECUTE OR SYNCHRONIZE
FILE_SHARE_READ                      equ 1h
FILE_SHARE_WRITE                     equ 2h
FILE_ATTRIBUTE_READONLY              equ 1h
FILE_ATTRIBUTE_HIDDEN                equ 2h
FILE_ATTRIBUTE_SYSTEM                equ 4h
FILE_ATTRIBUTE_DIRECTORY             equ 10h
FILE_ATTRIBUTE_ARCHIVE               equ 20h
FILE_ATTRIBUTE_NORMAL                equ 80h
FILE_ATTRIBUTE_TEMPORARY             equ 100h
FILE_ATTRIBUTE_COMPRESSED            equ 800h
FILE_NOTIFY_CHANGE_FILE_NAME         equ 1h
FILE_NOTIFY_CHANGE_DIR_NAME          equ 2h
FILE_NOTIFY_CHANGE_ATTRIBUTES        equ 4h
FILE_NOTIFY_CHANGE_SIZE              equ 8h
FILE_NOTIFY_CHANGE_LAST_WRITE        equ 10h
FILE_NOTIFY_CHANGE_SECURITY          equ 100h


CREATE_NEW                           equ 1
CREATE_ALWAYS                        equ 2
OPEN_EXISTING                        equ 3
OPEN_ALWAYS                          equ 4
TRUNCATE_EXISTING                    equ 5


DELETE                               equ 10000h
READ_CONTROL                         equ 20000h
WRITE_DAC                            equ 40000h
WRITE_OWNER                          equ 80000h
SYNCHRONIZE                          equ 100000h
STANDARD_RIGHTS_READ                 equ READ_CONTROL
STANDARD_RIGHTS_WRITE                equ READ_CONTROL
STANDARD_RIGHTS_EXECUTE              equ READ_CONTROL
STANDARD_RIGHTS_REQUIRED             equ 0F0000h
STANDARD_RIGHTS_ALL                  equ 1F0000h
SPECIFIC_RIGHTS_ALL                  equ 0FFFFh

INVALID_HANDLE_VALUE                 equ -1

FILE_BEGIN                           equ 0
FILE_CURRENT                         equ 1
FILE_END                             equ 2


SYSTEM_INFO STRUCT
  wProcessorArchitecture        WORD       ?
  wReserved                     WORD       ?
  dwPageSize                    DWORD      ?
  lpMinimumApplicationAddress   DWORD      ?
  lpMaximumApplicationAddress   DWORD      ?
  dwActiveProcessorMask         DWORD      ?
  dwNumberOfProcessors         DWORD      ?
  dwProcessorType               DWORD      ?
  dwAllocationGranularity       DWORD      ?
  wProcessorLevel               WORD       ?
  wProcessorRevision            WORD       ?
SYSTEM_INFO ENDS


PAGE_NOACCESS equ 01h     
PAGE_READONLY equ 02h     
PAGE_READWRITE equ 04h     
PAGE_WRITECOPY equ 08h     
PAGE_EXECUTE equ 10h     
PAGE_EXECUTE_READ equ 20h     
PAGE_EXECUTE_READWRITE equ 40h     
PAGE_EXECUTE_WRITECOPY equ 80h     
PAGE_GUARD equ 100h     
PAGE_NOCACHE equ 200h     
PAGE_WRITECOMBINE equ 400h     
MEM_COMMIT equ 1000h     
MEM_RESERVE equ 2000h     
MEM_DECOMMIT equ 4000h     
MEM_RELEASE equ 8000h     
MEM_FREE equ 10000h     
MEM_PRIVATE equ 20000h     
MEM_MAPPED equ 40000h     
MEM_RESET equ 80000h     
MEM_TOP_DOWN equ 100000h     
MEM_4MB_PAGES equ 80000000h     
