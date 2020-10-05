/*--------------------------------------------------------------------------------- */
/* Vesa.cpp - dgVoodoo VESA rendering implementation                                */
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

extern "C" {

#include "Dgw.h"
#include "Version.h"
#include "Main.h"
#include "Debug.h"
#include "Dos.h"
#include "VxdComm.h"
#include "RMDriver.h"
#include "VesaDefs.h"
#include "dgVoodooConfig.h"
#include "MMConv.h"
#include "dgVoodooGlide.h"
#include "Grabber.h"
#include "Texture.h"
#include "LfbDepth.h"
#include "Draw.h"
#include "Vesa.h"
#include "Log.h"

}

#include	"Renderer.hpp"
#include	"RendererGeneral.hpp"
#include	"RendererLfbDepth.hpp"

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
int					VESAModeWillBeSet = 0;
CRITICAL_SECTION	critSection;
_pixfmt				vesaLfbFormat;
RendererLfbDepth::LfbUsage lfbUsage;
unsigned int		*convertedPalette;
unsigned int		vesaActBank;
unsigned int		vgaPalRGBIndexW, vgaPalRGBIndexR, vgaPalEntryIndexW, vgaPalEntryIndexR;
unsigned int		vgaPalReadLatch, vgaPalWriteLatch;
int					vesaIOHooksInstalled = 0;

/*unsigned*/ char		vm320x200[] = "320x200";
/*unsigned*/ char		vm640x400[] = "640x400";
/*unsigned*/ char		vm640x480[] = "640x480";
/*unsigned*/ char		vm800x600[] = "800x600";
/*unsigned*/ char		vm1024x768[] = "1024x768";
/*unsigned*/ char		vm1280x1024[] = "1280x1024";
/*unsigned*/ char		vm256Color[] = " 256 Color";
/*unsigned*/ char		vmHiColor[] = " HiColor";
/*unsigned*/ char		vmTrueColor[] = " TrueColor";

unsigned char		vbeOemNameStr[]		= "dgVoodoo";
unsigned char		vbeProductNameStr[]	= "dgVoodoo VESA Emulation";
unsigned char		vbeRevisionStr[]	= "v1.2";
dgVoodooModeInfo	vesaModeList[] = {  {-1, 0x13, 320, 200, 8},	{-1, 0x100, 640, 400, 8},	{-1, 0x101, 640, 480, 8},
										{-1, 0x103, 800, 600, 8},	{-1, 0x105, 1024, 768, 8},	{-1, 0x107, 1280, 1024, 8},
										{-1, 0x10e, 320, 200, 16},	{-1, 0x10f, 320, 200, 32},	{-1, 0x111, 640, 480, 16},
										{-1, 0x112, 640, 480, 32},	{-1, 0x114, 800, 600, 16},	{-1, 0x115, 800, 600, 32},
										{-1, 0x117, 1024, 768, 16}, {-1, 0x118, 1024, 768, 32}, {-1, 0x11a, 1280, 1024, 16},
										{-1, 0x11b, 1280, 1024, 32}, {-1, (unsigned short) 0xFFFF} };
dgVoodooModeInfo	actVideoMode = {-1, (unsigned short) 0xEEEE};
dgVoodooModeInfo	const16BitFormat = {-1, 0x0, 0, 0, 16, 5, 11, 6, 5, 5, 0, 0, 0};
dgVoodooModeInfo	const32BitFormat = {-1, 0x0, 0, 0, 32, 8, 24, 8, 16, 8, 8, 8, 0};
RendererGeneral::DisplayMode*	supportedModes;
int					numOfSupportedModes;

int					vesaFreshThreadDebugId;

/*------------------------------------------------------------------------------------------*/
/*...................................... Predeklaráció  ....................................*/


static	dgVoodooModeInfo*	VBESearchInVideoModeList (unsigned short modeNumber);
static	DWORD WINAPI		VesaRefreshThread (LPVOID);

/*------------------------------------------------------------------------------------------*/
/*................ Functions related to maintaining VESA mode table (server) ...............*/


static void			ConvertToModeInfo (RendererGeneral::DisplayMode* displayMode, dgVoodooModeInfo *lpmi)
{
	ZeroMemory(lpmi, sizeof(dgVoodooModeInfo));
	lpmi->XRes = (short) displayMode->xRes;
	lpmi->YRes = (short) displayMode->yRes;
	if ( (lpmi->BitPP = (unsigned char) displayMode->pixFmt.BitPerPixel/*bitDepth*/) >= 16)	{
		lpmi->RedSize = displayMode->pixFmt.RBitCount;
		lpmi->RedPos = displayMode->pixFmt.RPos;
		lpmi->GreenSize = displayMode->pixFmt.GBitCount;
		lpmi->GreenPos = displayMode->pixFmt.GPos;
		lpmi->BlueSize = displayMode->pixFmt.BBitCount;
		lpmi->BluePos = displayMode->pixFmt.BPos;
		lpmi->RsvSize = displayMode->pixFmt.ABitCount;
		lpmi->RsvPos = displayMode->pixFmt.APos;
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


void				VESAModeRegisteringCallback (struct RendererGeneral::DisplayMode* displayMode)
{
	switch(platform) {
		case PLATFORM_DOSWIN9X:
		{
			dgVoodooModeInfo info;
			ConvertToModeInfo (displayMode, &info);
			DeviceIoControl(hDevice, DGSERVER_REGISTERMODE ,&info, 0, NULL, 0, NULL, NULL);
			break;
		}

		case PLATFORM_DOSWINNT:		
			//if (displayMode->xRes < 800 /*|| displayMode->pixFmt.BitPerPixel <= 8*/)
			//	break;
			if (numOfSupportedModes < 1024) {
				supportedModes[numOfSupportedModes++] = *displayMode;
			}
			break;
	}
}


int					InitVESA()	{
RendererGeneral::DisplayMode	displayMode;
dgVoodooModeInfo*				pVesaModeInfo;
	
	if ((supportedModes = (RendererGeneral::DisplayMode*) _GetMem (1024*sizeof (RendererGeneral::DisplayMode))) == NULL)
		return (0);

	numOfSupportedModes = 0;

	if (config.Flags & CFG_WINDOWED)	{
		rendererGeneral->GetCurrentDisplayMode (config.dispdev, config.dispdriver, &displayMode);
		ConvertToModeInfo (&displayMode, &actVideoMode);

		switch(platform) {
			case PLATFORM_DOSWIN9X:
				DeviceIoControl(hDevice, DGSERVER_REGISTERACTMODE, &actVideoMode, 0, NULL, 0, NULL, NULL);
				break;
			case PLATFORM_DOSWINNT:
				break;
		}
	}
	InitializeCriticalSection(&critSection);
	rendererGeneral->EnumDisplayModes (config.dispdev, config.dispdriver, 0, VESAModeRegisteringCallback);

	if (platform != PLATFORM_DOSWIN9X) {

		//ConvertToModeInfo (displayMode, &info);
		for (pVesaModeInfo = vesaModeList; pVesaModeInfo->ModeNumber != 0xFFFF; pVesaModeInfo++) {
			int i;
			for (i = 0; i < numOfSupportedModes; i++) {
				if ((pVesaModeInfo->XRes <= supportedModes[i].xRes) && (pVesaModeInfo->YRes <= supportedModes[i].yRes) &&
					((pVesaModeInfo->modeIndex == -1) ||
					((supportedModes[i].xRes <= supportedModes[pVesaModeInfo->modeIndex].xRes) && (supportedModes[i].yRes <= supportedModes[pVesaModeInfo->modeIndex].yRes)))) {
					pVesaModeInfo->modeIndex = i;
				}
			}
			for (i = 0; i < numOfSupportedModes; i++) {
				if ((supportedModes[i].xRes == supportedModes[pVesaModeInfo->modeIndex].xRes) &&
					(supportedModes[i].yRes == supportedModes[pVesaModeInfo->modeIndex].yRes)) {
					if ((pVesaModeInfo->BitPP <= supportedModes[i].pixFmt.BitPerPixel) &&
						(supportedModes[i].pixFmt.BitPerPixel <= supportedModes[pVesaModeInfo->modeIndex].pixFmt.BitPerPixel)) {
						pVesaModeInfo->modeIndex = i;
					}
				}
			}
			for (i = 0; i < numOfSupportedModes; i++) {
				if ((supportedModes[i].xRes == supportedModes[pVesaModeInfo->modeIndex].xRes) &&
					(supportedModes[i].yRes == supportedModes[pVesaModeInfo->modeIndex].yRes) &&
					(supportedModes[i].pixFmt.BitPerPixel == supportedModes[pVesaModeInfo->modeIndex].pixFmt.BitPerPixel)) {
					if ((config.VideoRefreshFreq <= supportedModes[i].refreshRate) &&
						(supportedModes[i].refreshRate <= supportedModes[pVesaModeInfo->modeIndex].refreshRate)) {
						pVesaModeInfo->modeIndex = i;
					}
				}
			}
			if (pVesaModeInfo->BitPP != 8) {
				RendererGeneral::DisplayMode* displayMode = &supportedModes[pVesaModeInfo->modeIndex];
				pVesaModeInfo->RedSize = displayMode->pixFmt.RBitCount;
				pVesaModeInfo->GreenSize = displayMode->pixFmt.GBitCount;
				pVesaModeInfo->BlueSize = displayMode->pixFmt.BBitCount;
				pVesaModeInfo->RedPos = displayMode->pixFmt.RPos;
				pVesaModeInfo->GreenPos = displayMode->pixFmt.GPos;
				pVesaModeInfo->BluePos = displayMode->pixFmt.BPos;
			}
		}
	}
	
	vca = c;
	return(1);
}


void				CleanupVESA()	{

	DeleteCriticalSection(&critSection);
	if (supportedModes != NULL) {
		_FreeMem (supportedModes);
		supportedModes = NULL;
	}
}


/*------------------------------------------------------------------------------------------*/
/*...................... Vesa mode setting/cleanup functions (server) ......................*/


unsigned int	VesaSetVBEMode()	{
/*unsigned*/ char	*temp;
char			windowname[256];
unsigned int	flags, tempui, init;
unsigned int	i;
	
	DEBUG_BEGINSCOPE("VesaSetVBEMode", DebugRenderThreadId);

	if (runflags & RF_INITOK) {
		VESAModeWillBeSet = 1;
		return(0x004F);
	}
	VESAModeWillBeSet = 0;
	if (vca->VESASession)	{
		VesaUnsetVBEMode(0);
		init = 0;
	} else	{
		CreateRenderingWindow();

		rendererGeneral->PrepareForInit (renderingWinHwnd);

		init = 1;
		if (!rendererGeneral->InitRendererApi ())
			return(0x014F);
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
	flags = ((config.Flags & CFG_WINDOWED) ? SWM_WINDOWED : 0);
	vesaInFresh = 0;
		
	RendererGeneral::DisplayMode displayMode = supportedModes[vca->actmodeinfo.modeIndex];
	if (config.Flags & CFG_WINDOWED) {
		displayMode.xRes = vca->actmodeinfo.XRes;
		displayMode.yRes = vca->actmodeinfo.YRes;
	}
	
	if ( (displayMode.pixFmt.BitPerPixel != 8) || (config.Flags & CFG_WINDOWED) ) {
		flags |= SWM_3D;
	}
	
	windowname[0] = 0;
	strcat(windowname, productName);
	if (vca->progname[0])	{
		strcat(windowname, " - ");
		strcat(windowname, vca->progname + 1);
	}
	strcat(windowname," (");
	if ((vca->actmodeinfo.ModeNumber & 0x3FFF) == 0x13) strcat(windowname,"VGA ");
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

	flags |= SWM_MULTITHREADED;

	int	initSuccess = 0;
	if (init)	{
		for (i = 3; i > 0; i--) {
			if (initSuccess = rendererGeneral->SetWorkingMode (displayMode.xRes, displayMode.yRes,
															   displayMode.pixFmt.BitPerPixel, displayMode.refreshRate, 2,
															   renderingWinHwnd, config.dispdev, config.dispdriver, flags)) {
				break;
			}
		}
	} else {
		initSuccess = rendererGeneral->ModifyWorkingMode (displayMode.xRes, displayMode.yRes,
														  displayMode.pixFmt.BitPerPixel, displayMode.refreshRate, 2, flags);
	}
	
	movie.cmiXres = displayMode.xRes;
	movie.cmiYres = displayMode.yRes;
	movie.cmiBuffNum = 2;
	appXRes = convbuffxres = vca->actmodeinfo.XRes;
	appYRes = vca->actmodeinfo.YRes;

	if (initSuccess) {
		lfbUsage = RendererLfbDepth::ColorBuffersOnly;
		if ((vca->actmodeinfo.XRes != displayMode.xRes) || (vca->actmodeinfo.YRes != displayMode.yRes)) {
			lfbUsage = RendererLfbDepth::OneScalingBuffer;
		}
		if (initSuccess = rendererLfbDepth->InitLfb (lfbUsage, displayMode.pixFmt.BitPerPixel != 8, 0)) {
			rendererLfbDepth->GetLfbFormat (&backBufferFormat);

			movie.cmiBitPP = backBufferFormat.BitPerPixel;
			movie.cmiByPP = movie.cmiBitPP / 8;
			
			if (movie.cmiBitPP == 8)	{
				initSuccess = rendererLfbDepth->CreatePrimaryPalette ();

			} else {
				
				if ((convertedPalette = (unsigned int *) _GetMem(256 * 4)) == NULL) {
					initSuccess = 0;
				}

				if (((config.Flags & CFG_GRABENABLE) || rendererGeneral->Is3DNeeded ()) && initSuccess)	{
					initSuccess = 0;
					if (DrawInit ()) {
						if (TexInit())	{
							initSuccess = GrabberInit(0);
							if (!initSuccess)
								TexCleanUp();
						}
						if (!initSuccess)
							DrawCleanUp ();
					}
				}
			}
			if (!initSuccess)
				rendererLfbDepth->DeInitLfb ();
		}
	}
	
	vca->VESASession = initSuccess;
	
	if (!initSuccess) {
		if (convertedPalette != NULL) {
			_FreeMem (convertedPalette);
			convertedPalette = NULL;
		}

		DestroyRenderingWindow ();
		rendererGeneral->UninitRendererApi ();
		return(0x014F);
	}
	
	if ((actResolution != tempui) && (config.Flags & CFG_WINDOWED)) {
		i = GetIdealWindowSizeForResolution (vca->actmodeinfo.XRes, vca->actmodeinfo.YRes); //??
		if (IsWindowSizeIsEqualToResolution (renderingWinHwnd)) {	
			SetWindowClientArea(renderingWinHwnd, i >> 16, i & 0xFFFF);
		} else {
			AdjustAspectRatio (renderingWinHwnd, i >> 16, i & 0xFFFF);
		}
	}
	
	actResolution = tempui;
	
	ShowWindow(warningBoxHwnd, SW_HIDE);
	/*if ((!(config.Flags & CFG_WINDOWED)) && IsZoomed (renderingWinHwnd)) {
		ShowWindow (renderingWinHwnd, SW_SHOWNORMAL);
	}*/
	ShowWindow (renderingWinHwnd, (config.Flags & CFG_WINDOWED) ? SW_SHOWNORMAL : SW_SHOWMAXIMIZED);
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
	SetForegroundWindow (renderingWinHwnd);	
	HideMouseCursor(0);
	//if (config.Flags & CFG_WINDOWED) {
		switch(platform) {
			case PLATFORM_DOSWIN9X:
				DeviceIoControl(hDevice, DGSERVER_SETFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
				break;
		}
	//}
	EnterCriticalSection(&critSection);


	rendererLfbDepth->ClearBuffers ();

	LeaveCriticalSection(&critSection);

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(0x004F);	
}


/* flag jelentései (csak belsôleg használt):	*/
/*	0	-	ne adja vissza a fókuszt a VM-re, mert egy újabb VESA-mód lesz beállítva */
/*	1	-	visszaadja a fókuszt, megjeleníti a konzolt és a warning-boxot, eltünteti a	*/
/*			kép-ablakot	*/
/*  2   -   */
void		VesaUnsetVBEMode(int flag)	{

	DEBUG_BEGINSCOPE("VesaUnsetVBEMode", DebugRenderThreadId);

	VESAModeWillBeSet = 0;
	if (runflags & RF_INITOK) return;
	if (!vca->VESASession) return;

	//LeaveCriticalSection(&critSection);

	vca->VESASession = 0;
	SetEvent(refreshTimerEventHandle);

	while (MsgWaitForMultipleObjects (1, &vesaRefreshThreadHandle, FALSE, INFINITE, QS_SENDMESSAGE) != (WAIT_OBJECT_0+0)) {
		MSG msg;
		PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE | PM_QS_SENDMESSAGE);
	}

	//WaitForSingleObject (vesaRefreshThreadHandle, INFINITE);
	CloseHandle (vesaRefreshThreadHandle);
	
	InitializeCriticalSection(&critSection);

	//EnterCriticalSection(&critSection);
	
	if (movie.cmiBitPP == 8)
		rendererLfbDepth->DestroyPrimaryPalette ();
	
	rendererLfbDepth->DeInitLfb ();

	if ( ((config.Flags & CFG_GRABENABLE) || rendererGeneral->Is3DNeeded ()) && (movie.cmiBitPP != 8) )	{
		GrabberCleanUp();
		TexCleanUp();
		DrawCleanUp ();
	}
	if (flag)	{
		rendererGeneral->RestoreWorkingMode ();
		rendererGeneral->UninitRendererApi ();
		RestoreMouseCursor(0);
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
					SetActiveWindow(consoleHwnd);
					SetForegroundWindow(consoleHwnd);
					ShowWindow(consoleHwnd, SW_MAXIMIZE);
					break;
			}
		}
	}

	if (convertedPalette != NULL) _FreeMem(convertedPalette);
	convertedPalette = NULL;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


unsigned int	VESAGetXRes ()
{
	return movie.cmiXres;
}


unsigned int	VESAGetYRes ()
{
	return movie.cmiYres;
}


/*------------------------------------------------------------------------------------------*/
/*.......................... Vesa screen refresh functions (server) ........................*/


void static	VESAResetDevice ()
{
	rendererGeneral->PrepareForDeviceReset ();
	if (!rendererGeneral->DeviceReset ())
		return;
	if (!rendererGeneral->ReallocResources ())
		return;
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
void*					buffPtr;
unsigned int			buffPitch;
int						convType;

	if ((vca->actmodeinfo.BitPP == 8) && (vca->kernelflag & KFLAG_PALETTECHANGED)) {
		if (movie.cmiBitPP == 8) {
			MMCConvARGBPaletteToABGR (vca->VESAPalette, (unsigned int *) dPalette);
			rendererLfbDepth->SetPrimaryPaletteEntries (dPalette);
		} else {
			MMCConvARGBPaletteToPixFmt(vca->VESAPalette, convertedPalette, 0, 256, &backBufferFormat);
		}
		vca->kernelflag &= ~KFLAG_PALETTECHANGED;
	}

	RendererLfbDepth::BufferType bufferType = (lfbUsage == RendererLfbDepth::OneScalingBuffer) ?
											  RendererLfbDepth::ScalingBuffer : RendererLfbDepth::ColorBuffer;
	
	if (!rendererLfbDepth->LockBuffer (1, bufferType, &buffPtr, &buffPitch))
		return;

	convType = MMCIsPixfmtTheSame(&vesaLfbFormat, &backBufferFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY;
	MMCConvertAndTransferData(&vesaLfbFormat, &backBufferFormat,
							  0, 0, 0,
							  ((char *)vca->VideoMemory + vca->DisplayOffset), buffPtr,
							  vca->actmodeinfo.XRes, vca->actmodeinfo.YRes,
							  (config.Flags2 & CFG_YMIRROR ? -1 : 1) * vca->BytesPerScanLine,
							  buffPitch,
							  convertedPalette, NULL,
							  convType, PALETTETYPE_CONVERTED);

	rendererLfbDepth->UnlockBuffer (1, bufferType);

	if (lfbUsage == RendererLfbDepth::OneScalingBuffer) {	
		rendererLfbDepth->BlitFromScalingBuffer (1, NULL, NULL, RendererLfbDepth::Copy, 0, 1);
	}
	
	if (config.Flags & CFG_GRABENABLE)	{
		if (vca->kernelflag & KFLAG_SCREENSHOT)	{
 			if (movie.cmiBitPP != 8)	{
				GrabberHookDos(1);
			} else {
				GrabberGrabLFB(1, NULL, dPalette);
			}
			vca->kernelflag &= ~KFLAG_SCREENSHOT;
		}			
		if (movie.cmiBitPP != 8)	{
			rendererGeneral->SetRenderTarget (1);
			rendererGeneral->BeginScene ();
			GrabberDrawLabel(0);
			rendererGeneral->EndScene ();
		}			
	}
}


/* Screen refreshing thread */
static DWORD WINAPI VesaRefreshThread (LPVOID /*lpParameter*/)	{
UINT			timerID;
unsigned int	freq;
int				deviceStatusLost;
	
	DEBUG_BEGINSCOPE("VesaRefreshThread", vesaFreshThreadDebugId);

	DEBUG_THREAD_REGISTER(vesaFreshThreadDebugId);

	if (vesaInFresh) return(1);
	vesaInFresh = 1;
	freq = 1000 / config.VideoRefreshFreq;
	timerID = timeSetEvent(freq, freq, (LPTIMECALLBACK) refreshTimerEventHandle, 0, TIME_PERIODIC | TIME_CALLBACK_EVENT_PULSE);
	deviceStatusLost = 0;
	while(1)	{
		WaitForSingleObject(refreshTimerEventHandle, INFINITE);
		if (!vca->VESASession)	{
			timeKillEvent(timerID);
			//LeaveCriticalSection(&critSection);
			break;
		}

		EnterCriticalSection(&critSection);

		if (platform == PLATFORM_DOSWINNT) {
			if (config.Flags & CFG_NTVDDMODE) {
				CopyA000ToBank (vesaActBank);
			} else {
				//EnterCriticalSection(&critSection);
				SendMessage (clientCmdHwnd, DGCM_VESAGRABACTBANK, 0, 0);
				//LeaveCriticalSection(&critSection);
			}
		}

		if (!(vca->kernelflag & KFLAG_VESAFRESHDISABLED)) {
			RendererGeneral::DeviceStatus deviceStatus;
			SendMessage (serverCmdHwnd, DGSM_GETDEVICESTATUS, (WPARAM) &deviceStatus, 0);

			if (deviceStatus != RendererGeneral::StatusLost) {
				if ((deviceStatus == RendererGeneral::StatusLostCanBeReset) || deviceStatusLost) {
					//DosRendererStatus (1);
					SendMessage (serverCmdHwnd, DGSM_RESETDEVICE, 0, 0);
				} else {
					VesaCopyLFBFrame ();
					rendererGeneral->SwitchToNextBuffer (1, 1);
				}
			} /*else {
				if (!deviceStatusLost)
					DosRendererStatus (0);
			}*/
			deviceStatusLost = (deviceStatus == RendererGeneral::StatusLost);
		}
		/* függôleges visszafutás beállít */
		vca->VGAStateRegister3DA |= 8;
		LeaveCriticalSection(&critSection);
	}
	vesaInFresh = 0;

	DEBUG_THREAD_UNREGISTER(vesaFreshThreadDebugId);

	DEBUG_ENDSCOPE(vesaFreshThreadDebugId);

	return(1);
}


/*------------------------------------------------------------------------------------------*/
/*.............................. Vesa server command handler ...............................*/

LRESULT		VESAServerCommandHandler(HWND /*hwnd*/, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
dgVoodooModeInfo	*modeInfo;

	switch (uMsg) {
		case DGSM_GETVESAMODEFORMAT:
			if ( (modeInfo = VBESearchInVideoModeList ((unsigned short) wParam)) != NULL) {
				vca->vesaModeinfo.XRes = modeInfo->XRes;
				vca->vesaModeinfo.YRes = modeInfo->YRes;
				vca->vesaModeinfo.BitPP = modeInfo->BitPP;
				vca->vesaModeinfo.ModeNumber = modeInfo->ModeNumber;
				vca->vesaModeinfo.modeIndex = modeInfo->modeIndex;

				if ((config.Flags & CFG_WINDOWED) && modeInfo->BitPP != 8) {
					if (modeInfo->BitPP == actVideoMode.BitPP)
						modeInfo = &actVideoMode;
					else
						modeInfo = modeInfo->BitPP == 16 ? &const16BitFormat : &const32BitFormat;
				}
				
				vca->vesaModeinfo.RedPos = modeInfo->RedPos;
				vca->vesaModeinfo.RedSize = modeInfo->RedSize;
				vca->vesaModeinfo.GreenPos = modeInfo->GreenPos;
				vca->vesaModeinfo.GreenSize = modeInfo->GreenSize;
				vca->vesaModeinfo.BluePos = modeInfo->BluePos;
				vca->vesaModeinfo.BlueSize = modeInfo->BlueSize;
				vca->vesaModeinfo.RsvPos = modeInfo->RsvPos;
				vca->vesaModeinfo.RsvSize = modeInfo->RsvSize;
			}
			return(0);

		case DGSM_RESETDEVICE:
			VESAResetDevice ();
			return (0);

		case DGSM_GETDEVICESTATUS:
			{
				RendererGeneral::DeviceStatus* deviceStatusPtr = (RendererGeneral::DeviceStatus*) wParam;
				*deviceStatusPtr = rendererGeneral->GetDeviceStatus ();
			}
			return (0);

		default:
			return(1);
	}
}


/*------------------------------------------------------------------------------------------*/
/*.............................. Vesa client command handler ...............................*/



LRESULT		VESALocalClientCommandHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
	
	switch (uMsg) {
		case DGCM_VESAGRABACTBANK:
			CopyA000ToBank (vesaActBank);
			return(0);
		default:
			return(1);
	}
}


void _stdcall VESAPortInHandler (unsigned short port, unsigned char *data)
{
	//LOG(1,"\nread: %0p",port);
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
	//LOG(1,"\nwrite: %0p, %0p", port, data);
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
//dgVoodooModeInfo	*modeInfo;
unsigned short		*modeList;
unsigned int		i;
unsigned int		result;
unsigned int		isVBE2;
unsigned short		vbeMode = 0;
unsigned int		*srcPalEntry, *dstPalEntry;
unsigned int		palEntryMask;
StateBuffer			*stateBuffer;
unsigned char		*protTable;
VesaDriverHeader	*vesaDriverHeader;
unsigned int		*palettePtr = NULL;
	
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
				*modeList = vesaModeList[i].ModeNumber/* & 0x7FFF*/;
				if (((unsigned int) vesaModeList[i].XRes * (unsigned int) vesaModeList[i].YRes *
					 vesaModeList[i].BitPP / 8) <= (config.VideoMemSize << 10) ) {

					SendMessage (serverCmdHwnd, DGSM_GETVESAMODEFORMAT, (WPARAM) (vesaModeList[i].ModeNumber), 0);
					if (vca->vesaModeinfo.modeIndex != -1) {
						modeList++;
					}
					//vesaModeList[i].ModeNumber |= 0x8000;
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
			if ( (/*modeInfo = */VBESearchInVideoModeList ((unsigned short) vesaRegisters->ecx & 0xFFFF)) != NULL) {
				
				SendMessage (serverCmdHwnd, DGSM_GETVESAMODEFORMAT, (WPARAM) (vesaRegisters->ecx & 0xFFFF), 0);

				modeInfoBlock = (ModeInfoBlock *) ((vesaRegisters->es << 4) + (vesaRegisters->edi & 0xFFFF));
				modeInfoBlock->ModeAttributes = 0x1B;
				modeInfoBlock->WinAAttributes = 0x7;
				modeInfoBlock->WinBAttributes = 0x0;
				modeInfoBlock->WinGranularity = 64;
				modeInfoBlock->WinSize = 64;
				modeInfoBlock->WinASegment = 0xA000;
				modeInfoBlock->WinBSegment = 0x0;
				modeInfoBlock->WinFuncPtr = 0x0;
				//LOG(1,"\nx: %d, y:%d, bpp:%d",modeInfo->XRes,modeInfo->YRes,modeInfo->BitPP);

				modeInfoBlock->XResolution = vca->vesaModeinfo.XRes;
				modeInfoBlock->BitsPerPixel = vca->vesaModeinfo.BitPP;
				modeInfoBlock->BytesPerScanLineVESA = ((unsigned int ) vca->vesaModeinfo.XRes) * vca->vesaModeinfo.BitPP / 8;
				modeInfoBlock->YResolution = vca->vesaModeinfo.YRes;
				modeInfoBlock->NumberOfImagePages = (config.VideoMemSize << 10) / ((unsigned int) modeInfoBlock->BytesPerScanLineVESA * vca->vesaModeinfo.YRes);
				modeInfoBlock->XCharSize = modeInfoBlock->YCharSize = 0;
				modeInfoBlock->NumberOfPlanes = 1;
				modeInfoBlock->MemoryModel = vca->vesaModeinfo.BitPP == 8 ? 4 : 6;
				modeInfoBlock->NumberOfBanks = 1;
				modeInfoBlock->BankSize = 0;
				modeInfoBlock->MIReserved = 1;

				modeInfoBlock->RedFieldPosition = vca->vesaModeinfo.RedPos;
				modeInfoBlock->RedMaskSize = vca->vesaModeinfo.RedSize;
				modeInfoBlock->GreenFieldPosition = vca->vesaModeinfo.GreenPos;
				modeInfoBlock->GreenMaskSize = vca->vesaModeinfo.GreenSize;
				modeInfoBlock->BlueFieldPosition = vca->vesaModeinfo.BluePos;
				modeInfoBlock->BlueMaskSize = vca->vesaModeinfo.BlueSize;
				modeInfoBlock->RsvdFieldPosition = vca->vesaModeinfo.RsvPos;
				modeInfoBlock->RsvdMaskSize = vca->vesaModeinfo.RsvSize;
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
				//vbeMode &= 0x3FFF;
				if (VBESearchInVideoModeList (vbeMode & 0x3FFF) != NULL) {
					SendMessage(serverCmdHwnd, DGSM_GETVESAMODEFORMAT, (WPARAM) (vbeMode & 0x3FFF), 0);
					if (vca->vesaModeinfo.modeIndex != -1) {
						
						vca->kernelflag |= KFLAG_VESAFRESHDISABLED;
						vca->actmodeinfo = vca->vesaModeinfo;
						
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
			if ((vca->actmodeinfo.ModeNumber & 0x3FFF) != 0x13) {
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
			palettePtr = (unsigned int *) DosMapFlat ((unsigned short) vesaRegisters->si, ((unsigned long) vesaRegisters->dx << 16) + (unsigned long) vesaRegisters->di);
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


int		VESAInit (void* /*driverHeader*/)
{
	
	if (!(config.Flags & CFG_VESAENABLE))
		return (2);
	
	ZeroMemory(&movie, sizeof(MovieData));
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


#endif /* VESA_SUPPORT */

#endif /* !GLIDE3 */