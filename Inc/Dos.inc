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
; DefinÌciÛk, amelyek a Dos-os Ès Windows-os DLL-ben is kellenek
;------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------
;A wrapper konfigur·ciÛj·t leÌrÛ strukt˙ra
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
reserved_c      DD      4 DUP(?)

dgVoodooConfig  ENDS

;------------------------------------------------------------------------------------------
; AlapÈrtelmezett ÈrtÈkek a config strukt˙ra mezÙihez
MAXTEXNUM_D     EQU     2048            ;Text˙r·k max. sz·ma
TEXTUREMEMSIZE  EQU     4*1024*1024     ;Voodoo-text˙ramem mÈrete

;------------------------------------------------------------------------------------------
; Konstansok a konfig strukt˙ra flagjeihez
CFG_WBUFFER				EQU		1h			;W-buffert mindig Z-bufferen kereszt¸l emul·l
CFG_RELYONDRIVERFLAGS	EQU		CFG_WBUFFER
CFG_WINDOWED			EQU		2h			;Ablakos mÛd
CFG_DISPMODE32BIT		EQU		4h			;32 bites videomÛd
CFG_SETREFRESHRATE		EQU		8h			;A frissÌtÈsi freq-t az alkalmaz·s
											;	·llÌtja be (egyÈbkÈnt az akt. freq)
CFG_PERFECTTEXMEM		EQU		10h			;	TˆkÈletes text˙ramemÛria-emul·ciÛ
CFG_WBUFFERMETHODISFORCED EQU	80h			;	A W-bufferelÈs mÛdszere kÈnyszerÌtett:
											;		- CFG_WBUFFER flag set: mindig valÛdi w-bufferelÈs
											;		- CFG_WBUFFER flag not set: mindig emul·lt w-bufferelÈs
											;						vagy a W-bufferelÈs mÛdszere detekt·lt, ekkor
											;		- CFG_RELYONDRIVERFLAGS set: a driver ·ltal a W-bufferelÈs t·mogat·s·ra
											;								 visszaadott flag alapj·n dˆnt
CFG_VESAENABLE          EQU     100h        ;BeÈpÌtett VESA-t·mogat·s engedÈlyezve
CFG_SETMOUSEFOCUS       EQU     200h        ;EgÈrfÛkusz az alkalmaz·sn·l
CFG_SUPPORTMODE13H      EQU     400h        ;320x200x256 VGA-mÛd t·mogat·sa
CFG_TEXTURES32BIT       EQU     800h        ;mindig 32 bites text˙r·k
CFG_TEXTURES16BIT       EQU     1000h       ;mindig 16 bites text˙r·k
CFG_TEXTURERELOAD       EQU     2000h       ;Text˙r·k ˙jratˆltÈse, ha kell
CFG_GRABENABLE          EQU     4000h       ;KÈpernyılop·s engedÈlyezve
CFG_GRABCLIPBOARD       EQU     8000h       ;KÈplop·s a v·gÛlapra
CFG_CKMGEFORCE          EQU     10000h      ;Colorkeying GeForce-ra (2-es mÛdszer)
CFG_TOMBRAIDER          EQU     20000h      ;TR1 ·rnyÈkhiba-javÌt·s
CFG_HUNGARIAN           EQU     40000h      ;A nyelv magyar
CFG_HWLFBCACHING        EQU     80000h      ;Hardveres gyorsÌtÛpufferek az LFB-hez
CFG_LFBREALVOODOO       EQU     100000h     ;Kˆzelebb az igazi hardverhez
CFG_TRIPLEBUFFER        EQU     200000h     ;Mindig triplapufferelÈs
CFG_ACTIVEBACKGROUND    EQU     400000h     ;A h·ttÈren is aktÌv a DOS-os prg
CFG_LFBFASTUNMATCHWRITE EQU     800000h     ;GyorsÌr·s a nem egyezı LFB form·tumokhoz
CFG_CTRLALT             EQU     1000000h    ;EgÈrfÛkusz elengedÈse Ctrl-Alt-ra
CFG_VSYNC               EQU     2000000h    ;F¸ggıleges visszafut·st mindig megv·rni
CFG_NTVDDMODE           EQU     4000000h    ;a wrapper VDD-mÛdban m˚kˆdik (XP-hez)
CFG_DISABLEMIPMAPPING   EQU     8000000h    ;Mipmapping tilt·sa
CFG_ENABLEGAMMARAMP     EQU     10000000h   ;Glide-gammaramp engedÈlyezÈse
CFG_ENABLEHWVBUFFER     EQU     20000000h   ;Hardveres vertex pufferek haszn·lata
CFG_BILINEARFILTER      EQU     40000000h   ;Mindig biline·ris sz˚rÈs a text˙r·khoz
CFG_CLIPPINGBYD3D       EQU     80000000h   ;V·g·s a Direct3D-n kereszt¸l

CFG2_CLOSESTREFRESHFREQ EQU     1h
CFG_FORCECLIPOPF        EQU     2h          ;CLI/POPF megfelelı kezelÈse - rejtett opciÛ
CFG_LFBRESCALECHANGESONLY EQU   4h          ;eltÈrı felbont·sok esetÈn csak a v·ltoz·sok ·tmÈretezÈse,
                                            ; Ès nem az egÈsz kÈpÈ (gyorsÌr·st igÈnyel) - rejtett opciÛ
CFG_LFBFORCEFASTWRITE   EQU   8h            ;GyorsÌr·s haszn·lata egyezı form·tumokhoz is - rejtett opciÛ
CFG_YMIRROR             EQU   10h           ;KÈpernyı f¸ggıleges t¸krˆzÈse (origÛ-tesztekhez) - rejtett opciÛ
CFG_FORCETRILINEARMIPMAPS EQU   20h         ;Triline·ris mipmapping kÈnyszerÌtÈse - rejtett opciÛ
CFG_PREFERTMUWIFPOSSIBLE  EQU   40h         ;t·bl·s kˆd, z-buffer Ès TMUDIFFW hint esetÈn a tmu w-t
CFG_LFBALWAYSFLUSHWRITES  EQU	80h	    ;A rÈgi lfb-unlock dude-mÛdja: mindig kiÌrja a v·ltoz·sokat
					    ;(nem cache-el) (kompatibilit·si Ès teszt-opciÛ, rejtett)
CFG_DEPTHEQUALTOCOLOR	EQU	100h	    ;A kÈppuffer Ès a mÈlysÈgi puffer bitmÈlysÈge legyen mindig egyenlı								    ;(kompatibilit·si Ès teszt-opciÛ, rejtett)
CFG_DISABLELFBREAD	EQU	200h	    ;LFB elÈrÈsÈnek tilt·sa olvas·sra
CFG_DISABLELFBWRITE	EQU	400h	    ;LFB elÈrÈsÈnek tilt·sa Ìr·sra
CFG_AUTOGENERATEMIPMAPS	EQU	800h	    ;A hi·nyzÛ mipmap-szintek automatikus gener·l·sa
CFG_LFBNOMATCHINGFORMATS  EQU	1000h	    ;Az LFB-form·tumok mindig k¸lˆnbˆzınek tekintendık - rejtett opciÛ
CFG_ALWAYSINDEXEDPRIMS	EQU	2000h		
CFG_HIDECONSOLE		EQU	4000h	    ;Konzol elrejtÈse (DOS)
CFG_LFBENABLETEXTURING	EQU	8000h
CFG_CFORMATAFFECTLFB	EQU	10000h
CFG_DISABLEVESAFLATLFB	EQU	20000h


;A konfig flagekben levı bitmezık ÈrtÈkei
CFG_CKMAUTOMATIC	EQU	00
CFG_CKMALPHABASED	EQU	01
CFG_CKMNATIVE		EQU	02
CFG_CKMNATIVETNT	EQU	03

;------------------------------------------------------------------------------------------
;Az egyes platformok kÛdjai
PLATFORM_WINDOWS          EQU   0
PLATFORM_DOSWIN9X         EQU   1
PLATFORM_DOSWINNT         EQU   2

;------------------------------------------------------------------------------------------
; Az aktu·lis videÛmÛd Ès annak pixelform·tuma (VESA-hoz)

dgVoodooModeInfo        STRUC

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
; EgÈrkurzor-infÛ: jelenleg haszn·laton kÌv˚li */
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

; A kommunik·ciÛs ter¸let felÈpÌtÈse (ez van az osztott memÛriater¸leten)
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
; Az egyes Glide-f˚ggvÈnyek kÛdjai

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
GUALPHASOURCE                   EQU             55
GRALPHATESTFUNCTION             EQU             56
GRALPHATESTREFERENCEVALUE       EQU             57

GRFOGCOLORVALUE                 EQU             58
GRFOGMODE                       EQU             59
GRFOGTABLE                      EQU             60
GLIDEGETINDTOWTABLE             EQU             61
GUFOGTABLEINDEXTOW              EQU             62
GUFOGGENERATEEXP                EQU             63
GUFOGGENERATEEXP2               EQU             64
GUFOGGENERATELINEAR             EQU             65

GRRENDERBUFFER                  EQU             66
GRSSTORIGIN                     EQU             67
GRCLIPWINDOW                    EQU             68
GRCULLMODE                      EQU             69
GRDISABLEALLEFFECTS             EQU             70
GRDITHERMODE                    EQU             71
GRGAMMACORRECTIONVALUE          EQU             72
GRGLIDEGETSTATE                 EQU             73
GRGLIDESETSTATE                 EQU             74
GRHINTS                         EQU             75

GRSSTSCREENWIDTH                EQU             76
GRSSTSCREENHEIGHT               EQU             77
GRSSTSTATUS                     EQU             78
GRSSTVIDEOLINE                  EQU             79
GRSSTVRETRACEON                 EQU             80
GRSSTCONTROLMODE                EQU             81
GRBUFFERNUMPENDING              EQU             82
GRRESETTRISTATS                 EQU             83
GRTRISTATS                      EQU             84
                                        
GRDEPTHBUFFERMODE               EQU             85
GRDEPTHBUFFERFUNCTION           EQU             86
GRDEPTHMASK                     EQU             87
GRLFBCONSTANTALPHA              EQU             88
GRLFBREADREGION                 EQU             89
GRLFBWRITEREGION                EQU             90
GRDEPTHBIASLEVEL                EQU             91
GLIDEGETCONVBUFFXRES            EQU             92

GRCHROMAKEYMODE                 EQU             93
GRCHROMAKEYVALUE                EQU             94

GRLFBLOCK                       EQU             95
GRLFBUNLOCK                     EQU             96
GRLFBWRITECOLORFORMAT           EQU             97
GRLFBWRITECOLORSWIZZLE          EQU             98
GLIDESETUPLFBDOSBUFFERS         EQU             99

;Glide1-fÅggvÇnyek
GRSSTOPEN                       EQU             100
GRLFBBYPASSMODE                 EQU             101
GRLFBORIGIN                     EQU             102
GRLFBWRITEMODE                  EQU             103
GRLFBBEGIN                      EQU             104
GRLFBEND                        EQU             105
GRLFBGETREADPTR                 EQU             106
GRLFBGETWRITEPTR                EQU             107
GUFBREADREGION                  EQU             108
GUFBWRITEREGION                 EQU             109


;VESA-f¸ggvÈnyek
VESASETVBEMODE                  EQU             110
VESAUNSETVBEMODE                EQU             111
VESAFRESH                       EQU             112

;Ezeket a f˚ggvÈnyeket a kernelmodul hivja
DGVOODOOCLIENTCRASHED           EQU             113
DGVOODOONEWCLIENTREGISTERING    EQU             114

;EgyÈb f¸ggvÈnyek
DGVOODOOGETCONFIG               EQU             115
DGVOODOORELEASEFOCUS            EQU             116

GLIDEINSTALLMOUSE		EQU		117
GLIDEUNINSTALLMOUSE		EQU		118
GLIDEISMOUSEINSTALLED		EQU		119

VESAGETXRES			EQU		120
VESAGETYRES			EQU		121

PORTLOG_IN			EQU		122
PORTLOG_OUT			EQU		123

GLIDEENDITEM                    EQU             0FFFFFFFFh
