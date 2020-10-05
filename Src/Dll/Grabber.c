/*--------------------------------------------------------------------------------- */
/* Grabber.c - dgVoodoo implementation of screen capturing                          */
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
/* dgVoodoo: Grabber.c																		*/
/*			 Képernyõmentés																	*/
/*------------------------------------------------------------------------------------------*/

#include	"dgVoodooBase.h"

#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>
#include	<math.h>
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

#include	"MMConv.h"

#include	"Resource.h"

/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák  ..................................*/

typedef struct	{
	
	int						labeled;
	LPDIRECTDRAWSURFACE7	lpDDSavedArea;

} _bufferlabelinfo;

/*-------------------------------------------------------------------------------------------*/

static CRITICAL_SECTION		gCritSection;
static HDC					compatibleDC;
static HBITMAP				grabBitmap, savedBitmap;
static unsigned char		*bitmap;
static BITMAPINFO			*bitmapInfo;
static unsigned short		*texturedata;
static LPDIRECTDRAWSURFACE7 labelTextures[64];
static int					labelXLeft, labelXRight;
static LOGFONT				font =	{-19, 0, 0, 0, 700, 0, 0, 0, 238, 3, 2, 1, 34, "Verdana"};
static HFONT				grabFont, savedFont;
static HHOOK				hookHandle;
static int					labeldraw = 0;
static int					labelcreated = 0;
static int					fileSuffix = -1;
static LPDIRECTDRAWSURFACE7	lpDDLabelSaveFront, lpDDLabelSaveBack;
static _bufferlabelinfo		buffLabelInfo[3];
static _pixfmt				bmpPixFmt8 = {8, 0, 0, 0, 0,  0, 0, 0, 0};
static _pixfmt				bmpPixFmt16 = {16, 5, 5, 5, 0,  10, 5, 0, 0};
static _pixfmt				bmpPixFmt32 = {32, 8, 8, 8, 0,  16, 8, 0, 0};
UINT						timerHandle;


/*-------------------------------------------------------------------------------------------*/
/*....................................... Függvények ........................................*/

void CALLBACK GrabberTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)	{

	labeldraw=0;
	KillTimer(NULL, timerHandle);
	GrabberDeleteLabel();
}


void GrabberGenerateFileName(unsigned char *filename)	{
static unsigned char	appFileDir[256];
static unsigned char	appFileName[256];
unsigned char			*srcFileName;
unsigned char			suffix[4];
WIN32_FIND_DATA			findData;
int						counter;
HANDLE					ffHandle;
unsigned char			c0,c1,c2;
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
	suffix[0] = (fileSuffix / 100) + '0';
	suffix[1] = ((fileSuffix / 10) % 10) + '0';
	suffix[2] = (fileSuffix % 10) + '0';
	suffix[3] = 0x0;
	filename[0] = 0x0;
	strcat(filename, appFileDir);
	strcat(filename, "\\");
	strcat(filename, appFileName);
	strcat(filename, suffix);
	strcat(filename, ".bmp");
}


void GrabberRestoreFrontBuff()	{

	if (buffLabelInfo[0].labeled)	{
		lpDDFrontBuff->lpVtbl->BltFast(lpDDFrontBuff, 0, (movie.cmiYres-32) / 2, buffLabelInfo[0].lpDDSavedArea, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	}
}


void GrabberGrabLFB(unsigned char *label, PALETTEENTRY *lpPalette)	{
HGLOBAL				hMem;
BITMAPINFO			*bitmap;
BITMAPFILEHEADER	bmpheader;
unsigned char		*p;
unsigned short		*q;
int					i;
HANDLE				hfile;
DDSURFACEDESC2		lfb;
RGBQUAD				*lpPalettePlaced;
unsigned char		GrabFileName[MAX_PATH];
int					failed;
_pixfmt				*bmpPixFmt;
unsigned char		string[MAXSTRINGLENGTH];

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

	ZeroMemory(&lfb, sizeof(DDSURFACEDESC2));
	lfb.dwSize = sizeof(DDSURFACEDESC);

	GrabberRestoreFrontBuff();

	switch(movie.cmiBitPP)	{
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
	
	lpDDFrontBuff->lpVtbl->Lock(lpDDFrontBuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_READONLY, NULL);
	//lpDDBackbuff->lpVtbl->Lock(lpDDBackBuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_READONLY, NULL);
	
	MMCConvertAndTransferData(&backBufferFormat, bmpPixFmt,
							  0, 0, 0,
							  lfb.lpSurface, q,
							  movie.cmiXres, movie.cmiYres,
							  -lfb.lPitch, movie.cmiXres*movie.cmiByPP,
							  NULL, NULL,
							  MMCIsPixfmtTheSame(&backBufferFormat, bmpPixFmt) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
							  0);
	
	lpDDFrontBuff->lpVtbl->Unlock(lpDDFrontBuff, NULL);
	//lpDDBackbuff->lpVtbl->Unlock(lpDDBackBuff, NULL);

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
unsigned char label[MAX_PATH+128];

	if (code < 0) return(CallNextHookEx(hookHandle, code, wParam, lParam));
	if (wParam == VK_PAUSE)	{
	//if (wParam == VK_SNAPSHOT)	{
		if ( ( ((lParam >> 30) & 1) == 0x0) && (code == HC_ACTION) ) {
			//TOGGLELOGWIN;
			if (labeldraw)	{
				KillTimer(NULL, timerHandle);
				GrabberDeleteLabel();
			}
			labeldraw = 1;
			//logenabled=1;
			GrabberGrabLFB (label, NULL);
			GrabberCreateLabel (label);
			timerHandle = SetTimer(NULL, 0, 2500, GrabberTimerProc);
			//return(1);
		}
	}
	//return(CallNextHookEx(hookHandle,code,wParam,lParam));
	return(0);
}

#ifdef	DOS_SUPPORT

void GrabberHookDos()	{
unsigned char label[MAX_PATH+128];
	
	//TOGGLELOGDOS;
	if (labeldraw)	{
		KillTimer(NULL, timerHandle);
		GrabberDeleteLabel();
	}
	labeldraw = 1;		
	GrabberGrabLFB(label, NULL);
	GrabberCreateLabel(label);
	DosSendSetTimerMessage();
}

#endif

int GrabberInit()	{
DDSURFACEDESC2	labelBufferDesc;
unsigned		int i;

	if (!(config.Flags & CFG_GRABENABLE)) return(TRUE);

	InitializeCriticalSection(&gCritSection);
	
	lpDDLabelSaveFront = lpDDLabelSaveBack = NULL;
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
	grabBitmap = CreateDIBSection(compatibleDC, bitmapInfo, DIB_RGB_COLORS, &bitmap, NULL,0);
	
	savedBitmap = SelectObject(compatibleDC, grabBitmap);
	grabFont = CreateFontIndirect(&font);
	savedFont = SelectObject(compatibleDC, grabFont);
	if (platform == PLATFORM_WINDOWS)
		hookHandle = SetWindowsHookEx(WH_KEYBOARD, GrabberHookWin, NULL, GetCurrentThreadId());

	CopyMemory(&labelBufferDesc, &lfb, sizeof(DDSURFACEDESC2));
	labelBufferDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	labelBufferDesc.dwWidth = movie.cmiXres;
	labelBufferDesc.dwHeight = 32;
	labelBufferDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	labelBufferDesc.ddsCaps.dwCaps2 = 0;

	for(i = 0; i < movie.cmiBuffNum; i++)	{
		if (lpDD->lpVtbl->CreateSurface(lpDD, &labelBufferDesc, &buffLabelInfo[i].lpDDSavedArea, NULL) != DD_OK) return(FALSE);
		buffLabelInfo[i].labeled = 0;
	}
	fileSuffix = -1;
	return(TRUE);
}


void GrabberCleanUp()	{

	if (!(config.Flags & CFG_GRABENABLE)) return;
	if (platform == PLATFORM_WINDOWS)
		UnhookWindowsHookEx(hookHandle);
	if (labeldraw)	{
		labeldraw = 0;
		KillTimer(NULL, timerHandle);
		GrabberDeleteLabel();
	}
	if (lpDDLabelSaveFront) lpDDLabelSaveFront->lpVtbl->Release(lpDDLabelSaveFront);
	if (lpDDLabelSaveBack) lpDDLabelSaveBack->lpVtbl->Release(lpDDLabelSaveBack);
	SelectObject(compatibleDC, savedBitmap);
	SelectObject(compatibleDC, savedFont);
	DeleteObject(grabBitmap);
	DeleteDC(compatibleDC);
	if (texturedata) _FreeMem(texturedata);
	if (bitmapInfo) _FreeMem(bitmapInfo);
	lpDDLabelSaveFront = lpDDLabelSaveBack = NULL;
	texturedata = NULL;
	bitmapInfo = NULL;
	DeleteCriticalSection(&gCritSection);
}


void GrabberCreateLabel(unsigned char *label)	{
unsigned int	i;
int				j,l,t,u,x,index;
unsigned short	*temp, *act, *acttexd;
SIZE			stringSize;
unsigned int	namedTexFormat;
void*			texPtr;
unsigned int	texPitch;
unsigned int	*constPalette;
_pixfmt			*srcPixFmt;
_pixfmt			*namedFormatPixFmt;
	
#ifndef GLIDE3
	labelcreated = 1;
	temp = (unsigned short *) _GetMem(32*32*2);
	for(i = 0; i < 64; i++) labelTextures[i] = NULL;
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
	index = (int) sqrt(2*t*t);
	u = movie.cmiXres / 2;
	for(j = -16; j <= 15; j++)	{
		for(l = -u; l < u; l++)	{
			x = (int) sqrt( (l*l) + (j*t/16)*(j*t/16) );
			if (x<= (stringSize.cx / 2) - 24 ) *(act++) = 0x8010;
			else	{
				x = (((int) 0x7F) * (x - ((stringSize.cx / 2) - 24) )) / (index - ((stringSize.cx / 2) -24) );
				*(act++) = ( (( ((int) 0x80) - x) ) << 8) + 0x10;
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
			
			if (TexCreateDXTexture (namedTexFormat, 32, 32, 1, labelTextures+index)) {
				if ((texPtr = TexGetPtrToTexture (labelTextures[index], &texPitch)) != NULL) {
					MMCConvertAndTransferData(srcPixFmt, namedFormatPixFmt,
											  0x0, 0x0, 0xFF,
											  temp, texPtr,
											  32, 32,
											  2*32, texPitch,
											  constPalette, NULL,
											  MMCIsPixfmtTheSame(srcPixFmt, namedFormatPixFmt) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
											  PALETTETYPE_CONVERTED);
					TexReleasePtrToTexture (labelTextures[index]);
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
#endif
}


void GrabberDeleteLabel()	{
int i;
	
	EnterCriticalSection (&gCritSection);

	for(i = 0; i < 64; i++) {
		if (labelTextures[i])
			TexDestroyDXTexture (labelTextures[i]);
	}
	labelcreated = 0;
	
	LeaveCriticalSection (&gCritSection);
}


void GrabberDrawLabel(int savelabelarea)	{
int						x, j, t;
RECT					saverect;
DWORD					savedState[128];
LPDIRECTDRAWSURFACE7	lpDDBuff, lpDDRenderTarget;
	
	if (!labeldraw) return;

	if (savelabelarea)	{
		saverect.left = 0;
		saverect.right = movie.cmiXres;
		saverect.top = (movie.cmiYres-32)/2;
		saverect.bottom = saverect.top+32;

		/* Az átmeneti puffer elveszett? */
		lpDDBuff = buffLabelInfo[renderbuffer].lpDDSavedArea;
		if (lpDDBuff->lpVtbl->IsLost(lpDDBuff) != DD_OK) lpDDBuff->lpVtbl->Restore(lpDDBuff);
		lpDDRenderTarget = (renderbuffer == GR_BUFFER_FRONTBUFFER) ? lpDDFrontBuff :
			( (renderbuffer == GR_BUFFER_BACKBUFFER) ? lpDDBackBuff : GetThirdBuffer() );

		/* A hamarosan frontbufferré váló backbuffer megfelelõ területének elmentése */
		lpDDBuff->lpVtbl->BltFast(lpDDBuff, 0, 0, lpDDRenderTarget, &saverect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	}
	buffLabelInfo[renderbuffer].labeled = savelabelarea;

	GlideGetD3DState(savedState);
	
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);	
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_COLORKEYENABLE, FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGENABLE, FALSE);

	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	

	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	
	x = labelXLeft;
	j = 0;
	while(x < labelXRight)	{
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, 0, labelTextures[j++]);

		t = (labelXRight - x) < 32 ? (labelXRight - x) : 32;

		DrawTile (x - 0.5f, movie.cmiYres / 2 - 16 - 0.5f, x + t - 0.5f, movie.cmiYres / 2 - 16 + 32 - 0.5f,
				  0.5f / 32.0f, 0.5f / 32.0f, ((float) (t - 1) + 0.5f) / 32.0f, 31.5f / 32.0f,
				  1.0f, 0.5f);
		x += 32;
	}
	GlideSetD3DState(savedState);
}


/* Ez a függvény a bufferswap után hívódik meg: teljes képernyôn megvárja, hogy a */
/* flip befejezôdjön, majd helyreállítja a volt frontbuffert. */
void GrabberCleanUnvisibleBuff()	{
LPDIRECTDRAWSURFACE7 lpDDBuff, lpDDToBuff;
unsigned int i;

	if (buffLabelInfo[0].labeled)	{
		i = movie.cmiBuffNum - 1;
		lpDDToBuff = ( (i == GR_BUFFER_BACKBUFFER) ? lpDDBackBuff : GetThirdBuffer() );
		if (!(config.Flags & CFG_WINDOWED))
			while(lpDDToBuff->lpVtbl->GetFlipStatus(lpDDToBuff, DDGFS_ISFLIPDONE) == DDERR_WASSTILLDRAWING);
	
		lpDDToBuff->lpVtbl->BltFast(lpDDToBuff, 0, (movie.cmiYres-32)/2, buffLabelInfo[0].lpDDSavedArea, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	}
	lpDDBuff = buffLabelInfo[0].lpDDSavedArea;
	for(i = 0; i < movie.cmiBuffNum; i++)	{
		buffLabelInfo[i].labeled = buffLabelInfo[i+1].labeled;
		buffLabelInfo[i].lpDDSavedArea = buffLabelInfo[i+1].lpDDSavedArea;
	}
	buffLabelInfo[i-1].lpDDSavedArea = lpDDBuff;
	buffLabelInfo[i-1].labeled = 0;
}
