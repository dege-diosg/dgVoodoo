.386

CurrentProductVersion   EQU     150h

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
CFG_STOREMIPMAP			EQU     2000h       ;Text�r�k �jrat�lt�se, ha kell
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
CFG_LFBFORCETEXTURING	EQU	8000h
CFG_CFORMATAFFECTLFB	EQU	10000h
CFG_DISABLEVESAFLATLFB	EQU	20000h


;A konfig flagekben lev� bitmez�k �rt�kei
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
                                CFG_STOREMIPMAP OR (CFG_CKMAUTOMATIC SHL 5) OR \
								CFG_ENABLEGAMMARAMP


DEFAULT_CONFIGFLAGS_DOS EQU     CFG_HWLFBCACHING OR CFG_RELYONDRIVERFLAGS OR \
                                CFG_LFBFASTUNMATCHWRITE OR CFG_PERFECTTEXMEM OR \
                                CFG_STOREMIPMAP OR CFG_NTVDDMODE OR CFG_TOMBRAIDER OR \
								(CFG_CKMAUTOMATIC SHL 5) OR \
								CFG_ENABLEGAMMARAMP

DEFAULT_CONFIGFLAGS2_WIN EQU    CFG_HIDECONSOLE OR CFG_FORCETRILINEARMIPMAPS OR \
								CFG_LFBRESCALECHANGESONLY OR CFG2_CLOSESTREFRESHFREQ

DEFAULT_CONFIGFLAGS2_DOS EQU    CFG_HIDECONSOLE OR CFG_FORCETRILINEARMIPMAPS OR \
								CFG_LFBRESCALECHANGESONLY OR CFG2_CLOSESTREFRESHFREQ

DEFAULT_DOS_TIMER_BOOST	EQU	80