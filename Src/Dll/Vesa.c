/*--------------------------------------------------------------------------------- */
/* Vesa.c - dgVoodoo VESA rendering implementation                                  */
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
/* dgVoodoo: Vesa.c																			*/
/*			 VESA implementáció																*/
/*------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------*/
/*....................................... Includes .........................................*/

#include "dgVoodooBase.h"
#include <windows.h>
#include <string.h>
#include "DDraw.h"
#include "D3d.h"

#include "Dgw.h"
#include "Version.h"
#include "Main.h"
#include "Debug.h"
#include "Dos.h"
#include "VxdComm.h"
#include "RMDriver.h"
#include "VesaDefs.h"
#include "dgVoodooConfig.h"
#include "Movie.h"
#include "MMConv.h"
#include "dgVoodooGlide.h"
#include "Grabber.h"
#include "Texture.h"
#include "LfbDepth.h"
#include "Vesa.h"
#include "Log.h"

#ifndef GLIDE3

#ifdef	VESA_SUPPORT

#define	GetRealModeFarPtr(p)	( (((unsigned int) (p)) & 0x0F) | ( (((unsigned int) (p)) >> 4) << 16) )


/*------------------------------------------------------------------------------------------*/
/*...................................... Globálisok ........................................*/

CommArea			*vca;
DWORD				vesaRefreshThreadId;
HANDLE				vesaRefreshThreadHandle = NULL;
HANDLE				refreshTimerEventHandle;
int					vesaInFresh = 0;
PALETTEENTRY		dPalette[256];
LPDIRECTDRAWPALETTE lpDDPalette;
int					VESAModeWillBeSet = 0;
CRITICAL_SECTION	critSection;
_pixfmt				vesaLfbFormat;
unsigned int		*convertedPalette;
unsigned int		vesaActBank;
unsigned int		vgaPalRGBIndexW, vgaPalRGBIndexR, vgaPalEntryIndexW, vgaPalEntryIndexR;
unsigned int		vgaPalReadLatch, vgaPalWriteLatch;
int					vesaIOHooksInstalled = 0;

unsigned char		vm320x200[] = "320x200";
unsigned char		vm640x400[] = "640x400";
unsigned char		vm640x480[] = "640x480";
unsigned char		vm800x600[] = "800x600";
unsigned char		vm1024x768[] = "1024x768";
unsigned char		vm1280x1024[] = "1280x1024";
unsigned char		vm256Color[] = " 256 Color";
unsigned char		vmHiColor[] = " HiColor";
unsigned char		vmTrueColor[] = " TrueColor";

unsigned char		vbeOemNameStr[]		= "dgVoodoo";
unsigned char		vbeProductNameStr[]	= "dgVoodoo VESA Emulation";
unsigned char		vbeRevisionStr[]	= "v1.1";
dgVoodooModeInfo	vesaModeList[] = {  {0x13, 320, 200, 8},	{0x100, 640, 400, 8},	{0x101, 640, 480, 8},
										{0x103, 800, 600, 8},	{0x105, 1024, 768, 8},	{0x107, 1280, 1024, 8},
										{0x10e, 320, 200, 16},	{0x10f, 320, 200, 32},	{0x111, 640, 480, 16},
										{0x112, 640, 480, 32},	{0x114, 800, 600, 16},	{0x115, 800, 600, 32},
										{0x117, 1024, 768, 16}, {0x118, 1024, 768, 32}, {0x11a, 1280, 1024, 16},
										{0x11b, 1280, 1024, 32}, {(unsigned short) 0xFFFF} };
dgVoodooModeInfo	actVideoMode = {(unsigned short) 0xEEEE};
dgVoodooModeInfo	const16BitFormat = {0x0, 0, 0, 16, 5, 11, 6, 5, 5, 0, 0, 0};
dgVoodooModeInfo	const32BitFormat = {0x0, 0, 0, 32, 8, 24, 8, 16, 8, 8, 8, 0};

int					vesaFreshThreadDebugId;

/*------------------------------------------------------------------------------------------*/
/*...................................... Predeklaráció  ....................................*/


static	dgVoodooModeInfo*	VBESearchInVideoModeList (unsigned short modeNumber);
static	DWORD WINAPI		VesaRefreshThread (LPVOID);

/*------------------------------------------------------------------------------------------*/
/*........................................ Függvények  .....................................*/


static unsigned int GetSizeAndPos(DWORD mask)
{
unsigned int pos=0;
unsigned int size=0;
	
	if(mask == 0) return(0);
	while (!(mask&1))	{
		mask>>=1;
		pos++;
	}
	while (mask&1)	{
		mask>>=1;
		size++;
	}
	return((size<<16)+pos);
}


static void ConvertToModeInfo(LPDDSURFACEDESC2 lpDDSD, dgVoodooModeInfo *lpmi)
{
DWORD RsvMask;
unsigned int temp;

	ZeroMemory(lpmi,sizeof(dgVoodooModeInfo));
	lpmi->XRes = (short) lpDDSD->dwWidth;
	lpmi->YRes = (short) lpDDSD->dwHeight;
	if ( (lpmi->BitPP = (unsigned char) lpDDSD->ddpfPixelFormat.dwRGBBitCount) >= 16)	{
		temp = GetSizeAndPos(RsvMask = lpDDSD->ddpfPixelFormat.dwRBitMask);
		lpmi->RedSize = temp >> 16;
		lpmi->RedPos = temp & 0xFF;
		RsvMask |= lpDDSD->ddpfPixelFormat.dwGBitMask;
		temp = GetSizeAndPos(lpDDSD->ddpfPixelFormat.dwGBitMask);
		lpmi->GreenSize = temp >> 16;
		lpmi->GreenPos = temp & 0xFF;
		RsvMask |= lpDDSD->ddpfPixelFormat.dwBBitMask;
		temp = GetSizeAndPos(lpDDSD->ddpfPixelFormat.dwBBitMask);
		lpmi->BlueSize = temp >> 16;
		lpmi->BluePos = temp & 0xFF;
		if (lpmi->BitPP == 16) RsvMask |= 0xFFFF0000;
		temp = GetSizeAndPos(~RsvMask);
		lpmi->RsvSize=temp >> 16;
		lpmi->RsvPos = temp & 0xFF;
	}
}


HRESULT WINAPI ServerRegisterModes(LPDDSURFACEDESC2 lpDDSD,  LPVOID lpContext)	{
dgVoodooModeInfo info;
dgVoodooModeInfo *pVesaModeInfo;
		
	ConvertToModeInfo(lpDDSD, &info);
	switch(platform) {
		case PLATFORM_DOSWIN9X:
			DeviceIoControl(hDevice, DGSERVER_REGISTERMODE ,&info, 0, NULL, 0, NULL, NULL);
			break;

		case PLATFORM_DOSWINNT:		
			for (pVesaModeInfo = vesaModeList; pVesaModeInfo->ModeNumber != 0xFFFF; pVesaModeInfo++) {
				if ( (pVesaModeInfo->XRes == info.XRes) && (pVesaModeInfo->YRes == info.YRes) &&
					 (pVesaModeInfo->BitPP == info.BitPP) ) {
					
					info.ModeNumber = pVesaModeInfo->ModeNumber;
					*pVesaModeInfo = info;
				}
			}
			break;
	}
	return(DDENUMRET_OK);
}


int InitVESA()	{
DDSURFACEDESC2 DDSDtemp;
	
	ZeroMemory(&DDSDtemp, sizeof(DDSURFACEDESC2));
	DDSDtemp.dwSize = sizeof(DDSURFACEDESC2);
	if (config.Flags & CFG_WINDOWED)	{
		GetActDispMode(NULL, &DDSDtemp);
		ConvertToModeInfo(&DDSDtemp, &actVideoMode);

		switch(platform) {
			case PLATFORM_DOSWIN9X:
				DeviceIoControl(hDevice, DGSERVER_REGISTERACTMODE, &actVideoMode, 0, NULL, 0, NULL, NULL);
				break;
			case PLATFORM_DOSWINNT:
				break;
		}
	}
	InitializeCriticalSection(&critSection);
	EnumDispModes(NULL, ServerRegisterModes, MV_FULLSCR);
	vca = c;
	return(1);
}


void CleanupVESA()	{

	DeleteCriticalSection(&critSection);
}


unsigned int VesaSetVBEMode()	{
unsigned char	*temp;
char			windowname[256];
unsigned int	flags, tempui, init;
MOVIEDATA		tempmd, *lpMd;
unsigned int	i;
	
	DEBUG_BEGIN("VesaSetVBEMode", DebugRenderThreadId, 153);

	if (runflags & RF_INITOK) {
		VESAModeWillBeSet = 1;
		return(0x004F);
	}
	VESAModeWillBeSet = 0;
	if (vca->VESASession)	{
		VesaUnsetVBEMode(0);
		lpMd = &tempmd;
		init = 0;
	} else	{
		lpMd = &movie;
		init = 1;
		CreateRenderingWindow();
	}
	vca->kernelflag &= ~KFLAG_VESAFRESHDISABLED;
		
	if (config.Flags & CFG_WINDOWED) {
		switch(platform) {
			case PLATFORM_DOSWIN9X:
				DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONSYSVM, NULL, 0, NULL, 0, NULL, NULL);
				break;
			case PLATFORM_DOSWINNT:
				SetForegroundWindow(renderingWinHwnd);
				break;
		}
	}
	flags = MV_VIDMEM | MV_NOCONSUMERCALL | ((config.Flags & CFG_WINDOWED) ? MV_WINDOWED : MV_FULLSCR);
	vesaInFresh = 0;
	lpMd->cmiFlags = flags;
		
	if ( (lpMd->cmiBitPP != 8) || (config.Flags & CFG_WINDOWED) ) {
		lpMd->cmiFlags |= MV_3D;
		if ((convertedPalette = (unsigned int *) _GetMem(256 * 4)) == NULL) return(0x014F);
	}

	if (init)	{
		if (InitDraw(&movie, NULL))	{
			lpDD = movie.cmiDirectDraw;
		} else return(0x014F);
	}
	lpMd->cmiXres = vca->actmodeinfo.XRes;
	lpMd->cmiYres = vca->actmodeinfo.YRes;
	lpMd->cmiBitPP = vca->actmodeinfo.BitPP;
	lpMd->cmiFreq = 60;
	lpMd->cmiBuffNum = 2;
	//lpMd->cmiCallWhenMIRQ = VESAGrabVBE;
	lpMd->cmiWinHandle = renderingWinHwnd;

	LOG(1,"\nm.x: %d, m.y: %d, m.bitpp: %d", lpMd->cmiXres, lpMd->cmiYres, lpMd->cmiBitPP);
	
	windowname[0] = 0;
	strcat(windowname, productName);
	if (vca->progname[0])	{
		strcat(windowname, " - ");
		strcat(windowname, vca->progname + 1);
	}
	strcat(windowname," (");
	if (vca->actmodeinfo.ModeNumber == 0x13) strcat(windowname,"VGA ");
	else strcat(windowname,"VESA ");
	switch( tempui = ((vca->actmodeinfo.XRes << 16) + vca->actmodeinfo.YRes) )	{
		case (320 << 16) + 200:			
			tempui = (640 << 16) + 400;
			temp = vm320x200;
			break;
		case (640 << 16) + 400:
			temp = vm640x400;
			break;
		case (640 << 16) + 480:
			temp = vm640x480;
			break;
		case (800 << 16) + 600:
			temp = vm800x600;
			break;
		case (1024 << 16) + 768:
			temp = vm1024x768;
			break;
		case (1280 << 16) + 1024:
			temp = vm1280x1024;
			break;
		default:
			temp = "unknown mode";
	}
	strcat(windowname, temp);
	switch(vca->actmodeinfo.BitPP)	{
		case 8:
			temp = vm256Color;
			break;
		case 16:
			temp = vmHiColor;
			break;
		case 32:
			temp = vmTrueColor;
			break;
		default:
			temp = "";
	}
	strcat(windowname, temp);
	strcat(windowname, ")");
	SetWindowText(renderingWinHwnd, windowname);
			
	if (init)	{
		i=3;
		while(i)	{
			if (InitMovie()) break;
			i--;
		}
		if (!i)	{
			UninitDraw();
			return(0x014F);
		}		
		HideMouseCursor(0);
	} else {
		if (!ReInitMovie(&tempmd))	{
			DeInitMovie();
			UninitDraw();
			return(0x014F);
		}
	}

	lpDDBackBuff = GABA();
	lpDDFrontBuff = GetFrontBuffer();
	
	LfbGetBackBufferFormat();
	
	if (movie.cmiBitPP == 8)	{
		lpDD->lpVtbl->CreatePalette(lpDD, DDPCAPS_8BIT | DDPCAPS_ALLOW256, //| DDPCAPS_PRIMARYSURFACE |
									dPalette, &lpDDPalette, NULL);
		GetFrontBuffer()->lpVtbl->SetPalette(GetFrontBuffer(), lpDDPalette);
		lpD3Ddevice=NULL;

	} else {
		
		if (config.Flags & CFG_GRABENABLE)	{
			lpD3D = movie.cmiDirect3D;
			if (!GlideSelectAndCreate3DDevice())	{
				DeInitMovie();
				UninitDraw();
				return(0x014F);
			}
			if (!TexInit())	{
				DeInitMovie();
				UninitDraw();
				return(0x014F);				
			}
			if (!GrabberInit())	{
				DeInitMovie();
				UninitDraw();
				return(0x014F);
			}
		}
	}	
	
	if (actResolution != tempui) {
		i = GetIdealWindowSizeForResolution (movie.cmiXres, movie.cmiYres);
		if (IsWindowSizeIsEqualToResolution (renderingWinHwnd)) {	
			SetWindowClientArea(renderingWinHwnd, i >> 16, i & 0xFFFF);
		} else {
			AdjustAspectRatio (renderingWinHwnd, i >> 16, i & 0xFFFF);
		}
	}
	
	actResolution = tempui;
	
	ShowWindow(warningBoxHwnd, SW_HIDE);
	ShowWindow(renderingWinHwnd, SW_SHOWNORMAL);	
	if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow((HWND) c->WinOldApHWND, SW_HIDE);

	vca->VESASession = 1;
	vca->VGARAMDACSize = 6;
	vca->VGAStateRegister3DA = 0;

	vesaLfbFormat.BitPerPixel = vca->actmodeinfo.BitPP;
	vesaLfbFormat.RPos = vca->actmodeinfo.RedPos;
	vesaLfbFormat.RBitCount = vca->actmodeinfo.RedSize;
	vesaLfbFormat.GPos = vca->actmodeinfo.GreenPos;
	vesaLfbFormat.GBitCount = vca->actmodeinfo.GreenSize;
	vesaLfbFormat.BPos = vca->actmodeinfo.BluePos;
	vesaLfbFormat.BBitCount = vca->actmodeinfo.BlueSize;
	vesaLfbFormat.APos = vesaLfbFormat.ABitCount = 0;
	
	//Init default palette
	for(i = 0; i < 256; i++) {
		vca->VESAPalette[i] = ((unsigned int) vesaDefaultPalette[3*i] << 16) +
							  ((unsigned int) vesaDefaultPalette[3*i+1] << 8) +
							  ((unsigned int) vesaDefaultPalette[3*i+2]);
	}
	vca->kernelflag |= KFLAG_PALETTECHANGED;

	refreshTimerEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	vesaRefreshThreadHandle = CreateThread(NULL, 4096, VesaRefreshThread, 0, 0, &vesaRefreshThreadId);
	SetThreadPriority(vesaRefreshThreadHandle, THREAD_PRIORITY_HIGHEST);
	SetForegroundWindow(renderingWinHwnd);	
	HideMouseCursor(0);
	//if (config.Flags & CFG_WINDOWED) {
		switch(platform) {
			case PLATFORM_DOSWIN9X:
				DeviceIoControl(hDevice, DGSERVER_SETFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
				break;
		}
	//}
	EnterCriticalSection(&critSection);
	GetFrontBuffer()->lpVtbl->Lock(GetFrontBuffer(), NULL, &lfb, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL);
	temp = lfb.lpSurface;
	/*for(j=0;j<movie.cmiYres;j++)	{
		for(i=0;i<movie.cmiXres*movie.cmiByPP;i++) temp[i]=0;
		temp+=ddsd1234.lPitch;
	}*/
	GetFrontBuffer()->lpVtbl->Unlock(GetFrontBuffer(),NULL);
	LeaveCriticalSection(&critSection);	
	return(0x004F);	

	DEBUG_END("VesaSetVBEMode", DebugRenderThreadId, 153);
}


/* flag jelentései (csak belsôleg használt):	*/
/*	0	-	ne adja vissza a fókuszt a VM-re, mert egy újabb VESA-mód lesz beállítva */
/*	1	-	visszaadja a fókuszt, megjeleníti a konzolt és a warning-boxot, eltünteti a	*/
/*			kép-ablakot	*/
/*  2   -   */
void VesaUnsetVBEMode(int flag)	{

	DEBUG_BEGIN("VesaUnsetVBEMode", DebugRenderThreadId, 154);

	VESAModeWillBeSet = 0;
	if (runflags & RF_INITOK) return;
	if (!vca->VESASession) return;
	EnterCriticalSection(&critSection);
	vca->VESASession = 0;	
	SetEvent(refreshTimerEventHandle);
	if (movie.cmiBitPP == 8) GetFrontBuffer()->lpVtbl->SetPalette(GetFrontBuffer(),NULL);
	
	if ( (config.Flags & CFG_GRABENABLE) && (movie.cmiBitPP != 8) )	{
		GrabberCleanUp();
		TexCleanUp();
		if (lpD3Ddevice) lpD3Ddevice->lpVtbl->Release(lpD3Ddevice);
		lpD3Ddevice=NULL;
	}
	if (flag)	{		
		DeInitMovie();
		RestoreMouseCursor(1);
		UninitDraw();
		LOG(1,"\nDestroyWindow!!..");
		DestroyRenderingWindow();
		if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow((HWND) c->WinOldApHWND, SW_SHOW);
		ShowWindow(renderingWinHwnd, SW_HIDE);
		ShowWindow(warningBoxHwnd, SW_SHOW);
		if (flag == 1) {
			switch(platform) {
				case PLATFORM_DOSWIN9X:
					DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
					break;
				
				case PLATFORM_DOSWINNT:
					//if (!(config.Flags & CFG_WINDOWED)) ShowWindow(consoleHwnd, SW_MAXIMIZE);
					SetActiveWindow(consoleHwnd);
					SetForegroundWindow(consoleHwnd);
					ShowWindow(consoleHwnd, SW_MAXIMIZE);
					//SetWindowPos(consoleHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREPOSITION | SWP_SHOWWINDOW);
					break;
			}
		}
	}
	LeaveCriticalSection(&critSection);

	WaitForSingleObject (vesaRefreshThreadHandle, INFINITE);
	CloseHandle (vesaRefreshThreadHandle);
	
	InitializeCriticalSection(&critSection);

	if (convertedPalette != NULL) _FreeMem(convertedPalette);
	convertedPalette = NULL;

	DEBUG_END("VesaUnsetVBEMode", DebugRenderThreadId, 154);
}


static void __inline CopyA000ToBank (int vesaBank)
{
	CopyMemory (((char *) vca->videoMemoryClient) + (vesaBank * 0x10000), (void *) 0xA0000, 0x10000);
}


static void __inline CopyBankToA000 (int vesaBank)
{
	CopyMemory ((void *) 0xA0000, ((char *) vca->videoMemoryClient) + (vesaBank * 0x10000), 0x10000);	
}


static void VesaCopyLFBFrame()	{
LPDIRECTDRAWSURFACE7	lpDDBackBuffer;
DDSURFACEDESC2			ddsdBackBuffer;
int						convType;

	if ( (lpDDBackBuffer = BeginDraw()) != NULL)	{
		
		ZeroMemory(&ddsdBackBuffer, sizeof(DDSURFACEDESC2));
		ddsdBackBuffer.dwSize = sizeof(DDSURFACEDESC2);

		if ((vca->actmodeinfo.BitPP == 8) && (vca->kernelflag & KFLAG_PALETTECHANGED)) {
			if (movie.cmiBitPP == 8) {
				MMCConvARGBPaletteToABGR (vca->VESAPalette, (unsigned int *) dPalette);
				lpDDPalette->lpVtbl->SetEntries(lpDDPalette, 0, 0, 256, dPalette);
			} else {
				MMCConvARGBPaletteToPixFmt(vca->VESAPalette, convertedPalette, 0, 256, &backBufferFormat);
			}
			vca->kernelflag &= ~KFLAG_PALETTECHANGED;
		}
		
		if ((lpDDBackBuffer->lpVtbl->Lock(lpDDBackBuffer, NULL, &ddsdBackBuffer, DDLOCK_WRITEONLY, 0)) != DD_OK) return;

		convType = MMCIsPixfmtTheSame(&vesaLfbFormat, &backBufferFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY;
		MMCConvertAndTransferData(&vesaLfbFormat, &backBufferFormat,
								  0, 0, 0,
								  ((char *)vca->VideoMemory + vca->DisplayOffset), ddsdBackBuffer.lpSurface,
								  vca->actmodeinfo.XRes, vca->actmodeinfo.YRes,
								  (config.Flags2 & CFG_YMIRROR ? -1 : 1) * vca->BytesPerScanLine,
								  ddsdBackBuffer.lPitch,
								  convertedPalette, NULL,
								  convType, PALETTETYPE_CONVERTED);
	
		lpDDBackBuffer->lpVtbl->Unlock(lpDDBackBuffer, NULL);
		
		if (config.Flags & CFG_GRABENABLE)	{
			if (vca->kernelflag & KFLAG_SCREENSHOT)	{
				if (movie.cmiBitPP != 8)	{
					lpDDFrontBuff = lpDDBackBuffer;
					GrabberHookDos();					
				} else {
					GrabberGrabLFB(NULL, dPalette);
				}
				vca->kernelflag &= ~KFLAG_SCREENSHOT;
			}			
			if (movie.cmiBitPP!=8)	{
				lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);
				lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, lpDDBackBuffer, 0);
				GrabberDrawLabel(0);
				lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
			}			
		}
		EndDraw();		
	}
}


/* Screen refreshing thread */
static DWORD WINAPI VesaRefreshThread(LPVOID lpParameter)	{
UINT			timerID;
unsigned int	freq;
	
	DEBUG_THREAD_REGISTER(vesaFreshThreadDebugId);

	DEBUG_BEGIN ("VesaRefreshThread", vesaFreshThreadDebugId, 156);

	if (vesaInFresh) return(1);
	vesaInFresh = 1;
	freq = 1000 / config.VideoRefreshFreq;
	timerID = timeSetEvent(freq, freq, refreshTimerEventHandle, 0, TIME_PERIODIC | TIME_CALLBACK_EVENT_PULSE);
	while(1)	{
		WaitForSingleObject(refreshTimerEventHandle, INFINITE);

		EnterCriticalSection(&critSection);

		if (platform == PLATFORM_DOSWINNT) {
			if (config.Flags & CFG_NTVDDMODE)
				CopyA000ToBank (vesaActBank);
			else
				SendMessage (clientCmdHwnd, DGCM_VESAGRABACTBANK, 0, 0);
		}
		
		if (!vca->VESASession)	{
			timeKillEvent(timerID);
			LeaveCriticalSection(&critSection);
			ExitThread(0);
		}
		if (!(vca->kernelflag & KFLAG_VESAFRESHDISABLED)) {
			VesaCopyLFBFrame ();
			CallConsumer (1);
		}
		/* függôleges visszafutás beállít */
		vca->VGAStateRegister3DA |= 8;
		LeaveCriticalSection(&critSection);
	}
	vesaInFresh = 0;
	return(1);

	DEBUG_END ("VesaRefreshThread", vesaFreshThreadDebugId, 156);
}


LRESULT		VESALocalClientCommandHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
	
	switch (uMsg) {
		case DGCM_VESAGRABACTBANK:
			EnterCriticalSection(&critSection);
			CopyA000ToBank (vesaActBank);
			LeaveCriticalSection(&critSection);
			return(0);
		default:
			return(1);
	}
}


LRESULT		VESAServerCommandHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
dgVoodooModeInfo	*modeInfo;

	switch (uMsg) {
		case DGSM_GETVESAMODEFORMAT:
			if ( (modeInfo = VBESearchInVideoModeList ((unsigned short) wParam)) != NULL) {
				
				vca->actmodeinfo.XRes = modeInfo->XRes;
				vca->actmodeinfo.YRes = modeInfo->YRes;
				vca->actmodeinfo.BitPP = modeInfo->BitPP;
				vca->actmodeinfo.ModeNumber = modeInfo->ModeNumber;

				if ((config.Flags & CFG_WINDOWED) && modeInfo->BitPP != 8) {
					if (modeInfo->BitPP == actVideoMode.BitPP)
						modeInfo = &actVideoMode;
					else
						modeInfo = modeInfo->BitPP == 16 ? &const16BitFormat : &const32BitFormat;
				}
				
				vca->actmodeinfo.RedPos = modeInfo->RedPos;
				vca->actmodeinfo.RedSize = modeInfo->RedSize;
				vca->actmodeinfo.GreenPos = modeInfo->GreenPos;
				vca->actmodeinfo.GreenSize = modeInfo->GreenSize;
				vca->actmodeinfo.BluePos = modeInfo->BluePos;
				vca->actmodeinfo.BlueSize = modeInfo->BlueSize;
				vca->actmodeinfo.RsvPos = modeInfo->RsvPos;
				vca->actmodeinfo.RsvSize = modeInfo->RsvSize;
			}
			return(0);
		default:
			return(1);
	}
}


void _stdcall VESAPortInHandler (unsigned short port, unsigned char *data)
{
	LOG(1,"\nread: %0p",port);
	switch(port) {
		case 0x3C7:
			*data = (unsigned char) vgaPalEntryIndexR;
			break;
		case 0x3C8:
			*data = (unsigned char) vgaPalEntryIndexW;
			break;
		case 0x3C9:
			if (vgaPalRGBIndexR == 0) {
				vgaPalReadLatch = vca->VESAPalette[vgaPalEntryIndexR];
				_asm {
					mov eax,vgaPalReadLatch
					bswap eax
					ror eax,8h
					mov vgaPalReadLatch,eax
				}
			}
			*data = (unsigned char) (vgaPalReadLatch & 0xFF);
			//LOG(1,"\nread palindex: %d, %0p",vgaPalEntryIndexR, vgaPalReadLatch);
			if (vca->VGARAMDACSize == 6)
				*data >>= 2;
			if (++vgaPalRGBIndexR == 3) {
				++vgaPalEntryIndexR;
				vgaPalEntryIndexR &= 0xFF;
				vgaPalRGBIndexR = 0;
			}
			break;
		case 0x3DA:
			LOG(1,"\n port 3da, retaddr=%0p, ret=%0p",&vca->VGAStateRegister3DA, vca->VGAStateRegister3DA);
			*data = vca->VGAStateRegister3DA;
			vca->VGAStateRegister3DA ^= 9;
			if (vca->VESASession)
				vca->VGAStateRegister3DA &= ~8;
			break;
		default:
			break;
	}
}


void _stdcall	VESAPortOutHandler (unsigned short port, unsigned char data)
{
	LOG(1,"\nwrite: %0p, %0p", port, data);
	switch(port) {
		case 0x3C7:
			vgaPalEntryIndexR = (unsigned int) data;
			vgaPalRGBIndexR = 0;
			break;
		case 0x3C8:
			vgaPalEntryIndexW = (unsigned int) data;
			vgaPalRGBIndexW = 0;
			break;
		case 0x3C9:
			//LOG(1,"\nwrite port 3c9: %d",data);
			if (vgaPalRGBIndexW == 0)
				vgaPalWriteLatch = 0x0;
			if (vca->VGARAMDACSize == 6)
				data <<= 2;
			
			vgaPalWriteLatch = (vgaPalWriteLatch << 8) | (unsigned int) data;
			
			if (++vgaPalRGBIndexW == 3) {
				vca->VESAPalette[vgaPalEntryIndexW] = vgaPalWriteLatch;
				LOG(1,"\nwrite palindex: %d, %0p",vgaPalEntryIndexW, vgaPalWriteLatch);
				++vgaPalEntryIndexW;
				vgaPalEntryIndexW &= 0xFF;
				vgaPalRGBIndexW = 0;
				vca->kernelflag |= KFLAG_PALETTECHANGED;
			}
			break;
		default:
			break;
	}
}


dgVoodooModeInfo*	VBESearchInVideoModeList(unsigned short modeNumber)
{
unsigned int		i = 0;

	LOG(1,"\nmodenumber:%0p",modeNumber);
	while(vesaModeList[i].ModeNumber != 0xFFFF) {
		LOG(1,"\ni:%d",i);
		if (modeNumber == (vesaModeList[i].ModeNumber & 0x7FFF))
			return vesaModeList + i;
		i++;
	}
	return NULL;
}


void	GetProtTablePtr (VesaDriverHeader **vesaDriverHeader, unsigned char **protTable)
{
	_asm {
		movzx	eax,WORD PTR ds:[0x10 * 4 + 2]
		xor		edx,edx
		mov		dx,ds:[0x10 * 4]
		shl		eax,4h
		mov		dx,[eax + edx - 2]
		cmp		DWORD PTR [eax+edx]VesaDriverHeader.driverHeader.driverId, DRIVERID
		je		_ptrFound
		xor		eax,eax
		mov		ebx,vesaDriverHeader
		mov		[ebx],eax
		mov		ebx,protTable
		mov		[ebx],eax
		jmp		_ptrNotFound
_ptrFound:
		mov		ebx,vesaDriverHeader
		lea		ecx,[eax+edx]
		mov		[ebx],ecx
		mov		ebx,protTable
		mov		dx,[eax + edx]VesaDriverHeader.protTable
		add		eax,edx
		mov		[ebx],eax
_ptrNotFound:
	}
}


unsigned long	VESAHandler (VESARegisters *vesaRegisters)
{
unsigned int		function;
unsigned char		mode;
VBEInfoBlock		*vbeInfoBlock;
ModeInfoBlock		*modeInfoBlock;
dgVoodooModeInfo	*modeInfo;
unsigned short		*modeList;
unsigned int		i;
unsigned int		result;
unsigned int		isVBE2;
unsigned short		vbeMode;
unsigned int		*srcPalEntry, *dstPalEntry;
unsigned int		palEntryMask;
StateBuffer			*stateBuffer;
unsigned char		*protTable;
VesaDriverHeader	*vesaDriverHeader;
unsigned int		*palettePtr;
	
	//LOG(1,"\nVESAHandler: eax: %0p, ebx: %0p, ecx: %0p, edx: %0p, esi: %0p, edi: %0p, es: %0p",
	//	vesaRegisters->eax, vesaRegisters->ebx, vesaRegisters->ecx, vesaRegisters->edx, 
	//	vesaRegisters->esi, vesaRegisters->edi, vesaRegisters->es);
	//_asm jjj: jmp jjj
	
	function = vesaRegisters->eax & 0xFFFF;
	LOG(1,"\nVesahandler begin, function: %0p",function);
	if ((function & 0xFF00) == 0) {
		mode = (config.Flags & CFG_SUPPORTMODE13H) ? 0x12 : 0x13;
		if ((function & 0xFF) <= mode) {
			LOG(1,"\nDe, ráfutott....");
			if (vesaIOHooksInstalled) {
				DOSUninstallIOHook (0x3C0, 0x3DF);
				vesaIOHooksInstalled = 0;
			}
			NTCallFunctionOnServer (VESAUNSETVBEMODE, 0, NULL);
			LOG(1,"\nVesahandler end...");
			return (0);
		}
	}
	switch(function) {

		case 0x4F00:
			vbeInfoBlock = (VBEInfoBlock *) ((vesaRegisters->es << 4) + (vesaRegisters->edi & 0xFFFF));
			isVBE2 = 0;
			if (* (unsigned int *) vbeInfoBlock == '2EBV') {
				ZeroMemory(vbeInfoBlock, sizeof(VBEInfoBlock));
				CopyMemory(&vbeInfoBlock->oemData, vbeOemNameStr, sizeof(vbeOemNameStr));
				CopyMemory(&vbeInfoBlock->oemData + sizeof(vbeOemNameStr), vbeProductNameStr, sizeof(vbeProductNameStr));
				CopyMemory(&vbeInfoBlock->oemData + sizeof(vbeOemNameStr) + sizeof(vbeProductNameStr),
						   vbeRevisionStr, sizeof(vbeRevisionStr));
				isVBE2 = 1;
			}
			vbeInfoBlock->vbeSignature = 'ASEV';
			vbeInfoBlock->vbeVersion = 0x0200;
			vbeInfoBlock->capabilities = 0x1;
			vbeInfoBlock->totalMemory = (config.VideoMemSize << 10) >> 16;

			modeList = (unsigned short *) &vbeInfoBlock->reserved;
			for(i = 0; vesaModeList[i].ModeNumber != 0xFFFF; i++) {
				*modeList = vesaModeList[i].ModeNumber & 0x7FFF;
				if (((unsigned int) vesaModeList[i].XRes * (unsigned int) vesaModeList[i].YRes *
					 vesaModeList[i].BitPP / 8) <= (config.VideoMemSize << 10) ) {
					modeList++;
					vesaModeList[i].ModeNumber |= 0x8000;
				}
			}
			*(modeList++) = 0xFFFF;
			vbeInfoBlock->videoModePtr = GetRealModeFarPtr (&vbeInfoBlock->reserved);
			if (isVBE2) {
				vbeInfoBlock->oemStringPtr = vbeInfoBlock->oemVendorNamePtr = 
					GetRealModeFarPtr (&vbeInfoBlock->oemData);
				vbeInfoBlock->oemProductNamePtr = GetRealModeFarPtr (&vbeInfoBlock->oemData + sizeof(vbeOemNameStr));
				vbeInfoBlock->oemProductRevPtr = GetRealModeFarPtr (&vbeInfoBlock->oemData + sizeof(vbeOemNameStr) + sizeof(vbeProductNameStr));
				vbeInfoBlock->oemSoftwareRev = 0x100;
			} else {
				vbeInfoBlock->oemStringPtr = GetRealModeFarPtr ((unsigned char *) modeList);
				CopyMemory((unsigned char *) modeList, vbeOemNameStr, sizeof(vbeOemNameStr));
			}

			vesaRegisters->eax = 0x004F;

			LOG(1,"\nVesahandler end...");
			return(1);
			break;
		
		case 0x4F01:
			vesaRegisters->eax = 0x014F;
			if ( (modeInfo = VBESearchInVideoModeList ((unsigned short) vesaRegisters->ecx & 0xFFFF)) != NULL) {
				modeInfoBlock = (ModeInfoBlock *) ((vesaRegisters->es << 4) + (vesaRegisters->edi & 0xFFFF));
				modeInfoBlock->ModeAttributes = 0x1B;
				modeInfoBlock->WinAAttributes = 0x7;
				modeInfoBlock->WinBAttributes = 0x0;
				modeInfoBlock->WinGranularity = 64;
				modeInfoBlock->WinSize = 64;
				modeInfoBlock->WinASegment = 0xA000;
				modeInfoBlock->WinBSegment = 0x0;
				modeInfoBlock->WinFuncPtr = 0x0;
				LOG(1,"\nx: %d, y:%d, bpp:%d",modeInfo->XRes,modeInfo->YRes,modeInfo->BitPP);
				modeInfoBlock->XResolution = modeInfo->XRes;
				modeInfoBlock->BitsPerPixel = modeInfo->BitPP;
				modeInfoBlock->BytesPerScanLineVESA = ((unsigned int ) modeInfo->XRes) * modeInfo->BitPP / 8;
				modeInfoBlock->YResolution = modeInfo->YRes;
				modeInfoBlock->NumberOfImagePages = (config.VideoMemSize << 10) / ((unsigned int) modeInfoBlock->BytesPerScanLineVESA * modeInfo->YRes);
				modeInfoBlock->XCharSize = modeInfoBlock->YCharSize = 0;
				modeInfoBlock->NumberOfPlanes = 1;
				modeInfoBlock->MemoryModel = modeInfo->BitPP == 8 ? 4 : 6;
				modeInfoBlock->NumberOfBanks = 1;
				modeInfoBlock->BankSize = 0;
				modeInfoBlock->MIReserved = 1;

				if ((config.Flags & CFG_WINDOWED) && modeInfo->BitPP != 8) {
					if (modeInfo->BitPP == actVideoMode.BitPP)
						modeInfo = &actVideoMode;
					else
						modeInfo = modeInfo->BitPP == 16 ? &const16BitFormat : &const32BitFormat;
				}
				modeInfoBlock->RedFieldPosition = modeInfo->RedPos;
				modeInfoBlock->RedMaskSize = modeInfo->RedSize;
				modeInfoBlock->GreenFieldPosition = modeInfo->GreenPos;
				modeInfoBlock->GreenMaskSize = modeInfo->GreenSize;
				modeInfoBlock->BlueFieldPosition = modeInfo->BluePos;
				modeInfoBlock->BlueMaskSize = modeInfo->BlueSize;
				modeInfoBlock->RsvdFieldPosition = modeInfo->RsvPos;
				modeInfoBlock->RsvdMaskSize = modeInfo->RsvSize;
				modeInfoBlock->DirectColorModeInfo = 0x2;
				modeInfoBlock->PhysBasePtr = 0x0;
				modeInfoBlock->OffScreenMemOffset = 0x0;
				modeInfoBlock->OffScreenMemSize = 0x0;

				vesaRegisters->eax = 0x004F;
			}
			break;
		
		case 0x13:
			vbeMode = (unsigned int) (vesaRegisters->al & 0x7F) | (((unsigned int) (vesaRegisters->al & 0x80)) << 8);

		case 0x4F02:
			if (function == 0x4F02) {
				vesaRegisters->eax = 0x014F;
				vbeMode = (unsigned short) vesaRegisters->ebx & 0xFFFF;
			}
			/*if (vesaIOHooksInstalled) {
				DOSUninstallIOHook (0x3C0, 0x3DF);
				vesaIOHooksInstalled = 0;
			}*/
			LOG(1,"\nSet VBE Mode %0p", vbeMode);
			if (!(vbeMode & 0x4000)) {
				if (!(vbeMode & 0x8000))
					ZeroMemory (vca->videoMemoryClient, config.VideoMemSize * 1024);
				vbeMode &= 0x3FFF;
				if (VBESearchInVideoModeList (vbeMode) != NULL) {
					vca->kernelflag |= KFLAG_VESAFRESHDISABLED;
					
					SendMessage(serverCmdHwnd, DGSM_GETVESAMODEFORMAT, (WPARAM) vbeMode, 0);
					
					vca->actmodeinfo.ModeNumber = vbeMode;
					vca->DisplayOffset = 0;
					vca->BytesPerScanLine = vca->actmodeinfo.XRes * vca->actmodeinfo.BitPP / 8;
					vesaActBank = 0;
					if (!vca->VESASession)
						InitializeCriticalSection(&critSection);
					result = NTCallFunctionOnServer (VESASETVBEMODE, 0, NULL);
					if (function == 0x4F02)
						vesaRegisters->eax = result;
					if (result == 0x004F && !vesaIOHooksInstalled ) {
						DOSInstallIOHook (0x3C0, 0x3DF, VESAPortInHandler, VESAPortOutHandler);
						vesaIOHooksInstalled = 1;
					}

					vgaPalRGBIndexW = vgaPalRGBIndexR = vgaPalEntryIndexW = vgaPalEntryIndexR = 0;
				}
			}
			
			LOG(1,"\n   Ret code: %0p", vesaRegisters->eax);
			return ((NTCallFunctionOnServer (VESAGETXRES, 0, NULL) << 16) + NTCallFunctionOnServer (VESAGETYRES, 0, NULL));
			break;
		
		case 0x4F03:
			vesaRegisters->bx = vca->actmodeinfo.ModeNumber;
			vesaRegisters->eax = 0x004F;
			break;
		
		case 0x4F04:
			vesaRegisters->eax = 0x004F;
			stateBuffer = (StateBuffer *) ((((unsigned int) vesaRegisters->es) << 4) + (unsigned int) vesaRegisters->bx);
			switch(vesaRegisters->dl) {
				case 0:
					vesaRegisters->bx = (sizeof(StateBuffer) + 0x3F) >> 6;
					break;
				
				case 1:
					CopyMemory(stateBuffer->palette, vca->VESAPalette, 256 * 4);
					stateBuffer->displayOffset = vca->DisplayOffset;
					stateBuffer->bytesPerScanline = vca->BytesPerScanLine;
					stateBuffer->vgaRAMDACSize = vca->VGARAMDACSize;
					break;
				
				case 2:
					CopyMemory(vca->VESAPalette, stateBuffer->palette, 256 * 4);
					vca->DisplayOffset = stateBuffer->displayOffset;
					vca->BytesPerScanLine = stateBuffer->bytesPerScanline;
					vca->VGARAMDACSize = stateBuffer->vgaRAMDACSize;
					break;
				
				default:
					vesaRegisters->eax = 0x014F;
					break;
			}
			break;
		
		case 0x4F05:
			LOG(1,"\n bx=%0p, dx=%0p",vesaRegisters->bx,vesaRegisters->dx);
			vesaRegisters->eax = 0x034F;
			if (vca->actmodeinfo.ModeNumber != 0x13) {
				vesaRegisters->eax = 0x014F;
				if (vesaRegisters->bl == 0) {
					if (vesaRegisters->bh) {
						if (vesaRegisters->bh == 1) {
							vesaRegisters->dx = vesaActBank;
							vesaRegisters->eax = 0x004F;
						}
					} else {
						if ( (((unsigned int) vesaRegisters->dx) * 0x10000) < (config.VideoMemSize << 10) ) {
							if (vca->VESASession) {
								EnterCriticalSection(&critSection);
								CopyA000ToBank (vesaActBank);
								vesaActBank = vesaRegisters->dx;
								CopyBankToA000 (vesaActBank);
								LeaveCriticalSection(&critSection);
							}
							vesaRegisters->eax = 0x004F;
						}
					}
				}
			}
			LOG(1,"\n ret=%0p",vesaRegisters->ax);
			break;
		
		// Set/Get Logical Line Length
		case 0x4F06:
			i = (unsigned int) vesaRegisters->cx;
			vesaRegisters->eax = 0x024F;
			switch(vesaRegisters->bl) {
				//Set in pixels
				case 0:
					if (i < vca->actmodeinfo.XRes)
						break;
					i = (i * vca->actmodeinfo.BitPP) / 8;
																//no break
				//Get in bytes
				case 1:
					if (vesaRegisters->bl == 1)
						i = vca->BytesPerScanLine;
																//no break
				//Set in bytes
				case 2:
					i = (i + 3) & ~3;
					if ((result = (config.VideoMemSize << 10) / i) >= vca->actmodeinfo.YRes) {
						vesaRegisters->bx = i;
						vesaRegisters->dx = result;
						vesaRegisters->eax = 0x004F;
						vca->BytesPerScanLine = i;
					}
					break;
					
				//Get maximum
				case 3:
					i = vesaRegisters->bx = (config.VideoMemSize << 10) / (vesaRegisters->dx = vca->actmodeinfo.YRes);
					vesaRegisters->eax = 0x004F;
					break;

				default:
					vesaRegisters->eax = 0x014F;
					break;
					
			}
			if (vesaRegisters->eax == 0x004F)
				vesaRegisters->cx = (i * 8) / vca->actmodeinfo.BitPP;
			
			break;
		
		case 0x4F07:
			vesaRegisters->eax = 0x014F;
			switch(vesaRegisters->bl) {
				case 0:
				case 0x80:
					vesaRegisters->eax = 0x024F;
					i = ((unsigned int) vesaRegisters->dx) * vca->BytesPerScanLine +
						((unsigned int) vesaRegisters->cx) * vca->actmodeinfo.BitPP / 8;
					if ( (vca->actmodeinfo.XRes * vca->actmodeinfo.YRes * vca->actmodeinfo.BitPP / 8 + i) <= (config.VideoMemSize << 10) ) {
						vca->DisplayOffset = i;
						vesaRegisters->eax = 0x004F;
					}
					break;

				case 1:
					if (vca->BytesPerScanLine == 0) {
						DEBUGLOG(0,"\n   Error: VESAHandler: VESA function 0x4F07: BytesPerScanline == 0 !");
						DEBUGLOG(0,"\n   Hiba: VESAHandler: VESA-funkció 0x4F07: BytesPerScanline == 0 !");
						break;
					}
					if (vca->actmodeinfo.BitPP == 0) {
						DEBUGLOG(1, "\n   Error: VESAHandler: VESA function 0x4F07: actmodeinfo.bitpp == 0 !");
						DEBUGLOG(1, "\n   Error: VESAHandler: VESA-funkció 0x4F07: actmodeinfo.bitpp == 0 !");
						break;
					}
					vesaRegisters->dx = vca->DisplayOffset / (vca->BytesPerScanLine);
					vesaRegisters->cx = ((vca->DisplayOffset % (vca->BytesPerScanLine)) * 8) / vca->actmodeinfo.BitPP;
					vesaRegisters->bh = 0;
					vesaRegisters->eax = 0x004F;
					break;
				
				default:
					break;
			}
			break;
		
		case 0x4F08:
			if (vca->VESASession && (vca->actmodeinfo.BitPP != 8)) {
				vesaRegisters->eax = 0x034F;
				break;
			}
			vesaRegisters->eax = 0x014F;
			switch(vesaRegisters->bl) {
				case 0:
					if (vesaRegisters->bh >= 8)
						vca->VGARAMDACSize = 8;
					else if (vesaRegisters->bh >= 6)
						vca->VGARAMDACSize = 6;
					else break;
					
				case 1:
					vesaRegisters->bh = vca->VGARAMDACSize;
					vesaRegisters->eax = 0x004F;
					break;
				default:
					break;
			}
			break;
		
		case 0x4F0B:
			palettePtr = DosMapFlat ((unsigned short) vesaRegisters->si, ((unsigned long) vesaRegisters->dx << 16) + (unsigned long) vesaRegisters->di);
			vesaRegisters->dx = (unsigned short) vesaRegisters->bh;

		case 0x4F09:
			if (function == 0x4F09)
				palettePtr = (unsigned int *) (((unsigned long) vesaRegisters->es << 4) + (unsigned long) vesaRegisters->di);
			
			function = vesaRegisters->bl & 0x7F;
			vesaRegisters->eax = 0x014F;
			
			if (function <= 3) {
				if (function <= 1) {
					if ((vesaRegisters->cx <= 0x100) && (vesaRegisters->dx < 0x100) &&
						((vesaRegisters->dx + vesaRegisters->cx) <= 0x100)) {
						palEntryMask = 0xFFFFFF;
						i = 0;
						if (function) {
							dstPalEntry = palettePtr;
							dstPalEntry += vesaRegisters->dx;
							srcPalEntry = &vca->VESAPalette[vesaRegisters->dx];
							if (vca->VGARAMDACSize == 6) {
								palEntryMask = 0xFCFCFC;
								i = 2;
							}
						} else {
							srcPalEntry = palettePtr;
							srcPalEntry += vesaRegisters->dx;
							dstPalEntry = &vca->VESAPalette[vesaRegisters->dx];
							if (vca->VGARAMDACSize == 6) {
								palEntryMask = 0x3F3F3F;
								i = 32 - 2;
							}
						}
						_asm {
							mov		edx,vesaRegisters
							movzx	edx,[edx]vesaRegisters.cx
							mov		ecx,i
							mov		ebx,palEntryMask
							mov		esi,srcPalEntry
							mov		edi,dstPalEntry
						convertNextPalentry:
							mov		eax,[esi + 4*edx - 4]
							and		eax,ebx
							ror		eax,cl
							mov		[edi + 4*edx - 4],eax
							dec		edx
							jne		convertNextPalentry
						}
						vesaRegisters->eax = 0x004F;
						vca->kernelflag |= KFLAG_PALETTECHANGED;
					}
					
				} else {
					vesaRegisters->eax = 0x024F;
				}
			}
			LOG(1,"\n0x4f09 called..");
			break;

		case 0x4F0A:
			vesaRegisters->eax = 0x014F;
			if (vesaRegisters->bl == 0) {
				GetProtTablePtr (&vesaDriverHeader, &protTable);
				if (vesaDriverHeader != NULL) {
					vesaRegisters->es = (((unsigned long) protTable) >> 4);
					vesaRegisters->di = (unsigned short) (((unsigned long) protTable) & 0xF);
					vesaRegisters->cx = vesaDriverHeader->protTableLen;
					vesaRegisters->eax = 0x004F;
				} else {
					vesaRegisters->eax = 0x024F;
				}
			}
			break;
		
		default:
			LOG(1,"\nVesahandler end...");
			return(0);
	}
	LOG(1,"\nVesahandler end...");
	return(1);
}


int		VESAInit (void *driverHeader)
{
	
	if (!(config.Flags & CFG_VESAENABLE))
		return (2);
	
	ZeroMemory(&movie, sizeof(MOVIEDATA));
	//vesaDriverHeader = (_VesaDriverHeader *) driverHeader;
	
	if (config.Flags & CFG_NTVDDMODE)	{
		if ((c->VideoMemory = c->videoMemoryClient = _GetMem(config.VideoMemSize * 1024)) == NULL) {
			return (1);
		}
		if (!InitVESA ()) {
			_FreeMem (c->VideoMemory);
			return(1);
		}
	} else {
		vca = c;
	}

	return(0);

}


unsigned int	VESAGetXRes ()
{
	return movie.cmiXres;
}


unsigned int	VESAGetYRes ()
{
	return movie.cmiYres;
}


#endif /* VESA_SUPPORT */

#endif /* !GLIDE3 */