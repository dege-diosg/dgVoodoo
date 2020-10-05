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
/*			 DOS-kommunikáció																*/
/*------------------------------------------------------------------------------------------*/

#ifndef	DOS_H
#define DOS_H

//#include "dgVoodooBase.h"
#include "dgVoodooConfig.h"

#ifdef __MSC__
#include "vddsvc.h"
#endif

/* Ezt a header-t a Dos-driver fordításakor is használjuk, ezért van az alábbi definíció */
#ifndef __MSC__
#define HANDLE	unsigned int
#define HWND	unsigned int
#endif

/*------------------------------------------------------------------------------------------*/
/*...................................... Definíciók ........................................*/

/* Az egyes platformok kódjai */
#ifndef	PLATFORM_CODES
#define PLATFORM_CODES

#define	PLATFORM_WINDOWS	0
#define	PLATFORM_DOSWIN9X	1
#define PLATFORM_DOSWINNT	2


/* A szervernek küldött üzenetek */
#define	DGSM_PROCEXECBUFF		(WM_USER+0)	/* A közös területen levõ parancsok (függvények) végrehajtása */
#define	DGSM_SETCONSOLEHWND		(WM_USER+1) /* A Dos-os prg konzolablakának HWND-jének eltárolása */
#define	DGSM_SETTIMER			(WM_USER+2)
#define	DGSM_DGVOODOORESET		(WM_USER+3)
#define	DGSM_SETCLIENTHWND		(WM_USER+4)
#define	DGSM_GETVESAMODEFORMAT	(WM_USER+5)
#define DGSM_CLOSEBINDING		(WM_USER+6)
#define DGSM_RESETDEVICE		(WM_USER+7)
#define DGSM_GETDEVICESTATUS	(WM_USER+8)

/* A kliensnek küldött üzenetek */
#define	DGCM_VESAGRABACTBANK	(WM_USER+64)
#define	DGCM_SUSPENDDOSTHREAD	(WM_USER+65)
#define	DGCM_RESUMEDOSTHREAD	(WM_USER+66)
#define DGCM_MOUSEDRIVERSTRUC	(WM_USER+1)
#define DGCM_MOUSEINTERRUPT		(WM_USER+68)


#endif

/*------------------------------------------------------------------------------------------*/
/*..................................... Struktúrák .........................................*/

/* Az aktuális videómód és annak pixelformátuma (VESA-hoz) */
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


/* Egérkurzor-infó: jelenleg használaton kívûli */
typedef struct	{
	unsigned int	x;
	unsigned int	y;
	unsigned int	brecttop_x, brecttop_y;
	unsigned int	brectbottom_x, brectbottom_y;
} MouseInfo;


/* Regiszterstruktúra a VESA paraméterekhez */
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

/* A közös memóriaterület típusa (regisztrációs vagy kommunikációs terület). */
/* A regisztrációs struktúra akkor használatos, amikor a wrapper XP alatt szerver módban fut. */
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

	unsigned int	areaId;					/* A közös memóriaterület típusa */

} AreaIdPrefix;


/*------------------------------------------------------------------------------------------*/

/* A függvénykódoknak és az átadandó adatoknak fenntartott területek mérete */
#define EXEBUFFSIZE		((32*1024)/4)
#define FUNCDATASIZE	1536*1024


/* Flagek a kernelflag mezõhöz */
/* (Ezek olyan flagek, amelyeket DOS-ból, és kernelmódban/VDD-ben egyaránt használunk */

#define KFLAG_INKERNEL			0x1			/* A wrapper éppen kernelmódban dolgozik */
#define KFLAG_VMWINDOWED		0x2			/* Win9x/Me: a kiszolgált VM ablakos módban indította a szervert - nem használt*/
#define KFLAG_SCREENSHOT		0x4			/* A megfelelõ billentyû a screenshot készítéséhez lenyomva */
#define KFLAG_GLIDEACTIVE		0x8			/* A glide aktív - nem használt */
#define KFLAG_SERVERWORKING		0x20		/* A szerver a megadott függvényeket dolgozza fel */
#define KFLAG_CTRLALT			0x80		/* Win9x/Me: a Ctrl-Alt lenyomva, egérfókusz-elengedési rutint meghívni */
#define KFLAG_PALETTECHANGED	0x100		/* A DOS-os VM megváltoztatta a VGA/VESA palettát */
#define KFLAG_VESAFRESHDISABLED	0x200		/* A VESA-képfrissítés tiltva */
#define KFLAG_DRAWMUTEXUSED		0x400		/* A rajzolási mutexet birtokolja a DOS-os szál */
#define KFLAG_SUSPENDDOSTHREAD	0x800		/* A rajzolási mutex elengedése után a DOS-szálat suspendelni kell */


/* A kommunikációs terület felépítése (ez van az osztott memóriaterületen) */
typedef struct	{
	
	AreaIdPrefix	areaId;					/* Terület (struktúra) típusa: AREAID_COMMAREA */
	unsigned int	kernelflag;				/* különbözõ flagek a DOS-os és Windowsos rész közti kommunikációhoz */
	unsigned int	ExeCodeIndex;			/* Eltárolt függvénykódok indexe */
	unsigned char	*FDPtr;					/* Mutató az átadandó adatokhoz (a FuncData mezõn belül): szabad terület */
	/*unsigned*/ char	progname[128];			/* Dos-os program neve */
	union {
		unsigned int	WinOldApHWND;			/* A Dos-os prg konzolablakának HWND-je (csak Win9x/Me) */
		HWND			consoleHwnd;	
	};
	unsigned int	VESAPalette[256];		/* VESA: aktuális paletta, ARGB */
	unsigned char	VESASession;			/* VESA: 1=aktív, 0=inaktív */
	unsigned char	VGAStateRegister3DA;	/* virtuális VGA 3DA (vertical blank) regisztere */
	unsigned char	VGARAMDACSize;			/* virtuális VGA RAMDAC mérete: 6 vagy 8 bit */
	unsigned char	Reserved;
	void			*VideoMemory;			/* virtuális videómemória lineáris címe */
	void			*videoMemoryClient;		/* virtuális videómemória lineáris címe a kliens címterében */
	unsigned int	BytesPerScanLine;		/* VESA: bytes per scan line */
	unsigned int	DisplayOffset;			/* VESA: display offset */
	dgVoodooModeInfo actmodeinfo;			/* Az aktuális VESA mód infója */
	dgVoodooModeInfo vesaModeinfo;
	
    unsigned long	ExeCodes[EXEBUFFSIZE];	/* Függvénykódoknak fenntartott hely */
	unsigned char	FuncData[FUNCDATASIZE];	/* Átadandó adatoknak fenntartott hely */
	unsigned char	LFB[];					/* A struktúra kibõvítve az LFB számára fenntartott hellyel */
											/* Csak WinXp szervermódban */

} CommArea;

/*------------------------------------------------------------------------------------------*/
/* Adatok, amelyeket a szerver átad a kernelmodulnak, amikor regisztrálja magát				*/
typedef struct	{

	AreaIdPrefix	areaId;					/* A közös memóriaterület típusa: AREAID_REGSTRUCTURE */
	HANDLE			hidWinHwnd;				/* A dolgozó szál rejtett ablakának kezelõje */
	CommArea		*serveraddrspace;		/* A közös terület címe a szerver címterében */
	dgVoodooConfig  serverConfig;			/* A szerver konfigja */

} ServerRegInfo;


/*------------------------------------------------------------------------------------------*/
/* A kliens által hívott szolgáltatások (a DOS-os overlay-bõl)								*/
/* (Win98/Me alatt a VXD dolgozza fel õket, NT alatt közvetlenül a szerver)					*/
#define DGCLIENT_BEGINGLIDE				0x1		/* Init és a kliens regisztrálása */
#define DGCLIENT_ENDGLIDE				0x2		/* kliens eldobása, cleanup */
#define DGCLIENT_CALLSERVER				0x3		/* Szerver meghívása, a pufferelt függvények*/
												/* végrehajtása */
#define DGCLIENT_GETPLATFORMTYPE		0x4		/* Platform lekérdezése */
#define DGCLIENT_RELEASE_TIME_SLICE		0x5		/* Idõszelet elengedése (csak NT) */
#define DGCLIENT_RELEASE_MUTEX			0x6		/* Rajzolási mutex elengedése (csak NT) */
#define DGCLIENT_VESA					0x7
#define DGCLIENT_BEGINVESA				0x8
#define DGCLIENT_ENDVESA				0x9
#define DGCLIENT_INSTALLMOUSE			0xA
#define DGCLIENT_UNINSTALLMOUSE			0xB
#define DGCLIENT_GETVERSION				0xC

/*------------------------------------------------------------------------------------------*/
/* Az egyes Glide-fûggvények kódjai */

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

/* Glide1-függvények */
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

/* VESA-függvények */
#define VESASETVBEMODE					111
#define VESAUNSETVBEMODE				112
#define VESAFRESH						113

/* Ezeket a függvényeket a kernelmodul hivja */
#define DGVOODOOCLIENTCRASHED			114
#define DGVOODOONEWCLIENTREGISTERING	115

/* Egyéb függvények */
#define DGVOODOOGETCONFIG				116
#define DGVOODOORELEASEFOCUS			117

#define GLIDEINSTALLMOUSE				118
#define GLIDEUNINSTALLMOUSE				119
#define GLIDEISMOUSEINSTALLED			120

#define VESAGETXRES						121
#define VESAGETYRES						122

#define PORTLOG_IN						123
#define PORTLOG_OUT						124


/* Ez a kód zárja le a kódszekvenciát */
#define GLIDEENDITEM					0xFFFFFFFF

#ifdef __MSC__
/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

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
/*.................................. Globális változók .....................................*/
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