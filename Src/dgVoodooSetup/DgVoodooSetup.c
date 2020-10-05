/*--------------------------------------------------------------------------------- */
/* dgVoodooSetup.c - dgVoodooSetup main GUI implementation                          */
/*                                                                                  */
/* Copyright (C) 2004 Dege                                                          */
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
/* dgVoodoo: dgVoodooSetup.c																*/
/*			 Setup																			*/
/*------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "uxtheme.h"
#include "resource.h"
#include "DDraw.h"
#include "d3d.h"
#include "movie.h"
#include "file.h"
#include "Version.h"
#include "dgVoodooConfig.h"
#include "CmdLine.h"

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/

#define	PWINDOWS				0
#define PDOS					1
#define PUNKNOWN				0xFF

#define	DTYPE_FLAGMASKTRUE		0
#define	DTYPE_FLAG2MASKTRUE		1 
#define	DTYPE_FLAGMASKFALSE		2
#define	DTYPE_FLAG2MASKFALSE	3
#define	DTYPE_FUNCTION			4
#define DTYPE_FUNCTIONFALSE		5
#define DTYPE_PLATFORM			6


#define	HWND_MAIN				0
#define	HWND_GLOBAL				1
#define	HWND_GLIDE				2
#define	HWND_VESA				3
#define HWND_HEADER				4

#define CFG_XPSTYLE				1

#define	MAXSTRINGLENGTH			1024

typedef struct	{
	
	unsigned short	productVersion;
	unsigned int	flags;
	unsigned int	reserved[7];

} SetupConfig;


typedef struct	{

	unsigned int	type;
	unsigned int	data;
	int				numofdependingitems;
	unsigned int	depending;
	unsigned int	dependingitems[];

} dependency;


typedef struct	{

	unsigned int	depending;
	unsigned int	dependingitem;
	unsigned int	conjunction;

} multipledepitem;


typedef struct	{

	unsigned short x;
	unsigned short y;

} displaymode;


typedef struct	{

	unsigned char *name;
	unsigned char *remark;

} _InstanceInfo;


typedef	void (_stdcall *SetThemeAppPropertiesProc)(DWORD);
typedef int (CALLBACK *DialogProc)(HWND, UINT, WPARAM, LPARAM);

enum dialogPages	{

	pageTop			=	0,
	pageGlobal		=	1,
	pageGlide		=	2,
	pageVesa		=	3,
	pageBottom		=	4,
	numberOfPages	=	5
};

/*------------------------------------------------------------------------------------------*/
/*.................................. Belsõ függvények ......................................*/

int					SaveConfiguration(char *filename);
int					LoadConfiguration(char *filename);
int					WritedgVoodooConfig(unsigned char *filename, unsigned int platform);
int					ReaddgVoodooConfig(unsigned char *filename, unsigned int config);
void				DisableConfig();
void				EnableConfig();
void				SetWindowToConfig();
LRESULT CALLBACK	dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				ActivateProperPage();
int					CheckEnablingDependencies(HWND hwnd, unsigned int item);
int					IsHwBuffCachingEnabled();
int					IsRescaleChangesOnlyEnabled();
int					IsItGlide211Config();

int CALLBACK		DialogTopProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK		DialogBottomProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK		DialogGlobalDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK		DialogGlideDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK		DialogVESADialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------------------*/
/*................................... Globális cuccok ......................................*/

char			*texMemSizeStr[]	= { "1024 kB", "2048 kB", "3072 kB", "4096 kB", "5120 kB", "6144 kB",
										"7168 kB", "8192 kB", "10240 kB", "12288 kB", "14336 kB", "16384 kB" };
char			*vidMemSizeStr[]	= {"256 kB","512 kB","1024 kB","2048 kB","3072 kB","4096 kB","5120 kB"};
unsigned int	lfbAccessStrs[numberOfLanguages][4] =
				{ {IDS_ENABLELFBACCESS, IDS_LFBDISABLEREAD, IDS_DISABLELFBWRITE, IDS_DISABLELFBACCESS},
				  {IDS_ENABLELFBACCESS_HUN, IDS_LFBDISABLEREAD_HUN, IDS_DISABLELFBWRITE_HUN, IDS_DISABLELFBACCESS_HUN}
				};

unsigned int	ckMethodStrs[numberOfLanguages][4] =
				{ {IDS_CKAUTOMATIC, IDS_CKALPHABASED, IDS_CKNATIVE, IDS_NATIVEFORTNT},
				  {IDS_CKAUTOMATIC_HUN, IDS_CKALPHABASED_HUN, IDS_CKNATIVE_HUN, IDS_NATIVEFORTNT_HUN}
				};

unsigned int	refreshFreqStrs[numberOfLanguages][1] =
				{	{IDS_REFRESHDEFAULT},
					{IDS_REFRESHDEFAULT_HUN} };

unsigned int	resolutionStrs[numberOfLanguages][1] =
				{	{IDS_RESDEFAULT},
					{IDS_RESDEFAULT_HUN} };


unsigned int	driverStrs[numberOfLanguages][1] =
				{	{IDS_DDRIVERDEFAULT},
					{IDS_DDRIVERDEFAULT_HUN} };

unsigned int	deviceStrs[numberOfLanguages][1] =
				{	{IDS_DDEVICEUNKNOWN},
					{IDS_DDEVICEUNKNOWN_HUN} };

unsigned int	instanceStrs[numberOfLanguages][1] =
				{	{IDS_NOWRAPPERINSTANCE},
					{IDS_NOWRAPPERINSTANCE_HUN} };

unsigned int	desktopModeStrs[numberOfLanguages][1] =
				{	{IDS_DESKTOPVIDEOMODE},
					{IDS_DESKTOPVIDEOMODE_HUN} };

unsigned int	desktopModeUnknownStrs[numberOfLanguages][1] =
				{	{IDS_DESKTOPVIDEOMODEUNKNOWN},
					{IDS_DESKTOPVIDEOMODEUNKNOWN_HUN} };

unsigned int	incompatibleWrapperFileStrs[numberOfLanguages][1] =
				{	{IDS_INCOMPATIBELWRAPPERFILE},
					{IDS_INCOMPATIBELWRAPPERFILE_HUN} };

unsigned int	invalidWrapperFileStrs[numberOfLanguages][1] =
				{	{IDS_INVALIDWRAPPERFILE},
					{IDS_INVALIDWRAPPERFILE_HUN} };

unsigned int	invalidWrapperFile2Strs[numberOfLanguages][1] =
				{	{IDS_INVALIDWRAPPERFILE2},
					{IDS_INVALIDWRAPPERFILE2_HUN} };

unsigned int	noWrapperFileFoundStrs[numberOfLanguages][1] =
				{	{IDS_NOWRAPPERFILEFOUND},
					{IDS_NOWRAPPERFILEFOUND_HUN} };

unsigned int	tabPaneStrs[numberOfLanguages][3] =
				{	{IDS_TABPANESTR1, IDS_TABPANESTR2, IDS_TABPANESTR3},
					{IDS_TABPANESTR1_HUN, IDS_TABPANESTR2_HUN, IDS_TABPANESTR3_HUN} };

unsigned int	errorStrs[numberOfLanguages][1] =
				{	{IDS_ERROR},
					{IDS_ERROR_HUN} };

unsigned int	cannotExportStrs[numberOfLanguages][1] =
				{	{IDS_CANNOTEXPORTCONFIG},
					{IDS_CANNOTEXPORTCONFIG_HUN} };

unsigned int	cannotSaveConfigStrs[numberOfLanguages][1] =
				{	{IDS_CANNOTSAVECONFIG},
					{IDS_CANNOTSAVECONFIG_HUN} };

unsigned int	cannotLoadConfigStrs[numberOfLanguages][1] =
				{	{IDS_CANNOTLOADCONFIG},
					{IDS_CANNOTLOADCONFIG_HUN} };

unsigned int	nameOfTextFileStrs[numberOfLanguages][1] =
				{	{IDS_NAMEOFTEXTFILE},
					{IDS_NAMEOFTEXTFILE_HUN} };

unsigned int	nameOfConfigFileStrs[numberOfLanguages][1] =
				{	{IDS_NAMEOFCONFIGFILE},
					{IDS_NAMEOFCONFIGFILE_HUN} };

unsigned int	searchingInstanceStrs[numberOfLanguages][1] =
				{	{IDS_SEARCHINGINSTANCE},
					{IDS_SEARCHINGINSTANCE_HUN} };

unsigned int	invalidConfigFileStrs[numberOfLanguages][1] =
				{	{IDS_INVALIDCONFIGFILE},
					{IDS_INVALIDCONFIGFILE_HUN} };

unsigned int	errorDuringConfigReadStrs[numberOfLanguages][1] =
				{	{IDS_ERRORDURINGCONFIGREAD},
					{IDS_ERRORDURINGCONFIGREAD_HUN} };

unsigned int	cannotWriteConfigStrs[numberOfLanguages][1] =
				{	{IDS_CANNOTWRITECONFIG},
					{IDS_CANNOTWRITECONFIG_HUN} };

unsigned int	errorDuringSaveStrs[numberOfLanguages][1] =
				{	{IDS_ERRORDURINGSAVE},
					{IDS_ERRORDURINGSAVE_HUN} };

unsigned int	invalidExtConfigStrs[numberOfLanguages][1] =
				{	{IDS_INCOMPATIBLEEXTVERSION},
					{IDS_INCOMPATIBLEEXTVERSION_HUN} };


char			tempstr[256];
char			filenamew[MAX_PATH];
char			cfgRemarkStr[]		= "\n;Current settings of the wrapper will be overrided by this file if a glide application is started from this directory!";
_InstanceInfo	instanceInfo[]		= { {"glide.dll", " (Glide 1)"}, {"glide2x.dll", " (Glide 2)"}, {NULL,NULL} };

unsigned int	texMemSizes[]		= {1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 10240, 12288, 14336, 16384 };
unsigned int	vidMemSizes[]		= {256, 512, 1024, 2048, 3072, 4096, 5120};
unsigned char	hwbuffcaching_valid_selections[] = {0, 1, 2};
unsigned char	fastwrite_valid_selections[] = {0,1};

dgVoodooConfig	config;

dgVoodooConfig	configs[2]			= { {DGVOODOOVERSION, 0x243, 8192 << 10,
										 CFG_HWLFBCACHING | CFG_RELYONDRIVERFLAGS | CFG_LFBFASTUNMATCHWRITE |
										 CFG_PERFECTTEXMEM | CFG_STOREMIPMAP | (CFG_CKMAUTOMATIC << 5),
										 100, 0, 0, 0, 0, 0, 0, 0, 0,
										 CFG_HIDECONSOLE | CFG_FORCETRILINEARMIPMAPS | CFG_LFBRESCALECHANGESONLY,
										 0, 0, English},

										{DGVOODOOVERSION, 0x243, 8192 << 10,
										 CFG_HWLFBCACHING | CFG_RELYONDRIVERFLAGS | CFG_LFBFASTUNMATCHWRITE |
										 CFG_PERFECTTEXMEM | CFG_STOREMIPMAP | (CFG_CKMAUTOMATIC << 5) | CFG_NTVDDMODE,
										 100, 0, 1024, 50, 0, 0, 0, 0, 0,
										 CFG_HIDECONSOLE | CFG_FORCETRILINEARMIPMAPS | CFG_LFBRESCALECHANGESONLY,
										 80, 0, English} };

SetupConfig		setupConfig;

OPENFILENAME	ofw = {sizeof(OPENFILENAME)};
OPENFILENAME	*of = &ofw;
unsigned int	platform = PUNKNOWN;
unsigned int	lang;
unsigned int	refrate_x, refrate_y, refrate_bpp;

dependency		_ed0	= {DTYPE_FLAGMASKTRUE, CFG_WINDOWED, 3, HWND_GLOBAL, IDC_SBDBOX,IDC_16BIT,IDC_32BIT};
dependency		_ed1	= {DTYPE_FLAGMASKFALSE, CFG_GRABENABLE, 2, HWND_GLOBAL, IDC_SAVETOFILE, IDC_SAVETOCLIPBOARD};
dependency		_ed2	= {DTYPE_FLAGMASKFALSE, CFG_VESAENABLE, 11, HWND_VESA, IDC_GBVESAREFFREQ,
																			   IDC_SLIDER2,
																			   IDC_SLIDERSTATIC2,
																			   IDC_45HZ,
																			   IDC_70HZ,
																			   IDC_GBVESAVIDMEM,
																			   IDC_VESALABEL2,
																			   IDC_COMBOVIDMEMSIZE,
																			   IDC_GBVESAMISC,
																			   IDC_SUPPORTMODE13H,
																			   IDC_DISABLEFLATLFB};
dependency		_ed3	= {DTYPE_FUNCTION, (unsigned int) IsHwBuffCachingEnabled, 1, HWND_GLIDE, IDC_HWBUFFCACHING};
dependency		_ed4	= {DTYPE_FLAG2MASKTRUE, CFG_DISABLELFBWRITE, 1, HWND_GLIDE, IDC_FASTWRITE};
dependency		_ed5	= {DTYPE_PLATFORM, PWINDOWS, 2, HWND_GLIDE, IDC_TOMBRAIDER, IDC_TIMERLOOP};
dependency		_ed6	= {DTYPE_PLATFORM, PWINDOWS, 10, HWND_GLOBAL, IDC_MISC,IDC_HIDECONSOLE,IDC_SETMOUSEFOCUS,
						   IDC_WINXPDOS,IDC_VDDMODE,IDC_ACTIVEBACKG,IDC_CTRLALT,IDC_CLIPOPF, IDC_PRESERVEWINDOWSIZE,
						   IDC_PRESERVEASPECTRATIO};
dependency		_ed7	= {DTYPE_FLAGMASKFALSE, CFG_SETMOUSEFOCUS, 1, HWND_GLOBAL, IDC_CTRLALT};
dependency		_ed8	= {DTYPE_FLAGMASKTRUE, CFG_DISABLEMIPMAPPING, 1, HWND_GLIDE, IDC_AUTOGENMIPMAPS};
dependency		_ed9	= {DTYPE_FLAGMASKTRUE, CFG_WINDOWED, 3, HWND_GLIDE, IDC_CLOSESTFREQ, IDC_REFRATESTATIC, IDC_REFRESHRATEVALUE};
dependency		_ed10	= {DTYPE_FLAG2MASKTRUE, CFG_CLOSESTREFRESHFREQ, 4, HWND_GLIDE, IDC_REFRATESTATIC, IDC_REFRESHRATEVALUE, IDC_RRMONITOR, IDC_RRAPP};
dependency		_ed11	= {DTYPE_FLAGMASKTRUE, CFG_DISABLEMIPMAPPING, 1, HWND_GLIDE, IDC_TRILINEARMIPMAPS};
dependency		_ed12	= {DTYPE_FUNCTION, (unsigned int) IsRescaleChangesOnlyEnabled, 1, HWND_GLIDE, IDC_RESCALECHANGES};
dependency		_ed13	= {DTYPE_FUNCTIONFALSE, (unsigned int) IsItGlide211Config, 1, HWND_GLIDE, IDC_REALHW};
//dependency		_ed14	= {DTYPE_FLAG2MASKTRUE, CFG_LFBDISABLETEXTURING, 1, HWND_GLIDE, IDC_FASTWRITE};
dependency		_ed15	= {DTYPE_FLAG2MASKTRUE, CFG_DISABLELFBWRITE, 1, HWND_GLIDE, IDC_LFBTEXTURETILES};
dependency		_ed16	= {DTYPE_FLAGMASKFALSE, CFG_WINDOWED, 2, HWND_GLOBAL, IDC_PRESERVEWINDOWSIZE, IDC_PRESERVEASPECTRATIO};
dependency		_ed17	= {DTYPE_FUNCTIONFALSE, (unsigned int) IsItGlide211Config, 1, HWND_HEADER, IDC_PDOS};

dependency		*dependencytable[] = { &_ed0, &_ed1, &_ed2, &_ed3, &_ed4, &_ed5, &_ed6, &_ed7, &_ed8, &_ed9, &_ed10,
									   &_ed11, &_ed12, &_ed13, /*&_ed14,*/ &_ed15, &_ed16, &_ed17, NULL };

multipledepitem _md[] = {	{HWND_GLOBAL, IDC_CTRLALT, 0}, {HWND_GLIDE, IDC_REFRESHRATEVALUE, 0},
							{HWND_GLIDE, IDC_REFRATESTATIC, 0}, {HWND_GLIDE, IDC_FASTWRITE, 0},
							{HWND_GLOBAL, IDC_PRESERVEWINDOWSIZE, 0}, {HWND_GLOBAL, IDC_PRESERVEASPECTRATIO},
							{0xFFFFFFFF} };

displaymode		enumdispmodes[48];
unsigned int	enumdispmodesnum;
unsigned int	enumfrequencies[16];
unsigned int	enumfreqnum;
int				instancenum;
int				configvalid = 0;

WNDCLASS		dlgClass = {0, dlgWindowProc, 0, DLGWINDOWEXTRA, 0, NULL, NULL, (HBRUSH) COLOR_WINDOW, NULL, "DGCLASS"};
unsigned int	pageIDs[numberOfLanguages][numberOfPages] =
				{	{IDD_DIALOGTOP_ENG, IDD_DIALOGGLOBAL_ENG, IDD_DIALOGGLIDE_ENG, IDD_DIALOGVESA_ENG, IDD_DIALOGBOTTOM_ENG},
					{IDD_DIALOGTOP_HUN, IDD_DIALOGGLOBAL_HUN, IDD_DIALOGGLIDE_HUN, IDD_DIALOGVESA_HUN, IDD_DIALOGBOTTOM_HUN} };

HWND			pages[numberOfLanguages][numberOfPages];

DialogProc		pageWindowProc[numberOfPages] = {DialogTopProc, DialogGlobalDialogProc,
												 DialogGlideDialogProc, DialogVESADialogProc,
												 DialogBottomProc};

unsigned int	aboutDlgIDs[numberOfLanguages] = {IDD_DIALOGABOUT, IDD_DIALOGABOUT_HUN};
HWND			hDlgMain;
HWND			hDlgGlobal;
HWND			hDlgGlide;
HWND			hDlgVESA;
HWND			hDlgTop;
HWND			hDlgBottom;
HWND			hActPage;
unsigned int	floatingMenuIDs[numberOfLanguages] = {IDR_FLOATINGMENU_ENG, IDR_FLOATINGMENU_HUN};
HMENU			hFloatingMenus[numberOfLanguages];
HINSTANCE		uxThemeHandle;
int				themingAvailable;

SetThemeAppPropertiesProc pSetThemeAppProperties;
int				cmdLineMode;


void			GetString (LPSTR lpBuffer, UINT uID);
int				EmptyCommandLine (LPSTR cmdLine);

/*------------------------------------------------------------------------------------------*/
/*...................................... Függvények ........................................*/
#include "cfgexp.c"


void	GetString (LPSTR lpBuffer, UINT uID)
{
	LoadString (dlgClass.hInstance, uID, lpBuffer, MAXSTRINGLENGTH);
}


int IsHwBuffCachingEnabled()	{

	return ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) != (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) );
	//return ( (!(config.Flags2 & CFG_DISABLELFBREAD)) || ( (!(config.Flags2 & CFG_DISABLELFBWRITE)) && (!(config.Flags2 & CFG_LFBFORCETEXTURING)) ) );
}


int IsItGlide211Config()	{

	return (config.GlideVersion == 0x211);
}


int IsRescaleChangesOnlyEnabled()	{
	if ( (config.Flags2 & CFG_DISABLELFBWRITE) || (!(config.Flags & CFG_LFBFASTUNMATCHWRITE)) || ( (config.dxres==0) && (config.dyres==0) ) ) return(FALSE);
	return(TRUE);
}


void dgVSsetScreenModeBox()	{
unsigned int	f,w;

	if (config.Flags & CFG_WINDOWED)	{
		f = BST_UNCHECKED;
		w = BST_CHECKED;
	} else {
		w = BST_UNCHECKED;
		f = BST_CHECKED;
	}
	SendMessage(GetDlgItem(hDlgGlobal,IDC_FULLSCR), BM_SETCHECK, f, 0);
	SendMessage(GetDlgItem(hDlgGlobal,IDC_WINDOWED), BM_SETCHECK, w, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetScreenBitDepthBox()	{
unsigned int	_16,_32;

	if (config.Flags & CFG_DISPMODE32BIT)	{
		_16 = BST_UNCHECKED;
		_32 = BST_CHECKED;
	} else {
		_32 = BST_UNCHECKED;
		_16 = BST_CHECKED;
	}
	SendMessage(GetDlgItem(hDlgGlobal, IDC_16BIT), BM_SETCHECK, _16, 0);
	SendMessage(GetDlgItem(hDlgGlobal, IDC_32BIT), BM_SETCHECK, _32, 0);
}


void dgVSsetTexBitDepthBox()	{
	
	SendMessage(GetDlgItem(hDlgGlide, IDC_TEX32), BM_SETCHECK, BST_CHECKED, 0);
}


void dgVSsetRefRateBox()	{
unsigned int	m,a;

	if (config.Flags & CFG_SETREFRESHRATE)	{
		a = BST_CHECKED;
		m = BST_UNCHECKED;
	} else {
		m = BST_CHECKED;
		a = BST_UNCHECKED;
	}
	SendMessage(GetDlgItem(hDlgGlide, IDC_RRMONITOR), BM_SETCHECK, m, 0);
	SendMessage(GetDlgItem(hDlgGlide, IDC_RRAPP), BM_SETCHECK, a, 0);
}


void dgVSsetClosestRefRateBox()	{
unsigned int	m;

	m = (config.Flags2 & CFG_CLOSESTREFRESHFREQ) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide,IDC_CLOSESTFREQ), BM_SETCHECK, m, 0);	
	CheckEnablingDependencies(0, 0);
}


void dgVSsetTextureBox()	{
unsigned int	tex16, tex32, texopt;

	tex16 = BST_UNCHECKED;
	tex32 = BST_UNCHECKED;
	texopt = BST_CHECKED;
	if (config.Flags & CFG_TEXTURES32BIT)	{
		tex16 = BST_UNCHECKED;
		tex32 = BST_CHECKED;
		texopt = BST_UNCHECKED;
	}
	if (config.Flags & CFG_TEXTURES16BIT)	{
		tex16 = BST_CHECKED;
		tex32 = BST_UNCHECKED;
		texopt = BST_UNCHECKED;
	}
	SendMessage(GetDlgItem(hDlgGlide, IDC_TEX16), BM_SETCHECK, tex16, 0);
	SendMessage(GetDlgItem(hDlgGlide, IDC_TEX32), BM_SETCHECK, tex32, 0);
	SendMessage(GetDlgItem(hDlgGlide, IDC_TEXOPT), BM_SETCHECK, texopt, 0);
}


void dgVSsetHwBuffCachingBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_HWLFBCACHING) ? BST_CHECKED :  BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_HWBUFFCACHING), BM_SETCHECK, checkState, 0);
}


void dgVSsetFastWriteBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_LFBFASTUNMATCHWRITE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_FASTWRITE), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetTextureTilesBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_LFBDISABLETEXTURING) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_LFBTEXTURETILES), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetRealHwBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_LFBREALVOODOO) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_REALHW), BM_SETCHECK, checkState, 0);
}


void dgVSsetCKComboBox() {

	SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOCOLORKEY), CB_SETCURSEL, GetCKMethodValue(config.Flags), 0);
}


void dgVSsetHideConsoleBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_HIDECONSOLE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_HIDECONSOLE), BM_SETCHECK, checkState, 0);
}


void dgVSsetSetMouseFocusBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_SETMOUSEFOCUS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_SETMOUSEFOCUS), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetVDDModeBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_NTVDDMODE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_VDDMODE), BM_SETCHECK, checkState, 0);
}


void dgVSsetCTRLALTBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_CTRLALT) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_CTRLALT), BM_SETCHECK, checkState, 0);
}


void dgVSsetActiveBackgroundBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_ACTIVEBACKGROUND) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_ACTIVEBACKG), BM_SETCHECK, checkState, 0);
}


void dgVSsetPreserveWindowSizeBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_PRESERVEWINDOWSIZE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_PRESERVEWINDOWSIZE), BM_SETCHECK, checkState, 0);
}


void dgVSsetPreserveAspectRatioBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_PRESERVEASPECTRATIO) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_PRESERVEASPECTRATIO), BM_SETCHECK, checkState, 0);
}


void dgVSsetCLIPOPFBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_FORCECLIPOPF) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_CLIPOPF), BM_SETCHECK, checkState, 0);

}


void dgVSsetSupportMode13hBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_SUPPORTMODE13H) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgVESA, IDC_SUPPORTMODE13H), BM_SETCHECK, checkState, 0);
}


void dgVSsetSupportDisableFlatLFBBox() {
unsigned int	checkState;
	
	checkState = (config.Flags2 & CFG_DISABLEVESAFLATLFB) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgVESA, IDC_DISABLEFLATLFB), BM_SETCHECK, checkState, 0);
}


void dgVSsetTombRaiderBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_TOMBRAIDER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide,IDC_TOMBRAIDER), BM_SETCHECK, checkState, 0);
}


void dgVSsetTripleBufferBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_TRIPLEBUFFER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_TRIPLEBUFFER), BM_SETCHECK, checkState, 0);
}


void dgVSsetVSyncBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_VSYNC) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_VSYNC), BM_SETCHECK, checkState, 0);
}


void dgVSsetGammaRampBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_ENABLEGAMMARAMP) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_GAMMARAMP), BM_SETCHECK, checkState, 0);
}


void dgVSsetClippingBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_CLIPPINGBYD3D) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_CLIPPING), BM_SETCHECK, checkState, 0);
}


void dgVSsetHwVBuffBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_ENABLEHWVBUFFER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_HWVBUFFER), BM_SETCHECK, checkState, 0);
}


void dgVSsetForceWBufferingBox()	{
unsigned int	checkState;

	checkState = ((config.Flags & (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) == (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_FORCEWBUFFERING), BM_SETCHECK, checkState, 0);
}


void dgVSsetTimerBoostBox ()		{
unsigned int	checkState;

	checkState = config.dosTimerBoostPeriod ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_TIMERLOOP), BM_SETCHECK, checkState, 0);
}

void dgVSsetPerfectTexMemBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_PERFECTTEXMEM) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_PERFECTTEXEMU), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetMipMappingDisableBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_DISABLEMIPMAPPING) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_MIPMAPPINGDISABLE), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetTrilinearMipMapsBox()	{
unsigned int	checkState;

	checkState = (config.Flags2 & CFG_FORCETRILINEARMIPMAPS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_TRILINEARMIPMAPS), BM_SETCHECK, checkState, 0);
}


void dgVSsetAutogenerateMipMapsBox()	{
unsigned int	checkState;

	checkState = (config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_AUTOGENMIPMAPS), BM_SETCHECK, checkState, 0);
}


void dgVSsetBilinearFilteringBox()	{
unsigned int	checkState;

	checkState = (config.Flags & CFG_BILINEARFILTER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlide, IDC_BILINEARFILTERING), BM_SETCHECK, checkState, 0);
}


void dgVSsetCaptureOnBox()	{
unsigned int	checkState;
	
	checkState = (config.Flags & CFG_GRABENABLE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetDlgItem(hDlgGlobal, IDC_SCAPTUREON), BM_SETCHECK, checkState, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetGrabModeBox()	{
unsigned int	sfile, sclipb;
	
	if (config.Flags & CFG_GRABCLIPBOARD)	{
		sfile = BST_UNCHECKED;
		sclipb = BST_CHECKED;
	} else {
		sfile = BST_CHECKED;
		sclipb = BST_UNCHECKED;
	}
	SendMessage(GetDlgItem(hDlgGlobal, IDC_SAVETOFILE), BM_SETCHECK, sfile, 0);
	SendMessage(GetDlgItem(hDlgGlobal, IDC_SAVETOCLIPBOARD), BM_SETCHECK, sclipb, 0);	
}


void dgVSsetGammaControl()	{
char	slider[8];
	
	SendMessage(GetDlgItem(hDlgGlide, IDC_SLIDER1), TBM_SETPOS, (LPARAM) TRUE, (WPARAM) config.Gamma);
	sprintf(slider, "%d%%", config.Gamma);
	SendMessage(GetDlgItem(hDlgGlide, IDC_SLIDERSTATIC), WM_SETTEXT, (LPARAM) 0, (WPARAM) slider);
}


void dgVSsetRefreshFreq()	{
char	slider[8];
	
	SendMessage(GetDlgItem(hDlgVESA, IDC_SLIDER2), TBM_SETPOS, (LPARAM) TRUE, (WPARAM) config.VideoRefreshFreq);
	sprintf(slider, "%dHz", config.VideoRefreshFreq);
	SendMessage(GetDlgItem(hDlgVESA, IDC_SLIDERSTATIC2), WM_SETTEXT, (LPARAM) 0, (WPARAM) slider);
}


void dgVSsetVESAEnableBox()	{
unsigned int	j,v;

	if (config.Flags & CFG_VESAENABLE)	{
		v = BST_CHECKED;
		j = 1;
	} else {
		v = BST_UNCHECKED;
		j = 0;
	}
	SendMessage(GetDlgItem(hDlgVESA, IDC_VESAENABLE), BM_SETCHECK, v, 0);
	CheckEnablingDependencies(0, 0);
}


void dgVSsetLang()	{
TCITEM			tabItem;
unsigned char	strBuffer[MAXSTRINGLENGTH];
	
	if (lang == config.language) return;

	tabItem.mask = TCIF_TEXT;
	tabItem.pszText = (GetString (strBuffer, tabPaneStrs[config.language][0]), strBuffer);
	TabCtrl_SetItem (GetDlgItem(hDlgMain, IDC_MAINTAB), 0, &tabItem);

	tabItem.pszText = (GetString (strBuffer, tabPaneStrs[config.language][1]), strBuffer);
	TabCtrl_SetItem (GetDlgItem(hDlgMain, IDC_MAINTAB), 1, &tabItem);

	tabItem.pszText = (GetString (strBuffer, tabPaneStrs[config.language][2]), strBuffer);
	TabCtrl_SetItem (GetDlgItem(hDlgMain, IDC_MAINTAB), 2, &tabItem);

	hDlgGlobal = pages[config.language][pageGlobal];
	hDlgGlide = pages[config.language][pageGlide];
	hDlgVESA = pages[config.language][pageVesa];
	hDlgTop = pages[config.language][pageTop];
	hDlgBottom = pages[config.language][pageBottom];
	
	ShowWindow(pages[config.language][pageTop], SW_SHOWNORMAL);
	ShowWindow(pages[config.language][pageBottom], SW_SHOWNORMAL);
	ShowWindow(pages[lang][pageTop], SW_HIDE);
	ShowWindow(pages[lang][pageBottom], SW_HIDE);
	
	lang = config.language;
	ActivateProperPage();					
}


void dgVSsetPlatformButtons(unsigned int newplatf)	{
unsigned int	pwin, pdos;
TC_ITEM			tabitem;
unsigned int	i;

	configs[platform] = config;
	platform = newplatf;
	if (newplatf == PWINDOWS)	{
		pwin = BST_CHECKED;
		pdos = BST_UNCHECKED;
		if (TabCtrl_GetCurSel(GetDlgItem(hDlgMain, IDC_MAINTAB)) == 2)	{
			TabCtrl_SetCurSel(GetDlgItem(hDlgMain, IDC_MAINTAB), 1);
		}
		TabCtrl_DeleteItem(GetDlgItem(hDlgMain, IDC_MAINTAB), 2);
		ActivateProperPage();
	} else {
		pdos = BST_CHECKED;
		pwin = BST_UNCHECKED;
		tabitem.mask = TCIF_TEXT;		
		tabitem.pszText = "VESA";
		SendMessage(GetDlgItem(hDlgMain, IDC_MAINTAB), TCM_INSERTITEM, (WPARAM) 2, (LPARAM) &tabitem);
		ActivateProperPage();
	}
	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageTop], IDC_PWINDOWS), BM_SETCHECK, (LPARAM) pwin, (WPARAM) 0);
		SendMessage(GetDlgItem(pages[i][pageTop], IDC_PDOS), BM_SETCHECK, (LPARAM) pdos, (WPARAM) 0);	
	}

	CopyMemory(&config, configs + newplatf, sizeof(dgVoodooConfig));
	dgVSsetLang();
	SetWindowToConfig();
}


void SetStringsForOpenFileName(unsigned char *strBuffer)	{

	if (lang == LANG_HUNGARIAN)	{
		ofw.lpstrFilter = "Dynamic Link Libraries (*.dll)\0*.dll\0Összes fájl (*.*)\0*.*\0\0\0";
	} else {
		ofw.lpstrFilter = "Dynamic Link Libraries (*.dll)\0*.dll\0All files (*.*)\0*.*\0\0\0";
	}
	ofw.lpstrTitle = (GetString (strBuffer, searchingInstanceStrs[lang][0]), strBuffer);
}


void AddInstance(unsigned char *instancename)	{
unsigned int	i, j;

	if (cmdLineMode)
		return;

	if (!instancenum)	{
		for (i = 0; i < numberOfLanguages; i++) {
			SendMessage(GetDlgItem(pages[i][pageTop], IDC_INSTANCELIST), CB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(pages[i][pageTop], IDC_INSTANCELIST), CB_RESETCONTENT, 0, 0);
		}
	}
	if ((i = SendMessage(GetDlgItem(pages[English][pageTop], IDC_INSTANCELIST), CB_FINDSTRING, (WPARAM) -1, (LPARAM) instancename)) != CB_ERR)	{
		for (j = 0; j < numberOfLanguages; j++) {
			SendMessage(GetDlgItem(pages[j][pageTop], IDC_INSTANCELIST), CB_DELETESTRING, i, 0);
		}
	}
	for (j = 0; j < numberOfLanguages; j++) {
		SendMessage(GetDlgItem(pages[j][pageTop], IDC_INSTANCELIST), CB_INSERTSTRING, (WPARAM) 0, (LPARAM) instancename);
	}
	instancenum++;
}


void SearchInstances(unsigned char *buffer)	{
int				i, j, pos, postemp;
	
	for(pos = 0; buffer[pos]; ) pos++;
	if (buffer[pos - 1] != '\\') buffer[pos++] = '\\';

	i = 0;
	while(instanceInfo[i].name != NULL)	{
		postemp = pos;
		for(j = 0; instanceInfo[i].name[j]; j++) buffer[postemp++] = instanceInfo[i].name[j];
		buffer[postemp] = 0;
		if (!ReaddgVoodooConfig(buffer, PWINDOWS))	{
			buffer[postemp] = 0;
			AddInstance(buffer);
		}
		i++;
	}
}


void SelectInstance(int index)	{
char			buffer[MAX_PATH];
int				readVal;

	if (instancenum)	{
		SendMessage(GetDlgItem(pages[English][pageTop], IDC_INSTANCELIST), CB_GETLBTEXT, (LPARAM) index,(WPARAM) buffer);
		readVal = ReaddgVoodooConfig(buffer, platform);
		switch(readVal)	{
			case 0:
				EnableConfig();
				break;
			case 2:
				DisableConfig (incompatibleWrapperFileStrs, DGVOODOOVERSION_STR);
				break;

			default:
			case 1:
				DisableConfig (invalidWrapperFileStrs, NULL);
				break;

		}
		dgVSsetLang();
		SetWindowToConfig();
		if (IsItGlide211Config()) {
			dgVSsetPlatformButtons(PWINDOWS);
		}
	}
}


HRESULT CALLBACK CallbackRenderingPrimitives(LPSTR DevDesc,
											 LPSTR DevName,
											 LPD3DDEVICEDESC7 lpD3DDeviceDesc,
											 LPVOID lpContext)	{
unsigned int i;
	
	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDRIVERS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) DevName);
	}
	return(D3DENUMRET_OK);
}


HRESULT WINAPI CallbackDisplayModes(LPDDSURFACEDESC2 lpddsd, LPVOID lpContext)	{
short			x, y;
unsigned int	i;

	if ( (lpddsd->ddpfPixelFormat.dwRGBBitCount!=16) && (lpddsd->ddpfPixelFormat.dwRGBBitCount!=32) )
		return(DDENUMRET_OK);

	x = (short) lpddsd->dwWidth;
	y = (short) lpddsd->dwHeight;
	for(i = 0; i < enumdispmodesnum; i++)	{
		if ( (x == enumdispmodes[i].x) && (y == enumdispmodes[i].y) ) return(DDENUMRET_OK);
	}
	enumdispmodes[enumdispmodesnum].x = x;
	enumdispmodes[enumdispmodesnum++].y = y;
	return(DDENUMRET_OK);
}


HRESULT WINAPI CallbackRefreshRates(LPDDSURFACEDESC2 lpddsd, LPVOID lpContext)	{
unsigned int	i,j;

	i = (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16;
	if ( (lpddsd->ddpfPixelFormat.dwRGBBitCount == i) &&
		 (lpddsd->dwWidth == config.dxres) && (lpddsd->dwHeight == config.dyres) &&
		 (lpddsd->dwRefreshRate != 0) )	{

		for(i = 0; i < enumfreqnum; i++)	{
			if (enumfrequencies[i] == lpddsd->dwRefreshRate) return(DDENUMRET_OK);
			if (enumfrequencies[i] > lpddsd->dwRefreshRate) break;
		}
		for(j = enumfreqnum; j > i; j--) enumfrequencies[j] = enumfrequencies[j - 1];
		
		enumfrequencies[i] = lpddsd->dwRefreshRate;
		enumfreqnum++;
	}
	return(DDENUMRET_OK);
}


BOOL WINAPI DDEnumCallB(GUID FAR *lpGUID, LPSTR  lpDriverDescription, LPSTR  lpDriverName, LPVOID lpContext	)	{
unsigned int	i;

	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDEVS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) lpDriverDescription);
	}
	return(1);
}


void CreateRefreshRateList()	{
unsigned int	i, j;
unsigned char	strBuffer[MAXSTRINGLENGTH];

	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageGlide], IDC_REFRESHRATEVALUE), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
		SendMessage(GetDlgItem(pages[i][pageGlide], IDC_REFRESHRATEVALUE), CB_ADDSTRING,
							   (WPARAM) 0, (LPARAM) (GetString (strBuffer, refreshFreqStrs[i][0]), strBuffer));
	}
	for(i = 0; i < enumfreqnum; i++)	{
		strBuffer[0] = 0;
		sprintf(strBuffer, "%d Hz", enumfrequencies[i]);
		for (j = 0; j < numberOfLanguages; j++) {
			SendMessage(GetDlgItem(pages[j][pageGlide], IDC_REFRESHRATEVALUE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
		}
	}
}


void DoRefreshRateListSelection()	{
int		i;

	for(i = enumfreqnum; i > 0; i--) if (config.RefreshRate == enumfrequencies[i - 1]) break;
	// Ha a listában nincs az aktuális frekvencia
	if (i == 0) config.RefreshRate = 0;
	SendMessage(GetDlgItem(hDlgGlide, IDC_REFRESHRATEVALUE), CB_SETCURSEL, (WPARAM) i, (LPARAM) 0);
}


void EnumerateRefreshRates()	{
MOVIEDATA		movie;
LPDIRECTDRAW7	lpDD;

	if ( (config.dxres != refrate_x) || (config.dyres != refrate_y) || ((config.Flags & CFG_DISPMODE32BIT) != refrate_bpp) )	{

		enumfreqnum = 0;
		InitDraw(&movie, NULL);
		lpDD = movie.cmiDirectDraw;	
		if ( (lpDD == NULL) ) return;		
		lpDD->lpVtbl->EnumDisplayModes(lpDD, DDEDM_REFRESHRATES, NULL, 0, CallbackRefreshRates);
		CreateRefreshRateList ();
		UninitDraw ();
		refrate_x = config.dxres;
		refrate_y = config.dyres;
		refrate_bpp = config.Flags & CFG_DISPMODE32BIT;
	}
	DoRefreshRateListSelection ();
}


void EnumerateRenderingDevices()	{
MOVIEDATA		movie;
LPDIRECT3D7		lpD3D;	
LPDIRECTDRAW7	lpDD;
int				i;
unsigned int	j;
unsigned short	x, y;
char			strBuffer[MAXSTRINGLENGTH];

	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageGlide], IDC_COMBORESOLUTION), CB_ADDSTRING, (WPARAM) 0, (LPARAM) (GetString (strBuffer, resolutionStrs[i][0]), strBuffer));	

		SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDRIVERS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) (GetString (strBuffer, driverStrs[i][0]), strBuffer));
	}
	
	if (!EnumDisplayDevs(DDEnumCallB))	{
		for (i = 0; i < numberOfLanguages; i++) {
			SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDEVS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) (GetString (strBuffer, deviceStrs[i][0]), strBuffer));
		}
		return;
	}
	
	movie.cmiFlags = MV_3D;
	InitDraw(&movie, NULL);
	lpDD = movie.cmiDirectDraw;
	lpD3D = movie.cmiDirect3D;
	if ( (lpDD == NULL) || (lpD3D == NULL) ) return;
	enumdispmodesnum = 0;
	lpD3D->lpVtbl->EnumDevices(lpD3D, CallbackRenderingPrimitives, 0);
	lpDD->lpVtbl->EnumDisplayModes(lpDD, 0, NULL, 0, CallbackDisplayModes);
	
	/* Felbontások rendezése növekvõ sorrendbe */
 	for(i = enumdispmodesnum - 1; i > 0; i--) {
		for(j = 0; j < (unsigned int) i; j++) {
			if (enumdispmodes[j].x > enumdispmodes[j + 1].x) {
				x = enumdispmodes[j].x;
				y = enumdispmodes[j].y;
				enumdispmodes[j].x = enumdispmodes[j+1].x;
				enumdispmodes[j].y = enumdispmodes[j+1].y;
				enumdispmodes[j+1].x = x;
				enumdispmodes[j+1].y = y;
			}
		}
	}
	for(j = 0; j < enumdispmodesnum; j++) {
		strBuffer[0] = 0;
		sprintf(strBuffer, "%dx%d", enumdispmodes[j].x, enumdispmodes[j].y);
		for (i = 0; i < numberOfLanguages; i++) {
			SendMessage(GetDlgItem(pages[i][pageGlide], IDC_COMBORESOLUTION), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
		}
	}
	
	UninitDraw();
}


BOOL CALLBACK ThemeChangeEnumProc(HWND hwnd, LPARAM lParam) {

	SendMessage (hwnd, 0x31A, 0, 0);
	return (TRUE);
}


void SetEnableTheming () {
unsigned int	i;
			
	(*pSetThemeAppProperties) ((setupConfig.flags & CFG_XPSTYLE ? STAP_ALLOW_CONTROLS : 0) | STAP_ALLOW_NONCLIENT);
	EnumChildWindows (hDlgMain, ThemeChangeEnumProc, 0);
	for (i = 0; i < numberOfLanguages; i++) {
		EnumChildWindows (pages[i][pageGlobal], ThemeChangeEnumProc, 0);
		EnumChildWindows (pages[i][pageGlide], ThemeChangeEnumProc, 0);
		EnumChildWindows (pages[i][pageVesa], ThemeChangeEnumProc, 0);
		EnumChildWindows (pages[i][pageTop], ThemeChangeEnumProc, 0);
		EnumChildWindows (pages[i][pageBottom], ThemeChangeEnumProc, 0);
	}
	RedrawWindow (hDlgMain, NULL, NULL, RDW_INVALIDATE);
}


LRESULT CALLBACK dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	if (uMsg == WM_CLOSE)	{
		EndDialog(hwnd, 1);
		return(0);
	} else
		return(DefDlgProc(hwnd, uMsg, wParam, lParam));
}


int CALLBACK dgVoodooAboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	switch(uMsg)	{
		case WM_INITDIALOG:
			return(1);

		case WM_COMMAND:
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{
				case ID_OK:
					EndDialog(hwndDlg, 1);
					return(1);
					break;
			}
	}
	return(0);
}


void HandleFloatingMenus(WPARAM wParam, LPARAM lParam)	{
int				item;
OPENFILENAME	fn = {sizeof(OPENFILENAME)};
char			filename[MAX_PATH];
unsigned int	i;
unsigned char   strBuffer1[MAXSTRINGLENGTH];
unsigned char   strBuffer2[MAXSTRINGLENGTH];
	
	item = TrackPopupMenu(GetSubMenu(hFloatingMenus[lang], 0), TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hDlgMain, NULL);
	switch(item)	{
		case IDM_EXPORTSETTINGS:
			if (lang == LANG_HUNGARIAN)	{
				fn.lpstrFilter = "Szövegfájlok (*.txt)\0*.txt\0Összes fájl (*.*)\0*.*\0\0\0";
			} else {
				fn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0\0\0";
			}
			fn.lpstrTitle = (GetString (strBuffer1, nameOfTextFileStrs[lang][0]), strBuffer1);
			fn.hwndOwner = hDlgMain;
			fn.lpstrCustomFilter = NULL;
			fn.nFilterIndex = 1;
			fn.lpstrFile = filename;
			fn.nMaxFile = 256;
			fn.lpstrFileTitle = NULL;
			fn.lpstrInitialDir = NULL;			
			fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES;
			fn.hInstance = dlgClass.hInstance;
			filename[0] = 0;
			if (GetSaveFileName(&fn))	{
				if (!ExportConfiguration(filename))
					MessageBox(hDlgMain, (GetString (strBuffer1, cannotExportStrs[lang][0]), strBuffer1),
										 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
			}
			break;

		case IDM_SAVECONFIG:
			if (lang == LANG_HUNGARIAN)	{
				fn.lpstrFilter = "Konfigurációs fájlok (*.cfg)\0*.cfg\0Összes fájl (*.*)\0*.*\0\0\0";
			} else {
				fn.lpstrFilter = "Configuration files (*.cfg)\0*.cfg\0All files (*.*)\0*.*\0\0\0";
			}
			fn.lpstrTitle = (GetString (strBuffer1, nameOfConfigFileStrs[lang][0]), strBuffer1);
			fn.hwndOwner = hDlgMain;
			fn.lpstrCustomFilter = NULL;
			fn.nFilterIndex = 1;
			fn.lpstrFile = filename;
			fn.nMaxFile = 256;
			fn.lpstrFileTitle = NULL;
			fn.lpstrInitialDir = NULL;			
			fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES;
			fn.hInstance = dlgClass.hInstance;
			filename[0] = 0;
			if (GetSaveFileName(&fn))	{
				if (!SaveConfiguration(filename))
					MessageBox(hDlgMain, (GetString (strBuffer1, cannotSaveConfigStrs[lang][0]), strBuffer1),
										 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
			}
			break;

		case IDM_LOADCONFIG:
			if (lang == LANG_HUNGARIAN)	{
				fn.lpstrFilter = "Konfigurációs fájlok (*.cfg)\0*.cfg\0Összes fájl (*.*)\0*.*\0\0\0";
			} else {
				fn.lpstrFilter = "Configuration files (*.cfg)\0*.cfg\0All files (*.*)\0*.*\0\0\0";
			}
			fn.lpstrTitle = (GetString (strBuffer1, nameOfConfigFileStrs[lang][0]), strBuffer1);
			fn.hwndOwner = hDlgMain;
			fn.lpstrCustomFilter = NULL;
			fn.nFilterIndex = 1;
			fn.lpstrFile = filename;
			fn.nMaxFile = 256;
			fn.lpstrFileTitle = NULL;
			fn.lpstrInitialDir = NULL;			
			fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST;
			fn.hInstance = dlgClass.hInstance;
			filename[0] = 0;
			if (GetOpenFileName(&fn))	{
				item = LoadConfiguration(filename);
				switch(item)	{
					case 0:
						MessageBox(hDlgMain, (GetString (strBuffer1, cannotLoadConfigStrs[lang][0]), strBuffer1),
										 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
						
						break;
					
					case -1:
						MessageBox(hDlgMain, (GetString (strBuffer1, invalidConfigFileStrs[lang][0]), strBuffer1),
										 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
						break;
					
					case -2:
						MessageBox(hDlgMain, (GetString (strBuffer1, invalidExtConfigStrs[lang][0]), strBuffer1),
										(GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
						break;

					default:
						config = configs[platform];
						SetWindowToConfig();
						break;
				}
			}
			break;

		case IDM_XPSTYLE:
			setupConfig.flags ^= CFG_XPSTYLE;
			for (i = 0; i < numberOfLanguages; i++) {
				CheckMenuItem (GetSubMenu(hFloatingMenus[i], 0), IDM_XPSTYLE, MF_BYCOMMAND | (setupConfig.flags & CFG_XPSTYLE ? MF_CHECKED : MF_UNCHECKED));
			}
			SetEnableTheming ();
			break;
	}
}


// A Globális lap ablakkezelôje
int CALLBACK DialogGlobalDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
int		i;
		
	switch(uMsg)	{	
		case WM_INITDIALOG:
			return(FALSE);
		
		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;
		
		case WM_COMMAND:
			if (HIWORD(wParam) == CBN_SELCHANGE)	{
				switch(LOWORD(wParam))	{
					case IDC_CBDISPDEVS:
						i = SendMessage(GetDlgItem(hDlgGlobal, IDC_CBDISPDEVS), CB_GETCURSEL, 0, 0);
						if (i == CB_ERR) i = 0;
						config.dispdev = i;
						break;

					case IDC_CBDISPDRIVERS:
						i = SendMessage(GetDlgItem(hDlgGlobal,IDC_CBDISPDRIVERS),CB_GETCURSEL,0,0);
						if (i == CB_ERR) i = 0;
						config.dispdriver = i;						
						break;
				}
			}
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{
				case IDC_FULLSCR:
					config.Flags &= ~CFG_WINDOWED;
					dgVSsetScreenModeBox();
					break;

				case IDC_WINDOWED:
					config.Flags |= CFG_WINDOWED;
					dgVSsetScreenModeBox();
					break;

				case IDC_16BIT:
					config.Flags &= ~CFG_DISPMODE32BIT;
					dgVSsetScreenBitDepthBox();
					EnumerateRefreshRates();
					break;

				case IDC_32BIT:
					config.Flags |= CFG_DISPMODE32BIT;
					dgVSsetScreenBitDepthBox();
					EnumerateRefreshRates();
					break;

				case IDC_HIDECONSOLE:
					config.Flags2 ^= CFG_HIDECONSOLE;
					dgVSsetHideConsoleBox();
					break;

				case IDC_SETMOUSEFOCUS:
					config.Flags ^= CFG_SETMOUSEFOCUS;
					dgVSsetSetMouseFocusBox();
					break;

				case IDC_CTRLALT:
					config.Flags ^= CFG_CTRLALT;
					dgVSsetCTRLALTBox();
					break;

				case IDC_VDDMODE:
					config.Flags ^= CFG_NTVDDMODE;
					dgVSsetVDDModeBox();
					break;

				case IDC_ACTIVEBACKG:
					config.Flags ^= CFG_ACTIVEBACKGROUND;
					dgVSsetActiveBackgroundBox();
					break;

				case IDC_PRESERVEWINDOWSIZE:
					config.Flags2 ^= CFG_PRESERVEWINDOWSIZE;
					dgVSsetPreserveWindowSizeBox();
					break;

				case IDC_PRESERVEASPECTRATIO:
					config.Flags2 ^= CFG_PRESERVEASPECTRATIO;
					dgVSsetPreserveAspectRatioBox();
					break;

				case IDC_CLIPOPF:
					config.Flags2 ^= CFG_FORCECLIPOPF;
					dgVSsetCLIPOPFBox();
					break;

				case IDC_SCAPTUREON:
					config.Flags ^= CFG_GRABENABLE;
					dgVSsetCaptureOnBox();
					break;

				case IDC_SAVETOFILE:
					config.Flags &= ~CFG_GRABCLIPBOARD;
					dgVSsetGrabModeBox();
					break;

				case IDC_SAVETOCLIPBOARD:
					config.Flags |= CFG_GRABCLIPBOARD;
					dgVSsetGrabModeBox();
					break;

			}
		default:
			return(0);			
	}
}


void GlobalPageActivated()	{

	if (hActPage) ShowWindow(hActPage, SW_HIDE);
	ShowWindow(hActPage = hDlgGlobal, SW_SHOWNORMAL);
}


// A Glide lap ablakkezelôje
int CALLBACK DialogGlideDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
int		i;
		
	switch(uMsg)	{	
		case WM_INITDIALOG:
			return(FALSE);

		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;

		case WM_COMMAND:
			if (HIWORD(wParam) == CBN_SELCHANGE)	{
				switch(LOWORD(wParam))	{
					case IDC_COMBOTEXMEMSIZE:
						i = SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOTEXMEMSIZE), CB_GETCURSEL, 0, 0);
						if (i == CB_ERR) i = 0;
						config.TexMemSize = texMemSizes[i]*1024;
						break;

					case IDC_COMBORESOLUTION:
						i = SendMessage(GetDlgItem(hDlgGlide, IDC_COMBORESOLUTION), CB_GETCURSEL, 0, 0);
						if (i--)	{
							config.dxres = enumdispmodes[i].x;
							config.dyres = enumdispmodes[i].y;
						} else {
							config.dxres = config.dyres = 0;
						}
						EnumerateRefreshRates();
						CheckEnablingDependencies(0, 0);
						break;

					case IDC_REFRESHRATEVALUE:
						i = SendMessage(GetDlgItem(hDlgGlide, IDC_REFRESHRATEVALUE), CB_GETCURSEL, 0, 0);
						if (i--)	{
							config.RefreshRate = enumfrequencies[i];
						} else {
							config.RefreshRate = 0;
						}
						break;

					case IDC_COMBOLFBACCESS:
						i = SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOLFBACCESS), CB_GETCURSEL, 0, 0);
						config.Flags2 &= ~(CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE);
						if (i & 1) config.Flags2 |= CFG_DISABLELFBREAD;
						if (i & 2) config.Flags2 |= CFG_DISABLELFBWRITE;
						CheckEnablingDependencies(0, 0);
						break;

					case IDC_COMBOCOLORKEY:
						i = SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOCOLORKEY), CB_GETCURSEL, 0, 0);
						SetCKMethodValue(config.Flags, i);
						break;

				}
			}
			if (HIWORD(wParam) != BN_CLICKED) return(0); 
			switch(LOWORD(wParam))	{
				case IDC_CLOSESTFREQ:
					config.Flags2 ^= CFG_CLOSESTREFRESHFREQ;
					dgVSsetClosestRefRateBox();
					break;

				case IDC_RRMONITOR:
					config.Flags &= ~CFG_SETREFRESHRATE;
					dgVSsetRefRateBox();
					break;

				case IDC_RRAPP:
					config.Flags |= CFG_SETREFRESHRATE;
					dgVSsetRefRateBox();
					break;

				case IDC_VSYNC:
					config.Flags ^= CFG_VSYNC;
					dgVSsetVSyncBox();
					break;

				case IDC_TEX16:
					config.Flags &= ~CFG_TEXTURES32BIT;
					config.Flags |= CFG_TEXTURES16BIT;
					dgVSsetTextureBox();
					break;

				case IDC_TEX32:
					config.Flags &= ~CFG_TEXTURES16BIT;
					config.Flags |= CFG_TEXTURES32BIT;
					dgVSsetTextureBox();
					break;

				case IDC_TEXOPT:
					config.Flags &= ~(CFG_TEXTURES32BIT | CFG_TEXTURES16BIT);					
					dgVSsetTextureBox();
					break;

				case IDC_HWBUFFCACHING:
					config.Flags ^= CFG_HWLFBCACHING;
					dgVSsetHwBuffCachingBox();
					break;

				case IDC_FASTWRITE:
					config.Flags ^= CFG_LFBFASTUNMATCHWRITE;
					dgVSsetFastWriteBox();
					break;

				case IDC_LFBTEXTURETILES:
					config.Flags2 ^= CFG_LFBDISABLETEXTURING;
					dgVSsetTextureTilesBox();
					break;

				case IDC_REALHW:
					config.Flags ^= CFG_LFBREALVOODOO;
					dgVSsetRealHwBox();
					break;

				case IDC_PERFECTTEXEMU:
					config.Flags ^= CFG_PERFECTTEXMEM;
					dgVSsetPerfectTexMemBox();
					break;

				case IDC_MIPMAPPINGDISABLE:
					config.Flags ^= CFG_DISABLEMIPMAPPING;
					dgVSsetMipMappingDisableBox();
					break;

				case IDC_TRILINEARMIPMAPS:
					config.Flags2 ^= CFG_FORCETRILINEARMIPMAPS;
					dgVSsetTrilinearMipMapsBox();
					break;

				case IDC_AUTOGENMIPMAPS:
					config.Flags2 ^= CFG_AUTOGENERATEMIPMAPS;
					dgVSsetAutogenerateMipMapsBox();
					break;

				case IDC_BILINEARFILTERING:
					config.Flags ^= CFG_BILINEARFILTER;
					dgVSsetBilinearFilteringBox();
					break;

				case IDC_TOMBRAIDER:
					config.Flags ^= CFG_TOMBRAIDER;
					dgVSsetTombRaiderBox();
					break;

				case IDC_TRIPLEBUFFER:
					config.Flags ^= CFG_TRIPLEBUFFER;
					dgVSsetTripleBufferBox();
					break;

				case IDC_GAMMARAMP:
					config.Flags ^= CFG_ENABLEGAMMARAMP;
					dgVSsetGammaRampBox();
					break;

				case IDC_CLIPPING:
					config.Flags ^= CFG_CLIPPINGBYD3D;
					dgVSsetClippingBox();
					break;

				case IDC_HWVBUFFER:
					config.Flags ^= CFG_ENABLEHWVBUFFER;
					dgVSsetHwVBuffBox();
					break;

				case IDC_FORCEWBUFFERING:
					if ((config.Flags & (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) == (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) {
						config.Flags &= ~CFG_WBUFFERMETHODISFORCED;
						config.Flags |= CFG_RELYONDRIVERFLAGS;
					} else {
						config.Flags |= CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER;
					}
					dgVSsetForceWBufferingBox ();
					break;

				case IDC_TIMERLOOP:
					config.dosTimerBoostPeriod = config.dosTimerBoostPeriod ? 0 : DEFAULT_DOS_TIMER_BOOST;
					dgVSsetTimerBoostBox ();
					break;

			}
		case WM_HSCROLL:
			if (lParam == (LPARAM) GetDlgItem(hwndDlg, IDC_SLIDER1)) lParam = IDC_SLIDER1;
			switch(lParam)	{
				case IDC_SLIDER1:
					switch(LOWORD(wParam))	{
						case TB_THUMBTRACK:
						case TB_BOTTOM:
						case TB_ENDTRACK:
						case TB_LINEDOWN:
						case TB_LINEUP:
						case TB_PAGEDOWN:
						case TB_PAGEUP:
						case TB_TOP:
							config.Gamma = SendMessage (GetDlgItem(hwndDlg,IDC_SLIDER1), TBM_GETPOS, 0, 0);
							dgVSsetGammaControl();
							return(1);
						default:
							return(0);

					}				
				default:
					return(0);		
			}						
		default:
			return(0);			
	}
}


void GlidePageActivated()	{

	if (hActPage) ShowWindow(hActPage, SW_HIDE);
	ShowWindow(hActPage = hDlgGlide, SW_SHOWNORMAL);
}


// A VESA lap ablakkezelôje
int CALLBACK DialogVESADialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
int		i;
		
	switch(uMsg)	{	
		case WM_INITDIALOG:
			return(FALSE);

		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;

		case WM_COMMAND:
			if (HIWORD(wParam) == CBN_SELCHANGE)	{
				switch(LOWORD(wParam))	{					
					case IDC_COMBOVIDMEMSIZE:
						i = SendMessage(GetDlgItem(hDlgVESA, IDC_COMBOVIDMEMSIZE), CB_GETCURSEL, 0, 0);
						if (i == CB_ERR) i = 0;
						config.VideoMemSize = vidMemSizes[i];
						break;
				}
			}
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{
				case IDC_VESAENABLE:
					config.Flags ^= CFG_VESAENABLE;
					dgVSsetVESAEnableBox();
					break;

				case IDC_SUPPORTMODE13H:
					config.Flags ^= CFG_SUPPORTMODE13H;
					dgVSsetSupportMode13hBox();
					break;

				case IDC_DISABLEFLATLFB:
					config.Flags2 ^= CFG_DISABLEVESAFLATLFB;
					dgVSsetSupportDisableFlatLFBBox();
					break;
			}			
		case WM_HSCROLL:
			if (lParam == (LPARAM) GetDlgItem(hwndDlg, IDC_SLIDER2)) lParam = IDC_SLIDER2;
			switch(lParam)	{
				case IDC_SLIDER2:
					switch(LOWORD(wParam))	{
						case TB_THUMBTRACK:
							config.VideoRefreshFreq = HIWORD(wParam);
							dgVSsetRefreshFreq();
							return(1);
						default:
							return(0);

					}
				default:
					return(0);
			}			

		default:
			return(0);			
	}
}


void VESAPageActivated()	{

	if (hActPage) ShowWindow(hActPage, SW_HIDE);
	ShowWindow(hActPage = hDlgVESA, SW_SHOWNORMAL);
}


void ActivateProperPage()	{
		
	switch(TabCtrl_GetCurSel(GetDlgItem(hDlgMain, IDC_MAINTAB)))	{
		case 0:
			GlobalPageActivated();
			break;
		case 1:
			GlidePageActivated();
			break;
		case 2:
			VESAPageActivated();
			break;
	}
}


int CALLBACK DialogTopProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
unsigned int	i, j;
int				readVal;
unsigned char	strBuffer1[MAXSTRINGLENGTH];
unsigned char	strBuffer2[MAXSTRINGLENGTH];
	
	switch(uMsg)	{
		case WM_INITDIALOG:			
			return(1);
			break;

		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;

		case WM_COMMAND:
			if (HIWORD(wParam) == CBN_SELCHANGE)	{
				switch(LOWORD(wParam))	{
					case IDC_INSTANCELIST:
						i = SendMessage(GetDlgItem(hDlgTop, IDC_INSTANCELIST), CB_GETCURSEL, 0, 0);
						for (j = 0; j < numberOfLanguages; j++) {
							if (j != lang) {
								SendMessage(GetDlgItem(pages[j][pageTop], IDC_INSTANCELIST), CB_SETCURSEL, i, 0);
							}
						}
						if (i == CB_ERR) i = 0;
						SelectInstance(i);
						return(0);
				}
			}
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{
				case IDC_PWINDOWS:
					if (platform == PWINDOWS) break;
					dgVSsetPlatformButtons(PWINDOWS);
					break;

				case IDC_PDOS:
					if (platform == PDOS) break;
					dgVSsetPlatformButtons(PDOS);
					break;

				case IDC_LANGENGLISH:
					config.language = English;
					dgVSsetLang();
					SetWindowToConfig();					
					break;

				case IDC_LANGHUNGARIAN:
					config.language = Hungarian;
					dgVSsetLang();
					SetWindowToConfig();
					break;

				case IDC_SEARCH:
					SetStringsForOpenFileName(strBuffer1);
					if (GetOpenFileName(of))	{
						readVal = ReaddgVoodooConfig(of->lpstrFile, platform);
						switch(readVal)	{
							case 0:
								AddInstance(of->lpstrFile);
								for (j = 0; j < numberOfLanguages; j++) {
									SendMessage(GetDlgItem(pages[j][pageTop], IDC_INSTANCELIST), CB_SETCURSEL, 0, 0);
								}
								SelectInstance(0);
								break;

							case 2:
								GetString (strBuffer1, incompatibleWrapperFileStrs[lang][0]);
								sprintf(strBuffer2, strBuffer1, DGVOODOOVERSION_STR);
								GetString (strBuffer1, errorDuringConfigReadStrs[lang][0]);
								MessageBox(hDlgMain, strBuffer2, strBuffer1, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
								break;

							default:
							case 1:
								MessageBox(hDlgMain, (GetString (strBuffer1, invalidWrapperFile2Strs[lang][0]), strBuffer1),
										   (GetString (strBuffer2, errorDuringConfigReadStrs[lang][0]), strBuffer2),
											MB_OK | MB_APPLMODAL | MB_ICONSTOP);
								break;
						}
					}
					break;
				case IDC_WINDIR:
				case IDC_ACTDIR:
					i = (LOWORD(wParam)) == IDC_WINDIR ? GetWindowsDirectory(filenamew, MAX_PATH) : GetCurrentDirectory(MAX_PATH, of->lpstrFile);
					if (i != 0)	{
						SearchInstances(filenamew);
						for (j = 0; j < numberOfLanguages; j++) {
							SendMessage(GetDlgItem(pages[j][pageTop], IDC_INSTANCELIST), CB_SETCURSEL, 0, 0);
						}
						SelectInstance(0);
					}
					break;				
				default:
					return(0);
			}
			return(1);
		
		default:
			return(0);
	}	
}


int CALLBACK DialogBottomProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
char			filename[MAX_PATH];

	switch(uMsg)	{
		case WM_INITDIALOG:			
			return(1);
			break;

		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;

		case WM_COMMAND:			
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{				
				case IDC_OKBUTTON:
					if (!configvalid)	{
						EndDialog(hwndDlg, 1);
					} else {
						SendMessage(GetDlgItem(pages[English][pageTop], IDC_INSTANCELIST), CB_GETLBTEXT,
									SendMessage(GetDlgItem(pages[English][pageTop], IDC_INSTANCELIST), CB_GETCURSEL, 0, 0), (WPARAM) filename);
						if (WritedgVoodooConfig(filename, platform) != 2) EndDialog(hDlgMain, 1);
					}
					break;

				case IDC_CANCELBUTTON:
					EndDialog(hDlgMain, 1);
					break;

				case IDC_ABOUTBUTTON:
					DialogBox(ofw.hInstance, MAKEINTRESOURCE(aboutDlgIDs[lang]), hwndDlg, dgVoodooAboutDialogProc);
					break;

				default:
					return(0);
			}
			return(1);
		
		default:
			return(0);
	}	
}


int CALLBACK dgVoodooSetupDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
unsigned		int i,j;
TC_ITEM			tabitem;
RECT			rect1,rect2;
DDSURFACEDESC2	tempsurface = {sizeof(DDSURFACEDESC)};
char			strBuffer[MAXSTRINGLENGTH];

	hDlgMain = hwndDlg;
	switch(uMsg)	{
		case WM_INITDIALOG:
			configs[0] = configs[1] = config;

			tabitem.mask = TCIF_TEXT;
			GetWindowRect(GetDlgItem(hwndDlg, IDC_MAINTAB), &rect1);
			
			tabitem.pszText = (GetString (strBuffer, tabPaneStrs[0][0]), strBuffer);
			SendMessage(GetDlgItem(hwndDlg, IDC_MAINTAB), TCM_INSERTITEM, (WPARAM) 0, (LPARAM) &tabitem);
			
			tabitem.pszText = (GetString (strBuffer, tabPaneStrs[0][1]), strBuffer);
			SendMessage(GetDlgItem(hwndDlg, IDC_MAINTAB), TCM_INSERTITEM, (WPARAM) 1, (LPARAM) &tabitem);
			
			tabitem.pszText = (GetString (strBuffer, tabPaneStrs[0][2]), strBuffer);
			SendMessage(GetDlgItem(hwndDlg, IDC_MAINTAB), TCM_INSERTITEM, (WPARAM) 2, (LPARAM) &tabitem);
			TabCtrl_GetItemRect(GetDlgItem(hwndDlg, IDC_MAINTAB), 0, &rect2);
			TabCtrl_SetItemSize(GetDlgItem(hwndDlg, IDC_MAINTAB),(rect1.right - rect1.left - 3) / 3, rect2.bottom - rect2.top);

			for (i = 0; i < numberOfLanguages; i++) {
				for (j = 0; j < numberOfPages; j++)	{
					pages[i][j] = CreateDialogParam(dlgClass.hInstance, MAKEINTRESOURCE(pageIDs[i][j]),
													hwndDlg, pageWindowProc[j], 0);
				}
				
				SetWindowPos(pages[i][pageTop], 0, 0, 73, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(pages[i][pageBottom], 0, 0, 560, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

				hFloatingMenus[i] = LoadMenu(dlgClass.hInstance, MAKEINTRESOURCE(floatingMenuIDs[i]));
			}
			
			instancenum = 0;
			configvalid = 0;
			GetCurrentDirectory(MAX_PATH, filenamew);
			if (SearchInstances(filenamew), !instancenum)	{
				GetWindowsDirectory(filenamew, MAX_PATH);
				SearchInstances(filenamew);
			}

			for (i = 0; i < numberOfLanguages; i++) {
				if (!instancenum)	{
					SendMessage(GetDlgItem(pages[i][pageTop], IDC_INSTANCELIST), CB_ADDSTRING,
										   (WPARAM) 0, (LPARAM) (GetString (strBuffer, instanceStrs[i][0]), strBuffer));
				}
				SendMessage(GetDlgItem(pages[i][pageTop], IDC_INSTANCELIST), CB_SETCURSEL, 0, 0);
			}

			lang = config.language;
			
			hDlgTop = pages[lang][pageTop];
			hDlgGlobal = pages[lang][pageGlobal];
			hDlgGlide = pages[lang][pageGlide];
			hDlgVESA = pages[lang][pageVesa];
			hDlgBottom = pages[lang][pageBottom];

			for (i = 0; i < numberOfLanguages; i++) {
				ShowWindow (pages[i][pageTop], i == lang ? SW_SHOWNORMAL : SW_HIDE);
				ShowWindow (pages[i][pageBottom], i == lang ? SW_SHOWNORMAL : SW_HIDE);
			}
						
			hActPage = 0;
			GetWindowRect(GetDlgItem(hwndDlg, IDC_MAINTAB), &rect1);
			GetWindowRect(hDlgGlobal, &rect2);
			OffsetRect(&rect1,-rect2.left, -rect2.top);
			TabCtrl_AdjustRect(GetDlgItem(hwndDlg, IDC_MAINTAB), FALSE, &rect1);
			
			for (i = 0; i < numberOfLanguages; i++) {
				SetWindowPos(pages[i][pageGlobal], 0, rect1.left, rect1.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(pages[i][pageGlide], 0, rect1.left, rect1.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(pages[i][pageVesa], 0, rect1.left, rect1.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

				SendMessage(GetDlgItem(pages[i][pageGlide], IDC_SLIDER1), TBM_SETRANGE, FALSE, (LPARAM) MAKELONG(0, 400));
				SendMessage(GetDlgItem(pages[i][pageGlide], IDC_SLIDER1), TBM_SETTICFREQ, 50, 0);
				SendMessage(GetDlgItem(pages[i][pageGlide], IDC_SLIDER1), TBM_SETPOS, TRUE, 100);

				SendMessage(GetDlgItem(pages[i][pageVesa], IDC_SLIDER2), TBM_SETRANGE, FALSE, (LPARAM) MAKELONG(45, 70));	
				SendMessage(GetDlgItem(pages[i][pageVesa], IDC_SLIDER2), TBM_SETTICFREQ, 5, 0);
				SendMessage(GetDlgItem(pages[i][pageVesa], IDC_SLIDER2), TBM_SETPOS, TRUE, 60);
			}
			
			SetStringsForOpenFileName(strBuffer);			
			ofw.hwndOwner = hwndDlg;
			ofw.lpstrCustomFilter = NULL;
			ofw.nFilterIndex = 1;
			ofw.lpstrFile = filenamew;
			ofw.nMaxFile = 256;
			ofw.lpstrFileTitle = NULL;
			ofw.lpstrInitialDir = NULL;
			ofw.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST;

			EnumerateRenderingDevices();

			for (i = 0; i < numberOfLanguages; i++) {
				for(j = 0; j < 12; j++)	{
					SendMessage(GetDlgItem(pages[i][pageGlide], IDC_COMBOTEXMEMSIZE), CB_ADDSTRING,
								(WPARAM) 0, (LPARAM) texMemSizeStr[j]);
				}
				for(j = 0; j < 7; j++)	{
					SendMessage(GetDlgItem(pages[i][pageVesa], IDC_COMBOVIDMEMSIZE), CB_ADDSTRING,
								(WPARAM) 0, (LPARAM) vidMemSizeStr[j]);
				}
				for(j = 0; j < 4; j++)	{
					SendMessage(GetDlgItem(pages[i][pageGlide], IDC_COMBOLFBACCESS), CB_ADDSTRING,
								(WPARAM) 0, (LPARAM) (GetString(strBuffer, lfbAccessStrs[i][j]), strBuffer));

					SendMessage(GetDlgItem(pages[i][pageGlide], IDC_COMBOCOLORKEY), CB_ADDSTRING,
								(WPARAM) 0, (LPARAM) (GetString(strBuffer, ckMethodStrs[i][j]), strBuffer));
				}

				if (setupConfig.flags & CFG_XPSTYLE) {
					CheckMenuItem (hFloatingMenus[i], IDM_XPSTYLE, MF_CHECKED);
				}
				if (!themingAvailable) {
					EnableMenuItem (hFloatingMenus[i], IDM_XPSTYLE, MF_GRAYED);
				}
			}

			if (GetActDispMode(NULL,&tempsurface))	{
				for (j = 0; j < numberOfLanguages; j++) {
					sprintf(tempstr, (GetString (strBuffer, desktopModeStrs[j][0]), strBuffer),
							tempsurface.dwWidth, tempsurface.dwHeight,
							i = tempsurface.ddpfPixelFormat.dwRGBBitCount);
					SendMessage(GetDlgItem(pages[j][pageTop], IDC_DESKTOPTEXT), WM_SETTEXT, 0, (LPARAM) tempstr);
				}
			} else {
				for (i = 0; i < numberOfLanguages; i++) {
					SendMessage(GetDlgItem(pages[i][pageTop], IDC_DESKTOPTEXT), WM_SETTEXT, 0,
								(LPARAM) (GetString (strBuffer, desktopModeUnknownStrs[i][0]), strBuffer));
				}
				i = 16;
			}

			tempstr[0] = '\0';
			if ( (i != 16) && (i != 32) ) sprintf(tempstr, "Warning! Incompatible mode!\0");
			SendMessage(GetDlgItem(hDlgMain, IDC_UNCOMPMODE), WM_SETTEXT, 0, (LPARAM) tempstr);
			
			platform = PWINDOWS;
			SendMessage(GetDlgItem(pages[English][pageTop], IDC_LANGENGLISH), BM_SETCHECK, TRUE, 0);
			SendMessage(GetDlgItem(pages[Hungarian][pageTop], IDC_LANGHUNGARIAN), BM_SETCHECK, TRUE, 0);

			refrate_x = refrate_y = refrate_bpp = -1;
			if (configvalid)	{
				SelectInstance(0);
				dgVSsetPlatformButtons(PWINDOWS);
			} else {
				DisableConfig (noWrapperFileFoundStrs, NULL);
				dgVSsetPlatformButtons(PWINDOWS);
			}
			
			if (themingAvailable) {
				SetEnableTheming ();
			}
			return(1);
			break;		

		case WM_NOTIFY:
			if ( ((LPNMHDR) lParam)->code == TCN_SELCHANGE)	{
				ActivateProperPage();				
			}
			return(0);

		case WM_CONTEXTMENU:
			HandleFloatingMenus(wParam, lParam);
			return(1);
			break;
		
		default:
			return(0);

	}	
}


int ReaddgVoodooConfig(unsigned char *filename, unsigned int platform)	{
dgVoodooConfig	thisConfigs[2];
HANDLE			hFile;
unsigned long	id;
int				pos;
 	
	if ( (hFile = _OpenFile(filename)) == INVALID_HANDLE_VALUE) return(1);
	pos = 0x40;
	if (_SetFilePos(hFile, pos, FILE_BEGIN) != pos)	{
		_CloseFile(hFile);
		return(1);
	}
	if ( _ReadFile(hFile, &thisConfigs, 2*sizeof(dgVoodooConfig)) != 2*sizeof(dgVoodooConfig)) {
		_CloseFile(hFile);
		return(1);
	}
	pos = 2 * sizeof(dgVoodooConfig) + 0x40;
	if (_SetFilePos(hFile, pos, FILE_BEGIN) != pos)	{
		_CloseFile(hFile);
		return(1);
	}
	if ( _ReadFile(hFile, &id, 4)!=4) {
		_CloseFile(hFile);
		return(1);
	}
	_CloseFile(hFile);

	configs[0] = thisConfigs[0];
	configs[1] = thisConfigs[1];

	if (id != 'EGED') return(1);
	if ((configs[0].ProductVersion != DGVOODOOVERSION) || (configs[1].ProductVersion != DGVOODOOVERSION)) return(2);
	
	config = configs[platform];
	configvalid = 1;
	return(0);
}


int WritedgVoodooConfig(unsigned char *filename, unsigned int platform)	{
HANDLE			handle;
unsigned long	id;
char			temp[256];
FILETIME		create, write;
int				pos;
unsigned char	strBuffer1[MAXSTRINGLENGTH];
unsigned char	strBuffer2[MAXSTRINGLENGTH];
	
	configs[platform] = config;
	if ( (handle = CreateFile(filename,GENERIC_READ | GENERIC_WRITE,
							  FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)	{
		
		MessageBox(hDlgMain, (GetString (strBuffer1, cannotWriteConfigStrs[lang][0]), strBuffer1),
				   (GetString (strBuffer2, errorDuringSaveStrs[lang][0]), strBuffer2), MB_ICONSTOP | MB_OK);
		return(2);
	}
	if (_SetFilePos(handle, 0x040, FILE_BEGIN) != 0x040)	{
		_CloseFile(handle);
		return(0);
	}	
	if ( _ReadFile(handle, temp, 2 * sizeof(dgVoodooConfig)) !=  2 * sizeof(dgVoodooConfig)) {	
		_CloseFile(handle);
		return(0);
	}
	if ( _ReadFile(handle, &id, 4) != 4) {
		_CloseFile(handle);
		return(0);
	}
	if (id != 'EGED')	{
		_CloseFile(handle);
		return(0);
	}
	pos = 0x40;
	if (_SetFilePos(handle, pos, FILE_BEGIN) != pos)	{
		_CloseFile(handle);
		return(0);
	}
	GetFileTime(handle, &create, NULL, &write);
	if (_WriteFile(handle, configs, 2 * sizeof(dgVoodooConfig)) != 2 * sizeof(dgVoodooConfig) )	{
		_CloseFile(handle);		
		return(2);
	}
	SetFileTime(handle, &create, NULL, &write);
	_CloseFile(handle);	
	return(1);
}


int SaveConfiguration(char *filename)	{
HANDLE		handle;

	if ( (handle = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
							  FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE) return(0);
	if (_WriteFile(handle, CFGFILEID, sizeof(CFGFILEID)) != sizeof(CFGFILEID) )	{
		_CloseFile(handle);
		return(0);
	}
	if (_WriteFile(handle, configs, 2 * sizeof(dgVoodooConfig)) != 2 * sizeof(dgVoodooConfig) )	{
		_CloseFile(handle);
		return(0);
	}
	if (_WriteFile(handle, cfgRemarkStr, sizeof(cfgRemarkStr)) != sizeof(cfgRemarkStr) )	{
		_CloseFile(handle);
		return(0);
	}
	_CloseFile(handle);	
	return(1);
}


int LoadConfiguration(char *filename)	{
dgVoodooConfig	thisConfigs[2];
HANDLE			handle;
char			id[16];
int				i;

	if ( (handle = CreateFile(filename,GENERIC_READ,
			FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL))== INVALID_HANDLE_VALUE) return(0);
	if ( _ReadFile(handle, id, sizeof(CFGFILEID))!=sizeof(CFGFILEID)) {	
		_CloseFile(handle);
		return(0);
	}
	for(i = 0; i < sizeof(CFGFILEID); i++) if (id[i] != CFGFILEID[i])	{
		_CloseFile(handle);
		return(-1);
	}
	if ( _ReadFile(handle, &thisConfigs, 2*sizeof(dgVoodooConfig))!=2*sizeof(dgVoodooConfig)) {
		_CloseFile(handle);
		return(0);
	}
	_CloseFile(handle);

	if ((thisConfigs[0].ProductVersion != DGVOODOOVERSION) || (thisConfigs[1].ProductVersion != DGVOODOOVERSION)) return(-2);
	
	configs[0] = thisConfigs[0];
	configs[1] = thisConfigs[1];
	return(1);
}


HWND GetHwndByName(int name)	{

	switch(name)	{
		case HWND_MAIN:
			return(hDlgMain);
		case HWND_GLOBAL:
			return(hDlgGlobal);
		case HWND_GLIDE:
			return(hDlgGlide);
		case HWND_VESA:
			return(hDlgVESA);
		case HWND_HEADER:
			return(hDlgTop);
		default:
			return(NULL);
	}
}


void __inline SetMultipleDepConjunctionsToTRUE()	{
multipledepitem *md = _md;

	while(md->depending != 0xFFFFFFFF) (*(md++)).conjunction = 0xFFFFFFFF;
}


int CheckEnablingDependencies(HWND hwnd, unsigned int item)	{
int				i,j,b;
dependency		*dep;
multipledepitem *md;
	
	if (!configvalid) return(FALSE);
	j = 0;
	SetMultipleDepConjunctionsToTRUE();
	while( (dep = dependencytable[j++]) != NULL)	{
		switch(dep->type)	{
			case DTYPE_FLAGMASKTRUE:
				b =! (config.Flags & dep->data);
				break;
			case DTYPE_FLAGMASKFALSE:
				b = config.Flags & dep->data;
				break;
			case DTYPE_FLAG2MASKTRUE:
				b = !(config.Flags2 & dep->data);
				break;
			case DTYPE_FLAG2MASKFALSE:
				b = config.Flags2 & dep->data;
				break;
			case DTYPE_FUNCTION:
				b = (*( (int (*)()) dep->data)) ();
				break;
			case DTYPE_FUNCTIONFALSE:
				b = !(*( (int (*)()) dep->data)) ();
				break;
			case DTYPE_PLATFORM:
				b = (platform != dep->data);
				break;
		}
		b = b ? TRUE : FALSE;
		for(i = 0; i < dep->numofdependingitems; i++)	{
			md = _md;
			while(md->depending != 0xFFFFFFFF)	{
				if ( (dep->depending == md->depending) && (dep->dependingitems[i] == md->dependingitem) )	{
					md->conjunction = md->conjunction && b;
					break;
				}
				md++;
			}
			if (md->depending == 0xFFFFFFFF)	{
				if (hwnd == 0) EnableWindow(GetDlgItem(GetHwndByName(dep->depending), dep->dependingitems[i]), b);
					else if( (hwnd == GetHwndByName(dep->depending)) && (item == dep->dependingitems[i]) ) 
						return(b);
			}
		}
	}

	md = _md;
	while(md->depending != 0xFFFFFFFF)	{
		if (md->conjunction != 0xFFFFFFFF)
			if (hwnd == 0) EnableWindow(GetDlgItem(GetHwndByName(md->depending), md->dependingitem), md->conjunction);
				else if( (hwnd == GetHwndByName(md->depending)) && (item == md->dependingitem) ) 
					return(md->conjunction);
		md++;
	}
	return(TRUE);
}


BOOL CALLBACK EnablingCallback(HWND hwnd, LPARAM lParam)	{

	EnableWindow(hwnd, CheckEnablingDependencies( (HWND) lParam, GetDlgCtrlID(hwnd) ));
	return(TRUE);
}


BOOL CALLBACK DisablingCallback(HWND hwnd, LPARAM lParam)	{
	
	EnableWindow(hwnd, FALSE);
	return(TRUE);
}


void _inline DisableAllChildWindows(HWND hwnd)	{

	EnumChildWindows(hwnd, DisablingCallback, 0);
}


void _inline EnableAllChildWindows(HWND hwnd)	{

	EnumChildWindows(hwnd, EnablingCallback, (LPARAM) hwnd);
}


void DisableConfig(char *strTable, char *toCopyInStr/*char *ErrorMsgEng, char *ErrorMsgHun*/)	{
unsigned int	i;
unsigned char	strBuffSrc[MAXSTRINGLENGTH];
unsigned char	strBuffDst[MAXSTRINGLENGTH];

	for (i = 0; i < numberOfLanguages; i++) {
		DisableAllChildWindows(pages[i][pageGlobal]);
		DisableAllChildWindows(pages[i][pageGlide]);
		DisableAllChildWindows(pages[i][pageVesa]);
		
		if (toCopyInStr != NULL) {
			GetString (strBuffSrc, strTable[i]);
			sprintf (strBuffDst, strBuffSrc, toCopyInStr);
		} else {
			GetString (strBuffDst, strTable[i]);
		}
		SendMessage(GetDlgItem(pages[i][pageTop], IDC_ERROR), WM_SETTEXT, 0, strTable[i] );
	}
}


void EnableConfig()	{
unsigned int	i = 0;

	if (configvalid)	{
		for (i = 0; i < numberOfLanguages; i++) {
			EnableAllChildWindows(pages[i][pageGlobal]);
			EnableAllChildWindows(pages[i][pageGlide]);
			EnableAllChildWindows(pages[i][pageVesa]);
			SendMessage(GetDlgItem(pages[i][pageTop], IDC_ERROR), WM_SETTEXT, 0, (LPARAM) "");
		}
	}
}


void SetWindowToConfig()	{
unsigned int	i,j;
			
	i = config.TexMemSize >> 10;
	for(j = 0; (i != texMemSizes[j]) && (j < 12); j++);
	if (j >= 12) j = 0;
	SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOTEXMEMSIZE), CB_SETCURSEL, j, 0);

	i = config.VideoMemSize;
	for(j = 0; (i != vidMemSizes[j]) && (j < 7); j++);
	if (j >= 7) j = 0;
	SendMessage(GetDlgItem(hDlgVESA, IDC_COMBOVIDMEMSIZE), CB_SETCURSEL, j, 0);

	j = 0;
	if (config.Flags2 & CFG_DISABLELFBREAD) j = 1;
	if (config.Flags2 & CFG_DISABLELFBWRITE) j |= 2;
	SendMessage(GetDlgItem(hDlgGlide, IDC_COMBOLFBACCESS), CB_SETCURSEL, j, 0);

	if ( (config.dxres == 0) || (config.dyres == 0) ) j = 0;
		else for(i = 0; i < enumdispmodesnum; i++)	{
			if ( (config.dxres == enumdispmodes[i].x) && (config.dyres == enumdispmodes[i].y) ) j = i + 1;
		}
    SendMessage(GetDlgItem(hDlgGlide, IDC_COMBORESOLUTION), CB_SETCURSEL, j, 0);
	EnumerateRefreshRates();

	for (i = 0; i < numberOfLanguages; i++) {
		SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDEVS), CB_SETCURSEL, config.dispdev, 0);
		SendMessage(GetDlgItem(pages[i][pageGlobal], IDC_CBDISPDRIVERS), CB_SETCURSEL, config.dispdriver, 0);
	}

	dgVSsetScreenModeBox();
	dgVSsetScreenBitDepthBox();
	dgVSsetTexBitDepthBox();
	dgVSsetClosestRefRateBox();
	dgVSsetRefRateBox();
	dgVSsetTextureBox();
	dgVSsetHwBuffCachingBox();
	dgVSsetFastWriteBox();
	dgVSsetTextureTilesBox();
	dgVSsetRealHwBox();
	dgVSsetCKComboBox();
	dgVSsetGammaControl();
	dgVSsetRefreshFreq();
	dgVSsetVSyncBox();
	dgVSsetHideConsoleBox();
	dgVSsetSetMouseFocusBox();
	dgVSsetCTRLALTBox();
	dgVSsetVDDModeBox();
	dgVSsetActiveBackgroundBox();
	dgVSsetPreserveWindowSizeBox();
	dgVSsetPreserveAspectRatioBox();
	dgVSsetCLIPOPFBox();
	dgVSsetSupportMode13hBox();
	dgVSsetSupportDisableFlatLFBBox();
	dgVSsetVESAEnableBox();
	dgVSsetPerfectTexMemBox();
	dgVSsetMipMappingDisableBox();
	dgVSsetTrilinearMipMapsBox();
	dgVSsetAutogenerateMipMapsBox();
	dgVSsetBilinearFilteringBox();
	dgVSsetCaptureOnBox();
	dgVSsetGrabModeBox();
	dgVSsetTombRaiderBox();	
	dgVSsetTripleBufferBox();	
	dgVSsetGammaRampBox();
	dgVSsetClippingBox();
	dgVSsetHwVBuffBox();
	dgVSsetForceWBufferingBox ();
	dgVSsetTimerBoostBox ();
	CheckEnablingDependencies(0,0);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)	{
DDSURFACEDESC2	temp;
/*LOGFONT		font =	{-19, 0, 0, 0, 700, 0, 0, 0, 238, 3, 2, 1, 34, "Verdana"};
CHOOSEFONT	cf= {sizeof(CHOOSEFONT),NULL,0,&font,0,CF_SCREENFONTS|CF_TTONLY,0,0,NULL,NULL,NULL,NULL,0,0,0};*/


	ofw.hInstance = dlgClass.hInstance = hInstance;

	CopyMemory (&setupConfig, (const void *) (((char *) hInstance) + 0x40), sizeof(SetupConfig));

	//ChooseFont(&cf);
	
	dlgClass.hCursor = LoadCursor(0,IDC_ARROW);
	dlgClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClass(&dlgClass);

	if (EmptyCommandLine (cmdLine)) {

		if (!GetActDispMode(NULL,&temp))	{
			MessageBox(NULL, "You need DirectX 7.0 or greater installed to use dgVoodoo!",
					   "Warning",MB_OK | MB_ICONWARNING);
		}		

		themingAvailable = 1;
		uxThemeHandle = LoadLibrary ("uxtheme.dll");
		pSetThemeAppProperties = (SetThemeAppPropertiesProc) GetProcAddress (uxThemeHandle, "SetThemeAppProperties");
		if (pSetThemeAppProperties == NULL) {
			uxThemeHandle = NULL;
			themingAvailable = 0;
		}
		
		DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOGMAIN), NULL, dgVoodooSetupDialogProc, 0);

		if (uxThemeHandle != NULL)
			FreeLibrary (uxThemeHandle);
	
	} else {
		
		ProcessCommandLine (cmdLine);

	}
}