/*--------------------------------------------------------------------------------- */
/* DosServerNt.c - Main server implementation for WinNT                             */
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
/* dgVoodoo: DosServerNt.c																	*/
/*			 NT alatti implementáció DOS szerveroldali része								*/
/*------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------*/
/*....................................... Includes .........................................*/

#include	"Resource.h"

#include    "Version.h"
#include	"dgVoodooConfig.h"
#include	"dgVoodooBase.h"
#include	<stdio.h>
#include	<windows.h>
#include	<winuser.h>

#include	"Main.h"
#include	"DDraw.h"
#include	"D3d.h"
#include	"movie.h"
#include	"Dos.h"
#include	"DosMouseNt.h"
#include	"Vesa.h"

#include	"debug.h"
#include	"Log.h"

#include	"DosCommon.h"

#ifndef GLIDE3

#ifdef	DOS_SUPPORT


/*------------------------------------------------------------------------------------------*/
/*...................................... Függvények ........................................*/


static	void CleanUp ()
{
	
	if (ntSharedMemHandle != NULL) CloseHandle(ntSharedMemHandle);
	DestroyServerCommandWindow ();
}


void SetRegInfoAreaNT(ServerRegInfo *regInfo)
{
	
	regInfo = (ServerRegInfo *) c;
	regInfo->areaId.areaId = AREAID_REGSTRUCTURE;
	regInfo->serveraddrspace = c;
	regInfo->hidWinHwnd = serverCmdHwnd;
}


int EXPORT DosNTEntryPoint()	{
DDSURFACEDESC2	temp;
DWORD			locktime, flashcount;
MSG				msg;
unsigned int	lfbSize;
unsigned char	errorTitle[MAXSTRINGLENGTH];
unsigned char	errorMsg[MAXSTRINGLENGTH];

	
	DEBUG_THREAD_REGISTER(DebugRenderThreadId);

	DEBUG_BEGIN("DosNTEntryPoint_NTRemoteRenderingThread", DebugRenderThreadId, 23);
	
	DEBUGLOG(0,"\nDosNTEntryPoint: Entering in server");
	DEBUGLOG(1,"\nDosNTEntryPoint: Kezdés a szerveren");
	
	platform = PLATFORM_DOSWINNT;
	ntSharedMemHandle = NULL;

	CopyMemory(&config, &pConfigsInHeader[1], sizeof(dgVoodooConfig));

	GetString (errorTitle, IDS_INITERRORTITLE);

	if (!GetActDispMode(NULL, &temp))	{
		GetString (errorMsg, IDS_NODIRECTXINSTALLED);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		return(0);
	}	
		
	c = NULL;
	if (config.Flags & CFG_NTVDDMODE)	{
		GetString (errorMsg, IDS_CANNOTRUNASSERVER);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		return(0);
	}
	
	if ((ntSharedMemHandle = OpenFileMapping(FILE_MAP_WRITE, FALSE, SharedMemName)) != NULL)	{
		GetString (errorMsg, IDS_SERVERISALREADYRUNNING);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		CleanUp ();
		return(0);
	}

	lfbSize = (config.Flags & CFG_VESAENABLE) ? (1024*768*3*4 >= config.VideoMemSize ? 1024*768*3*4 : config.VideoMemSize) : 1024*768*3*4;
	if ( (ntSharedMemHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
												sizeof(CommArea) + (1024*768*3*4),
												SharedMemName)) == NULL)	{
		
		GetString (errorMsg, IDS_CANNOTCREATEMEMORYMAPPING);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		CleanUp ();
		return(0);
	}
	if ( (c = (CommArea *) MapViewOfFile(ntSharedMemHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0)) == NULL)	{
		GetString (errorMsg, IDS_CANNOTCREATEMEMORYMAPPING);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);

		DEBUGLOG(0,"\n   Error: DosNTEntryPoint: Cannot create memory mapping for communication(2), error code: 0%x",GetLastError());
		DEBUGLOG(1,"\n   Hiba: DosNTEntryPoint: Nem tudom létrehozni a memórialeképezést a kommunikációhoz(2), hibakód: 0%x",GetLastError());

		CleanUp ();
		return(0);
	}
	CreateWarningBox();	

	if (config.Flags & CFG_WINDOWED) {
		if ( (temp.ddpfPixelFormat.dwRGBBitCount != 16) && (temp.ddpfPixelFormat.dwRGBBitCount != 32) )	{
			GetString (errorMsg, IDS_INCOMPATIBLEDESKTOPMODE);
			MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
			CleanUp ();
			return(0);
		}
	}

	ZeroMemory(&movie, sizeof(MOVIEDATA));
	RegisterMainClass();
	
	CreateServerCommandWindow ();

	SetRegInfoAreaNT((ServerRegInfo *) c);
	InitVESA();

	/* Get foreground locktimeout */
	SystemParametersInfo(0x2000,0,(void *) &foregroundLockTimeOut, SPIF_SENDCHANGE);
	/* Get foreground flashcount */
	SystemParametersInfo(0x2004,0,(void *) &foregroundFlashCount, SPIF_SENDCHANGE);	
	/* Set foreground locktimeout */	
	SystemParametersInfo(0x2001,0,0, SPIF_SENDCHANGE);
	/* Set foreground flashcount */
	SystemParametersInfo(0x2005,0,0, SPIF_SENDCHANGE);	

	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	DEBUGLOG(0,"\nDosNTEntryPoint: waiting for a client to be served");
	DEBUGLOG(1,"\nDosNTEntryPoint: várakozás egy kiszolgálandó kliensre");
	
	while(1)	{
		if (!GetMessage(&msg,NULL,0,0)) break;		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	DEBUGLOG(0,"\nDosNTEntryPoint: shutting down, application is closed by user");
	DEBUGLOG(1,"\nDosNTEntryPoint: kiszolgáló kilövése, az alkalmazást a felhasználó bezárta");

	CleanUp ();

	/* Get foreground locktimeout */
	SystemParametersInfo(0x2000, 0, &locktime, SPIF_SENDCHANGE);
	/* Get foreground flashcount */
	SystemParametersInfo(0x2004, 0, &flashcount, SPIF_SENDCHANGE);	
	
	/* Set foreground locktimeout */	
	if (locktime == 0) SystemParametersInfo(0x2001, 0, (void *) foregroundLockTimeOut, SPIF_SENDCHANGE);
	/* Set foreground flashcount */
	if (flashcount == 0) SystemParametersInfo(0x2005, 0, (void *) foregroundFlashCount, SPIF_SENDCHANGE);

	DEBUGLOG(0,"\nDosNTEntryPoint: Leaving successfully in server");
	DEBUGLOG(1,"\nDosNTEntryPoint: Vége a szerveren");
	return(1);
	
	DEBUG_END("DosNTEntryPoint_NTRemoteRenderingThread", DebugRenderThreadId, 23);
}

#endif	/* DOS_SUPPORT */

#endif	/* GLIDE3 */