/*--------------------------------------------------------------------------------- */
/* DosClientNT.c - Glide VDD, client side (NTVDM) part for WinNT                    */
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
/* dgVoodoo: DosClientNt.c																	*/
/*			 NT alatti implementáció DOS kliensoldali (NTVDM) része							*/
/*			 VDD-módban a kliens lokális szerverként mûködik								*/
/*------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------*/
/*....................................... Includes .........................................*/


#include    "Version.h"
#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"
#include	"Dgw.h"
#include	<stdio.h>
#include	<windows.h>
#include	<winuser.h>
#include	"Main.h"
#include	"Dos.h"
#include	"Vesa.h"
#include	"DosMouseNt.h"

#include	"debug.h"
#include	"Log.h"

#include	"DosCommon.h"

#include	"Resource.h"


#ifndef GLIDE3

#ifdef	DOS_SUPPORT


static void _stdcall NTVDM_Kill_Callback(unsigned short DosPDB);

/*------------------------------------------------------------------------------------------*/
/*...................................... Globálisok ........................................*/


static	unsigned int	ntVdmInitOk = 0;
static	HANDLE			ntDosThreadHandle;

static	HANDLE			ntTimerLoopHandle;
static	DWORD			ntTimerBoostingLoopID;
static	int				timerLoopExit;

static	HANDLE			vddRenderingThreadHandle;	/* VDD-mód: a dolgozó szál handle-je */
static	DWORD			vddRenderingThreadId;		/* VDD-mód: a dolgozó szál id-je */
static	int				ntVDDRenderingRefCount;
static	int				serverOpeningRefCount;
static	ServerRegInfo	regInfo;
static	HANDLE			localClientThreadHandle;
static	DWORD			localClientThreadId;
static	int				debugLocalClientThreadId;

/*static*/	HANDLE		ntSharedMemHandle;			/* szerver-mód: az osztott memterület handle-je */
static	HANDLE			drawMutex;

static	int				DebugDosThreadId;

/* Az NTVDM-bõl importált függvények címei */
static	void			(_stdcall *_setEAX)(unsigned int);
static	void			(_stdcall *_setEBX)(unsigned int);
static	void			(_stdcall *_setECX)(unsigned int);
static	void			(_stdcall *_setEDX)(unsigned int);
static	void			(_stdcall *_setESI)(unsigned int);
static	void			(_stdcall *_setEDI)(unsigned int);
static	void			(_stdcall *_setEBP)(unsigned int);
static	void			(_stdcall *_setES)(unsigned int);
static	void			(_stdcall *_setCF) (unsigned int);
/*static*/	void			(_stdcall *_call_ica_hw_interrupt)(int ms, int line, int count);
static	void*			(_stdcall *_vdmMapFlat)(unsigned short selector, unsigned long offset, VDM_MODE mode);
static	BOOL			(_stdcall *_VDDInstallIOHook)(HANDLE hVDD, WORD cPortRange, PVDD_IO_PORTRANGE pPortRange, PVDD_IO_HANDLERS ioHandler);
static	void			(_stdcall *_VDDDeInstallIOHook)(HANDLE hVDD, WORD cPortRange, PVDD_IO_PORTRANGE pPortRange);
static	int				(_stdcall *_VDDInstallUserHook)(HANDLE, PFNVDD_UCREATE, PFNVDD_UTERMINATE, PFNVDD_UBLOCK, PFNVDD_URESUME);
static	int				(_stdcall *_VDDDeInstallUserHook)(HANDLE);

static	unsigned long	(_stdcall *_getEAX)();
static	unsigned long	(_stdcall *_getEBX)();
static	unsigned long	(_stdcall *_getECX)();
static	unsigned long	(_stdcall *_getEDX)();
static	unsigned long	(_stdcall *_getESI)();
static	unsigned long	(_stdcall *_getEDI)();
static	unsigned long	(_stdcall *_getEIP)();
static	unsigned short	(_stdcall *_getES)();
static	unsigned short	(_stdcall *_getCS)();
static	unsigned long	(_stdcall *_getEFLAGS)();

/*------------------------------------------------------------------------------------------*/
/*.................................. Predeklaráció .........................................*/

static DWORD WINAPI TimerBoostingLoop(LPVOID lpParameter);


unsigned char* DosGetSharedLFB()	{

	return(c->LFB);
}


void*	DosMapFlat (unsigned short selector, unsigned int offset)
{
	return ((*_vdmMapFlat)(selector, offset, VDM_PM) );
}


/*------------------------------------------------------------------------------------------*/
/*................................ Timer Boosting szál .....................................*/


static	void	CreateTimerBoostingThread ()
{
	timerLoopExit = 0;
	ntTimerLoopHandle = (config.dosTimerBoostPeriod != 0) ? CreateThread(NULL, 8192, TimerBoostingLoop, 0, 0, &ntTimerBoostingLoopID) : NULL;
}


static	void	DestroyTimerBoostingThread ()
{
	timerLoopExit = 1;
	if (ntTimerLoopHandle != NULL) {
		WaitForSingleObject(ntTimerLoopHandle, INFINITE);
		CloseHandle (ntTimerLoopHandle);
		ntTimerLoopHandle = NULL;
	}
}


static DWORD WINAPI TimerBoostingLoop(LPVOID lpParameter)	{
DWORD	prevTick, tickSum;
__int64	highResTimerFreq;
__int64 highResTimerTickSum;
__int64 highResTimerTickPrev;
__int64 highResTimerTickNow;
int		highResTimerUsed;
int		numberOfDosTimerTicks;

	SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	tickSum = 0;
	highResTimerFreq = highResTimerTickSum = 0;
	QueryPerformanceFrequency ((LARGE_INTEGER *) &highResTimerFreq);
	highResTimerFreq = (highResTimerFreq * config.dosTimerBoostPeriod) / 1000;
	highResTimerUsed = highResTimerFreq != 0;

	while(!timerLoopExit)	{
		if (highResTimerUsed) {
			QueryPerformanceCounter ((LARGE_INTEGER *) &highResTimerTickPrev);
			Sleep(1);
			QueryPerformanceCounter ((LARGE_INTEGER *) &highResTimerTickNow);
			highResTimerTickSum += highResTimerTickNow - highResTimerTickPrev;
			numberOfDosTimerTicks = (int) (highResTimerTickSum / highResTimerFreq);
			if (numberOfDosTimerTicks) {
				(*_call_ica_hw_interrupt) (ICA_MASTER, 0, numberOfDosTimerTicks);
				highResTimerTickSum %= highResTimerFreq;
			}
		} else {
			prevTick = GetTickCount();
			Sleep(1);
			tickSum += GetTickCount () - prevTick;
			numberOfDosTimerTicks = tickSum / config.dosTimerBoostPeriod;
			if (numberOfDosTimerTicks) {
				(*_call_ica_hw_interrupt) (ICA_MASTER, 0, numberOfDosTimerTicks);
				tickSum %= config.dosTimerBoostPeriod;
			}
		}
	}
	return (0);
}

/*------------------------------------------------------------------------------------------*/
/*................................ Kliensnek szóló üzenetek ................................*/


LRESULT CALLBACK LocalWindowCommandProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (hWnd == clientCmdHwnd) {
		
		switch(uMsg)	{
			case WM_CREATE:
				return(0);

			case WM_CLOSE:
				PostQuitMessage (0);
				return (0);

			case WM_DESTROY:
				return(0);

			case DGCM_SUSPENDDOSTHREAD:
				if (c->kernelflag & KFLAG_DRAWMUTEXUSED) {
					c->kernelflag |= KFLAG_SUSPENDDOSTHREAD;
				} else {
					SuspendThread (ntDosThreadHandle);
				}
				return (0);

			case DGCM_RESUMEDOSTHREAD:
				ResumeThread (ntDosThreadHandle);
				return (0);
			
			default:
				if (NTLocalMouseWinHandler(hWnd, uMsg, wParam, lParam) == 0)
					return(0);
				
				if (VESALocalClientCommandHandler(hWnd, uMsg, wParam, lParam) == 0)
					return(0);
		}
	}
	
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}


void	CreateClientCommandWindow ()
{
	clientCmdHwnd = CreateWindowEx (0, "DGVOODOOLOCALCMDCLASS", "", WS_OVERLAPPEDWINDOW,
								    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
								    hInstance, NULL);
}


void	DestroyClientCommandWindow ()
{
	DestroyWindow (clientCmdHwnd);
}
/*------------------------------------------------------------------------------------------*/
/*................................ NT-VDD Init és dispatch .................................*/


/* InitDosNT: Init függvény, ami akkor fut le, ha a DLL-t az NTVDM-hez csatolják */
void __declspec(dllexport) __cdecl InitDosNT()	{
HINSTANCE	hInst;
HWND		(_stdcall *GetConsoleWindow)();
void		(_stdcall *NtVdmControl)(int,int);
WNDCLASS	localCmdWinClass = {0, NULL, 0, 0, 0, NULL, NULL, NULL, NULL, "DGVOODOOLOCALCMDCLASS"};

	DEBUG_THREAD_REGISTER(DebugDosThreadId);

	DEBUG_BEGIN("dos_thread->InitDosNT (client)", DebugDosThreadId, 24);

	DEBUGLOG(0, "\nInitDosNT: Entering");
	DEBUGLOG(1, "\nInitDosNT: Kezdés");

	if (ntVdmInitOk) {
		DEBUGLOG(0, "\n   Remark: DLL is already attached to NTVDM, probably due to VESA support...");
		DEBUGLOG(1, "\n   Megjegyzés: a DLL már csatolva van az NTVDM-hez, valószínûleg a VESA-támogatás miatt...");
		return;
	}
	
	platform = PLATFORM_DOSWINNT;
	ntVdmInitOk	= 0;
	ntVDDRenderingRefCount = serverOpeningRefCount = 0;

	CopyMemory(&config, &pConfigsInHeader[1], sizeof(dgVoodooConfig));
	
	/* NTDLL */
	hInst = GetModuleHandle("ntdll.dll");
	if (hInst)	{
		NtVdmControl = (void (_stdcall *)(int,int)) GetProcAddress(hInst,"NtVdmControl");
		if ( (config.Flags2 & CFG_FORCECLIPOPF) && NtVdmControl) (*NtVdmControl)(0x0d,1);
	}
	
	/* NTVDM */
	hInst = GetModuleHandle("ntvdm.exe");
	
	DEBUGCHECK (0, hInst == NULL, "\n   Error: InitDosNT: Cannot get module handle of ntvdm.exe");
	DEBUGCHECK (1, hInst == NULL, "\n   Hiba: InitDosNT: Az ntvdm.exe module handle-jét nem tudom lekérdezni");

	_setEAX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setEAX");
	_setEBX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setEBX");
	_setECX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setECX");
	_setEDX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setEDX");
	_setESI = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setESI");
	_setEDI = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setEDI");
	_setEBP = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setEBP");
	_setES = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setES");
	_setCF = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst, "setCF");
	
	_getEAX = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEAX");
	_getEBX = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEBX");
	_getECX = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getECX");
	_getEDX = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEDX");
	_getESI = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getESI");
	_getEDI = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEDI");
	_getES = (unsigned short (_stdcall *)() ) GetProcAddress(hInst, "getES");
	_getEIP = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEIP");
	_getCS = (unsigned short (_stdcall *)() ) GetProcAddress(hInst, "getCS");
	_getEFLAGS = (unsigned long (_stdcall *)() ) GetProcAddress(hInst, "getEFLAGS");
	
	_call_ica_hw_interrupt = (void (_stdcall *)(int,int,int)) GetProcAddress(hInst, "call_ica_hw_interrupt");
	_vdmMapFlat = (void* (_stdcall *)(unsigned short, unsigned long, VDM_MODE)) GetProcAddress(hInst, "VdmMapFlat");
	_VDDInstallIOHook =		(BOOL (_stdcall *)(HANDLE, WORD, PVDD_IO_PORTRANGE, PVDD_IO_HANDLERS)) GetProcAddress(hInst, "VDDInstallIOHook");
	_VDDDeInstallIOHook = (void (_stdcall *)(HANDLE, WORD, PVDD_IO_PORTRANGE)) GetProcAddress(hInst, "VDDDeInstallIOHook");
	_VDDInstallUserHook = (int (_stdcall *)(HANDLE, PFNVDD_UCREATE, PFNVDD_UTERMINATE, PFNVDD_UBLOCK, PFNVDD_URESUME)) GetProcAddress(hInst, "VDDInstallUserHook");
	_VDDDeInstallUserHook = (int (_stdcall *)(HANDLE)) GetProcAddress(hInst, "VDDDeInstallUserHook");

	(*_setCF)(1);

	if ( (_setEAX == NULL) || (_setEBX == NULL) || (_setECX == NULL) || (_setEDX == NULL) ||
		 (_setESI == NULL) || (_setEDI == NULL) || (_setEBP == NULL) || (_setCF == NULL) ||
		 (_getEAX == NULL) || (_getEBX == NULL) || (_getECX == NULL) || (_getEDX == NULL) ||
		 (_getESI == NULL) || (_getEDI == NULL) || (_getES == NULL) || (_getEIP == NULL) ||
		 (_getCS == NULL) || (_getEFLAGS == NULL) || (_setES == NULL) ||
		 (_call_ica_hw_interrupt == NULL) || (_vdmMapFlat == NULL) || (_VDDInstallIOHook == NULL) ||
		 (_VDDDeInstallIOHook == NULL) || (_VDDInstallUserHook == NULL) || (_VDDDeInstallUserHook == NULL) ) {

		DEBUGLOG(0, "\n   Fatal error: Cannot get address(es) of needed NTVDM function(s)");
		DEBUGLOG(1, "\n   Végzetes hiba: Nem tudom lekérdezni a szükséges NTVDM-függvény(ek) címét/címeit");
		return;
	}

	/* Console window */
	DEBUGLOG(0, "\nInitDosNT: NT mode: %s",(config.Flags & CFG_NTVDDMODE) ? "VDD" : "Server");
	DEBUGLOG(1, "\nInitDosNT: NT-mód: %s",(config.Flags & CFG_NTVDDMODE) ? "VDD" : "Szerver");
	
	if ( (GetConsoleWindow = (HWND (_stdcall *)()) GetProcAddress(GetModuleHandle("kernel32.dll"), "GetConsoleWindow")) == NULL)	{
		DEBUGLOG(0, "\n   Fatal error: Cannot get address of function GetConsoleWindow");
		DEBUGLOG(1, "\n   Végzetes hiba: Nem tudom lekérdezni a GetConsoleWindow függvény címét");
		return;
	}
	if ( (consoleHwnd = (*GetConsoleWindow)()) == NULL)	{
		DEBUGLOG(0, "\n   Fatal error: Cannot get handle of the console window");
		DEBUGLOG(1, "\n   Végzetes hiba: Nem tudom lekérdezni a konzolablak kezelõjét");
		return;
	}
	
	RegisterMainClass();

	localCmdWinClass.lpfnWndProc = LocalWindowCommandProc;
	localCmdWinClass.hInstance = hInstance;
	RegisterClass(&localCmdWinClass);

	if (!DuplicateHandle (GetCurrentProcess (), GetCurrentThread (), GetCurrentProcess (), &ntDosThreadHandle,
						  0, FALSE, DUPLICATE_SAME_ACCESS)) {
		ntDosThreadHandle = NULL;
	}
	
	(*_setCF)(0);
	ntVdmInitOk = 1;

	DEBUGLOG(0, "\nInitDosNT: Leaving successfully");
	DEBUGLOG(1, "\nInitDosNT: Sikeres befejezés");
	
	DEBUG_END("dos_thread->InitDosNT (client)", DebugDosThreadId, 24);
}


void	ExitDosNt ()
{
	
	
	if (platform == PLATFORM_DOSWINNT) {
		if (ntDosThreadHandle != NULL)
			CloseHandle (ntDosThreadHandle);
		ntDosThreadHandle = NULL;
		
		// Shouldn't happen to be called, unless in NTVDM kill
		if (serverOpeningRefCount) {
			serverOpeningRefCount = 1;
			CloseServerCommunicationArea ();
		}
	}

	DEBUGLOG(0, "\nExitDosNT: Successful\n");
	DEBUGLOG(1, "\nExitDosNT: Sikeres\n");

}


/* DispatchDosNT: a CH-ban megadott szolgáltatás meghívása */
void __declspec(dllexport) __cdecl DispatchDosNT()	{
VESARegisters	vesaRegisters;
int				errorCode;
unsigned long	params[16];
unsigned char	errorTitle[MAXSTRINGLENGTH];
unsigned char	errorMsg[MAXSTRINGLENGTH];
			
	switch( ((*_getECX)() >> 8) & 0xFF )	{

		/* Glide init, bejelentkezés (szerver lefoglalása) */
		case DGCLIENT_BEGINGLIDE:
			DEBUG_BEGIN("dos_thread->DispatchDosNT(BG)", DebugDosThreadId, 25);
			
			(*_setCF)(1);
			(*_setEAX)(0);		// Shared memory pool kezdõcíme
			if (!ntVdmInitOk) return;

			CreateTimerBoostingThread ();
			
			if (config.Flags & CFG_NTVDDMODE)	{
				if (InitVDDRendering ())	{
					DestroyTimerBoostingThread ();
					DEBUGLOG(0,"\n   Error: DGCLIENT_BEGINGLIDE has failed: initializing local rendering thread has failed");
					DEBUGLOG(1,"\n   Hiba: DGCLIENT_BEGINGLIDE nem sikerült: a helyi, megjelenítést végzõ szál létrehozása nem sikerült");
					return;
				} else {
					if ( (drawMutex = OpenMutex(SYNCHRONIZE, FALSE, MutexEventName)) == NULL) 
						drawMutex = CreateMutex(NULL, FALSE, MutexEventName);
					(*_setECX)((unsigned int) c);
					(*_setCF)(0);
				}
			} else {
				if ((errorCode = OpenServerCommunicationArea (&regInfo)) == 0) {
					(*_setECX)((unsigned int) regInfo.serveraddrspace);	
				} else {
					DestroyTimerBoostingThread ();

					DEBUGLOG(0,"\n   Error: DGCLIENT_BEGINGLIDE has failed: opening connection to server has failed");
					DEBUGLOG(1,"\n   Hiba: DGCLIENT_BEGINGLIDE nem sikerült: a kapcsolatfelvétel a szerverrel nem sikerült");
					
					GetString (errorTitle, IDS_OPENSERVERTITLE);
					GetString (errorMsg, IDS_OPENSERVERFAILED);
					
					MessageBox(NULL, errorMsg, errorTitle, MB_OK | MB_ICONSTOP | MB_APPLMODAL);
					return;
				}
			}
			(*_VDDInstallUserHook)(hInstance, NULL, (PFNVDD_UTERMINATE) NTVDM_Kill_Callback, NULL, NULL);
			
			(*_setEDX)( (unsigned int) c);
			(*_setCF)(0);
			return;
			
			DEBUG_END("dos_thread->DispatchDosNT(BG)", DebugDosThreadId, 25);

		/* Glide vége, kijelentkezés (szerver elengedése) */
		case DGCLIENT_ENDGLIDE:
			DEBUG_BEGIN("dos_thread->DispatchDosNT(EG)", DebugDosThreadId, 26);

			if (config.Flags & CFG_NTVDDMODE)	{
				ShutDownVDDRendering ();
			} else {
				CloseServerCommunicationArea ();
			}

			DestroyTimerBoostingThread ();

			CloseHandle(drawMutex);
			
			(*_VDDDeInstallUserHook)(hInstance);
			
			(*_setEAX)(0);
			return;

			DEBUG_END("dos_thread->DispatchDosNT(EG)", DebugDosThreadId, 26);

		/* Szerver meghívása */
		case DGCLIENT_CALLSERVER:
			DEBUG_BEGIN("dos_thread->DispatchDosNT(CS)", DebugDosThreadId, 27);
			
			c->kernelflag |= KFLAG_INKERNEL | KFLAG_DRAWMUTEXUSED;

			WaitForSingleObject(drawMutex, INFINITE);

			c->kernelflag |= KFLAG_SERVERWORKING;
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
			PostMessage(serverCmdHwnd, DGSM_PROCEXECBUFF, 0, 0);
			c->kernelflag &= ~KFLAG_INKERNEL;
			
			(*_setEAX)(0);
			return;
			
			DEBUG_END("dos_thread->DispatchDosNT(CS)", DebugDosThreadId, 27);
		
		/* Platform lekérdezése */
		case DGCLIENT_GETPLATFORMTYPE:
			(*_setEAX)(PLATFORM_DOSWINNT);
			return;

		/* DOS idõszeletének elengedése (NT) */
		case DGCLIENT_RELEASE_TIME_SLICE:
			SwitchToThread();
			return;

		case DGCLIENT_RELEASE_MUTEX:
			DEBUG_BEGIN("dos_thread->DispatchDosNT(RM)", DebugDosThreadId, 28);
			
			ReleaseMutex(drawMutex);
			c->kernelflag &= ~KFLAG_DRAWMUTEXUSED;
			
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

			if (c->kernelflag & KFLAG_SUSPENDDOSTHREAD) {
				c->kernelflag &= ~KFLAG_SUSPENDDOSTHREAD;
				SuspendThread (ntDosThreadHandle);
			}
			
			DEBUG_END("dos_thread->DispatchDosNT(RM)", DebugDosThreadId, 28);
			return;

		case DGCLIENT_VESA:
			DEBUG_BEGIN("dos_thread->DispatchDosNT(VESA)", DebugDosThreadId, 155);

			vesaRegisters.eax = (*_getEAX) () >> 16;
			vesaRegisters.ebx = (*_getEBX) ();
			vesaRegisters.ecx = (*_getECX) () >> 16;
			vesaRegisters.edx = (*_getEDX) ();
			vesaRegisters.esi = (*_getESI) ();
			vesaRegisters.edi = (*_getEDI) ();
			vesaRegisters.es = (unsigned long) (*_getES) ();

			(*_setEBP) (VESAHandler (&vesaRegisters));
			
			(*_setEAX) (vesaRegisters.eax << 16);
			(*_setEBX) (vesaRegisters.ebx);
			(*_setECX) (vesaRegisters.ecx << 16);
			(*_setEDX) (vesaRegisters.edx);
			(*_setESI) (vesaRegisters.esi);
			(*_setEDI) (vesaRegisters.edi);
			(*_setES) (vesaRegisters.es);
			return;

			DEBUG_END("dos_thread->DispatchDosNT(VESA)", DebugDosThreadId, 155);

		case DGCLIENT_BEGINVESA:
			if (config.Flags & CFG_NTVDDMODE)	{
				if ((errorCode = InitVDDRendering ()) == 0) {
					if ((errorCode = VESAInit ((void *) _getEDX ())) != 0) {
						ShutDownVDDRendering ();
					}
				}
			} else {
				if ((errorCode = OpenServerCommunicationArea (&regInfo)) == 0) {
					if ((errorCode = VESAInit ((void *) _getEDX ())) != 0) {
						CloseServerCommunicationArea ();
					}
				}
			}
			(*_setECX) (errorCode);
			return;

		case DGCLIENT_ENDVESA:
			if (config.Flags & CFG_NTVDDMODE)	{
				ShutDownVDDRendering ();
			} else {
				CloseServerCommunicationArea ();
			}
			return;

		case DGCLIENT_INSTALLMOUSE:
			params[0] = (((unsigned int) (*_getCS) ()) << 4) + (*_getEDX) ();
			if (NTCallFunctionOnServer (GLIDEINSTALLMOUSE, 1, params))
				(*_setCF) (0);
			else
				(*_setCF) (1);
			return;

		case DGCLIENT_UNINSTALLMOUSE:
			if (NTCallFunctionOnServer (GLIDEUNINSTALLMOUSE, 0, NULL))
				(*_setCF) (0);
			else
				(*_setCF) (1);
			return;

		case DGCLIENT_GETVERSION:
			(*_setEAX) ((unsigned long) DGVOODOOVERSION);
			return;

		default:
			return;
	}
}

/*------------------------------------------------------------------------------------------*/
/*................................ Lokális megjelenítõszál .................................*/


DWORD WINAPI VDDRenderingThread(LPVOID lpParameter)	{
MSG msg;
	
	DEBUG_THREAD_REGISTER(DebugRenderThreadId);

	DEBUG_BEGIN("VDDRenderingThread", DebugRenderThreadId, 22);
	
	DEBUGLOG(0,"\nVDDRenderingThread: Entering thread");
	DEBUGLOG(1,"\nVDDRenderingThread: Szál elindítása");

	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	CreateServerCommandWindow ();
	CreateClientCommandWindow ();
	
	SetEvent(threadSyncEvent);
	
	if (serverCmdHwnd != NULL && clientCmdHwnd != NULL)	{
		while(1)	{
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	
	} else {
		DEBUGLOG(0,"\n   Error: Creating command window(s) has failed, exiting thread");
		DEBUGLOG(1,"\n   Hiba: A parancsablak(ok) létrehozása nem sikerült, szál megszüntetése");
	}

	DestroyClientCommandWindow ();
	DestroyServerCommandWindow ();

	DEBUGLOG(0,"\nVDDRenderingThread: Exiting thread successfully");
	DEBUGLOG(1,"\nVDDRenderingThread: Szál sikeres befejezése");
	ExitThread(0);
	return(0);
	
	DEBUG_END("VDDRenderingThread", DebugRenderThreadId, 22);
}


int InitVDDRendering ()
{
int	error = 0;

	if (ntVDDRenderingRefCount++)
		return 0;

	if ( (c = _GetMem(sizeof(CommArea))) != NULL)	{
		ZeroMemory(c, sizeof(CommArea));
		c->areaId.areaId = AREAID_COMMAREA;
		c->ExeCodeIndex = 0;
		c->FDPtr = c->FuncData;
		c->WinOldApHWND = (unsigned int) consoleHwnd;
	} else	{
		error = 1;
		DEBUGLOG(0, "\n   Error: InitVDDRendering: failed, cannot allocate memory for CA");
		DEBUGLOG(1, "\n   Hiba: InitVDDRendering: nem sikerült, nem tudok memóriát foglalni a CA-hoz");
	}
	if (!error)
		if ( (threadSyncEvent = CreateEvent(NULL,TRUE,FALSE,NULL)) == NULL)	{
			DEBUGLOG(0, "\n   Error: InitVDDRendering failed, cannot create an event object (TSE), error code: 0%x", GetLastError());
			DEBUGLOG(1, "\n   Hiba: InitVDDRendering nem sikerült, nem tudok létrehozni egy eseményobjektumot (TSE), hibakód: 0%x", GetLastError());
			error = 1;
		}
	if (!error)
		if ( (vddRenderingThreadHandle = CreateThread(NULL, 32768, VDDRenderingThread, consoleHwnd, 0, &vddRenderingThreadId)) == NULL)	{
			DEBUGLOG(0,"\n   Error: InitVDDRendering failed: cannot create the rendering thread, error code: 0%x", GetLastError());
			DEBUGLOG(1,"\n   Hiba: InitVDDRendering nem sikerült: a megjelenítõ-szálat nem tudom létrehozni, hibakód: 0%x", GetLastError());
			error = 1;
		}
	if (!error)	{
		WaitForSingleObject(threadSyncEvent, INFINITE);
		if (serverCmdHwnd == NULL) error = 1;
	}
	if (error)	{
		if (c != NULL) _FreeMem(c);
		if (threadSyncEvent) CloseHandle(threadSyncEvent);
	}
	return error;
}


void ShutDownVDDRendering ()
{
	if (!(--ntVDDRenderingRefCount)) {
		
		DEBUGCHECK (0, c == NULL, "\n   Error: ShutDownVDDRendering: comm pointer is NULL!");
		DEBUGCHECK (1, c == NULL, "\n   Hiba: ShutDownVDDRendering: a comm pointer NULL!");
		
		if (c != NULL) _FreeMem(c);
		c = NULL;
		PostThreadMessage(vddRenderingThreadId, WM_QUIT, 0, 0);
		WaitForSingleObject(vddRenderingThreadHandle, INFINITE);
		CloseHandle(vddRenderingThreadHandle);
		CloseHandle(threadSyncEvent);
	}
}


/*------------------------------------------------------------------------------------------*/
/*.......................... Lokális kommunikációs szál, szerver módban ....................*/


DWORD WINAPI LocalClientThread(LPVOID lpParameter)	{
MSG msg;

	DEBUG_THREAD_REGISTER(debugLocalClientThreadId);

	DEBUG_BEGIN("LocalClientThread", debugLocalClientThreadId, 154);

	CreateClientCommandWindow ();
		
	SetEvent(threadSyncEvent);
	if (clientCmdHwnd == NULL)	{
		DEBUGLOG(0,"\n   Error: LocalClientThread failed, cannot create local client wobject, error code: 0%x", GetLastError());
		DEBUGLOG(1,"\n   Error: LocalClientThread nem sikerült, nem tudom létrehozni az lokális kliensoldali wobjektumot, hibakód: 0%x", GetLastError());
		return(1);
	}
	
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	while(1)	{
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
	DestroyClientCommandWindow ();
	return(0);
	
	DEBUG_END("LocalClientThread", debugLocalClientThreadId, 154);
}


int CreateLocalClientThread()
{
	if ( (threadSyncEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL)	{
		if ((localClientThreadHandle = CreateThread(NULL, 4096, LocalClientThread, 0, 0, &localClientThreadId)) == NULL)	{
			CloseHandle(threadSyncEvent);
			threadSyncEvent = NULL;
			return(0);
		}
		WaitForSingleObject(threadSyncEvent, INFINITE);
		return(1);
	} else {
		localClientThreadHandle = NULL;
		DEBUGLOG(0,"\n   Error: CreateLocalClientThread failed, cannot create local client sync event, error code: 0%x",GetLastError());
		DEBUGLOG(1,"\n   Hiba: CreateLocalClientThread nem sikerült, nem tudom létrehozni egy szinkronobjektumot a kliensoldali kommunikációhoz , hibakód: 0%x",GetLastError());
		return(0);
	}
}


void DestroyLocalClientThread()
{
	if (localClientThreadHandle != NULL)	{
		WaitForSingleObject(threadSyncEvent, INFINITE);
		CloseHandle(threadSyncEvent);

		if (clientCmdHwnd != NULL)	{
			SendMessage(clientCmdHwnd, WM_CLOSE, 0, 0);
			WaitForSingleObject(localClientThreadHandle, INFINITE);
			CloseHandle (localClientThreadHandle);
			localClientThreadHandle = NULL;
		}
	}
}


int OpenServerCommunicationArea (ServerRegInfo *regInfo)
{
int				errorCode;
ServerRegInfo	*pRegInfo;
AreaIdPrefix	*pAreaId;
CommArea		*commAreaInServer;

	if (serverOpeningRefCount++)
		return(0);

	errorCode = 1;
	if (CreateLocalClientThread()) {
		NTInitMouse();
		ntSharedMemHandle = OpenFileMapping(FILE_MAP_WRITE, FALSE, SharedMemName);
		errorCode = 3;
		if (ntSharedMemHandle != NULL)	{
			errorCode = 1;
			if ( (c = (CommArea *) MapViewOfFile(ntSharedMemHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
				pAreaId = (AreaIdPrefix *) c;
				if (pAreaId->areaId == AREAID_REGSTRUCTURE) {
					pRegInfo = (ServerRegInfo *) pAreaId;
					if (regInfo != NULL)
						*regInfo = *pRegInfo;
					commAreaInServer = pRegInfo->serveraddrspace;
					serverCmdHwnd = pRegInfo->hidWinHwnd;
					c->areaId.areaId = AREAID_COMMAREA;
					c->kernelflag = 0;
					c->ExeCodeIndex = 0;
					c->FDPtr = c->FuncData;
					c->consoleHwnd = consoleHwnd;
					c->videoMemoryClient = &c->LFB;
					c->VideoMemory = &commAreaInServer->LFB;
					SendMessage(serverCmdHwnd, DGSM_SETCONSOLEHWND, (WPARAM) consoleHwnd, 0);
					SendMessage(serverCmdHwnd, DGSM_SETCLIENTHWND, (WPARAM) clientCmdHwnd, 0);
					errorCode = 0;
				} else if (pAreaId->areaId == AREAID_COMMAREA) {
					errorCode = 4;
					DEBUGLOG(0,"\n   Error: OpenServerCommunicationArea: opening connection to server has failed: cannot find regstructure, probably server is already serving another client");
					DEBUGLOG(1,"\n   Hiba: OpenServerCommunicationArea: a kapcsolatfelvétel a szerverrel nem sikerült: nem találom a regstruktúrát, a szerver valószínûleg már egy másik klienst szolgál ki");
				} else	{
					DEBUGLOG(0,"\n   Error: OpenServerCommunicationArea: opening connection to server has failed: cannot find regstructure for unknown reason");
					DEBUGLOG(1,"\n   Hiba: OpenServerCommunicationArea: a kapcsolatfelvétel a szerverrel nem sikerült: nem találom a regstruktúrát ismeretlen okok miatt");
				}
			} else {
				CloseHandle(ntSharedMemHandle);
				DEBUGLOG(0,"\nError: OpenServerCommunicationArea: opening connection to server has failed: cannot map shared mem, error code: 0%x",GetLastError());
				DEBUGLOG(1,"\nHiba: OpenServerCommunicationArea: a kapcsolatfelvétel a szerverrel nem sikerült: nem tudom leképezni az osztott memóriát, hibakód: 0%x",GetLastError());
			}
		} else	{
			DEBUGLOG(0,"\nError: OpenServerCommunicationArea: opening connection to server has failed: cannot open shared memory, error code: 0%x",GetLastError());
			DEBUGLOG(1,"\nHiba: OpenServerCommunicationArea: a kapcsolatfelvétel a szerverrel nem sikerült: nem tudom megnyitni az osztott memóriát, hibakód: 0%x",GetLastError());
		}
		if (errorCode) {
			DestroyLocalClientThread ();
			serverOpeningRefCount = 0;
		}
	}
	return(errorCode);
}


void CloseServerCommunicationArea ()
{
	if (serverOpeningRefCount > 0) {
		if (!(--serverOpeningRefCount)) {
			if (ntSharedMemHandle != NULL)
				CloseHandle(ntSharedMemHandle);
			ntSharedMemHandle = NULL;
			DestroyLocalClientThread();
			SendMessage(serverCmdHwnd, DGSM_CLOSEBINDING, (WPARAM) clientCmdHwnd, 0);
		}
	}
}



DWORD		NTCallFunctionOnServer (unsigned long funcCode, unsigned int pNum, unsigned long *params)
{
	unsigned long	*execBuff = c->ExeCodes + c->ExeCodeIndex;
	execBuff[0] = funcCode;
	CopyMemory(execBuff + 1, params, pNum * sizeof(unsigned long));
	execBuff[pNum + 1] = 0xFFFFFFFF;
	c->ExeCodeIndex = 0;
	SendMessage (serverCmdHwnd, DGSM_PROCEXECBUFF, 0, 0);
	return execBuff[0];
}


int			DOSInstallIOHook (unsigned short firstPort, unsigned short lastPort,
							  void (_stdcall *inHandler)(unsigned short, unsigned char *),
							  void (_stdcall *outHandler)(unsigned short, unsigned char) )
{
VDD_IO_PORTRANGE	portRange;
VDD_IO_HANDLERS		ioHandlers;

	ZeroMemory (&ioHandlers, sizeof(VDD_IO_HANDLERS));

	portRange.First = firstPort;
	portRange.Last = lastPort;
	ioHandlers.inb_handler = (PFNVDD_INB) inHandler;
	ioHandlers.outb_handler = (PFNVDD_OUTB) outHandler;
	return (*_VDDInstallIOHook) (hInstance, 1, &portRange, &ioHandlers);
}


void		DOSUninstallIOHook (unsigned short firstPort, unsigned short lastPort)
{
	VDD_IO_PORTRANGE	portRange;
	portRange.First = firstPort;
	portRange.Last = lastPort;

	(*_VDDDeInstallIOHook) (hInstance, 1, &portRange);
}


static void _stdcall NTVDM_Kill_Callback(unsigned short DosPDB)	{
	
	SendMessage(serverCmdHwnd, DGSM_DGVOODOORESET, 0, 0);
	ExitDosNt ();
}


#endif	/* DOS_SUPPORT */

#endif	/* GLIDE3 */