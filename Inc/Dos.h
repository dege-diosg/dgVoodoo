/*--------------------------------------------------------------------------------- */
/* DOS.H - All needed structs, defines, etc used for communication between          */
/*         DOS and Windows (including Glide, dgVesa)                                */
/*                                                                                  */
/* Copyright (C) 2003 Dege                                                          */
/*                                                                                  */
/* This library is free software; you can redistribute it and/or                    */
/* modify it under the terms of the GNU Lesser General Public                       */
/* License as published by the Free Software Foundation; either                     */
/* version 2.1 of the License, or (at your option) any later version.               */
/*                                                                                  */
/* This library is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                */
/* Lesser General Public License for more details.                                  */
/*                                                                                  */
/* You should have received a copy of the GNU Lesser General Public                 */
/* License along with this library; if not, write to the Free Software              */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   */
/*--------------------------------------------------------------------------------- */


/*------------------------------------------------------------------------------------------*/
/* dgVoodoo: Dos.h																			*/
/*			 DOS-kommunik�ci�																*/
/*------------------------------------------------------------------------------------------*/

#ifndef	DOS_H
#define DOS_H

//#include "dgVoodooBase.h"
#include "dgVoodooConfig.h"

#ifdef __MSC__
#include "vddsvc.h"
#endif

/* Ezt a header-t a Dos-driver ford�t�sakor is haszn�ljuk, ez�rt van az al�bbi defin�ci� */
#ifndef __MSC__
#define HANDLE	unsigned int
#define HWND	unsigned int
#endif

/*------------------------------------------------------------------------------------------*/
/*...................................... Defin�ci�k ........................................*/

/* Az egyes platformok k�djai */
#ifndef	PLATFORM_CODES
#define PLATFORM_CODES

#define	PLATFORM_WINDOWS	0
#define	PLATFORM_DOSWIN9X	1
#define PLATFORM_DOSWINNT	2


/* A szervernek k�ld�tt �zenetek */
#define	DGSM_PROCEXECBUFF		(WM_USER+0)	/* A k�z�s ter�leten lev� parancsok (f�ggv�nyek) v�grehajt�sa */
#define	DGSM_SETCONSOLEHWND		(WM_USER+1) /* A Dos-os prg konzolablak�nak HWND-j�nek elt�rol�sa */
#define	DGSM_SETTIMER			(WM_USER+2)
#define	DGSM_DGVOODOORESET		(WM_USER+3)
#define	DGSM_SETCLIENTHWND		(WM_USER+4)
#define	DGSM_GETVESAMODEFORMAT	(WM_USER+5)
#define DGSM_CLOSEBINDING		(WM_USER+6)
#define DGSM_RESETDEVICE		(WM_USER+7)
#define DGSM_GETDEVICESTATUS	(WM_USER+8)

/* A kliensnek k�ld�tt �zenetek */
#define	DGCM_VESAGRABACTBANK	(WM_USER+64)
#define	DGCM_SUSPENDDOSTHREAD	(WM_USER+65)
#define	DGCM_RESUMEDOSTHREAD	(WM_USER+66)
#define DGCM_MOUSEDRIVERSTRUC	(WM_USER+1)
#define DGCM_MOUSEINTERRUPT		(WM_USER+68)


#endif

/*------------------------------------------------------------------------------------------*/
/*..................................... Strukt�r�k .........................................*/

/* Az aktu�lis vide�m�d �s annak pixelform�tuma (VESA-hoz) */
typedef struct	{

	int				modeIndex;
	unsigned short	ModeNumber;
	unsigned short	XRes;
	unsigned short	YRes;
	unsigned char	BitPP;
	unsigned char	RedSize;
	unsigned char	RedPos;
	unsigned char	GreenSize;
	unsigned char	GreenPos;
	unsigned char	BlueSize;
	unsigned char	BluePos;
	unsigned char	RsvSize;
	unsigned char	RsvPos;
	unsigned char	reserved;

} dgVoodooModeInfo;


/* Eg�rkurzor-inf�: jelenleg haszn�laton k�v�li */
typedef struct	{
	unsigned int	x;
	unsigned int	y;
	unsigned int	brecttop_x, brecttop_y;
	unsigned int	brectbottom_x, brectbottom_y;
} MouseInfo;


/* Regiszterstrukt�ra a VESA param�terekhez */
typedef struct	{

	union {
		struct {
			unsigned char	al;
			unsigned char	ah;
		};
		unsigned short	ax;
		unsigned long	eax;
	};
	union {
		struct {
			unsigned char	bl;
			unsigned char	bh;
		};
		unsigned short	bx;
		unsigned long	ebx;
	};
	union {
		struct {
			unsigned char	cl;
			unsigned char	ch;
		};
		unsigned short	cx;
		unsigned long	ecx;
	};
	union {
		struct {
			unsigned char	dl;
			unsigned char	dh;
		};
		unsigned short	dx;
		unsigned long	edx;
	};
	union {
		unsigned short	si;
		unsigned long	esi;
	};
	union {
		unsigned short	di;
		unsigned long	edi;
	};
	unsigned long	es;

} VESARegisters;


/*------------------------------------------------------------------------------------------*/

/* A k�z�s mem�riater�let t�pusa (regisztr�ci�s vagy kommunik�ci�s ter�let). */
/* A regisztr�ci�s strukt�ra akkor haszn�latos, amikor a wrapper XP alatt szerver m�dban fut. */
#define AREAID_REGSTRUCTURE_GLIDE211	2
#define AREAID_REGSTRUCTURE_GLIDE243	3
#define AREAID_COMMAREA					1


#define	LFBMAXPIXELSIZE					4

#ifndef GLIDE1

#define AREAID_REGSTRUCTURE				AREAID_REGSTRUCTURE_GLIDE243

#else

#define AREAID_REGSTRUCTURE				AREAID_REGSTRUCTURE_GLIDE211

#endif

typedef struct	{

	unsigned int	areaId;					/* A k�z�s mem�riater�let t�pusa */

} AreaIdPrefix;


/*------------------------------------------------------------------------------------------*/

/* A f�ggv�nyk�doknak �s az �tadand� adatoknak fenntartott ter�letek m�rete */
#define EXEBUFFSIZE		((32*1024)/4)
#define FUNCDATASIZE	1536*1024


/* Flagek a kernelflag mez�h�z */
/* (Ezek olyan flagek, amelyeket DOS-b�l, �s kernelm�dban/VDD-ben egyar�nt haszn�lunk */

#define KFLAG_INKERNEL			0x1			/* A wrapper �ppen kernelm�dban dolgozik */
#define KFLAG_VMWINDOWED		0x2			/* Win9x/Me: a kiszolg�lt VM ablakos m�dban ind�totta a szervert - nem haszn�lt*/
#define KFLAG_SCREENSHOT		0x4			/* A megfelel� billenty� a screenshot k�sz�t�s�hez lenyomva */
#define KFLAG_GLIDEACTIVE		0x8			/* A glide akt�v - nem haszn�lt */
#define KFLAG_SERVERWORKING		0x20		/* A szerver a megadott f�ggv�nyeket dolgozza fel */
#define KFLAG_CTRLALT			0x80		/* Win9x/Me: a Ctrl-Alt lenyomva, eg�rf�kusz-elenged�si rutint megh�vni */
#define KFLAG_PALETTECHANGED	0x100		/* A DOS-os VM megv�ltoztatta a VGA/VESA palett�t */
#define KFLAG_VESAFRESHDISABLED	0x200		/* A VESA-k�pfriss�t�s tiltva */
#define KFLAG_DRAWMUTEXUSED		0x400		/* A rajzol�si mutexet birtokolja a DOS-os sz�l */
#define KFLAG_SUSPENDDOSTHREAD	0x800		/* A rajzol�si mutex elenged�se ut�n a DOS-sz�lat suspendelni kell */


/* A kommunik�ci�s ter�let fel�p�t�se (ez van az osztott mem�riater�leten) */
typedef struct	{
	
	AreaIdPrefix	areaId;					/* Ter�let (strukt�ra) t�pusa: AREAID_COMMAREA */
	unsigned int	kernelflag;				/* k�l�nb�z� flagek a DOS-os �s Windowsos r�sz k�zti kommunik�ci�hoz */
	unsigned int	ExeCodeIndex;			/* Elt�rolt f�ggv�nyk�dok indexe */
	unsigned char	*FDPtr;					/* Mutat� az �tadand� adatokhoz (a FuncData mez�n bel�l): szabad ter�let */
	/*unsigned*/ char	progname[128];			/* Dos-os program neve */
	union {
		unsigned int	WinOldApHWND;			/* A Dos-os prg konzolablak�nak HWND-je (csak Win9x/Me) */
		HWND			consoleHwnd;	
	};
	unsigned int	VESAPalette[256];		/* VESA: aktu�lis paletta, ARGB */
	unsigned char	VESASession;			/* VESA: 1=akt�v, 0=inakt�v */
	unsigned char	VGAStateRegister3DA;	/* virtu�lis VGA 3DA (vertical blank) regisztere */
	unsigned char	VGARAMDACSize;			/* virtu�lis VGA RAMDAC m�rete: 6 vagy 8 bit */
	unsigned char	Reserved;
	void			*VideoMemory;			/* virtu�lis vide�mem�ria line�ris c�me */
	void			*videoMemoryClient;		/* virtu�lis vide�mem�ria line�ris c�me a kliens c�mter�ben */
	unsigned int	BytesPerScanLine;		/* VESA: bytes per scan line */
	unsigned int	DisplayOffset;			/* VESA: display offset */
	dgVoodooModeInfo actmodeinfo;			/* Az aktu�lis VESA m�d inf�ja */
	dgVoodooModeInfo vesaModeinfo;
	
    unsigned long	ExeCodes[EXEBUFFSIZE];	/* F�ggv�nyk�doknak fenntartott hely */
	unsigned char	FuncData[FUNCDATASIZE];	/* �tadand� adatoknak fenntartott hely */
	unsigned char	LFB[];					/* A strukt�ra kib�v�tve az LFB sz�m�ra fenntartott hellyel */
											/* Csak WinXp szerverm�dban */

} CommArea;

/*------------------------------------------------------------------------------------------*/
/* Adatok, amelyeket a szerver �tad a kernelmodulnak, amikor regisztr�lja mag�t				*/
typedef struct	{

	AreaIdPrefix	areaId;					/* A k�z�s mem�riater�let t�pusa: AREAID_REGSTRUCTURE */
	HANDLE			hidWinHwnd;				/* A dolgoz� sz�l rejtett ablak�nak kezel�je */
	CommArea		*serveraddrspace;		/* A k�z�s ter�let c�me a szerver c�mter�ben */
	dgVoodooConfig  serverConfig;			/* A szerver konfigja */

} ServerRegInfo;


/*------------------------------------------------------------------------------------------*/
/* A kliens �ltal h�vott szolg�ltat�sok (a DOS-os overlay-b�l)								*/
/* (Win98/Me alatt a VXD dolgozza fel �ket, NT alatt k�zvetlen�l a szerver)					*/
#define DGCLIENT_BEGINGLIDE				0x1		/* Init �s a kliens regisztr�l�sa */
#define DGCLIENT_ENDGLIDE				0x2		/* kliens eldob�sa, cleanup */
#define DGCLIENT_CALLSERVER				0x3		/* Szerver megh�v�sa, a pufferelt f�ggv�nyek*/
												/* v�grehajt�sa */
#define DGCLIENT_GETPLATFORMTYPE		0x4		/* Platform lek�rdez�se */
#define DGCLIENT_RELEASE_TIME_SLICE		0x5		/* Id�szelet elenged�se (csak NT) */
#define DGCLIENT_RELEASE_MUTEX			0x6		/* Rajzol�si mutex elenged�se (csak NT) */
#define DGCLIENT_VESA					0x7
#define DGCLIENT_BEGINVESA				0x8
#define DGCLIENT_ENDVESA				0x9
#define DGCLIENT_INSTALLMOUSE			0xA
#define DGCLIENT_UNINSTALLMOUSE			0xB
#define DGCLIENT_GETVERSION				0xC

/*------------------------------------------------------------------------------------------*/
/* Az egyes Glide-f�ggv�nyek k�djai */

#define GRGLIDEINIT						0
#define	GRSSTWINOPEN					1
#define GRSSTWINCLOSE					2
#define GRGLIDESHUTDOWN					3

#define GRBUFFERCLEAR					4
#define GRBUFFERSWAP					5

#define GUDRAWTRIANGLEWITHCLIP			6
#define GUAADRAWTRIANGLEWITHCLIP		7
#define GUDRAWPOLYGONVERTEXLISTWITHCLIP 8
#define GRDRAWTRIANGLE					9
#define GRDRAWPOLYGONVERTEXLIST			10
#define GRAADRAWPOLYGON					11
#define GRAADRAWPOLYGONVERTEXLIST		12
#define GRAADRAWTRIANGLE				13
#define GRDRAWPLANARPOLYGON				14
#define GRDRAWPLANARPOLYGONVERTEXLIST	15
#define GRDRAWPOLYGON					16
#define GRAADRAWLINE					17
#define GRDRAWLINE						18
#define GRAADRAWPOINT					19
#define GRDRAWPOINT						20

#define GRTEXDOWNLOADMIPMAP				21
#define GRTEXDOWNLOADMIPMAPLEVEL		22
#define GRTEXDOWNLOADMIPMAPLEVELPARTIAL	23
#define GRTEXDOWNLOADTABLE				24
#define GRTEXDOWNLOADTABLEPARTIAL		25
#define GRTEXNCCTABLE					26
#define GRTEXSOURCE						27
#define GRTEXMIPMAPMODE					28
#define GRTEXCOMBINE					29
#define GRTEXFILTERMODE					30
#define GRTEXCLAMPMODE					31
#define GRTEXLODBIASVALUE				32
#define GRTEXMULTIBASE					33
#define GRTEXMULTIBASEADDRESS			34

#define GUTEXALLOCATEMEMORY				35
#define GUTEXCHANGEATTRIBUTES			36
#define GUTEXCOMBINEFUNCTION			37
#define GUTEXDOWNLOADMIPMAP				38
#define GUTEXDOWNLOADMIPMAPLEVEL		39
#define GUTEXGETCURRENTMIPMAP			40
#define GUTEXGETMIPMAPINFO				41
#define GUTEXMEMQUERYAVAIL				42
#define GUTEXMEMRESET					43
#define GUTEXSOURCE						44
#define GU3DFGETINFO					45
#define GU3DFLOAD						46
#define GLIDEGETUTEXTURESIZE			47

#define GRCOLORCOMBINE					48
#define GUCOLORCOMBINEFUNCTION			49
#define GRCONSTANTCOLORVALUE4			50
#define GRCOLORMASK						51
#define GRCONSTANTCOLORVALUE			52

#define GRALPHABLENDFUNCTION			53
#define GRALPHACOMBINE					54
#define	GRALPHACONTROLSITRGBLIGHTING	55
#define GUALPHASOURCE					56
#define GRALPHATESTFUNCTION				57
#define GRALPHATESTREFERENCEVALUE		58

#define GRFOGCOLORVALUE					59
#define GRFOGMODE						60
#define GRFOGTABLE						61
#define GLIDEGETINDTOWTABLE				62
#define GUFOGTABLEINDEXTOW				63
#define GUFOGGENERATEEXP				64
#define GUFOGGENERATEEXP2				65
#define GUFOGGENERATELINEAR				66

#define GRRENDERBUFFER					67
#define GRSSTORIGIN						68
#define GRCLIPWINDOW					69
#define GRCULLMODE						70
#define GRDISABLEALLEFFECTS				71
#define GRDITHERMODE					72
#define GRGAMMACORRECTIONVALUE			73
#define GRGLIDEGETSTATE					74
#define GRGLIDESETSTATE					75
#define GRHINTS							76

#define GRSSTSCREENWIDTH				77
#define GRSSTSCREENHEIGHT				78
#define GRSSTSTATUS						79
#define GRSSTVIDEOLINE					80
#define GRSSTVRETRACEON					81
#define GRSSTCONTROLMODE				82
#define GRBUFFERNUMPENDING				83
#define GRRESETTRISTATS					84
#define GRTRISTATS						85

#define GRDEPTHBUFFERMODE				86
#define GRDEPTHBUFFERFUNCTION			87
#define GRDEPTHMASK						88
#define GRLFBCONSTANTALPHA				89
#define GRLFBREADREGION					90
#define GRLFBWRITEREGION				91
#define GRDEPTHBIASLEVEL				92
#define GLIDEGETCONVBUFFXRES			93

#define GRCHROMAKEYMODE					94
#define GRCHROMAKEYVALUE				95

#define GRLFBLOCK						96
#define GRLFBUNLOCK						97
#define GRLFBWRITECOLORFORMAT			98
#define GRLFBWRITECOLORSWIZZLE			99
#define GLIDESETUPLFBDOSBUFFERS			100

/* Glide1-f�ggv�nyek */
#define GRSSTOPEN						101
#define GRLFBBYPASSMODE					102
#define GRLFBORIGIN						103
#define GRLFBWRITEMODE					104
#define GRLFBBEGIN						105
#define GRLFBEND						106
#define GRLFBGETREADPTR					107
#define GRLFBGETWRITEPTR				108
#define GUFBREADREGION					109
#define GUFBWRITEREGION					110

/* VESA-f�ggv�nyek */
#define VESASETVBEMODE					111
#define VESAUNSETVBEMODE				112
#define VESAFRESH						113

/* Ezeket a f�ggv�nyeket a kernelmodul hivja */
#define DGVOODOOCLIENTCRASHED			114
#define DGVOODOONEWCLIENTREGISTERING	115

/* Egy�b f�ggv�nyek */
#define DGVOODOOGETCONFIG				116
#define DGVOODOORELEASEFOCUS			117

#define GLIDEINSTALLMOUSE				118
#define GLIDEUNINSTALLMOUSE				119
#define GLIDEISMOUSEINSTALLED			120

#define VESAGETXRES						121
#define VESAGETYRES						122

#define PORTLOG_IN						123
#define PORTLOG_OUT						124


/* Ez a k�d z�rja le a k�dszekvenci�t */
#define GLIDEENDITEM					0xFFFFFFFF

#ifdef __MSC__
/*------------------------------------------------------------------------------------------*/
/*............................. Bels� f�ggv�nyek predeklar�ci�ja ...........................*/

void					HideMouseCursor(int);
void					RestoreMouseCursor(int);
HWND					CreateRenderingWindow();
void					DestroyRenderingWindow();
void					CreateServerCommandWindow();
void					DestroyServerCommandWindow();
void					SetWindowClientArea(HWND, int, int);
int						IsWindowSizeIsEqualToResolution (HWND window);
void					AdjustAspectRatio (HWND window, int x, int y);
unsigned int			GetIdealWindowSizeForResolution (unsigned int xRes, unsigned int yRes);
unsigned char*			DosGetSharedLFB();
void					DosSendSetTimerMessage();
DWORD					NTCallFunctionOnServer (unsigned long funcCode, unsigned int pNum, unsigned long *params);
int						DOSInstallIOHook (unsigned short firstPort, unsigned short lastPort,
										  void (_stdcall *inHandler)(unsigned short, unsigned char *),
										  void (_stdcall *outHandler)(unsigned short, unsigned char) );
void					DOSUninstallIOHook (unsigned short firstPort, unsigned short lastPort);
void					ExitDosNt ();
int						InitVDDRendering ();
void					ShutDownVDDRendering ();
int						OpenServerCommunicationArea (ServerRegInfo *regInfo);
void					CloseServerCommunicationArea ();
void*					DosMapFlat (unsigned short selector, unsigned int offset);
void					DosRendererStatus (int status);

/*------------------------------------------------------------------------------------------*/
/*.................................. Glob�lis v�ltoz�k .....................................*/
extern	CommArea		*c;
extern	HWND			warningBoxHwnd;
extern	HWND			renderingWinHwnd;
extern	HWND			serverCmdHwnd;
extern	HWND			clientCmdHwnd;
extern	HWND			consoleHwnd;
extern	unsigned int	actResolution;
extern	void			(_stdcall *_call_ica_hw_interrupt)(int ms, int line, int count);
extern	HANDLE			hDevice;
extern	ServerRegInfo	regInfo;

#endif

#endif	/* DOS_H */