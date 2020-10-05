/*--------------------------------------------------------------------------------- */
/* dgVoodooSetupTopBottomPages.cpp - dgVoodooSetup Top & Bottom UI pages of the     */
/*                                   main setup dialog                              */
/*                                                                                  */
/* Copyright (C) 2007 Dege                                                          */
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
/* dgVoodoo: dgVoodooSetupTopBottomPages.cpp												*/
/*			 Top & Bottom pages of the main setup dialog									*/
/*------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "Version.h"
#include "dgVoodooSetup.h"
#include "ConfigTree.h"

#include "Resource.h"
#include "file.h"

namespace	UI	{

// --------------------------------- Top & Bottom Pages -------------------------------------

class	TopPage: public Page
{
private:
	dgVoodooConfig			configs[2];
	static WrapperInstances	wrapperInstances;
	static unsigned int		currentInstance;
	OPENFILENAME			ofw;
	char					ofwTitle[MAXSTRINGLENGTH];
	static char				filenamew[MAX_PATH];
	static bool				configSet;
	
	static AdapterHandler::DisplayMode	displayMode;
	static unsigned int					bitDepth;
	static unsigned int					refRate;

private:
	void			SetPlatformState (HWND pageWnd, bool glide211Config);
	void			GetDisplayMode ();
	void			RefreshDisplayMode (HWND pageWnd);
	void			RefreshInstanceList (HWND pageWnd);
	void			SelectInstance (HWND pageWnd, unsigned int index);
	void			SetConfigOfInstance (HWND pageWnd, unsigned int index);

protected:
	static INT_PTR	CALLBACK TopPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool	Init (HWND pageWnd);
	virtual bool	ButtonClicked (HWND pageWnd, WORD id);
	virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);
	void			DisplayChanged ();

public:
	TopPage (MainDialog& mainDialog);

	virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
	virtual void	SetPageToConfig (HWND pageWnd);

	bool			FlushCurrentConfig ();
};


class	BottomPage: public Page
{
protected:
	static INT_PTR	CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static INT_PTR	CALLBACK BottomPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool	Init (HWND pageWnd);
	virtual bool	ButtonClicked (HWND pageWnd, WORD id);
	virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);

public:
	BottomPage (MainDialog& mainDialog);

	virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
	virtual void	SetPageToConfig (HWND pageWnd);
};

// ------------------------------------- Top Page ------------------------------------------
char				TopPage::filenamew[MAX_PATH];
unsigned int		TopPage::currentInstance = 0;
WrapperInstances	TopPage::wrapperInstances;
bool				TopPage::configSet = false;
AdapterHandler::DisplayMode		TopPage::displayMode;
unsigned int					TopPage::bitDepth = 0;
unsigned int					TopPage::refRate = 0;

void				TopPage::SetPlatformState (HWND pageWnd, bool glide211Config)
{
	EnableWindow (GetDlgItem (pageWnd, IDC_PDOS), glide211Config ? FALSE : TRUE);
}


void				TopPage::GetDisplayMode ()
{
	if (!adapterHandler.GetCurrentDisplayMode (displayMode, bitDepth, refRate)) {
		displayMode.x = displayMode.y = 0;
	}
}


void				TopPage::RefreshDisplayMode (HWND pageWnd)
{
	char	strBuffer[MAXSTRINGLENGTH];
	if (displayMode.x != 0 && displayMode.y != 0 && bitDepth != 0 && refRate != 0) {
		char	formatStr[MAXSTRINGLENGTH];
		resourceManager.GetString (langIndex, IDS_DESKTOPVIDEOMODE, formatStr);
		sprintf (strBuffer, formatStr, displayMode.x, displayMode.y, bitDepth, refRate);
	} else {
		resourceManager.GetString (langIndex, IDS_DESKTOPVIDEOMODEUNKNOWN, strBuffer);
	}
	SendMessage (GetDlgItem (pageWnd, IDC_DESKTOPTEXT), WM_SETTEXT, 0, (LPARAM) strBuffer);
}


void				TopPage::RefreshInstanceList (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_INSTANCELIST), CB_RESETCONTENT, 0, 0);

	if (wrapperInstances.GetNumOfInstances () != 0) {
		for (unsigned int i = 0; i < wrapperInstances.GetNumOfInstances (); i++) {
			WrapperInstances::WrapperInstance instance = wrapperInstances.GetInstance (i);
			SendMessage (GetDlgItem (pageWnd, IDC_INSTANCELIST), CB_INSERTSTRING, (WPARAM) 0, (LPARAM) instance.filePath);
		}
	} else {
		char strBuffer[MAXSTRINGLENGTH];
		resourceManager.GetString (langIndex, IDS_NOWRAPPERINSTANCE, strBuffer);
		SendMessage (GetDlgItem (pageWnd, IDC_INSTANCELIST), CB_INSERTSTRING, (WPARAM) 0, (LPARAM) strBuffer);
	}
}


void				TopPage::SelectInstance (HWND pageWnd, unsigned int index)
{
	SetConfigOfInstance (pageWnd, index);
	index = wrapperInstances.GetNumOfInstances () - index - 1;
	SendMessage (GetDlgItem (pageWnd, IDC_INSTANCELIST), CB_SETCURSEL, (WPARAM) index, 0);
	
}


void				TopPage::SetConfigOfInstance (HWND pageWnd, unsigned int index)
{
	currentInstance = index;
	if (wrapperInstances.GetNumOfInstances () != 0) {
		char	strBuffer[MAXSTRINGLENGTH];
		switch (wrapperInstances.ReadConfig (index, configs)) {
			case WrapperInstances::ConfigOK:
				strBuffer[0] = 0x0;
				if (!configSet) {
					bool glide211Config = (configs[platform].GlideVersion == 0x211);
					SetPlatformState (pageWnd, glide211Config);
					if (glide211Config && platform == PDOS) {
						platform = PWINDOWS;
						SendMessage (GetDlgItem (pageWnd, IDC_PWINDOWS), BM_SETCHECK, (LPARAM) BST_CHECKED, (WPARAM) 0);
						SendMessage (GetDlgItem (pageWnd, IDC_PDOS), BM_SETCHECK, (LPARAM) BST_UNCHECKED, (WPARAM) 0);	
					}
					::SetConfig (configs[platform]);
					mainDialog.SetConfigState (UI::MainDialog::Valid);
				}
				break;
			case WrapperInstances::ConfigError:
			case WrapperInstances::FileNotFound:
				resourceManager.GetString (langIndex, IDS_INVALIDWRAPPERFILE, strBuffer);
				if (!configSet) {
					mainDialog.SetConfigState (UI::MainDialog::Invalid);
				}
				break;
			case WrapperInstances::ConfigInvalidVersion:
				resourceManager.GetString (langIndex, IDS_INCOMPATIBELWRAPPERFILE, strBuffer);
				if (!configSet) {
					mainDialog.SetConfigState (UI::MainDialog::Invalid);
				}
				break;
		}
		SendMessage (GetDlgItem (pageWnd, IDC_ERROR), WM_SETTEXT, 0, (LPARAM) strBuffer);
		configSet = true;
	}
}


INT_PTR CALLBACK	TopPage::TopPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_CTLCOLORSTATIC:
			if (GetDlgCtrlID ((HWND) lParam) == IDC_ERROR) {
				INT_PTR retVal = DefWindowProc (hwndDlg, uMsg, wParam, lParam);
				SetTextColor ((HDC) wParam, RGB (255, 128, 0));
				return retVal;//(INT_PTR) GetStockObject (HOLLOW_BRUSH);
			}
			break;
	}
	return Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}


bool				TopPage::Init (HWND pageWnd)
{
	AddControlToToolTip (pageWnd, IDC_PWINDOWS);
	AddControlToToolTip (pageWnd, IDC_PDOS);
	//AddControlToToolTip (pageWnd, IDC_LANGENGLISH);
	//AddControlToToolTip (pageWnd, IDC_LANGHUNGARIAN);
	AddControlToToolTip (pageWnd, IDC_INSTANCELIST);
	AddControlToToolTip (pageWnd, IDC_ACTDIR);
	AddControlToToolTip (pageWnd, IDC_WINDIR);
	AddControlToToolTip (pageWnd, IDC_SYSDIR);
	AddControlToToolTip (pageWnd, IDC_SEARCH);

	char	strBuffer[MAXSTRINGLENGTH];
	resourceManager.GetString (0, IDS_LANGNAME, strBuffer);
	SendMessage (GetDlgItem (pageWnd, IDC_CBLANGUAGE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
	for (unsigned int i = 0; i < resourceManager.GetNumOfExternalResources (); i++) {
		resourceManager.GetLangNameOfExternalResource (i, strBuffer);
		SendMessage (GetDlgItem (pageWnd, IDC_CBLANGUAGE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
	}

	ZeroMemory (&ofw, sizeof (ofw));
	ofw.lStructSize = sizeof(OPENFILENAME);
	ofw.hwndOwner = hDlgMain;
	ofw.lpstrCustomFilter = NULL;
	ofw.nFilterIndex = 1;
	ofw.lpstrFile = filenamew;
	ofw.nMaxFile = 256;
	ofw.lpstrFileTitle = NULL;
	ofw.lpstrInitialDir = NULL;
	ofw.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST;
	ofw.lpstrTitle = ofwTitle;
	ofw.lpstrFilter = "Dynamic Link Libraries (*.dll)\0*.dll\0Összes fájl (*.*)\0*.*\0\0\0";
	//ofw.lpstrFilter = "Dynamic Link Libraries (*.dll)\0*.dll\0All files (*.*)\0*.*\0\0\0";
	
	resourceManager.GetString (langIndex, IDS_SEARCHINGINSTANCE, ofwTitle);

	SetPageToConfig (pageWnd);

	SetWindowPos (pageWnd, 0, 0, 73, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return true;
}


bool				TopPage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_SEARCH:
			{
				if (GetOpenFileName (&ofw))	{
					char	strBuffer1[MAXSTRINGLENGTH];
					char	strBuffer2[MAXSTRINGLENGTH];
					switch (WrapperInstances::GetConfigAndStatus (ofw.lpstrFile))	{
						case WrapperInstances::ConfigOK:
							{
								WrapperInstances::WrapperInstance instance;
								strcpy (instance.filePath, ofw.lpstrFile);
								if (!wrapperInstances.IsInList (ofw.lpstrFile)) {
									configSet = false;
									wrapperInstances.AddInstance (instance);
									RefreshInstanceList (pageWnd);
									SelectInstance (pageWnd, wrapperInstances.GetNumOfInstances () - 1);
								}
							}
							break;
						case WrapperInstances::ConfigError:
						case WrapperInstances::FileNotFound:
							{
								resourceManager.GetString (langIndex, IDS_INVALIDWRAPPERFILE2, strBuffer1);
								sprintf (strBuffer2, strBuffer1, DGVOODOOVERSION_STR);
								resourceManager.GetString (langIndex, IDS_ERRORDURINGCONFIGREAD, strBuffer1);
								MessageBox(hDlgMain, strBuffer2, strBuffer1, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
							}
							break;
						default:
						case WrapperInstances::ConfigInvalidVersion:
							{
								resourceManager.GetString (langIndex, IDS_INCOMPATIBELWRAPPERFILE, strBuffer1);
								resourceManager.GetString (langIndex, IDS_ERRORDURINGCONFIGREAD, strBuffer2);
								MessageBox(hDlgMain, strBuffer1, strBuffer2, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
							}
							break;
					}
				}
			}
			break;

		case IDC_ACTDIR:
			{
				char curPath[MAX_PATH];
				GetCurrentDirectory (MAX_PATH, curPath);
				if (wrapperInstances.SearchWrapperInstances (curPath)) {
					RefreshInstanceList (pageWnd);
					configSet = false;
					SelectInstance (pageWnd, wrapperInstances.GetNumOfInstances () - 1);
				}
			}
			break;

		case IDC_SYSDIR:
			{
				char curPath[MAX_PATH];
				GetSystemDirectory (curPath, MAX_PATH);
				if (wrapperInstances.SearchWrapperInstances (curPath)) {
					RefreshInstanceList (pageWnd);
					configSet = false;
					SelectInstance (pageWnd, wrapperInstances.GetNumOfInstances () - 1);
				}
			}
			break;

		case IDC_PWINDOWS:
			{
				if (platform != PWINDOWS) {
					platform = PWINDOWS;
					configs[PDOS] = config;
					::SetConfig (configs[platform]);
				}
			}
			break;

		case IDC_PDOS:
			{
				if (platform != PDOS) {
					platform = PDOS;
					configs[PWINDOWS] = config;
					::SetConfig (configs[platform]);
				}
			}
			break;

		default:
			return false;
	}
	return true;
}


bool				TopPage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_INSTANCELIST:
			{
				configSet = false;
				unsigned int i = SendMessage (GetDlgItem (pageWnd, IDC_INSTANCELIST), CB_GETCURSEL, 0, 0);
				i = wrapperInstances.GetNumOfInstances () - i - 1;
				SetConfigOfInstance (pageWnd, i);
			}
			break;
		
		case IDC_CBLANGUAGE:
			{
				unsigned int i = SendMessage (GetDlgItem (pageWnd, IDC_CBLANGUAGE), CB_GETCURSEL, 0, 0);
				mainDialog.SetLanguage (i);
			}
			break;
		
		default:
			return false;
	}
	return true;
}


void			TopPage::DisplayChanged ()
{
	GetDisplayMode ();
	RefreshDisplayMode (pageWnd[langIndex]);
}


TopPage::TopPage (MainDialog& mainDialog):
	Page			(mainDialog)
{
	configs[0] = defaultConfigs[0];
	configs[1] = defaultConfigs[1];

	char	tempPath[MAX_PATH];
	GetCurrentDirectory (MAX_PATH, tempPath);
	wrapperInstances.SearchWrapperInstances (tempPath);
	if (wrapperInstances.GetNumOfInstances () == 0)	{
		GetSystemDirectory (tempPath, MAX_PATH);
		wrapperInstances.SearchWrapperInstances (tempPath);
	}

	GetDisplayMode ();
}


HWND				TopPage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::HeaderPage, TopPageProc);
}


void				TopPage::SetPageToConfig (HWND pageWnd)
{
	RefreshDisplayMode (pageWnd);
	RefreshInstanceList (pageWnd);
	SelectInstance (pageWnd, currentInstance);

	SendMessage (GetDlgItem (pageWnd, IDC_CBLANGUAGE), CB_SETCURSEL, (WPARAM) config.language, (LPARAM) 0);
	SetPlatformState (pageWnd, config.GlideVersion == 0x211);
	unsigned int pWin = (platform == PWINDOWS) ? BST_CHECKED : BST_UNCHECKED;
	unsigned int pDos = (platform != PWINDOWS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_PWINDOWS), BM_SETCHECK, (LPARAM) pWin, (WPARAM) 0);
	SendMessage (GetDlgItem (pageWnd, IDC_PDOS), BM_SETCHECK, (LPARAM) pDos, (WPARAM) 0);	
}


bool				TopPage::FlushCurrentConfig ()
{
	configs[platform] = config;
	return wrapperInstances.WriteConfig (currentInstance, configs);
}


UI::Page*			CreateTopPage (MainDialog& mainDialog)
{
	return new TopPage (mainDialog);
}


bool				FlushCurrentConfig (UI::Page* topPage)
{
	return static_cast<TopPage*> (topPage)->FlushCurrentConfig ();
}

// ------------------------------------ Bottom Page ----------------------------------------

INT_PTR CALLBACK BottomPage::BottomPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}


bool			BottomPage::Init (HWND pageWnd)
{
	SetWindowPos (pageWnd, 0, 0, 546, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return true;
}


INT_PTR	CALLBACK BottomPage::AboutDialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)	{
		case WM_INITDIALOG:
			return(1);

		case WM_COMMAND:
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			switch(LOWORD(wParam))	{
				case ID_OK:
					EndDialog (hwndDlg, 1);
					return 1;
					break;
			}
	}
	return(0);
}


bool			BottomPage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_CANCELBUTTON:
			::Exit (false);
			break;
		
		case IDC_OKBUTTON:
			::Exit (true);
			break;

		case IDC_ABOUTBUTTON:
			{
				void* resData =  resourceManager.FindResourceData (langIndex, ResourceManager::Dialog, IDD_DIALOGABOUT);
				if (resData != NULL) {
					DialogBoxIndirect (hInstance, (LPCDLGTEMPLATE) resData, hDlgMain, AboutDialogProc);
				} else {
					DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOGABOUT), hDlgMain, AboutDialogProc);
				}
			}
			break;
		
		default:
			return false;
	}
	return true;
}


bool			BottomPage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	return false;
}


BottomPage::BottomPage (MainDialog& mainDialog):
	Page (mainDialog)
{
}


HWND			BottomPage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::BottomPage, BottomPageProc);
}


void			BottomPage::SetPageToConfig (HWND pageWnd)
{
}


UI::Page*		CreateBottomPage (MainDialog& mainDialog)
{
	return new BottomPage (mainDialog);
}

}	// namespace UI