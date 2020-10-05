/*--------------------------------------------------------------------------------- */
/* MOVIE.H - C-header for MOVIE                                                     */
/*           (my olden old frame-presenter frame system ported from DOS)            */
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

/*--------------------------------------------------------------------------*/
/*                         C Header for Movie                               */
/*--------------------------------------------------------------------------*/

#ifndef	MOVIE_H
#define MOVIE_H

/* typedefs                                                                 */
/*typedef unsigned long dword;*/
typedef int (*mvMessageProc) (HWND, UINT, WPARAM, LPARAM);

typedef struct  {

unsigned long   cmiFlags;
unsigned long   cmiBuffNum;
unsigned long   cmiFreq;
unsigned long   cmiXres;
unsigned long   cmiYres;
unsigned long   cmiDispFreq;
unsigned long   cmiConvRGB;
unsigned long   cmiConvByPP;
unsigned long   cmiRGB;
unsigned long   cmiByPP;
unsigned long   cmiBitPP;
unsigned long   cmiPitch;
unsigned long   cmiSurfaceMem;
HWND            cmiWinHandle;
char            *cmiWinTitle;
HICON   		cmiWinIcon;
HCURSOR  		cmiWinCursor;
WNDPROC         cmiWinProc;
unsigned long   cmiWinPosX;
unsigned long   cmiWinPosY;
unsigned long   cmiWinSizeX;
unsigned long   cmiWinSizeY;
unsigned long   cmiPicBX;
unsigned long   cmiPicBY;
unsigned long   cmiPicJX;
unsigned long   cmiPicJY;
unsigned long   cmiWinGapX;
unsigned long   cmiWinGapY;
LPDIRECTDRAW7	cmiDirectDraw;
LPDIRECT3D7		cmiDirect3D;
unsigned long   cmiFrames;
unsigned long   cmiZBitDepth;
unsigned long   cmiTexBitPP;
GUID			cmiDeviceGUID;
HWND			cmiWinPost;
LPDDPIXELFORMAT cmiPixelFormat;
void			(*cmiCallWhenMIRQ)();
void			(*cmiCallPostC)();
unsigned long	cmiStencilBitDepth;

} MOVIEDATA;

/* State flags */
#define MV_FULLSCR      0x1
#define MV_WINDOWED     0x2
#define MV_VIDMEM       0x4
#define MV_INITOK       0x8
#define MV_MINIMIZED    0x10
#define MV_MAXIMIZED    0x20
#define MV_WINDOWSIZEOK 0x40
#define MV_CURSORHIDED  0x80
#define MV_3D		0x100
#define MV_NOCONSUMERCALL 0x400
#define MV_ANTIALIAS	0x800
#define MV_STENCIL		0x1000

/* Z-buffer flags */
#define MVZBUFF_AUTO	0x80000000

/* Texture flags */
#define MVTEXTFORM_AUTO	0x80000000

/*------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

    int _stdcall EnumDisplayDevs(LPDDENUMCALLBACK func);
    int _stdcall EnumDispModes(GUID *device, LPDDENUMMODESCALLBACK2 func, \
				    unsigned int mode);
	void _stdcall mvGetDispDevCaps(GUID *device, DDCAPS *caps);
	int _stdcall GetActDispMode(GUID *device, LPDDSURFACEDESC2 surface);
    int _stdcall InitDraw(MOVIEDATA *movie, GUID *device);
    void _stdcall UninitDraw();
    int _stdcall InitMovie();
	int _stdcall ReInitMovie(MOVIEDATA *movie);
    void _stdcall CallConsumer(int);
    void _stdcall DeInitMovie();
    unsigned int _stdcall GetDefMovieWinProc();
    void _stdcall SetFullScreenMode();
    void _stdcall SetWindowedMode();
    void _stdcall ToggleScreenMode();
    LPDIRECTDRAWSURFACE7 _stdcall BeginDraw();
    void _stdcall EndDraw();
    int _stdcall mvRestoreSurface(VOID *surface);
    int _stdcall mvLockSurface(VOID FAR *surface);
    void _stdcall mvUnlockSurface(VOID FAR *surface);
    LPDIRECTDRAWSURFACE7 _stdcall GABA();
    LPDIRECTDRAWSURFACE7 _stdcall GetFrontBuffer();
    LPDIRECTDRAWSURFACE7 _stdcall GetThirdBuffer();
    void _stdcall ShowFrontBuffer();
    int _stdcall GetBufferNumPending();
    LPDIRECTDRAWSURFACE7 _stdcall CreateZBuffer();
	int _stdcall DestroyZBuffer();
	int _stdcall InitTextureLoad(LPDIRECT3DDEVICE7 dev);
	int _stdcall OpenTexture(BITMAPINFO *bitmap);
	LPDIRECTDRAWSURFACE7 _stdcall CloseTexture();
	int _stdcall CreateAlphaOnTexture(LPDIRECTDRAWSURFACE7 texture, unsigned char *alpha);
	int _stdcall DeInitTextureLoad();

#ifdef __cplusplus
}
#endif

extern	MOVIEDATA					movie;

#endif