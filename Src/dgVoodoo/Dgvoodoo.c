/*--------------------------------------------------------------------------------- */
/* dgVoodoo.c - dgVoodoo Server Proc implementation                                 */
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

#include <windows.h>
#include <winuser.h>
#include "resource.h"

LRESULT CALLBACK dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WNDCLASS dlgClass = {0, dlgWindowProc, 0, DLGWINDOWEXTRA, 0, NULL, NULL, COLOR_WINDOW, NULL, "DGCLASS"};//DGVOODOODLGCLASS"};


LRESULT CALLBACK dlgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
	
	switch (uMsg) {
		case WM_CLOSE:
			EndDialog(hwnd,2);
			return(0);

		default:
			return(DefDlgProc(hwnd,uMsg,wParam,lParam));
	}
}


int CALLBACK dgVoodooDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	switch(uMsg)	{
		
		case WM_INITDIALOG:
			return(1);
			break;

		case WM_COMMAND:			
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			EndDialog(hwndDlg,1);
			switch(LOWORD(wParam))	{

				case ID_GLIDE211:
					EndDialog(hwndDlg,1);
					break;

				case ID_GLIDE243:
					EndDialog(hwndDlg,0);
					break;
			}
			return(1);
		
		default:
			return(0);

	}	
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)	{
HINSTANCE		hDllInstance211, hDllInstance243, hDllInstance;
FARPROC			entryPoint;
OSVERSIONINFO	versioninfo;
int				error;
char			fn[256];

	hDllInstance211 = LoadLibrary("glide.dll");
	hDllInstance243 = LoadLibrary("glide2x.dll");

	if ( (hDllInstance211 == NULL) && (hDllInstance243 == NULL) )	{

		MessageBox(NULL,"Cannot find GLIDE.DLL or GLIDE2X.DLL!","Error during loading wrapper file",MB_OK | MB_ICONSTOP);
		return(1);
		
	}

	GetModuleFileName(hDllInstance211, fn, 256);
	GetModuleFileName(hDllInstance243, fn, 256);

	hDllInstance = hDllInstance243;
	FreeLibrary(hDllInstance211);

	dlgClass.hInstance = hInstance;
	dlgClass.hCursor = LoadCursor(0, IDC_ARROW);
	dlgClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClass(&dlgClass);

	/*error = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,dgVoodooDialogProc,0);
	GetLastError ();*/
	
	/*if ( (hDllInstance211 != NULL) && (hDllInstance243 != NULL) )	{

		dlgClass.hInstance = hInstance;
		dlgClass.hCursor = LoadCursor(0, IDC_ARROW);
		dlgClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		RegisterClass(&dlgClass);
		error = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,dgVoodooDialogProc,0);
		switch(error)	{
			case -1:
				MessageBox(NULL, "Internal Win32 error!", "Error during loading wrapper file", MB_OK | MB_ICONSTOP);
				FreeLibrary(hDllInstance211);
				FreeLibrary(hDllInstance243);
				return(1);
				break;
			case 0:
				hDllInstance = hDllInstance243;
				FreeLibrary(hDllInstance211);
				break;
			case 1:
				hDllInstance = hDllInstance211;
				FreeLibrary(hDllInstance243);
				break;
			case 2:
				return(1);
		}
	} else hDllInstance = (hDllInstance211 != NULL) ? hDllInstance211 : hDllInstance243;*/

	error = 0;
	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&versioninfo);
	if (versioninfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		entryPoint = GetProcAddress(hDllInstance, "_Dos9xEntryPoint@0");
	} else if (versioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		entryPoint = GetProcAddress(hDllInstance, "_DosNTEntryPoint@0");
	} else if (versioninfo.dwPlatformId == VER_PLATFORM_WIN32s)	{

		MessageBox(NULL,"dgVoodoo is not supported under Win32s! You need Win95 or greater!",
						"Fatal error",MB_OK | MB_ICONSTOP);
		error = 1;
	}

	if (entryPoint == NULL)	{
		
		MessageBox(NULL,"Cannot find entrypoint in GLIDE2X.DLL! This is not the dgVoodoo wrapper file!",
					"Error during loading wrapper file",MB_OK | MB_ICONSTOP);
		error = 1;
	}

	if (!error) {
		(*entryPoint)();
	}


	FreeLibrary(hDllInstance);
	return(1);
}