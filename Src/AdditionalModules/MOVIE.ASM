;-----------------------------------------------------------------------------
; MOVIE.ASM - Frame-presenter frame system for animation ported from DOS
;             This version relies on DDRAW 7 instead of VESA
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

; MOVIE for DirectDraw & Direct3D
;----------------------------------------------------------------------------
;       Picture Consumer for Picture Producer (CPU)
;       A producer †ltal legy†rtott egyes kÇpkock†kat kipakol¢ rutin, s az
;       ahhoz tartoz¢ elj†r†sok
;----------------------------------------------------------------------------

INCLUDE MACROS.INC

MOVIETYPE       EQU     STANDALONE_OMF ;ASSEMBLY_INCLUDE

MV_NO           EQU     0
MV_YES          EQU     1

MOVIE3D         EQU     MV_YES

MaxBuffNum      EQU     8                       ;Rajzpufferek maxim†lis sz†ma
;----------------------------------------------------------------------------
;A TrSemafor bitjei:

semBUSY         EQU     01h                     ;A consumer foglalt
semVALID        EQU     02h                     ;A rajzolt puffer haszn†lhat¢
semSetPal       EQU     04h                     ;jelezhetjÅk a Consumernek,
                                                ;hogy a kîv. kÇpkirak†skor
                                                ;palett†t is †ll°tsa be
semPaused       EQU     10h                     ;Consumer paused
semSoftware     EQU     20h                     ;norm†lmem-rajzpufferek
semSHOOTED      EQU     80h                     ;A consumer megh°v¢dott
semWMTIMER      EQU     40h

;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
;Windows GUID structure
GUID            STRUC

Data1           DD      ?
Data2           DW      ?
Data3           DW      ?
Data4           DB      ?
                DB      ?
                DB      ?
                DB      ?
                DB      ?
                DB      ?
                DB      ?
                DB      ?

GUID            ENDS
;----------------------------------------------------------------------------
;A Movie strukt£r†ja, amelyen keresztÅl a Movie Çs a program kommunik†l
MOVIEDATA       STRUC

cmiFlags        DD      ?
cmiBuffNum      DD      ?
cmiFreq         DD      ?
cmiXres         DD      ?
cmiYres         DD      ?
cmiDispFreq     DD      ?
cmiConvRGB      DD      ?
cmiConvByPP     DD      ?
cmiRGB          DD      ?
cmiByPP         DD      ?
cmiBitPP        DD      ?
cmiPitch        DD      ?
cmiSurfaceMem   DD      ?
cmiWinHandle    DD      ?
cmiWinTitle     DD      ?
cmiWinIcon      DD      ?
cmiWinCursor    DD      ?
cmiWinProc      DD      ?
cmiWinPosX      DD      ?
cmiWinPosY      DD      ?
cmiWinSizeX     DD      ?
cmiWinSizeY     DD      ?
cmiPicBX        DD      ?
cmiPicBY        DD      ?
cmiPicJX        DD      ?
cmiPicJY        DD      ?
cmiWinGapX      DD      ?
cmiWinGapY      DD      ?
cmiDirectDraw   DD      ?
cmiDirect3D     DD      ?
cmiFrames       DD      ?
cmiZBitDepth    DD      ?
cmiTexBitPP     DD      ?
cmiDeviceGUID   GUID    <>
cmiWinPost      DD      ?
cmiPixelFormat  DD      ?
cmiCallWhenMIRQ DD      ?
cmiCallPostC    DD      ?
cmiStencilBitDepth DD   ?

MOVIEDATA       ENDS

;----------------------------------------------------------------------------
;State flags
MV_FULLSCR      EQU     1h              ;Teljes kÇpernyìs m¢d (InitMovie)
MV_WINDOWED     EQU     2h              ;Ablakos m¢d (InitMovie)
MV_VIDMEM       EQU     4h              ;KÇppufferek a videomem¢ri†ban (InitMovie)
MV_INITOK       EQU     8h
MV_MINIMIZED    EQU     10h
MV_MAXIMIZED    EQU     20h
MV_WINDOWSIZEOK EQU     40h
MV_CURSORHIDED  EQU     80h
MV_3D           EQU     100h            ;Direct3D lÇtrehoz†sa (InitDraw)
MV_MOVIEWINDOW  EQU     200h
MV_NOCONSUMERCALL EQU   400h            ;a Consumer h°v†sa kîzvetlenÅl az
                                        ;alkalmaz†sb¢l tîrtÇnik, nem az
                                        ;idìz°tìn keresztÅl
MV_ANTIALIAS    EQU     800h
MV_STENCIL      EQU     1000h
;----------------------------------------------------------------------------
;Z-buffer flags
MVZBUFF_AUTO    EQU     80000000h       ;Z-puffer form†tum†nak automatikus
                                        ;kiv†laszt†sa
;----------------------------------------------------------------------------
;Texturing flags
MVTEXFORM_AUTO  EQU     80000000h       ;Text£ra form†tum†nak automatikus
                                        ;kiv†laszt†sa
;----------------------------------------------------------------------------
;Internal flags
mviModeChanging EQU     1h
mviTIMERMsend   EQU     2h

;----------------------------------------------------------------------------
;Hibak¢dok
MV_NOERR                EQU     0h
MVERR_DDRAWINIT         EQU     1h
MVERR_CANTSETMODE       EQU     2h

;----------------------------------------------------------------------------

IF      (MOVIETYPE  EQ  STANDALONE_OMF) OR (MOVIETYPE  EQ  NEEDEDONLY_OMF)

.386
.model  FLAT

UNICODE EQU 0
INCLUDE W32MAIN.INC
INCLUDE MMSYSTEM.INC

ENDIF

INCLUDE DDRAW.INC
INCLUDE D3D.INC

.code

JUMPS

;C-szintaxis: void EnumDisplayDevices(LPDDENUMCALLBACK func);
;Ezzel lekÇrdezhetjÅk a display device-ok azonos°t¢it, neveit, stb.
PUBLIC  _EnumDisplayDevs@4
PUBLIC  _EnumDisplayDevs
_EnumDisplayDevs@4:
_EnumDisplayDevs        PROC

        pushad
        or      mvDDrawInited,0h
        jne     mvEDDDirectDrawInitOK
        call    LoadDirectDraw&MM
        push    LARGE 0h
        pop     eax
        jc      mvEDDFinished
mvEDDDirectDrawInitOK:
        push    LARGE 0h
        push    DWORD PTR [esp+8+8*4]
        call    [DirectDrawEnumerate]
        push    LARGE 1h
        pop     eax
mvEDDFinished:
        mov     [esp+7*4],eax
        popad
        ret     4

_EnumDisplayDevs        ENDP

;C-szintaxis:   int GetActDispMode(GUID *dev, LPDDSURFACEDESC2 surface);
PUBLIC  _GetActDispMode@8
PUBLIC  _GetActDispMode
_GetActDispMode@8:
_GetActDispMode         PROC

        pushad
        or      mvDDrawInited,0h
        jne     mvGADMDirectDrawInitOK
        call    LoadDirectDraw&MM
        push    LARGE 0h
        pop     eax
        jc      mvGADMError
mvGADMDirectDrawInitOK:

        mov     eax,[esp+4+8*4]
        call    mvCreateDDobject        
        or      eax,eax
        je      mvGADMnoError
        xor     eax,eax
        jmp     mvGADMError
mvGADMnoError:
        mov     ebx, DWORD PTR [esp+8+8*4]
        push    ebx
        mov     [ebx].dwSize, SIZE DDSURFACEDESC2
        mov     ebx,lpDD
        push    ebx
        mov     eax,[ebx]
        call    [eax].GetDisplayMode7
        push    ebx
        mov     eax,[ebx]
        call    [eax].Release
        push    LARGE 1h
        pop     eax
mvGADMError:
        mov     [esp+7*4],eax
        popad
        ret     8

_GetActDispMode         ENDP

;C-szintaxis: void mvGetDispDevCaps(GUID *device, DDCAPS *caps);
PUBLIC _mvGetDispDevCaps@8
PUBLIC _mvGetDispDevCaps
_mvGetDispDevCaps@8:
_mvGetDispDevCaps       PROC

        pushad
        mov     eax,[esp+4+8*4]
        call    mvCreateDDobject
        push    LARGE NULL
        push    DWORD PTR [esp+8+8*4+4]
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].GetCaps7
        popad
        ret     8        

_mvGetDispDevCaps       ENDP

;C-szintaxis: void EnumDispModes(GUID *device, LPDDENUMMODESCALLBACK2 func,\
;                                 unsigned int mode);
;Az adott display device-on be†ll°that¢ videom¢dokat kÇrdezhetjÅk le
;ParamÇterek:   device:         Az adott device
;               func:           A megh°v¢d¢ callback-rutin
;               mode:           MV_FULLSCR vagy MV_WINDOWED
;                               windowed esetÇn csak az aktu†lis megjelen°tÇsi
;                               m¢ddal kompatibilis m¢dokat sorolja fel
PUBLIC _EnumDispModes@12
PUBLIC _EnumDispModes
_EnumDispModes@12:
_EnumDispModes          PROC

        pushad
        mov     ebp,esp
	or      mvDDrawInited,0h
        jne     mvEDMDirectDrawInitOK
        call    LoadDirectDraw&MM
        push    LARGE 0h
        pop     eax
        jc      mvEDMError
mvEDMDirectDrawInitOK:

        mov     eax,[ebp+4+4*8]
        call    mvCreateDDobject
        or      eax,eax
        jne     mvEDMError

        push    DWORD PTR [ebp+8+8*4]
        push    LARGE 0h
        push    LARGE NULL
        push    LARGE 0 ;DDEDM_REFRESHRATES
        mov     ebx,lpDD
        push    ebx
        mov     eax,[ebx]
        call    [eax].EnumDisplayModes7

        push    ebx
        mov     eax,[ebx]
        call    [eax].Release
mvEDMError:
        popad
        ret     12

mvCreateDDobject:
        push    LARGE NULL
        push    OFFSET IID_IDirectDraw7
        push    OFFSET lpDD
        push    eax
        call    [DirectDrawCreateEx]
        ret
        
_EnumDispModes          ENDP

;Ezzel a fÅggvÇnnyel hozzuk lÇtre a fì DirectDraw (Çs szÅksÇg szerint a
; Direct3D) objektumot. SzintÇn ez a fÅggvÇny kÇsz°ti el azt az ablakot,
;amelyet (amelybe) a rajzol†shoz haszn†lunk.
;ParamÇterek:   moviestruc:     A program Çs a Movie kommunik†ci¢j†hoz
;                               haszn†lt struk£ra c°me. M†r rîgtîn ebben kell
;                               megadni az ablak al†bbi paramÇtereit:
;                        cmiWinTitle:  ablak c°mÇre mutat¢ pointer
;                        cmiWinIcon:   ablak ikonja
;                        cmiWinCursor: ablak kurzora
;                        cmiWinProc:   ablakkezelìbìl megh°vand¢ rutin
;               device:         Mely display device-on fogunk dolgozni
;                               (NULL= primary display device, amin a GDI is
;                                dolgozik)
;                               A lehetsÇges device-ok GUID-jait (azonos°t¢it)
;                               lekÇrdezhetjÅk az EnumDispayDevices fgv-nyel

;C-szintaxis:   int InitDraw(MOVIE *moviestruc, GUID *device);
PUBLIC          _InitDraw@8
PUBLIC          _InitDraw
_InitDraw@8:
_InitDraw       PROC

        pushad
        mov     ebp,esp
        or      mvDDrawInited,0h
        jne     mvDirectDrawInitOK

        call    LoadDirectDraw&MM
        jc      mvDirectDrawNotFound

mvDirectDrawInitOK:
        mov     eax,[esp+4+8*4]
        mov     mvMovie,eax

;------ Ablak lÇtrehoz†sa, amelyben a kÇsz kÇp l†that¢ lesz

        xor     eax,eax
        mov     ebx,mvMovie
        or      [ebx].cmiWinHandle,eax
        jne     mvApplicationOwnWindow
mvDirectDrawCreateWin:
        push    LARGE 0h
        W32     GetModuleHandleA,1
        mov     mvWinClass.wc_hInstance,eax

        push    eax
        mov     eax,[ebx].cmiWinCursor
        mov     mvWinClass.wc_hCursor,eax
        mov     eax,NULL
        mov     mvWinClass.wc_hbrBackground,eax
        mov     eax,[ebx].cmiWinIcon
        mov     mvWinClass.wc_hIcon,eax

        push    OFFSET mvWinClass
        W32     RegisterClassA,1
        pop     eax

        push    LARGE NULL
        push    eax
        push    LARGE NULL              ;menu id
        push    LARGE NULL
        push    LARGE 250
        push    LARGE 250
        push    LARGE CW_USEDEFAULT
        push    LARGE CW_USEDEFAULT
        push    LARGE WS_OVERLAPPEDWINDOW
        push    [ebx].cmiWinTitle
        push    OFFSET mvWinClassName
        push    LARGE 0h ;WS_EX_TRANSPARENT
        W32     CreateWindowExA,12
        mov     [ebx].cmiWinHandle,eax
        mov     [ebx].cmiWinPost,eax

        push    OFFSET DestRect
        push    eax
        W32     GetClientRect,2
        mov     eax,250
        sub     eax,DestRect.rc_bottom
        mov     [ebx].cmiWinGapY,eax
        mov     eax,250
        sub     eax,DestRect.rc_right
        mov     [ebx].cmiWinGapX,eax
        push    OFFSET DestRect
        push    eax
        W32     GetWindowRect,2
        mov     eax,DestRect.rc_top
        mov     [ebx].cmiWinPosY,eax
        mov     eax,DestRect.rc_left
        mov     [ebx].cmiWinPosX,eax
        or      [ebx].cmiFlags,MV_MOVIEWINDOW
        jmp     mvWindowCreatOK

mvApplicationOwnWindow:

;        push    OFFSET mvWindProc
;        push    LARGE -4 ;GWL_WINDPROC
;        push    [ebx].cmiWinHandle
;        W32     SetWindowLongA,3
;        mov     [ebx].cmiWinProc,eax

mvWindowCreatOK:
;------ DirectDraw fìobjektum lÇtrehoz†sa

        push    LARGE NULL
        push    OFFSET IID_IDirectDraw7
        push    OFFSET lpDD
        push    DWORD PTR [ebp+8+4*8] ;LARGE NULL
        call    [DirectDrawCreateEx]
        or      eax,eax
        mov     eax,MVERR_DDRAWINIT
        jne     mvDirectDrawNotFound
        mov     eax,lpDD
        mov     [ebx].cmiDirectDraw,eax

;------ SzÅksÇg esetÇn Direct3D fìobjektum lÇtrehoz†sa

        test    [ebx].cmiFlags,MV_3D
        je      mvIDsuccess
        push    LARGE OFFSET lpD3D
        push    OFFSET IID_IDirect3D7
        push    eax
        mov     eax,[eax]
        call    [eax].QueryInterface7
        or      eax,eax
        jne     mvDirectDrawNotFound
        mov     eax,lpD3D
        mov     [ebx].cmiDirect3D,eax

mvIDsuccess:
        DB      0B0h
mvDirectDrawNotFound:
        stc
        mov     eax,FALSE
        jc      mvRetID
        mov     eax,TRUE
mvRetID:mov     [esp+7*4],eax
        popad
        ret     8

LoadDirectDraw&MM:
        lea     eax,mvDirectDrawDLL
        lea     ebx,DirectDrawAddresses
        lea     ecx,DirectDrawFunctions
        lea     esi,mvDDLibHandle
        call    mvGetLibFuncs
        jc      mvDirectDrawNotFound_

        lea     eax,mvWinMMDLL
        lea     ebx,WinMMAddresses
        lea     ecx,WinMMFunctions
        lea     esi,mvWINMMLibHandle
        call    mvGetLibFuncs
        jc      mvDirectDrawNotFound_

        mov     mvDDrawInited,1h
mvDirectDrawNotFound_:
        ret        

mvGetLibFuncs:
        push    ebx ecx eax
        W32     LoadLibraryA,1
        mov     [esi],eax
;        mov     mvLibHandle,eax
        xchg    eax,ecx
        pop     esi edi
        jecxz   mvGLFOK
mvGetNextDDrawfunc:
        or      BYTE PTR [esi],0h
        je      mvGLFOK_
        push    ecx esi ecx
        W32     GetProcAddress,2
        pop     ecx
        or      eax,eax
        stc
        je      mvGLFOK
        cld
        stosd
mvIDnextfunc:
        lodsb
        or      al,al
        jne     mvIDnextfunc
        jmp     mvGetNextDDrawfunc
mvGLFOK_:
        clc
        ret
mvGLFOK:stc
        ret

_InitDraw       ENDP

;Lev†lasztja a grafika miatt csatolt DLL-eket.
PUBLIC          _UninitDraw@0
PUBLIC          _UninitDraw
_UninitDraw@0:
_UninitDraw     PROC

        pushad
        or      mvDDrawInited,0h
        je      mvUDalreadyUI
        mov     mvDDrawInited,0h

        mov     ecx,lpD3D
        jecxz   mvUDno3d
        push    ecx
        mov     eax,[ecx]
        call    [eax].Release7
mvUDno3d:

;        mov     eax,mvMovie
;        push    LARGE DDSCL_NORMAL
;        push    LARGE [eax].cmiWinHandle
;        mov     eax,lpDD
;        push    eax
;        mov     eax,[eax]
;        call    [eax].SetCooperativeLevel7

        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].Release7

        mov     ebx,mvMovie
        test    [ebx].cmiFlags,MV_MOVIEWINDOW
        je      mvUDnotourWindow

        push    [ebx].cmiWinHandle
        W32     DestroyWindow,1
        push    [mvWinClass.wc_hInstance]
        push    OFFSET mvWinClassName
        W32     UnregisterClassA,2
        jmp     mvUDfreeLIBs
mvUDnotourWindow:
;        push    [ebx].cmiWinProc
;        push    LARGE -4 ;GWL_WINDPROC
;        push    [ebx].cmiWinHandle
;        W32     SetWindowLongA,3

mvUDfreeLIBs:
        push    [mvWINMMLibHandle]
        W32     FreeLibrary,1
        push    [mvDDLibHandle]
        W32     FreeLibrary,1

        mov     eax,mvMovie
        mov     [eax].cmiFlags,0h
        mov     eax,NULL
        mov     lpDD,eax
        mov     lpD3D,eax
        mov     lpDDSPrimary,eax
        mov     lpDDSBack,eax
        mov     lpDDSThird,eax
        mov     lpDDClipper,eax
        mov     lpDDPal,eax
        mov     mvMovie,eax
        mov     DirectDrawCreateEx,eax
mvUDalreadyUI:
        popad
        ret

_UninitDraw     ENDP

;----------------------------------------------------------------------------
;A flageknek megfelelìen lÇtrehozza az al†bbiakat:
;       MV_SOFTWARE     -       Szoftverrel rajzolt pufferek (hagyom†nyos)
;                               Ezeket a videomem¢ri†ban hozza lÇtre, ha az
;                               MV_VIDMEM flag be van †ll°tva
;       MV_3D           -       Hardverrel renderelt puffer

;A hanszn†lt MOVIE strukt£r†ban ki kell tîlteni:
;       cmiFlags:
;       cmiXres:        V°zszintes felbont†s
;       cmiYres:        FÅggìleges felbont†s
;       cmiBitPP:       BitmÇlysÇg (8,15,16,24,32)
;       cmiBuffNum:     Pufferek sz†ma
;       cmiFreq:        Pufferek megjelen°tÇsÇnek frekvenci†ja

PUBLIC          _InitMovie@0
PUBLIC          _InitMovie
_InitMovie@0:
_InitMovie      PROC

        pushad
        mov     edi,mvMovie
        test    [edi].cmiFlags,MV_INITOK
        jne     mvAlreadyInited
        lea     eax,OFFSCREENBUFFS
        mov     DrawnBSoft,eax
        mov     ToDrawBSoft,eax
        mov     DrawnedNum,0h
        mov     esi,[edi].cmiFlags
        mov     eax,esi
        and     eax,MV_FULLSCR+MV_WINDOWED
        cmp     eax,MV_FULLSCR+MV_WINDOWED
        je      mvAlreadyInited
        mov     eax,[edi].cmiXres
        mov     [edi].cmiWinSizeX,eax
        mov     eax,[edi].cmiYres
        mov     [edi].cmiWinSizeY,eax
        xchg    eax,edx
        mov     eax,[edi].cmiBitPP
        shr     eax,3h
        mov     [edi].cmiByPP,eax
        mov     [edi].cmiConvByPP,eax
        
;------ DirectDraw cooperative level be†ll°t†sa

        pushad
;        push    LARGE DDSCL_ALLOWREBOOT OR DDSCL_FULLSCREEN OR DDSCL_EXCLUSIVE
;        push    [edi].cmiWinHandle
;        mov     eax,lpDD
;        push    eax
;        mov     eax,[eax]
;        call    [eax].SetCooperativeLevel7


        mov     eax,DDSCL_NORMAL
        test    esi,MV_WINDOWED
        jne     mvWindowedMode1
        mov     eax,DDSCL_ALLOWREBOOT OR DDSCL_FULLSCREEN OR DDSCL_EXCLUSIVE
mvWindowedMode1:
        push    eax
        push    [edi].cmiWinHandle
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].SetCooperativeLevel7
        or      eax,eax
        mov     DDERR,eax
        popad
        stc
        mov     eax,MVERR_DDRAWINIT
        jne     mvIMreturn

		xor		eax,eax
		mov		lpDDClipper,eax
		test    [edi].cmiFlags,MV_WINDOWED
        je      mvNeedntClipper

        push    LARGE NULL
        push    OFFSET lpDDClipper
        push    LARGE NULL
        call    [DirectDrawCreateClipper]

		mov     eax,lpDDClipper
        push    [edi].cmiWinHandle
        push    LARGE 0h
        push    eax
        mov     eax,[eax]
        call    [eax].SetHWnd

mvNeedntClipper:

        call    _Create
        jnc     mvCreatingSuccessful

        mov     eax,mvMovie
        push    LARGE DDSCL_NORMAL
        push    [eax].cmiWinHandle
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].SetCooperativeLevel7

		mov     ecx,lpDDClipper
		jecxz	mvFailNoClipper
		push	ecx
        mov     eax,[ecx]
        call    [eax].Release

mvFailNoClipper:
        stc
        jmp     mvIMreturn

mvCreatingSuccessful:

;        test    [edi].cmiFlags,MV_WINDOWED
;        je      mvNeedntClipper

        mov     ebx,[edi].cmiWinHandle
        test    [edi].cmiFlags,MV_MOVIEWINDOW
        je      mvNoWinGaps

        push    (LARGE SWP_NOMOVE) + (LARGE SWP_NOZORDER)
        mov     eax,[edi].cmiWinSizeY
        add     eax,[edi].cmiWinGapY
        push    eax
        mov     eax,[edi].cmiWinSizeX
        add     eax,[edi].cmiWinGapX
        push    eax
        push    LARGE 0h
        push    LARGE 0h
        push    LARGE 0h
        push    ebx
        W32     SetWindowPos,7

        test    [edi].cmiFlags,MV_WINDOWED
        je      mvHideWindow

        push    LARGE SW_SHOWNORMAL
        push    ebx
        W32     ShowWindow,2
mvHideWindow:
        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvDontHideCursor

        push    LARGE FALSE
        mov     ebx,[edi].cmiFlags
        test    ebx,MV_CURSORHIDED
        jne     mvDontHideCursor
        or      [edi].cmiFlags,MV_CURSORHIDED
        test    ebx,MV_FULLSCR
        jne     mvCursorVisOK
        pop     eax
        and     [edi].cmiFlags,NOT MV_CURSORHIDED
        push    LARGE TRUE
mvCursorVisOK:        
        W32     ShowCursor,1

mvDontHideCursor:
mvNoWinGaps:
        call    _SetUpTimer
        lea     eax,MovieConsumer
        or      [edi].cmiFlags,MV_INITOK

mvAlreadyInited:
        clc
mvIMreturn:
        mov     eax,FALSE
        jc      mvRetIM
        mov     eax,TRUE
mvRetIM:
        mov     ecx,DDERR
        mov     [esp+6*4],ecx
        mov     [esp+7*4],eax
        popad
        ret

_InitMovie      ENDP

_Create:
        test    esi,MV_WINDOWED
        jne     mvDontSetDM
        pushad

;------ DirectDraw videom¢d be†ll°t†sa, ha full screen

        push    LARGE 5h
        pop     ebx
mvIMTryAgainToSetDM:
        push    LARGE 0h
        push    [edi].cmiDispFreq ;LARGE 0h
        push    [edi].cmiBitPP
        push    [edi].cmiYres
        push    [edi].cmiXres
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].SetDisplayMode7
        cmp     eax,80004005h
        jne     mvIMCheckDMSucc
        dec     ebx
        jne     mvIMTryAgainToSetDM
;        cmp     eax,8076021Ch ;DDERR_WASSTILLDRAWING
;        je      mvIMTryAgainToSetDM
mvIMCheckDMSucc:
        or      eax,eax
        mov     DDERR,eax
        popad
        je      mvDontSetDM

        mov     eax,MVERR_CANTSETMODE
        stc
        jmp     mvIMreturn_C

mvDontSetDM:
        mov     al,semVALID
        test    esi,MV_VIDMEM
        jne     mvIMSVOK
        or      al,semSoftware
mvIMSVOK:
        mov     TrSemafor,al
        push    edi
        lea     edi,ddsd
        cld
        mov     ecx, SIZE DDSURFACEDESC2
        push    ecx
        xor     al,al
        rep     stosb
        lea     edi,ddbltfx
        mov     ecx, SIZE DDBLTFX
        push    ecx
        rep     stosb
        pop     eax
        mov     ddbltfx.dwSize,eax
        pop     ecx edi

;------ Primary surface lÇtrehoz†sa

;--- jellemzìk, ha ablakos m¢d

        mov     ddsd.dwSize,ecx
        mov     eax,DDSD_CAPS
        mov     ebx,DDSCAPS_PRIMARYSURFACE
        test    esi,MV_WINDOWED
        jne     mvWindowedMode2

;--- jellemzìk, ha teljes kÇpernyì

        or      eax,DDSD_BACKBUFFERCOUNT
;        mov     ecx,[edi].cmiPixelFormat
;        jecxz   mvIMnoformatdefined
;        or      eax,DDSD_PIXELFORMAT
;        push    esi edi
;        mov     esi,ecx
;        lea     edi,[ddsd.ddpfPixelFormat]
;        mov     ecx,SIZE DDPIXELFORMAT
;        cld
;        rep     movsb
;        pop     edi esi
mvIMnoformatdefined:
        or      ebx,DDSCAPS_FLIP OR DDSCAPS_COMPLEX
        push    LARGE 1h
        pop     edx
        test    esi,MV_VIDMEM
        je      mvStoreBackBuffNum
        test    esi,MV_3D
        je      mvIMnoDirect3D
        or      ebx,DDSCAPS_3DDEVICE
mvIMnoDirect3D:
        mov     edx,[edi].cmiBuffNum
        dec     edx
        cmp     edx,2h
        jbe     mvStoreBackBuffNum
        push    LARGE 2h
        pop     edx                             ;Hardveres teljes kÇpernyì
                                                ;esetÇn max. 3 puffer!
                                                ;(egyelìre)
mvStoreBackBuffNum:
        mov     ddsd.dwBackBufferCount2,edx

mvWindowedMode2:
        mov     ddsd.dwFlagsSD2,eax
        mov     ddsd.ddsCaps2.dwCapsDC2,ebx
        test    esi,MV_ANTIALIAS
        je      _noantialias
        test    ebx,DDSCAPS_3DDEVICE
        je      _noantialias
        mov     ddsd.ddsCaps2.dwCapsDC22,DDSCAPS2_HINTANTIALIASING
_noantialias:

        push    LARGE NULL
        push    OFFSET lpDDSPrimary
        push    OFFSET ddsd
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].CreateSurface7

        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvWindowedMode3
        
        mov     ddscaps.dwCapsDC2,DDSCAPS_BACKBUFFER
        push    OFFSET lpDDSBack
        push    OFFSET ddscaps
        mov     eax,lpDDSPrimary
        push    eax
        mov     eax,[eax]
        call    [eax].GetAttachedSurface7

        mov     eax,lpDDSBack
        mov     lpDDSThird,eax
        cmp     [edi].cmiBuffNum,3h
        jb      mvIMNoThirdBuffer
        
        push    OFFSET mvEnumSurfaceCallback
        push    OFFSET lpDDSThird
        push    eax
        mov     eax,[eax]
        call    [eax].EnumAttachedSurfaces

mvIMNoThirdBuffer:

;        push    LARGE NULL
;        push    OFFSET lpDDPal
;        push    OFFSET Palette
;        push    LARGE DDPCAPS_8BIT
;        mov     eax,lpDD
;        push    eax
;        mov     eax,[eax]
;        call    [eax].CreatePalette7

;        mov     eax,lpDDPal
;        push    eax
;        mov     eax,lpDDSPrimary
;        push    eax
;        mov     eax,[eax]
;        call    [eax].SetPalette7

mvWindowedMode3:
        mov     ddspixelf.dwSize, SIZE DDPIXELFORMAT
        push    OFFSET ddspixelf
        mov     eax,lpDDSPrimary
        push    eax
        mov     eax,[eax]
        call    [eax].GetPixelFormat7

        test    [edi].cmiFlags, MV_WINDOWED
        je      mvWindowedMode4

        push    [lpDDClipper]
        mov     eax,lpDDSPrimary
        push    eax
        mov     eax,[eax]
        call    [eax].SetClipper7

;        mov     ddspixelf.dwBBitMask,00FF0000h
;        mov     ddspixelf.dwGBitMask,0000FF00h
;        mov     ddspixelf.dwRBitMask,000000FFh
;        mov     ddspixelf.dwRGBAlphaBitMask,0FF000000h

;        mov     ddspixelf.dwRGBBitCount,32
;        mov     ddspixelf.dwFlagsPF,DDPF_RGB
;        mov     ddspixelf.dwSize, SIZE DDPIXELFORMAT
mvWindowedMode4:
        xor     esi,esi
        mov     ebx,ddspixelf.dwBBitMask
        call    mvGetPixelPos
        or      esi,edx
        shl     esi,8h
        mov     ebx,ddspixelf.dwGBitMask
        call    mvGetPixelPos
        or      esi,edx
        shl     esi,8h
        mov     ebx,ddspixelf.dwRBitMask
        call    mvGetPixelPos
        or      esi,edx
        mov     [edi].cmiRGB,esi
        mov     [edi].cmiConvRGB,esi
        mov     eax,ddspixelf.dwRGBBitCount
        mov     [edi].cmiBitPP,eax
        shr     eax,3h
        mov     [edi].cmiByPP,eax
        mov     [edi].cmiConvByPP,eax

;------------------------------------
;Pufferek lÇtrehoz†sa a fizikai vagy a videomem¢ri†ban

        mov     eax,[edi].cmiFlags
        test    eax,MV_FULLSCR
        je      mvCreateNeeded
        test    eax,MV_VIDMEM
        jne     mvIMreturn_C ;mvBuffersInPrimarySurface

mvCreateNeeded:
        mov     ecx,[edi].cmiBuffNum
        push    edi
        mov     ebx,edi
        lea     edi,OFFSCREENBUFFS
mvCreateBuffers:
        push    ecx edi
        mov     eax,[ebx].cmiXres
        mov     ddsd.dwWidth2,eax
        mov     SrcRect.rc_right,eax
        mov     eax,[ebx].cmiYres
        mov     ddsd.dwHeight2,eax
        mov     SrcRect.rc_bottom,eax
        mov     ddsd.dwFlagsSD2,DDSD_CAPS OR DDSD_WIDTH OR DDSD_HEIGHT
        mov     eax,DDSCAPS_VIDEOMEMORY
        test    [ebx].cmiFlags,MV_VIDMEM
        jne     mvCreateVidMemBuffer
        or      ddsd.dwFlagsSD2,DDSD_PIXELFORMAT
        mov     eax,DDSCAPS_SYSTEMMEMORY
mvCreateVidMemBuffer:
        test    [ebx].cmiFlags,MV_3D
        je      mvIMnoDirect3D2
        or      eax,DDSCAPS_3DDEVICE
        test    [ebx].cmiFlags,MV_ANTIALIAS
        je      _noantialias2
        mov     ddsd.ddsCaps2.dwCapsDC22,DDSCAPS2_HINTANTIALIASING
_noantialias2:
mvIMnoDirect3D2:
        mov     ddsd.ddsCaps2.dwCapsDC2,eax
        lea     esi,ddspixelf
        lea     edi,ddsd.ddpfPixelFormat
        mov     ecx,SIZE DDPIXELFORMAT
        cld
        rep     movsb
        push    LARGE NULL
        push    DWORD PTR [esp+4]
        push    OFFSET ddsd
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].CreateSurface7
        or      eax,eax
        pop     edi ecx
        add     edi,4h
        loopd   mvCreateBuffers
        pop     edi
        mov     eax,[edi].cmiBuffNum
        shl     eax,2h
        add     eax,OFFSET OFFSCREENBUFFS
        mov     ENDOFFSCREENBUFFS,eax
mvIMreturn_C:
        ret

_SetUpTimer:
        mov     eax,1000
        cdq
        div     [edi].cmiFreq
        mov     mvIRQTime,eax

        xor     eax,eax
        test    [edi].cmiFlags,MV_NOCONSUMERCALL
        jne     mvIMNoTimerCreate
        push    LARGE TIME_PERIODIC
        push    LARGE 0h
        push    OFFSET MovieIRQ
        mov     eax,mvIRQTime
        push    eax eax
        call    [timeSetEvent]
mvIMNoTimerCreate:
        mov     mvIRQtimer,eax
        ret

mvGetPixelPos:
        xor     edx,edx
        push    LARGE 4h
        pop     ecx
mvGPPnextbyte:
        or      bl,bl
        jne     mvGPPok
        shr     ebx,8h
        inc     edx
        loopd   mvGPPnextbyte
mvGPPok:ret

mvEnumSurfaceCallback:
        mov     eax,[esp+4]
        mov     ecx,[esp+12]
        mov     [ecx],eax
        mov     eax,0
        ret     12

;----------------------------------------------------------------------------
PUBLIC          _DeInitMovie@0
PUBLIC          _DeInitMovie
_DeInitMovie@0:
_DeInitMovie:
        pushad

        mov     edi,mvMovie
        mov     ebx,[edi].cmiFlags
        test    ebx,MV_MOVIEWINDOW
        je      mvDontShowCursor
        test    ebx,MV_CURSORHIDED OR MV_WINDOWED
        jne     mvDontShowCursor

        push    LARGE FALSE
        and     [edi].cmiFlags,NOT MV_CURSORHIDED
        test    ebx,MV_FULLSCR
        jne     mvCursorVisOK_show
        pop     eax
        and     [edi].cmiFlags,NOT MV_CURSORHIDED
        push    LARGE TRUE
mvCursorVisOK_show:
        W32     ShowCursor,1

mvDontShowCursor:
        call    _Destroy
        mov     eax,mvMovie
        mov     [eax].cmiFlags,0h
;        and     [eax].cmiFlags,NOT (MV_INITOK+MV_FULLSCR+MV_WINDOWED+MV_VIDMEM)

		xor		ecx,ecx
		xchg    ecx,lpDDClipper
		jecxz	mvNoClipper
		push	ecx
        mov     eax,[ecx]
        call    [eax].Release

mvNoClipper:
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].RestoreDisplayMode7

        push    LARGE DDSCL_NORMAL
        mov     eax,mvMovie
        push    [eax].cmiWinHandle
;        push    LARGE NULL
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].SetCooperativeLevel7
        
        popad
        ret

;----------------------------------------------------------------------------
PUBLIC  _ReInitMovie@4
PUBLIC  _ReInitMovie
_ReInitMovie@4:
_ReInitMovie:
        pushad
        call    _Destroy
        mov     ecx,[esp+4+8*4]
        mov     edx,mvMovie
        mov     eax,[ecx].cmiXres
        mov     [edx].cmiXres,eax
        mov     eax,[ecx].cmiYres
        mov     [edx].cmiYres,eax
        mov     eax,[ecx].cmiBitPP
        mov     [edx].cmiBitPP,eax
        shr     eax,3h
        mov     [edx].cmiByPP,eax
        mov     [edx].cmiConvByPP,eax
        mov     eax,[ecx].cmiBuffNum
        mov     [edx].cmiBuffNum,eax
        mov     eax,[ecx].cmiFreq
        mov     [edx].cmiFreq,eax
        mov     eax,[ecx].cmiCallWhenMIRQ
        mov     [edx].cmiCallWhenMIRQ,eax
        mov     esi,[edx].cmiFlags
        call    _Create
        push    LARGE FALSE
        pop     eax
        jc      mvRIMError
        call    _SetUpTimer
        push    LARGE TRUE
        pop     eax
mvRIMError:
        mov     [esp+7*4],eax
        popad
        ret     4

;----------------------------------------------------------------------------
_Destroy:
        mov     ecx,mvIRQtimer
        jecxz   mvDIMNoTimerCreated
        push    ecx
        call    [timeKillEvent]
mvDIMNoTimerCreated:

        call    _DestroyZBuffer
        mov     edi,mvMovie

        mov     ecx,[edi].cmiBuffNum
        lea     ebx,OFFSCREENBUFFS
mvDIMnextbuff:
        mov     eax,[ebx]
        cmp     eax,NULL
        je      mvDIMnosurface
        push    ecx eax
        mov     eax,[eax]
        call    [eax].Release7
        pop     ecx
        mov     DWORD PTR [ebx],NULL
mvDIMnosurface:
        lea     ebx,[ebx+4]
        loopd   mvDIMnextbuff

;        mov     eax,lpDDPal
;        push    eax
;        mov     eax,[eax]
;        call    [eax].Release7
;        mov     lpDDPal,NULL

;        mov     eax,lpDDSBack
;        cmp     eax,NULL
;        je      mvDIMnoback
;        push    eax
;        mov     eax,[eax]
;        call    [eax].Release7
;        mov     lpDDSBack,NULL

mvDIMnoback:
		mov     eax,lpDDSPrimary
		push	eax
        
		push    LARGE NULL
        push    eax
        mov     eax,[eax]
        call    [eax].SetClipper7
		mov     lpDDSPrimary, NULL

		pop		eax
        push    eax
        mov     eax,[eax]
        call    [eax].Release7
        mov     lpDDSPrimary, NULL
        ret


;----------------------------------------------------------------------------
PUBLIC  _GetDefMovieWinProc@0
PUBLIC  _GetDefMovieWinProc
_GetDefMovieWinProc@0:
_GetDefMovieWinProc:
        lea     eax,mvWindProc
        ret

;----------------------------------------------------------------------------

PUBLIC          _SetFullScreenMode@0
PUBLIC          _SetFullScreenMode
_SetFullScreenMode@0:
_SetFullScreenMode:
        pushad
        mov     eax,MV_FULLSCR
        call    mvSetScreenMode
        popad
        ret
mvSetScreenMode:
        mov     edi,mvMovie
        test    [edi].cmiFlags,eax
        jne     mvAlreadyThisScr
        or      mvFlags,mviModeChanging
        call    _DeInitMovie
        mov     esi,[edi].cmiFlags
        and     [edi].cmiFlags,NOT (MV_FULLSCR+MV_WINDOWED)
        or      [edi].cmiFlags,eax
        push    [edi].cmiWinSizeX
        push    [edi].cmiWinSizeY
        push    edi
        call    _InitMovie
        pop     edi
        pop     [edi].cmiWinSizeY
        pop     [edi].cmiWinSizeX
mvAlreadyThisScr:
        and     mvFlags,NOT mviModeChanging
        ret

;----------------------------------------------------------------------------
PUBLIC  _SetWindowedMode@0
PUBLIC  _SetWindowedMode
_SetWindowedMode@0:
_SetWindowedMode:
        pushad
        mov     eax,MV_WINDOWED
        call    mvSetScreenMode
        mov     edi,mvMovie
        push    LARGE SWP_NOZORDER
        mov     ebx,[edi].cmiWinSizeY
        add     ebx,[edi].cmiWinGapY
        push    ebx
        mov     ebx,[edi].cmiWinSizeX
        add     ebx,[edi].cmiWinGapX
        push    ebx
        push    [edi].cmiWinPosY
        push    [edi].cmiWinPosX
        push    LARGE 0h
        push    [edi].cmiWinHandle
        W32     SetWindowPos,7

        mov     eax,[edi].cmiFlags
        mov     ebx,SW_MAXIMIZE
        test    eax,MV_MAXIMIZED
        jne     mvSWMsetwin
        mov     ebx,SW_MINIMIZE
        test    eax,MV_MINIMIZED
        je      mvSWMOK
mvSWMsetwin:
        push    ebx
        push    [edi].cmiWinHandle
        W32     ShowWindow,2
mvSWMOK:
        popad
        ret

;----------------------------------------------------------------------------
PUBLIC  _ToggleScreenMode@0
PUBLIC  _ToggleScreenMode
_ToggleScreenMode@0:
_ToggleScreenMode:
        push    eax
        mov     eax,mvMovie
        test    [eax].cmiFlags,MV_WINDOWED
        pop     eax
        jne     _SetFullScreenMode
        jmp     _SetWindowedMode

;----------------------------------------------------------------------------
;Feladata a kîvetkezì puffer allok†l†sa, amibe majd rajzolni lehet. A rajzol†s
;vÇgÇt az EndDraw-val kell jelezni, amely elintÇzi, hogy a puffer kikerÅljîn
;a kÇpernyìre. (AzÇrt kell kÇt fÅggvÇny, mert rajzol†s kîzben keletkezhetnek
;hib†k, amelyek esetÇn mÇgsem rajzoljuk tele a puffert; ilyenkor megpr¢b†ljuk
;az egÇszet elìlrìl kezdeni (a BeginDraw h°v†s†val) £gy, hogy ugyanaz a puffer
;allok†l¢dik.

PUBLIC  _BeginDraw@0
PUBLIC  _BeginDraw
_BeginDraw@0:
_BeginDraw:

        pushad
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].TestCooperativeLevel7
        or      eax,eax
        mov     eax,NULL
        jne     BDnextbuffallocated
        mov     eax,lpDDSPrimary
        call    mvRestoreSurface
        mov     eax,NULL
        jc      BDnextbuffallocated
        mov     eax,lpDDZBuffer
        call    mvRestoreSurface
        mov     eax,NULL
        jc      BDnextbuffallocated
        mov     edi,mvMovie
        mov     edx,DrawnedNum
        inc     edx
        cmp     edx,[edi].cmiBuffNum
        jae     BDnextbuffallocated
        mov     eax,[edi].cmiFlags
        and     eax,MV_FULLSCR OR MV_VIDMEM
        cmp     eax,MV_FULLSCR OR MV_VIDMEM
        je      mvAllocHardwBuff

;------- Buffer allok†l†sa a pufferkÇszletbìl

mvAllocFromBuffSet:
        mov     eax,ToDrawBSoft
        mov     eax,[eax]
        jmp     BDnextbuffallocated

;------ Buffer allok†l†sa, amikor a pufferkÇszlet egyben flipping chain is
;       ( MV_VIDMEM + MV_FULLSCR )

mvAllocHardwBuff:
;hardveres pufferl†nc esetÇn m†s k¢d kell majd ide
        mov     eax,lpDDSBack
        cmp     DrawnedNum,0h
        jne     BDnobuffallocated
        push    eax
        push    LARGE DDGFS_ISFLIPDONE
        push    eax
        mov     eax,[eax]
        call    [eax].GetFlipStatus7
        or      eax,eax
        pop     eax
        je      BDnextbuffallocated
BDnobuffallocated:
        mov     eax,NULL
BDnextbuffallocated:
        mov     [esp+7*4],eax
        call    mvRestoreSurface
        popad
        ret

;----------------------------------------------------------------------------
;Ezzel a fÅggvÇnnyel jelezhetjÅk, hogy a BeginDraw-val allok†lt pufferbe
;sikeresen belerajzoltunk mindent, °gy az mehet a kÇpernyìre.

PUBLIC  _EndDraw@0
PUBLIC  _EndDraw
_EndDraw@0:
_EndDraw:
        mov     eax,mvMovie
        mov     eax,[eax].cmiFlags
        and     eax,MV_VIDMEM OR MV_FULLSCR
        cmp     eax,MV_VIDMEM OR MV_FULLSCR
        je      mvEDsetsemafor
        mov     eax,ToDrawBSoft
        mov     DrawnBSoft,eax
        call    mvGetNextBuffPSoft
        mov     ToDrawBSoft,eax
mvEDsetsemafor:
        inc     DrawnedNum
        mov     eax,TRUE
        ret

;----------------------------------------------------------------------------
;Aktu†lis rajzpuffer (surface) objektum†nak c°me
;Ez a fÅggvÇny teljesen ekvivalens a BeginDraw h°v†s†val (he he).
PUBLIC  _GABA@0
PUBLIC  _GABA
_GABA@0:
_GABA   PROC

        jmp     _BeginDraw

_GABA   ENDP
;----------------------------------------------------------------------------
;Az front buffer (az aktu†lis, megjelen°tÇs alatt †ll¢ buffer)
;objektum†nak c°mÇt adja vissza
PUBLIC  _GetFrontBuffer@0
PUBLIC  _GetFrontBuffer
_GetFrontBuffer@0:
_GetFrontBuffer:
        mov     eax,mvMovie
        test    [eax].cmiFlags,MV_WINDOWED
        jne     mvGFBwindowed
        mov     eax,lpDDSPrimary
        ret
mvGFBwindowed:
        mov     eax,ToDrawBSoft
        lea     eax,[eax-4]
        cmp     eax,OFFSET OFFSCREENBUFFS
        jae     mvGFBgot
        mov     eax,ENDOFFSCREENBUFFS
        lea     eax,[eax-4]
mvGFBgot:
        mov     eax,[eax]
        ret

;----------------------------------------------------------------------------
;A harmadik puffer objektum†t adja vissza (tripla- vagy tîbb puffer esetÇn)
PUBLIC  _GetThirdBuffer@0
PUBLIC  _GetThirdBuffer
_GetThirdBuffer@0:
_GetThirdBuffer:
        mov     eax,mvMovie
        test    [eax].cmiFlags,MV_WINDOWED
        jne     mvGTBwindowed
        mov     eax,lpDDSThird
        ret
mvGTBwindowed:
        mov     eax,ToDrawBSoft
        call    mvGetNextBuffPSoft
        mov     eax,[eax]      
        ret

;----------------------------------------------------------------------------
;Ezzel a fÅggvÇnnyel a frontbuffer tartalm†t jelen°thetjÅk meg, ha ablakos
;m¢dban futunk. Ez akkor j¢, amikor a frontbufferbe rajzolunk, Çs egybìl
;meg szeretnÇnk annak tartalm†t jelen°teni (mint ahogy fullscr-be menne a
;dolog)

PUBLIC  _ShowFrontBuffer@0
PUBLIC  _ShowFrontBuffer
_ShowFrontBuffer@0:
_ShowFrontBuffer:

        mov     eax,mvMovie
        test    [eax].cmiFlags,MV_WINDOWED
        je      mvSFBnotwindowed
        pushad
        mov     edx,lpDDSPrimary
        mov     ebx,DrawnBSoft
        call    mvDoBlit
        popad
mvSFBnotwindowed:
        ret
;----------------------------------------------------------------------------
;Ezzel a rutinnal kÇrdezhetjÅk le, hogy h†ny megrajzolt puffer v†r
;megjelen°tÇsre

PUBLIC  _GetBufferNumPending@0
PUBLIC  _GetBufferNumPending
_GetBufferNumPending@0:
_GetBufferNumPending:
        mov     eax,DrawnedNum
        ret
                              
;----------------------------------------------------------------------------
;Ez a rutin helyre†ll°t egy surface-t, ha szÅksÇges
;
;Be:    EAX     -       Helyre†ll°tand¢ surface c°me
;Ki:    EAX     -       TRUE: sikerÅlt

PUBLIC  mvRestoreSurface@4
PUBLIC  mvRestoreSurface

mvRestoreSurface@4:
mvRestoreSurface:
        pushad
        cmp     eax,NULL
        je      mvNULLsurface
        xchg    eax,ebx
        push    ebx
        mov     eax,[ebx]
        call    [eax].IsLost7
        cmp     eax,0 ;DD_OK
mvNULLsurface:
        mov     eax,TRUE
        je      mvSurfaceNotLost

        push    ebx
        mov     eax,[ebx]
        call    [eax].Restore7
        cmp     eax,0 ;DD_OK
        mov     eax,TRUE
;        je      mvSurfaceRestored
        je      mvSurfaceNotLost
        cmp     eax,8876024Bh
        mov     eax,FALSE
        jne     mvNotChangedMode
        call    _SetFullScreenMode
mvNotChangedMode:
mvSurfaceNotLost:
        mov     [esp+7*4],eax
        popad
        ret
mvSurfaceRestored:
        mov     eax,ebx
        call    mvLockSurface
        cmp     eax,FALSE
        mov     eax,TRUE
        je      mvSurfaceNotLost
        mov     edx,[edi].cmiYres
        mov     ecx,[edi].cmiXres
        shr     ecx,2h
        cld
        mov     ebp,[edi].cmiPitch
        mov     edi,[edi].cmiSurfaceMem
        xor     eax,eax
mvRSNullNextLine:
        push    ecx edi
        rep     stosd
        pop     edi ecx
        add     edi,ebp
        dec     edx
        jne     mvRSNullNextLine
        xchg    eax,ebx
        call    mvUnlockSurface
        mov     eax,TRUE
        jmp     mvSurfaceNotLost
        
;----------------------------------------------------------------------------
;Be:    EAX     -       Lockoland¢ surface c°me

mvLockSurface:
        pushad
        mov     ebx,eax
        mov     edi,mvMovie
;        push    OFFSET ddsd
;        push    ebx
;        mov     eax,[ebx]
;        call    [eax].GetSurfaceDesc7
;        or      eax,eax
;        mov     eax,FALSE
;        jne     BDnextbuffnotavail2
;        mov     eax,ddsd.lPitch2
;        mov     [edi].cmiPitch,eax

        push    LARGE NULL
        push    DDLOCK_SURFACEMEMORYPTR + DDLOCK_DONOTWAIT
        push    OFFSET ddsd
        push    LARGE NULL
        push    ebx
        mov     eax,[ebx]
        call    [eax].Lock7
        or      eax,eax
        mov     eax,FALSE
        jne     BDnextbuffnotavail2
        mov     eax,ddsd.lpSurface2
        mov     [edi].cmiSurfaceMem,eax
        mov     eax,ddsd.lPitch2
        mov     [edi].cmiPitch,eax
        mov     eax,TRUE
BDnextbuffnotavail2:
        mov     [esp+7*4],eax
        popad
        ret

;----------------------------------------------------------------------------
;Be:    EAX     -       Unlockoland¢ surface c°me

mvUnlockSurface:
        pushad
        push    LARGE NULL
        push    eax
        mov     eax,[eax]
        call    [eax].Unlock7
        popad
        ret

;----------------------------------------------------------------------------
;Timer-callback, ami a WM_TIMER-t post†zza a post†zand¢ ablaknak

MovieIRQ:
        pushad

        test    TrSemafor,semPaused
        jne     MIRQpaused
        mov     eax,mvMovie
        inc     [eax].cmiFrames
        test    TrSemafor,semBUSY
        jne     MIRQpaused

        mov     ecx,[eax].cmiCallWhenMIRQ
        jecxz   MI_nocallback
        call    ecx

MI_nocallback:
        push    LARGE 1h                        ;visszafut†st v†rja meg
        call    _CallConsumer

        mov     eax,mvMovie
        mov     ecx,[eax].cmiCallPostC
        jecxz   MI_nocallback2
        call    ecx
MI_nocallback2:

;        test    TrSemafor,semWMTIMER
;        jne     MIRQpaused
;        push    LARGE 0h
;        push    LARGE 0
;        push    LARGE WM_TIMER
;        push    [eax].cmiWinPost
;        W32     PostMessageA,4
;        cmp     eax,FALSE
;        je      MIRQpaused
;        or      TrSemafor,semWMTIMER

MIRQpaused:
        popad
        ret     20

;------------- Rajzolt ablak ¸zenetkezelıje

mvWindProc:
        push    ebp
        mov     ebp,esp
        push    ebx esi edi

        lea     esi,mvDefMessages
        mov     eax,uMsg
mvMSGdispatch:
        or      DWORD PTR [esi],0h
        je      mvCallDefProc
        cmp     eax,[esi]
        lea     esi,[esi+8]
        jne     mvMSGdispatch
        call    [esi-4]
        jnc     mvMessageHandled

mvCallDefProc:
        push    DWORD PTR lParam
        push    DWORD PTR wParam
        push    DWORD PTR uMsg
        push    DWORD PTR hwnd
        mov     ecx,mvMovie
        jecxz   mvNoChain
        or      [ecx].cmiWinProc,0h
        je      mvNoChain
        call    [ecx].cmiWinProc
        jmp     mvMessageHandled
mvNoChain:
        W32     DefWindowProcA,4        ;Åzenetek ablakon belÅli szÇtv†logat†sa
mvMessageHandled:
        pop     edi esi ebx ebp
        ret     16

;--------- WM_TIMER:  kÈpkirakÛ meghÌv·sa

mvWMTIMER:
        and     TrSemafor,NOT semWMTIMER
        push    LARGE 0h
        call    MovieConsumer
        xor     eax,eax
        ret

;--------- WM_SIZE:  kÈp ˙jrarajzol·sa

mvWMSIZED:
        mov     edi,mvMovie
        test    [edi].cmiFlags,MV_INITOK
        je      mvWMScreatingWin
        test    mvFlags,mviModeChanging
        jne     mvWMScreatingWin
        mov     eax,[wParam]
        xor     ebx,ebx
        cmp     eax,SIZE_MINIMIZED
        jne     mvVMSnotmin
        mov     ebx,MV_MINIMIZED
mvVMSnotmin:
        cmp     eax,SIZE_MAXIMIZED
        jne     mvVMSnotmax
        mov     ebx,MV_MAXIMIZED
mvVMSnotmax:
        and     [edi].cmiFlags,NOT (MV_MAXIMIZED+MV_MINIMIZED)
        or      [edi].cmiFlags,ebx
        movzx   eax,WORD PTR [lParam]
        mov     [edi].cmiWinSizeX,eax
        movzx   eax,WORD PTR [lParam+2]
        mov     [edi].cmiWinSizeY,eax
mvWMReblitPic:
        mov     edx,lpDDSPrimary
        or      edx,edx
        je      mvWMScreatingWin
        mov     ebx,DrawnBSoft
        call    mvDoBlit
mvWMScreatingWin:
        xor     eax,eax
        ret


;--------- WM_MOVE:  ablak koordin†t†inak feljegyzÇse

mvWMMOVED:
        mov     edi,mvMovie
        test    [edi].cmiFlags,MV_INITOK
        je      mvWMScreatingWin
        push    OFFSET DestRect
        push    DWORD PTR hwnd
        W32     GetWindowRect,2
        mov     eax,DestRect.rc_left
        mov     [edi].cmiWinPosX,eax
        mov     eax,DestRect.rc_top
        mov     [edi].cmiWinPosY,eax
        xor     eax,eax
        ret

;--------- WM_PAINT:  kÈp ˙jrarajzol·sa

mvWMPAINT:
        push    LARGE NULL
        push    DWORD PTR [hwnd]
        W32     ValidateRect,2
        jmp     mvWMReblitPic

;------ maga a consumer

MovieConsumer:
        or      TrSemafor,semBUSY
        or      DrawnedNum,0h
        je      mvNoDrawnBuffer
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].TestCooperativeLevel7
        or      eax,eax
        jne     mvNoDrawnBuffer
;        mov     esi,1h
        mov     edi,mvMovie
        mov     eax,[edi].cmiFlags
        and     eax,MV_FULLSCR OR MV_VIDMEM
        cmp     eax,MV_FULLSCR OR MV_VIDMEM
        je      mvFlipHardwareBuff

;------ Szoftveres puffer kim·sol·sa

;        mov     ebx,DrawnBSoft
        
        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvWindowedMode6

        mov     edx,lpDDSBack
        push    edx
        push    LARGE DDGFS_ISFLIPDONE 
        push    edx
        mov     eax,[edx]
        call    [eax].GetFlipStatus7
        pop     edx
        or      eax,eax
        jne     mvNoDrawnBuffer

mvWindowedMode6:
;        mov     eax,ebx
;        call    mvGetNextBuffPSoft
;        mov     DrawnBSoft,eax
;        mov     ebx,eax
;        push    ebx
        or      DWORD PTR [esp+4],0h
        je      mvCDontWaitVerticalBlank
        call    mvGetVBStatus
        jne     mvCDontWaitVerticalBlank
mvCWaitVerticalBlank:
        call    mvGetVBStatus
        je      mvCWaitVerticalBlank

mvCDontWaitVerticalBlank:
        mov     edx,lpDDSPrimary
        mov     ebx,DrawnBSoft
        call    mvDoBlit
        or      eax,eax
;        pop     eax
;;;;;;        jne     mvNoDrawnBuffer
;        call    mvGetNextBuffPSoft
;        mov     DrawnBSoft,eax
;        dec     DrawnedNum
;        xor     esi,esi
        
mvNoNewBufferSoft:
;-------------

mvFlipHardwareBuff:
        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvWindowedMode5

;        jmp     mvStillFlipping
        mov     ecx,[esp+4]
        push    eax
        mov     eax,lpDDSPrimary
        or      ecx,DDFLIP_WAIT
        push    ecx
;        push    LARGE DDFLIP_WAIT ;DDFLIP_DONOTWAIT
        push    LARGE NULL
        push    eax
        mov     eax,[eax]
        call    [eax].Flip7
        pop     ebx
        or      eax,eax
        jne     mvStillFlipping

mvWindowedMode5:
;        sub     DrawnedNum,esi
        dec     DrawnedNum
mvStillFlipping:
        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvCNoPalSetting

mvNoNewBuffer:
        test    TrSemafor,semSetPal
        je      mvCNoPalSetting

        push    OFFSET Palette
        push    LARGE 256
        push    LARGE 0h
        push    LARGE 0h
        mov     eax,lpDDPal
        push    eax
        mov     eax,[eax]
        call    [eax].SetEntries
        and     TrSemafor,NOT semSetPal
mvCNoPalSetting:
mvNoDrawnBuffer:
        and     TrSemafor,NOT semBUSY
        xor     eax,eax
        ret     4

mvGetNextBuffPSoft:
        lea     eax,[eax+4]
        cmp     eax,ENDOFFSCREENBUFFS
        jb      mvGNBPok
        lea     eax,OFFSCREENBUFFS
mvGNBPok:
        ret

mvDoBlit:
        mov     edi,mvMovie
        test    [edi].cmiFlags,MV_WINDOWED
        jne     mvWindowedMode7
        push    LARGE NULL
        push    LARGE 01000000h;  DDBLT_WAIT
        push    LARGE NULL
        push    DWORD PTR [ebx]
        push    LARGE NULL
        push    edx
        mov     eax,[edx]
        call    [eax].Blt7
        ret

mvWindowedMode7:
        push    edx
        xor     eax,eax
        mov     TempPoint.pt_y,eax
        mov     TempPoint.pt_x,eax
        push    OFFSET TempPoint
        push    [edi].cmiWinHandle
        W32     ClientToScreen,2
        push    OFFSET DestRect
        push    [edi].cmiWinHandle
        W32     GetClientRect,2
                mov ecx,[edi].cmiPicJX
                or  ecx,[edi].cmiPicJY
                je  mvNoSizeRestriction
                mov eax,[edi].cmiPicBX
                add DestRect.rc_left,eax
                add eax,[edi].cmiPicJX
                mov DestRect.rc_right,eax
                mov eax,[edi].cmiPicBY
                add DestRect.rc_top,eax
                add eax,[edi].cmiPicJY
                mov DestRect.rc_bottom,eax
mvNoSizeRestriction:
        push    TempPoint.pt_y
        push    TempPoint.pt_x
        push    OFFSET DestRect
        W32     OffsetRect,3

        pop     edx
        push    LARGE NULL
        push    LARGE 01000000h;  DDBLT_WAIT
        push    LARGE NULL ;OFFSET SrcRect
        push    DWORD PTR [ebx]
        push    OFFSET DestRect
        push    edx
        mov     eax,[edx]
        call    [eax].Blt7
        ret

mvGetVBStatus:
        push    eax
        mov     eax,esp
        push    eax
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].GetVerticalBlankStatus7
        pop     eax
        or      eax,eax
        ret

PUBLIC  _CallConsumer@4
PUBLIC  _CallConsumer
_CallConsumer@4:
_CallConsumer:
        push    ebx esi edi
        and     TrSemafor,NOT semWMTIMER
        xor     eax,eax
        or      DWORD PTR [esp+4 +3*4],0h
        jnz     _ccpok
        mov     eax,DDFLIP_NOVSYNC
_ccpok: push    eax
        call    MovieConsumer
        xor     eax,eax
        pop     edi esi ebx
        ret     4


IF      MOVIE3D  EQ  MV_YES

;-------------- ZBuffer lÇtrehoz†sa
;Be:    cmiZBitDepth:   max. ennyi bit mÇlysÇgñ Z-puffer lÇtrehoz†sa
;                       MVZBUFF_AUTO: automatikus v†laszt†s a Z-pufferre

PUBLIC  _CreateZBuffer@0
PUBLIC  _CreateZBuffer
_CreateZBuffer@0:

_CreateZBuffer  PROC

        pushad
        mov     edi,mvMovie

;------ Egy†ltal†n lÇtrehozt†k a Direct3D interfÇszt?

mvCZBretry:
        xor     eax,eax
        test    [edi].cmiFlags,MV_3D
        je      mvEZnoZBuff
        cmp     lpDDZBuffer,NULL
        jne     mvEZnoZBuff

        xor     ebp,ebp
        mov     eax,NULL
        lea     ebx,ddcaps
        lea     esi,[edi].cmiDeviceGUID
;        mov     esi,[edi].cmiDeviceGUID
        call    mvIsHardwareDevice
        je      mvZBhardware
        inc     ebp
        xchg    eax,ebx
mvZBhardware:

        mov     ddcaps.dwSize, SIZE DDCAPS
        push    eax ebx
        mov     eax,lpDD
        push    eax
        mov     eax,[eax]
        call    [eax].GetCaps7

;------ Az eszkîz egy†ltal†n t†mogatja a Z-puffert?
        
        test    ddcaps.ddsCapsDDC.dwCapsDC2,DDSCAPS_ZBUFFER
        push    LARGE 0h
        pop     eax
        je      mvEZnoZBuff

;------ Az adott kîrÅlmÇnyek kîzîtt a legjobb form†tum£ Z-puffer kiv†laszt†sa

mvCZPSelectNewFormat:
        cmp     [edi].cmiZBitDepth,MVZBUFF_AUTO
        jne     mvEZnoauto
        mov     [edi].cmiZBitDepth,32
mvEZnoauto:
        mov     mvZtemp,0h
        mov     mvAtemp,0h
        push    LARGE 0h
        push    OFFSET mvEZcallback
        lea     eax,[edi].cmiDeviceGUID
        push    eax
;        push    LARGE [edi].cmiDeviceGUID
        mov     eax,lpD3D
        push    eax
        mov     eax,[eax]
        call    [eax].EnumZBufferFormats7
        xor     eax,eax
        or      mvZtemp,0h
        je      mvEZnoZBuff

        mov     ddsd.dwSize2, SIZE DDSURFACEDESC2
        mov     ddsd.dwFlagsSD2,DDSD_WIDTH OR DDSD_HEIGHT OR DDSD_PIXELFORMAT OR DDSD_CAPS
        mov     eax,[edi].cmiXres
        mov     ddsd.dwWidth2,eax
        mov     eax,[edi].cmiYres
        mov     ddsd.dwHeight2,eax
        xor     eax,eax
        mov     ddsd.ddsCaps2.dwCapsDC22,eax
        mov     ddsd.ddsCaps2.dwCapsDC32,eax
        mov     ddsd.ddsCaps2.dwCapsDC42,eax
        mov     eax,DDSCAPS_VIDEOMEMORY
        or      ebp,ebp
        je      mvZBcapsok
        mov     eax,DDSCAPS_SYSTEMMEMORY
mvZBcapsok:
        or      eax,DDSCAPS_ZBUFFER
        mov     ddsd.ddsCaps2.dwCapsDC2,eax
        mov     eax,lpDD
        push    LARGE NULL
        push    OFFSET lpDDZBuffer
        push    OFFSET ddsd
        push    eax
        mov     eax,[eax]
        call    [eax].CreateSurface7
        or      eax,eax
        push    LARGE 0h
        pop     eax 
        je      mvEZattach
        cmp     [edi].cmiZBitDepth,16
        je      mvEZnoZBuff
        mov     [edi].cmiZBitDepth,16
        jmp     mvCZPSelectNewFormat

mvEZattach:
;------ Z-buffer csatol†sa        

        mov     edx,AddAttachedSurface7
        call    mvAttDetZbuffer
        mov     ecx,mvZtemp
        mov     [edi].cmiZBitDepth,ecx
        or      eax,eax
        mov     eax,lpDDZBuffer
        je      mvCZBnoerror
        call    _DestroyZBuffer
        xor     eax,eax
        mov     ebx,[edi].cmiBitPP
        cmp     ebx,[edi].cmiZBitDepth
        je      mvCZBnoerror
        mov     [edi].cmiZBitDepth,ebx
        jmp     mvCZBretry
mvEZnoZBuff:
        push    eax
        call    _DestroyZBuffer
        pop     eax
mvCZBnoerror:
        mov     [esp+7*4],eax
        popad
        ret

mvEZcallback:
        push    esi edi
        mov     esi,[esp+4+8]
        mov     edx,mvMovie
        test    [edx].cmiFlags,MV_STENCIL
        je      mvEZDontStudyStencilBuffs
        test    [esi].dwFlagsPF,DDPF_ZBUFFER OR DDPF_STENCILBUFFER
        je      mvEZCcontinue
        jmp     mvEZCheckIt
mvEZDontStudyStencilBuffs:
        cmp     [esi].dwFlagsPF,DDPF_ZBUFFER
        jne     mvEZCcontinue
mvEZCheckIt:
        mov     eax,[esi].dwZBufferBitDepthPF
        cmp     eax,[edx].cmiZBitDepth
        ja      mvEZCcontinue
        cmp     eax,mvZtemp
        jb      mvEZCcontinue
        je      mvEZDontResetStencilDepth
        mov     [edx].cmiStencilBitDepth,0FFFFFFFFh
mvEZDontResetStencilDepth:
        mov     ecx,[esi].dwStencilBitDepth
        cmp     ecx,[edx].cmiStencilBitDepth
        jbe     mvEZIsitZero
        test    [edx].cmiFlags,MV_STENCIL
        je      mvEZCcontinue
        or      [edx].cmiStencilBitDepth,0h
        jne     mvEZCcontinue
mvEZIsitZero:
        test    [edx].cmiFlags,MV_STENCIL
        je      mvEZStoreFormat
        or      ecx,ecx
        jne     mvEZStoreFormat
        cmp     [edx].cmiStencilBitDepth,0FFFFFFFFh
        jne     mvEZCcontinue
mvEZStoreFormat:
        mov     mvZtemp,eax
        mov     [edx].cmiStencilBitDepth,ecx
        cld
        mov     ecx,SIZE DDPIXELFORMAT
        lea     edi,ddsd.ddpfPixelFormat
        rep     movsb
mvEZCcontinue:
        mov     eax,D3DENUMRET_OK
        pop     edi esi
        ret     8
        
_CreateZBuffer  ENDP

PUBLIC  _DestroyZBuffer@0
PUBLIC  _DestroyZBuffer
_DestroyZBuffer@0:
_DestroyZBuffer PROC

        push    esi edi ebx
        mov     eax,lpDDZBuffer
        cmp     eax,NULL
        xchg    eax,ebx
        je      mvDZBnoZBuffer
        mov     edx,DeleteAttachedSurface7
        mov     edi,mvMovie
        call    mvAttDetZbuffer
;        or      eax,eax
;        jne     mvDZBnoZBuffer
        push    ebx
        mov     eax,[ebx]
        call    [eax].Release
        mov     lpDDZBuffer,NULL
mvDZBnoZBuffer:
        pop     ebx edi esi
        ret

_DestroyZBuffer ENDP

mvAttDetZbuffer:
        cld
        mov     ecx,[edi].cmiBuffNum
        lea     esi,OFFSCREENBUFFS
        mov     eax,[edi].cmiFlags
        and     eax,MV_FULLSCR+MV_VIDMEM
        cmp     eax,MV_FULLSCR+MV_VIDMEM
        jne     mvEZattachnext
;        push    LARGE 1h
;        pop     ecx
        lea     esi,lpDDSPrimary
;        lea     esi,lpDDSBack
mvEZattachnext:
        lodsd
        push    ecx edx
        push    [lpDDZBuffer]
        cmp     edx,DeleteAttachedSurface7
        jne     mvEZnoflags
        push    LARGE 0h
mvEZnoflags:
        push    eax
        mov     eax,[eax]
        call    [eax+edx]
        pop     edx ecx
        or      eax,eax
        jne     mvEZcantattach
        loopd   mvEZattachnext
mvEZcantattach:
        ret

mvIsHardwareDevice:
        push    edi esi
        lea     edi,IID_IDirect3DHALDevice
        call    mvCompareGUIDs
        pop     esi edi
        je      mvIsHDyes
        push    edi
        lea     edi,IID_IDirect3DTnLHalDevice
        call    mvCompareGUIDs
        pop     edi
mvIsHDyes:
        ret
        
ENDIF

mvCompareGUIDs:
        push    ecx
        mov     ecx,SIZE GUID
        cld
        repe    cmpsb
        pop     ecx
        ret
        
;----------------------------------------------------------------------------

.data

DrawnBSoft      DD      OFFSET OFFSCREENBUFFS
                                        ;MutatÛ a l·thatÛ pufferobjektum
                                        ;interfÈszÈre (szoftveres puffer)
ToDrawBSoft     DD      OFFSET OFFSCREENBUFFS
                                        ;MutatÛ a rajzolt puffer interfÈszÈre
                                        ;(szoftveres puffer)
mvMovie         DD      NULL            ;Pointer a movie strukt˙r·ra
SrcRect         RECT    <0,0,0,0>
TempPoint       POINT   <>
DestRect        RECT    <>

mvDirectDrawDLL DB      'DDRAW.DLL',0h
mvWinMMDLL      DB      'WINMM.DLL',0h
mvFlags         DB      0h              ;Movie internal flags

mvDDrawInited   DB      0h
DirectDrawFunctions     LABEL
                DB      'DirectDrawCreateEx',0h
                DB      'DirectDrawCreateClipper',0h
                DB      'DirectDrawEnumerateA',0h
                DB      0h
WinMMFunctions  LABEL
                DB      'timeSetEvent',0h
                DB      'timeKillEvent',0h
                DB      0h

mvWinClass      WNDCLASS  <0,OFFSET mvWindProc,0,0,?,?,?,?,NULL,OFFSET mvWinClassName>
mvWinClassName  DB      'DEGEMOVIE',0h
mvDefMessages   LABEL
                DD      WM_PAINT, OFFSET mvWMPAINT
                DD      WM_SIZE, OFFSET mvWMSIZED
                DD      WM_MOVE, OFFSET mvWMMOVED
;                DD      WM_TIMER, OFFSET mvWMTIMER
                DD      0h

lpDD            DD      NULL            ;Pointer a DirectDraw interfÈszre
lpD3D           DD      NULL            ;Pointer a Direct3D interfÈszre
lpDDPal         DD      NULL            ;Pointer a paletta interfÈszre
lpDDClipper     DD      NULL            ;Pointer az elsıdleges puffer
                                        ;v·gÛobjektum·ra (ablakos mÛdban)

;Ezen h†rom v†ltoz¢nak egym†s ut†n kell szerepelnie!
lpDDSPrimary    DD      NULL            ;Pointer az elsìdleges pufferre
lpDDSBack       DD      NULL            ;Pointer a backbufferre
lpDDSThird      DD      NULL            ;Pointer a harmadik bufferre (ha van)
lpDDZBuffer     DD      NULL            ;Pointer a Z-bufferre
DrawnedNum      DD      0

IID_IDirectDraw7 GUID <15e65ec0h,3b9ch,11d2h,0b9h,2fh,00,60h,97h,97h,0eah,5bh>
IID_IDirect3D7   GUID <0f5049e77h,4861h,11d2h,0a4h,07h,0h,0a0h,0c9h,06h,29h,0a8h>
IID_IDirect3DHALDevice  LABEL
                 GUID <84E63dE0h,46AAh,11CFh,81h,6Fh,00h,00h,0C0h,20h,15h,6Eh>
IID_IDirect3DTnLHalDevice LABEL
                 GUID <0f5049e78h,4861h,11d2h,0a4h,7h,0h,0a0h,0c9h,6h,29h,0a8h>

.data?

OFFSCREENBUFFS LABEL
mvBuffers       DD      MaxBuffNum DUP(?)  ;szoftver-renderelt pufferek

ENDOFFSCREENBUFFS  DD      ?

ddsd            DDSURFACEDESC2 <>       ;pufferleÌrÛ strukt˙ra
ddsdalpha       DDSURFACEDESC2 <>       ;pufferleÌrÛ strukt˙ra az alpha elj·r·shoz
ddscaps         DDSCAPS2 <>             ;puffer tulajdons·gait leÌrÛ strukt˙ra
ddspixelf       DDPIXELFORMAT <>        ;puffer pixelform·tumot leÌrÛ struc
ddcaps          DDCAPS <>               ;eszkˆz kÈpessÈgeit leÌrÛ struc
ddbltfx         DDBLTFX       <>        ;info strukt. a blittelÈshez

Palette         DB      256*4 DUP(?)    ;Paletta szÌnei
TrSemafor       DB      ?

DDERR           DD      ?
BuffPitch       DD      ?
mvIRQTime       DD      ?
mvIRQtimer      DD      ?
mvWINMMLibHandle DD     ?
mvDDLibHandle   DD      ?
;mvLibHandle     DD      ?
;mvOldWinProc    DD      ?

mvScrX          DD      ?
mvScrY          DD      ?
mvScrBPP        DD      ?
mvZtemp         DD      ?
mvAtemp         DD      ?
mvHDC           DD      ?
mvDDHDC         DD      ?

DirectDrawAddresses     LABEL
DirectDrawCreateEx      DD      ?
DirectDrawCreateClipper DD      ?
DirectDrawEnumerate     DD      ?

WinMMAddresses  LABEL
timeSetEvent    DD      ?
timeKillEvent   DD      ?

IF      (MOVIETYPE  EQ  STANDALONE_OMF) OR (MOVIETYPE  EQ  NEEDEDONLY_OMF)

END

ENDIF
