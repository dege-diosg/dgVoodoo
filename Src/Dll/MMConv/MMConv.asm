;----------------------------------------------------------------------------
;dgVoodoo:  MultiMedia Converter
;
;	    MMConv.asm (MMX-implementáció)
;
;	    Ez a modul az egyes színformátumok közötti konverziót
;           biztosítja
;
;	    Támogatott formátumok (tetszõleges komponens-sorrenddel):
;		-  8 bit: P8
;		- 16 bit: RGB555, RGB565, ARGB1555, ARGB4444
;		- 32 bit: RGB888, ARGB8888
;
;	    Konverzió fajtái:
;		-  8bit -> 16 vagy 32 bit
;		- 16bit -> 16 vagy 32 bit
;		- 16bit -> 16bit ColorKey-jel
;		- 16bit -> 32bit ColorKey-jel
;		- 32bit -> 16 vagy 32 bit
;----------------------------------------------------------------------------

.586p

.model flat
UNICODE EQU 0
;INCLUDE w32main.inc
INCLUDE macros.inc

INCLUDE mmconv.inc
.mmx

;-------------------------------------------------------------------------------
;...............................................................................


PAGE_NOACCESS							EQU		01h
PAGE_READONLY							EQU		02h
PAGE_READWRITE							EQU		04h
PAGE_WRITECOPY							EQU		08h
PAGE_EXECUTE							EQU		10h
PAGE_EXECUTE_READ						EQU		20h
PAGE_EXECUTE_READWRITE					EQU		40h
PAGE_EXECUTE_WRITECOPY					EQU		80h
PAGE_GUARD								EQU		100h
PAGE_NOCACHE							EQU		200h
PAGE_WRITECOMBINE						EQU		400h

;-------------------------------------------------------------------------------
;...............................................................................


INCLUDE MMConv8toXX.asm
INCLUDE MMConv16to16.asm
INCLUDE MMConv16to16CK.asm
INCLUDE	MMConv16to32.asm
INCLUDE MMConv16to32CK.asm
INCLUDE MMConv32to16.asm
INCLUDE MMConv32to16CK.asm
INCLUDE MMConv32to32.asm
INCLUDE MMConv32to32CK.asm

INCLUDE MMConvCopy.asm

;---------------------------------------------------------------------------
.code

CopyPixFmt:
		push	esi
		push	edi
		push	ecx
		mov		esi,[esp + 1*4 + 3*4]
		mov		edi,[esp + 2*4 + 3*4]
		push	LARGE SIZE _pixfmt / 4
		pop		ecx
		cld
		rep		movsd
		pop		ecx
		pop		edi
		pop		esi
		ret		2*4

;---------------------------------------------------------------------------
PUBLIC  _MMCFillBuffer@24
PUBLIC  _MMCFillBuffer
_MMCFillBuffer@24:
_MMCFillBuffer:
        push    edi
        mov     edi,[esp+4 +1*4]
        mov     eax,[esp+12 +1*4]
        or      DWORD PTR [esp+24 +1*4],0
        jns     GITS_pitchnotnegative
        neg     DWORD PTR [esp+24 +1*4]
GITS_pitchnotnegative:
		mov		ecx,[esp+8 + 1*4]
		cmp		cl,32
		je		GITS_ckOk
        push    ax
        shl     eax,10h
        pop     ax
GITS_ckOk:
		movd	mm0,eax
		punpckldq mm0,mm0
        cld
		imul	ecx,[esp+16 +1*4]
		shr		ecx,6h
        mov     edx,[esp+20 +1*4]
		lea		edi,[edi+8*ecx]
		neg		ecx
		mov		eax,ecx
WriteNextInitScanLine:
writeNextPixel:
		movq	[edi+8*ecx],mm0
		nop
		inc		ecx
		jnz		writeNextPixel
		add     edi,[esp+24 +1*4]
		mov		ecx,eax
		dec     edx
        jne     WriteNextInitScanLine
		emms
        pop     edi
        ret     24

;---------------------------------------------------------------------------
PUBLIC _MMCGetPixelValueFromARGB@8
PUBLIC _MMCGetPixelValueFromARGB
_MMCGetPixelValueFromARGB@8:
_MMCGetPixelValueFromARGB:
        mov     eax,[esp+1*4 + 0]
        push    ebx
        push	esi
		rol		eax,8h
        mov     esi,[esp+2*4 + 2*4]
        mov     edx,eax
        mov     ecx,24+8h
        sub     ecx,(_pixfmt PTR [esi]).RBitCount
        shr     edx,cl
        mov     ecx,(_pixfmt PTR [esi]).RPos
        shl     edx,cl
        mov     ebx,eax
        and     ebx,0FF0000h
        mov     ecx,16+8h
        sub     ecx,(_pixfmt PTR [esi]).GBitCount
        shr     ebx,cl
        mov     ecx,(_pixfmt PTR [esi]).GPos
        shl     ebx,cl
        or      edx,ebx
        mov     ebx,eax
        and     ebx,0FF00h
        mov     ecx,8+8h
        sub     ecx,(_pixfmt PTR [esi]).BBitCount
        shr     ebx,cl
        mov     ecx,(_pixfmt PTR [esi]).BPos
        shl     ebx,cl
        or      edx,ebx
		movzx	eax,al
        mov     ecx,0+8h
        sub     ecx,(_pixfmt PTR [esi]).ABitCount
        shr     eax,cl
        mov     ecx,(_pixfmt PTR [esi]).APos
        shl     eax,cl
        or      edx,eax
        xchg    eax,edx
        pop     esi
        pop		ebx
        ret     8

;---------------------------------------------------------------------------
;unsigned int _stdcall MMCGetARGBFromPixelValue (unsigned int color, _pixfmt *byFmt, unsigned int constAlpha);
PUBLIC _MMCGetARGBFromPixelValue@12
PUBLIC _MMCGetARGBFromPixelValue
_MMCGetARGBFromPixelValue@12:
_MMCGetARGBFromPixelValue:
        mov     edx,[esp+1*4 + 0]
        push    ebx
        push	esi
        mov     esi,[esp+2*4 + 2*4]
		mov		eax,[esp+3*4 + 2*4]
		mov		ecx,(_pixfmt PTR [esi]).APos
		mov		ebx,edx
		shr		ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).ABitCount
		jecxz	_MMCGAFPV_noAlpha
		ror		ebx,cl
		cmp		cl,1h
		jne		_MMCGAFPV_notOneBit
		sar		ebx,7h
		mov		cl,8h
_MMCGAFPV_notOneBit:
		shld	eax,ebx,cl
		sub		cl,8h
		neg		cl
		shld	eax,ebx,cl
_MMCGAFPV_noAlpha:
		mov		ecx,(_pixfmt PTR [esi]).RPos
		mov		ebx,edx
		shr		ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).RBitCount
		ror		ebx,cl
		shld	eax,ebx,cl
		sub		cl,8h
		neg		cl
		shld	eax,ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).GPos
		mov		ebx,edx
		shr		ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).GBitCount
		ror		ebx,cl
		shld	eax,ebx,cl
		sub		cl,8h
		neg		cl
		shld	eax,ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).BPos
		mov		ebx,edx
		shr		ebx,cl
		mov		ecx,(_pixfmt PTR [esi]).BBitCount
		ror		ebx,cl
		shld	eax,ebx,cl
		sub		cl,8h
		neg		cl
		shld	eax,ebx,cl
		pop		esi
		pop		ebx
		ret		12

;---------------------------------------------------------------------------
;unsigned int _stdcall MMCConvertPixel (unsigned int srcColor, _pixfmt *srcFmt, _pixfmt *dstFmt, unsigned int
;					constAlpha);
PUBLIC  _MMCConvertPixel@16
PUBLIC  _MMCConvertPixel
_MMCConvertPixel@16:
_MMCConvertPixel:
        push    DWORD PTR [esp + 4*4 + 0]
        push    DWORD PTR [esp + 2*4 + 1*4]
        push    DWORD PTR [esp + 1*4 + 2*4]
		call	_MMCGetARGBFromPixelValue
        push    DWORD PTR [esp + 3*4 + 0]
		push	eax
		call	_MMCGetPixelValueFromARGB
		ret		16

;---------------------------------------------------------------------------
PUBLIC  _MMCConvertToPixFmt@12
PUBLIC  _MMCConvertToPixFmt
_MMCConvertToPixFmt@12:
_MMCConvertToPixFmt:
        push    edi
        push	esi
        mov     edi,[esp+2*4 +2*4]
        mov     edx,[esp+1*4 +2*4]
        push    ebx
        mov     eax,(DDPIXELFORMAT PTR [edx]).dwRGBBitCount
        mov     (_pixfmt PTR [edi]).BitPerPixel,eax
        mov     ecx,(DDPIXELFORMAT PTR [edx]).dwRBitMask
        mov     esi,ecx
        call    _GCTPFMT_getposandcount
        mov     (_pixfmt PTR [edi]).RPos,eax
        mov     (_pixfmt PTR [edi]).RBitCount,ebx
        mov     ecx,(DDPIXELFORMAT PTR [edx]).dwGBitMask
        or      esi,ecx
        call    _GCTPFMT_getposandcount
        mov     (_pixfmt PTR [edi]).GPos,eax
        mov     (_pixfmt PTR [edi]).GBitCount,ebx
        mov     ecx,(DDPIXELFORMAT PTR [edx]).dwBBitMask
        or      esi,ecx
        call    _GCTPFMT_getposandcount
        mov     (_pixfmt PTR [edi]).BPos,eax
        mov     (_pixfmt PTR [edi]).BBitCount,ebx
        mov     ecx,(DDPIXELFORMAT PTR [edx]).dwRGBAlphaBitMask
        or      ecx,ecx
        jne     _GCTPFMT_NoMissingAlpha
        or      DWORD PTR [esp+3*4 +3*4],0h
        je      _GCTPFMT_NoMissingAlpha
        mov     ecx,esi
        not     ecx
        cmp     (DDPIXELFORMAT PTR [edx]).dwRGBBitCount,32
        je      _GCTPFMT_NoMissingAlpha
        movzx   ecx,cx
_GCTPFMT_NoMissingAlpha:
        call    _GCTPFMT_getposandcount
        mov     (_pixfmt PTR [edi]).APos,eax
        mov     (_pixfmt PTR [edi]).ABitCount,ebx
        pop     ebx
        pop		esi
        pop		edi
        ret     12

_GCTPFMT_getposandcount:
        xor     eax,eax
        xor     ebx,ebx
        jecxz   _GCTPFMT_nomask
_GCTPFMT_pos_search:
        inc     eax
        shr     ecx,1h
        jnc     _GCTPFMT_pos_search
        dec     eax    
        xor     ebx,ebx
_GCTPFMT_count_next:
        inc     ebx
        shr     ecx,1h
        jc      _GCTPFMT_count_next
_GCTPFMT_nomask:
        ret

;---------------------------------------------------------------------------
PUBLIC  _MMCConvertAndTransferData@60
PUBLIC  _MMCConvertAndTransferData
_MMCConvertAndTransferData@60:
_MMCConvertAndTransferData:
        push    esi
        push	edi
        push	ebx
        push	ebp
        mov     esi,[esp+6*4 + 4*4]     ;src ptr
        mov     edi,[esp+7*4 + 4*4]     ;dst ptr
        mov     eax,[esp+12*4 + 4*4]    ;palette
        mov     palette,eax
        mov     eax,[esp+13*4 + 4*4]    ;palette map
        mov     paletteMap,eax
        mov     eax,[esp+8*4 + 4*4]     ;x
        mov     x,eax
        mov     ecx,[esp+9*4 + 4*4]     ;y
        mov     y,ecx
        dec     ecx
        mov     eax,[esp+10*4 + 4*4]     ;src pitch
        mov     srcpitch,eax
        or      eax,eax
        jns     _pos_srcpitch
        imul    eax,ecx
        sub     esi,eax
_pos_srcpitch:
        mov     eax,[esp+11*4 + 4*4]    ;dst pitch
        mov     dstpitch,eax
        or      eax,eax
        jns     _pos_dstpitch
        imul    eax,ecx
        sub     edi,eax
_pos_dstpitch:
        mov     eax,[esp+5*4 + 4*4]     ;constant alpha
        mov     constalpha,eax
        mov     ebx,[esp+1*4 + 4*4]     ;src format
        mov     eax,[esp+3*4 + 4*4]     ;colorkey
		mov     edx,[esp+4*4 + 4*4]     ;colorkey mask
		cmp		(_pixfmt PTR [ebx]).BitPerPixel,32
		je		_GCATD_dontDuplicate
		push	dx
		shl		edx,10h
		pop		dx
		push	ax
		shl		eax,10h
		pop		ax
_GCATD_dontDuplicate:
		and		eax,edx
        mov     colorkey,eax
        mov     colorKeyMask,edx

        mov     edx,[esp+2*4 + 4*4]     ;dst format

        mov     eax,[esp+14*4 + 4*4]    ;conversion type
        cmp     eax,4h
        jae     _GCATD_unsupported
        cmp     al,2h
        jbe     _GCATD_notcopying
        mov     edx,ebx
_GCATD_notcopying:

        mov     ecx,[_convertertables+4*eax]
        mov     eax,(_pixfmt PTR [ebx]).BitPerPixel
        shr     eax,3h
        mov     al,_callcode_src[eax-1]
        shl     eax,1h
        push    edx
        mov     edx,(_pixfmt PTR [edx]).BitPerPixel
        shr     edx,3h
        or      al,_callcode_dst[edx-1]
        pop     edx
        mov     ecx,[ecx+4*eax]
        jecxz   _GCATD_unsupported
		mov		eax,[esp+15*4 + 4*4]	;palette type
        call    ecx
        emms
_GCATD_unsupported:
        pop     ebp
        pop		ebx
        pop		edi
        pop		esi
        ret     15*4

;---------------------------------------------------------------------------
PUBLIC  _MMCIsPixfmtTheSame@8
PUBLIC  _MMCIsPixfmtTheSame
_MMCIsPixfmtTheSame@8:
_MMCIsPixfmtTheSame:
		push    esi
		push	edi
        mov     esi,[esp+1*4 +2*4]
        mov     edi,[esp+2*4 +2*4]
        mov     ecx,(SIZE _pixfmt / 4)
        cld
        xor     eax,eax
        repz    cmpsd
		sete	al
        pop     edi
        pop		esi
        ret     8

;---------------------------------------------------------------------------
.data

_masktable      	DB      0h, 1h, 3h, 7h, 0Fh, 1Fh, 3Fh, 7Fh, 0FFh
_masktable_up   	DB      0h, 80h, 0C0h, 0E0h, 0F0h, 0F8h, 0FCh, 0FEh, 0FFh

;az alsó i-(8-i) = 2*i-8 bit kimaszkolva (tábla 0-8-ig)
_masktable_magic 	DB		0h, 0h, 0h, 0h, 0Fh, 1Ch, 30h, 40h, 0h

_invshrcnt			DB		8h, 7h, 6h, 5h, 4h, 3h, 2h, 1h, 0h
_callcode_src   	DB      2h,0h,0h,1h
_callcode_dst   	DB      0h,0h,0h,1h
initok          	DB      0h

align   4
x               	DD      ?
y               	DD      ?
srcpitch        	DD      ?
dstpitch        	DD      ?
constalpha      	DD      ?
colorkey        	DD      ?
colorKeyMask		DD		?
palette         	DD      ?
paletteMap      	DD      ?

;- 16 to 16
;- 16 to 32
;- 32 to 16
;- 32 to 32
;-  8 to 16
;-  8 to 32

;general conversion routines
_convertfunctions       LABEL	DWORD
                DD      _general16to16converter
                DD      _general16to32converter
                DD      _general32to16converter
                DD      _general32to32converter
				DD		_general8to16converter
				DD		_general8to32converter

;general conversion routines with colorkeying
_ckconvertfunctions     LABEL	DWORD
                DD      _ckgeneral16to16converter
                DD      _ckgeneral16to32converter
                DD      _ckgeneral32to16converter
                DD      _ckgeneral32to32converter
				DD		0h
				DD		0h

;copy routines
_copyfunctions          LABEL	DWORD
                DD      _general16to16copy
                DD      0h
                DD      0h
                DD      _general32to32copy
                DD      _general8to8copy

;copy routines with colorkeying
_ckcopyfunctions        LABEL	DWORD
                DD      _ckgeneral16to16copy
                DD      0h
                DD      0h
                DD      _ckgeneral32to32copy

_convertertables        LABEL	DWORD
                DD      _convertfunctions
                DD      _ckconvertfunctions
                DD      _copyfunctions
                DD      _ckcopyfunctions

END
