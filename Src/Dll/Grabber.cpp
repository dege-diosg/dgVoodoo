/*--------------------------------------------------------------------------------- */
/* Grabber.cpp - dgVoodoo implementation of screen capturing                        */
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
/* dgVoodoo: Grabber.cpp																	*/
/*			 Képernyõmentés																	*/
/*------------------------------------------------------------------------------------------*/

extern "C" {

#include	"dgVoodooBase.h"

#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>
#include	<string.h>
#include	"Dgw.h"
#include	"File.h"
#include	"Version.h"
#include	"Main.h"

#include	"dgVoodooConfig.h"
#include	"dgVoodooGlide.h"
#include	"Grabber.h"
#include	"LfbDepth.h"
#include	"Dos.h"

#include	"Debug.h"
#include	"Log.h"
#include	"Texture.h"
#include	"Draw.h"

#include	"Resource.h"

}

#include	"Renderer.hpp"
#include	"RendererGeneral.hpp"
#include	"RendererTexturing.hpp"
#include	"RendererLfbDepth.hpp"

#include	<math.h>

/*-------------------------------------------------------------------------------------------*/

static CRITICAL_SECTION		gCritSection;
static HDC					compatibleDC;
static HBITMAP				grabBitmap, savedBitmap;
static unsigned char		*bitmap;
static BITMAPINFO			*bitmapInfo;
static unsigned short		*texturedata;
static TextureType			labelTextures[64];
static int					labelXLeft, labelXRight;
static LOGFONT				font =	{-19, 0, 0, 0, 700, 0, 0, 0, 238, 3, 2, 1, 34, "Verdana"};
static HFONT				grabFont, savedFont;
static HHOOK				hookHandle;
static int					labelDraw = 0;
static int					fileSuffix = -1;
static _pixfmt				bmpPixFmt8 = {8, 0, 0, 0, 0,  0, 0, 0, 0};
static _pixfmt				bmpPixFmt16 = {16, 5, 5, 5, 0,  10, 5, 0, 0};
static _pixfmt				bmpPixFmt32 = {32, 8, 8, 8, 0,  16, 8, 0, 0};
static int					auxBuffersUsed;
UINT						timerHandle;

/*-------------------------------------------------------------------------------------------*/
/*....................................... Függvények ........................................*/

void CALLBACK GrabberTimerProc(HWND /*hwnd*/, UINT /*uMsg*/, UINT /*idEvent*/, DWORD /*dwTime*/)	{

	DEBUG_DISABLEPROFILING;
	KillTimer(NULL, timerHandle);
	GrabberDeleteLabel();
}


void GrabberGenerateFileName(/*unsigned*/ char *filename)	{
static /*unsigned*/ char	appFileDir[256];
static /*unsigned*/ char	appFileName[256];
/*unsigned*/ char			*srcFileName;
/*unsigned*/ char			suffix[4];
WIN32_FIND_DATA			findData;
int						counter;
HANDLE					ffHandle;
/*unsigned*/ char			c0,c1,c2;
int						i;
	
	counter = fileSuffix;

#ifndef DOS_SUPPORT

	srcFileName = moduleName;

#else

#ifdef GLIDE3
	srcFileName = moduleName;
#else
	srcFileName = (platform == PLATFORM_WINDOWS) ? moduleName : c->progname;
#endif

#endif
	if (fileSuffix == -1)	{
		appFileName[0] = 0x0;
		appFileDir[0] = 0x0;
		if (srcFileName[0] == 0x0)	{
			strcat(appFileName, "dgVoodoo");
			strcat(appFileDir, "c:\\Temp");
		} else {
			strcat(appFileName, srcFileName + 1);
			i = strlen(appFileName);
			while( (i != 0) && (appFileName[i] != '.') ) i--;
			if (i) appFileName[i] = 0x0;
			appFileDir[0] = srcFileName[0];
			appFileDir[1] = 0x0;
			strcat(appFileDir, ":\\Temp");
		}		
		filename[0] = 0;
		strcat(filename, appFileDir);
		strcat(filename, "\\");
		strcat(filename, appFileName);
		strcat(filename, "???");
		strcat(filename, ".bmp");
		CreateDirectory(appFileDir, NULL);
		counter = 0;
		if ( (ffHandle = FindFirstFile(filename, &findData)) != INVALID_HANDLE_VALUE)	{		
			do	{
				c0 = findData.cFileName[strlen(appFileName)];
				c1 = findData.cFileName[strlen(appFileName) + 1];
				c2 = findData.cFileName[strlen(appFileName) + 2];
				if ( ( (c0 <= '9') && (c0 >= '0') ) && ( (c1 <= '9') && (c1 >= '0') ) &&
					( (c2 <= '9') && (c2 >= '0') ) )	{
					fileSuffix = (((int) c0) - '0') * 100 + (((int) c1) - '0') * 10 + (((int) c2) - '0');
					if (counter < fileSuffix) counter = fileSuffix;
				}
			} while (FindNextFile(ffHandle, &findData) != 0);
			FindClose(ffHandle);
		} else {
			GetLastError();
		}
		fileSuffix = counter + 1;
	} else fileSuffix++;
	suffix[0] = (char) ((fileSuffix / 100) + '0');
	suffix[1] = (char) (((fileSuffix / 10) % 10) + '0');
	suffix[2] = (char) ((fileSuffix % 10) + '0');
	suffix[3] = 0x0;
	filename[0] = 0x0;
	strcat(filename, appFileDir);
	strcat(filename, "\\");
	strcat(filename, appFileName);
	strcat(filename, suffix);
	strcat(filename, ".bmp");
}


void GrabberGrabLFB(int buffer, /*unsigned*/ char *label, PALETTEENTRY *lpPalette)	{
HGLOBAL				hMem;
BITMAPINFO			*bitmap;
BITMAPFILEHEADER	bmpheader;
unsigned char		*p;
unsigned short		*q;
int					i;
HANDLE				hfile;
RGBQUAD				*lpPalettePlaced;
/*unsigned*/ char		GrabFileName[MAX_PATH];
int					failed;
_pixfmt				*bmpPixFmt = NULL;
/*unsigned*/ char		string[MAXSTRINGLENGTH];

	hMem = GlobalAlloc(GMEM_DDESHARE,sizeof(BITMAPINFO)+1024+movie.cmiXres*movie.cmiYres*movie.cmiByPP);
	bitmap = (BITMAPINFO *) GlobalLock(hMem);
	ZeroMemory(bitmap, sizeof(BITMAPINFO));
	bitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap->bmiHeader.biWidth = movie.cmiXres;
	bitmap->bmiHeader.biHeight = movie.cmiYres;
	bitmap->bmiHeader.biPlanes = 1;
	bitmap->bmiHeader.biBitCount = (unsigned short) movie.cmiBitPP;
	bitmap->bmiHeader.biCompression = BI_RGB;
	bitmap->bmiHeader.biSizeImage = 0;
	bitmap->bmiHeader.biClrUsed = 0;
	bitmap->bmiHeader.biClrImportant = 0;
	p = (unsigned char *) bitmap;
	p += sizeof(BITMAPINFO); //+1024;
	if (lpPalette)	{
		lpPalettePlaced = (RGBQUAD *) p;
		for(i = 0; i < 256; i++)	{
			lpPalettePlaced[i].rgbRed = lpPalette[i].peRed;
			lpPalettePlaced[i].rgbGreen = lpPalette[i].peGreen;
			lpPalettePlaced[i].rgbBlue = lpPalette[i].peBlue;
			lpPalettePlaced[i].rgbReserved = 0x0;
		}		
		bitmap->bmiHeader.biClrUsed = 256;		
		p += 256*sizeof(RGBQUAD);
	}
	q = (unsigned short *) p;	

	if (buffer == 0)
		GrabberRestoreFrontBuff();

	switch(backBufferFormat.BitPerPixel/*movie.cmiBitPP*/)	{
		case 8:
			bmpPixFmt = &bmpPixFmt8;
			break;
		case 16:
			bmpPixFmt = &bmpPixFmt16;
			break;
		case 32:
			bmpPixFmt = &bmpPixFmt32;
			break;
	}
	
	void*			buffPtr;
	unsigned int	buffPitch;
	if (rendererLfbDepth->LockBuffer  (buffer, RendererLfbDepth::ColorBuffer, &buffPtr, &buffPitch)) {
	
		MMCConvertAndTransferData(&backBufferFormat, bmpPixFmt,
								  0, 0, 0,
								  buffPtr, q,
								  movie.cmiXres, movie.cmiYres,
								  -((int) buffPitch), movie.cmiXres*movie.cmiByPP,
								  NULL, NULL,
								  MMCIsPixfmtTheSame(&backBufferFormat, bmpPixFmt) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
								  0);
		
		rendererLfbDepth->UnlockBuffer (buffer, RendererLfbDepth::ColorBuffer);
	} else {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		if (label)	{
			label[0] = 0x0;
			GetString (string, IDS_LFBSAVINGCANNOTGETDATA);
			strcat(label, string);
		}
		return;
	}

	if (config.Flags & CFG_GRABCLIPBOARD)	{
		GlobalUnlock(hMem);
		OpenClipboard(NULL);
		SetClipboardData(CF_DIB, hMem);
		CloseClipboard();
		if (label)	{
			label[0] = 0x0;
			GetString (string, IDS_LFBSAVEDTOCLIPBOARD);
			strcat(label, string);
		}
	} else {
		GrabberGenerateFileName(GrabFileName);
		DEBUGLOG (0, "\nSaveLfb: Generated screenshot filename: %s", GrabFileName);
		DEBUGLOG (1, "\nSaveLfb: Generált fájlnév a képmentéshez: %s", GrabFileName);
		if (label)	{
			label[0] = 0x0;
			GetString (string, IDS_LFBSAVEDTOFILEPREFIX);
			strcat(label, string);
			strcat(label, GrabFileName);
			GetString (string, IDS_LFBSAVEDTOFILESUFFIX);
			strcat(label, string);
		}
		hfile = _CreateFile(GrabFileName, FILE_ATTRIBUTE_NORMAL);
		failed = (hfile == INVALID_HANDLE_VALUE);
		
		bmpheader.bfType = 'MB';
		bmpheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (movie.cmiXres * movie.cmiYres * movie.cmiByPP);
		bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		if (lpPalette) bmpheader.bfOffBits += 256 * sizeof(RGBQUAD);
		bmpheader.bfReserved1 = bmpheader.bfReserved2 = 0;	
		if (_WriteFile(hfile, &bmpheader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER)) {
			GetLastError();
			failed = 1;
		}
		if (_WriteFile(hfile, bitmap, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))	{
			failed = 1;
		}
		if (lpPalette)	{
			if (_WriteFile(hfile, lpPalettePlaced, 256*sizeof(RGBQUAD)) != 256*sizeof(RGBQUAD))	{
				failed = 1;
			}
		}
		if (_WriteFile(hfile, q, movie.cmiXres*movie.cmiYres*movie.cmiByPP)!= (int) (movie.cmiXres*movie.cmiYres*movie.cmiByPP) )	{
			failed = 1;
		}

		_CloseFile(hfile);
		if (failed)	{
			if (label) {
				label[0] = 0x0;
				GetString (string, IDS_LFBSAVINGTOFILEFAILED);
				strcat(label, string);
			}
		}
		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
}


LRESULT CALLBACK GrabberHookWin(int code, WPARAM wParam, LPARAM lParam)	{
/*unsigned*/ char label[MAX_PATH+128];

	if (code < 0) return(CallNextHookEx(hookHandle, code, wParam, lParam));
	if (wParam == VK_PAUSE)	{
	//if (wParam == VK_SNAPSHOT)	{
		if ( ( ((lParam >> 30) & 1) == 0x0) && (code == HC_ACTION) ) {
			//TOGGLELOGWIN;

			//SetWindowPos (movie.cmiWinHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | /*SWP_NOREDRAW |*/ SWP_NOSIZE /*| SWP_NOZORDER | SWP_NOSENDCHANGING*/);

			if (labelDraw)	{
				KillTimer (NULL, timerHandle);
				GrabberDeleteLabel ();
			}
			GrabberGrabLFB (0, label, NULL);
			GrabberCreateLabel (label);
			timerHandle = SetTimer (NULL, 0, 2500, GrabberTimerProc);
			DEBUG_ENABLEPROFILING;
			//return(1);
		}
	}
	//return(CallNextHookEx(hookHandle,code,wParam,lParam));
	return(0);
}

#ifdef	DOS_SUPPORT

void GrabberHookDos(int buffer)	{
/*unsigned*/ char label[MAX_PATH+128];
	
	//TOGGLELOGDOS;
	if (labelDraw)	{
		KillTimer(NULL, timerHandle);
		GrabberDeleteLabel();
	}
	GrabberGrabLFB(buffer, label, NULL);
	GrabberCreateLabel(label);
	DosSendSetTimerMessage();
}

#endif

int GrabberInit(int createAuxBuffers)	{

	if (!(config.Flags & CFG_GRABENABLE))
		return(TRUE);

	InitializeCriticalSection(&gCritSection);
	
	texturedata = NULL;
	bitmapInfo = NULL;

	compatibleDC = CreateCompatibleDC(NULL);
	
	texturedata = (unsigned short *) _GetMem(movie.cmiXres * 32 * 2);
	
	bitmapInfo = (BITMAPINFO *) _GetMem(sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
	
	ZeroMemory(bitmapInfo, sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
	bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth = movie.cmiXres;
	bitmapInfo->bmiHeader.biHeight = -32;
	bitmapInfo->bmiHeader.biPlanes = 1;
	bitmapInfo->bmiHeader.biBitCount = 8;
	bitmapInfo->bmiHeader.biCompression = BI_RGB;
	bitmapInfo->bmiColors[1].rgbRed = 0xFF;
	grabBitmap = CreateDIBSection(compatibleDC, bitmapInfo, DIB_RGB_COLORS, (void **) &bitmap, NULL,0);
	
	savedBitmap = (HBITMAP) SelectObject(compatibleDC, grabBitmap);
	grabFont = CreateFontIndirect(&font);
	savedFont = (HFONT) SelectObject(compatibleDC, grabFont);
	if (platform == PLATFORM_WINDOWS)
		hookHandle = SetWindowsHookEx(WH_KEYBOARD, GrabberHookWin, NULL, GetCurrentThreadId());

	fileSuffix = -1;
	
	return ((auxBuffersUsed = createAuxBuffers) ? rendererLfbDepth->CreateAuxBuffers (movie.cmiXres, 32) : 1);
}


void GrabberCleanUp()	{

	if (!(config.Flags & CFG_GRABENABLE)) return;
	if (platform == PLATFORM_WINDOWS)
		UnhookWindowsHookEx(hookHandle);
	if (labelDraw)	{
		KillTimer(NULL, timerHandle);
		GrabberDeleteLabel();
	}
	if (auxBuffersUsed)
		rendererLfbDepth->DestroyAuxBuffers ();

	SelectObject(compatibleDC, savedBitmap);
	SelectObject(compatibleDC, savedFont);
	DeleteObject(grabBitmap);
	DeleteDC(compatibleDC);
	if (texturedata) _FreeMem(texturedata);
	if (bitmapInfo) _FreeMem(bitmapInfo);
	texturedata = NULL;
	bitmapInfo = NULL;
	DeleteCriticalSection(&gCritSection);
}


void GrabberCreateLabel(/*unsigned*/ char *label)	{
unsigned int			i;
int						j,l,t,u,x,index;
unsigned short			*temp, *act, *acttexd;
SIZE					stringSize;
enum DXTextureFormats	namedTexFormat;
void*					texPtr;
unsigned int			texPitch;
unsigned int			*constPalette;
_pixfmt					*srcPixFmt;
_pixfmt					*namedFormatPixFmt;
	
#ifndef GLIDE3

	temp = (unsigned short *) _GetMem(32*32*2);
	ZeroMemory (labelTextures, sizeof(labelTextures));
	for(i = 0; i < movie.cmiXres*32; i++)	{
		texturedata[i] = 0x8010;
		bitmap[i] = 0;
	}

	SetTextAlign(compatibleDC, TA_CENTER);
	SetBkColor(compatibleDC, 0);
	SetTextColor(compatibleDC, 0x00FFFFFF);
	i = strlen(label);
	GetTextExtentPoint32(compatibleDC, label, i, &stringSize);
	stringSize.cx += 20;
	
	/* Ellipszisszerû átlászósággal kitöltjük a téglalapot */
	act = texturedata;
	t = stringSize.cx / 2;
	index = (int) sqrt(2.0f*t*t);
	u = movie.cmiXres / 2;
	for(j = -16; j <= 15; j++)	{
		for(l = -u; l < u; l++)	{
			x = (int) sqrt( (l*l) + (j*t/16)*(j*t/16.0f) );
			if (x<= (stringSize.cx / 2) - 24 ) *(act++) = 0x8010;
			else	{
				x = (((int) 0x7F) * (x - ((stringSize.cx / 2) - 24) )) / (index - ((stringSize.cx / 2) -24) );
				*(act++) = (unsigned short) (( (( ((int) 0x80) - x) ) << 8) + 0x10);
			}
		}
	}

	if (stringSize.cx > (int) movie.cmiXres) stringSize.cx = movie.cmiXres;
	labelXLeft = (movie.cmiXres - stringSize.cx) / 2;
	labelXRight = labelXLeft + stringSize.cx;
	TextOut(compatibleDC, movie.cmiXres / 2, 4, label, i);
		
	for(i = 0; i < movie.cmiXres*32; i++)	{
		if (bitmap[i]) texturedata[i] = 0xFF1C;
	}
		
	x = labelXLeft;
	index = 0;
	
	srcPixFmt = TexGetFormatPixFmt (GR_TEXFMT_ARGB_8332);
	if ((namedTexFormat = TexGetMatchingDXFormat (GR_TEXFMT_ARGB_8332)) != Pf_Invalid) {
		namedFormatPixFmt = TexGetDXFormatPixelFormat (namedTexFormat);
		constPalette = TexGetDXFormatConstPalette (namedTexFormat, TexGetFormatConstPaletteIndex (GR_TEXFMT_ARGB_8332));
		while(x < labelXRight)	{
			act = temp;
			acttexd = texturedata+x;
			for(j = 0; j < 32; j++)	{			
				for(i = 0;i < 32; i++)	{
					*(act++) = *(acttexd++);
				}
				acttexd += movie.cmiXres - 32;
			}
			i = runflags;
			runflags |= RF_CANFULLYRUN;
			
			if (rendererTexturing->CreateDXTexture (namedTexFormat, 32, 32, 1, labelTextures + index, 0)) {
				if ((texPtr = rendererTexturing->GetPtrToTexture (labelTextures[index], 0, &texPitch)) != NULL) {
					MMCConvertAndTransferData(srcPixFmt, namedFormatPixFmt,
											  0x0, 0x0, 0xFF,
											  temp, texPtr,
											  32, 32,
											  2*32, texPitch,
											  constPalette, NULL,
											  MMCIsPixfmtTheSame(srcPixFmt, namedFormatPixFmt) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
											  PALETTETYPE_CONVERTED);
					rendererTexturing->ReleasePtrToTexture (labelTextures[index], 0);
				}
			} else {
				labelTextures[index] = NULL;
			}
			runflags = i;
			x += 32;
			index++;
		}
	}
	_FreeMem(temp);

	labelDraw = 1;
#endif
}


void GrabberDeleteLabel()	{
int i;
	
	EnterCriticalSection (&gCritSection);

	labelDraw = 0;

	for(i = 0; i < 64; i++) {
		if (labelTextures[i]) {
			rendererTexturing->DestroyDXTexture (labelTextures[i]);
			labelTextures[i] = NULL;
		}
	}
	
	LeaveCriticalSection (&gCritSection);
}


void GrabberDrawLabel(int savelabelarea)	{
int			x, j, t;
RECT		saverect;
	
	if (labelDraw) {
		if (savelabelarea)	{
			saverect.left = 0;
			saverect.right = movie.cmiXres;
			saverect.top = (movie.cmiYres-32)/2;
			saverect.bottom = saverect.top+32;

			rendererLfbDepth->BlitToAuxBuffer (renderbuffer, &saverect);
		}

		rendererGeneral->SaveRenderState (RendererGeneral::ScreenShotLabel);
		rendererGeneral->SetRenderState (RendererGeneral::ScreenShotLabel, 0, 0, 0);
		
		x = labelXLeft;
		j = 0;
		while(x < labelXRight)	{
			rendererTexturing->SetTextureAtStage (0, labelTextures[j++]);

			t = (labelXRight - x) < 32 ? (labelXRight - x) : 32;

			DrawTile (x - 0.5f, movie.cmiYres / 2 - 16 - 0.5f, x + t - 0.5f, movie.cmiYres / 2 - 16 + 32 - 0.5f,
					  0.5f / 32.0f, 0.5f / 32.0f, ((float) (t - 1) + 0.5f) / 32.0f, 31.5f / 32.0f,
					  1.0f, 0.5f, 0);
			x += 32;
		}

		rendererGeneral->RestoreRenderState (RendererGeneral::ScreenShotLabel);
	}
}


/* Ez a függvény a bufferswap után hívódik meg: teljes képernyôn megvárja, hogy a */
/* flip befejezôdjön, majd helyreállítja a volt frontbuffert. */
void GrabberCleanUnvisibleBuff()	{

	rendererLfbDepth->BlitFromAuxBuffer (movie.cmiBuffNum - 1, 0, (movie.cmiYres-32) / 2);
}


void GrabberRestoreFrontBuff()	{

	rendererLfbDepth->BlitFromAuxBuffer (0, 0, (movie.cmiYres-32) / 2);
}
