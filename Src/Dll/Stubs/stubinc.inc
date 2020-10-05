.386

CurrentProductVersion   EQU     140h

CFG_WBUFFER				EQU		1h			;W-buffert mindig Z-bufferen keresztül emulál
CFG_RELYONDRIVERFLAGS	EQU		CFG_WBUFFER
CFG_WINDOWED			EQU		2h			;Ablakos mód
CFG_DISPMODE32BIT		EQU		4h			;32 bites videomód
CFG_SETREFRESHRATE		EQU		8h			;A frissítési freq-t az alkalmazás
											;	állítja be (egyébként az akt. freq)
CFG_PERFECTTEXMEM		EQU		10h			;	Tökéletes textúramemória-emuláció
CFG_WBUFFERMETHODISFORCED EQU	80h			;	A W-bufferelés módszere kényszerített:
											;		- CFG_WBUFFER flag set: mindig valódi w-bufferelés
											;		- CFG_WBUFFER flag not set: mindig emulált w-bufferelés
											;						vagy a W-bufferelés módszere detektált, ekkor
											;		- CFG_RELYONDRIVERFLAGS set: a driver által a W-bufferelés támogatására
											;								 visszaadott flag alapján dönt
CFG_VESAENABLE          EQU     100h        ;Beépített VESA-támogatás engedélyezve
CFG_SETMOUSEFOCUS       EQU     200h        ;Egérfókusz az alkalmazásnál
CFG_SUPPORTMODE13H      EQU     400h        ;320x200x256 VGA-mód támogatása
CFG_TEXTURES32BIT       EQU     800h        ;mindig 32 bites textúrák
CFG_TEXTURES16BIT       EQU     1000h       ;mindig 16 bites textúrák
CFG_STOREMIPMAP			EQU     2000h       ;Textúrák újratöltése, ha kell
CFG_GRABENABLE          EQU     4000h       ;Képernyõlopás engedélyezve
CFG_GRABCLIPBOARD       EQU     8000h       ;Képlopás a vágólapra
CFG_CKMGEFORCE          EQU     10000h      ;Colorkeying GeForce-ra (2-es módszer)
CFG_TOMBRAIDER          EQU     20000h      ;TR1 árnyékhiba-javítás
CFG_HUNGARIAN           EQU     40000h      ;A nyelv magyar
CFG_HWLFBCACHING        EQU     80000h      ;Hardveres gyorsítópufferek az LFB-hez
CFG_LFBREALVOODOO       EQU     100000h     ;Közelebb az igazi hardverhez
CFG_TRIPLEBUFFER        EQU     200000h     ;Mindig triplapufferelés
CFG_ACTIVEBACKGROUND    EQU     400000h     ;A háttéren is aktív a DOS-os prg
CFG_LFBFASTUNMATCHWRITE EQU     800000h     ;Gyorsírás a nem egyezõ LFB formátumokhoz
CFG_CTRLALT             EQU     1000000h    ;Egérfókusz elengedése Ctrl-Alt-ra
CFG_VSYNC               EQU     2000000h    ;Függõleges visszafutást mindig megvárni
CFG_NTVDDMODE           EQU     4000000h    ;a wrapper VDD-módban mûködik (XP-hez)
CFG_DISABLEMIPMAPPING   EQU     8000000h    ;Mipmapping tiltása
CFG_ENABLEGAMMARAMP     EQU     10000000h   ;Glide-gammaramp engedélyezése
CFG_ENABLEHWVBUFFER     EQU     20000000h   ;Hardveres vertex pufferek használata
CFG_BILINEARFILTER      EQU     40000000h   ;Mindig bilineáris szûrés a textúrákhoz
CFG_CLIPPINGBYD3D       EQU     80000000h   ;Vágás a Direct3D-n keresztül

CFG2_CLOSESTREFRESHFREQ EQU     1h
CFG_FORCECLIPOPF        EQU     2h          ;CLI/POPF megfelelõ kezelése - rejtett opció
CFG_LFBRESCALECHANGESONLY EQU   4h          ;eltérõ felbontások esetén csak a változások átméretezése,
                                            ; és nem az egész képé (gyorsírást igényel) - rejtett opció
CFG_LFBFORCEFASTWRITE   EQU   8h            ;Gyorsírás használata egyezõ formátumokhoz is - rejtett opció
CFG_YMIRROR             EQU   10h           ;Képernyõ függõleges tükrözése (origó-tesztekhez) - rejtett opció
CFG_FORCETRILINEARMIPMAPS EQU   20h         ;Trilineáris mipmapping kényszerítése - rejtett opció
CFG_PREFERTMUWIFPOSSIBLE  EQU   40h         ;táblás köd, z-buffer és TMUDIFFW hint esetén a tmu w-t
CFG_LFBALWAYSFLUSHWRITES  EQU	80h	    ;A régi lfb-unlock dude-módja: mindig kiírja a változásokat
					    ;(nem cache-el) (kompatibilitási és teszt-opció, rejtett)
CFG_DEPTHEQUALTOCOLOR	EQU	100h	    ;A képpuffer és a mélységi puffer bitmélysége legyen mindig egyenlõ								    ;(kompatibilitási és teszt-opció, rejtett)
CFG_DISABLELFBREAD	EQU	200h	    ;LFB elérésének tiltása olvasásra
CFG_DISABLELFBWRITE	EQU	400h	    ;LFB elérésének tiltása írásra
CFG_AUTOGENERATEMIPMAPS	EQU	800h	    ;A hiányzó mipmap-szintek automatikus generálása
CFG_LFBNOMATCHINGFORMATS  EQU	1000h	    ;Az LFB-formátumok mindig különbözõnek tekintendõk - rejtett opció
CFG_ALWAYSINDEXEDPRIMS	EQU	2000h		
CFG_HIDECONSOLE		EQU	4000h	    ;Konzol elrejtése (DOS)
CFG_LFBFORCETEXTURING	EQU	8000h
CFG_CFORMATAFFECTLFB	EQU	10000h
CFG_DISABLEVESAFLATLFB	EQU	20000h


;A konfig flagekben levõ bitmezõk értékei
CFG_CKMAUTOMATIC	EQU	00
CFG_CKMALPHABASED	EQU	01
CFG_CKMNATIVE		EQU	02
CFG_CKMNATIVETNT	EQU	03


DGVOODOOCONFIG  STRUC

ProductVersion  	DW      ?
GlideVersion    	DW      ?
TexMemSize      	DD      ?
Flags           	DD      ?
Gamma           	DD      ?
wVersion        	DD      ?
VideoMemSize    	DD      ?
VideoRefreshFreq 	DD      ?
DResX           	DW      ?
DResY           	DW      ?
dispdev         	DW      ?
dispdriver      	DW      ?
RefreshRate     	DD      ?
Flags2          	DD      ?
dosTimerBoostPeriod 	DW	?
AlphaCKRefValue 	DB      ?
                	DB      5 DUP(?)

DGVOODOOCONFIG  ENDS

DEFAULT_CONFIGFLAGS_WIN EQU     CFG_HWLFBCACHING OR CFG_RELYONDRIVERFLAGS OR \
                                CFG_LFBFASTUNMATCHWRITE OR CFG_PERFECTTEXMEM OR \
                                CFG_STOREMIPMAP OR (CFG_CKMAUTOMATIC SHL 5)

DEFAULT_CONFIGFLAGS_DOS EQU     CFG_HWLFBCACHING OR CFG_RELYONDRIVERFLAGS OR \
                                CFG_LFBFASTUNMATCHWRITE OR CFG_PERFECTTEXMEM OR \
                                CFG_STOREMIPMAP OR CFG_NTVDDMODE OR \
								(CFG_CKMAUTOMATIC SHL 5)

DEFAULT_CONFIGFLAGS2_WIN EQU    CFG_HIDECONSOLE OR CFG_FORCETRILINEARMIPMAPS OR \
				CFG_LFBRESCALECHANGESONLY

DEFAULT_CONFIGFLAGS2_DOS EQU    CFG_HIDECONSOLE OR CFG_FORCETRILINEARMIPMAPS OR \
				CFG_LFBRESCALECHANGESONLY

DEFAULT_DOS_TIMER_BOOST	EQU	80