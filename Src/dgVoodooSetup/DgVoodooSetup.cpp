/*--------------------------------------------------------------------------------- */
/* dgVoodooSetup.cpp - dgVoodooSetup main GUI implementation                        */
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
/*			 Main Setup																		*/
/*------------------------------------------------------------------------------------------*/

#include "dgVoodooSetup.h"
#include <commctrl.h>
#include <stdio.h>
#include "uxtheme.h"
#include "Resource.h"

#include "file.h"
#include "Version.h"
#include "CmdLine.h"
//#include "ConfigTree.h"

namespace	UI	{
	class	TopPage;
	class	BottomPage;
}

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/


UI::Page* topPage = NULL;
UI::Page* bottomPage = NULL;
CV::ConfigView* configView = NULL;

#define CFG_XPSTYLE				1
#define CFG_RENDERERSPECIFIC	2

#define	MAXSTRINGLENGTH			1024

typedef struct	{
	
	unsigned short	productVersion;
	unsigned int	flags;
	unsigned int	reserved[7];

} SetupConfig;


/*------------------------------------------------------------------------------------------*/
/*.................................. Belsõ függvények ......................................*/

int					SaveConfiguration(char *filename);
int					LoadConfiguration(char *filename);
void				DisableConfig (/*char *strTable, char *toCopyInStr*/);
void				EnableConfig ();
void				SetWindowToConfig();
void				SetupHideWindow (HWND window);
void				SetupShowWindow (HWND window);
LRESULT CALLBACK	dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		ThemeChangeEnumProc(HWND hwnd, LPARAM lParam);
void				ActivateProperPage (int doSetConfig, int doEnableConfig);
int					IsHwBuffCachingEnabled();
int					IsRescaleChangesOnlyEnabled();

/*------------------------------------------------------------------------------------------*/
/*................................... Globális cuccok ......................................*/

unsigned int	tabPaneStrs[numberOfLanguages][4] =
				{	{IDS_TABPANESTR1, IDS_TABPANESTR2, IDS_TABPANESTR3, IDS_TABPANESTR4},
					{IDS_TABPANESTR1_HUN, IDS_TABPANESTR2_HUN, IDS_TABPANESTR3_HUN, IDS_TABPANESTR4_HUN} };

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


char			filenamew[MAX_PATH];
char			cfgRemarkStr[]		= "\n;Current settings of the wrapper will be overrided by this file if a glide application is started from this directory!";

dgVoodooConfig	config;

dgVoodooConfig	defaultConfigs[2]	= { {DGVOODOOVERSION, 0x243, 8192 << 10,
										 CFG_HWLFBCACHING | CFG_RELYONDRIVERFLAGS | CFG_LFBFASTUNMATCHWRITE |
										 CFG_PERFECTTEXMEM | CFG_STOREMIPMAP | CFG_ENABLEGAMMARAMP | (CFG_CKMAUTOMATIC << 5),
										 100, 0, 0, 0, 0, 0, 0, 0, 0,
										 CFG_HIDECONSOLE | CFG_FORCETRILINEARMIPMAPS | CFG_LFBRESCALECHANGESONLY | CFG_CLOSESTREFRESHFREQ,
										 0, 0, English, DirectX7},

										{DGVOODOOVERSION, 0x243, 8192 << 10,
										 CFG_HWLFBCACHING | CFG_RELYONDRIVERFLAGS | CFG_LFBFASTUNMATCHWRITE |
										 CFG_PERFECTTEXMEM | CFG_STOREMIPMAP | CFG_ENABLEGAMMARAMP | (CFG_CKMAUTOMATIC << 5) | CFG_NTVDDMODE,
										 100, 0, 1024, 50, 0, 0, 0, 0, 0,
										 CFG_HIDECONSOLE | CFG_FORCETRILINEARMIPMAPS | CFG_LFBRESCALECHANGESONLY | CFG_CLOSESTREFRESHFREQ,
										 80, 0, English, DirectX7} };

SetupConfig		setupConfig;

unsigned int	platform = PWINDOWS;//PUNKNOWN;
unsigned int	lang;
HINSTANCE		hInstance;

int				configvalid = 0;

WNDCLASS		dlgClass = {0, dlgWindowProc, 0, DLGWINDOWEXTRA, 0, NULL, NULL, (HBRUSH) COLOR_WINDOW, NULL, "DGCLASS"};

//unsigned int	aboutDlgIDs[numberOfLanguages] = {IDD_DIALOGABOUT, IDD_DIALOGABOUT};
HWND			hDlgMain;
HWND			hActPage;
HWND			hToolTip;
unsigned int	floatingMenuIDs[numberOfLanguages] = {IDR_FLOATINGMENU, IDR_FLOATINGMENU};
HMENU			hFloatingMenus[numberOfLanguages];
int				themingAvailable;


void			GetString (LPSTR lpBuffer, UINT uID);
int				EmptyCommandLine (LPSTR cmdLine);
int				ExportConfiguration (char *filename, const dgVoodooConfig& config);

TOOLINFO		toolInfo;

/*------------------------------------------------------------------------------------------*/
/*...................................... AdapterHander ......................................*/

AdapterHandler			adapterHandler;


AdapterHandler::AdapterHandler ():
	currentAPI		(RANone),
	iHandler		(NULL),
	displayModesNum	(0),
	refreshRatesNum (0)
{
}


void			AdapterHandler::ShutDownCurrentAPI ()
{
	if (iHandler != NULL) {
		iHandler->ShutDown ();
	}
}


bool			AdapterHandler::SwitchToAPI (RendererAPI api)
{
	if (api != currentAPI) {
		ShutDownCurrentAPI ();
		currentAPI = api;
		switch (api) {
			case DirectX7:
				iHandler = GetDX7AdapterHandler ();
				break;
			case DirectX9:
				iHandler = GetDX9AdapterHandler ();
				break;
			default:
				iHandler = NULL;
				break;
		}
		if (iHandler != NULL) {
			if (!iHandler->Init ()) {
				iHandler = NULL;
			}
		}
	}
	return iHandler != NULL;
}


void			AdapterHandler::AddResolution (unsigned int xRes, unsigned int yRes)
{
	unsigned int i = 0;
	for(; i < displayModesNum; i++)	{
		if (xRes == displayModes[i].x && yRes == displayModes[i].y)
			break;
	}
	if (i == displayModesNum) {
		displayModes[displayModesNum].x = xRes;
		displayModes[displayModesNum++].y = yRes;
	}
}


void			AdapterHandler::AddRefreshRate (unsigned int freq)
{
	unsigned int i = 0;
	for(; i < refreshRatesNum; i++)	{
		if (freq == refreshRates[i])
			break;
	}
	if (i == refreshRatesNum) {
		refreshRates[refreshRatesNum++] = freq;
	}
}


const char*		AdapterHandler::GetApiName (RendererAPI api)
{
	switch (api) {
		case DirectX7:
			return GetDX7AdapterHandler ()->GetApiName ();
		case DirectX9:
			return GetDX9AdapterHandler ()->GetApiName ();
	}
	return NULL;
}


bool			AdapterHandler::IsAPIAvailable (RendererAPI api)
{
	SwitchToAPI (api);
	return iHandler != NULL;
}


unsigned int	AdapterHandler::GetNumOfAdapters (RendererAPI api)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		return iHandler->GetNumOfAdapters ();
	}
	return 0;
}


const char*		AdapterHandler::GetDescriptionOfAdapter (RendererAPI api, unsigned int index)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		return iHandler->GetDescriptionOfAdapter ((unsigned short) index);
	}
	return NULL;
}


unsigned int	AdapterHandler::GetNumOfDeviceTypes (RendererAPI api, unsigned int adapter)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		return iHandler->GetNumOfDeviceTypes (adapter);
	}
	return 0;
}


const char*		AdapterHandler::GetDescriptionOfDeviceType (RendererAPI api, unsigned int adapter, unsigned int index)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		return iHandler->GetDescriptionOfDeviceType (adapter, (unsigned short) index);
	}
	return NULL;
}


void			AdapterHandler::RefreshResolutionListByAdapter (RendererAPI api, unsigned int adapter)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		displayModesNum = 0;
		iHandler->EnumResolutions (adapter, *this);
		if (displayModesNum != 0) {
			for (unsigned int i = displayModesNum - 1; i > 0; i--) {
				for (unsigned int j = 0; j < i; j++) {
					if (displayModes[j].x > displayModes[j + 1].x) {
						unsigned int x = displayModes[j].x;
						unsigned int y = displayModes[j].y;
						displayModes[j].x = displayModes[j+1].x;
						displayModes[j].y = displayModes[j+1].y;
						displayModes[j+1].x = x;
						displayModes[j+1].y = y;
					}
				}
			}
		}
	}
}	


unsigned int	AdapterHandler::GetNumOfResolutions ()
{
	return displayModesNum;
}


AdapterHandler::DisplayMode		AdapterHandler::GetResolution (unsigned int index)
{
	return displayModes[index];
}


void			AdapterHandler::RefreshRefreshRateListByDisplayMode (RendererAPI api, unsigned int adapter,
																	 const DisplayMode& mode, unsigned int bitDepth)
{
	SwitchToAPI (api);
	if (iHandler != NULL) {
		refreshRatesNum = 0;
		iHandler->EnumRefreshRates (adapter, mode.x, mode.y, bitDepth, *this);
		if (refreshRatesNum != 0) {
			for (unsigned int i = refreshRatesNum - 1; i > 0; i--) {
				for (unsigned int j = 0; j < i; j++) {
					if (refreshRates[j] > refreshRates[j + 1]) {
						unsigned int f = refreshRates[j];
						refreshRates[j] = refreshRates[j+1];
						refreshRates[j+1] = f;
					}
				}
			}
		}
	}
}


unsigned int	AdapterHandler::GetNumOfRefreshRates ()
{
	return refreshRatesNum;
}


unsigned int	AdapterHandler::GetRefreshRate (unsigned int index)
{
	return refreshRates[index];
}


bool			AdapterHandler::GetCurrentDisplayMode (DisplayMode& resolution, unsigned int& bitDepth, unsigned int& refRate)
{
	if (iHandler == NULL) {
		SwitchToAPI (DirectX9);
	}
	if (iHandler != NULL) {
		unsigned int xRes = 0;
		unsigned int yRes = 0;
		if (iHandler->GetCurrentDisplayMode (xRes, yRes, bitDepth, refRate)) {
			resolution.x = xRes;
			resolution.y = yRes;
			return true;
		}
	}
	return false;
}


void			AdapterHandler::ShutDown ()
{
	ShutDownCurrentAPI ();
}

/*------------------------------------------------------------------------------------------*/
/*..................................... WrapperInstances ....................................*/

WrapperInstances::InstanceInfo		WrapperInstances::instanceInfo[]	= { {"glide.dll", " (Glide 1.x)"}, {"glide2x.dll", " (Glide 2.x)"}, {NULL, NULL} };

WrapperInstances::WrapperInstances ():
	numOfInstances (0)
{
}

WrapperInstances::~WrapperInstances ()
{

}


WrapperInstances::ConfigStatus		WrapperInstances::GetConfigAndStatus (const char* path, bool dllFile,
																		  dgVoodooConfig (*configs)[2])
{

dgVoodooConfig	thisConfigs[2];
HANDLE			hFile;
unsigned long	id;

	ConfigStatus status = FileNotFound;
	
	if ((hFile = _OpenFile (path)) != INVALID_HANDLE_VALUE) {
		status = ConfigError;
		int pos = dllFile ? 0x40 : 0;
		if (_SetFilePos (hFile, pos, FILE_BEGIN) == pos)	{
			if (_ReadFile(hFile, &thisConfigs, 2*sizeof(dgVoodooConfig)) == 2*sizeof(dgVoodooConfig)) {
				pos = 2 * sizeof(dgVoodooConfig) + 0x40;
				if (_SetFilePos (hFile, pos, FILE_BEGIN) == pos)	{
					if (_ReadFile (hFile, &id, 4) == 4) {
						if (configs != NULL) {
							(*configs)[0] = thisConfigs[0];
							(*configs)[1] = thisConfigs[1];
						}
						if (id == 'EGED') {
							if ((thisConfigs[0].ProductVersion != DGVOODOOVERSION) ||
								(thisConfigs[1].ProductVersion != DGVOODOOVERSION)) {
								status = ConfigInvalidVersion;
							} else {
								status = ConfigOK;
							}
						}
					}
				}
			}
		}
		_CloseFile(hFile);
	}
	return status;
}


WrapperInstances::ConfigStatus		WrapperInstances::GetConfigAndStatus (const char* path, dgVoodooConfig (*configs)[2] /*= NULL*/)
{
	return GetConfigAndStatus (path, true, configs);
}


WrapperInstances::ConfigStatus		WrapperInstances::GetExternalConfigAndStatus (const char* path, dgVoodooConfig (*configs)[2] /*= NULL*/)
{
	return GetConfigAndStatus (path, false, configs);
}


WrapperInstances::ConfigStatus		WrapperInstances::ReadConfig (unsigned int instance, dgVoodooConfig (&configs)[2])
{
	if (instance < numOfInstances) {
		return GetConfigAndStatus (wrapperInstances[instance].filePath, &configs);
	}
	return FileNotFound;
}


bool				WrapperInstances::WriteConfig (const char* path, bool dllFile, const dgVoodooConfig (&configs)[2])
{
char			temp[256];
FILETIME		create, write;

	bool succeeded = false;
	HANDLE handle = CreateFile (path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE)	{
		int pos = dllFile ? 0x40 : 0;
		if (_SetFilePos (handle, pos, FILE_BEGIN) == pos)	{
			if (_ReadFile (handle, temp, 2 * sizeof(dgVoodooConfig)) ==  2 * sizeof(dgVoodooConfig)) {
				unsigned long	id;
				if (_ReadFile (handle, &id, 4) == 4) {
					if (id == 'EGED')	{
						//pos = 0x40;
						if (_SetFilePos (handle, pos, FILE_BEGIN) == pos)	{
							GetFileTime (handle, &create, NULL, &write);
							if (_WriteFile (handle, (void*) (configs), 2 * sizeof(dgVoodooConfig)) == 2 * sizeof(dgVoodooConfig) )	{
								succeeded = true;
							}
							SetFileTime (handle, &create, NULL, &write);
						}
					}
				}
			}
		}
		_CloseFile(handle);
	} /*else {
		char	strBuffer1[MAXSTRINGLENGTH];
		char	strBuffer2[MAXSTRINGLENGTH];
		resourceManager.GetString (langIndex, IDS_CANNOTWRITECONFIG, strBuffer1);
		resourceManager.GetString (langIndex, IDS_ERRORDURINGSAVE, strBuffer2);
		MessageBox (hDlgMain, strBuffer1, strBuffer2, MB_ICONSTOP | MB_OK);
	}*/
	return succeeded;
}


bool				WrapperInstances::WriteConfig (unsigned int instance, const dgVoodooConfig (&configs)[2])
{
	bool succeeded = false;

	if (instance < numOfInstances) {
		succeeded = WriteConfig (wrapperInstances[instance].filePath, true, configs);
	}
	return succeeded;
}


bool				WrapperInstances::WriteExternalConfig (const char* path, const dgVoodooConfig (&configs)[2])
{
	return WriteConfig (path, false, configs);
}


bool				WrapperInstances::SearchWrapperInstances (const char *buffer, unsigned int glideVersion /*= 0*/)
{
	char	path[1024];
	strcpy (path, buffer);
	int pos = strlen (path);
	
	if (path[pos-1] != '\\') path[pos++] = '\\';
	path[pos] = 0x0;

	bool found = false;
	for (int i = 0; instanceInfo[i].name != NULL; i++)	{
		if (glideVersion == 0 || glideVersion == (i+1)) {
			int	postemp = pos;
			for (int j = 0; instanceInfo[i].name[j]; j++) path[postemp++] = instanceInfo[i].name[j];
			path[postemp] = 0;
			if (!IsInList (path)) {
				if (GetConfigAndStatus (path, NULL) != FileNotFound)	{
					path[postemp] = 0;
					WrapperInstance instance;
					strcpy (instance.filePath, path);
					AddInstance (instance);
					found = true;
				}
			}
		}
	}
	return found;
}


bool				WrapperInstances::IsInList (const char* path) const
{
	char src[MAXSTRINGLENGTH];
	strcpy (src, path);
	strupr (src);
	bool inList = false;
	for (unsigned int i = 0; i < numOfInstances; i++) {
		char dst[MAXSTRINGLENGTH];
		strcpy (dst, wrapperInstances[i].filePath);
		strupr (dst);
		if (strcmp (src, dst) == 0) {
			inList = true;
			break;
		}
	}
	return inList;
}


void				WrapperInstances::AddInstance (const WrapperInstance& instance)
{
	if (numOfInstances < 32) {
		wrapperInstances[numOfInstances++] = instance;
	}
}


void				WrapperInstances::Clear ()
{
	numOfInstances = 0;
}


unsigned int		WrapperInstances::GetNumOfInstances () const
{
	return numOfInstances;
}


WrapperInstances::WrapperInstance		WrapperInstances::GetInstance (unsigned int index)
{
	return wrapperInstances[index];
}

/*------------------------------------------------------------------------------------------*/
/*.................................... MainDialog ......................................*/

UI::MainDialog::MainDialog ():
	uxThemeHandle			(NULL),
	pSetThemeAppProperties	(NULL)
{
	ZeroMemory (pageVisible, sizeof (pageVisible));
	ZeroMemory (hFloatingMenus, sizeof(hFloatingMenus));

	uxThemeHandle = LoadLibrary ("uxtheme.dll");
	if (uxThemeHandle != NULL) {
		pSetThemeAppProperties = (SetThemeAppPropertiesProc) GetProcAddress (uxThemeHandle, "SetThemeAppProperties");
	}
}


UI::MainDialog::~MainDialog ()
{
	if (uxThemeHandle != NULL) {
		FreeLibrary (uxThemeHandle);
	}
}


unsigned int	UI::MainDialog::GetTabIndex (unsigned int pageIndex)
{
	unsigned int cnt = 0;
	for (int i = 0; i < (pageIndex+1); i++) {
		if (pageVisible[i]) {
			cnt++;
		}
	}
	return cnt-1;
}


unsigned int	UI::MainDialog::GetPageIndex (unsigned int tabIndex)
{
	unsigned int cnt = 0;
	for (int i = 0; i < 8; i++) {
		if (pageVisible[i]) {
			if (cnt == tabIndex)
				return i;

			cnt++;
		}
	}
	return 0;
}


void	UI::MainDialog::SetLanguage (unsigned int newLangIndex)
{
	langIndex = newLangIndex;
	config.language = langIndex;

	SetWindowPos (topPage->GetPageWindow (hDlgMain, hToolTip), NULL, 0, 0, 0, 0,
					SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos (bottomPage->GetPageWindow (hDlgMain, hToolTip), NULL, 0, 0, 0, 0,
					SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);

	topPage->SetLanguage (langIndex);
	bottomPage->SetLanguage (langIndex);

	SetWindowPos (topPage->GetPageWindow (hDlgMain, hToolTip), NULL, 0, 0, 0, 0,
					SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos (bottomPage->GetPageWindow (hDlgMain, hToolTip), NULL, 0, 0, 0, 0,
					SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
	
	InvalidateRect (topPage->GetPageWindow (hDlgMain, hToolTip), NULL, TRUE);
	InvalidateRect (bottomPage->GetPageWindow (hDlgMain, hToolTip), NULL, TRUE);
	
	configView->SetLanguage (langIndex);

	for (int i = 0; i < TabCtrl_GetItemCount(GetDlgItem (hDlgMain, IDC_MAINTAB)); i++) {
		char	strBuffer[MAXSTRINGLENGTH];
		configView->GetPageName (GetPageIndex (i), strBuffer);
		
		TC_ITEM		tabitem;
		tabitem.mask = TCIF_TEXT;
		tabitem.pszText = strBuffer;
		TabCtrl_SetItem (GetDlgItem (hDlgMain, IDC_MAINTAB), i, &tabitem);
	}
	SelectPage (GetSelectedPage ());
}


void	UI::MainDialog::SetConfigState (ConfigState state)
{
	configView->SetConfigState ((CV::ConfigView::ConfigState) state);
}


void	UI::MainDialog::ShowPage (unsigned int pageIndex)
{
	if (!pageVisible[pageIndex]) {
		pageVisible[pageIndex] = true;
		char	strBuffer[MAXSTRINGLENGTH];
		configView->GetPageName (pageIndex, strBuffer);
		
		TC_ITEM		tabitem;
		tabitem.mask = TCIF_TEXT;
		tabitem.pszText = strBuffer;
		TabCtrl_InsertItem (GetDlgItem (hDlgMain, IDC_MAINTAB), GetTabIndex (pageIndex), &tabitem);
	}
}


void	UI::MainDialog::HidePage (unsigned int pageIndex)
{
	if (pageVisible[pageIndex]) {
		unsigned int tabIndex = GetTabIndex (pageIndex);
		if (TabCtrl_GetCurSel(GetDlgItem (hDlgMain, IDC_MAINTAB)) == tabIndex)	{
			SelectPage (tabIndex - 1);
		}
		pageVisible[pageIndex] = false;
		TabCtrl_DeleteItem(GetDlgItem (hDlgMain, IDC_MAINTAB), tabIndex);
	}
}


void	UI::MainDialog::SelectPage (unsigned int tabIndex)
{
	if (TabCtrl_GetItemCount (GetDlgItem (hDlgMain, IDC_MAINTAB)) != 0) {
		TabCtrl_SetCurSel (GetDlgItem (hDlgMain, IDC_MAINTAB), tabIndex);
		unsigned int pageIndex = GetPageIndex (tabIndex);
		HWND pageToShow = configView->GetPageWindow (pageIndex, hDlgMain, hToolTip);
		if (pageToShow != NULL) {
			HWND pageToShow = configView->GetPageWindow (pageIndex, hDlgMain, hToolTip);
			if (pageToShow != NULL) {
				RECT tabRect;
				GetWindowRect (GetDlgItem (hDlgMain, IDC_MAINTAB), &tabRect);
				MapWindowPoints (NULL, hDlgMain, (LPPOINT) &tabRect, 2);
				TabCtrl_AdjustRect (GetDlgItem (hDlgMain, IDC_MAINTAB), FALSE, &tabRect);
				SetWindowPos (pageToShow, GetWindow (GetDlgItem (hDlgMain, IDC_MAINTAB), GW_HWNDPREV), tabRect.left, tabRect.top, 0, 0, SWP_NOSIZE);
			}
			if (hActPage) {
				SetupHideWindow (hActPage);
			}
			SetupShowWindow (hActPage = pageToShow);
		}
	}
}


unsigned int	UI::MainDialog::GetSelectedPage () const
{
	return TabCtrl_GetCurSel(GetDlgItem (hDlgMain, IDC_MAINTAB));
}


void			UI::MainDialog::FillGetOpenFileStruc (OPENFILENAME&	fn, const char* filter1, const char* filter2)
{
char			filter[MAXSTRINGLENGTH];
char			strBuffer[MAXSTRINGLENGTH];

	ZeroMemory (&fn, sizeof(fn));
	fn.lStructSize = sizeof(OPENFILENAME);
	strcpy (filter, filter1);
	strcpy (filter+strlen(filter)+1, filter2);
	unsigned int len = strlen(filter1) + strlen(filter2) + 2;
	filter[len] = filter[len+1] = 0x0;
	fn.lpstrFilter = filter;
	resourceManager.GetString (langIndex, IDS_NAMEOFTEXTFILE, strBuffer);
	fn.lpstrTitle = strBuffer;
	fn.hwndOwner = hDlgMain;
	fn.lpstrCustomFilter = NULL;
	fn.nFilterIndex = 1;
	fn.nMaxFile = 256;
	fn.lpstrFileTitle = NULL;
	fn.lpstrInitialDir = NULL;			
	fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES;
	fn.hInstance = GetModuleHandle (NULL);
}


bool			UI::MainDialog::IsMenuItemChecked (unsigned int id)
{
	bool checked = false;
	MENUITEMINFO info;
	ZeroMemory (&info, sizeof(info));
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	for (unsigned int i = 0; i < 16; i++) {
		if (hFloatingMenus[i] != NULL) {
			GetMenuItemInfo (GetSubMenu (hFloatingMenus[i], 0), id, FALSE, &info);
			return ((info.fState & MFS_CHECKED) == MFS_CHECKED);
		}
	}

	return false;
}


bool			UI::MainDialog::ToggleMenuItemCheck (unsigned int id)
{
	bool checked = IsMenuItemChecked (id);
	for (unsigned int i = 0; i < 16; i++) {
		if (hFloatingMenus[i] != NULL) {
			CheckMenuItem (GetSubMenu (hFloatingMenus[i], 0), id, (checked ? MF_UNCHECKED : MF_CHECKED));
		}
	}
	return checked;
}


bool			UI::MainDialog::SetMenuItemStatus (unsigned int id, bool enabled)
{
	for (unsigned int i = 0; i < 16; i++) {
		if (hFloatingMenus[i] != NULL) {
			EnableMenuItem (GetSubMenu (hFloatingMenus[i], 0), id, (enabled ? MF_ENABLED : MF_DISABLED));
		}
	}
	return true;//checked;
}


void			UI::MainDialog::HandleFloatingMenus (WPARAM wParam, LPARAM lParam)
{
OPENFILENAME	fn;
char			filename[MAX_PATH];
char			strBuffer1[MAXSTRINGLENGTH];
char			strBuffer2[MAXSTRINGLENGTH];

	unsigned int num = configView->GetCustomMenuItemsNum ();
	HMENU hMenu = hFloatingMenus[langIndex];
	if (hMenu == NULL) {
		hMenu = hFloatingMenus[langIndex] = resourceManager.GetMenu (langIndex, IDR_FLOATINGMENU);
		if (hMenu != NULL) {
			if (num != 0) {
				for (unsigned int i = 0; i < num; i++) {
					unsigned int id = configView->GetCustomMenuItem (i, strBuffer1);
					bool checked = IsMenuItemChecked (id);
					InsertMenu (GetSubMenu (hMenu, 0), i, MF_BYPOSITION | MF_ENABLED | MF_STRING | (checked ? MF_CHECKED : MF_UNCHECKED),
								id, strBuffer1);
				}
				InsertMenu (GetSubMenu (hMenu, 0), num, MF_BYPOSITION | MF_ENABLED | MF_SEPARATOR, 0, 0);
			}
			ToggleMenuItemCheck (IDM_XPSTYLE);
		}
	}
	unsigned int mid = 0;
	if (hMenu != NULL) {
		mid = TrackPopupMenu (GetSubMenu (hMenu, 0), TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hDlgMain, NULL);
	}
	switch (mid)	{
		case IDM_EXPORTSETTINGS:
			resourceManager.GetString (langIndex, IDS_TEXTFILESFILTER, strBuffer1);
			resourceManager.GetString (langIndex, IDS_ALLFILESFILTER, strBuffer2);
			FillGetOpenFileStruc (fn, strBuffer1, strBuffer2);
			filename[0] = 0;
			fn.lpstrFile = filename;
			if (GetSaveFileName(&fn))	{
				if (!ExportConfiguration (filename, config))
					MessageBox(hDlgMain, (GetString (strBuffer1, cannotExportStrs[lang][0]), strBuffer1),
										 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
			}
			break;

		case IDM_XPSTYLE:
				setupConfig.flags ^= CFG_XPSTYLE;
				ToggleMenuItemCheck (mid);
				SetEnableTheming ();
				break;

		default:
			bool checked = false;
			if (configView->IsCustomMenuItemCheckable (mid)) {
				checked = ToggleMenuItemCheck (mid);
			}
			configView->HandleMenuItem (mid, !checked);
			break;
	}
		

		//	case IDM_SAVECONFIG:
		//		if (lang == LANG_HUNGARIAN)	{
		//			fn.lpstrFilter = "Konfigurációs fájlok (*.cfg)\0*.cfg\0Összes fájl (*.*)\0*.*\0\0\0";
		//		} else {
		//			fn.lpstrFilter = "Configuration files (*.cfg)\0*.cfg\0All files (*.*)\0*.*\0\0\0";
		//		}
		//		fn.lpstrTitle = (GetString (strBuffer1, nameOfConfigFileStrs[lang][0]), strBuffer1);
		//		fn.hwndOwner = hDlgMain;
		//		fn.lpstrCustomFilter = NULL;
		//		fn.nFilterIndex = 1;
		//		fn.lpstrFile = filename;
		//		fn.nMaxFile = 256;
		//		fn.lpstrFileTitle = NULL;
		//		fn.lpstrInitialDir = NULL;			
		//		fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES;
		//		fn.hInstance = dlgClass.hInstance;
		//		filename[0] = 0;
		//		if (GetSaveFileName(&fn))	{
		//			if (!SaveConfiguration(filename))
		//				MessageBox(hDlgMain, (GetString (strBuffer1, cannotSaveConfigStrs[lang][0]), strBuffer1),
		//									 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
		//		}
		//		break;

		//	case IDM_LOADCONFIG:
		//		if (lang == LANG_HUNGARIAN)	{
		//			fn.lpstrFilter = "Konfigurációs fájlok (*.cfg)\0*.cfg\0Összes fájl (*.*)\0*.*\0\0\0";
		//		} else {
		//			fn.lpstrFilter = "Configuration files (*.cfg)\0*.cfg\0All files (*.*)\0*.*\0\0\0";
		//		}
		//		fn.lpstrTitle = (GetString (strBuffer1, nameOfConfigFileStrs[lang][0]), strBuffer1);
		//		fn.hwndOwner = hDlgMain;
		//		fn.lpstrCustomFilter = NULL;
		//		fn.nFilterIndex = 1;
		//		fn.lpstrFile = filename;
		//		fn.nMaxFile = 256;
		//		fn.lpstrFileTitle = NULL;
		//		fn.lpstrInitialDir = NULL;			
		//		fn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST;
		//		fn.hInstance = dlgClass.hInstance;
		//		filename[0] = 0;
		//		if (GetOpenFileName(&fn))	{
		//			item = LoadConfiguration(filename);
		//			switch(item)	{
		//				case 0:
		//					MessageBox(hDlgMain, (GetString (strBuffer1, cannotLoadConfigStrs[lang][0]), strBuffer1),
		//									 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
		//					
		//					break;
		//				
		//				case -1:
		//					MessageBox(hDlgMain, (GetString (strBuffer1, invalidConfigFileStrs[lang][0]), strBuffer1),
		//									 (GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
		//					break;
		//				
		//				case -2:
		//					MessageBox(hDlgMain, (GetString (strBuffer1, invalidExtConfigStrs[lang][0]), strBuffer1),
		//									(GetString (strBuffer2, errorStrs[lang][0]), strBuffer2), MB_OK);
		//					break;

		//				default:
		//					config = defaultConfigs[platform];
		//					SetWindowToConfig();
		//					break;
		//			}
		//		}
		//		break;

		//	case IDM_XPSTYLE:
		//		setupConfig.flags ^= CFG_XPSTYLE;
		//		for (i = 0; i < numberOfLanguages; i++) {
		//			CheckMenuItem (GetSubMenu(hFloatingMenus[i], 0), IDM_XPSTYLE, MF_BYCOMMAND | (setupConfig.flags & CFG_XPSTYLE ? MF_CHECKED : MF_UNCHECKED));
		//		}
		//		SetEnableTheming ();
		//		break;
}


void			UI::MainDialog::DisplayChanged ()
{
	topPage->DisplayChanged ();
}


void			UI::MainDialog::SetEnableTheming ()
{
unsigned int	i;

	if (pSetThemeAppProperties != NULL) {
		(*pSetThemeAppProperties) ((setupConfig.flags & CFG_XPSTYLE ? STAP_ALLOW_CONTROLS : 0) | STAP_ALLOW_NONCLIENT);
		EnumChildWindows (hDlgMain, ThemeChangeEnumProc, 0);
		configView->ThemeChanged ();
		RedrawWindow (hDlgMain, NULL, NULL, RDW_INVALIDATE);
	}
}


int CALLBACK	UI::MainDialog::DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
RECT			rect1,rect2;
MainDialog*		self = NULL;

	hDlgMain = hwndDlg;
	switch(uMsg)	{
		case WM_INITDIALOG:
			{
				self = (MainDialog*) lParam;
				SetWindowLong (hwndDlg, GWL_USERDATA, (LONG) lParam);

				// Adjust main tab
				GetWindowRect(GetDlgItem(hwndDlg, IDC_MAINTAB), &rect1);

				TabCtrl_GetItemRect(GetDlgItem(hwndDlg, IDC_MAINTAB), 0, &rect2);
				TabCtrl_SetItemSize(GetDlgItem(hwndDlg, IDC_MAINTAB),(rect1.right - rect1.left - 3) / 3, rect2.bottom - rect2.top);

				// Create tooltip
				hToolTip = CreateWindowEx (0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
										   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
										   hwndDlg, NULL, dlgClass.hInstance, NULL);

				SetWindowPos (hToolTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

				ZeroMemory (&toolInfo, sizeof (toolInfo));
				toolInfo.cbSize = sizeof (toolInfo);
				toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
				toolInfo.hwnd = hwndDlg;
				toolInfo.lpszText = LPSTR_TEXTCALLBACK;

				SendMessage (hToolTip, (UINT) TTM_ACTIVATE, (WPARAM) TRUE, 0);
				SendMessage (hToolTip, (UINT) TTM_SETMAXTIPWIDTH, 0, 512);
				SendMessage (hToolTip, (UINT) TTM_SETDELAYTIME, TTDT_AUTOPOP, 15000);

				// Initialize
				configView = CV::configViewManager.GetView (self, CV::ConfigViewManager::ClassicView, 0);
				topPage = UI::CreateTopPage (*self);
				bottomPage = UI::CreateBottomPage (*self);

				ShowWindow (topPage->GetPageWindow (hwndDlg, hToolTip), SW_SHOWNA);
				ShowWindow (bottomPage->GetPageWindow (hwndDlg, hToolTip), SW_SHOWNA);

				self->SetEnableTheming ();

				//SetupUIPages ();

				/*for (i = 0; i < numberOfLanguages; i++) {
					self->hFloatingMenus[i] = LoadMenu(dlgClass.hInstance, MAKEINTRESOURCE(floatingMenuIDs[i]));
				}*/

				self->SetLanguage (config.language);

				configView->Init ();
			}
			return 1;

		case WM_DESTROY:
			{
				self = (MainDialog*) GetWindowLong (hwndDlg, GWL_USERDATA);
				/*for (i = 0; i < numberOfLanguages; i++) {
					for (j = 0; j < numberOfPages; j++)	{
						//DestroyWindow (pages[i][j]);
					}
				}*/
				DestroyWindow (hToolTip);
			}
			return 1;

		case WM_NOTIFY:
			{
				self = (MainDialog*) GetWindowLong (hwndDlg, GWL_USERDATA);
				if ( ((LPNMHDR) lParam)->code == TCN_SELCHANGE)	{
					self->SelectPage (TabCtrl_GetCurSel(GetDlgItem(hDlgMain, IDC_MAINTAB)));
					return 1;
				} /*else if (((LPNMHDR) lParam)->code == TTN_GETDISPINFO) {
					ttnDispInfo = (LPNMTTDISPINFO) lParam;
					ttnDispInfo->hinst = NULL;
					if (config.language == English) {
						ttnDispInfo->lpszText = (GetString (toolTipBuffer, ttnDispInfo->lParam), toolTipBuffer);
					} else {
						ttnDispInfo->lpszText = (GetString (toolTipBuffer, ttnDispInfo->lParam+2000), toolTipBuffer);
					}
					return 1;
				}*/
			}
			return 0;

		case WM_CONTEXTMENU:
			{
				self = (MainDialog*) GetWindowLong (hwndDlg, GWL_USERDATA);
				self->HandleFloatingMenus (wParam, lParam);
			}
			return 1;

		case WM_DISPLAYCHANGE:
			{
				self = (MainDialog*) GetWindowLong (hwndDlg, GWL_USERDATA);
				self->DisplayChanged ();
			}
			return 0;
		
		default:
			return 0;
	}	
}


void			UI::MainDialog::Invoke ()
{
	DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_DIALOGMAIN), NULL, DialogProc, (LPARAM) this);
}

/*------------------------------------------------------------------------------------------*/
/*...................................... Függvények ........................................*/

void	GetString (LPSTR lpBuffer, UINT uID)
{
	LoadString (dlgClass.hInstance, uID, lpBuffer, MAXSTRINGLENGTH);
}


BOOL CALLBACK ThemeChangeEnumProc(HWND hwnd, LPARAM lParam) {

	SendMessage (hwnd, 0x31A, 0, 0);
	return (TRUE);
}


void	SetupHideWindow (HWND window)
{
	SetWindowPos (window, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE);
}


void	SetupShowWindow (HWND window)
{
	SetWindowPos (window, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE);
	InvalidateRect (window, NULL, TRUE);
	RedrawWindow (window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}


void	AddControlToToolTip (HWND hToolTip, TOOLINFO* toolInfo, HWND parentDlgHwnd, int ctlId)
{
	toolInfo->uId = (UINT_PTR) GetDlgItem(parentDlgHwnd, ctlId);
	toolInfo->lParam = ctlId;
	SendMessage(hToolTip, (UINT) TTM_ADDTOOL, 0, (LPARAM) toolInfo);
}
			

LRESULT CALLBACK dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	if (uMsg == WM_CLOSE)	{
		EndDialog(hwnd, 1);
		return(0);
	} else
		return(DefDlgProc(hwnd, uMsg, wParam, lParam));
}


void	SetConfig (const dgVoodooConfig& configParam)
{
	config = configParam;
	configView->ConfigChanged ();
}


void	Exit (bool writeCurrentConfig)
{
	if (writeCurrentConfig) {
		FlushCurrentConfig (topPage);
	}
	EndDialog (hDlgMain, 1);
}


char	toolTipBuffer[MAXSTRINGLENGTH];


int SaveConfiguration(char *filename)	{
HANDLE		handle;

	if ( (handle = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
							  FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE) return(0);
	if (_WriteFile(handle, CFGFILEID, sizeof(CFGFILEID)) != sizeof(CFGFILEID) )	{
		_CloseFile(handle);
		return(0);
	}
/*	if (_WriteFile(handle, configs, 2 * sizeof(dgVoodooConfig)) != 2 * sizeof(dgVoodooConfig) )	{
		_CloseFile(handle);
		return(0);
	}*/
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
	
	//configs[0] = thisConfigs[0];
	//configs[1] = thisConfigs[1];
	return(1);
}


BOOL CALLBACK EnablingCallback(HWND hwnd, LPARAM lParam)	{

	//EnableWindow(hwnd, CheckEnablingDependencies( (HWND) lParam, GetDlgCtrlID(hwnd) ));
	return(TRUE);
}


BOOL CALLBACK DisablingCallback(HWND hwnd, LPARAM lParam)	{
	
	//EnableWindow(hwnd, FALSE);
	return(TRUE);
}


void _inline DisableAllChildWindows(HWND hwnd)	{

	//EnumChildWindows(hwnd, DisablingCallback, 0);
}


void _inline EnableAllChildWindows(HWND hwnd)	{

	//EnumChildWindows(hwnd, EnablingCallback, (LPARAM) hwnd);
}


void DisableConfig (/*char *strTable, char *toCopyInStr*/)	{
unsigned int	i;
//char	strBuffSrc[MAXSTRINGLENGTH];
//char	strBuffDst[MAXSTRINGLENGTH];

	for (i = 0; i < numberOfLanguages; i++) {
		//DisableAllChildWindows(pages[i][pageGlobal]);
		//DisableAllChildWindows(pages[i][pageGlide]);
		//DisableAllChildWindows(pages[i][pageVesa]);
		//DisableAllChildWindows(pages[i][pageRenderer]);
		
		/*if (toCopyInStr != NULL) {
			GetString (strBuffSrc, strTable[i]);
			sprintf (strBuffDst, strBuffSrc, toCopyInStr);
		} else {
			GetString (strBuffDst, strTable[i]);
		}
		SendMessage(GetDlgItem(pages[i][pageTop], IDC_ERROR), WM_SETTEXT, 0, strTable[i] );*/
	}
}


void EnableConfig()	{
unsigned int	i = 0;

	if (configvalid)	{
		for (i = 0; i < numberOfLanguages; i++) {
			//EnableAllChildWindows(pages[i][pageGlobal]);
			//EnableAllChildWindows(pages[i][pageGlide]);
			//EnableAllChildWindows(pages[i][pageVesa]);
			//EnableAllChildWindows(pages[i][pageRenderer]);
			//SendMessage(GetDlgItem(pages[i][pageTop], IDC_ERROR), WM_SETTEXT, 0, (LPARAM) "");
		}
	}
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)	{
//DDSURFACEDESC2	temp;
/*LOGFONT		font =	{-19, 0, 0, 0, 700, 0, 0, 0, 238, 3, 2, 1, 34, "Verdana"};
CHOOSEFONT	cf= {sizeof(CHOOSEFONT),NULL,0,&font,0,CF_SCREENFONTS|CF_TTONLY,0,0,NULL,NULL,NULL,NULL,0,0,0};*/

	if (resourceManager.Init ()) {

		::hInstance = hInstance;
		//ofw.hInstance = dlgClass.hInstance = hInstance;

		CopyMemory (&setupConfig, (const void *) (((char *) hInstance) + 0x40), sizeof(SetupConfig));

		//ChooseFont(&cf);
		
		dlgClass.hCursor = LoadCursor(0,IDC_ARROW);
		dlgClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		RegisterClass(&dlgClass);

		if (EmptyCommandLine (cmdLine)) {

			if (!adapterHandler.IsAPIAvailable (DirectX7)) {
				MessageBox(NULL, "You need DirectX 7.0 or greater installed to use dgVoodoo!",
						   "Warning",MB_OK | MB_ICONWARNING);
			}		

			UI::MainDialog mainDialog;
			mainDialog.Invoke ();

		} else {
			
			ProcessCommandLine (cmdLine);

		}
		adapterHandler.ShutDown ();

		resourceManager.Exit ();
	}
}


CV::ConfigView::ConfigView (UI::MainDialog& mainDialog, unsigned int langIndex):
	mainDialog	(mainDialog),
	langIndex		(langIndex)
{
}


CV::ConfigView::~ConfigView ()
{
}


void	CV::ConfigView::SetLanguage (unsigned int langIndexParam)
{
	langIndex = langIndexParam;
}


/*bool	CV::ConfigView::Activate (unsigned int langIndex)
{
	return true;
}*/


UI::Page::Page (MainDialog& mainDialog, unsigned int langIndexParam /*= 0*/):
	mainDialog (mainDialog),
	toolTipWnd (NULL)
{
	ZeroMemory (pageWnd, sizeof (pageWnd));

	ZeroMemory (&toolInfo, sizeof (toolInfo));
	toolInfo.cbSize = sizeof (toolInfo);
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_TRANSPARENT;
	toolInfo.lpszText = LPSTR_TEXTCALLBACK;
	langIndex = langIndexParam;
}


UI::Page::~Page ()
{
}


INT_PTR	UI::Page::DefaultProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Page* self = NULL;

	switch (uMsg)	{
		case WM_INITDIALOG:
			self = reinterpret_cast<Page*> (lParam);
			self->pageWnd[self->langIndex] = hwndDlg;
			break;

		default:
			self = reinterpret_cast<Page*> (GetWindowLong (hwndDlg, GWL_USERDATA));
			break;

	}

	LRESULT result = 0;

	if (self != NULL) {
		result = self->HandleMsg (hwndDlg, uMsg, wParam, lParam);
	}

	return result;
}


int		UI::Page::HandleMsg (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)	{
		case WM_INITDIALOG:
			toolInfo.hwnd = hwnd;
			langIndex = GetLangIndexFromWindow (hwnd);
			if (Init (hwnd)) {
				return TRUE;
			} else {
				return FALSE;
			}
			break;
			//return(FALSE);

		case WM_NOTIFY:
			if (((LPNMHDR) lParam)->code == TTN_GETDISPINFO) {
				LPNMTTDISPINFO ttnDispInfo = (LPNMTTDISPINFO) lParam;
				ttnDispInfo->hinst = NULL;
				ttnDispInfo->lpszText = toolTipBuffer;
				resourceManager.GetString (GetLangIndexFromWindow (hwnd), (WORD) ttnDispInfo->lParam, toolTipBuffer);
				return (1);
			}
			return(0);
			break;

		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED) {
				if (ButtonClicked (hwnd, LOWORD (wParam))) {
					return 1;
				}
			} else if (HIWORD(wParam) == CBN_SELCHANGE)	{
				if (ComboBoxSelChanged (hwnd, LOWORD (wParam))) {
					return 1;
				}
			}
			break;

		case WM_HSCROLL:
			{
				int id = GetDlgCtrlID ((HWND) lParam);
				if (id != 0) {
					switch(LOWORD(wParam))	{
						case TB_THUMBTRACK:
						case TB_BOTTOM:
						case TB_ENDTRACK:
						case TB_LINEDOWN:
						case TB_LINEUP:
						case TB_PAGEDOWN:
						case TB_PAGEUP:
						case TB_TOP:
							if (SliderChanged (hwnd, id)) {
								return 1;
							}
					}
				}
			}
			break;
	}

	return 0;//DefDlgProc (hwnd, msg, lParam, wParam);
}


void	UI::Page::AddControlToToolTip (HWND pageHwnd, int ctlId)
{
	toolInfo.uId = (UINT_PTR) GetDlgItem (pageHwnd, ctlId);
	toolInfo.lParam = ctlId;
	SendMessage (toolTipWnd, (UINT) TTM_ADDTOOL, 0, (LPARAM) &toolInfo);
}


HWND	UI::Page::GetWindowFromResourceManager (unsigned int langIndex, HWND mainDlgWnd, HWND toolTipWnd, ResourceManager::UIPages page, DLGPROC lpDialogFunc)
{
	this->toolTipWnd = toolTipWnd;
	this->langIndex = langIndex;
	/*pageWnd[langIndex] =*/ resourceManager.GetUIPageWindow (langIndex, page, mainDlgWnd, lpDialogFunc, this);
	SetWindowLong (pageWnd[langIndex], GWL_USERDATA, (LONG) this);
	return pageWnd[langIndex];
}


unsigned int	UI::Page::GetLangIndexFromWindow (HWND hWnd)
{
	for (unsigned int i = 0; i < MAXLANGNUM; i++) {
		if (hWnd == pageWnd[i]) {
			return i;
		}
	}
	return 0;
}


bool			UI::Page::ButtonClicked (HWND pageWnd, WORD id)
{
	return false;
}


bool			UI::Page::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	return false;
}


bool			UI::Page::SliderChanged (HWND pageWnd, WORD id)
{
	return false;
}


void			UI::Page::DisplayChanged ()
{
}


void			UI::Page::ThemeChanged ()
{
	for (int i = 0; i < MAXLANGNUM; i++) {
		if (pageWnd[i] != NULL) {
			EnumChildWindows (pageWnd[i], ThemeChangeEnumProc, 0);
		}
	}
}


void			UI::Page::SetLanguage (unsigned int langIndexP)
{
	langIndex = langIndexP;
	if (pageWnd[langIndex] != NULL) {
		SetPageToConfig (pageWnd[langIndex]);
	}
}


HWND			UI::Page::GetWindowFromItem (unsigned int item)
{
	return GetDlgItem (pageWnd[langIndex], item);
}


namespace	CV	{

class	ClassicConfigView;

ConfigView*		ConfigViewManager::GetView (UI::MainDialog* dialog, Views view, unsigned int langIndex)
{
	switch (view) {
		case ClassicView:
			if (views[ClassicView] == NULL)	{
				views[ClassicView] = CreateClassicConfigView (*dialog, langIndex);
			} else {
				views[ClassicView]->SetLanguage (langIndex);
			}
			break;

		default:
			return NULL;
	}
	return views[view];
}


ConfigViewManager	configViewManager;

}