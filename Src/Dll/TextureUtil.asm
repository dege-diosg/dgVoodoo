;-----------------------------------------------------------------------------
; TEXTUREUTIL.ASM - Utility functions for texturing
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

.586p
.model FLAT
UNICODE EQU 0
;INCLUDE w32main.inc
INCLUDE macros.inc

INCLUDE MMConv\mmconv.inc

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

GR_TEXFMT_8BIT                          EQU     0h
GR_TEXFMT_RGB_332                       EQU     1h
GR_TEXFMT_YIQ_422                       EQU     1h
GR_TEXFMT_ALPHA_8                       EQU     2h      ;(0..0xFF) alpha
GR_TEXFMT_INTENSITY_8                   EQU     3h      ;(0..0xFF) intensity
GR_TEXFMT_ALPHA_INTENSITY_44            EQU     4h
GR_TEXFMT_P_8                           EQU     5h      ;8-bit palette
GR_TEXFMT_RSVD0                         EQU     6h
GR_TEXFMT_RSVD1                         EQU     7h
GR_TEXFMT_16BIT                         EQU     8h
GR_TEXFMT_ARGB_8332                     EQU     8h
GR_TEXFMT_AYIQ_8422                     EQU     9h
GR_TEXFMT_RGB_565                       EQU     0ah
GR_TEXFMT_ARGB_1555                     EQU     0bh
GR_TEXFMT_ARGB_4444                     EQU     0ch
GR_TEXFMT_ALPHA_INTENSITY_88            EQU     0dh
GR_TEXFMT_AP_88                         EQU     0eh     ;8-bit alpha 8-bit palette
GR_TEXFMT_RSVD2                         EQU     0fh

_ncctable       STRUC

yRGB            DB      16 DUP(?)
iRGB            DW      12 DUP(?)
qRGB            DW      12 DUP(?)

_ncctable       ENDS

;BSWAP_EAX       MACRO
;        DB      0Fh,0C8h
;ENDM

BSWAP_EDX       MACRO
        DB      0Fh,0CAh
ENDM

INCLUDE TEXCK.ASM

.code

;-------------------------------------------------------------------------------
;...............................................................................

;Visszaadja egy ARGB-ben megadott színhez tartozó paletta- vagy ncc-indexet.
;Az összehasonlítás 24 biten történik, azaz csak az RGB komponenseket vizsgálja.

;unsigned int _stdcall TexGetTexIndexValue(int isNCC, DWORD color, void *table);

PUBLIC  _TexGetTexIndexValue@12
PUBLIC  _TexGetTexIndexValue
_TexGetTexIndexValue@12:
_TexGetTexIndexValue:
        pushad
        mov     ecx,[esp+4 +8*4]
        mov     edi,[esp+8 +8*4]
        mov     esi,[esp+12+ 8*4]
        and     edi,00FFFFFFh
        or      ecx,ecx
        je      _GTIV_Palette

        xor     eax,eax
_GTIV_NextElement:
        mov     ecx,eax
        shr     ecx,4h
        movzx   ebx,(_ncctable PTR [esi]).yRGB[ecx]
        mov     ecx,eax
        mov     edx,ebx
        push    ebx
        shr     ecx,2h
        and     ecx,3h
        imul    ecx,3*2
        mov     ebp,eax        
        and     ebp,3h
        imul    ebp,3*2
        add     dx,(_ncctable PTR [esi]).iRGB[ecx+0*2]
        add     dx,(_ncctable PTR [esi]).qRGB[ebp+0*2]
        or      dh,dh
        je      _GTIV_ClampOk1
        jns     _GTIV_ClampUp1
        xor     edx,edx
        jmp     _GTIV_ClampOk1
_GTIV_ClampUp1:
        mov     edx,0FFh
_GTIV_ClampOk1:
        shl     edx,10h
        add     bx,(_ncctable PTR [esi]).iRGB[ecx+1*2]
        add     bx,(_ncctable PTR [esi]).qRGB[ebp+1*2]
        or      bh,bh
        je      _GTIV_ClampOk2
        jns     _GTIV_ClampUp2
        xor     bl,bl
        jmp     _GTIV_ClampOk2
_GTIV_ClampUp2:
        mov     bl,0FFh
_GTIV_ClampOk2:
        mov     dh,bl
        pop     ebx
        add     bx,(_ncctable PTR [esi]).iRGB[ecx+2*2]
        add     bx,(_ncctable PTR [esi]).qRGB[ebp+2*2]
        or      bh,bh
        je      _GTIV_ClampOk3
        jns     _GTIV_ClampUp3
        xor     ebx,ebx
        jmp     _GTIV_ClampOk3
_GTIV_ClampUp3:
        mov     bl,0FFh
_GTIV_ClampOk3:
        mov     dl,bl
        cmp     edx,edi
        je      _GTIV_IndexFound
        inc     eax
        cmp     eax,100h
        jne     _GTIV_NextElement
        jmp     _GTIV_IndexFound

_GTIV_Palette:
;        mov     ecx,[esp+8 +12]
        xor     eax,eax
_GTIV_CheckNextPal:
        cmp     edi,[esi+4*eax]
        je      _GTIV_IndexFound
        inc     eax
        cmp     eax,100h
        jne     _GTIV_CheckNextPal
_GTIV_IndexFound:
        mov     [esp+7*4],eax
        popad
        ret     12


;int _stdcall TexCopyPalette(GuTexPalette *dst, GuTexPalette *src, int size);

PUBLIC  _TexCopyPalette@12
PUBLIC  _TexCopyPalette
_TexCopyPalette@12:
_TexCopyPalette:
        push    ebx
        mov     ebx,[esp+4+1*4]
        mov     edx,[esp+8+1*4]
        mov     ecx,[esp+12+1*4]
        push    LARGE 1h
_CopyPnext:
        mov     eax,[edx]
        add     edx,4h
        and     eax,00FFFFFFh
        cmp     eax,[ebx]
        je      _CPequ
        mov     DWORD PTR [esp],0h
_CPequ: mov     [ebx],eax
        add     ebx,4h
        dec     ecx
        jne     _CopyPnext
        pop     eax
        pop     ebx
        ret     12


;void _stdcall TexCopyNCCTable(GuNccTable *dst, GuNccTable *src);

PUBLIC  _TexCopyNCCTable@8
PUBLIC  _TexCopyNCCTable
_TexCopyNCCTable@8:
_TexCopyNCCTable:
        push    esi
        push    edi
        mov     esi,[esp+8+2*4]
        mov     edi,[esp+4+2*4]
        cld
        movsd
        movsd
        movsd
        movsd                           ;yRGB mezõ átmásolva
        mov     ecx,(3*4*2)/2           ;ennyi 32 bites integer (iRGB ‚s qRGB)
_CopyNCCnext:
        lodsd
        shrd    edx,eax,9h
        sar     edx,32-9
        shl     eax,7h
        sar     eax,7h
        mov     ax,dx
        stosd
        dec     ecx
        jne     _CopyNCCnext
        pop     edi
        pop     esi
        ret     8


;int _stdcall TexComparePalettes(GuTexPalette *src, GuTexPalette *dst);

PUBLIC  _TexComparePalettes@8
PUBLIC  _TexComparePalettes
_TexComparePalettes@8:
_TexComparePalettes:
        push    esi
        push    edi
        mov     esi,[esp+4+2*4]
        mov     edi,[esp+8+2*4]
        mov     ecx,256
        push    LARGE 1h
        pop     eax
        cld
        repz    cmpsd
        jne     _CPok
        xor     eax,eax
_CPok:  
        pop     edi
        pop     esi
        ret     8


;int _stdcall TexComparePalettesWithMap(GuTexPalette *src, GuTexPalette *dst, unsigned char *map);

PUBLIC  _TexComparePalettesWithMap@12
PUBLIC  _TexComparePalettesWithMap
_TexComparePalettesWithMap@12:
_TexComparePalettesWithMap:
        push    esi
        push    edi
        mov     esi,[esp+4+2*4]
        mov     edi,[esp+8+2*4]
        mov     edx,[esp+12+2*4]
        mov     ecx,256
        lea     edi,[edi+4*ecx-4]
        lea     esi,[esi+4*ecx-4]
        xor     eax,eax
        std
_CPWM_Continue:
        repz    cmpsd
        je      _CPWM_Ok
        cmp     BYTE PTR [edx+ecx],0h
        je      _CPWM_Continue
        push    LARGE 1h
        pop     eax
_CPWM_Ok:
        cld
        pop     edi
        pop     esi
        ret     12


;int _stdcall TexCompareNCCTables(GuNccTable *src, GuNccTable *dst);

PUBLIC  _TexCompareNCCTables@8
PUBLIC  _TexCompareNCCTables
_TexCompareNCCTables@8:
_TexCompareNCCTables:
        push    esi
        push    edi
        mov     esi,[esp+4+2*4]
        mov     edi,[esp+8+2*4]
        mov     ecx,(SIZE _ncctable)/4
        cld
        repz    cmpsd
        push    LARGE 1h
        pop     eax
        jnz     _CNCC_End
        xor     eax,eax
_CNCC_End:
        pop     edi
        pop     esi
        ret     8


;Ez az eljárás az adott NCC-táblát palettára konvertálja
;Be:    AH == 0         A paletta RGB formátumú
;       AH != 0         A paletta BGR formátumú
_ConvertNCCTable:
        push    esi
        xor     al,al
        mov     esi,edx
_CNCC_NextElement:
        movzx   ecx,al
        shr     ecx,4h
        movzx   ebx,(_ncctable PTR [esi]).yRGB[ecx]
        movzx   ecx,al
        mov     edx,ebx
        push    ebx
        shr     ecx,2h
        and     ecx,3h
        imul    ecx,3*2
        mov     ebp,eax        
        and     ebp,3h
        imul    ebp,3*2
        add     dx,(_ncctable PTR [esi]).iRGB[ecx+0*2]
        add     dx,(_ncctable PTR [esi]).qRGB[ebp+0*2]
        or      dh,dh
        je      _CNCC_ClampOk1
        jns     _CNCC_ClampUp1
        xor     edx,edx
        jmp     _CNCC_ClampOk1
_CNCC_ClampUp1:
        mov     edx,0FFh
_CNCC_ClampOk1:
        shl     edx,10h
        add     bx,(_ncctable PTR [esi]).iRGB[ecx+1*2]
        add     bx,(_ncctable PTR [esi]).qRGB[ebp+1*2]
        or      bh,bh
        je      _CNCC_ClampOk2
        jns     _CNCC_ClampUp2
        xor     bl,bl
        jmp     _CNCC_ClampOk2
_CNCC_ClampUp2:
        mov     bl,0FFh
_CNCC_ClampOk2:
        mov     dh,bl
        pop     ebx
        add     bx,(_ncctable PTR [esi]).iRGB[ecx+2*2]
        add     bx,(_ncctable PTR [esi]).qRGB[ebp+2*2]
        or      bh,bh
        je      _CNCC_ClampOk3
        jns     _CNCC_ClampUp3
        xor     ebx,ebx
        jmp     _CNCC_ClampOk3
_CNCC_ClampUp3:
        mov     bl,0FFh
_CNCC_ClampOk3:
        mov     dl,bl
        or      ah,ah
        je      _CNCC_RGBformat
        BSWAP_EDX
        ror     edx,8h
_CNCC_RGBformat:
        mov     [edi],edx
        add     edi,4h
;        mov     [edi+4*eax],edx
        inc     al
        jne     _CNCC_NextElement
        pop     esi
        ret


;void _stdcall TexConvertNCCToABGRPalette(GuNccTable *, PALETTEENTRY *);

;Ez az eljárás az NCC-táblát PALENTRY (BGR) palettává alakítja
PUBLIC  _TexConvertNCCToABGRPalette@8
PUBLIC  _TexConvertNCCToABGRPalette
_TexConvertNCCToABGRPalette@8:
_TexConvertNCCToABGRPalette:
        pushad
        mov     edx,[esp+4+8*4]
        mov     edi,[esp+8+8*4]
        mov     ah,1h
        call    _ConvertNCCTable
        popad
        ret     8


;void _stdcall TexConvertNCCToARGBPalette(GuNccTable *, unsigned int *);

;Ez az eljárás az NCC-táblát ARGB palettává alakítja
PUBLIC  _TexConvertNCCToARGBPalette@8
PUBLIC  _TexConvertNCCToARGBPalette
_TexConvertNCCToARGBPalette@8:
_TexConvertNCCToARGBPalette:
        pushad
        mov     edx,[esp+4+8*4]
        mov     edi,[esp+8+8*4]
        mov     ah,0h
        call    _ConvertNCCTable
        popad
        ret     8

;-------------------------------------------------------------------------------

;void _stdcall	TexConvertToMultiTexture(_pixfmt *dstFormat,
;                                        void *src, void *dstPal, void *dstAlpha,
;                                        unsigned int x, unsigned int y,
;                                        int dstPitchPal, int dstPitchAlpha);

;Egy AP88 textúrából multitextúrát készít
PUBLIC	_TexConvertToMultiTexture@32
PUBLIC	_TexConvertToMultiTexture
_TexConvertToMultiTexture@32:
_TexConvertToMultiTexture:
		push	ebx
		push	edi
		push	esi

		mov		edx,[esp + 1*4 +3*4]
		call	_TexConvToMultiTexInit

		mov		ecx,[esp + 5*4 +3*4]		;x
		mov		x,ecx
		mov		eax,[esp + 6*4 +3*4]		;y
		mov		y,eax
		mov		eax,[esp + 7*4 +3*4]		;dstPitchPal
		mov		dstPitchPal,eax
		mov		eax,[esp + 8*4 +3*4]		;dstPitchAlpha
		mov		dstPitchAlpha,eax
		mov		esi,[esp + 2*4 +3*4]		;src
		mov		ebx,[esp + 3*4 +3*4]		;dstPal
		mov		edi,[esp + 4*4 +3*4]		;dstAlpha
		lea		eax,_TexConvertToMultiTexture16
		cmp		(_pixfmt PTR [edx]).BitPerPixel,16
		je		_TCTMT_call
		lea		eax,_TexConvertToMultiTexture32
_TCTMT_call:
		call	eax
		emms
		pop		esi
		pop		edi
		pop		ebx
		ret		8*4


BeginOfMultiTexConv	LABEL NEAR

_TexConvertToMultiTexture16:
		lea		edi,[edi+2*ecx]
		lea		ebx,[ebx+ecx]
		mov		edx,y
		cmp		ecx,4
		jb		_TexConvToMultiTex16mini
_TexConvertToMultiTexture16_nexty:
		mov		ecx,x
		lea		esi,[esi+2*ecx]
		push	edi
		push	ebx
		shr		ecx,2h
		neg		ecx
_TexConvertToMultiTexture16_nextx:
		movq	mm0,[esi+8*ecx]	;
		movq	mm1,mm0		;
		pand	mm0,mm7			;alpha mask
		pand	mm1,mm6		;	;color mask
_TeXConvToMultiAlphaMMXShr1:
		psrlw	mm0,0h			;alpha shr
		movq	[edi+8*ecx],mm0 ;
		packuswb mm1,mm1
		movd	DWORD PTR [ebx+4*ecx],mm1	;
		nop
		inc		ecx		;
		jnz		_TexConvertToMultiTexture16_nextx
		pop		ebx
		pop		edi
		add		edi,dstPitchAlpha
		add		ebx,dstPitchPal
		dec		edx
		jnz		_TexConvertToMultiTexture16_nexty
		ret
_TexConvToMultiTex16mini:
_TexConvertToMultiTexture16mini_nexty:
		mov		ecx,x
		add		esi,ecx
		push	edi
		push	ebx
		neg		ecx
_TexConvertToMultiTexture16mini_nextx:
		mov		ax,[esi+2*ecx]
		mov		[ebx+ecx],al
		and		eax,edx			;alpha mask
_TeXConvToMultiAlphaShr1:
		shr		eax,0h			;alpha shr
		mov		[edi+2*ecx],ax
		inc		ecx
		jnz		_TexConvertToMultiTexture16mini_nextx
		pop		ebx
		pop		edi
		add		edi,dstPitchAlpha
		add		ebx,dstPitchPal
		dec		edx
		jnz		_TexConvertToMultiTexture16mini_nexty
		ret


_TexConvertToMultiTexture32:
		lea		edi,[edi+4*ecx]
		lea		ebx,[ebx+ecx]
		mov		edx,y
		cmp		ecx,4
		jb		_TexConvToMultiTex32mini
_TexConvertToMultiTexture32_nexty:
		mov		ecx,x
		lea		esi,[esi+2*ecx]
		push	edi
		push	ebx
		shr		ecx,1h
		neg		ecx
_TexConvertToMultiTexture32_nextx:
		movq	mm0,[esi+4*ecx]	;
		pxor	mm2,mm2
		movq	mm1,mm0		;
		pand	mm0,mm7			;alpha mask
		punpcklwd mm2,mm0	;
		pand	mm1,mm6			;color mask
_TeXConvToMultiAlphaMMXShr2:
		psrld	mm2,0h		;	;alpha shr
		pxor	mm3,mm3
		movq	[edi+8*ecx],mm2	;
		punpckhwd mm3,mm0
_TeXConvToMultiAlphaMMXShr3:
		psrld	mm3,0h		;	;alpha shr
		add		ecx,2h
		movq	[edi+8*ecx+8-2*8],mm3 ;
		packuswb mm1,mm1
		movd	DWORD PTR [ebx+2*ecx-2*2],mm1	;
		jnz		_TexConvertToMultiTexture32_nextx ;
		pop		ebx
		pop		edi
		add		edi,dstPitchAlpha
		add		ebx,dstPitchPal
		dec		edx
		jnz		_TexConvertToMultiTexture32_nexty
		ret
_TexConvToMultiTex32mini:
_TexConvertToMultiTexture32mini_nexty:
		mov		ecx,x
		add		esi,ecx
		push	edi
		push	ebx
		neg		ecx
_TexConvertToMultiTexture32mini_nextx:
		mov		ax,[esi+2*ecx]
		mov		[ebx+ecx],al
		and		eax,edx			;alpha mask
_TeXConvToMultiAlphaShr2:
		shr		eax,0h			;alpha shr
		mov		[edi+4*ecx],eax
		inc		ecx
		jnz		_TexConvertToMultiTexture32mini_nextx
		pop		ebx
		pop		edi
		add		edi,dstPitchAlpha
		add		ebx,dstPitchPal
		dec		edx
		jnz		_TexConvertToMultiTexture32mini_nexty
		ret

EndOfMultiTexConv	LABEL NEAR

_TexConvToMultiTexInit:
		or		_texInit,0h
		jnz		_TexConvMultiTexOK
		push    edx
        push    eax

        push    esp
        push    LARGE PAGE_EXECUTE_READWRITE
        push    LARGE (EndOfMultiTexConv - BeginOfMultiTexConv)
        push    OFFSET BeginOfMultiTexConv
        W32     VirtualProtect,4
        mov     _texInit,0FFh
        pop     eax
        pop		edx

_TexConvMultiTexOK:
		mov		eax,(_pixfmt PTR [edx]).ABitCount
		mov		al,_masktable[eax]
		shl		eax,8
		push	ax
		push	ax
		push	ax
		push	ax
		movq	mm7,[esp]
		mov		eax,00FF00FFh
		push	eax
		push	eax
		movq	mm6,[esp]
		add		esp,16
		mov		eax,(_pixfmt PTR [edx]).BitPerPixel
		sub		eax,(_pixfmt PTR [edx]).APos
		sub		eax,(_pixfmt PTR [edx]).ABitCount
		cmp		al,_texSHRCacheValue
		je		_TexConvMultiTexEnd
		mov		_texSHRCacheValue,al
		mov		BYTE PTR _TeXConvToMultiAlphaMMXShr1+3,al
		mov		BYTE PTR _TeXConvToMultiAlphaMMXShr2+3,al
		mov		BYTE PTR _TeXConvToMultiAlphaMMXShr3+3,al
		mov		BYTE PTR _TeXConvToMultiAlphaShr1+2,al
		mov		BYTE PTR _TeXConvToMultiAlphaShr2+2,al
_TexConvMultiTexEnd:
		ret

;-------------------------------------------------------------------------------

.data

pitch               DD  ?
dstPitchPal			DD	?
dstPitchAlpha		DD	?
x                   DD  ?
y                   DD  ?
_texInit			DB	0h
_texSHRCacheValue	DB	0EEh
_masktable			DB	0h, 80h, 0C0h, 0E0h, 0F0h, 0F8h, 0FCh, 0FEh, 0FFh

END
