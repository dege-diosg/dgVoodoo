;-----------------------------------------------------------------------------
; FAKEVESA.ASM - Real mode fake VESA driver to suppress real VESA when needed
;                (during Glide session)
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


;Val¢s m¢d£ megszak¡t skezel“ az INT10h-hez: ha nincs VESA-emul ci¢,
;akkor nem hagyja, hogy a t‚nyleges VESA-driverrel kommunik ljon a
;program, am¡g a Glide akt¡v (ilyenkor a konzolablaknak mindig
;sz”veges m¢dban kell lennie).

INCLUDE	..\..\Inc\RMDriver.inc
INCLUDE	..\..\Inc\VesaDefs.inc

;hamis 1.2-es VESA
.386



code segment para public use16
        assume cs:code

org 100h
_start:

EXTRA_MEMORY    EQU     1024            ;a paletta sz m ra

vesaDrvHeader	LABEL _VesaDriverHeader
	DD		_DRIVERID
	DW		OFFSET _vesaentrypoint
	DW		MODULE_END - 100h
	DW		EXTRA_MEMORY
	DW		0

	DW		0
	DD		0
	DW		0
	DW		0
	DW		OFFSET vesaDrvProtTable
	DW		OFFSET vesaDrvProtTableEnd - OFFSET vesaDrvProtTable
	DB		0

_vesaentrypoint:
        pusha
        push    cs
        pop     ds
        push    WORD PTR 0h
        pop     es
        or      bp,bp
        jne     _vesaentry_reason_uninstall
        mov     eax,es:[10h*4]
        mov     vesaDrvHeader.origInt10Handler,eax
        mov     DWORD PTR _int10prevaddr,eax
        mov     ax,cs
        shl     eax,10h
        lea     ax,_int10
        mov     es:[10h*4],eax
        jmp     _vesaentry_end
_vesaentry_reason_uninstall:
        mov     eax,vesaDrvHeader.origInt10Handler
        mov     es:[10h*4],eax
_vesaentry_end:
        popa
        retf

_allowed:
        DB      0EAh
_int10prevaddr:
        DD      ?
_int10:
        cmp     ah,0h
        je      _iret
        cmp     ah,4Fh
        jne     _allowed
        cmp     al,0Bh
        jbe     _vesafuncnumberok
        mov     al,0Bh
_vesafuncnumberok:
        push    bp
        movzx   bp,al
        shl     bp,1h
        call    cs:[func_table+bp]
        pop     bp
_iret:  iret

Return_VBE_Info:
        push    ds
        push    es
        pop     ds
        mov     DWORD PTR [di].vbeSignature,'ASEV'
        mov     [di].vbeVersion,0102h
        mov     [di].oemStringPtr[2],cs
        mov     [di].oemStringPtr[0],OFFSET OEMString
        mov     DWORD PTR [di].capabilities,1b
        mov     [di].videoModePtr[2],cs
        mov     [di].videoModePtr[0],OFFSET VideoModeList
        mov     [di].totalMemory,1024/64
        pop     ds
        mov     ax,004Fh
        ret

Return_SVGA_Mode_Info:
        push    ds
        push    es
        pop     ds
        push    di
        push    cx
        mov     cx,SIZE ModeInfoBlock
        xor     al,al
        cld
        rep     stosb
        pop     cx
        pop     di
        push    si
;        call    searchmode
        lea     si,VideoModeList
nextmode:
        cmp     WORD PTR cs:[si],0FFFFh
        je      _01error
        add     si,2h
        cmp     cx,cs:[si-2]
        jne     nextmode
        sub     si,OFFSET VideoModeList
        mov     ax,[XRES+si-2]
        mov     [di].xResolution,ax
        mov     [di].bytesPerScanLineVESA,ax
        mov     ax,[YRES+si-2]
        mov     [di].yResolution,ax
        mov     [di].xCharSize,8h
        mov     [di].yCharSize,8h
        mov     [di].numberOfPlanes,1h
        mov     [di].bitsPerPixel,8h
        mov     [di].numberOfBanks,1h
        mov     [di].memoryModel,4h
        mov     [di].bankSize,0h
        mov     [di].directColorModeInfo,10b

        mov     [di].modeAttributes,11111b
        mov     [di].winAAttributes,111b
        mov     [di].winBAttributes,0b
        mov     [di].winGranularity,64
        mov     [di].winSize,64
        mov     [di].winASegment,0A000h
        mov     [di].winBSegment,0h
        mov     DWORD PTR [di].winFuncPtr,0h
        pop     si
        pop     ds        
        mov     ax,004Fh
        ret
_01error:
        pop     si
        pop     ds
        mov     ax,014Fh
        ret

Set_VBE_Mode:
        push    bx
        push    si
        and     bx,7FFFh
        mov     windowpos,0h
        mov     DACSize,6h
        lea     si,VideoModeList
nextmode_02:
        cmp     WORD PTR cs:[si],0FFFFh
        je      _02_error
        add     si,2h
        cmp     bx,cs:[si-2]
        jne     nextmode_02
	sub     si,OFFSET VideoModeList
        mov     ax,[XRES+si-2]
        mov     scanlinelength,ax
        mov     curmode,bx
        mov     ax,014Fh
        jmp     _02ok
_02_error:
        mov     ax,014Fh
_02ok:
        pop     si
        pop     bx
        ret

Return_SVGA_VideoMode:
        mov     bx,curmode
        mov     ax,004Fh
        ret

SaveRestoreSVGAState:
        mov     bx,2h
        mov     ax,004Fh
        ret

SetGetSVGABank:
        cmp     bl,0h
        jne     SGBankError
        cmp     bh,0h
        jne     getbank
        mov     windowpos,dx
        jmp     SGBankOk
getbank:mov     dx,windowpos
SGBankOk:
        mov     ax,004Fh
        ret        
SGBankError:
VESAError:
        mov     ax,014Fh
        ret

SetGetScanLineLength:
        cmp     bl,0h
        jne     _06get
        mov     scanlinelength,cx
_06get: mov     cx,scanlinelength
        mov     ax,curmode
;        push    si
;        lea     si,VideoModeList
;nextmode_02:
;        add     si,2h
;        cmp     ax,[si-2]
;        jne     nextmode_02
;        pop     si
        mov     dx,8000h
	mov     ax,004Fh
        ret

SetGetDisplayStart:
        cmp     bl,1h
        je      _07get
        mov     firstscanline,dx
        mov     firstpixel,cx
        jmp     _07ok
_07get: mov     bh,0h
        mov     cx,firstpixel
        mov     dx,firstscanline
_07ok:  mov     ax,004Fh
        ret

SetGetDACSize:
        cmp     bl,1h
        je      _08get
        mov     DACSize,bh
_08get: mov     bh,DACSize
        mov     ax,004Fh
        ret

Return_VBE_Prot_Interface:
        push    cs
        pop     es
        lea     di,vesaDrvHeader.protTable
        mov     cx,vesaDrvHeader.protTableLen
        mov     ax,004Fh
        ret

SetGetPaletteData:
        cmp     bl,1h
        ja      _09Error                
        push    si
        push    di
        push    ds
        push    es
        je      _09get
        push    es
        pop     ds
        push    cs
        pop     es
        mov     si,di
        lea     di,palette
        jmp     _09copy
_09get: push    cs
        pop     ds
        lea     si,palette
_09copy:
        shl     dx,2h
        shl     cx,2h
        add     si,dx
        add     di,dx
        cld
        rep     movsb
        pop     es
        pop     ds
        pop     di
        pop     si
        mov     ax,014Fh
        ret
_09Error:
        mov     ax,024Fh
        ret

Funcnotsupported:
        mov     ax,0100h
        ret

vesaDrvProtTable		LABEL
				DW      OFFSET _05_ - OFFSET protTable
                DW      OFFSET _07_ - OFFSET protTable
                DW      OFFSET _09_ - OFFSET protTable
                DW      0h
_05_:
                mov     ax,4f05h
                int     10h
                ret
_07_:
                mov     ax,4f07h
                int     10h
                ret
_09_:
                mov     ax,4f09h
                int     10h
                ret
vesaDrvProtTableEnd		LABEL


OEMString       DB      'dgVoodoo Fake VESA 1.2',0h
DACSize         DB      6h
VideoModeList   DW      100h,101h,103h,105h,107h,0FFFFh
XRES            DW      640,640,800,1024,1280
YRES            DW      400,480,600,768,1024
curmode         DW      ?
windowpos       DW      0h
scanlinelength  DW      ?
firstscanline   DW      ?
firstpixel      DW      ?
func_table      DW      Return_VBE_Info
                DW      Return_SVGA_Mode_Info
                DW      Set_VBE_Mode
                DW      Return_SVGA_VideoMode
                DW      SaveRestoreSVGAState
                DW      SetGetSVGABank
                DW      SetGetScanLineLength
                DW      SetGetDisplayStart
                DW      SetGetDACSize
                DW      SetGetPaletteData 
                DW      Return_VBE_Prot_Interface
                DW      Funcnotsupported

palette LABEL BYTE

MODULE_END LABEL

code ends
end _start
