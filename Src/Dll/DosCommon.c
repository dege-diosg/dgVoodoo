/*--------------------------------------------------------------------------------- */
/* DosCommon.c - Glide general DOS-serving stuffs                                   */
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
/* dgVoodoo: DosCommon.c																	*/
/*			 DOS-kommunikáció																*/
/*------------------------------------------------------------------------------------------*/

#include	"Main.h"
#include    "Version.h"
#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"
#include	<stdio.h>
#include	<windows.h>
#include	<winuser.h>
#include	"resource.h"
#include	"dgVoodooGlide.h"
#include	"Dos.h"
#include	"VxdComm.h"
#include	"DosMouseNt.h"
#include	"Vesa.h"
#include	"Grabber.h"
#include	"Draw.h"
#include	"Texture.h"
#include	"LfbDepth.h"

#include	"debug.h"
#include	"Log.h"


#ifndef GLIDE3

#ifdef	DOS_SUPPORT


/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/



/*------------------------------------------------------------------------------------------*/
/*...................................Predeclarations........................................*/

LRESULT CALLBACK	WarningBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				BlockVirtualMachine();
void				UnBlockVirtualMachine();
void				dgVoodooReset();
static void			ProcessExecBuffer();
void				SetRegInfoAreaNT(ServerRegInfo *reginfo);


/*------------------------------------------------------------------------------------------*/
/*................................... Globális változók.....................................*/

DWORD				foregroundFlashCount;
DWORD				foregroundLockTimeOut;
int					OwnFocus;

unsigned char		*errorTitle;

CommArea			*c;

/* A kezdeti warning-box ablakosztálya */
WNDCLASS			dlgClass = {0, WarningBoxProc, 0, DLGWINDOWEXTRA, 0, NULL, NULL, NULL, NULL, "DGVOODOODLGCLASS"};
HWND				warningBoxHwnd;

/* A renderelõablak és a szerver parancs-ablakának az ablakosztálya */
WNDCLASS			mainWinClass = {0, NULL, 0, 0, 0, NULL, NULL, NULL, NULL, "DGVOODOOWINDOWCLASS"};
HWND				renderingWinHwnd, serverCmdHwnd, clientCmdHwnd, consoleHwnd;
unsigned int		actResolution = 0;
POINT				CursorPos;
unsigned int		MouseHided = 0;
RECT				actRenderingWinRect = {-1, -1, -1, -1};
const unsigned int	mainWndStyleEx = 0;
const unsigned int	mainWndStyle = WS_OVERLAPPEDWINDOW;

HANDLE				threadSyncEvent;							/* event az ideiglenes init-szinkronizációhoz */
								
const char			SharedMemName[] = "dgVoodooSM20040719";		/* szerver-mód: az osztott memterület neve */
const char			MutexEventName[] = "dgVoodooMTX20040719";	/* A rajzoláshoz használt mutex neve */

/*------------------------------------------------------------------------------------------*/
/*.................. WindowProc a renderelõ és szerverparancs-ablakhoz .....................*/


LRESULT CALLBACK MovieWindowFunction(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
static int		ctrldown;
int				retCode;
RECT			oldRect, newRect;
int				xSize, ySize;
int				code;
int				sizeDirection;
unsigned char	errorTitle[MAXSTRINGLENGTH];
unsigned char	errorMsg[MAXSTRINGLENGTH];
PAINTSTRUCT		paintStruct;


	switch(uMsg)	{
		case WM_CREATE:
			return(0);

		case WM_DESTROY:
			return(0);
	}

	if (hWnd == serverCmdHwnd)	{
		switch(uMsg)	{
			case DGSM_PROCEXECBUFF:
				ProcessExecBuffer();
				c->kernelflag &= ~KFLAG_SERVERWORKING;
				return(0);
			
			case DGSM_SETCONSOLEHWND:
				consoleHwnd = (HWND) wParam;
				return(0);
			
			case DGSM_SETTIMER:
				timerHandle = SetTimer(NULL, 0, 2500, GrabberTimerProc);
				return(0);
			
			case DGSM_DGVOODOORESET:
				dgVoodooReset();
				return(0);

			case DGSM_SETCLIENTHWND:
				clientCmdHwnd = (HWND) wParam;
				return(0);

			case DGSM_CLOSEBINDING:
				SetRegInfoAreaNT ((ServerRegInfo *) c);
				return (0);
			
			default:
				if (VESAServerCommandHandler(hWnd, uMsg, wParam, lParam) == 0)
					return(0);
		}
	}
	
	if (hWnd == renderingWinHwnd)	{
		switch(uMsg)	{
			case WM_CLOSE:
				if (config.Flags & CFG_NTVDDMODE) {
					PostMessage (consoleHwnd, WM_CLOSE, 0, 0);
				} else {
					GetString (errorTitle, IDS_SERVERKILLERRORTITLE);
					GetString (errorMsg, IDS_SURETOKILLSERVER);
					
					retCode = MessageBox(renderingWinHwnd, errorMsg, errorTitle, MB_YESNO | MB_ICONSTOP | MB_APPLMODAL);
					
					if (retCode == IDYES)
						PostMessage (warningBoxHwnd, WM_CLOSE, 0, 0);
				}
				return(0);
			
			case WM_ACTIVATE:
				wParam = ((LOWORD(wParam)) != WA_INACTIVE);
			
			case WM_ACTIVATEAPP:
				if ( (runflags & RF_INITOK) || c->VESASession )	{
					if (platform == PLATFORM_DOSWIN9X)	{
						if (wParam)	{
							if (!OwnFocus)	{
								if (config.Flags & CFG_SETMOUSEFOCUS)	{
									HideMouseCursor(1);
								}
								UnBlockVirtualMachine();
								OwnFocus=1;
							}
						} else {
							if (OwnFocus)	{
								if (config.Flags & CFG_SETMOUSEFOCUS)	{
									RestoreMouseCursor(1);
								}
								BlockVirtualMachine();
								OwnFocus=0;
							}
						}
					}
					if (platform == PLATFORM_DOSWINNT)	{
						if (wParam)	{
							//if (!OwnFocus)	{
								if (config.Flags & CFG_SETMOUSEFOCUS)	{
									DIMouseAcquire();
									HideMouseCursor(1);
								}
								if (!(config.Flags & CFG_ACTIVEBACKGROUND)) {
									SendMessage (clientCmdHwnd, DGCM_RESUMEDOSTHREAD, 0, 0);
								}
								OwnFocus=1;
							//}
						} else	{
							//if (OwnFocus)	{
								if (config.Flags & CFG_SETMOUSEFOCUS)	{
									DIMouseUnacquire();
									RestoreMouseCursor(1);
								}
								if (!(config.Flags & CFG_ACTIVEBACKGROUND)) {
									SendMessage (clientCmdHwnd, DGCM_SUSPENDDOSTHREAD, 0, 0);
								}
								OwnFocus=0;
							//}
						}
					}
					DosRendererStatus (wParam);
				}
				return(0);

//			case WM_WINDOWPOSCHANGING:
//				break;
			
			case WM_LBUTTONUP:
				if ( (config.Flags & (CFG_SETMOUSEFOCUS | CFG_CTRLALT)) == (CFG_SETMOUSEFOCUS | CFG_CTRLALT) )	{
					if (platform == PLATFORM_DOSWIN9X)	{
						HideMouseCursor(1);
						DeviceIoControl(hDevice,DGSERVER_SETFOCUSONVM,NULL,0,NULL,0,NULL,NULL);
					}
					if (platform == PLATFORM_DOSWINNT)	{
						DIMouseAcquire();
						HideMouseCursor(1);
					}
				}
				return(0);


			case WM_KEYDOWN:
				if (platform == PLATFORM_DOSWINNT)	{
					if ( (wParam == VK_PAUSE) && (config.Flags & CFG_GRABENABLE) )	{
						c->kernelflag |= KFLAG_SCREENSHOT;
						break;
					}
					if ( (config.Flags & CFG_SETMOUSEFOCUS)	&& (config.Flags & CFG_CTRLALT) ) {
						if (wParam==VK_CONTROL) ctrldown=1;
						if ( (wParam==VK_MENU) && ctrldown)	{
							DIMouseUnacquire();
							RestoreMouseCursor(1);
							ctrldown=0;
							return(0);
						}
						if ( (wParam!=VK_CONTROL) && (wParam!=VK_MENU) ) ctrldown=0;
					}
					SendMessage(consoleHwnd, uMsg, wParam, lParam);
				}
				return(0);
				break;

			case WM_KEYUP:
				if (platform == PLATFORM_DOSWINNT)	{
					ctrldown = 0;
					if ( (wParam == VK_PAUSE) && (config.Flags & CFG_GRABENABLE) )	{
						return(0);
						break;
					}
					SendMessage(consoleHwnd, uMsg, wParam, lParam);
				}
				return(0);
				break;

			case WM_SYSKEYDOWN:				
				if (platform == PLATFORM_DOSWINNT)	{
					if ( (config.Flags & CFG_SETMOUSEFOCUS)	&& (config.Flags & CFG_CTRLALT) ) {
						if ( (wParam == VK_MENU) && ctrldown)	{
							DIMouseUnacquire();
							RestoreMouseCursor(1);
							ctrldown = 0;
							return(0);
						}
					}
					ctrldown=0;
					SendMessage(consoleHwnd, uMsg, wParam, lParam);
				}
				return(0);
				break;
			
			case WM_SYSKEYUP:
				if (platform == PLATFORM_DOSWINNT)	{
					ctrldown = 0;
					SendMessage(consoleHwnd, uMsg, wParam, lParam);
				}
				return(0);
				break;

			case WM_SIZING:
				if (((runflags & RF_INITOK) || c->VESASession) && (config.Flags2 & CFG_PRESERVEASPECTRATIO))	{
					SetRect(&oldRect, 0, 0, 0, 0);
					AdjustWindowRectEx (&oldRect, mainWndStyle, FALSE, mainWndStyleEx);
					newRect =  * (LPRECT) lParam;
					newRect.left -= oldRect.left;
					newRect.top -= oldRect.top;
					newRect.right -= oldRect.right;
					newRect.bottom -= oldRect.bottom;
					xSize = ((newRect.bottom - newRect.top) * movie.cmiXres) / movie.cmiYres;
					ySize = ((newRect.right - newRect.left) * movie.cmiYres) / movie.cmiXres;
					code = (xSize - (newRect.right - newRect.left)) < (ySize - (newRect.bottom - newRect.top));
					sizeDirection = wParam;
					switch(sizeDirection) {
						case WMSZ_TOPLEFT:
							sizeDirection = code ? WMSZ_TOP : WMSZ_LEFT;
							break;
						case WMSZ_TOPRIGHT:
							sizeDirection = code ? WMSZ_BOTTOM : WMSZ_LEFT;
							break;
						case WMSZ_BOTTOMLEFT:
							sizeDirection = code ? WMSZ_TOP : WMSZ_RIGHT;
							break;
						case WMSZ_BOTTOMRIGHT:
							sizeDirection = code ? WMSZ_BOTTOM : WMSZ_RIGHT;
							break;
					}
					switch(sizeDirection) {
						case WMSZ_LEFT:
							newRect.top = newRect.bottom - ySize;
							break;
						
						case WMSZ_RIGHT:
							newRect.bottom = newRect.top + ySize;
							break;

						case WMSZ_TOP:
							newRect.left = newRect.right - xSize;
							break;
						
						case WMSZ_BOTTOM:
							newRect.right = newRect.left + xSize;
							break;

					}
					AdjustWindowRectEx (&newRect, mainWndStyle, FALSE, mainWndStyleEx);
					* (LPRECT) lParam = newRect;
				}
				break;
			
			case WM_SIZE:
				if ((runflags & RF_INITOK) || c->VESASession)	{
					if (RefreshDisplayByFrontBuffer ())
						ValidateRgn (hWnd, NULL);
				}
				break;

			case WM_PAINT:
				if ((runflags & RF_INITOK) || c->VESASession)	{
					if (!RefreshDisplayByFrontBuffer ()) {
						if (!OwnFocus) {
							InvalidateRgn (hWnd, NULL, FALSE);
							BeginPaint (hWnd, &paintStruct);
							FillRect (paintStruct.hdc, &paintStruct.rcPaint, (HBRUSH) GetStockObject (BLACK_BRUSH));
							EndPaint (hWnd, &paintStruct);
						}
					} else {
						ValidateRgn (hWnd, NULL);
					}
				}
				break;

			default:
				break;
				/*if ((runflags & RF_INITOK) || c->VESASession)	{
					movieDefWinProc = (WNDPROC) GetDefMovieWinProc();
					return(movieDefWinProc(hWnd, uMsg, wParam, lParam));
				} else return(DefWindowProc(hWnd, uMsg, wParam, lParam));*/
		}
	}
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}


/*------------------------------------------------------------------------------------------*/
/*......................... WarningBox Window- és Dialog függvénye .........................*/


LRESULT CALLBACK WarningBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	if (uMsg == WM_CLOSE)	{
		PostQuitMessage(0);
		return(0);
	} else return(DefDlgProc(hwnd, uMsg, wParam, lParam));
}


LRESULT CALLBACK WarningBoxDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	switch(uMsg)	{
		case WM_INITDIALOG:
#ifndef GLIDE1
			SendMessage(GetDlgItem(hwnd, IDC_GLIDEINFO), WM_SETTEXT, 0, (LPARAM) "(Glide 2.43)");
#else
			SendMessage(GetDlgItem(hwnd, IDC_GLIDEINFO), WM_SETTEXT, 0, (LPARAM) "(Glide 2.11)");
#endif
			return(FALSE);
		default:
			return(0);
	}
}


/*------------------------------------------------------------------------------------------*/
/*................................... Ablak segéd-függvények ...............................*/



void SetWindowClientArea(HWND window, int x, int y)
{
RECT	rect;
int		rx, ry;

	if (!(config.Flags & CFG_WINDOWED)) return;
	GetWindowRect(window, &rect);
	rx = rect.right - rect.left;
	ry = rect.bottom - rect.top;
	GetClientRect(window,&rect);
	rx = rx - (rect.right - rect.left);
	ry = ry - (rect.bottom - rect.top);
	
	SetWindowPos(window, NULL, 0, 0, x + rx, y + ry, SWP_NOMOVE | SWP_NOZORDER);
}


int	IsWindowSizeIsEqualToResolution (HWND window)
{
RECT			rect;
unsigned int	idealSize;

	if (!(config.Flags2 & CFG_PRESERVEWINDOWSIZE) || (actResolution == 0))
		return 1;

	GetClientRect (window, &rect);
	idealSize = GetIdealWindowSizeForResolution (actResolution >> 16, actResolution & 0xFFFF);

	if (((unsigned int) rect.right) == (idealSize >> 16) && ((unsigned int) rect.bottom) == (idealSize & 0xFFFF))
		return 1;

	return 0;
}


void AdjustAspectRatio (HWND window, int x, int y) {
RECT	rect;
int		xSize, ySize;

	if (config.Flags2 & CFG_PRESERVEASPECTRATIO) {
		GetClientRect (window, &rect);
		xSize = ((rect.bottom - rect.top) * x) / y;
		ySize = ((rect.right - rect.left) * y) / x;
		if ((xSize - x) < (ySize - y)) {
			x = xSize;
			y = rect.bottom - rect.top;
		} else {
			y = ySize;
			x = rect.right - rect.left;
		}
		SetWindowClientArea (window, x, y);
	}
}


unsigned int GetIdealWindowSizeForResolution (unsigned int xRes, unsigned int yRes) {
unsigned int i;
	
	i = (xRes < 512) && (yRes < 384) ? 2 : 1;
	return (((i * xRes) << 16) + i * yRes);
}


/*------------------------------------------------------------------------------------------*/
/*................................... Egérkurzor Show/Hide .................................*/


void HideMouseCursor(int mfocus)	{
//POINT Pos;
//RECT rect;
	
	//if (Platform==PLATFORM_DOSWINNT) return;
	if (mfocus && (!(config.Flags & CFG_SETMOUSEFOCUS)) ) return;
	if (!mfocus && (config.Flags & CFG_WINDOWED)) return;
	while(ShowCursor(FALSE)>=0);
	/*if (!MouseHided)	{
		GetCursorPos(&CursorPos);
		Pos.x=Pos.y=32767;
		//ClientToScreen(mainWinHwnd,&Pos);
		//SetCursorPos(Pos.x,Pos.y);
		ShowCursor(FALSE);
		rect.left=rect.top=rect.bottom=rect.right=32767;
		//ClipCursor(&rect);
		MouseHided=1;		
	}*/
}


void RestoreMouseCursor(int mfocus)	{
	
	//if (Platform==PLATFORM_DOSWINNT) return;
	if (mfocus && (!(config.Flags & CFG_SETMOUSEFOCUS)) ) return;
	if (!mfocus && (config.Flags & CFG_WINDOWED)) return;
	if (mfocus && (!(config.Flags & CFG_WINDOWED)) ) HideMouseCursor(mfocus);
		else while(ShowCursor(TRUE)<0);
	/*if (MouseHided)	{
		//SetCursorPos(CursorPos.x,CursorPos.y);
		//while(ShowCursor(TRUE)<0);
		ShowCursor(TRUE);
		//ClipCursor(NULL);
		MouseHided=0;
	}*/
}


/*------------------------------------------------------------------------------------------*/
/*....................... Az egyes ablakok létrehozása/eldobása ............................*/


void RegisterMainClass()	{
	
	mainWinClass.lpfnWndProc = MovieWindowFunction;
	mainWinClass.hInstance = hInstance;
	mainWinClass.hCursor = LoadCursor(0,IDC_ARROW);
	mainWinClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	RegisterClass(&mainWinClass);
}


HWND CreateRenderingWindow()	{

	renderingWinHwnd = CreateWindowEx(mainWndStyleEx, "DGVOODOOWINDOWCLASS", "", mainWndStyle, \
									  CW_USEDEFAULT, CW_USEDEFAULT, 250,250, NULL, NULL, \
									  hInstance, NULL);
	//movie.cmiWinHandle = renderingWinHwnd;
	//movie.cmiFlags = MV_3D;
	if (actRenderingWinRect.right != -1)
		SetWindowPos(renderingWinHwnd, renderingWinHwnd,
					 actRenderingWinRect.left, actRenderingWinRect.top,
					 actRenderingWinRect.right - actRenderingWinRect.left,
					 actRenderingWinRect.bottom - actRenderingWinRect.top, SWP_NOZORDER);
	
	return (renderingWinHwnd);
}


void DestroyRenderingWindow()	{

	GetWindowRect(renderingWinHwnd, &actRenderingWinRect);
	DestroyWindow(renderingWinHwnd);
}


void CreateServerCommandWindow()
{
	serverCmdHwnd = CreateWindowEx(0, "DGVOODOOWINDOWCLASS", "", WS_OVERLAPPEDWINDOW, \
								   CW_USEDEFAULT, CW_USEDEFAULT, 250,250, NULL, NULL, \
								   hInstance, NULL);
}


void DestroyServerCommandWindow()
{
	if (serverCmdHwnd != NULL)
		DestroyWindow(serverCmdHwnd);
}


void CreateWarningBox()	{
int resID;

	dlgClass.hInstance = hInstance;
	dlgClass.hCursor = LoadCursor(0, IDC_ARROW);
	dlgClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClass(&dlgClass);
	resID = (config.Flags & CFG_HUNGARIAN) ? IDD_DIALOGHUN : IDD_DIALOGENG;
	warningBoxHwnd = CreateDialog(hInstance, MAKEINTRESOURCE(resID), NULL, WarningBoxDialogProc);
}


void DestroyWarningBox()	{
	
	DestroyWindow(warningBoxHwnd);
	UnregisterClass("DGVOODOODLGCLASS", hInstance);

}


static void ProcessExecBuffer()	{
float			*t;
unsigned long	*execbuff;

	if (c->kernelflag & KFLAG_SCREENSHOT)
		GrabberHookDos(0);
	c->kernelflag &= ~KFLAG_SCREENSHOT;	
	execbuff = c->ExeCodes;
	while(execbuff[0] != GLIDEENDITEM)	{
		switch(execbuff[0])	{

			/* A Glide függvényei */
			case GRGLIDEINIT:
				grGlideInit();
				execbuff++;
				break;

			case GRSSTOPEN:	/* Glide1 */
#ifdef GLIDE1
				config.Flags |= CFG_LFBREALVOODOO;
				if (c->VESASession)	{
					VesaUnsetVBEMode(2);
					VESAModeWillBeSet = 1;
				}
				if (platform == PLATFORM_DOSWIN9X)
				execbuff[0] = grSstWinOpenDos9x((FxU32) NULL,
												(GrScreenResolution_t) execbuff[1],
												(GrScreenRefresh_t) execbuff[2],
												(GrColorFormat_t) execbuff[3],
												(GrOriginLocation_t) execbuff[4],
												(int) execbuff[6],
												0);
				if (platform == PLATFORM_DOSWINNT)
				execbuff[0] = grSstWinOpenDosNT((FxU32) NULL,
												(GrScreenResolution_t) execbuff[1],
												(GrScreenRefresh_t) execbuff[2],
												(GrColorFormat_t) execbuff[3],
												(GrOriginLocation_t) execbuff[4],
												(int) execbuff[6],
												0);
				OwnFocus = 1;
				execbuff += 7;
#endif
				break;
			
			case GRSSTWINOPEN:
				if (c->VESASession)	{
					VesaUnsetVBEMode(2);
					VESAModeWillBeSet = 1;
				}
				execbuff[0] = grSstWinOpen((FxU32) execbuff[1],
										   (GrScreenResolution_t) execbuff[2],
										   (GrScreenRefresh_t) execbuff[3],
										   (GrColorFormat_t) execbuff[4],
										   (GrOriginLocation_t) execbuff[5],
										   (int) execbuff[6],
										   (int) execbuff[7]);
				/*if (platform == PLATFORM_DOSWIN9X)
				execbuff[0] = grSstWinOpenDos9x((FxU32) execbuff[1],
												(GrScreenResolution_t) execbuff[2],
												(GrScreenRefresh_t) execbuff[3],
												(GrColorFormat_t) execbuff[4],
												(GrOriginLocation_t) execbuff[5],
												(int) execbuff[6],
												(int) execbuff[7]);
				if (platform == PLATFORM_DOSWINNT)
				execbuff[0] = grSstWinOpenDosNT((FxU32) execbuff[1],
												(GrScreenResolution_t) execbuff[2],
												(GrScreenRefresh_t) execbuff[3],
												(GrColorFormat_t) execbuff[4],
												(GrOriginLocation_t) execbuff[5],
												(int) execbuff[6],
												(int) execbuff[7]);*/
				OwnFocus = 1;
				execbuff += 8;
				break;
			
			case GRSSTWINCLOSE:
				grSstWinClose ();
				if (VESAModeWillBeSet)	{
					VesaSetVBEMode();
				}
				execbuff++;
				break;

			case GRGLIDESHUTDOWN:
				grGlideShutdown ();
				if (VESAModeWillBeSet)	{
					VesaSetVBEMode();
				}
				execbuff++;
				break;

			case GRSSTCONTROLMODE:
				grSstControlMode((FxU32) execbuff[1]);
				execbuff += 2;
				break;

			case GRBUFFERCLEAR:
				grBufferClear((GrColor_t) execbuff[1], (GrAlpha_t) execbuff[2],
							  (FxU16) execbuff[3]);
				execbuff += 4;
				break;

			case GRBUFFERSWAP:
				grBufferSwap((int) execbuff[1]);
				execbuff += 2;
				break;

			case GRBUFFERNUMPENDING:
				execbuff[0] = grBufferNumPending();
				execbuff++;
				break;

			case GRRESETTRISTATS:
				grResetTriStats();
				execbuff++;
				break;

			case GRTRISTATS:
				grTriStats(execbuff+1, execbuff+2);
				execbuff += 3;
				break;

			case GRRENDERBUFFER:
				grRenderBuffer((GrBuffer_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRSSTORIGIN:
				grSstOrigin((GrOriginLocation_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRSSTSTATUS:
				execbuff[0] = grSstStatus();
				execbuff++;
				break;

			case GRSSTVIDEOLINE:
				execbuff[0] = grSstVideoLine();
				execbuff++;
				break;

			case GRSSTVRETRACEON:
				execbuff[0] = grSstVRetraceOn();
				execbuff++;
				break;

			case GRCULLMODE:
				grCullMode((GrCullMode_t) execbuff[1]);
				execbuff += 2;
				break;

			case GRDISABLEALLEFFECTS:
				grDisableAllEffects();
				execbuff++;
				break;

			case GRDITHERMODE:
				grDitherMode((GrDitherMode_t) execbuff[1]);
				execbuff += 2;
				break;

			case GRGAMMACORRECTIONVALUE:
				grGammaCorrectionValue(*((float *) &(execbuff[1])));
				execbuff += 2;
				break;

			case GRGLIDEGETSTATE:
				grGlideGetState((_GrState *) execbuff[1]);
				execbuff += 2;
				break;

			case GRGLIDESETSTATE:
				grGlideSetState((_GrState *) execbuff[1]);
				execbuff += 2;
				break;

			case GRHINTS:
				grHints((GrHint_t) execbuff[1], (FxU32) execbuff[2]);
				execbuff += 3;
				break;

			case GRCOLORCOMBINE:
				grColorCombine((GrCombineFunction_t) execbuff[1],
							   (GrCombineFactor_t) execbuff[2],
							   (GrCombineLocal_t) execbuff[3],
							   (GrCombineOther_t) execbuff[4],
							   (FxBool) execbuff[5]);
				execbuff += 6;
				break;

			case GRALPHACOMBINE:
				grAlphaCombine((GrCombineFunction_t) execbuff[1],
							   (GrCombineFactor_t) execbuff[2],
							   (GrCombineLocal_t) execbuff[3],
							   (GrCombineOther_t) execbuff[4],
							   (FxBool) execbuff[5]);
				execbuff += 6;
				break;

			case GRALPHACONTROLSITRGBLIGHTING:
				grAlphaControlsITRGBLighting ((FxBool) execbuff[1]);
				execbuff += 2;
				break;

			case GRCONSTANTCOLORVALUE:				
				grConstantColorValue((GrColor_t) execbuff[1]);
				execbuff += 2;
				break;

			case GRCONSTANTCOLORVALUE4:
				grConstantColorValue4( *((float*) &execbuff[1]), *((float*) &execbuff[2]), *((float*) &execbuff[3]),
										*((float*) &execbuff[4]));
				execbuff += 5;
				break;

			case GRALPHABLENDFUNCTION:
				grAlphaBlendFunction((GrAlphaBlendFnc_t) execbuff[1],
									 (GrAlphaBlendFnc_t) execbuff[2],
									 (GrAlphaBlendFnc_t) execbuff[3],
									 (GrAlphaBlendFnc_t) execbuff[4]);
				execbuff += 5;
				break;

			case GRALPHATESTFUNCTION:
				grAlphaTestFunction((GrCmpFnc_t) execbuff[1]);
				execbuff += 2;
				break;

			case GRALPHATESTREFERENCEVALUE:
				grAlphaTestReferenceValue((GrAlpha_t) execbuff[1]);
				execbuff += 2;
				break;

			case GRCOLORMASK:
				grColorMask((FxBool) execbuff[1], (FxBool) execbuff[2]);
				execbuff += 3;
				break;

			case GUCOLORCOMBINEFUNCTION:
				guColorCombineFunction((GrColorCombineFnc_t) execbuff[1]);
				execbuff += 2;
				break;

			case GUALPHASOURCE:
				guAlphaSource( (GrAlphaSource_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRAADRAWPOLYGON:
				grAADrawPolygon((int) execbuff[1], (const int *) execbuff[2],
								(const GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GRAADRAWPOLYGONVERTEXLIST:
				grAADrawPolygonVertexList( (int) execbuff[1],
											(const GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRAADRAWTRIANGLE:
				grAADrawTriangle( (GrVertex *) execbuff[1],
								  (GrVertex *) execbuff[2],
								  (GrVertex *) execbuff[3],
								  (FxBool) execbuff[4],
								  (FxBool) execbuff[5],
								  (FxBool) execbuff[6] );
				execbuff += 7;
				break;

			case GRDRAWPLANARPOLYGON:
				grDrawPlanarPolygon( (int) execbuff[1],
									(int *) execbuff[2],
									(const GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GRDRAWPLANARPOLYGONVERTEXLIST:
				grDrawPlanarPolygonVertexList( (int) execbuff[1],
												(const GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRDRAWPOLYGON:
				grDrawPolygon( (int) execbuff[1], (int *) execbuff[2],
								(const GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GRDRAWTRIANGLE:
				grDrawTriangle( (GrVertex *) execbuff[1], (GrVertex *) execbuff[2],
								(GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GUDRAWTRIANGLEWITHCLIP:
				guDrawTriangleWithClip( (GrVertex *) execbuff[1], (GrVertex *) execbuff[2],
								(GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GUAADRAWTRIANGLEWITHCLIP:
				guAADrawTriangleWithClip( (GrVertex *) execbuff[1], (GrVertex *) execbuff[2],
								(GrVertex *) execbuff[3] );
				execbuff += 4;
				break;

			case GUDRAWPOLYGONVERTEXLISTWITHCLIP:
				guDrawPolygonVertexListWithClip( (int) execbuff[1], (const GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRDRAWPOLYGONVERTEXLIST:
				grDrawPolygonVertexList( (int) execbuff[1], (GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRTEXDOWNLOADMIPMAP:
				grTexDownloadMipMap( (GrChipID_t) execbuff[1], (FxU32) execbuff[2],
									(FxU32) execbuff[3], (GrTexInfo *) execbuff[4] );
				execbuff += 5;
				break;

			case GRTEXDOWNLOADMIPMAPLEVEL:
				grTexDownloadMipMapLevel( (GrChipID_t) execbuff[1], (FxU32) execbuff[2],
										(GrLOD_t) execbuff[3], (GrLOD_t) execbuff[4],
										(GrAspectRatio_t) execbuff[5],
										(GrTextureFormat_t) execbuff[6],
										(FxU32) execbuff[7], (void *) execbuff[8] );
				execbuff += 9;
				break;

			case GRTEXDOWNLOADMIPMAPLEVELPARTIAL:
				grTexDownloadMipMapLevelPartial( (GrChipID_t) execbuff[1], (FxU32) execbuff[2],
										(GrLOD_t) execbuff[3], (GrLOD_t) execbuff[4],
										(GrAspectRatio_t) execbuff[5],
										(GrTextureFormat_t) execbuff[6],
										(FxU32) execbuff[7], (void *) execbuff[8],
										(int	) execbuff[9], (int) execbuff[10] );
				execbuff += 11;
				break;

			case GRTEXDOWNLOADTABLE:
				grTexDownloadTable( (GrChipID_t) execbuff[1], (GrTexTable_t) execbuff[2],
									(void *) execbuff[3] );
				execbuff += 4;
				break;

			case GRTEXDOWNLOADTABLEPARTIAL:
				grTexDownloadTablePartial( (GrChipID_t) execbuff[1],
											(GrTexTable_t) execbuff[2],
											(void *) execbuff[3],
											(int) execbuff[4],
											(int) execbuff[5] );
				execbuff += 6;
				break;

			case GRTEXNCCTABLE:
				grTexNCCTable( (GrChipID_t) execbuff[1],
								(GrNCCTable_t) execbuff[2] );
				execbuff += 3;
				break;

			case GRTEXSOURCE:
				grTexSource( (GrChipID_t) execbuff[1],
							 (FxU32) execbuff[2],
							 (FxU32) execbuff[3],
							 (GrTexInfo *) execbuff[4] );
				execbuff += 5;
				break;

			case GRTEXMULTIBASE:
				grTexMultibase((GrChipID_t) execbuff[1],
								(FxBool) execbuff[2] );
				execbuff += 3;
				break;

			case GRTEXMULTIBASEADDRESS:
				grTexMultibaseAddress((GrChipID_t) execbuff[1],
										(GrTexBaseRange_t) execbuff[2],
										(FxU32) execbuff[3],
										(FxU32) execbuff[4],
										(GrTexInfo *) execbuff[5]);
				execbuff += 6;
				break;

			case GRTEXMIPMAPMODE:
				grTexMipMapMode( (GrChipID_t) execbuff[1],
								(GrMipMapMode_t) execbuff[2],
								(FxBool) execbuff[3] );
				execbuff += 4;
				break;

			case GRTEXCOMBINE:
				grTexCombine( (GrChipID_t) execbuff[1],
								(GrCombineFunction_t) execbuff[2],
								(GrCombineFactor_t) execbuff[3],
								(GrCombineFunction_t) execbuff[4],
								(GrCombineFactor_t) execbuff[5],
								(FxBool) execbuff[6],
								(FxBool) execbuff[7] );
				execbuff += 8;
				break;

			case GRTEXFILTERMODE:
				grTexFilterMode( (GrChipID_t) execbuff[1],
								(GrTextureFilterMode_t) execbuff[2],
								(GrTextureFilterMode_t) execbuff[3] );
				execbuff += 4;
				break;

			case GRTEXCLAMPMODE:
				grTexClampMode( (GrChipID_t) execbuff[1],
								(GrTextureClampMode_t) execbuff[2],
								(GrTextureClampMode_t) execbuff[3] );
				execbuff += 4;
				break;

			case GRTEXLODBIASVALUE:
				grTexLodBiasValue( (GrChipID_t) execbuff[1], *((float *) &execbuff[2]) );
				execbuff += 3;
				break;

			/* texture utility functions */
			case GUTEXALLOCATEMEMORY:
				execbuff[0] = guTexAllocateMemory( 
									(GrChipID_t) execbuff[1], (FxU8) execbuff[2],
									(int) execbuff[3], (int) execbuff[4],
									(GrTextureFormat_t) execbuff[5],
									(GrMipMapMode_t) execbuff[6],
									(GrLOD_t) execbuff[7], (GrLOD_t) execbuff[8],
									(GrAspectRatio_t) execbuff[9],
									(GrTextureClampMode_t) execbuff[10],
									(GrTextureClampMode_t) execbuff[11],
									(GrTextureFilterMode_t) execbuff[12],
									(GrTextureFilterMode_t) execbuff[13],
									(float) execbuff[14], (FxBool) execbuff[15] );
				execbuff += 16;
				break;

			case GUTEXCHANGEATTRIBUTES:
				execbuff[0] = guTexChangeAttributes(
									(GrMipMapId_t) execbuff[1],
									(int) execbuff[3], (int) execbuff[4],
									(GrTextureFormat_t) execbuff[5],
									(GrMipMapMode_t) execbuff[6],
									(GrLOD_t) execbuff[7], (GrLOD_t) execbuff[8],
									(GrAspectRatio_t) execbuff[9],
									(GrTextureClampMode_t) execbuff[10],
									(GrTextureClampMode_t) execbuff[11],
									(GrTextureFilterMode_t) execbuff[12],
									(GrTextureFilterMode_t) execbuff[13]);
				execbuff += 14;
				break;

			case GUTEXCOMBINEFUNCTION:
				guTexCombineFunction( (GrChipID_t) execbuff[1],
									(GrTextureCombineFnc_t) execbuff[2] );
				execbuff += 3;
				break;

			case GUTEXDOWNLOADMIPMAP:
				guTexDownloadMipMap( (GrMipMapId_t) execbuff[1],
									(void *) execbuff[2], (GuNccTable *) execbuff[3] );
				execbuff += 4;
				break;

			case GUTEXDOWNLOADMIPMAPLEVEL:
				guTexDownloadMipMapLevel( (GrMipMapId_t) execbuff[1],
									(GrLOD_t) execbuff[2], (void **) execbuff[3] );
				execbuff += 4;
				break;

			case GUTEXSOURCE:
				guTexSource( (GrMipMapId_t) execbuff[1] );
				execbuff += 2;
				break;

			case GUTEXGETCURRENTMIPMAP:
				execbuff[0] = guTexGetCurrentMipMap( (GrChipID_t) execbuff[1] );
				execbuff += 2;
				break;

			case GUTEXMEMRESET:
				guTexMemReset();
				execbuff++;
				break;

			case GUTEXMEMQUERYAVAIL:
				execbuff[0] = guTexMemQueryAvail((GrChipID_t) execbuff[1]);
				execbuff += 2;
				break;

			case GUTEXGETMIPMAPINFO:
				execbuff[0] = GlideCopyMipMapInfo( (GrMipMapInfo *) execbuff[1], (GrMipMapId_t) execbuff[2]);
				execbuff += 3;
				break;

			case GRSSTSCREENWIDTH:
				execbuff[0] = grSstScreenWidth();
				execbuff++;
				break;

			case GRSSTSCREENHEIGHT:
				execbuff[0] = grSstScreenHeight();
				execbuff++;
				break;

			case GLIDEGETCONVBUFFXRES:
				execbuff[0] = LfbGetConvBuffXRes();
				execbuff++;
				break;

			case GRCLIPWINDOW:
				grClipWindow((FxU32) execbuff[1], (FxU32) execbuff[2],
							 (FxU32) execbuff[3], (FxU32) execbuff[4] );
				execbuff += 5;
				break;

			case GRDEPTHBUFFERMODE:
				grDepthBufferMode( (GrDepthBufferMode_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRDEPTHBUFFERFUNCTION:
				grDepthBufferFunction( (GrCmpFnc_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRDEPTHMASK:
				grDepthMask( (FxBool) execbuff[1] );
				execbuff += 2;
				break;

			case GRDEPTHBIASLEVEL:
				grDepthBiasLevel( (FxI16) execbuff[1] );
				execbuff += 2;
				break;

			case GRCHROMAKEYMODE:
				grChromakeyMode( (GrChromakeyMode_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRCHROMAKEYVALUE:
				grChromakeyValue( (GrColor_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRLFBBYPASSMODE:	/* Glide1 */
#ifdef GLIDE1
				grLfbBypassMode( (GrLfbBypassMode_t) execbuff[1] );
				execbuff += 2;
#endif
				break;

			case GRLFBORIGIN:	/* Glide1 */
#ifdef GLIDE1
				grLfbOrigin( (GrOriginLocation_t) execbuff[1] );
				execbuff += 2;
#endif
				break;

			case GRLFBWRITEMODE:	/* Glide1 */
#ifdef GLIDE1
				grLfbWriteMode( (GrLfbWriteMode_t) execbuff[1] );
				execbuff += 2;
#endif
				break;

			case GRLFBBEGIN:	/* Glide1 */
#ifdef GLIDE1
				grLfbBegin();
				execbuff++;
#endif
				break;

			case GRLFBEND:	/* Glide1 */
#ifdef GLIDE1
				grLfbEnd();
				execbuff++;
#endif
				break;

			case GRLFBGETREADPTR:	/* Glide1 */
#ifdef GLIDE1
				execbuff[0] = (unsigned long) grLfbGetReadPtr( (GrBuffer_t) execbuff[1]);
				execbuff += 2;
#endif
				break;

			case GRLFBGETWRITEPTR:	/* Glide1 */
#ifdef GLIDE1
				execbuff[0] = (unsigned long) grLfbGetWritePtr( (GrBuffer_t) execbuff[1]);
				execbuff += 2;
#endif
				break;

			case GRLFBLOCK:
				execbuff[0] = grLfbLock((GrLock_t) execbuff[1],
										(GrBuffer_t) execbuff[2],
										(GrLfbWriteMode_t) execbuff[3],
										(GrOriginLocation_t) execbuff[4],
										(FxBool) execbuff[5],
										(GrLfbInfo_t *) execbuff[6] );
				execbuff += 7;
				break;

			case GRLFBUNLOCK:
				execbuff[0] = grLfbUnlock((GrLock_t) execbuff[1],
										  (GrBuffer_t) execbuff[2] );
				execbuff += 3;
				break;

			case GRLFBREADREGION:
				execbuff[0] = grLfbReadRegion((GrBuffer_t) execbuff[1],
											  (FxU32) execbuff[2],
											  (FxU32) execbuff[3],
											  (FxU32) execbuff[4],
											  (FxU32) execbuff[5],
											  (FxU32) execbuff[6],
											  (void *) execbuff[7] );
				execbuff += 8;
				break;

			case GRLFBWRITEREGION:
				execbuff[0] = grLfbWriteRegion((GrBuffer_t) execbuff[1],
											   (FxU32) execbuff[2],
											   (FxU32) execbuff[3],
											   (FxU32) execbuff[4],
											   (FxU32) execbuff[5],
											   (FxU32) execbuff[6],
											   (FxU32) execbuff[7],
											   (void *) execbuff[8] );
				execbuff += 9;
				break;

			case GRLFBCONSTANTALPHA:
				grLfbConstantAlpha( (GrAlpha_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRLFBWRITECOLORFORMAT:
				grLfbWriteColorFormat( (GrColorFormat_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRLFBWRITECOLORSWIZZLE:
				grLfbWriteColorSwizzle( (FxBool) execbuff[1], (FxBool) execbuff[2]);
				execbuff += 3;
				break;

			case GLIDESETUPLFBDOSBUFFERS:
				execbuff[0] = LfbSetUpLFBDOSBuffers((unsigned char *) execbuff[1],
													(unsigned char *) execbuff[2],
													(unsigned char *) execbuff[3],
													(int) execbuff[4]);
				execbuff += 5;
				break;

			case GRAADRAWLINE:
				grAADrawLine( (GrVertex *) execbuff[1], (GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRDRAWLINE:
				grDrawLine( (GrVertex *) execbuff[1], (GrVertex *) execbuff[2] );
				execbuff += 3;
				break;

			case GRAADRAWPOINT:
				grAADrawPoint( (GrVertex *) execbuff[1] );
				execbuff += 2;
				break;

			case GRDRAWPOINT:
				grDrawPoint( (GrVertex *) execbuff[1] );
				execbuff += 2;
				break;

			case GU3DFGETINFO:
				execbuff[0] = gu3dfGetInfo((char *) execbuff[1], (Gu3dfInfo *) execbuff[2] );
				execbuff += 3;
				break;

			case GU3DFLOAD:
				execbuff[0] = gu3dfLoad((char *) execbuff[1], (Gu3dfInfo *) execbuff[2] );
				execbuff += 3;
				break;

			case GUFOGTABLEINDEXTOW:
				t = (float *) &(execbuff[0]);
				t[0] = guFogTableIndexToW((int) execbuff[1] );
				execbuff += 2;
				break;

			case GUFOGGENERATEEXP:
				t = (float *) &(execbuff[2]);
				guFogGenerateExp((GrFog_t *) execbuff[1], t[0]);
				execbuff += 3;
				break;

			case GUFOGGENERATEEXP2:
				t = (float *) &(execbuff[2]);
				guFogGenerateExp2((GrFog_t *) execbuff[1], t[0] );
				execbuff += 3;
				break;

			case GUFOGGENERATELINEAR:
				t = (float *) &(execbuff[2]);
				guFogGenerateLinear((GrFog_t *) execbuff[1], t[0], t[1] );
				execbuff += 4;
				break;

			case GRFOGCOLORVALUE:
				grFogColorValue((GrColor_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRFOGMODE:
				grFogMode((GrFogMode_t) execbuff[1] );
				execbuff += 2;
				break;

			case GRFOGTABLE:
				grFogTable( (GrFog_t *) execbuff[1] );
				execbuff += 2;
				break;

			case GLIDEGETINDTOWTABLE:
				GlideGetIndToWTable( (float *) execbuff[1] );
				execbuff += 2;
				break;
			
			/* kernelmodul által hívott függvények */

			
			/* VESA: adott mód beállítása */
			case VESASETVBEMODE:				
				execbuff[0] = VesaSetVBEMode();
				execbuff++;
				break;
			
			/* VESA: adott mód kikapcsolása */
			case VESAUNSETVBEMODE:
				VesaUnsetVBEMode(1);				
				execbuff++;
				break;			

			/* Ezt a függvényt akkor hívja a kernelmodul, ha a kliens még regisztrálva van, */
			/* de a program végetért (összeomlott, a VM-et megsemmisítik vagy */
			/* csak nem hívta meg a cleanup függvényeket) */
			case DGVOODOOCLIENTCRASHED:
				dgVoodooReset();
				execbuff++;
				break;

			/* Ezt a függvényt akkor hívja a kernelmodul, ha a egy új DOS-os kliens */
			/* sikeresen regisztrálja magát */
			case DGVOODOONEWCLIENTREGISTERING:				
				actResolution = 0;
				execbuff++;
				break;

			case DGVOODOOGETCONFIG:
				CopyMemory((void *) execbuff[1],&config,sizeof(dgVoodooConfig));
				execbuff += 2;
				break;

			case DGVOODOORELEASEFOCUS:
				RestoreMouseCursor(1);
				DeviceIoControl(hDevice, DGSERVER_RELEASEKBDMOUSEFOCUS, NULL, 0, NULL, 0, NULL, NULL);
				execbuff++;
				break;

			case GLIDEGETUTEXTURESIZE:
				execbuff[0] = GlideGetUTextureSize( (GrMipMapId_t) execbuff[1], (GrLOD_t) execbuff[2], execbuff[3]);
				execbuff += 4;
				break;

			case GLIDEINSTALLMOUSE:
				execbuff[0] = NTInstallMouse((MouseDriverInfo *) execbuff[1]);
				execbuff += 2;
				break;

			case GLIDEUNINSTALLMOUSE:
				execbuff[0] = NTUnInstallMouse();
				execbuff++;
				break;

			case GLIDEISMOUSEINSTALLED:
				execbuff[0] = NTIsMouseInstalled ();
				execbuff++;
				break;

			case VESAGETXRES:
				execbuff[0] = VESAGetXRes ();
				execbuff++;
				break;

			case VESAGETYRES:
				execbuff[0] = VESAGetYRes ();
				execbuff++;
				break;

/*			case PORTLOG_IN:
				LOG(1,"\nin (%0x) = %0x",(unsigned int) c->ExeCodes[i+1],(unsigned int) c->ExeCodes[i+2]);				
				i+=3;
				break;
			case PORTLOG_OUT:
				LOG(1,"\nout (%0x,%0x)",(unsigned int) c->ExeCodes[i+1],(unsigned int) c->ExeCodes[i+2]);
				i+=3;
				break;*/
			default:;
				DEBUGLOG(0,"\n   Fatal error: ProcessExecBuffer: Invalid function code in communication with DOS! DLL and OVL are probably not compatible!!");
				DEBUGLOG(1,"\n   Végzetes hiba: ProcessExecBuffer: Érvénytelen függvénykód a DOS-szal való kommunikáció közben! A DLL és az OVL valószínûleg nem kompatibilisek!!");
				break;	
		}
	}	
}


void DosSendSetTimerMessage()	{
	
	SendMessage(serverCmdHwnd, DGSM_SETTIMER, 0, 0);
}


void DosRendererStatus (int status)
{
	if (!(config.Flags & CFG_WINDOWED)) {
		/*if (status && IsZoomed (renderingWinHwnd)) {
			return;
		}*/
		ShowWindow (renderingWinHwnd, status ? SW_MAXIMIZE : SW_MINIMIZE);
	}
}


void dgVoodooReset()	{

	if (c->VESASession || VESAModeWillBeSet) VesaUnsetVBEMode(0);
	grGlideShutdown ();
	ShowWindow(renderingWinHwnd, SW_HIDE);
	ShowWindow(warningBoxHwnd, SW_SHOW);
	ShowWindow((HWND) c->WinOldApHWND, SW_SHOW);
	actResolution = 0;
}


#endif /* DOS_SUPPORT */

#endif /* !GLIDE3 */
