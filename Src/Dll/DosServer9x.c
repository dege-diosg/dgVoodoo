/*--------------------------------------------------------------------------------- */
/* DosServer9x.c - Main server implementation for Win9x                             */
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
/* dgVoodoo: DosServer9x.c																	*/
/*			 9x/Me alatti implementáció szerver része										*/
/*------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------*/
/*....................................... Includes .........................................*/

#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"
#include    "Version.h"
#include	<stdio.h>
#include	<windows.h>
#include	<winuser.h>
#include	"Main.h"
#include	"Dos.h"
#include	"VxdComm.h"
#include	"DosMouseNt.h"
#include	"Vesa.h"
#include	"dgVoodooGlide.h"

#include	"debug.h"
#include	"Log.h"

#include	"DosCommon.h"
#include	"Resource.h"

#ifndef GLIDE3

#ifdef	DOS_SUPPORT

/*------------------------------------------------------------------------------------------*/
/*...................................... Globálisok ........................................*/


/*static */ HANDLE	hDevice;					/* a DGVOODOO.VXD device handle-je */
static	HANDLE		workerThreadHandle;			/* A kommunikációs (dolgozó) szál handle-je */
static	DWORD		workerThreadID;				/* A kommunikációs (dolgozó) szál id-je */
static	int			workerExit = 0;				/* 1=a kommunikációs szál terminál */
static  int			workerResumed = 0;			/* 1=a kommunikációs szál kreálás óta felébresztve */


/*------------------------------------------------------------------------------------------*/
/*...................................... Függvények ........................................*/



void BlockVirtualMachine()
{

	DeviceIoControl(hDevice,DGSERVER_SUSPENDVM,NULL,0,NULL,0,NULL,NULL);
	if (VESAFreshThreadHandle) SuspendThread(VESAFreshThreadHandle);

}


void UnBlockVirtualMachine()
{

	if (VESAFreshThreadHandle) ResumeThread(VESAFreshThreadHandle);
	DeviceIoControl(hDevice,DGSERVER_RESUMEVM,NULL,0,NULL,0,NULL,NULL);

}



DWORD WINAPI WorkerThread(LPVOID lpParameter)
{
	if (!workerExit)	{
		DeviceIoControl(hDevice, DGSERVER_BLOCKWORKERTHREAD, NULL, 0, NULL, 0, NULL, NULL);
		while (!workerExit)	{
			if (c != NULL)
				SendMessage(serverCmdHwnd, DGSM_PROCEXECBUFF, 0, 0);
			DeviceIoControl(hDevice, DGSERVER_RETURNVM, NULL, 0, NULL, 0, NULL, NULL);
		}
	}
	return (0);
}


void CleanUp ()
{
	
	workerExit = 1;
	if (!workerResumed) {
		ResumeThread (workerThreadHandle);
	} else {
		DeviceIoControl (hDevice, DGSERVER_WAKEUPWORKERTHREAD, NULL, 0, NULL, 0, NULL, NULL);
	}
	WaitForSingleObject (workerThreadHandle, INFINITE);
	CloseHandle (workerThreadHandle);
	DestroyServerCommandWindow ();
}


int EXPORT Dos9xEntryPoint()
{
int				currDispModeBitDepth;
VxdRegInfo		regInfo;
MSG				msg;
DWORD			locktime, flashcount;
unsigned char	errorTitle[MAXSTRINGLENGTH];
unsigned char	errorMsg[MAXSTRINGLENGTH];

	
	CopyMemory(&config, &pConfigsInHeader[1], sizeof(dgVoodooConfig));
	config.Flags |= CFG_NTVDDMODE;
	
	platform = PLATFORM_DOSWIN9X;
	
	ReCreateRenderer ();

	GetString (errorTitle, IDS_INITERRORTITLE);

	if (!IsRendererApiAvailable ()) {
		GetString (errorMsg, IDS_NODIRECTXINSTALLED);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		return(0);
	}
		
	c = NULL;	
	hDevice = CreateFile("\\\\.\\dgvoodoo.vxd", 0, 0, NULL, 0, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	//CloseHandle(hDevice);
	if (hDevice == INVALID_HANDLE_VALUE)	{
		GetString (errorMsg, IDS_CANNOTLOADVXD);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		return(0);
	}		

	CreateWarningBox ();

	RegisterMainClass ();

	CreateServerCommandWindow ();
	
	workerThreadHandle = CreateThread (NULL, 32768, WorkerThread, 0, CREATE_SUSPENDED, &workerThreadID);
	workerResumed = 0;
	
	regInfo.workerThread = workerThreadHandle;
	regInfo.commAreaPtr = &c;
	regInfo.videoMemSize = (config.Flags & CFG_VESAENABLE) ? config.VideoMemSize * 1024 : 0;
	regInfo.configPtr = &config;

	if (config.Flags & CFG_WINDOWED) {
		currDispModeBitDepth = GetCurrentDisplayModeBitDepth (config.dispdev, config.dispdriver);
		if ((currDispModeBitDepth != 16) && (currDispModeBitDepth != 32) )	{
			GetString (errorMsg, IDS_INCOMPATIBLEDESKTOPMODE);
			MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
			CleanUp();
			CloseHandle(hDevice);
			return(0);
		}
	}

	if (!DeviceIoControl(hDevice, DGSERVER_REGSERVER, &regInfo, 0, NULL, 0, NULL, NULL)) return(0);
	if (regInfo.errorCode == DGSERVERR_ALREADYREGISTERED)	{
		GetString (errorMsg, IDS_SERVERISALREADYRUNNING);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		CleanUp();
		CloseHandle(hDevice);
		return(0);
	}
	if (regInfo.errorCode == DGSERVERR_INITVESAFAILED)	{
		GetString (errorMsg, IDS_VESAINITFAILED);
		MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP);
		CleanUp();
		CloseHandle(hDevice);
		return(0);
	}

	ResumeThread (workerThreadHandle);
	workerResumed = 1;

	InitVESA();

	/* Get foreground locktimeout */
	SystemParametersInfo(0x2000, 0, (void *) &foregroundLockTimeOut, SPIF_SENDCHANGE);
	/* Get foreground flashcount */
	SystemParametersInfo(0x2004, 0, (void *) &foregroundFlashCount, SPIF_SENDCHANGE);
	/* Set foreground locktimeout */	
	SystemParametersInfo(0x2001, 0, 0, SPIF_SENDCHANGE);
	/* Set foreground flashcount */
	SystemParametersInfo(0x2005, 0, 0, SPIF_SENDCHANGE);	
	
	while(1)	{
		if (!GetMessage(&msg,NULL,0,0)) break;	
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}
	
	CleanupVESA();	
	CleanUp();
	DeviceIoControl(hDevice, DGSERVER_UNREGSERVER, NULL, 0, NULL, 0, NULL, NULL);
	CloseHandle(hDevice);

	/* Get foreground locktimeout */
	SystemParametersInfo(0x2000, 0, &locktime, SPIF_SENDCHANGE);
	/* Get foreground flashcount */
	SystemParametersInfo(0x2004, 0, &flashcount, SPIF_SENDCHANGE);	
	
	/* Set foreground locktimeout */	
	if (locktime == 0) SystemParametersInfo(0x2001, 0, (void *) foregroundLockTimeOut, SPIF_SENDCHANGE);
	/* Set foreground flashcount */
	if (flashcount == 0) SystemParametersInfo(0x2005, 0, (void *) foregroundFlashCount, SPIF_SENDCHANGE);

	return(1);
}

#endif	/* DOS_SUPPORT */

#endif	/* GLIDE3 */