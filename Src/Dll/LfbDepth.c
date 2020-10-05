/*--------------------------------------------------------------------------------- */
/* LfbDepth.c - Glide implementation, stuffs related to Linear Frame / Depth buffer */
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
/* dgVoodoo: LfbDepth.c																		*/
/*			 Linear frame buffer és depth buffering függvények								*/
/*------------------------------------------------------------------------------------------*/

/* LFB mûveletek: a front- és backbuffer-t külön-külön is bírja lockolni, akár egyszerre */
/* írásra és olvasásra is. Ugyanez igaz az aux bufferre is, ha az a triplapuffer eleme. */
/* Az mélységi és alphapuffert nem lehet lockolni. */
/* Támogatott írási formátumok: RGB565, RGB555, ARGB1555, RGB888, ARGB8888 */

/* A DirectX-pufferek lehetnek 16 vagy 32 bitesek: */
/*  32 bit esetén a támogatott formátumok: RGB888, ARGB8888 */
/*  16 bit esetén a támogatott formátumok: RGB555, RGB565, ARGB1555, ARGB4444 */
/* A konvertáláshoz a MultiMediaConvertert használja. */

#include	"dgVoodooBase.h"
#include	"Version.h"
#include	"Main.h"
#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>

#include	"dgVoodooConfig.h"
#include	"DDraw.h"
#include	"D3d.h"
#include	"Dgw.h"
#include	"Movie.h"
#include	"dgVoodooGlide.h"
#include	"Debug.h"
#include	"Log.h"
#include	"Draw.h"
#include	"LfbDepth.h"
#include	"Texture.h"
#include	"Grabber.h"
#include	"Dos.h"

#include	"MMConv.h"

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók ...... ..................................*/

/* Képességbitek */
#define	ZBUFF_OK			1
#define WBUFF_OK			2

/* Konstansok a depthbuffering állapotához */
#define DBUFFER_Z			1
#define DBUFFER_W			2

#define WNEAR				0.01f
#define WFAR				65536.0f

/* Hardver blittelési képeségei */
#define BLT_FASTBLT			1			/* A segédpuffereket a videómemóriában hoztuk létre */
#define BLT_CANMIRROR		2			/* A hardver képes az y-tükrözésre blittelés közben */
#define BLT_CANSRCCOLORKEY	4			/* A hardver támogatja a source colorkeying-es blittelést */
#define BLT_CANBLTSYSMEM	8			/* A system memóriába is képes hardveresen blittelni - nem használt */

/* Az lfb mûködését szabályzó flagek */
#define LFB_RESCALE_CHANGES_ONLY	1	/* A felbontások eltérésekor csak a változásokat skálázza át és */
										/* írja a pufferbe, és nem az egész puffert. */
										/* Ez csak a blue screen technikával használható (aka gyorsírás) */
#define LFB_FORCE_FASTWRITE			2	/* Gyorsírás kényszerítése egyezõ formátumokhoz is */
#define LFB_NO_MATCHING_FORMATS		4	/* A formátumokat soha nem tekinti egyezõnek */

/* Az egyes belsõ pufferek */
#define BUFF_COLOR			0			/* Color buffer, azaz maga a back- vagy frontbuffer (mindig hw) */
#define BUFF_RESIZING		1			/* Belsõ konvertálópuffer az átméretezéshez vagy pitch-átalakításhoz (hw vagy sw) */
#define BUFF_CONVERTING		2			/* Szoftveres buffer a formátumkonverzióhoz (mindig sw) */
#define BUFF_DEPTH			3			/* Maga a mélységi buffer */


/* A lock struktúra locktype mezôje bitjeinek definiálása */
#define LOCK_TEMPDISABLE	0x1			/* A hardverpuffert ideiglenesen nem lehet zárolni */
#define LOCK_LOCKED			0x2			/* legalább az egyikre lockolva = read | write */
#define LOCK_ISLOCKED		0x3			/* Lekérdezési maszk: a puffer logikailag le van zárolva? */

/* Flagek az átméretezõpufferhez */
#define LOCK_RCACHED		0x4			/* a puffer tartalma érvényes, közvetlen írásra és olvasásra alkalmas */
#define LOCK_RFASTCACHED	0x8			/* a puffer tartalma érvényes, tartalma a gyorsíráshoz megfelelõ */
#define LOCK_RLOWERORIGIN	0x10		/* a puffer origója a bal alsó sarok */
#define LOCK_RSOFTWARE		0x20		/* tartalom a szoftveres pufferben */
#define LOCK_RNOTFLUSHED	0x40        /* a pufferbe történt írások nincsenek kiürítve */
#define LOCK_RSTATEMASK		(LOCK_RCACHED | LOCK_RFASTCACHED | LOCK_RLOWERORIGIN | LOCK_RSOFTWARE)

/* Konvertáló terület */
#define LOCK_CCACHED		0x80
#define LOCK_CFASTCACHED	0x100
#define LOCK_CLOWERORIGIN	0x200
#define LOCK_CNOTFLUSHED	0x400
#define LOCK_CTEXTURE		0x800
#define LOCK_CSTATEMASK		(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CLOWERORIGIN | LOCK_CTEXTURE)

#define LOCK_DIRTY			0x1000
#define LOCK_PIPELINE		0x2000
#define LOCK_LOWERORIGIN	0x4000		/* Globális: a zárolás alatt a puffer origója a bal alsó sarok */

#define LOCK_CACHEBITS		(LOCK_RCACHED | LOCK_RFASTCACHED | LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE)


#define LI_LOCKTYPEREAD		0
#define LI_LOCKTYPEWRITE	1

/* A lock struktúra disable mezõjének bitjei */
#define DISABLE_READ		0x1			/* A puffer olvasásra való zárolása tiltott (használaton kívüli puffer címe) */
#define DISABLE_WRITE		0x2			/* A puffer írásra való zárolása tiltott (használaton kívüli puffer címe) */
#define DISABLE_ALL			0x3			/* A puffer írásra és olvasásra is elérhetetlen */

/* Kulcsszín értéke a gyorsíráshoz, 32 bit ARGB */
#define DEFAULTCOLORKEY		0xFF0008FF

#define LFBMAXPIXELSIZE		4

#define	TILETEXTURE_NUM		3


/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák  ..................................*/

/* Csak Glide 2.11-hez: a zárolás egyes paraméterei */
typedef struct	{

	FxBool				pixelPipeLine;
	GrLfbWriteMode_t	writeMode;
	GrOriginLocation_t	origin;
	void				*writePtr[3];
	void				*readPtr[3];
	GrBuffer_t			fbRead, fbWrite;
	int					begin;

} Glide211LockInfo;


typedef struct	{
	
	int					lockType;
	int					lowerOrigin;
	_pixfmt				*lockFormat;
	unsigned int		colorKey;
	unsigned int		colorKeyMask;
	int					useTextures;
	int					textureFormatIndex;
	int					viaPixelPipeline;
	RECT				lockRect;
	void				*buffPtr;
	unsigned int		buffPitch;
	
} LockInfo;

/* Egy adott eléréstípus (írás, olvasás) adatai */
typedef struct	{

	unsigned int lockNum;				/* Adott elérésre történõ zárolások száma */

} _plockinfo;


typedef struct	{

	LPDIRECTDRAWSURFACE7	tTexture[TILETEXTURE_NUM];
	_pixfmt					tTextureFormat;

} TileTextureData;


enum FormatIndex {

	format565 = 0,
	format555,
	format1555,
	format888,
	format8888,
	numberOfFormats,
	unsupportedFormat
};


/* Egy bufferhez (front, back, aux) tartozó lockinfo */
typedef struct	{
	
	LPDIRECTDRAWSURFACE7	lpDDbuff;		/* Color buffer DirecDraw-objektuma (BUFF_COLOR) */
	LPDIRECTDRAWSURFACE7	lpDDConvbuff;	/* konvertálópuffer (BUFF_RESIZING) */
	LPDIRECTDRAWSURFACE7	lpDDConvbuff2;	/* szoftveres konvertálópuffer (BUFF_RESIZING) */
	unsigned char			*temparea;		/* átmeneti terület, ahová és ahonnan konvertálunk (BUFF_CONVERTING) */
	unsigned char			*temparea_client;  /* ugyanezen terület címe a kliens címterében (DOS, XP-hez) */
	unsigned int			colorKey;		/* Gyorsírás esetén a felhasznált colorkey ARGB-ben */
	unsigned int			colorKeyMask;	/* Gyorsírás esetén a colorkeyhez felhasznált maszk ARGB-ben */
	_plockinfo				writeLockInfo;
	_plockinfo				readLockInfo;
	void					*ptr;
	int						stride;
	unsigned int			locktype;		/* puffer állapotát jelzõ bitek */
	unsigned int			usedBuffer;
	unsigned int			disabled;		/* zárolás tiltásának fajtái */
	_pixfmt					*lockformat;
	RECT					lockRect;
	int						textureFormatIndex;
	GrLfbWriteMode_t		writeMode;
	int						bufferNum;

} _lock;


/* A három buffer lockinfoja, sorrendben: front, back, aux (a glide konstansok szerint) */
_lock					bufflocks[3];
_lock					templocks[3];
unsigned int			bltcaps;
unsigned int			lfbflags;
int						convbuffxres;
LPDIRECTDRAWSURFACE7	lpDDCopyTempBuff;
void					*CopyTempArea;
GrColorFormat_t			readcFormat, writecFormat;
DDSURFACEDESC2			lfb;
_pixfmt					backBufferFormat;
unsigned int			constAlpha;
unsigned int			constDepth;

#ifdef GLIDE1

Glide211LockInfo		glide211LockInfo;

#endif

/* Adott írási formátumhoz és módhoz tartozó formátumok, argb,abgr,rgba,bgra */
_pixfmt	wMode565[]		= { {16, 5,6,5,0,11,5,0,0}, {16, 5,6,5,0,0,5,11,0}, {16, 5,6,5,0,11,5,0,0}, {16, 5,6,5,0,0,5,11,0} };
_pixfmt wMode555[]		= { {16, 5,5,5,0,10,5,0,0}, {16, 5,5,5,0,0,5,10,0}, {16, 5,5,5,0,11,6,1,0}, {16, 5,5,5,0,1,6,11,0} };
_pixfmt wMode1555[]		= { {16, 5,5,5,1,10,5,0,15}, {16, 5,5,5,1,0,5,10,15}, {16, 5,5,5,1,11,6,1,0}, {16, 5,5,5,1,1,6,11,0} };
_pixfmt wMode888[]		= { {32, 8,8,8,0,16,8,0,0}, {32, 8,8,8,0,0,8,16,0}, {32, 8,8,8,0,24,16,8,0}, {32, 8,8,8,0,8,16,24,0} };
_pixfmt wMode8888[]		= { {32, 8,8,8,8,16,8,0,24}, {32, 8,8,8,8,0,8,16,24}, {32, 8,8,8,8,24,16,8,0}, {32, 8,8,8,8,8,16,24,0} };
_pixfmt *wModeTable[]	= { wMode565, wMode555, wMode1555, wMode888, wMode8888 };

enum FormatIndex		writeModeToIndex[]	= {format565, format555, format1555, unsupportedFormat, format888, format8888,
											   unsupportedFormat, unsupportedFormat, unsupportedFormat,
											   unsupportedFormat, unsupportedFormat, unsupportedFormat,
											   unsupportedFormat, unsupportedFormat, unsupportedFormat, unsupportedFormat};
int						locked;
unsigned int			depthbuffering;
unsigned int			depthcaps, depthbuffercreated;
GrDepthBufferMode_t		actdepthmode;
LPDIRECTDRAWSURFACE7	lpDDdepth;

DWORD					saved3DStatesNoPipeline[] = { D3DRENDERSTATE_ALPHATESTENABLE, D3DRENDERSTATE_ALPHABLENDENABLE,
													  D3DRENDERSTATE_ZENABLE, D3DRENDERSTATE_CULLMODE,
													  D3DRENDERSTATE_COLORKEYENABLE, D3DRENDERSTATE_FOGENABLE,
													  D3DRENDERSTATE_TEXTUREFACTOR,
													  STATEDESC_DELIMITER };

DWORD					saved3DStatesPipeline[] = { D3DRENDERSTATE_CULLMODE, D3DRENDERSTATE_COLORKEYENABLE,
													D3DRENDERSTATE_TEXTUREFACTOR, D3DRENDERSTATE_ZWRITEENABLE,
												    STATEDESC_DELIMITER };

DWORD					saved3DStageStates[]	= { 0, D3DTSS_COLOROP, 0, D3DTSS_COLORARG1, 0, D3DTSS_ALPHAOP, 0, D3DTSS_ALPHAARG1,
													0, D3DTSS_MAGFILTER, 0, D3DTSS_MINFILTER, 1, D3DTSS_COLOROP, STATEDESC_DELIMITER};

DWORD					saved3DTextures[]		= { 0, STATEDESC_DELIMITER };


unsigned int			tileTexSizeX = 256, tileTexSizeY = 256;
TileTextureData			tileTextures[numberOfFormats];
unsigned char			tileTexFormatsFor565[] = {Pf16_RGB565, Pf16_RGB555, Pf32_RGB888, Pf32_ARGB8888, Pf_Invalid};
unsigned char			tileTexFormatsFor555[] = {Pf16_RGB555, Pf16_RGB565, Pf16_ARGB1555, Pf32_RGB888, Pf32_ARGB8888, Pf_Invalid};
unsigned char			tileTexFormatsFor1555[] = {Pf16_ARGB1555, Pf32_ARGB8888, Pf_Invalid};
unsigned char			tileTexFormatsFor888[] = {Pf32_RGB888, Pf32_ARGB8888, Pf_Invalid};
unsigned char			tileTexFormatsFor8888[] = {Pf32_ARGB8888, Pf_Invalid};
unsigned char			*tileTexFormats[] = { tileTexFormatsFor565, tileTexFormatsFor555, tileTexFormatsFor1555,
										      tileTexFormatsFor888, tileTexFormatsFor8888 };

/*------------------------------------------------------------------------------------------*/
/*........................... LFB elérését lehetõvé tevõ függvények  .......................*/

void	LfbGetBackBufferFormat() {
	
	ZeroMemory (&lfb, sizeof(DDSURFACEDESC2));
	lfb.dwSize = sizeof(DDSURFACEDESC2);
	lfb.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	lpDDBackBuff->lpVtbl->GetPixelFormat(lpDDBackBuff, &lfb.ddpfPixelFormat);

	MMCConvertToPixFmt(&lfb.ddpfPixelFormat, &backBufferFormat, 0);
}


/* Lekérdezi az LFB formátumát, s megnézi, hogy támogatja-e azt vagy sem. Ennek megfelelõen */
/* Az egyes bufferek zárolhatóságát is beállítja. */
int		LfbGetAndCheckLFBFormat()	{
unsigned int		disabletype;

	DEBUG_BEGIN("LfbGetAndCheckLFBFormat", DebugRenderThreadId, 85);
	
	LfbGetBackBufferFormat ();

	bufflocks[0].disabled = bufflocks[1].disabled = bufflocks[2].disabled = DISABLE_ALL;
	bufflocks[0].bufferNum = 0;
	bufflocks[1].bufferNum = 1;
	bufflocks[2].bufferNum = 2;
	disabletype = ((config.Flags2 & CFG_DISABLELFBREAD) ? DISABLE_READ : 0) | ((config.Flags2 & CFG_DISABLELFBWRITE) ? DISABLE_WRITE : 0);
	
	switch(backBufferFormat.BitPerPixel)	{
		case 32:
			if ( (backBufferFormat.RBitCount == 8) && (backBufferFormat.GBitCount == 8) &&
				(backBufferFormat.BBitCount == 8) && ((backBufferFormat.ABitCount == 8) || (backBufferFormat.ABitCount == 0)) )	{
			} else {
				disabletype = DISABLE_ALL;
				
				DEBUGLOG(0,"\n   Error: LfbGetAndCheckLFBFormat: unsupported DirectDraw LFB format in 32 bit mode: LFB access will be disabled in Glide session.");
				DEBUGLOG(1,"\n   Hiba: LfbGetAndCheckLFBFormat: a DirectDraw LFB formátuma nem támogatott 32 bites módban: Az LFB elérése tiltva lesz.");
			}
			break;
		
		case 16:
			if ( (backBufferFormat.RBitCount == 5) && ( (backBufferFormat.GBitCount == 5) || (backBufferFormat.GBitCount == 6) ) &&
				 (backBufferFormat.BBitCount == 5) && (backBufferFormat.ABitCount <= 1) )	{
			} else {
				disabletype = DISABLE_ALL;

				DEBUGLOG(0,"\n   Error: LfbGetAndCheckLFBFormat: unsupported DirectDraw LFB format in 16 bit mode: LFB access will be disabled in Glide session.");
				DEBUGLOG(1,"\n   Hiba: LfbGetAndCheckLFBFormat: a DirectDraw LFB formátuma nem támogatott 16 bites módban: Az LFB elérése tiltva lesz.");
			}
			break;
		
		default:
		
				DEBUGLOG(0,"\n   Fatal error: LfbGetAndCheckLFBFormat: unknown DirectDraw LFB format, it is not 16 or 32 bit mode: LFB access will be disabled in Glide session.");
				DEBUGLOG(1,"\n   Végzetes hiba: LfbGetAndCheckLFBFormat: ismeretlen DirectDraw LFB-formátum, nem 16 vagy 32 bites: Az LFB elérése tiltva lesz.");
			break;
	}
	bufflocks[0].disabled = bufflocks[1].disabled = disabletype;
	bufflocks[2].disabled = (movie.cmiBuffNum == 3) ? disabletype : DISABLE_ALL;
	return(1);
	
	DEBUG_END("LfbGetAndCheckLFBFormat", DebugRenderThreadId, 85);
}


/* Ez a függvény hozza létre a szükséges segéd (konvertáló) puffereket. */
int GlideLFBAllocConvBuffs()	{
unsigned int	i, hwbuff;
DDSURFACEDESC2	ddsd;
DDCAPS			caps;

	DEBUG_BEGIN("GlideLFBAllocConvBuffs", DebugRenderThreadId, 86);
	
	/* Segédpufferek mutatóinak nullázása: nem biztos, hogy szükség lesz rájuk. */
	for(i = 0; i < 3; i++) bufflocks[i].lpDDConvbuff = NULL;
	
	/* Ha az LFB-elérés tiltva, akkor készen vagyunk. */
	if ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) == (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) ) return(1);

	ZeroMemory(&caps, sizeof(DDCAPS));
	caps.dwSize = sizeof(DDCAPS);
	lpDD->lpVtbl->GetCaps(lpDD, &caps, NULL);
	bltcaps = 0;
	
	/* Feltételezzük, hogy ha a kártya képes az egyes blittelési módokra, akkor azok kombinációira is képes! */
	/* (ezt sajnos a flagekbõl nem lehet kideríteni) */
	if (hwbuff = (caps.dwCaps & DDCAPS_BLT))	{
		if ( (caps.dwCaps & DDCAPS_COLORKEY) && (caps.dwCKeyCaps & DDCKEYCAPS_SRCBLT) ) bltcaps |= BLT_CANSRCCOLORKEY;
		if ( (caps.dwFXCaps & DDFXCAPS_BLTMIRRORUPDOWN) ) bltcaps |= BLT_CANMIRROR;
	}
	if (drawresample)	{
		if (!(caps.dwCaps & DDCAPS_BLTSTRETCH)) hwbuff = 0;
	} else {
		if ( (!(bltcaps & BLT_CANMIRROR)) && (!(bltcaps & BLT_CANSRCCOLORKEY)) && (!(config.Flags & CFG_LFBREALVOODOO)) ) hwbuff = 0;
	}
	if (caps.dwCaps & DDCAPS_CANBLTSYSMEM )	{
		bltcaps |= BLT_CANBLTSYSMEM;
		//hwbuff=0;
	}
	
	/* Globális tulajdonságok beállitása */
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	lpDDFrontBuff->lpVtbl->GetPixelFormat(lpDDFrontBuff, &ddsd.ddpfPixelFormat);
	ddsd.dwHeight = appYRes;
	ddsd.dwWidth = convbuffxres;
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.dwFlags |= DDSD_CKSRCBLT;
   
	if (hwbuff)	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		for(i = 0; i < movie.cmiBuffNum; i++)	{
			if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &bufflocks[i].lpDDConvbuff, NULL) != DD_OK) break;
		}
		/* Ha nem tudtuk az összeset, akkor majd a sysmembõl */
		if (i != movie.cmiBuffNum)	{
			for(i = 0; i < movie.cmiBuffNum; i++)
				if (bufflocks[i].lpDDConvbuff) bufflocks[i].lpDDConvbuff->lpVtbl->Release(bufflocks[i].lpDDConvbuff);
			bltcaps &= ~(BLT_CANMIRROR | BLT_CANSRCCOLORKEY);
			hwbuff = 0;
		} else bltcaps |= BLT_FASTBLT;
	}
	
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	
	/* Egy átmeneti szoftveres puffer a REALVOODOO opcióhoz (pufferek rotálásához). */
	if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDCopyTempBuff, NULL) != DD_OK) return(0);
	
	if ((!hwbuff) || ( (!(bltcaps & BLT_CANMIRROR)) || (!(bltcaps & BLT_CANSRCCOLORKEY)) ) )	{
		for(i = 0; i < movie.cmiBuffNum; i++)	{
			if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &bufflocks[i].lpDDConvbuff2, NULL) != DD_OK) break;
			if (!hwbuff) bufflocks[i].lpDDConvbuff = bufflocks[i].lpDDConvbuff2;
		}
		/* Ha nem megy, akkor hiba */
		if (i != movie.cmiBuffNum)	{
			for(i = 0;i < movie.cmiBuffNum; i++)
				if (bufflocks[i].lpDDConvbuff2) bufflocks[i].lpDDConvbuff2->lpVtbl->Release(bufflocks[i].lpDDConvbuff2);
			return(0);
		}		
	}
	return(1);
	
	DEBUG_END("GlideLFBAllocConvBuffs", DebugRenderThreadId, 86);
}


void LfbDestroyTileTextures() {
int	i, j;

	for (i = 0; i < numberOfFormats; i++)	{
		for(j = 0; j < TILETEXTURE_NUM; j++) {
			if (tileTextures[i].tTexture[j] != NULL)
				TexDestroyDXTexture(tileTextures[i].tTexture[j]);
			tileTextures[i].tTexture[j] = NULL;
		}
	}
}


int	LfbCreateTileTextures()	{
int				i, j;
unsigned char	*requiredFormat;

	ZeroMemory (tileTextures, numberOfFormats * sizeof (TileTextureData));
//	if (config.Flags2 & CFG_LFBENABLETEXTURING)	{
		for (i = 0; i < numberOfFormats; i++)	{
			for(requiredFormat = tileTexFormats[i]; *requiredFormat != Pf_Invalid; requiredFormat++) {
				if (TexIsDXFormatSupported (*requiredFormat))
					break;
			}
			if (*requiredFormat != Pf_Invalid)	{
				TexGetDXTexPixFmt (*requiredFormat, &tileTextures[i].tTextureFormat);
				for(j = 0; j < TILETEXTURE_NUM; j++) {
					if (!TexCreateDXTexture(*requiredFormat, tileTexSizeX, tileTexSizeY, 1, &tileTextures[i].tTexture[j])) {
					//if ((tileTextures[i].tTexture[j] = TexCreateDXTexture(*requiredFormat, tileTexSizeX, tileTexSizeY)) == NULL) {
						LfbDestroyTileTextures ();
						return (0);
					}
				}
			}
		}
//	}
	return (1);
}


#ifndef GLIDE3

#ifdef	DOS_SUPPORT

int LfbSetUpLFBDOSBuffers(unsigned char *buff0, unsigned char *buff1, unsigned char *buff2, int onlyclient)	{
	
	DEBUG_BEGIN("LfbSetUpLFBDOSBuffers", DebugRenderThreadId, 160);
	
	LOG(1,"\nsetupdosb(buff0=%p, buff1=%p, buff2=%p, onlyclient=%d, runflags=%d",buff0,buff1,buff2,onlyclient,runflags);

	if (!(runflags & RF_INITOK)) return(0);	
	
	bufflocks[0].temparea_client = buff0;
	bufflocks[1].temparea_client = buff1;
	bufflocks[2].temparea_client = buff2;
	if (onlyclient) {
		bufflocks[0].temparea = DosGetSharedLFB();
		bufflocks[1].temparea = DosGetSharedLFB() + LFBMAXPIXELSIZE * convbuffxres * appYRes;
		bufflocks[2].temparea = DosGetSharedLFB() + 2 * LFBMAXPIXELSIZE * convbuffxres * appYRes;
	} else {		
		bufflocks[0].temparea = buff0;
		bufflocks[1].temparea = buff1;
		bufflocks[2].temparea = buff2;
	}	
	return(GlideLFBAllocConvBuffs());
	
	DEBUG_END("LfbSetUpLFBDOSBuffers", DebugRenderThreadId, 160);
}

#endif

#endif


/* Létrehozza, s inicializálja a belsõ puffereket, stb. */
int LfbInit()	{
int i;

	DEBUG_BEGIN("GlideLFBInit", DebugRenderThreadId, 87);

	//config.Flags2 |= CFG_DEPTHEQUALTOCOLOR;

	ZeroMemory(bufflocks, 3*sizeof(_lock));
	
#ifdef GLIDE1
	ZeroMemory(&glide211LockInfo, sizeof(Glide211LockInfo));
#endif
	if ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) == (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) )
		config.Flags &= ~CFG_HWLFBCACHING;
	
	convbuffxres = appXRes;
	if ( (config.Flags & CFG_LFBREALVOODOO) && (appXRes <= 1024) ) convbuffxres = 1024;
	if (platform == PLATFORM_WINDOWS)	{
		for(i = 0; i < 3; i++)
			if ( (bufflocks[i].temparea = bufflocks[i].temparea_client = (unsigned char *) _GetMem(convbuffxres * appYRes * LFBMAXPIXELSIZE)) == NULL)
				return(0);
	}
	if ( (CopyTempArea = _GetMem(convbuffxres * appYRes * LFBMAXPIXELSIZE)) == NULL) return(0);
	
	readcFormat = writecFormat = (config.Flags2 & CFG_CFORMATAFFECTLFB) ? cFormat : GR_COLORFORMAT_ARGB;

	bufflocks[0].lpDDbuff = lpDDFrontBuff;
	bufflocks[1].lpDDbuff = lpDDBackBuff;

	LfbGetAndCheckLFBFormat();
	LfbCreateTileTextures();
	
	/* A csak változások átméretezése csak gyorsírással használható, ezért ilyenkor az egyezõ formátumoknál */
	/* is erõltetjük a gyorsírást */
	if (drawresample && (config.Flags2 & CFG_LFBRESCALECHANGESONLY))	{
		lfbflags = LFB_RESCALE_CHANGES_ONLY | LFB_FORCE_FASTWRITE;
		config.Flags |= CFG_LFBFASTUNMATCHWRITE;
	}
	if (config.Flags2 & CFG_LFBFORCEFASTWRITE) lfbflags |= LFB_FORCE_FASTWRITE;
	if ( (config.Flags2 & CFG_LFBNOMATCHINGFORMATS) ||
		((platform == PLATFORM_DOSWINNT) && (!(config.Flags & CFG_NTVDDMODE))) ) lfbflags |= LFB_NO_MATCHING_FORMATS;
	
	if (platform == PLATFORM_WINDOWS)
		if (!GlideLFBAllocConvBuffs()) return(0);
	
	return(1);
	
	DEBUG_END("GlideLFBInit", DebugRenderThreadId, 87);
}


/* Clean up az LFB-hez */
void LfbCleanUp()	{
int i;
	
	DEBUG_BEGIN("GlideLFBCleanUp",DebugRenderThreadId, 88);

	/* Az esetlegesen zárolva maradt pufferek feloldása */
	GlideLFBBeforeSwap();
	LfbDestroyTileTextures();
	if (CopyTempArea) _FreeMem(CopyTempArea);
	CopyTempArea = NULL;
	for(i = 0; i < 3; i++)	{
		if (bufflocks[i].lpDDConvbuff == bufflocks[i].lpDDConvbuff2) bufflocks[i].lpDDConvbuff2 = NULL;
		if (bufflocks[i].lpDDConvbuff != NULL)
			bufflocks[i].lpDDConvbuff->lpVtbl->Release(bufflocks[i].lpDDConvbuff);
		if (bufflocks[i].lpDDConvbuff2 != NULL)
			bufflocks[i].lpDDConvbuff2->lpVtbl->Release(bufflocks[i].lpDDConvbuff2);
	}
	if (platform == PLATFORM_WINDOWS)	{
		if (bufflocks[0].temparea) _FreeMem(bufflocks[0].temparea);
		if (bufflocks[1].temparea) _FreeMem(bufflocks[1].temparea);
		if (bufflocks[2].temparea) _FreeMem(bufflocks[2].temparea);
		bufflocks[0].temparea = bufflocks[1].temparea = bufflocks[2].temparea = NULL;
	}
	
	DEBUG_END("GlideLFBCleanUp",DebugRenderThreadId, 88);
}


int LfbGetConvBuffXRes()	{
	
	return(convbuffxres);

}


int __inline IsRectContained (RECT *srcRect, RECT *dstRect) {

	return ( (srcRect->left >= dstRect->left)  &&
			 (srcRect->right <= dstRect->right) &&
			 (srcRect->top >= dstRect->top) &&
			 (srcRect->bottom <= dstRect->bottom) );
}


int LfbFlushInternalAuxBuffer (unsigned int buffer, _lock *lockPtr)	{
LPDIRECTDRAWSURFACE7	lpDDbuff, lpDDbuffNext, lpDDConvbuff;
DDBLTFX					ddBltFx;
DWORD					blitMode;
unsigned int			ctype;
RECT					flushRect;
unsigned int			origX, origY, sizeX, sizeY, buffOrigY;
int						tileTexInd;
float					constDepthOow, constDepthOoz;
DWORD					saved3DStateArea[16];
DWORD					saved3DStageStateArea[16];
DWORD					saved3DTexArea[1];
DDCOLORKEY				texColorKey;
float					texScaleX, texScaleY;
unsigned int			incrX, incrY, drawX, drawY;
int						formatIndex;
unsigned int			pixelSize;
RECT					*lRect, dstRect;
unsigned int			dTop;
float					xScale, yScale;
int						renderBuffer;
unsigned int			temp;

	switch(buffer)	{
		case BUFF_RESIZING:
			if (lockPtr->locktype & LOCK_RNOTFLUSHED)	{
				lpDDConvbuff = (lockPtr->locktype & LOCK_RSOFTWARE) ? lockPtr->lpDDConvbuff2 : lockPtr->lpDDConvbuff;
				lpDDbuff = lockPtr->lpDDbuff;
				ZeroMemory(&ddBltFx, sizeof(DDBLTFX));
				ddBltFx.dwSize = sizeof(DDBLTFX);
				xScale = ((float) movie.cmiXres) / appXRes;
				yScale = ((float) movie.cmiYres) / appYRes;
				dstRect.left = (LONG) (lockPtr->lockRect.left * xScale);
				dstRect.right = (LONG) (lockPtr->lockRect.right * xScale);
				
				if (lockPtr->locktype & LOCK_RLOWERORIGIN)	{
					ddBltFx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
					blitMode = DDBLT_DDFX;
					dstRect.top = (LONG) ((appYRes - lockPtr->lockRect.bottom) * yScale);
					dstRect.bottom = (LONG) ((appYRes - lockPtr->lockRect.top) * yScale);
				} else {
					blitMode = 0;
					dstRect.top = (LONG) (lockPtr->lockRect.top * yScale);
					dstRect.bottom = (LONG) (lockPtr->lockRect.bottom * yScale);
				}
				if (!((lockPtr->colorKeyMask == 0x0) && (lockPtr->colorKey == 0x0))) {
					if ( (lockPtr->locktype & LOCK_RFASTCACHED) && (lockPtr->colorKeyMask != 0x0) ) blitMode |= DDBLT_KEYSRC;
					lpDDbuff->lpVtbl->Blt(lpDDbuff, &dstRect, lpDDConvbuff, &lockPtr->lockRect, blitMode | DDBLT_WAIT, &ddBltFx);
				}
				if (lockPtr->writeLockInfo.lockNum == 0)
					lockPtr->locktype &= ~LOCK_RNOTFLUSHED;
				return(1);
			}
			break;
		/* konvertálópuffer ürítése */
		case BUFF_CONVERTING:
			if (lockPtr->locktype & LOCK_CNOTFLUSHED)	{

				if (!((lockPtr->colorKeyMask == 0x0) && (lockPtr->colorKey == 0x0))) {
					if (!(lockPtr->locktype & LOCK_CTEXTURE))	{
						if ( (lockPtr->locktype & LOCK_CCACHED) || (lockPtr->colorKeyMask == 0x0) )	{
							ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &backBufferFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY;
						} else	{
							ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &backBufferFormat) ? CONVERTTYPE_RAWCOPYCOLORKEY : CONVERTTYPE_COLORKEY;
						}
				
					
						lpDDbuff = drawresample ? (lockPtr->locktype & LOCK_RSOFTWARE ? lockPtr->lpDDConvbuff2 : lockPtr->lpDDConvbuff) : lockPtr->lpDDbuff;
						if (lpDDbuff->lpVtbl->Lock(lpDDbuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL) != DD_OK) return(-1);
						lRect = &lockPtr->lockRect;
						dTop = (lockPtr->locktype & LOCK_CLOWERORIGIN) ? appYRes - lRect->bottom : lRect->top;
						pixelSize = lockPtr->lockformat->BitPerPixel / 8;

						/* Writing to LFB */
						MMCConvertAndTransferData(lockPtr->lockformat, &backBufferFormat,
												  MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
												  MMCGetPixelValueFromARGB(lockPtr->colorKeyMask, lockPtr->lockformat),
												  constAlpha,
												  ((char *) lockPtr->temparea) + pixelSize * (lRect->top * convbuffxres + lRect->left),
												  ((char *) lfb.lpSurface) + dTop * lfb.lPitch + lRect->left * backBufferFormat.BitPerPixel / 8,
												  lRect->right - lRect->left, lRect->bottom - lRect->top,
												  (lockPtr->locktype & LOCK_CLOWERORIGIN ? -1 : 1) * (lockPtr->lockformat->BitPerPixel / 8) * convbuffxres,
												  lfb.lPitch,
												  NULL, NULL,
												  ctype, 0);

						if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(-1);
						if (drawresample) lockPtr->locktype |= LOCK_RNOTFLUSHED;
					} else {
						GlideSave3DTextures (saved3DTextures, saved3DTexArea);
						GlideSave3DStageStates (saved3DStageStates, saved3DStageStateArea);
						
						if (!(lockPtr->locktype & LOCK_PIPELINE))	{
							GlideSave3DStates (saved3DStatesNoPipeline, saved3DStateArea);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGENABLE, FALSE);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
							constDepthOow = constDepthOoz = 1.0f;
						} else {
							GlideSave3DStates (saved3DStatesPipeline, saved3DStateArea);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZWRITEENABLE, FALSE);
							constDepthOow = (astate.vertexmode & VMODE_WBUFFEMU) ? DrawGetFloatFromDepthValue (constDepth, DVNonlinearZ) : 
																				   DrawGetFloatFromDepthValue (constDepth, DVTrueW) / 65528.0f;
							constDepthOoz = DrawGetFloatFromDepthValue (constDepth, DVLinearZ);
						}
						
						renderBuffer = renderbuffer;
						grRenderBuffer (lockPtr->bufferNum);
						
						lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
						lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE);
						lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_COLORKEYENABLE,
															lockPtr->colorKeyMask != 0x0 ? TRUE : FALSE);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_MAGFILTER, D3DTFG_POINT);//LINEAR);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_MINFILTER, D3DTFG_POINT);//LINEAR);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
						lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
						if (lockPtr->lockformat->ABitCount) {
							lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						} else {
							lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
							lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_TEXTUREFACTOR, constAlpha << 24);
						}
						
						formatIndex = lockPtr->textureFormatIndex;

						if (lockPtr->colorKeyMask == 0x0)	{
							ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY;
						} else	{
							ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat) ? CONVERTTYPE_RAWCOPYCOLORKEY : CONVERTTYPE_COLORKEY;
						}

						texColorKey.dwColorSpaceHighValue = texColorKey.dwColorSpaceLowValue =
														MMCConvertPixel (
														MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
														lockPtr->lockformat,
														&tileTextures[formatIndex].tTextureFormat,
														constAlpha);

						ZeroMemory(&ddBltFx, sizeof(DDBLTFX));
						ddBltFx.dwSize = sizeof(DDBLTFX);
						ddBltFx.dwFillColor = texColorKey.dwColorSpaceHighValue;

						texScaleX = ((float) movie.cmiXres) / appXRes;
						texScaleY = ((float) movie.cmiYres) / appYRes;

						if (lockPtr->locktype & LOCK_CLOWERORIGIN) {
							SetRect (&flushRect, lockPtr->lockRect.left, appYRes - lockPtr->lockRect.bottom,
												 lockPtr->lockRect.right, appYRes - lockPtr->lockRect.top);
							buffOrigY = lockPtr->lockRect.bottom;
						} else {
							SetRect (&flushRect, lockPtr->lockRect.left, lockPtr->lockRect.top,
												 lockPtr->lockRect.right, lockPtr->lockRect.bottom);
							buffOrigY = lockPtr->lockRect.top;
						}

						origY = flushRect.top;
						tileTexInd = 0;
						
						lpDDbuff = tileTextures[formatIndex].tTexture[0];
						if ((ctype == CONVERTTYPE_RAWCOPYCOLORKEY) || (ctype == CONVERTTYPE_COLORKEY)) {
							lpDDbuff->lpVtbl->Blt (lpDDbuff, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
						}
						
						while(sizeY = flushRect.bottom - origY, sizeY != 0) {
							if (sizeY >= tileTexSizeY)	{
								sizeY = tileTexSizeY;
								incrY = 2; drawY = 2;
							} else {
								incrY = 0; drawY = 0;//1;
							}
							origX = flushRect.left;
							while(sizeX = flushRect.right - origX, sizeX != 0) {
								if (sizeX >= tileTexSizeX)	{
									sizeX = tileTexSizeX;
									incrX = 2; drawX = 2;
								} else {
									incrX = 0; drawX =0;//1;
								}
								//lpDDbuff = tileTextures[formatIndex].tTexture[tileTexInd];

								if (lpDDbuff->lpVtbl->IsLost (lpDDbuff) == DDERR_SURFACELOST) {
									if (lpDDbuff->lpVtbl->Restore (lpDDbuff) != DD_OK)
										return(-1);
								}

								if (++tileTexInd == TILETEXTURE_NUM) tileTexInd = 0;
								lpDDbuffNext = tileTextures[formatIndex].tTexture[tileTexInd];

								if ((ctype == CONVERTTYPE_RAWCOPYCOLORKEY) || (ctype == CONVERTTYPE_COLORKEY)) {
									lpDDbuffNext->lpVtbl->Blt (lpDDbuffNext, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
								}
								if (lpDDbuff->lpVtbl->Lock(lpDDbuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL) != DD_OK) return(-1);

								temp = (lockPtr->locktype & LOCK_CLOWERORIGIN) ? buffOrigY - sizeY : buffOrigY;

								/* Writing to texture tile */
								MMCConvertAndTransferData(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat,
														  MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
														  MMCGetPixelValueFromARGB(lockPtr->colorKeyMask, lockPtr->lockformat),
														  constAlpha,
														  ((char *) lockPtr->temparea) + (wModeTable[formatIndex]->BitPerPixel /8) * ((temp * convbuffxres) + origX),
														  lfb.lpSurface,
														  sizeX, sizeY,
														  ((lockPtr->locktype & LOCK_CLOWERORIGIN) ? -1 : 1) * ((lockPtr->lockformat->BitPerPixel / 8) * convbuffxres),
														  lfb.lPitch,
														  NULL, NULL,
														  ctype,
														  0);
								if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(-1);

								lpDDbuff->lpVtbl->SetColorKey (lpDDbuff, DDCKEY_SRCBLT, &texColorKey);
								lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, 0, lpDDbuff);

								lpDDbuff = lpDDbuffNext;

								DrawTile( (origX * texScaleX),
										  (origY * texScaleY),
										  ((origX + sizeX - drawX) * texScaleX),
										  ((origY + sizeY - drawY) * texScaleY),
										  (0.0f + 0.5f) / tileTexSizeX,
										  (0.0f + 0.5f) / tileTexSizeY, 
										  (((float) sizeX - drawX) + 0.5f) / tileTexSizeX,
										  (((float) sizeY - drawY) + 0.5f) / tileTexSizeY,
										  constDepthOow, constDepthOoz);
								origX += sizeX - incrX;
							}
							origY += sizeY - incrY;
							if (lockPtr->locktype & LOCK_CLOWERORIGIN) {
								buffOrigY -= (sizeY - incrY);
							} else {
								buffOrigY += sizeY - incrY;
							}
						}
						GlideRestore3DTextures (saved3DTextures, saved3DTexArea);
						if (lockPtr->locktype & LOCK_PIPELINE)	{
							GlideRestore3DStates (saved3DStatesPipeline, saved3DStateArea);
							GlideRestore3DStageStates (saved3DStageStates, saved3DStageStateArea);
						} else {
							GlideRestore3DStates (saved3DStatesNoPipeline, saved3DStateArea);
							GlideRestore3DStageStates (saved3DStageStates, saved3DStageStateArea);
						}
						grRenderBuffer (renderBuffer);
					}
				}
				if (lockPtr->writeLockInfo.lockNum == 0)
					lockPtr->locktype &= ~LOCK_CNOTFLUSHED;
				return(1);
			}
			break;
	}
	return(0);
}


void LfbFlushInteralAuxBufferContents (int buffer)	{
_lock *lockptr;

	lockptr = bufflocks + buffer;
	if (lockptr->locktype & LOCK_ISLOCKED) return;
	LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockptr);
	LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockptr);
}


void LfbSetBuffDirty(GrBuffer_t buffer)	{
	
	if (bufflocks[buffer].locktype & (LOCK_CNOTFLUSHED | LOCK_RNOTFLUSHED)) LfbFlushInteralAuxBufferContents (buffer);
	bufflocks[buffer].locktype |= LOCK_DIRTY;
}


int LfbPrepareInternalResizingBuffer (_lock *lockPtr, unsigned int requiredState, LockInfo *pLockInfo)	{
LPDIRECTDRAWSURFACE7	lpDDConvbuff;
DDCOLORKEY				ddColorKey;
DDBLTFX					ddbltfx;
DWORD					blitMode;
RECT					srcRect;
float					xScale, yScale;

	lpDDConvbuff = (requiredState & LOCK_RSOFTWARE) ? lockPtr->lpDDConvbuff2 : lockPtr->lpDDConvbuff;

	/* A kívánt állapot megegyezik a jelenlegivel? */
	if ( (requiredState & LOCK_RSTATEMASK) != (lockPtr->locktype & LOCK_RSTATEMASK) ||
		((requiredState & LOCK_RFASTCACHED) && (pLockInfo->colorKey != lockPtr->colorKey)) )	{
		
		LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr);
		
		if ((pLockInfo->colorKeyMask != 0x0) || (pLockInfo->lockType == LI_LOCKTYPEREAD)) {
			ZeroMemory(&ddbltfx, sizeof(DDBLTFX));
			ddbltfx.dwSize = sizeof(DDBLTFX);
			
			/* Sajna a DirectX nem támogat colokey-maszkot, ezért ezt nem tudjuk figyelembe venni */
			ddColorKey.dwColorSpaceHighValue =
			ddColorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(pLockInfo->colorKey, &backBufferFormat);
			
			lpDDConvbuff->lpVtbl->SetColorKey(lpDDConvbuff,	DDCKEY_SRCBLT, &ddColorKey);

			if (requiredState & LOCK_RFASTCACHED)	{
				ddbltfx.dwFillColor = ddColorKey.dwColorSpaceLowValue;
				lpDDConvbuff->lpVtbl->Blt(lpDDConvbuff, &pLockInfo->lockRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
			} else {
				xScale = ((float) movie.cmiXres) / appXRes;
				yScale = ((float) movie.cmiYres) / appYRes;
				srcRect.left = (LONG) (pLockInfo->lockRect.left * xScale);
				srcRect.right = (LONG) (pLockInfo->lockRect.right * xScale);
				if (requiredState & LOCK_RLOWERORIGIN)	{
					ddbltfx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
					blitMode = DDBLT_DDFX;
					srcRect.top = (LONG) ((appYRes - pLockInfo->lockRect.bottom) * yScale);
					srcRect.bottom = (LONG) ((appYRes - pLockInfo->lockRect.top) * yScale);
				} else {
					blitMode = 0;
					srcRect.top = (LONG) (pLockInfo->lockRect.top * yScale);
					srcRect.bottom = (LONG) (pLockInfo->lockRect.bottom * yScale);
				}
				lpDDConvbuff->lpVtbl->Blt(lpDDConvbuff, &pLockInfo->lockRect, lockPtr->lpDDbuff, &srcRect, blitMode | DDBLT_WAIT, &ddbltfx);
			}
		}
		lockPtr->locktype &= ~(LOCK_LOCKED | LOCK_RSTATEMASK);
		lockPtr->locktype |= requiredState;
	}
	return(1);
}


int LfbLockInternalAuxBuffer (unsigned int buffer, LockInfo *pLockInfo, _lock *lockptr) {
LPDIRECTDRAWSURFACE7	lpDDbuff, lpDDConvbuff;
unsigned int			requiredstate;
unsigned int			pixelSize;
RECT					*lRect;
unsigned int			dTop;

	switch(buffer)	{
		case BUFF_RESIZING:
			/* A kívánt állapot meghatározása */
			requiredstate = pLockInfo->lowerOrigin ? LOCK_RLOWERORIGIN : 0;
			requiredstate |= (pLockInfo->lockType == LI_LOCKTYPEREAD) ? LOCK_RCACHED :
							 ( (lfbflags & LFB_FORCE_FASTWRITE) ? LOCK_RFASTCACHED : LOCK_RCACHED);
			if ( ((requiredstate & LOCK_RLOWERORIGIN) == (lockptr->locktype & LOCK_RLOWERORIGIN)) && (lockptr->locktype & LOCK_RCACHED) )	{
				requiredstate |= LOCK_RCACHED;
				requiredstate &= ~LOCK_RFASTCACHED;
			}
			if ( ((requiredstate & LOCK_RLOWERORIGIN) && (!(bltcaps & BLT_CANMIRROR))) ||
				 ((requiredstate & LOCK_RFASTCACHED) && (!(bltcaps & BLT_CANSRCCOLORKEY))) ) requiredstate |= LOCK_RSOFTWARE;
					
			/* Ha a puffer le van lockolva, de a kívánt pufferrel nem egyezik, akkor egyezõvé tesszük */
			if ( (lockptr->locktype & LOCK_ISLOCKED) && ((requiredstate & LOCK_RSOFTWARE) != (lockptr->locktype & LOCK_RSOFTWARE)))	{
				requiredstate &= ~LOCK_RSOFTWARE;
				requiredstate |= lockptr->locktype & LOCK_RSOFTWARE;
			}
			
			lpDDConvbuff = (requiredstate & LOCK_RSOFTWARE) ? lockptr->lpDDConvbuff2 : lockptr->lpDDConvbuff;
			
			/* Szépséghiba: ha a megkívánt állapot megegyezik a jelenlegivel, feleslegesen lefut egy unlock-lock pár */
			if (lockptr->locktype & LOCK_ISLOCKED) if (lpDDConvbuff->lpVtbl->Unlock(lpDDConvbuff, NULL) != DD_OK) return(0);
			
			if (!LfbPrepareInternalResizingBuffer (lockptr, requiredstate, pLockInfo)) return(0);

			lockptr->colorKey = pLockInfo->colorKey;
			lockptr->colorKeyMask = pLockInfo->colorKeyMask;

			if (lpDDConvbuff->lpVtbl->Lock(lpDDConvbuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_NOSYSLOCK ,NULL) != DD_OK) return(0);
			
			if (pLockInfo->lockType == LI_LOCKTYPEWRITE) lockptr->locktype |= LOCK_RNOTFLUSHED;
			break;

		case BUFF_CONVERTING:
			/* A kívánt állapot meghatározása */
						
			/* Módszer kiválasztása: lockolás olvasásra: mindig full readback (cached), írás textúrákkal: texture-cached, */
			/* írás gyorsírással: fast-cached, egyébként írásra is full readback  */
			if (pLockInfo->lockType == LI_LOCKTYPEREAD)
				requiredstate = LOCK_CCACHED;
			else if (pLockInfo->useTextures)
				requiredstate = LOCK_CTEXTURE;
			else if (config.Flags & CFG_LFBFASTUNMATCHWRITE)
				requiredstate = LOCK_CFASTCACHED;
			else
				requiredstate = LOCK_CCACHED;

			requiredstate |= pLockInfo->lowerOrigin ? LOCK_CLOWERORIGIN : 0;
			
			/* Ha a puffer adott régiója teljes visszaolvasott adatot tartalmaz, akkor mindig ezt használjuk fel,  */
			/* felülbírálva az elõzõleg kiválasztott módszert */
			if ( ((requiredstate & LOCK_CLOWERORIGIN) == (lockptr->locktype & LOCK_CLOWERORIGIN)) &&
				 (lockptr->locktype & LOCK_CCACHED) &&
				 IsRectContained (&pLockInfo->lockRect, &lockptr->lockRect) )	{
				requiredstate |= LOCK_CCACHED;
				requiredstate &= ~(LOCK_CFASTCACHED | LOCK_CTEXTURE);
			}

			pixelSize = pLockInfo->lockFormat->BitPerPixel / 8;
			/* A kívánt állapot megegyezik a jelenlegivel? */
			if ( ((requiredstate & LOCK_CSTATEMASK) != (lockptr->locktype & LOCK_CSTATEMASK)) ||
				  (!MMCIsPixfmtTheSame(pLockInfo->lockFormat, lockptr->lockformat)) ||
				  (!IsRectContained (&pLockInfo->lockRect, &lockptr->lockRect)) ||
				  ( (requiredstate & (LOCK_CFASTCACHED | LOCK_CTEXTURE)) &&
				   ((pLockInfo->colorKey & pLockInfo->colorKeyMask) != (lockptr->colorKey & lockptr->colorKeyMask)) ) )	{
				
				LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockptr);
				lockptr->colorKey = pLockInfo->colorKey;
				lockptr->colorKeyMask = pLockInfo->colorKeyMask;

				if ((pLockInfo->colorKeyMask != 0x0) || (pLockInfo->lockType == LI_LOCKTYPEREAD)) {
					lRect = &pLockInfo->lockRect;
					dTop = (requiredstate & LOCK_CLOWERORIGIN) ? appYRes - lRect->bottom : lRect->top;
					
					/* A fast-cached és texture-cached állapotokhoz a colorkey-jel initeljük a buffert */
					if (requiredstate & (LOCK_CFASTCACHED | LOCK_CTEXTURE))	{
						MMCFillBuffer( ((char *) lockptr->temparea) + pixelSize * (lRect->top * convbuffxres + lRect->left),
									  pLockInfo->lockFormat->BitPerPixel,
									  MMCGetPixelValueFromARGB(pLockInfo->colorKey, pLockInfo->lockFormat),
									  lRect->right - lRect->left, lRect->bottom - lRect->top, pixelSize * convbuffxres);
					} else {
					/* egyébként kiolvassuk a color- vagy resizingbuffert */
						lpDDbuff = drawresample ? (lockptr->locktype & LOCK_RSOFTWARE ? lockptr->lpDDConvbuff2 : lockptr->lpDDConvbuff) : lockptr->lpDDbuff;
						if (lpDDbuff->lpVtbl->Lock(lpDDbuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL) != DD_OK) return(0);
						
						/* Reading from LFB */
						MMCConvertAndTransferData(&backBufferFormat, pLockInfo->lockFormat,
												  0, 0, constAlpha,
												  ((char *) lfb.lpSurface) + dTop * lfb.lPitch + lRect->left * backBufferFormat.BitPerPixel / 8,
												  ((char *) lockptr->temparea) + pixelSize * (lRect->top * convbuffxres + lRect->left),
												  lRect->right - lRect->left, lRect->bottom - lRect->top,
												  lfb.lPitch,
												  ((requiredstate & LOCK_CLOWERORIGIN) ? -1 : 1) * pixelSize * convbuffxres,
												  NULL, NULL,
												  MMCIsPixfmtTheSame(pLockInfo->lockFormat, &backBufferFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
												  0);
						if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(0);
					}
				}
				lockptr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE | LOCK_CLOWERORIGIN);
				lockptr->locktype |= requiredstate;
			}
			if (pLockInfo->lockType == LI_LOCKTYPEWRITE) lockptr->locktype |= LOCK_CNOTFLUSHED;
			lfb.lpSurface = lockptr->temparea_client;
			lfb.lPitch = pixelSize * convbuffxres;
			break;
		
	}
	if (pLockInfo->viaPixelPipeline) lockptr->locktype |= LOCK_PIPELINE;
	lockptr->lockRect = pLockInfo->lockRect;
	return(1);
}


int LfbEvaluateAlphaTestFunction (DWORD alphaTestFunc, DWORD alphaValue, DWORD alphaReference) {
	
	switch(alphaTestFunc) {
		case D3DCMP_NEVER:
			return (0);
		case D3DCMP_LESS:
			return (alphaValue < alphaReference);
		case D3DCMP_EQUAL:
			return (alphaValue == alphaReference);
		case D3DCMP_LESSEQUAL:
			return (alphaValue <= alphaReference);
		case D3DCMP_GREATER:
			return (alphaValue > alphaReference);
		case D3DCMP_NOTEQUAL:
			return (alphaValue != alphaReference);
		case D3DCMP_GREATEREQUAL:
			return (alphaValue >= alphaReference);
		case D3DCMP_ALWAYS:
		default:
			return (1);
	}
}


void LfbGetColorKeyAndMaskForWrite (GrLfbWriteMode_t writeMode, int activePipeline,
									unsigned int *colorKey, unsigned int *colorKeyMask) {
int evalBool1, evalBool2;

	*colorKey = DEFAULTCOLORKEY;
	*colorKeyMask = 0xFFFFFFFF;
	if (activePipeline) {
		if (astate.colorKeyEnable) {
			*colorKey = astate.colorkey;
			*colorKeyMask = 0x00FFFFFF;
		} else if (astate.alphatestfunc != D3DCMP_ALWAYS) {
			if (writeMode == GR_LFBWRITEMODE_1555)	{
				evalBool1 = LfbEvaluateAlphaTestFunction (astate.alphatestfunc, 0x0, astate.alpharef);
				evalBool2 = LfbEvaluateAlphaTestFunction (astate.alphatestfunc, 0xFF, astate.alpharef);
				if (evalBool1 == evalBool2)	{
					if (!evalBool1)
						*colorKeyMask = *colorKey = 0x0;
				} else {
					*colorKeyMask = 0xFF000000;
					*colorKey = (evalBool1) ? (DEFAULTCOLORKEY | 0xFF000000) : (DEFAULTCOLORKEY & ~0xFF000000);
				}
			} else {
				if (!LfbEvaluateAlphaTestFunction (astate.alphatestfunc, constAlpha, astate.alpharef)) {
					*colorKeyMask = *colorKey = 0x0;
				}
			}
		} /*else if () {
		}*/
	}
}


int LfbLockViaTextures (GrLock_t type, GrLfbWriteMode_t writeMode, FxBool pixelPipeline) {
	//return 0;
	
	if ((type != GR_LFB_READ_ONLY) && (!(config.Flags2 & CFG_LFBDISABLETEXTURING)))	{
		if (pixelPipeline) {
			if (astate.alphaBlendEnable || (astate.fogmode != GR_FOG_DISABLE) ||
				(astate.depthbuffmode != GR_DEPTHBUFFER_DISABLE) ||
				((astate.alphatestfunc != D3DCMP_ALWAYS) && (writeMode == GR_LFBWRITEMODE_8888)) ) {
				LOG (0, "\n     LockViaTextures: TRUE");
				return 1;
			}
		}
	}
	LOG (0, "\n     LockViaTextures: FALSE");
	return 0;

}


int		LfbLockBuffer (int buffer, LockInfo *pLockInfo) {
_lock					*lockPtr;
_plockinfo				*plockPtr;
unsigned int			lOrigin;
unsigned int			disableMask, requiredState;
LPDIRECTDRAWSURFACE7	lpDDBuff;
int						rv;
unsigned int			temp;

	lockPtr = bufflocks + buffer;

	if (config.Flags2 & CFG_YMIRROR) pLockInfo->lowerOrigin ^= 1;
	
	lOrigin = pLockInfo->lowerOrigin ? LOCK_LOWERORIGIN : 0;
	
	/* Általános vizsgálatok, ha a buffer már lockolva van: */
	/* - ha a két origó eltér, hiba */
	/* - ha a két színformátum nem egyezik, hiba */
	/* - ha a buffer már írásra nyitva van, és a pipeline attribok nem egyeznek, hiba */
	if (lockPtr->locktype & LOCK_LOCKED) {

		if ( (lOrigin != (lockPtr->locktype & LOCK_LOWERORIGIN)) ||
			 (!MMCIsPixfmtTheSame(pLockInfo->lockFormat, lockPtr->lockformat)) ) return(0);

		if ( (pLockInfo->lockType == LI_LOCKTYPEWRITE) && (lockPtr->writeLockInfo.lockNum) ) {
			temp = pLockInfo->viaPixelPipeline ? LOCK_PIPELINE : 0;
			if (temp != (lockPtr->locktype & LOCK_PIPELINE)) return(0);
		}
	}
	
	plockPtr = (pLockInfo->lockType == LI_LOCKTYPEWRITE) ? &lockPtr->writeLockInfo : &lockPtr->readLockInfo;

	DrawFlushPrimitives(0);
	/* Ha az adott típusú lockolás már megtörtént, akkor visszaadjuk annak az adatait */
	if (plockPtr->lockNum)	{
		if (!IsRectContained (&pLockInfo->lockRect, &lockPtr->lockRect))
			return (0);
		pLockInfo->buffPtr = lockPtr->ptr;
		pLockInfo->buffPitch = lockPtr->stride;
		plockPtr->lockNum++;
		return(1);
	}			
	
	lockPtr->locktype &= ~LOCK_LOWERORIGIN;
	lockPtr->locktype |= lOrigin;

	if ( (lpDDBuff = lockPtr->lpDDbuff) == NULL) return(0);

	/* Ha a cooperative level nem megfelelõ, akkor minimális lenne az esély, hogy a */
	/* lockolni kivánt puffer nem veszett el, de amúgy sem látjuk a glide-alkalmazás */
	/* képernyõjét, ezért ekkor az átmeneti terület cimét adjuk vissza. Ha már zárolva */
	/* van a puffer úgy, hogy nem az átmeneti terület cimét adtuk vissza, akkor */
	/* a normális módon próbáljuk mégegyszer zárolni, legfeljebb nem sikerül. */
	if (lpDD->lpVtbl->TestCooperativeLevel(lpDD) != DD_OK)	{
		if ( !( (!(lockPtr->locktype & LOCK_TEMPDISABLE)) && (lockPtr->locktype & LOCK_LOCKED)) )
			lockPtr->locktype |= LOCK_TEMPDISABLE;
	}
	
	disableMask = (pLockInfo->lockType == LI_LOCKTYPEREAD) ? DISABLE_READ : DISABLE_WRITE;
	/* Ha a pufferelérés tiltott, akkor az átmeneti terület címével visszatérünk, és kész. */
	if ( (!(lockPtr->disabled & disableMask)) && (!(lockPtr->locktype & LOCK_TEMPDISABLE)) )	{
		/* Megnézzük, hogy a segédpuffer megvan-e még, ha nem, akkor helyreállitjuk */
		if (lockPtr->lpDDConvbuff != NULL)	{
			if (lockPtr->lpDDConvbuff->lpVtbl->IsLost(lockPtr->lpDDConvbuff) != DD_OK)	{
				lockPtr->lpDDConvbuff->lpVtbl->Restore(lockPtr->lpDDConvbuff);
				lockPtr->locktype &= ~LOCK_CACHEBITS;
			}
		}
		/* A lockolni kivánt puffer esetleges helyreállitása: teljes képernyõn mindig a */
		/* frontbuffert állitjuk helyre, mert egy complex surface tagjait külön-külön */
		/* nem lehet helyreállitani. */
		if (lpDDBuff->lpVtbl->IsLost(lpDDBuff) != DD_OK)	{
			if (config.Flags & CFG_WINDOWED)	{
				lpDDBuff->lpVtbl->Restore(lpDDBuff);
			} else {
				lpDDFrontBuff->lpVtbl->Restore(lpDDFrontBuff);
			}
			lockPtr->locktype &= ~LOCK_CACHEBITS;
		}
		if (buffer == 0) GrabberRestoreFrontBuff();
		//if (buffer == GR_BUFFER_FRONTBUFFER) GrabberRestoreFrontBuff();

		/* mely puffert használjuk majd a lockoláshoz */
		if (!(lockPtr->locktype & LOCK_LOCKED))	{
			//if (!depthBuffer)	{
				
				lockPtr->usedBuffer = BUFF_COLOR;
				
				if ( (drawresample) || (pLockInfo->lowerOrigin) ||
					 (config.Flags & CFG_LFBREALVOODOO) || (lfbflags & LFB_FORCE_FASTWRITE) )
					lockPtr->usedBuffer = BUFF_RESIZING;
				
				if ( (!MMCIsPixfmtTheSame(pLockInfo->lockFormat, &backBufferFormat)) ||
					 (lfbflags & LFB_NO_MATCHING_FORMATS) || pLockInfo->useTextures )
					lockPtr->usedBuffer = BUFF_CONVERTING;
			
			/*} else {
				lockPtr->usedBuffer = BUFF_DEPTH;
			}*/
		}
		
		if ( (lockPtr->locktype & LOCK_DIRTY) && (!(lockPtr->locktype & LOCK_ISLOCKED)) )
			lockPtr->locktype &= ~(LOCK_DIRTY | LOCK_CACHEBITS);

		if (!drawresample)	{
			switch(lockPtr->usedBuffer)	{
				case BUFF_COLOR:
					if (!(lockPtr->locktype & LOCK_ISLOCKED))	{
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
						if (rv) lockPtr->locktype &= ~(LOCK_RCACHED | LOCK_RFASTCACHED);
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr)) == -1) return(0);
						if (rv) lockPtr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE);
						lpDDBuff = lockPtr->lpDDbuff;
						if (lpDDBuff->lpVtbl->Lock(lpDDBuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL) != DD_OK) return(0);
					}
					break;
				
				case BUFF_RESIZING:
					/* Ha a konvertálóterületet ki kell üríteni, akkor az átméretezõpuffer tartalma érvénytelenné válik */
					if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
					if (rv) lockPtr->locktype &= ~(LOCK_RCACHED | LOCK_RFASTCACHED);
					if (!LfbLockInternalAuxBuffer (BUFF_RESIZING, pLockInfo, lockPtr))
						return(0);
					break;
				
				case BUFF_CONVERTING:
					if ((rv = LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr)) == -1) return(FXFALSE);
					if (rv) lockPtr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE);
					if (!LfbLockInternalAuxBuffer (BUFF_CONVERTING, pLockInfo, lockPtr))
						return(0);
					break;

			}
		} else {
			switch(lockPtr->usedBuffer)	{
				case BUFF_RESIZING:
					if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
					if (!LfbLockInternalAuxBuffer (BUFF_RESIZING, pLockInfo, lockPtr))
						return(0);
					break;
				case BUFF_CONVERTING:
					if (pLockInfo->lockType == LI_LOCKTYPEWRITE) {
						if (!LfbLockInternalAuxBuffer (BUFF_CONVERTING, pLockInfo, lockPtr))
							return(0);
					} else {
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
					}

					if (!pLockInfo->useTextures) {
						requiredState = (pLockInfo->lockType == LI_LOCKTYPEREAD) ? LOCK_RCACHED :
										( (lfbflags & LFB_RESCALE_CHANGES_ONLY) ? LOCK_RFASTCACHED : LOCK_RCACHED);
						if ( (!(lockPtr->locktype & LOCK_RLOWERORIGIN)) && (lockPtr->locktype & LOCK_RCACHED) )	{
							requiredState |= LOCK_RCACHED;
							requiredState &= ~LOCK_RFASTCACHED;
						}
						if ( ((requiredState & LOCK_RFASTCACHED) && (!(bltcaps & BLT_CANSRCCOLORKEY))) ) requiredState |= LOCK_RSOFTWARE;
						if (!LfbPrepareInternalResizingBuffer (lockPtr, requiredState, pLockInfo)) return(0);
					} else
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr)) == -1) return(0);

					if (pLockInfo->lockType == LI_LOCKTYPEREAD) {
						if (!LfbLockInternalAuxBuffer (BUFF_CONVERTING, pLockInfo, lockPtr))
							return(0);
					}
					break;
			}
		}
		pLockInfo->buffPtr = lockPtr->ptr = lfb.lpSurface;
		pLockInfo->buffPitch = lockPtr->stride = lfb.lPitch;
	} else {
		pLockInfo->buffPtr = lockPtr->ptr = lockPtr->temparea_client;
		pLockInfo->buffPitch = lockPtr->stride = (pLockInfo->lockFormat->BitPerPixel / 8) * convbuffxres;
	}
	lockPtr->lockformat = pLockInfo->lockFormat;
	lockPtr->textureFormatIndex = pLockInfo->textureFormatIndex;
	plockPtr->lockNum++;
	if (!(lockPtr->disabled & disableMask)) lockPtr->locktype |= LOCK_LOCKED;
	return(1);
}


int	LfbUnlockBuffer (int buffer, int lockType) {
_plockinfo				*plockPtr;
_lock					*lockPtr;
unsigned int			lockSum, disableMask;
LPDIRECTDRAWSURFACE7	lpDDbuff;

	lockPtr = bufflocks + buffer;
	plockPtr = (lockType == LI_LOCKTYPEWRITE) ? &lockPtr->writeLockInfo : &lockPtr->readLockInfo;
	
	if (plockPtr->lockNum == 0) return(0);
	lockSum = lockPtr->readLockInfo.lockNum + lockPtr->writeLockInfo.lockNum;

	disableMask = (lockType == LI_LOCKTYPEWRITE) ? DISABLE_WRITE : DISABLE_READ;
	if ( (!(lockPtr->disabled & disableMask)) && ((--lockSum) == 0) && (!(lockPtr->locktype & LOCK_TEMPDISABLE)) )	{
		switch(lockPtr->usedBuffer)	{
			case BUFF_COLOR:
				lpDDbuff = lockPtr->lpDDbuff;
				if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(0);
				break;
			case BUFF_RESIZING:
				lpDDbuff = (lockPtr->locktype & LOCK_RSOFTWARE) ? lockPtr->lpDDConvbuff2 : lockPtr->lpDDConvbuff;
				if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(0);
				break;
			/*case BUFF_DEPTH:
				lpDDbuff = lockPtr->lpDDbuff;
				if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(0);
				break;*/
		}
	}
	plockPtr->lockNum--;
	if (!(lockPtr->disabled & disableMask))	{
		if (lockSum) lockPtr->locktype |= LOCK_LOCKED;
			else {
				lockPtr->locktype &= ~(LOCK_ISLOCKED);
				if ( (lockPtr->locktype & LOCK_PIPELINE) || (config.Flags2 & CFG_LFBALWAYSFLUSHWRITES) ||
					(buffer == GR_BUFFER_FRONTBUFFER) )
					LfbFlushInteralAuxBufferContents (buffer);
				lockPtr->locktype &= ~LOCK_PIPELINE;
			}
		if (buffer == GR_BUFFER_FRONTBUFFER) ShowFrontBuffer();
	}
	return (1);
}


unsigned int qw = 1;
int i,j;
char *r;
FxBool EXPORT1 grLfbLock(GrLock_t type, 
						GrBuffer_t buffer, 
						GrLfbWriteMode_t writeMode,
						GrOriginLocation_t origin, 
						FxBool pixelPipeline,
						GrLfbInfo_t *info)	{

int						depthBuffer;
LockInfo				lockInfo;

	DEBUG_BEGIN("grLfbLock", DebugRenderThreadId, 89);
	
	LOG(1,"\ngrLfbLock(type=%s, buffer=%s, writeMode=%s, origin=%s, pixelPipeline=%d, info=%p)",
		        locktype_t[type], buffer_t[buffer], lfbwritemode_t[(writeMode==GR_LFBWRITEMODE_ANY) ? 0x10 : writeMode], origin_t[origin],
				pixelPipeline, info);

	if (!(runflags & RF_INITOK)) return(FXFALSE);

	//if (buffer ==1) buffer=0;
	//buffer = GR_BUFFER_FRONTBUFFER;

	/* Alpha- és mélységi puffert nem tud lockolni */	
	if ( (buffer != GR_BUFFER_FRONTBUFFER) && (buffer != GR_BUFFER_BACKBUFFER) &&
		 (buffer != GR_BUFFER_AUXBUFFER)) return(FXFALSE);

	/* A lockolás idejére a hardvernek tétlennek kell lennie, és a pixel pipeline sem */
	/* képes ilyenkor m¾ködni (a memóriaírás által) */
	if (type & GR_LFB_NOIDLE) return(FXFALSE);
	type &= GR_LFB_READ_ONLY | GR_LFB_WRITE_ONLY;
				
	/* Ha a kívánt pixelformátum nem támogatott (RGB565, RGB555, ARGB1555, RGB888, ARGB8888), akkor hiba */
	if ( (buffer != GR_BUFFER_AUXBUFFER) || ( (movie.cmiBuffNum == 3) && (depthbuffering == 0) ) )	{
		if ( (writeMode == GR_LFBWRITEMODE_ANY) || (type == GR_LFB_READ_ONLY) ) writeMode = GR_LFBWRITEMODE_565;
		if ((writeMode > GR_LFBWRITEMODE_ZA16) || (writeModeToIndex[writeMode] == unsupportedFormat) ) {
			
			DEBUGLOG(0,"\n   Error: grLfbLock: Application has tried to lock the color buffer with an unsupported write format. Lock has failed. (Contact the author)");
			DEBUGLOG(1,"\n   Hiba: grLfbLock: Az alkalmazás a képpuffert egy nem támogatott írási formátummal próbálta zárolni. A zárolás nem sikerült. (Beszélj a szerzõvel)");
			return(FXFALSE);
		
		}
	}
	lockInfo.lockFormat = (type == GR_LFB_READ_ONLY) ? &wModeTable[GR_LFBWRITEMODE_565][readcFormat] :
													   &wModeTable[writeModeToIndex[writeMode]][writecFormat];
	info->writeMode = writeMode;
	
	if ( (buffer == GR_BUFFER_AUXBUFFER) && depthbuffering )	{
		DEBUGLOG(0,"\n   Warning: grLfbLock: Application tries to lock the depth or alpha buffer. This feature is unsupported in this version.");
		DEBUGLOG(1,"\n   Figyelmeztetés: grLfbLock: Az alkalmazás a mélységi vagy alpha-puffert próbálja meg zárolni. Ezt ez a verzió nem támogatja.");
		depthBuffer = 1;

		info->lfbPtr = bufflocks[2].temparea;
		info->strideInBytes = movie.cmiXres * 2;
		info->origin = GR_ORIGIN_UPPER_LEFT;

		if (type == GR_LFB_WRITE_ONLY) {
			lpD3Ddevice->lpVtbl->Clear(lpD3Ddevice, 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			//return(FXFALSE);
		}
		return(FXTRUE);
		//_asm mov qw,0x4000000
		//_asm nop
	} else {
		depthBuffer = 0;

		if (pixelPipeline) {
			buffer = renderbuffer;
			origin = astate.locOrig;
		}
	}
	
	/* Struktúra méretének ellenôrzése */
	//if (info->size < sizeof(GrLfbInfo_t)) return(FXFALSE);
	
	/* origó beállítása */	
	switch (origin) {
		case GR_ORIGIN_UPPER_LEFT:
		case GR_ORIGIN_ANY:
			lockInfo.lowerOrigin = 0;
			info->origin = GR_ORIGIN_UPPER_LEFT;
			break;
		
		case GR_ORIGIN_LOWER_LEFT:
			lockInfo.lowerOrigin = 1;
			info->origin = GR_ORIGIN_LOWER_LEFT;
			break;

		default:
			return (FXFALSE);			
	}

	lockInfo.lockType = (type == GR_LFB_READ_ONLY) ? LI_LOCKTYPEREAD : LI_LOCKTYPEWRITE;
	if (pixelPipeline) {
		lockInfo.lockRect.left = astate.minx;
		lockInfo.lockRect.top = astate.miny;
		lockInfo.lockRect.right = astate.maxx;
		lockInfo.lockRect.bottom = astate.maxy;
	} else {
		lockInfo.lockRect.left = 0;
		lockInfo.lockRect.top = 0;
		lockInfo.lockRect.right = appXRes;
		lockInfo.lockRect.bottom = appYRes;
	}

	/* Az írásra zárolás textúrákon keresztül történjen? Ilyenkor mindig a converting buffert használjuk. */
	/* Ay origot is vizsgalni kellene a masodik feltetelben!!! */
	lockInfo.useTextures = LfbLockViaTextures (type, writeMode, pixelPipeline);// && (!(MMCIsPixfmtTheSame(lockInfo.lockFormat, &backBufferFormat) && drawresample));
	lockInfo.viaPixelPipeline = pixelPipeline;
	lockInfo.textureFormatIndex = writeModeToIndex[writeMode];
	LfbGetColorKeyAndMaskForWrite (writeMode, pixelPipeline, &lockInfo.colorKey, &lockInfo.colorKeyMask);
	
	if (!LfbLockBuffer (buffer, &lockInfo))
		return (FXFALSE);
	
	info->lfbPtr = lockInfo.buffPtr;
	info->strideInBytes = lockInfo.buffPitch;
	bufflocks[buffer].writeMode = writeMode;

	return(FXTRUE);
	
	DEBUG_END("grLfbLock", DebugRenderThreadId, 89);
}


FxBool EXPORT1 grLfbUnlock( GrLock_t type, GrBuffer_t buffer )	{

	DEBUG_BEGIN("grLfbUnlock", DebugRenderThreadId, 90);

	LOG(0,"\ngrLfbUnlock(type=%s, buffer=%s)", locktype_t[type], buffer_t[buffer]);
	if (!(runflags & RF_INITOK)) return(FXFALSE);

	//buffer = GR_BUFFER_FRONTBUFFER;

	if ( (buffer != GR_BUFFER_FRONTBUFFER) && (buffer != GR_BUFFER_BACKBUFFER) 
			&& (buffer != GR_BUFFER_AUXBUFFER) ) return(FXFALSE);

	type &= GR_LFB_READ_ONLY | GR_LFB_WRITE_ONLY;
	
	if (!LfbUnlockBuffer (buffer, (type & GR_LFB_WRITE_ONLY) ? LI_LOCKTYPEWRITE : LI_LOCKTYPEREAD))
		return (FXFALSE);
	else
		return(FXTRUE);
	
	DEBUG_END("grLfbUnlock", DebugRenderThreadId, 90);
}


#ifdef GLIDE1

void EXPORT grLfbBypassMode( GrLfbBypassMode_t mode )	{
GrLfbInfo_t info;
FxBool		pixelPipeLine;

	DEBUG_BEGIN("grLfbBypassMode", DebugRenderThreadId, 91);
	
	LOG(1,"\ngrLfbBypassMode(mode=%s)",bypassmode_t[mode]);

	pixelPipeLine = (mode == GR_LFBBYPASS_ENABLE) ? FXFALSE : FXTRUE;

	if (pixelPipeLine != glide211LockInfo.pixelPipeLine) {
		
		glide211LockInfo.pixelPipeLine = pixelPipeLine;
		if (glide211LockInfo.begin) {
			for(i = 0; i < 3; i++)	{
				if (glide211LockInfo.writePtr[i] != NULL)	{
					grLfbUnlock (GR_LFB_WRITE_ONLY, i);
					if (pixelPipeLine == FXFALSE) {
						LfbSetBuffDirty (i);
					}
					grLfbLock(GR_LFB_WRITE_ONLY, i, glide211LockInfo.writeMode, glide211LockInfo.origin, pixelPipeLine, &info);
				}
			}
		}		
	}
	
	DEBUG_END("grLfbBypassMode", DebugRenderThreadId, 91);
}


void EXPORT grLfbOrigin(GrOriginLocation_t origin)	{

	DEBUG_BEGIN("grLfbOrigin", DebugRenderThreadId, 92);
	
	LOG(1,"\ngrLfbOrigin(origin=%s)", origin_t[origin == GR_ORIGIN_ANY ? 2 : origin]);
	glide211LockInfo.origin = origin;

	DEBUG_END("grLfbOrigin", DebugRenderThreadId, 92);
}


void EXPORT grLfbWriteMode( GrLfbWriteMode_t mode )	{

	DEBUG_BEGIN("grLfbWriteMode", DebugRenderThreadId, 93);
	
	LOG(1,"\ngrLfbWriteMode(mode=%s)", lfbwritemode_t[mode]);
	glide211LockInfo.writeMode = mode;

	DEBUG_END("grLfbWriteMode", DebugRenderThreadId, 93);
}


void EXPORT grLfbBegin()	{
GrLfbInfo_t info;
int i;

	DEBUG_BEGIN("grLfbBegin", DebugRenderThreadId, 94);

	LOG(1,"\ngrLfbBegin()");

	if (!glide211LockInfo.begin)	{
		glide211LockInfo.begin = 1;
		for(i = 0; i < 3; i++)	{
			if (glide211LockInfo.readPtr[i] != NULL)	{
				grLfbLock(GR_LFB_READ_ONLY, i, glide211LockInfo.writeMode, glide211LockInfo.origin, glide211LockInfo.pixelPipeLine, &info);
			}
			if (glide211LockInfo.writePtr[i] != NULL)	{
				grLfbLock(GR_LFB_WRITE_ONLY, i, glide211LockInfo.writeMode, glide211LockInfo.origin, glide211LockInfo.pixelPipeLine, &info);
			}
		}
	}
	
	DEBUG_END("grLfbBegin", DebugRenderThreadId, 94);
}


void EXPORT grLfbEnd()	{
int	i;
	
	DEBUG_BEGIN("grLfbEnd", DebugRenderThreadId, 95);
	
	LOG(1,"\ngrLfbEnd()");
	
	if (glide211LockInfo.begin)	{
		for(i = 0; i < 3; i++)	{
			if (glide211LockInfo.readPtr[i] != NULL)	{
				grLfbUnlock(GR_LFB_READ_ONLY, i);
			}
			if (glide211LockInfo.writePtr[i] != NULL)	{
				grLfbUnlock(GR_LFB_WRITE_ONLY, i);
			}
		}
		glide211LockInfo.begin = 0;
	}
	
	DEBUG_END("grLfbEnd", DebugRenderThreadId, 95);
}


//const FxU32 * EXPORT grLfbGetReadPtr(GrBuffer_t buffer )	{
unsigned int EXPORT grLfbGetReadPtr(GrBuffer_t buffer )	{
GrLfbInfo_t info;
	
	DEBUG_BEGIN("grLfbGetReadPtr",DebugRenderThreadId, 96);
	
	LOG(1,"\ngrLfbGetReadPtr(buffer=%s)", buffer_t[buffer]);

	glide211LockInfo.fbRead = buffer;
	if (glide211LockInfo.readPtr[buffer] == NULL)	{
		info.size = sizeof(GrLfbInfo_t);
		if (grLfbLock(GR_LFB_READ_ONLY, buffer, glide211LockInfo.writeMode, glide211LockInfo.origin, glide211LockInfo.pixelPipeLine, &info))	{
			glide211LockInfo.readPtr[buffer] = info.lfbPtr;
		}
		if (!glide211LockInfo.begin) grLfbUnlock(GR_LFB_READ_ONLY, buffer);
	}
	return((unsigned int) glide211LockInfo.readPtr[buffer]);

	DEBUG_END("grLfbGetReadPtr",DebugRenderThreadId, 96);
}


unsigned int EXPORT grLfbGetWritePtr(GrBuffer_t buffer )	{
GrLfbInfo_t info;

	DEBUG_BEGIN("grLfbGetWritePtr", DebugRenderThreadId, 97);

	LOG(1,"\ngrLfbGetWritePtr(buffer=%s)", buffer_t[buffer]);

	glide211LockInfo.fbWrite = buffer;
	if (glide211LockInfo.writePtr[buffer] == NULL)	{
		info.size = sizeof(GrLfbInfo_t);
		if (grLfbLock(GR_LFB_WRITE_ONLY, buffer, glide211LockInfo.writeMode, glide211LockInfo.origin, glide211LockInfo.pixelPipeLine, &info))	{
			glide211LockInfo.writePtr[buffer] = info.lfbPtr;
		}
		if (!glide211LockInfo.begin) grLfbUnlock(GR_LFB_WRITE_ONLY, buffer);
	}
	return((unsigned int) glide211LockInfo.writePtr[buffer]);

	DEBUG_END("grLfbGetWritePtr", DebugRenderThreadId, 97);
}

#endif


void __inline GlideCopyTempArea(int pitch, int x, int y, unsigned char *dst, unsigned char *src)	{
int i;

	for(i=0;i<y;i++)	{
		CopyMemory(dst,src,2 * x);
		src+=pitch;
		dst+=pitch;
	}
}


void GlideLFBBeforeSwap()	{
int i;

	DEBUG_BEGIN("GlideLFBBeforeSwap",DebugRenderThreadId, 98);
	
	CopyMemory(templocks,bufflocks,3*sizeof(_lock));
	locked = 0;
	for(i = 0; i < 3; i++)	{
		if (bufflocks[i].locktype & LOCK_LOCKED) locked=1;
		if (bufflocks[i].readLockInfo.lockNum)	{
			bufflocks[i].readLockInfo.lockNum=1;
			grLfbUnlock(GR_LFB_READ_ONLY,i);
		}
		if (bufflocks[i].writeLockInfo.lockNum)	{
			bufflocks[i].writeLockInfo.lockNum=1;
			grLfbUnlock(GR_LFB_WRITE_ONLY,i);
		}
		LfbFlushInteralAuxBufferContents (i);
	}
	
	DEBUG_END("GlideLFBBeforeSwap",DebugRenderThreadId, 98);
}


void GlideLFBAfterSwap()	{
int						i,j;
GrOriginLocation_t		origin;
GrLfbInfo_t				info;
LPDIRECTDRAWSURFACE7	lpDDBuff, lpDDBuff2;
unsigned char			*temparea, *temparea_client;
RECT					convRect;
unsigned int			locktype;

	DEBUG_BEGIN("GlideLFBAfterSwap", DebugRenderThreadId, 99);

	bufflocks[0].lpDDbuff = lpDDFrontBuff;
	bufflocks[1].lpDDbuff = lpDDBackBuff;	
	j = 2;
	if (movie.cmiBuffNum == 3)	{		
		if (!depthbuffering)	{
			j = 3;
			bufflocks[2].lpDDbuff = GetThirdBuffer();
		}
	}

	/* Ha bufferswap alatt vmelyik buffer zárolva volt, vagy ha a "real voodoo" opció engedélyezve van, */
	/* akkor a fõ szempont, hogy a pufferek "zárolási címei" ne változzanak. Azaz, nem a bufferek címeit */
	/* cserélgetjük, hanem szükség szerint a tartalmukat. */
	if (locked || (config.Flags & CFG_LFBREALVOODOO) )	{
		convRect.left = convRect.top = 0;
		convRect.right = appXRes;
		convRect.bottom = appYRes;

		/* Ha a segédpufferek léteznek, akkor: */
		/* Mindegyikre megnézzük: ha nem eleve mocskos, akkor ha érdemes másolni, másolunk, */
		/* egyébként piszkosra állítjuk az állapotát */
		if (bufflocks[0].lpDDConvbuff != NULL)	{
			if ( ((bufflocks[0].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == (LOCK_RSOFTWARE | LOCK_RCACHED)) ||
				(( (bufflocks[0].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == LOCK_RCACHED) && (!(bltcaps & BLT_FASTBLT))) )	{
				lpDDCopyTempBuff->lpVtbl->BltFast(lpDDCopyTempBuff, 0, 0, bufflocks[0].lpDDConvbuff,
													&convRect, DDBLTFAST_NOCOLORKEY);
			} else bufflocks[0].locktype &= ~LOCK_CACHEBITS;
			for(i = 1; i < j; i++)	{
				if ( ((bufflocks[i-1].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == (LOCK_RSOFTWARE | LOCK_RCACHED)) ||
					(( (bufflocks[i-1].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == LOCK_RCACHED) && (!(bltcaps & BLT_FASTBLT))) )	{
					bufflocks[i-1].lpDDConvbuff->lpVtbl->BltFast(bufflocks[i-1].lpDDConvbuff,
																 0, 0, bufflocks[i].lpDDConvbuff,
																 &convRect, DDBLTFAST_NOCOLORKEY);

				}
			} bufflocks[i].locktype &= ~LOCK_CACHEBITS;
			if ( ((bufflocks[0].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == (LOCK_RSOFTWARE | LOCK_RCACHED)) ||
				(( (bufflocks[0].locktype & (LOCK_RSOFTWARE | LOCK_RCACHED)) == LOCK_RCACHED) && (!(bltcaps & BLT_FASTBLT))) )	{
				bufflocks[j-1].lpDDConvbuff->lpVtbl->BltFast(bufflocks[j-1].lpDDConvbuff,
																0, 0, lpDDCopyTempBuff,
																&convRect, DDBLTFAST_NOCOLORKEY);
			}
		}

		if ( (bufflocks[0].locktype & LOCK_CCACHED) && (bltcaps & BLT_FASTBLT) )	{
			GlideCopyTempArea(convbuffxres, appXRes, appYRes, CopyTempArea, bufflocks[0].temparea);
		}
		for(i = 1; i < j; i++)	{
			if ( (bufflocks[i].locktype & LOCK_CCACHED) && (bltcaps & BLT_FASTBLT) )	{
				GlideCopyTempArea(convbuffxres, appXRes, appYRes, bufflocks[i-1].temparea, bufflocks[i].temparea);
			}
		}
		if ( (bufflocks[0].locktype & LOCK_CCACHED) && (bltcaps & BLT_FASTBLT) )	{
			GlideCopyTempArea(convbuffxres, appXRes, appYRes, bufflocks[j-1].temparea, CopyTempArea);
		}

		/* Adott tulajdonságok rotálása */
		locktype = bufflocks[0].locktype;
		for(i = 1; i < j; i++) bufflocks[i-1].locktype = bufflocks[i].locktype;
		bufflocks[j-1].locktype = locktype;

		for(i = 0; i < j; i++)	{
			origin = templocks[i].locktype & LOCK_LOWERORIGIN ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
			if (config.Flags2 & CFG_YMIRROR) origin = (origin == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
			if (templocks[i].readLockInfo.lockNum)	{
				grLfbLock(GR_LFB_IDLE | GR_LFB_READ_ONLY, i,
						GR_LFBWRITEMODE_565, origin, FXFALSE, &info);
				bufflocks[i].readLockInfo.lockNum=templocks[i].readLockInfo.lockNum;
			}
			if (templocks[i].writeLockInfo.lockNum)	{
				grLfbLock(GR_LFB_IDLE | GR_LFB_WRITE_ONLY, i,
						templocks[i].writeMode, origin, FXFALSE, &info);
				bufflocks[i].writeLockInfo.lockNum=templocks[i].writeLockInfo.lockNum;
			}
		}		
	} else	{
		/* A pufferek jellemzõit rotáljuk */
		temparea = bufflocks[0].temparea;
		temparea_client = bufflocks[0].temparea_client;
		lpDDBuff = bufflocks[0].lpDDConvbuff;
		lpDDBuff2 = bufflocks[0].lpDDConvbuff2;
		locktype = bufflocks[0].locktype;
		for(i = 1; i < j; i++)	{
			bufflocks[i-1].lpDDConvbuff = bufflocks[i].lpDDConvbuff;
			bufflocks[i-1].lpDDConvbuff2 = bufflocks[i].lpDDConvbuff2;
			bufflocks[i-1].temparea = bufflocks[i].temparea;
			bufflocks[i-1].temparea_client = bufflocks[i].temparea_client;
			bufflocks[i-1].locktype = bufflocks[i].locktype;
		}
		bufflocks[j-1].lpDDConvbuff = lpDDBuff;
		bufflocks[j-1].lpDDConvbuff2 = lpDDBuff2;
		bufflocks[j-1].temparea = temparea;
		bufflocks[j-1].temparea_client = temparea_client;
		bufflocks[j-1].locktype = locktype;
	}
	
	DEBUG_END("GlideLFBAfterSwap", DebugRenderThreadId, 99);
}


void GlideLFBCheckLock(int buffer)	{
int					i;
GrOriginLocation_t	origin;
GrLfbInfo_t			info;

	if (!(bufflocks[buffer].locktype & LOCK_ISLOCKED)) return;
	if ( (lfbflags & LFB_FORCE_FASTWRITE) && (config.Flags & CFG_LFBFASTUNMATCHWRITE) ) return;

	DEBUGLOG(0,"\n   Warning: LfbCheckLock: Unexpected behavior detected: drawing by hardware into a locked buffer");
	DEBUGLOG(0,"\n                          Forcing fastwriting for LFB operations, even for matching formats");
	DEBUGLOG(1,"\n   Figyelmeztetés: LfbCheckLock: Nem várt mûködési mód észlelve: rajzolás a hardverrel egy zárolt pufferbe");
	DEBUGLOG(1,"\n                                 Gyorsírás erõltetése az LFB-mûveletekez, még az egyezõ formátumokhoz is");

	CopyMemory(templocks, bufflocks, 3*sizeof(_lock));
	for(i = 0; i < 3; i++)	{
		if (bufflocks[i].readLockInfo.lockNum)	{
			bufflocks[i].readLockInfo.lockNum = 1;
			grLfbUnlock(GR_LFB_READ_ONLY,i);
		}
		if (bufflocks[i].writeLockInfo.lockNum)	{
			bufflocks[i].writeLockInfo.lockNum = 1;
			grLfbUnlock(GR_LFB_WRITE_ONLY,i);
		}
		LfbFlushInteralAuxBufferContents (i);
	}
	config.Flags |= CFG_LFBFASTUNMATCHWRITE;
	lfbflags |= LFB_FORCE_FASTWRITE;
	for(i = 0; i < 3; i++)	{
		bufflocks[i].locktype &= ~LOCK_CACHEBITS;
		origin = templocks[i].locktype & LOCK_LOWERORIGIN ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (config.Flags2 & CFG_YMIRROR) origin = (origin == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (templocks[i].readLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_READ_ONLY, i,
					GR_LFBWRITEMODE_565, origin, FXFALSE, &info);
			bufflocks[i].readLockInfo.lockNum = templocks[i].readLockInfo.lockNum;
		}
		if (templocks[i].writeLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_WRITE_ONLY, i,
					templocks[i].writeMode, origin, FXFALSE, &info);
			bufflocks[i].writeLockInfo.lockNum = templocks[i].writeLockInfo.lockNum;
		}
	}
}


/*void LFBFlush(int buffer)	{
int					i;
GrOriginLocation_t	origin;
GrLfbInfo_t			info;


	CopyMemory(templocks, bufflocks, 3*sizeof(_lock));
	for(i = 0; i < 3; i++)	{
		if (bufflocks[i].readLockInfo.lockNum)	{
			bufflocks[i].readLockInfo.lockNum = 1;
			grLfbUnlock(GR_LFB_READ_ONLY,i);
		}
		if (bufflocks[i].writeLockInfo.lockNum)	{
			bufflocks[i].writeLockInfo.lockNum = 1;
			grLfbUnlock(GR_LFB_WRITE_ONLY,i);
		}
		LfbFlushInteralAuxBufferContents (i);
	}

	for(i = 0; i < 3; i++)	{
		bufflocks[i].locktype &= ~LOCK_CACHEBITS;
		origin = templocks[i].locktype & LOCK_LOWERORIGIN ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (config.Flags2 & CFG_YMIRROR) origin = (origin == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (templocks[i].readLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_READ_ONLY, i,
					GR_LFBWRITEMODE_565, origin, FXFALSE, &info);
			bufflocks[i].readLockInfo.lockNum = templocks[i].readLockInfo.lockNum;
		}
		if (templocks[i].writeLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_WRITE_ONLY, i,
					templocks[i].writeMode, origin, FXFALSE, &info);
			bufflocks[i].writeLockInfo.lockNum = templocks[i].writeLockInfo.lockNum;
		}
	}
}*/



FxBool EXPORT1 grLfbWriteRegion( GrBuffer_t dst_buffer, 
								FxU32 dst_x, FxU32 dst_y, 
								GrLfbSrcFmt_t src_format, 
								FxU32 src_width, FxU32 src_height, 
								FxI32 src_stride, void *src_data )	{
FxU32			temp;
int				temp_y;
unsigned int	i;
LockInfo		lockInfo;
void			*buffPtr;
unsigned int	pixelSize;
	

	DEBUG_BEGIN("grLfbWriteRegion",DebugRenderThreadId, 100);

	LOG(0,"\ngrLfbWriteRegion(dst_buffer=%s, dst_x=%d, dst_y=%d, src_format=%s, src_width=%d, src_height=%d, src_stride=%d, src_data=%p)",
		                     buffer_t[dst_buffer], dst_x, dst_y, lfbsrcfmt_t[src_format], src_width, src_height,
							 src_stride, src_data);
	
	if (!(runflags & RF_INITOK)) return(FXFALSE);
	
	DEBUGCHECK(0,(writeModeToIndex[src_format] == unsupportedFormat), "\n   Warning: grLfbWriteRegion: Unsupported write format, operation cannot be completed");
	DEBUGCHECK(1,(writeModeToIndex[src_format] == unsupportedFormat), "\n   Figyelmeztetés: grLfbWriteRegion: Nem támogatott írási formátum, a mûveletet nem lehet végrehajtani");
	
	if (src_stride < 0)	{
		src_stride = -src_stride;
		temp_y = dst_y;
		temp_y -= src_height-1;
		src_data = ((char *) src_data) - (src_height-1)*src_stride;
		if (temp_y < 0)	{
			src_height += temp_y;
			src_data = ((char *) src_data) - temp_y*src_stride;
			temp_y = 0;
		}
		dst_y = temp_y;
	}
	if ( (dst_x >= appXRes) || (dst_y >= appYRes) ) return(FXTRUE);
	if ( (src_width == 0) || (src_height == 0) ) return(FXTRUE);
	if ( (temp = dst_x + src_width) > appXRes ) src_width -= temp - appXRes;
	if ( (temp = dst_y + src_height) > appYRes ) src_height -= temp - appYRes;
	
	lockInfo.lockType = LI_LOCKTYPEWRITE;
	lockInfo.colorKey = 0xFFFFFFFF;
	lockInfo.colorKeyMask = 0x0;
	lockInfo.lowerOrigin = 0;
	lockInfo.viaPixelPipeline = 0;
	lockInfo.lockFormat = &wModeTable[writeModeToIndex[src_format]][writecFormat];
	lockInfo.useTextures = //LfbLockViaTextures (LI_LOCKTYPEWRITE, 0);
	lockInfo.textureFormatIndex = writeModeToIndex[src_format];
	lockInfo.lockRect.top = dst_y;
	lockInfo.lockRect.left = dst_x;
	lockInfo.lockRect.right = dst_x + src_width;
	lockInfo.lockRect.bottom = dst_y + src_height;
	pixelSize = lockInfo.lockFormat->BitPerPixel /8;
	
	if (LfbLockBuffer (dst_buffer, &lockInfo)) {
		buffPtr = ((char *) lockInfo.buffPtr) + dst_y * lockInfo.buffPitch + pixelSize * dst_x;
		for(i = 0; i < src_height; i++)	{
			CopyMemory(buffPtr, src_data, pixelSize * src_width);
			buffPtr = ((char *) buffPtr) + lockInfo.buffPitch;
			src_data = ((char *) src_data) + src_stride;
		}
		return (LfbUnlockBuffer (dst_buffer, LI_LOCKTYPEWRITE));
	}

	DEBUGLOG(0, "\n   Error: grLfbWriteRegion: locking requested buffer has failed!");
	DEBUGLOG(1, "\n   Hiba: grLfbWriteRegion: a kívánt puffer zárolása nem sikerült!");
	return (FXFALSE);
	
	DEBUG_END("grLfbWriteRegion",DebugRenderThreadId, 100);
}								


/* A specifikáció szerint ez az eljárás mindig RGB565 formátumban olvas vissza. */
FxBool EXPORT1 grLfbReadRegion(	GrBuffer_t src_buffer, 
								FxU32 src_x, FxU32 src_y, 
								FxU32 src_width, FxU32 src_height, 
								FxU32 dst_stride, void *dst_data )	{

LockInfo		lockInfo;
void			*buffPtr;
unsigned int	i;

	DEBUG_BEGIN("grLfbReadRegion", DebugRenderThreadId, 101);

	LOG(0,"\ngrLfbReadRegion(src_buffer=%s, src_x=%d, src_y=%d, src_width=%d, src_height=%d, dst_stride=%d, dst_data=%p)",
		                     buffer_t[src_buffer], src_x, src_y, src_width, src_height, dst_stride, dst_data);
	
	if (!(runflags & RF_INITOK)) return(FXFALSE);
	
	if ( (src_width == 0) || (src_height == 0) ) return(FXTRUE);
	if ( (src_x >= appXRes) || ((src_x + src_width) > appXRes) ) return(FXFALSE);
	if ( (src_y >= appYRes) || ((src_y + src_height) > appYRes) ) return(FXFALSE);

	lockInfo.lockType = LI_LOCKTYPEREAD;
	lockInfo.colorKey = lockInfo.colorKeyMask = 0x0;
	lockInfo.lowerOrigin = 0;
	lockInfo.viaPixelPipeline = 0;
	lockInfo.lockFormat = &wModeTable[GR_LFBWRITEMODE_565][writecFormat];
	lockInfo.useTextures = 0;
	lockInfo.lockRect.top = src_y;
	lockInfo.lockRect.left = src_x;
	lockInfo.lockRect.right = src_x + src_width;
	lockInfo.lockRect.bottom = src_y + src_height;
	
	if (LfbLockBuffer (src_buffer, &lockInfo)) {
		buffPtr = ((char *) lockInfo.buffPtr) + src_y * lockInfo.buffPitch + 2 * src_x;
		for(i = 0; i < src_height; i++)	{
			CopyMemory(dst_data, buffPtr, 2 * src_width);
			buffPtr = ((char *) buffPtr) + lockInfo.buffPitch;
			dst_data = ((char *) dst_data) + dst_stride;
		}
		return (LfbUnlockBuffer (src_buffer, LI_LOCKTYPEREAD));
	}

	DEBUGLOG(0, "\n   Error: grLfbReadRegion: locking requested buffer has failed!");
	DEBUGLOG(1, "\n   Hiba: grLfbReadRegion: a kívánt puffer zárolása nem sikerült!");
	return (FXFALSE);
	
	DEBUG_END("grLfbReadRegion", DebugRenderThreadId, 101);
}	


#ifdef GLIDE1


void EXPORT guFbReadRegion( int src_x, int src_y, int w, int h, void *dst, const int strideInBytes )	{

	DEBUG_BEGIN("guFbReadRegion", DebugRenderThreadId, 102);

	if (!(runflags & RF_INITOK)) return;

	grLfbReadRegion(glide211LockInfo.fbRead, src_x, src_y, w, h, strideInBytes, dst);

	DEBUG_END("guFbReadRegion", DebugRenderThreadId, 102);
}


void EXPORT guFbWriteRegion( int dst_x, int dst_y, int w, int h, const void *src, const int strideInBytes )	{

	DEBUG_BEGIN("guFbWriteRegion", DebugRenderThreadId, 103);

	if (!(runflags & RF_INITOK)) return;
	
	grLfbWriteRegion(glide211LockInfo.fbWrite, dst_x, dst_y, glide211LockInfo.writeMode, w, h, strideInBytes, (void *) src);

	DEBUG_END("guFbWriteRegion", DebugRenderThreadId, 103);
}

#endif


void EXPORT grLfbConstantAlpha( GrAlpha_t alpha )	{
	
	LOG(0,"\ngrLfbConstantAlpha(alpha = %0x)", alpha);
	constAlpha = (unsigned int) alpha;
}


void EXPORT grLfbConstantDepth( FxU16 depth ) {

	LOG(0,"\ngrLfbConstantDepth(depth = %0x)", depth);
	constDepth = (unsigned int) depth;
}


void EXPORT grLfbWriteColorFormat( GrColorFormat_t cFormat)	{

	DEBUG_BEGIN("grLfbWriteColorFormat", DebugRenderThreadId, 104);

	LOG(0,"\ngrLfbWriteColorFormat(cFormat=%s)",cFormat_t[cFormat]);
	
	if (cFormat == writecFormat) return;
	
	// Az esetleges zárolt puffereket mindenképp elengedjük
	GlideLFBBeforeSwap();
	
	writecFormat = cFormat;

	GlideLFBAfterSwap();

	DEBUG_END("grLfbWriteColorFormat", DebugRenderThreadId, 104);
}


void EXPORT	grLfbWriteColorSwizzle(FxBool swizzleBytes, FxBool swapWords) {
	
	DEBUGLOG(0,"\n   Error: grLfbWriteColorSwizzle: This version of dgVoodoo doesn't support grLfbWriteColorSwizzle !");
	DEBUGLOG(1,"\n   Hiba: grLfbWriteColorSwizzle: A dgVoodoo ezen verziója nem támogatja a grLfbWriteColorSwizzle-t !");
}

/*-------------------------------------------------------------------------------------------*/
/*.............................. Depth buffering függvények .................................*/

void	LfbDetermineWBufferingMethod()	{
D3DDEVICEDESC7	devdesc;
DWORD			savedState[128];
D3DTLVERTEX		vertex[4];
HRESULT			hr;

	//config.Flags |= CFG_WBUFFERMETHODISFORCED;
	//config.Flags &= ~CFG_WBUFFER;

	lpD3Ddevice->lpVtbl->GetCaps(lpD3Ddevice, &devdesc);
	if (devdesc.dvMaxVertexW < 65528.0f)	{
		DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Maximum W-based depth value that device supports is %f, true W-buffering CANNOT be used!", devdesc.dvMaxVertexW);
		DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: Az eszköz által támogatott maximális W-alapú mélységi érték %f, a valódi W-pufferelés NEM használható!", devdesc.dvMaxVertexW);
		return;
	}
	if (config.Flags & CFG_WBUFFERMETHODISFORCED)	{
		if (config.Flags & CFG_WBUFFER)	{
			depthcaps |= WBUFF_OK;
			DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Being forced to use true W-buffering!");
			DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: A wrapper valódi W-pufferelés használatára van kényszerítve!");
		} else {
			DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Being forced to use emulated W-buffering!");
			DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: A wrapper emulált W-pufferelés használatára van kényszerítve!");
		}
	} else {
		if (config.Flags & CFG_RELYONDRIVERFLAGS)	{
			if ( (devdesc.dpcLineCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER) &&
				(devdesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER) )	{
				depthcaps |= WBUFF_OK;
				DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering can be used!");
				DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés használható!");
			} else {
				DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Relying on driver flags, true W-buffering CANNOT be used, using emulated W-buffering instead!");
				DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: A driver által visszaadott flag-ek alapján a valódi W-pufferelés NEM használható, helyette emuláció!");
			}
		} else {

			GlideGetD3DState(savedState);

			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);	
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGENABLE, FALSE);
			
			movie.cmiZBitDepth = (config.Flags2 & CFG_DEPTHEQUALTOCOLOR) ? movie.cmiBitPP : MVZBUFF_AUTO;
			if ( (lpDDdepth = CreateZBuffer()) != NULL)	{
				lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, lpDDBackBuff, 0);
				hr = lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, D3DZB_USEW);
				if (hr == DD_OK)	{
					vertex[0].sy = vertex[1].sy = -0.0f;
					vertex[2].sy = vertex[3].sy = 100.0f;
					vertex[0].sx = -0.0f;
					vertex[1].sx = 100.5f;
					vertex[2].sx = -0.0f;
					vertex[3].sx = 100.5f;
					
					vertex[0].rhw = 1.0f/16384.0f;
					vertex[1].rhw = 1.0f/16384.0f;
					vertex[2].rhw = 1.0f/8192.0f;
					vertex[3].rhw = 1.0f/8192.0f;
					vertex[0].sz = vertex[1].sz = vertex[2].sz = vertex[3].sz = 0.0f;
					vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = 0xFFFFFFFF;
					
					lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZFUNC, D3DCMP_LESS);
					hr = lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);
					if (hr == DD_OK) {
						lpD3Ddevice->lpVtbl->Clear(lpD3Ddevice, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
						lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, vertex, 3, D3DDP_WAIT);
						lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, vertex + 1, 3, D3DDP_WAIT);
						vertex[0].rhw = vertex[1].rhw = vertex[2].rhw = 1.0f/4096.0f;
						vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = 0xFF888888;
						lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, vertex, 3, D3DDP_WAIT);
						//lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_LINELIST, D3DFVF_TLVERTEX, vertex, 2, D3DDP_WAIT);
						//lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_POINTLIST, D3DFVF_TLVERTEX, vertex, 1, D3DDP_WAIT);
						lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
						hr = lpDDBackBuff->lpVtbl->Lock(lpDDBackBuff, NULL, &lfb, DDLOCK_WAIT | DDLOCK_READONLY, NULL);
						if (hr == DD_OK) {
							//Reading back pixel(s) at position (2,4)
							if ( ((unsigned int *) ((char *) lfb.lpSurface + 2 * lfb.lPitch) )[4] == 0)	{
								depthcaps |= WBUFF_OK;
								DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: drawing test: true W-buffering is available!");
								DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: rajzolási teszt: a valódi W-pufferelés rendelkezésre áll!");
							} else {
								DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: drawing test: true W-buffering is NOT available!");
								DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: rajzolási teszt: a valódi W-pufferelés NEM áll rendelkezésre!");
							}
							lpDDBackBuff->lpVtbl->Unlock(lpDDBackBuff, NULL);
						} else {
							DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: drawing test error: cannot read back lfb, W-buffering is not available");
							DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: rajzolási teszt hiba: az lfb-t nem tudom visszaolvasni, a W-pufferelés nem használható");
						}
					} else {
						DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: drawing test error: BeginScene error, W-buffering is not available");
						DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: rajzolási teszt hiba: BeginScene hiba, a W-pufferelés nem használható");
					}
					lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, lpDDBackBuff, 0);
				} else {
					DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: drawing test error: cannot set depth buffering mode to D3DZB_USEW, W-buffering is not available");
					DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: rajzolási teszt hiba: a mélységi pufferelést nem tudom D3DZB_USEW-re állítani, a W-pufferelés nem használható");
				}
				DestroyZBuffer();
			} else {
				DEBUGLOG(0,"\nLfbDetermineWBufferingMethod: Used depth buffer bit depth: %d", movie.cmiZBitDepth);
				DEBUGLOG(0,"\n   Error: LfbDetermineWBufferingMethod: Creating depth buffer failed, W-buffering not available");
				DEBUGLOG(1,"\nLfbDetermineWBufferingMethod: A használt bitmélység: %d", movie.cmiZBitDepth);
				DEBUGLOG(1,"\n	 Hiba: LfbDetermineWBufferingMethod: A mélységi puffer létrehozása nem sikerült, a W-bufferelés nem használható");
			}

			GlideSetD3DState(savedState);
		}
	}
}


void EXPORT grDepthBufferMode( GrDepthBufferMode_t mode )	{
DWORD d3dmode;
DWORD vertexmode;
HRESULT hr;
unsigned int flags;
	
	//return;
	//if (auxbuffmode!=AUX_MODE_NONE) return;
	//mode=GR_DEPTHBUFFER_DISABLE;
	DEBUG_BEGIN("grDepthBufferMode", DebugRenderThreadId, 105);
	
	LOG(0,"\ngrDepthBufferMode(mode=%s)",depthbuffermode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (mode == (GrDepthBufferMode_t) astate.depthbuffmode) return;
	DrawFlushPrimitives(0);
	if (mode == GR_DEPTHBUFFER_DISABLE)	{
		depthbuffering = 0;
		if ( movie.cmiBuffNum == 3 )	{
			bufflocks[2].lpDDbuff = GetThirdBuffer();
			LfbSetBuffDirty(GR_BUFFER_AUXBUFFER);	
			bufflocks[2].disabled = bufflocks[1].disabled;
			bufflocks[2].locktype = 0;
		} else {
			bufflocks[2].disabled = DISABLE_ALL;
			bufflocks[2].lpDDbuff = NULL;
		}
	} else {
		if (!(depthcaps & ZBUFF_OK)) return;
		if (!depthbuffercreated)	{
			movie.cmiFlags |= MV_STENCIL;
			movie.cmiZBitDepth = (config.Flags2 & CFG_DEPTHEQUALTOCOLOR) ? movie.cmiBitPP : MVZBUFF_AUTO;
			if ( (lpDDdepth = CreateZBuffer())==NULL)	{
				DEBUGLOG(0,"\ngrDepthBufferMode: used depth buffer bit depth: %d",movie.cmiZBitDepth);
				DEBUGLOG(0,"\n   Error: grDepthBufferMode: Creating depth buffer failed");
				DEBUGLOG(1,"\ngrDepthBufferMode: a használt bitmélység: %d",movie.cmiZBitDepth);
				DEBUGLOG(1,"\n   Hiba: grDepthBufferMode: A mélységi puffer létrehozása nem sikerült");
				return;
			}
			DEBUGLOG(0,"\ngrDepthBufferMode: Used depth buffer bit depth: %d",movie.cmiZBitDepth);
			DEBUGLOG(0,"\n                   Stencil bits: %d",movie.cmiStencilBitDepth);
			DEBUGLOG(0,"\ngrDepthBufferMode: Creating depth buffer OK");
			DEBUGLOG(1,"\ngrDepthBufferMode  A használt bitmélység: %d",movie.cmiZBitDepth);
			DEBUGLOG(1,"\n                   Stencil bitek száma: %d",movie.cmiStencilBitDepth);
			DEBUGLOG(1,"\ngrDepthBufferMode: A mélységi puffer létrehozása OK");
			if (movie.cmiStencilBitDepth == 0)	{
				DEBUGLOG(0,"\n   Warning: grDepthBufferMode: Couldn't create a depth buffer with stencil component. 'Compare to bias' depth buffering modes won't work.\
							\n                               Tip: your videocard either does not support stencil buffers (old cards) or only does it at higher bit depths.\
							\n                               If you are using a 16 bit mode then try to use the wrapper in 32 bit mode!");

				DEBUGLOG(1,"\n   Figyelmeztetés: grDepthBufferMode: Nem sikerült olyan mélységi puffert létrehozni, amely rendelkezik stencil-összetevõvel.\
							\n                                      A 'Compare to bias'-módok nem fognak mûködni a mélységi puffer használatakor.\
							\n                                      Tipp: a videókártyád vagy egyáltalán nem támogatja a stencil-puffereket (régi kártyák) vagy csak nagyobb\
							\n                                      bitmélység mellett. Ha egy 16 bites módot használsz, próbálj ki egy 32 bites módot!");
			}
			
			depthbuffercreated = 1;
			lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, (renderbuffer == GR_BUFFER_FRONTBUFFER) ? lpDDFrontBuff : lpDDBackBuff, 0);
		}
		bufflocks[2].disabled = DISABLE_ALL;
		bufflocks[2].locktype = 0;
		bufflocks[2].lpDDbuff = lpDDdepth;
		/* For depth buffer reads - may will be removed */
		FillMemory (bufflocks[2].temparea, convbuffxres * appYRes * 2, 0xFF);

	}
	flags = 0;
	switch(astate.depthbuffmode = (DWORD) mode)	{

		case GR_DEPTHBUFFER_ZBUFFER_COMPARE_TO_BIAS:
		case GR_DEPTHBUFFER_ZBUFFER:
			d3dmode = D3DZB_TRUE;
			depthbuffering = DBUFFER_Z;
			vertexmode = VMODE_ZBUFF;
			if (mode == GR_DEPTHBUFFER_ZBUFFER_COMPARE_TO_BIAS) flags = STATE_COMPAREDEPTH;
			bufflocks[2].disabled = DISABLE_READ;
			break;
		
		case GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS:
		case GR_DEPTHBUFFER_WBUFFER:			
			depthbuffering = DBUFFER_W;
			if (mode == GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS) flags = STATE_COMPAREDEPTH;
			if (!(depthcaps & WBUFF_OK)) {
				d3dmode = D3DZB_TRUE;
				vertexmode = VMODE_WBUFFEMU;
			} else {
				vertexmode = VMODE_WBUFF;
				d3dmode = D3DZB_USEW;
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
													D3DRENDERSTATE_TEXTUREPERSPECTIVE,
													astate.perspective = TRUE);
			}
			break;
		
		default:
			d3dmode = D3DZB_FALSE;
			vertexmode = VMODE_ZBUFF;
			break;
	}
	hr = lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, d3dmode);
	DEBUGCHECK(0,(hr!=DD_OK),"\n   Error: grDepthBufferMode: Return of setting depth buffer mode: %0x, error",hr);
	DEBUGCHECK(1,(hr!=DD_OK),"\n   Hiba: grDepthBufferMode: A mélységi puffer módjának beállításának visszatérési értéke: %0x, hiba",hr);
	if (flags != (astate.flags & STATE_COMPAREDEPTH))	{
		if (movie.cmiStencilBitDepth) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILENABLE,(flags & STATE_COMPAREDEPTH) ? TRUE : FALSE);
		astate.flags = (astate.flags & (~STATE_COMPAREDEPTH)) | flags;
		/* Az új módnak megfelelõen vagy nullára (compare to bias) vagy a tényleges bias értékre állítjuk a depth bias-t */
		/* (compare to bias módban a mélységi pufferbe beírt értékekhez nem adódik hozzá a bias, azaz nullára állítjuk) */
		grDepthBiasLevel((FxI16) astate.depthbiaslevel);
	}
	astate.vertexmode &= ~(VMODE_ZBUFF | VMODE_WBUFF | VMODE_WBUFFEMU);
	astate.vertexmode |= vertexmode;
	
	DEBUG_END("grDepthBufferMode",DebugRenderThreadId, 105);
}


void EXPORT grDepthBufferFunction( GrCmpFnc_t func )	{
DWORD temp;
		
	//return;
	DEBUG_BEGIN("grDepthBufferFunction", DebugRenderThreadId, 106);
	
	LOG(0,"\ngrDepthBufferFunction(func=%s)",cmpfnc_t[func]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (func >= 8) return;
	if ((temp = cmpFuncLookUp[func]) != astate.zfunc)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ZFUNC,
											astate.zfunc = temp);
		GlideSetVertexWUndefined();
	}
	
	DEBUG_END("grDepthBufferFunction", DebugRenderThreadId, 106);
}


void EXPORT grDepthMask( FxBool enable )	{
DWORD mask;
	
	//return;
	DEBUG_BEGIN("grDepthMask", DebugRenderThreadId, 107);
	
	LOG(0,"\ngrDepthMask(enable=%s)",fxbool_t[enable]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	mask = enable ? TRUE : FALSE;
	if (mask != astate.zenable)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZWRITEENABLE,
											astate.zenable = mask);
	}
	
	DEBUG_END("grDepthMask", DebugRenderThreadId, 107);
}


void EXPORT grDepthBiasLevel(FxI16 level)	{

	LOG(0,"\ngrDepthBiasLevel(level=%x)",level);
	if (astate.depthbiaslevel != level)	{
		astate.depthbiaslevel = level;
		if (!(astate.flags & STATE_COMPAREDEPTH))	{
			depthBias_W = ((int) level) << 11;
			depthBias_Z = (float) level;
		} else {
			depthBias_W = 0;
			depthBias_Z = 0.0f;
		}
		depthBias_cz = ((float) ((unsigned int) level)) / 65535.0f;
		depthBias_cw = ((float) ((( (unsigned int) level & 0xFFF) | 0x1000) << (((unsigned int)level)>>12))) / (4096.0f);
	}
}


/* Inicializálja a depth bufferinget */
int LfbDepthBufferingInit()	{
DDCAPS		caps = {sizeof(DDCAPS)};
D3DMATRIX	proj;
	
	DEBUG_BEGIN("LfbDepthBufferingInit", DebugRenderThreadId, 108);
	
	depthcaps = 0;
	lpDDdepth = NULL;
	actdepthmode = GR_DEPTHBUFFER_DISABLE;
	lpDD->lpVtbl->GetCaps(lpDD, &caps, NULL);
	if (caps.ddsCaps.dwCaps & DDSCAPS_ZBUFFER) depthcaps |= ZBUFF_OK;
	
	grDepthBufferFunction(GR_CMP_LESS);
	
	depthbuffering = depthbuffercreated = 0;
	ZeroMemory(&proj, sizeof(D3DMATRIX));

	proj._11 = proj._22 = proj._33 = proj._44 = 1.0f;
	proj._33 = (WNEAR / (WFAR - WNEAR)) + 1.0f;
	proj._44 = WNEAR;
	proj._43 = 0.0f;
	proj._34 = 1.0f;
	lpD3Ddevice->lpVtbl->SetTransform(lpD3Ddevice, D3DTRANSFORMSTATE_PROJECTION, &proj);
	
	astate.depthbiaslevel = 0;
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILPASS, D3DSTENCILOP_ZERO);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILFAIL, D3DSTENCILOP_KEEP);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILREF, 0);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILMASK, 0xFFFFFFFF);

	LfbDetermineWBufferingMethod();
	
	return(1);
	
	DEBUG_END("LfbDepthBufferingInit", DebugRenderThreadId, 108);
}


/* Cleanup a depth bufferinghez */
void LfbDepthBufferingCleanUp()	{
	
	DEBUG_BEGIN("LfbDepthBufferingCleanUp", DebugRenderThreadId, 109);

	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
	if (depthbuffercreated) DestroyZBuffer();
	depthbuffering = depthbuffercreated = 0;
	
	DEBUG_END("LfbDepthBufferingCleanUp", DebugRenderThreadId, 109);
}
