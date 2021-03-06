;-----------------------------------------------------------------------------
; DOS.INC - Some extract from dgVoodooConfig and DOS.H for the assembly
;
; Copyright (C) 2003 Dege
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

;------------------------------------------------------------------------------------------
; Defin�ci�k, amelyek a Dos-os �s Windows-os DLL-ben is kellenek
;------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------
;A wrapper konfigur�ci�j�t le�r� strukt�ra
dgVoodooConfig  STRUC

ProductVersion  DW	?
GlideVersion    DW	?
TexMemSize      DD      ?
Flags           DD      ?
Gamma           DD      ?
Version         DD      ?
VideoMemSize_c  DD      ?
VideoRefreshFreq DD     ?
dxres           DW      ?
dyres           DW      ?
dispdev         DW      ?
dispdriver      DW      ?
refreshRate		DD		?
flags2			DD		?
dosTimerBoostPeriod DW	?
alphaCKRefValue DB		?
language		DB		?
rendererApi		DB		?
reserved_c      DB      3 DUP(?)

dgVoodooConfig  ENDS

;------------------------------------------------------------------------------------------
; Alap�rtelmezett �rt�kek a config strukt�ra mez�ihez
MAXTEXNUM_D     EQU     2048            ;Text�r�k max. sz�ma
TEXTUREMEMSIZE  EQU     4*1024*1024     ;Voodoo-text�ramem m�rete

;------------------------------------------------------------------------------------------
; Konstansok a konfig strukt�ra flagjeihez
CFG_WBUFFER				EQU		1h			;W-buffert mindig Z-bufferen kereszt�l emul�l
CFG_RELYONDRIVERFLAGS	EQU		CFG_WBUFFER
CFG_WINDOWED			EQU		2h			;Ablakos m�d
CFG_DISPMODE32BIT		EQU		4h			;32 bites videom�d
CFG_SETREFRESHRATE		EQU		8h			;A friss�t�si freq-t az alkalmaz�s
											;	�ll�tja be (egy�bk�nt az akt. freq)
CFG_PERFECTTEXMEM		EQU		10h			;	T�k�letes text�ramem�ria-emul�ci�
CFG_WBUFFERMETHODISFORCED EQU	80h			;	A W-bufferel�s m�dszere k�nyszer�tett:
											;		- CFG_WBUFFER flag set: mindig val�di w-bufferel�s
											;		- CFG_WBUFFER flag not set: mindig emul�lt w-bufferel�s
											;						vagy a W-bufferel�s m�dszere detekt�lt, ekkor
											;		- CFG_RELYONDRIVERFLAGS set: a driver �ltal a W-bufferel�s t�mogat�s�ra
											;								 visszaadott flag alapj�n d�nt
CFG_VESAENABLE          EQU     100h        ;Be�p�tett VESA-t�mogat�s enged�lyezve
CFG_SETMOUSEFOCUS       EQU     200h        ;Eg�rf�kusz az alkalmaz�sn�l
CFG_SUPPORTMODE13H      EQU     400h        ;320x200x256 VGA-m�d t�mogat�sa
CFG_TEXTURES32BIT       EQU     800h        ;mindig 32 bites text�r�k
CFG_TEXTURES16BIT       EQU     1000h       ;mindig 16 bites text�r�k
CFG_TEXTURERELOAD       EQU     2000h       ;Text�r�k �jrat�lt�se, ha kell
CFG_GRABENABLE          EQU     4000h       ;K�perny�lop�s enged�lyezve
CFG_GRABCLIPBOARD       EQU     8000h       ;K�plop�s a v�g�lapra
CFG_CKMGEFORCE          EQU     10000h      ;Colorkeying GeForce-ra (2-es m�dszer)
CFG_TOMBRAIDER          EQU     20000h      ;TR1 �rny�khiba-jav�t�s
CFG_HUNGARIAN           EQU     40000h      ;A nyelv magyar
CFG_HWLFBCACHING        EQU     80000h      ;Hardveres gyors�t�pufferek az LFB-hez
CFG_LFBREALVOODOO       EQU     100000h     ;K�zelebb az igazi hardverhez
CFG_TRIPLEBUFFER        EQU     200000h     ;Mindig triplapufferel�s
CFG_ACTIVEBACKGROUND    EQU     400000h     ;A h�tt�ren is akt�v a DOS-os prg
CFG_LFBFASTUNMATCHWRITE EQU     800000h     ;Gyors�r�s a nem egyez� LFB form�tumokhoz
CFG_CTRLALT             EQU     1000000h    ;Eg�rf�kusz elenged�se Ctrl-Alt-ra
CFG_VSYNC               EQU     2000000h    ;F�gg�leges visszafut�st mindig megv�rni
CFG_NTVDDMODE           EQU     4000000h    ;a wrapper VDD-m�dban m�k�dik (XP-hez)
CFG_DISABLEMIPMAPPING   EQU     8000000h    ;Mipmapping tilt�sa
CFG_ENABLEGAMMARAMP     EQU     10000000h   ;Glide-gammaramp enged�lyez�se
CFG_ENABLEHWVBUFFER     EQU     20000000h   ;Hardveres vertex pufferek haszn�lata
CFG_BILINEARFILTER      EQU     40000000h   ;Mindig biline�ris sz�r�s a text�r�khoz
CFG_CLIPPINGBYD3D       EQU     80000000h   ;V�g�s a Direct3D-n kereszt�l

CFG2_CLOSESTREFRESHFREQ EQU     1h
CFG_FORCECLIPOPF        EQU     2h          ;CLI/POPF megfelel� kezel�se - rejtett opci�
CFG_LFBRESCALECHANGESONLY EQU   4h          ;elt�r� felbont�sok eset�n csak a v�ltoz�sok �tm�retez�se,
                                            ; �s nem az eg�sz k�p� (gyors�r�st ig�nyel) - rejtett opci�
CFG_LFBFORCEFASTWRITE   EQU   8h            ;Gyors�r�s haszn�lata egyez� form�tumokhoz is - rejtett opci�
CFG_YMIRROR             EQU   10h           ;K�perny� f�gg�leges t�kr�z�se (orig�-tesztekhez) - rejtett opci�
CFG_FORCETRILINEARMIPMAPS EQU   20h         ;Triline�ris mipmapping k�nyszer�t�se - rejtett opci�
CFG_PREFERTMUWIFPOSSIBLE  EQU   40h         ;t�bl�s k�d, z-buffer �s TMUDIFFW hint eset�n a tmu w-t
CFG_LFBALWAYSFLUSHWRITES  EQU	80h	    ;A r�gi lfb-unlock dude-m�dja: mindig ki�rja a v�ltoz�sokat
					    ;(nem cache-el) (kompatibilit�si �s teszt-opci�, rejtett)
CFG_DEPTHEQUALTOCOLOR	EQU	100h	    ;A k�ppuffer �s a m�lys�gi puffer bitm�lys�ge legyen mindig egyenl�								    ;(kompatibilit�si �s teszt-opci�, rejtett)
CFG_DISABLELFBREAD	EQU	200h	    ;LFB el�r�s�nek tilt�sa olvas�sra
CFG_DISABLELFBWRITE	EQU	400h	    ;LFB el�r�s�nek tilt�sa �r�sra
CFG_AUTOGENERATEMIPMAPS	EQU	800h	    ;A hi�nyz� mipmap-szintek automatikus gener�l�sa
CFG_LFBNOMATCHINGFORMATS  EQU	1000h	    ;Az LFB-form�tumok mindig k�l�nb�z�nek tekintend�k - rejtett opci�
CFG_ALWAYSINDEXEDPRIMS	EQU	2000h		
CFG_HIDECONSOLE		EQU	4000h	    ;Konzol elrejt�se (DOS)
CFG_LFBENABLETEXTURING	EQU	8000h
CFG_CFORMATAFFECTLFB	EQU	10000h
CFG_DISABLEVESAFLATLFB	EQU	20000h


;A konfig flagekben lev� bitmez�k �rt�kei
CFG_CKMAUTOMATIC	EQU	00
CFG_CKMALPHABASED	EQU	01
CFG_CKMNATIVE		EQU	02
CFG_CKMNATIVETNT	EQU	03

;------------------------------------------------------------------------------------------
;Az egyes platformok k�djai
PLATFORM_WINDOWS          EQU   0
PLATFORM_DOSWIN9X         EQU   1
PLATFORM_DOSWINNT         EQU   2

;------------------------------------------------------------------------------------------
; Az aktu�lis vide�m�d �s annak pixelform�tuma (VESA-hoz)

dgVoodooModeInfo        STRUC

modeIndex				DD		?
ModeNumber              DW      ?
XRes                    DW      ?
YRes                    DW      ?
BitPP                   DB      ?
RedSize                 DB      ?
RedPos                  DB      ?
GreenSize               DB      ?
GreenPos                DB      ?
BlueSize                DB      ?
BluePos                 DB      ?
RsvSize                 DB      ?
RsvPos                  DB      ?
                        DB      ?

dgVoodooModeInfo        ENDS

;------------------------------------------------------------------------------------------
; Eg�rkurzor-inf�: jelenleg haszn�laton k�v�li */
MouseInfo       STRUC

mi_x            DD      ?
mi_y            DD      ?
brecttop_x      DD      ?
brecttop_y      DD      ?
brectbottom_x   DD      ?
brectbottom_y   DD      ?

MouseInfo       ENDS

;------------------------------------------------------------------------------------------

EXEBUFFSIZE     EQU     ((32*1024)/4)
FUNCDATASIZE    EQU     1536*1024

KFLAG_INKERNEL  	EQU     1h
KFLAG_VMWINDOWED	EQU	2h
KFLAG_SCREENSHOT        EQU     4h
KFLAG_GLIDEACTIVE       EQU     8h
KFLAG_SERVERWORKING	EQU	20h
KFLAG_CTRLALT		EQU	80h
KFLAG_PALETTECHANGED	EQU	100h
KFLAG_VESAFRESHDISABLED EQU	200h

AREAID_COMMAREA                 EQU     1
AREAID_REGSTRUCTURE_GLIDE211    EQU     2
AREAID_REGSTRUCTURE_GLIDE243    EQU     3

; A kommunik�ci�s ter�let fel�p�t�se (ez van az osztott mem�riater�leten)
CommArea        STRUC

areaId          DD      ?
kernelflag      DD      ?
ExeCodeIndex    DD      ?
FDPtr           DD      ?
progname        DB      128 DUP(?)
WinOldApHWND    DD      ?
VESAPalette     DD      256 DUP(?)
VESASession     DB      ?
VGAStateRegister3DA     DB      ?
VGARAMDACSize   DB      ?
                DB      ?
VideoMemory     DD      ?
videoMemoryClient DD	?
BytesPerScanLine DD     ?
DisplayOffset   DD      ?
actmodeinfo     dgVoodooModeInfo <>
vesaModeinfo    dgVoodooModeInfo <>
;mouseinfo       MouseInfo <>

ExeCodes        DD      EXEBUFFSIZE DUP(?)
FuncData        DB      FUNCDATASIZE DUP(?)

;LFB		DB	0 DUP(?)

CommArea        ENDS

;------------------------------------------------------------------------------------------
ServerRegInfo   STRUC

areaId_regs     DD      ?
workerthread_   DD      ?
commaptr        DD      ?
VideoMemSize    DD      ?
ConfigPtr       DD      ?
ErrorCode       DD      ?

ServerRegInfo   ENDS


;------------------------------------------------------------------------------------------
; Az egyes Glide-f�ggv�nyek k�djai

GRGLIDEINIT                     EQU             0
GRSSTWINOPEN                    EQU             1
GRSSTWINCLOSE                   EQU             2
GRGLIDESHUTDOWN                 EQU             3

GRBUFFERCLEAR                   EQU             4
GRBUFFERSWAP                    EQU             5

GUDRAWTRIANGLEWITHCLIP          EQU             6
GUAADRAWTRIANGLEWITHCLIP        EQU             7
GUDRAWPOLYGONVERTEXLISTWITHCLIP EQU             8
GRDRAWTRIANGLE                  EQU             9
GRDRAWPOLYGONVERTEXLIST         EQU             10
GRAADRAWPOLYGON                 EQU             11
GRAADRAWPOLYGONVERTEXLIST       EQU             12
GRAADRAWTRIANGLE                EQU             13
GRDRAWPLANARPOLYGON             EQU             14
GRDRAWPLANARPOLYGONVERTEXLIST   EQU             15
GRDRAWPOLYGON                   EQU             16
GRAADRAWLINE                    EQU             17
GRDRAWLINE                      EQU             18
GRAADRAWPOINT                   EQU             19
GRDRAWPOINT                     EQU             20


GRTEXDOWNLOADMIPMAP             EQU             21
GRTEXDOWNLOADMIPMAPLEVEL        EQU             22
GRTEXDOWNLOADMIPMAPLEVELPARTIAL EQU             23
GRTEXDOWNLOADTABLE              EQU             24
GRTEXDOWNLOADTABLEPARTIAL       EQU             25
GRTEXNCCTABLE                   EQU             26
GRTEXSOURCE                     EQU             27
GRTEXMIPMAPMODE                 EQU             28
GRTEXCOMBINE                    EQU             29
GRTEXFILTERMODE                 EQU             30
GRTEXCLAMPMODE                  EQU             31
GRTEXLODBIASVALUE               EQU             32
GRTEXMULTIBASE                  EQU             33
GRTEXMULTIBASEADDRESS           EQU             34

GUTEXALLOCATEMEMORY             EQU             35
GUTEXCHANGEATTRIBUTES           EQU             36
GUTEXCOMBINEFUNCTION            EQU             37
GUTEXDOWNLOADMIPMAP             EQU             38
GUTEXDOWNLOADMIPMAPLEVEL        EQU             39
GUTEXGETCURRENTMIPMAP           EQU             40
GUTEXGETMIPMAPINFO              EQU             41
GUTEXMEMQUERYAVAIL              EQU             42
GUTEXMEMRESET                   EQU             43
GUTEXSOURCE                     EQU             44
GU3DFGETINFO                    EQU             45
GU3DFLOAD                       EQU             46
GLIDEGETUTEXTURESIZE            EQU             47

GRCOLORCOMBINE                  EQU             48
GUCOLORCOMBINEFUNCTION          EQU             49
GRCONSTANTCOLORVALUE4           EQU             50
GRCOLORMASK                     EQU             51
GRCONSTANTCOLORVALUE            EQU             52
                                        
GRALPHABLENDFUNCTION            EQU             53
GRALPHACOMBINE                  EQU             54
GRALPHACONTROLSITRGBLIGHTING	EQU				55
GUALPHASOURCE                   EQU             56
GRALPHATESTFUNCTION             EQU             57
GRALPHATESTREFERENCEVALUE       EQU             58

GRFOGCOLORVALUE                 EQU             59
GRFOGMODE                       EQU             60
GRFOGTABLE                      EQU             61
GLIDEGETINDTOWTABLE             EQU             62
GUFOGTABLEINDEXTOW              EQU             63
GUFOGGENERATEEXP                EQU             64
GUFOGGENERATEEXP2               EQU             65
GUFOGGENERATELINEAR             EQU             66

GRRENDERBUFFER                  EQU             67
GRSSTORIGIN                     EQU             68
GRCLIPWINDOW                    EQU             69
GRCULLMODE                      EQU             70
GRDISABLEALLEFFECTS             EQU             71
GRDITHERMODE                    EQU             72
GRGAMMACORRECTIONVALUE          EQU             73
GRGLIDEGETSTATE                 EQU             74
GRGLIDESETSTATE                 EQU             75
GRHINTS                         EQU             76

GRSSTSCREENWIDTH                EQU             77
GRSSTSCREENHEIGHT               EQU             78
GRSSTSTATUS                     EQU             79
GRSSTVIDEOLINE                  EQU             80
GRSSTVRETRACEON                 EQU             81
GRSSTCONTROLMODE                EQU             82
GRBUFFERNUMPENDING              EQU             83
GRRESETTRISTATS                 EQU             84
GRTRISTATS                      EQU             85
                                        
GRDEPTHBUFFERMODE               EQU             86
GRDEPTHBUFFERFUNCTION           EQU             87
GRDEPTHMASK                     EQU             88
GRLFBCONSTANTALPHA              EQU             89
GRLFBREADREGION                 EQU             90
GRLFBWRITEREGION                EQU             91
GRDEPTHBIASLEVEL                EQU             92
GLIDEGETCONVBUFFXRES            EQU             93

GRCHROMAKEYMODE                 EQU             94
GRCHROMAKEYVALUE                EQU             95

GRLFBLOCK                       EQU             96
GRLFBUNLOCK                     EQU             97
GRLFBWRITECOLORFORMAT           EQU             98
GRLFBWRITECOLORSWIZZLE          EQU             99
GLIDESETUPLFBDOSBUFFERS         EQU             100

;Glide1-f�ggv�nyek
GRSSTOPEN                       EQU             101
GRLFBBYPASSMODE                 EQU             102
GRLFBORIGIN                     EQU             103
GRLFBWRITEMODE                  EQU             104
GRLFBBEGIN                      EQU             105
GRLFBEND                        EQU             106
GRLFBGETREADPTR                 EQU             107
GRLFBGETWRITEPTR                EQU             108
GUFBREADREGION                  EQU             109
GUFBWRITEREGION                 EQU             110


;VESA-f�ggv�nyek
VESASETVBEMODE                  EQU             111
VESAUNSETVBEMODE                EQU             112
VESAFRESH                       EQU             113

;Ezeket a f�ggv�nyeket a kernelmodul hivja
DGVOODOOCLIENTCRASHED           EQU             114
DGVOODOONEWCLIENTREGISTERING    EQU             115

;Egy�b f�ggv�nyek
DGVOODOOGETCONFIG               EQU             116
DGVOODOORELEASEFOCUS            EQU             117

GLIDEINSTALLMOUSE				EQU				118
GLIDEUNINSTALLMOUSE				EQU				119
GLIDEISMOUSEINSTALLED			EQU				120

VESAGETXRES						EQU				121
VESAGETYRES						EQU				122

PORTLOG_IN						EQU				123
PORTLOG_OUT						EQU				124

GLIDEENDITEM                    EQU             0FFFFFFFFh
