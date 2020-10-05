/*--------------------------------------------------------------------------------- */
/* Main.cpp - Glide implementation, DLL Main                                        */
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
/* dgVoodoo: Main.c																			*/
/*			 DLL belépési pontja és a fõ globális változók									*/
/*------------------------------------------------------------------------------------------*/

#include "dgVoodooBase.h"

#include <windows.h>
#include <string.h>

extern "C" {

#include "Version.h"
#include "Main.h"
#include "Debug.h"
#include "Dos.h"
#include "DosCommon.h"

#include "Resource.h"

}

#include "Renderer.hpp"

/*------------------------------------------------------------------------------------------*/
/*..................................... Globálisok .........................................*/

extern "C" {

HINSTANCE		hInstance;
/*unsigned*/ char	moduleName[256];
int				platform;
unsigned int	instructionSet = MMX_SUPPORTED | SSE_SUPPORTED | SSE2_SUPPORTED;
dgVoodooConfig	config;
dgVoodooConfig	*pConfigsInHeader;
char			productName[] = PRODUCTNAME;				/* A termék neve */
MovieData		movie;
int				DebugRenderThreadId;
char			myDeepThoughts[] = "Gyurcsány csótány te pápai zsidótakony, bazd meg a redvás anyád, aztán három réteg föld rád az összes patkány haveroddal együtt!!! Irtsuk a liberóbuzikat is, de ne naranccsal, hanem késsel-villával, lapáttal, lángszóróval, láncfûrésszel!! A komcsilibsi-féregtõl csak úgy lehet megszabadulni, ha péppé taposod a húgyagyát!!!";

}

/*------------------------------------------------------------------------------------------*/
/*................................... Belépési pontok ......................................*/

/* DLL belépési pont: windows config struktúra átmásolása,  return: init sikerült */
int _stdcall DllMain(HINSTANCE hDLL, DWORD fdwReason, LPVOID /*lpvReserved*/)	{
int	i;

	if (fdwReason == DLL_PROCESS_ATTACH)	{
		platform = PLATFORM_WINDOWS;
		hInstance = hDLL;
		
		pConfigsInHeader = (dgVoodooConfig *) (((unsigned char *) hDLL) + 0x040);
		CopyMemory(&config, pConfigsInHeader + 0, sizeof(dgVoodooConfig));

		GetModuleFileName(NULL, moduleName, 256);
		i = strlen(moduleName);
		while( (i != 0) && (moduleName[i] != ':') && (moduleName[i] != '\\') ) i--;
		moduleName[1] = 0;
		strcat(moduleName, moduleName + i + 1);

		DEBUG_SETLANGUAGE(config.language == Hungarian ? DEBUG_LANG_HUNGARIAN : DEBUG_LANG_ENGLISH);
		
		DEBUG_SETMODULEINSTANCE(hDLL);
		
		DEBUG_THREAD_REGISTER(DebugRenderThreadId);

		renderer.CreateRenderer ();

	} else if (fdwReason == DLL_PROCESS_DETACH) {

#ifdef	DOS_SUPPORT

		ExitDosNt ();

#endif

		renderer.DestroyRenderer ();
	}
	return(TRUE);
}


/* EXE belépési pont: csak debug céllal, egyébként nem használt */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*cmdLine*/, int /*nCmdShow*/)	{

	DllMain(hInstance, DLL_PROCESS_ATTACH, 0);	
	//DosNTEntryPoint();
	//Dos9xEntryPoint();
	return (1);
}


void	ReCreateRenderer ()
{
	renderer.DestroyRenderer ();
	renderer.CreateRenderer ();
}


void	GetString (LPSTR lpBuffer, UINT uID)
{
	if (config.language == Hungarian) {
		uID = uID - IDS_BASE + IDS_BASE_HUN;
	}
	LoadString (hInstance, uID, lpBuffer, MAXSTRINGLENGTH);
	GetLastError ();
}


void	CrashCallback ()
{
	//SetWindowPos (movie.cmiWinHandle, HWND_NOTOPMOST, 0, 0, 0, 0, /*SWP_ASYNCWINDOWPOS | SWP_HIDEWINDOW |*/ SWP_NOMOVE | /*SWP_NOREDRAW |*/ SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_NOSENDCHANGING);
	//ShowWindow (movie.cmiWinHandle, SW_FORCEMINIMIZE);
	//SetWindowLong (movie.cmiWinHandle, GWL_STYLE, 0);
}