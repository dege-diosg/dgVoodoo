/*--------------------------------------------------------------------------------- */
/* DosMouseNt.c - Glide virtual mouse emulated by DirectInput7 for WinNT            */
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
/* dgVoodoo: Mouse.c																		*/
/*			 Egértámogatás XP alá DOS-hoz													*/
/*------------------------------------------------------------------------------------------*/


#include <windows.h>
#include "Dinput.h"

#include "dgVoodooBase.h"
#include "Debug.h"
#include "Version.h"
#include "Main.h"
#include "dgVoodooConfig.h"

#include "Dos.h"
#include "DosMouseNt.h"
#include "nt_vdd.h"
#include "vddsvc.h"


#ifndef GLIDE3

#ifdef	DOS_SUPPORT


/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/

#define	MOUSE_MOVEMENT		1
#define LEFT_PRESSED		2
#define LEFT_RELEASED		4
#define RIGHT_PRESSED		8
#define RIGHT_RELEASED		16
#define CENTER_PRESSED		32
#define CENTER_RELEASED		64

#define MCOMMAND_ACQUIRE	1
#define MCOMMAND_UNACQUIRE	2
#define MCOMMAND_EXIT		3


/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák ...................................*/


/*------------------------------------------------------------------------------------------*/
/*................................... Globális változók.....................................*/


HANDLE			ntMouseEventThreadHandle;
DWORD			NTMouseEventID;

HANDLE			ntMouseEvent, ntCommandEvent, ntMouseSyncEvent;

unsigned int	mouseCommand;
unsigned int	acquired;
MouseDriverInfo *mouseinfo;
DIMOUSESTATE	MouseState;
BYTE			rgbButtons[3];
short			eventmask, button;
short			move_x, move_y;
DWORD			prevtime,alltime;
short			move_x_ds, move_y_ds, allmove;
int				ntMouseInstalled = 0;
int				go = 0;

int				DebugMouseThreadId;


/*------------------------------------------------------------------------------------------*/
/*................................ Mouse-support függvények ................................*/

void NTMouseInterruptDoWorkAndConversion()	{
int i;

	eventmask = 0;
	if ( (move_x != 0) || (move_y != 0) )	{
		alltime += GetTickCount() - prevtime;
		if (alltime > 1000)	{
			alltime = 0;
			allmove = move_x_ds = move_y_ds = 0;
		}
		move_x_ds += move_x;
		move_y_ds += move_y;
		allmove += ABS(move_x) + ABS(move_y);
		if (alltime < 1000)	{
			if (allmove >= mouseinfo->ds_threshold)	{
				move_x += move_x_ds;
				move_y += move_y_ds;
				alltime = 0;
				allmove = move_x_ds = move_y_ds = 0;
			}
		}
		eventmask |= MOUSE_MOVEMENT;
	}
	button = 2;
	for(i = 0; i < 3; i++)	{
		if (rgbButtons[i] != MouseState.rgbButtons[i])	{
			if (MouseState.rgbButtons[i] & 0x80) eventmask |= button;
				else eventmask |= (button << 1);
		}
		button <<= 2;
		rgbButtons[i] = MouseState.rgbButtons[i];
	}
}


DWORD WINAPI NTMouseEventThread(LPVOID realModeHandle)	{
HINSTANCE			hDInput;
HRESULT				(WINAPI *DirectInputCreateEx)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
LPDIRECTINPUT7		lpDI;
LPDIRECTINPUTDEVICE lpDIdevice;
DIPROPDWORD			DIPropDWORD;
HANDLE				waitObjects[2];

	DEBUG_THREAD_REGISTER(DebugMouseThreadId);

	DEBUG_BEGIN("NTMouseThread", DebugMouseThreadId, 153);

	mouseinfo = (MouseDriverInfo *) realModeHandle;
	if (!(config.Flags & CFG_NTVDDMODE)) SendMessage(clientCmdHwnd, DGCM_MOUSEDRIVERSTRUC, (WPARAM) mouseinfo, 0);

	DIPropDWORD.diph.dwSize = sizeof(DIPROPDWORD);
	DIPropDWORD.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	DIPropDWORD.diph.dwObj = DIPH_DEVICE;
	go = 0;
	if ( (hDInput = LoadLibrary("dinput.dll")) != NULL)	{
		if ( (DirectInputCreateEx = (HRESULT (WINAPI *)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter))
			GetProcAddress(hDInput, "DirectInputCreateEx")) !=NULL)	{
			
			if ((*DirectInputCreateEx)(hInstance, DIRECTINPUT_VERSION, &IID_IDirectInput7, &lpDI, NULL) == DI_OK)	{
				if ( lpDI->lpVtbl->CreateDeviceEx(lpDI, &GUID_SysMouse, &IID_IDirectInputDevice7, &lpDIdevice, NULL) == DI_OK)	{
					go = 1;
					
					waitObjects[1] = ntMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
					waitObjects[0] = ntCommandEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
					if ((ntMouseEvent == NULL) || (ntCommandEvent == NULL))
						go = 0;
					
					SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
					lpDIdevice->lpVtbl->SetDataFormat(lpDIdevice, &c_dfDIMouse);
					lpDIdevice->lpVtbl->SetCooperativeLevel(lpDIdevice, renderingWinHwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
					DIPropDWORD.dwData = DIPROPAXISMODE_REL;
					lpDIdevice->lpVtbl->SetProperty(lpDIdevice, DIPROP_AXISMODE, &DIPropDWORD.diph);
					lpDIdevice->lpVtbl->SetEventNotification(lpDIdevice, ntMouseEvent);

					acquired = 0;
					if (!FAILED(lpDIdevice->lpVtbl->Acquire(lpDIdevice))) acquired = 1;
					
					prevtime = alltime = 0;
					move_x_ds = move_y_ds = allmove = 0;
					rgbButtons[0] = rgbButtons[1] = rgbButtons[2] = 0;
					
					SetEvent (ntMouseSyncEvent);
					while(go)	{
						ResetEvent (ntMouseSyncEvent);
						switch(WaitForMultipleObjects(2, waitObjects, FALSE, INFINITE))	{
							case (WAIT_OBJECT_0 + 1):
								if (lpDIdevice->lpVtbl->GetDeviceState(lpDIdevice, sizeof(MouseState), &MouseState) != DIERR_INPUTLOST)	{
									SendMessage(clientCmdHwnd, DGCM_MOUSEINTERRUPT, (MouseState.lX << 16) + MouseState.lY,
												((unsigned int) MouseState.rgbButtons[0] << 16) + ((unsigned int) MouseState.rgbButtons[1] << 8) + ((unsigned int) MouseState.rgbButtons[2]) );
								} else lpDIdevice->lpVtbl->Acquire(lpDIdevice);
								break;

							case WAIT_OBJECT_0:
								ResetEvent (ntCommandEvent);
								switch(mouseCommand)	{
									case MCOMMAND_ACQUIRE:
										if (!acquired) if (!FAILED(lpDIdevice->lpVtbl->Acquire(lpDIdevice))) acquired = 1;
										break;
									case MCOMMAND_UNACQUIRE:
										if (acquired) if (!FAILED(lpDIdevice->lpVtbl->Unacquire(lpDIdevice))) acquired = 0;
										break;
									case MCOMMAND_EXIT:
										go = 0;
										break;
									default:
										break;
								}
								mouseCommand = 0;
								SetEvent (ntMouseSyncEvent);
								break;
							default:
								break;

						}
					}
					lpDIdevice->lpVtbl->Unacquire(lpDIdevice);
					RestoreMouseCursor(1);
					lpDIdevice->lpVtbl->Release(lpDIdevice);
					lpDI->lpVtbl->Release(lpDI);
					if (ntMouseEvent != NULL)
						CloseHandle (ntMouseEvent);
					if (ntCommandEvent != NULL)
						CloseHandle (ntCommandEvent);
				} else {
					DEBUGLOG (0,"\n   Error: NTMouseEventThread: Initializing emulated mouse failed: cannot create DirectInput device!");
					DEBUGLOG (1,"\n   Hiba: NTMouseEventThread: Az emulált egér inicializálása nem sikerült: nem tudom létrehozni a DirectInput-eszközt!");
				}
			} else {
				DEBUGLOG (0,"\n   Error: NTMouseEventThread: Initializing emulated mouse failed: cannot acquire a proper version DirectInput interface!");
				DEBUGLOG (1,"\n   Hiba: NTMouseEventThread: Az emulált egér inicializálása nem sikerült: nem tudom lekérdezni a megfelelõ verziójú DirectInput interfészt!");
			}
		} else {
			DEBUGLOG (0,"\n   Error: NTMouseEventThread: Initializing emulated mouse failed: cannot find DirectInputCreateEx!");
			DEBUGLOG (1,"\n   Hiba: NTMouseEventThread: Az emulált egér inicializálása nem sikerült: nem találom a DirectInputCreateEx-t!");
		}
		FreeLibrary(hDInput);
	} else {
		DEBUGLOG (0,"\n   Error: NTMouseEventThread: Initializing emulated mouse failed: cannot load DINPUT.DLL!");
		DEBUGLOG (1,"\n   Hiba: NTMouseEventThread: Az emulált egér inicializálása nem sikerült: nem tudom megnyitni a DINPUT.DLL-t!");
	}
	SetEvent (ntMouseSyncEvent);
	return(0);

	DEBUG_END("NTMouseThread", DebugMouseThreadId, 153);
}


LRESULT NTLocalMouseWinHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	switch(uMsg)	{
				
		case DGCM_MOUSEDRIVERSTRUC:
			mouseinfo = (MouseDriverInfo *) wParam;
			return(0);
		
		/* HIWORD(wParam)=move_x, LOWORD(wParam)=move_y, lParam=button states */
		case DGCM_MOUSEINTERRUPT:
			MouseState.rgbButtons[0] = (((unsigned int) lParam) >> 16) & 0xFF;
			MouseState.rgbButtons[1] = (((unsigned int) lParam) >> 8) & 0xFF;
			MouseState.rgbButtons[2] = (((unsigned int) lParam) >> 0) & 0xFF;
			move_x += 2 * (((unsigned int) wParam) >> 16);
			move_y += 2 * (((unsigned int) wParam) & 0xFFFF);
			if (mouseinfo->irqevent == 0)	{
				NTMouseInterruptDoWorkAndConversion();
				mouseinfo->irqevent = eventmask;
				mouseinfo->irqmove_x = move_x;
				mouseinfo->irqmove_y = move_y;
				(*_call_ica_hw_interrupt)(ICA_SLAVE, 4, 1);
				move_x = move_y = 0;
			}
			return(0);
		
		default:
			return(1);
	}
}


void DIMouseAcquire()	{

	if (ntMouseInstalled && (!acquired))	{
		mouseCommand=MCOMMAND_ACQUIRE;
		SetEvent(ntCommandEvent);
		while(mouseCommand)
			WaitForSingleObject (ntMouseSyncEvent, INFINITE);
	}
}


void DIMouseUnacquire()	{

	if (ntMouseInstalled && acquired)	{
		mouseCommand=MCOMMAND_UNACQUIRE;
		SetEvent(ntCommandEvent);
		while(mouseCommand)
			WaitForSingleObject (ntMouseSyncEvent, INFINITE);
	}
}


void NTInitMouse()
{
	prevtime = alltime = 0;
	move_x = move_y = move_x_ds = move_y_ds = allmove = 0;
	rgbButtons[0] = rgbButtons[1] = rgbButtons[2] = 0;
}


int	NTInstallMouse(void *realModeHandle)	{

	if ((!ntMouseInstalled) && (config.Flags & CFG_SETMOUSEFOCUS))	{
		NTInitMouse();
		if ((ntMouseSyncEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL) {
			if ((ntMouseEventThreadHandle = CreateThread(NULL, 8192, NTMouseEventThread, realModeHandle, 0, &NTMouseEventID)) != NULL) {
				HideMouseCursor(1);
				ntMouseInstalled = 1;
				WaitForSingleObject (ntMouseSyncEvent, INFINITE);
				if (!go) {
					DEBUGLOG (0,"\n   Error: NTInstallMouse: Initializing emulated mouse failed: mouse interrupt thread has died!");
					DEBUGLOG (1,"\n   Hiba: NTInstallMouse: Az emulált egér inicializálása nem sikerült: az egérmegszakítás-szál meghalt!");
					CloseHandle (ntMouseSyncEvent);
					return(0);
				}
			} else {
				DEBUGLOG (0,"\n   Error: NTInstallMouse: Initializing emulated mouse failed: cannot create needed thread!");
				DEBUGLOG (1,"\n   Hiba: NTInstallMouse: Az emulált egér inicializálása nem sikerült: nem tudom létrehozni a szükséges szálat!");
				CloseHandle (ntMouseSyncEvent);
				return(0);
			}
		} else {
			DEBUGLOG (0,"\n   Error: NTInstallMouse: Initializing emulated mouse failed: cannot create needed Win32 objects!");
			DEBUGLOG (1,"\n   Hiba: NTInstallMouse: Az emulált egér inicializálása nem sikerült: nem tudom létrehozni a szükséges Win32-objektumokat!");
			return(0);
		}
	}
	return(1);
}


int	NTUnInstallMouse()	{

	if (ntMouseInstalled)	{
		mouseCommand = MCOMMAND_EXIT;
		SetEvent(ntCommandEvent);
		WaitForSingleObject(ntMouseEventThreadHandle, INFINITE);
		CloseHandle (ntMouseSyncEvent);
		CloseHandle(ntMouseEventThreadHandle);
		ntMouseInstalled = 0;
		return(1);
	}
	return(0);
}


int NTIsMouseInstalled ()
{
	return (ntMouseInstalled);
}

#endif

#endif /* !GLIDE3 */