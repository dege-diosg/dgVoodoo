/*--------------------------------------------------------------------------------- */
/* LfbDepth.cpp - Glide implementation, stuffs related to                           */
/*                Linear Frame / Depth buffer                                       */
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

extern "C" {

#include	"dgVoodooBase.h"
#include	"Version.h"
#include	"Main.h"
#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>

#include	"dgVoodooConfig.h"
#include	"Dgw.h"
#include	"dgVoodooGlide.h"
#include	"Debug.h"
#include	"Log.h"
#include	"Draw.h"
#include	"LfbDepth.h"
#include	"Texture.h"
#include	"Grabber.h"
#include	"Dos.h"

#include	"MMConv.h"

}

#include	"Renderer.hpp"
#include	"RendererGeneral.hpp"
#include	"RendererTexturing.hpp"
#include	"RendererLfbDepth.hpp"

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók ...... ..................................*/

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
#define LOCK_RNOTFLUSHED	0x40        /* a pufferbe történt írások nincsenek kiürítve */
#define LOCK_RSTATEMASK		(LOCK_RCACHED | LOCK_RFASTCACHED | LOCK_RLOWERORIGIN)

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

#define DEFAULT_TILETEXSIZEX	256
#define DEFAULT_TILETEXSIZEY	256
#define	MAX_TILETEXTURENUM		3


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

	TextureType			tTexture[MAX_TILETEXTURENUM];
	_pixfmt				tTextureFormat;
	DXTextureFormats	dxTexFormatIndex;

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

	unsigned char			*temparea;		/* átmeneti terület, ahová és ahonnan konvertálunk (BUFF_CONVERTING) */
	unsigned char			*temparea_client;  /* ugyanezen terület címe a kliens címterében (DOS, XP-hez) */
	unsigned int			colorKey;		/* Gyorsírás esetén a felhasznált colorkey ARGB-ben */
	unsigned int			colorKeyMask;	/* Gyorsírás esetén a colorkeyhez felhasznált maszk ARGB-ben */
	_plockinfo				writeLockInfo;
	_plockinfo				readLockInfo;
	void					*ptr;
	unsigned int			stride;
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
void					*copyTempArea;
GrColorFormat_t			readcFormat, writecFormat;
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
DWORD					depthBufferingModeToVertexMode[] = {VMODE_ZBUFF, VMODE_WBUFFEMU, VMODE_WBUFF, VMODE_ZBUFF};

unsigned int			tileTexSizeX = DEFAULT_TILETEXSIZEX;
unsigned int			tileTexSizeY = DEFAULT_TILETEXSIZEY;
unsigned int			tileTexNum = 0;

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

/* Lekérdezi az LFB formátumát, s megnézi, hogy támogatja-e azt vagy sem. Ennek megfelelõen */
/* Az egyes bufferek zárolhatóságát is beállítja. */
int		LfbGetAndCheckLFBFormat()	{
unsigned int		disabletype;

	DEBUG_BEGINSCOPE("LfbGetAndCheckLFBFormat", DebugRenderThreadId);
	
	rendererLfbDepth->GetLfbFormat (&backBufferFormat);

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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


/*DXTextureFormats	LfbPixFmtToDXTexFormat (_pixfmt* pixFmt)
{
	switch (pixFmt->BitPerPixel) {
		case 16:
			if (pixFmt->RBitCount
			break;
		case 32:
			break;
	}
	
}*/


void LfbDestroyTileTextures() {

	for (unsigned int i = 0; i < numberOfFormats; i++)	{
		for(unsigned int j = 0; j < tileTexNum; j++) {
			if (tileTextures[i].tTexture[j] != NULL)
				rendererTexturing->DestroyDXTexture(tileTextures[i].tTexture[j]);
			tileTextures[i].tTexture[j] = NULL;
		}
	}
}


int	LfbCreateTileTextures()	{
int						i;
unsigned char			*requiredFormat;
enum DXTextureFormats	texFormat = Pf_Invalid;

	ZeroMemory (tileTextures, numberOfFormats * sizeof (TileTextureData));

	if (GetLfbTexureTilingMethod (config.Flags2) == CFG_LFBDISABLEDTEXTURING)
		return (1);

	if (rendererTexturing->AreNonPow2TexturesSupported ()) {
		tileTexNum = 1;
		tileTexSizeX = appXRes;
		tileTexSizeY = appYRes;
	} else {
		tileTexNum = MAX_TILETEXTURENUM;
		tileTexSizeX = DEFAULT_TILETEXSIZEX;
		tileTexSizeY = DEFAULT_TILETEXSIZEY;
	}

	for (i = 0; i < numberOfFormats; i++)	{
		for(requiredFormat = tileTexFormats[i]; *requiredFormat != Pf_Invalid; requiredFormat++) {
			texFormat = (enum DXTextureFormats) *requiredFormat;
			if (TexIsDXFormatSupported (texFormat))
				break;
		}
		if (*requiredFormat != Pf_Invalid)	{
			TexGetDXTexPixFmt (texFormat, &tileTextures[i].tTextureFormat);
			tileTextures[i].dxTexFormatIndex = texFormat;
			for(unsigned int j = 0; j < tileTexNum; j++) {
				if (!rendererTexturing->CreateDXTexture(texFormat, tileTexSizeX, tileTexSizeY, 1, &tileTextures[i].tTexture[j], 0)) {
					LfbDestroyTileTextures ();
					return (0);
				}
			}
		}
	}
	return (1);
}


#ifndef GLIDE3

#ifdef	DOS_SUPPORT

int LfbSetUpLFBDOSBuffers(unsigned char *buff0, unsigned char *buff1, unsigned char *buff2, int onlyclient)	{
	
	DEBUG_BEGINSCOPE("LfbSetUpLFBDOSBuffers", DebugRenderThreadId);
	
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

	//Fake Depth buffer olvasáshoz
	if (movie.cmiBuffNum != 3)
		FillMemory (bufflocks[2].temparea, convbuffxres * appYRes * 2, 0xFF);

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (1);
}

#endif

#endif


/* Létrehozza, s inicializálja a belsõ puffereket, stb. */
int LfbInit()	{
int i;

	DEBUG_BEGINSCOPE("GlideLFBInit", DebugRenderThreadId);

	//config.Flags2 |= CFG_DEPTHEQUALTOCOLOR;
	ZeroMemory(bufflocks, 3 * sizeof(_lock));
	
#ifdef GLIDE1
	ZeroMemory(&glide211LockInfo, sizeof(Glide211LockInfo));
#endif
	if ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) == (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) )
		config.Flags &= ~CFG_HWLFBCACHING;
	
	convbuffxres = appXRes;
	if ( (config.Flags & CFG_LFBREALVOODOO) && (appXRes <= 1024) ) convbuffxres = 1024;

	if (!rendererLfbDepth->InitLfb (RendererLfbDepth::Complex, config.Flags & CFG_HWLFBCACHING, config.Flags & CFG_LFBREALVOODOO))
		return (0);

	if (platform == PLATFORM_WINDOWS)	{
		for(i = 0; i < 3; i++) {
			if ( (bufflocks[i].temparea = bufflocks[i].temparea_client = (unsigned char *) _GetMem(convbuffxres * appYRes * LFBMAXPIXELSIZE)) == NULL)
				return(0);
		}

		//Fake Depth buffer olvasáshoz
		if (movie.cmiBuffNum != 3)
			FillMemory (bufflocks[2].temparea, convbuffxres * appYRes * 2, 0xFF);
	}
	if ( (copyTempArea = _GetMem(convbuffxres * appYRes * LFBMAXPIXELSIZE)) == NULL) return(0);
	
	readcFormat = writecFormat = (config.Flags2 & CFG_CFORMATAFFECTLFB) ? cFormat : GR_COLORFORMAT_ARGB;

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

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


/* Clean up az LFB-hez */
void LfbCleanUp()	{
	
	DEBUG_BEGINSCOPE("GlideLFBCleanUp",DebugRenderThreadId);

	/* Az esetlegesen zárolva maradt pufferek feloldása */
	LfbBeforeSwap ();
	LfbDestroyTileTextures();
	if (copyTempArea) _FreeMem(copyTempArea);
	copyTempArea = NULL;

	if (rendererLfbDepth != NULL)
		rendererLfbDepth->DeInitLfb ();
	
	if (platform == PLATFORM_WINDOWS)	{
		if (bufflocks[0].temparea) _FreeMem(bufflocks[0].temparea);
		if (bufflocks[1].temparea) _FreeMem(bufflocks[1].temparea);
		if (bufflocks[2].temparea) _FreeMem(bufflocks[2].temparea);
		bufflocks[0].temparea = bufflocks[1].temparea = bufflocks[2].temparea = NULL;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
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


int 	DoTextureTiling (_lock *lockPtr)
{
	float	constDepthOow, constDepthOoz;

	if (!(lockPtr->locktype & LOCK_PIPELINE))	{
		rendererGeneral->SaveRenderState (RendererGeneral::LfbNoPipeline);
		rendererGeneral->SetRenderState (RendererGeneral::LfbNoPipeline, lockPtr->colorKeyMask != 0x0, 1, 0);
		constDepthOow = constDepthOoz = 1.0f;
	} else {
		rendererGeneral->SaveRenderState (RendererGeneral::LfbPipeline);
		rendererGeneral->SetRenderState (RendererGeneral::LfbPipeline, lockPtr->colorKeyMask != 0x0,
										 lockPtr->lockformat->ABitCount == 0, constAlpha);
		constDepthOow = (astate.vertexmode & VMODE_WBUFFEMU) ? DrawGetFloatFromDepthValue (constDepth, DVNonlinearZ) : 
															   DrawGetFloatFromDepthValue (constDepth, DVTrueW) / 65528.0f;
		constDepthOoz = DrawGetFloatFromDepthValue (constDepth, DVLinearZ);
	}

	rendererTexturing->SetTextureStageMagFilter (0, GR_TEXTUREFILTER_POINT_SAMPLED);
	rendererTexturing->SetTextureStageMinFilter (0, GR_TEXTUREFILTER_POINT_SAMPLED);
	
	int formatIndex = lockPtr->textureFormatIndex;

	unsigned int ctype;
	
 	if (lockPtr->colorKeyMask == 0x0)	{
		ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY;
	} else	{
		ctype = MMCIsPixfmtTheSame(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat) ? CONVERTTYPE_RAWCOPY/*COLORKEY*/ : CONVERTTYPE_COLORKEY;
	}

	unsigned int texColorKey = MMCConvertPixel (MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
												lockPtr->lockformat,
												&tileTextures[formatIndex].tTextureFormat,
												constAlpha);

	//unsigned int texColorKeyARGB = MMCGetARGBFromPixelValue (texColorKey, &tileTextures[formatIndex].tTextureFormat, constAlpha);

	float texScaleX = ((float) movie.cmiXres) / appXRes;
	float texScaleY = ((float) movie.cmiYres) / appYRes;
	RECT  flushRect;
	unsigned int origX, origY, sizeX, sizeY, buffOrigY;

	if (lockPtr->locktype & LOCK_CLOWERORIGIN) {
		SetRect (&flushRect, lockPtr->lockRect.left, appYRes - lockPtr->lockRect.bottom,
							 lockPtr->lockRect.right, appYRes - lockPtr->lockRect.top);
		buffOrigY = lockPtr->lockRect.bottom;
	} else {
		SetRect (&flushRect, lockPtr->lockRect.left, lockPtr->lockRect.top,
							 lockPtr->lockRect.right, lockPtr->lockRect.bottom);
		buffOrigY = lockPtr->lockRect.top;
	}

	unsigned int tileTexInd = 0;
	origY = flushRect.top;
	TextureType currTexture = tileTextures[formatIndex].tTexture[0];
	TextureType nextTexture;
	
	/*if ((ctype == CONVERTTYPE_RAWCOPYCOLORKEY) || (ctype == CONVERTTYPE_COLORKEY)) {
		rendererTexturing->FillTexture (currTexture, 0, texColorKey, &tileTextures[formatIndex].tTextureFormat);
	}*/

	unsigned int oldTexColorKey = rendererTexturing->GetSrcColorKeyForTextureFormat (tileTextures[formatIndex].dxTexFormatIndex);

	rendererTexturing->SetSrcColorKeyForTextureFormat (tileTextures[formatIndex].dxTexFormatIndex, texColorKey);
	TextureType lpDDNativeCkAux = rendererTexturing->GetAuxTextureForNativeCk (tileTextures[formatIndex].dxTexFormatIndex);
	if (lpDDNativeCkAux != NULL)
		rendererTexturing->SetTextureAtStage (1, lpDDNativeCkAux);

	unsigned int incrX, incrY, drawX, drawY;
	
	while(sizeY = flushRect.bottom - origY, sizeY != 0) {
		// itt >= volt!!
		if (sizeY > tileTexSizeY)	{
			sizeY = tileTexSizeY;
			incrY = 2; drawY = 2;
		} else {
			incrY = 0; drawY = 0;//1;
		}
		origX = flushRect.left;
		while(sizeX = flushRect.right - origX, sizeX != 0) {
			if (sizeX > tileTexSizeX)	{
				sizeX = tileTexSizeX;
				incrX = 2; drawX = 2;
			} else {
				incrX = 0; drawX =0;//1;
			}
			
			rendererTexturing->RestoreTexturePhysicallyIfNeeded (currTexture); //!!!!!!
			/*if (lpDDbuff->lpVtbl->IsLost (lpDDbuff) == DDERR_SURFACELOST) {
				if (lpDDbuff->lpVtbl->Restore (lpDDbuff) != DD_OK)
					return(-1);
			}*/

			if ((ctype == CONVERTTYPE_RAWCOPYCOLORKEY) || (ctype == CONVERTTYPE_COLORKEY))
				rendererTexturing->FillTexture (currTexture, 0, texColorKey, &tileTextures[formatIndex].tTextureFormat);

			if (++tileTexInd == tileTexNum)
				tileTexInd = 0;
			
			nextTexture = tileTextures[formatIndex].tTexture[tileTexInd];

			/*if ((ctype == CONVERTTYPE_RAWCOPYCOLORKEY) || (ctype == CONVERTTYPE_COLORKEY))
				rendererTexturing->FillTexture (nextTexture, 0, texColorKey, &tileTextures[formatIndex].tTextureFormat);*/

			unsigned int texturePitch;
			void* texturePtr = rendererTexturing->GetPtrToTexture (currTexture, 0, &texturePitch);
			if (texturePtr == NULL)
				return (-1);

			unsigned int temp = (lockPtr->locktype & LOCK_CLOWERORIGIN) ? buffOrigY - sizeY : buffOrigY;

			/* Writing to texture tile */
			MMCConvertAndTransferData(lockPtr->lockformat, &tileTextures[formatIndex].tTextureFormat,
									  MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
									  MMCGetPixelValueFromARGB(lockPtr->colorKeyMask, lockPtr->lockformat),
									  constAlpha,
									  ((char *) lockPtr->temparea) + (wModeTable[formatIndex]->BitPerPixel /8) * ((temp * convbuffxres) + origX),
									  texturePtr,
									  sizeX, sizeY,
									  ((lockPtr->locktype & LOCK_CLOWERORIGIN) ? -1 : 1) * ((lockPtr->lockformat->BitPerPixel / 8) * convbuffxres),
									  texturePitch,
									  NULL, NULL,
									  ctype,
									  0);
			rendererTexturing->ReleasePtrToTexture (currTexture, 0);

			rendererTexturing->SetSrcColorKeyForTexture (currTexture, texColorKey);

			rendererTexturing->SetTextureAtStage (0, currTexture);

			currTexture = nextTexture;

			DrawTile( (origX * texScaleX),
					  (origY * texScaleY),
					  ((origX + sizeX - drawX) * texScaleX),
					  ((origY + sizeY - drawY) * texScaleY),
					  (0.0f + 0.5f) / tileTexSizeX,
					  (0.0f + 0.5f) / tileTexSizeY, 
					  (((float) sizeX - drawX) + 0.5f) / tileTexSizeX,
					  (((float) sizeY - drawY) + 0.5f) / tileTexSizeY,
					  constDepthOow, constDepthOoz, 0);
			origX += sizeX - incrX;
		}
		origY += sizeY - incrY;
		if (lockPtr->locktype & LOCK_CLOWERORIGIN) {
			buffOrigY -= (sizeY - incrY);
		} else {
			buffOrigY += sizeY - incrY;
		}
	}

	rendererTexturing->SetSrcColorKeyForTextureFormat (tileTextures[formatIndex].dxTexFormatIndex, oldTexColorKey);

	if (lockPtr->locktype & LOCK_PIPELINE)	{
		rendererGeneral->RestoreRenderState (RendererGeneral::LfbPipeline);
	} else {
		rendererGeneral->RestoreRenderState (RendererGeneral::LfbNoPipeline);
	}

	return (1);
}


int LfbFlushInternalAuxBuffer (unsigned int buffer, _lock *lockPtr)	{
unsigned int			ctype;
unsigned int			pixelSize;
RECT					*lRect, dstRect;
unsigned int			dTop;
float					xScale, yScale;
int						renderBuffer;
RendererLfbDepth::BlitType		blitType;
void*					buffPtr;
unsigned int			buffPitch;
RendererLfbDepth::BufferType	bufferType;

	switch(buffer)	{
		case BUFF_RESIZING:
			if (lockPtr->locktype & LOCK_RNOTFLUSHED)	{
				xScale = ((float) movie.cmiXres) / appXRes;
				yScale = ((float) movie.cmiYres) / appYRes;
				dstRect.left = (LONG) (lockPtr->lockRect.left * xScale);
				dstRect.right = (LONG) (lockPtr->lockRect.right * xScale);
				
				if (!((lockPtr->colorKeyMask == 0x0) && (lockPtr->colorKey == 0x0))) {
					if (lockPtr->locktype & LOCK_RLOWERORIGIN)	{
						dstRect.top = (LONG) ((appYRes - lockPtr->lockRect.bottom) * yScale);
						dstRect.bottom = (LONG) ((appYRes - lockPtr->lockRect.top) * yScale);
					} else {
						dstRect.top = (LONG) (lockPtr->lockRect.top * yScale);
						dstRect.bottom = (LONG) (lockPtr->lockRect.bottom * yScale);
					}

					blitType = ((lockPtr->locktype & LOCK_RFASTCACHED) && (lockPtr->colorKeyMask != 0x0)) ?
								RendererLfbDepth::CopyWithColorKey : RendererLfbDepth::Copy;
					
					rendererLfbDepth->BlitFromScalingBuffer (lockPtr->bufferNum, &lockPtr->lockRect, &dstRect,
															 blitType, lockPtr->locktype & LOCK_RLOWERORIGIN,
															 drawresample);
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

						bufferType = drawresample ? RendererLfbDepth::ScalingBuffer : RendererLfbDepth::ColorBuffer;
						if (!rendererLfbDepth->LockBuffer (lockPtr->bufferNum, bufferType, &buffPtr, &buffPitch))
							return (0);

						lRect = &lockPtr->lockRect;
						dTop = (lockPtr->locktype & LOCK_CLOWERORIGIN) ? appYRes - lRect->bottom : lRect->top;
						pixelSize = lockPtr->lockformat->BitPerPixel / 8;

						/* Writing to LFB */
						MMCConvertAndTransferData(lockPtr->lockformat, &backBufferFormat,
												  MMCGetPixelValueFromARGB(lockPtr->colorKey, lockPtr->lockformat),
												  MMCGetPixelValueFromARGB(lockPtr->colorKeyMask, lockPtr->lockformat),
												  constAlpha,
												  ((char *) lockPtr->temparea) + pixelSize * (lRect->top * convbuffxres + lRect->left),
												  ((char *) buffPtr) + dTop * buffPitch + lRect->left * backBufferFormat.BitPerPixel / 8,
												  lRect->right - lRect->left, lRect->bottom - lRect->top,
												  (lockPtr->locktype & LOCK_CLOWERORIGIN ? -1 : 1) * (lockPtr->lockformat->BitPerPixel / 8) * convbuffxres,
												  buffPitch,
												  NULL, NULL,
												  ctype, 0);

						if (!rendererLfbDepth->UnlockBuffer (lockPtr->bufferNum, bufferType))
							return (-1);

						if (drawresample) lockPtr->locktype |= LOCK_RNOTFLUSHED;
					} else {
					
						renderBuffer = renderbuffer;
						grRenderBuffer (lockPtr->bufferNum);
						

						int retVal = 1;
						if (!(astate.flags & STATE_NORENDERTARGET))
							retVal = DoTextureTiling (lockPtr);
						
						grRenderBuffer (renderBuffer);

						if (retVal != 1)
							return (retVal);
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


void LfbInvalidateCachedContent ()
{
	for (int i = 0; i < 3; i++) {
		bufflocks[i].locktype &= ~LOCK_CACHEBITS;
	}
}


int	LfbIsMatchingStatesInScalingBuffer (_lock *lockPtr, unsigned int requiredState, LockInfo *pLockInfo) {
	
	return !( (requiredState & LOCK_RSTATEMASK) != (lockPtr->locktype & LOCK_RSTATEMASK) ||
			((requiredState & LOCK_RFASTCACHED) && (pLockInfo->colorKey != lockPtr->colorKey)) ||
			 (!IsRectContained (&pLockInfo->lockRect, &lockPtr->lockRect)) );
}


int LfbPrepareInternalResizingBuffer (_lock *lockPtr, unsigned int requiredState, LockInfo *pLockInfo, int pointerIsInUse)	{
RECT					srcRect;
float					xScale, yScale;

	/* A kívánt állapot megegyezik a jelenlegivel? */
	if (!LfbIsMatchingStatesInScalingBuffer(lockPtr, requiredState, pLockInfo)) {
		
		LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr);
		
		if ((pLockInfo->colorKeyMask != 0x0) || (pLockInfo->lockType == LI_LOCKTYPEREAD)) {

			xScale = ((float) movie.cmiXres) / appXRes;
			yScale = ((float) movie.cmiYres) / appYRes;
			srcRect.left = (LONG) (pLockInfo->lockRect.left * xScale);
			srcRect.right = (LONG) (pLockInfo->lockRect.right * xScale);
			if (requiredState & LOCK_RLOWERORIGIN)	{
				srcRect.top = (LONG) ((appYRes - pLockInfo->lockRect.bottom) * yScale);
				srcRect.bottom = (LONG) ((appYRes - pLockInfo->lockRect.top) * yScale);
			} else {
				srcRect.top = (LONG) (pLockInfo->lockRect.top * yScale);
				srcRect.bottom = (LONG) (pLockInfo->lockRect.bottom * yScale);
			}
			
			if (requiredState & LOCK_RFASTCACHED)	{
				rendererLfbDepth->FillScalingBuffer (lockPtr->bufferNum, &pLockInfo->lockRect, pLockInfo->colorKey,
													 requiredState & LOCK_RLOWERORIGIN, pointerIsInUse); 

			} else {
				rendererLfbDepth->BlitToScalingBuffer (lockPtr->bufferNum, &srcRect, &pLockInfo->lockRect,
													   requiredState & LOCK_RLOWERORIGIN, drawresample, pointerIsInUse);

			}
		}
		lockPtr->locktype &= ~(LOCK_LOCKED | LOCK_RSTATEMASK);
		lockPtr->locktype |= requiredState;
	}
	return(1);
}


int LfbLockInternalAuxBuffer (unsigned int buffer, LockInfo *pLockInfo, _lock *lockptr) {
unsigned int					requiredstate;
unsigned int					pixelSize;
RECT							*lRect;
unsigned int					dTop;
void*							buffPtr;
unsigned int					buffPitch;
RendererLfbDepth::BufferType	bufferType;

	switch(buffer)	{
		case BUFF_RESIZING:
			/* A kívánt állapot meghatározása */
			requiredstate = (pLockInfo->lowerOrigin && rendererLfbDepth->IsMirroringSupported ()) ? LOCK_RLOWERORIGIN : 0;
			requiredstate |= (pLockInfo->lockType == LI_LOCKTYPEREAD) ? LOCK_RCACHED :
							 ( (lfbflags & LFB_FORCE_FASTWRITE) ? LOCK_RFASTCACHED : LOCK_RCACHED);
			if ( ((requiredstate & LOCK_RLOWERORIGIN) == (lockptr->locktype & LOCK_RLOWERORIGIN)) && (lockptr->locktype & LOCK_RCACHED) )	{
				requiredstate |= LOCK_RCACHED;
				requiredstate &= ~LOCK_RFASTCACHED;
			}
			
			/* Szépséghiba: ha a megkívánt állapot megegyezik a jelenlegivel, feleslegesen lefut egy unlock-lock pár */
			if (lockptr->locktype & LOCK_ISLOCKED) {
				if (!rendererLfbDepth->UnlockBuffer (lockptr->bufferNum, RendererLfbDepth::ScalingBuffer))
					return (0);
			}
			
			if (!LfbPrepareInternalResizingBuffer (lockptr, requiredstate, pLockInfo, lockptr->locktype & LOCK_ISLOCKED))
				return(0);

			lockptr->colorKey = pLockInfo->colorKey;
			lockptr->colorKeyMask = pLockInfo->colorKeyMask;

			if (!rendererLfbDepth->LockBuffer (lockptr->bufferNum, RendererLfbDepth::ScalingBuffer, &lockptr->ptr, &lockptr->stride))
				return (0);
			
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
						bufferType = drawresample ? RendererLfbDepth::ScalingBuffer : RendererLfbDepth::ColorBuffer;
						if (!rendererLfbDepth->LockBuffer (lockptr->bufferNum, bufferType, &buffPtr, &buffPitch))
							return (0);
						
						/* Reading from LFB */
						MMCConvertAndTransferData(&backBufferFormat, pLockInfo->lockFormat,
												  0, 0, constAlpha,
												  ((char *) buffPtr) + dTop * buffPitch + lRect->left * backBufferFormat.BitPerPixel / 8,
												  ((char *) lockptr->temparea) + pixelSize * (lRect->top * convbuffxres + lRect->left),
												  lRect->right - lRect->left, lRect->bottom - lRect->top,
												  buffPitch,
												  ((requiredstate & LOCK_CLOWERORIGIN) ? -1 : 1) * pixelSize * convbuffxres,
												  NULL, NULL,
												  MMCIsPixfmtTheSame(pLockInfo->lockFormat, &backBufferFormat) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
												  0);
						if (!rendererLfbDepth->UnlockBuffer (lockptr->bufferNum, bufferType))
							return (0);
					}
				}
				lockptr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE | LOCK_CLOWERORIGIN);
				lockptr->locktype |= requiredstate;
			}
			if (pLockInfo->lockType == LI_LOCKTYPEWRITE) lockptr->locktype |= LOCK_CNOTFLUSHED;
			lockptr->ptr = lockptr->temparea_client;
			lockptr->stride = pixelSize * convbuffxres;
			break;
		
	}
	if (pLockInfo->viaPixelPipeline) lockptr->locktype |= LOCK_PIPELINE;
	lockptr->lockRect = pLockInfo->lockRect;
	return(1);
}


int LfbEvaluateAlphaTestFunction (GrCmpFnc_t alphaTestFunc, DWORD alphaValue, DWORD alphaReference) {
	
	switch(alphaTestFunc) {
		case GR_CMP_NEVER:
			return (0);
		case GR_CMP_LESS:
			return (alphaValue < alphaReference);
		case GR_CMP_EQUAL:
			return (alphaValue == alphaReference);
		case GR_CMP_LEQUAL:
			return (alphaValue <= alphaReference);
		case GR_CMP_GREATER:
			return (alphaValue > alphaReference);
		case GR_CMP_NOTEQUAL:
			return (alphaValue != alphaReference);
		case GR_CMP_GEQUAL:
			return (alphaValue >= alphaReference);
		case GR_CMP_ALWAYS:
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
			*colorKey = GlideGetColor(astate.colorkey);
			*colorKeyMask = 0x00FFFFFF;
		} else if (astate.alphaTestFunc != GR_CMP_ALWAYS) {
			if (writeMode == GR_LFBWRITEMODE_1555)	{
				evalBool1 = LfbEvaluateAlphaTestFunction (astate.alphaTestFunc, 0x0, astate.alpharef);
				evalBool2 = LfbEvaluateAlphaTestFunction (astate.alphaTestFunc, 0xFF, astate.alpharef);
				if (evalBool1 == evalBool2)	{
					if (!evalBool1)
						*colorKeyMask = *colorKey = 0x0;
				} else {
					*colorKeyMask = 0xFF000000;
					*colorKey = (evalBool1) ? (DEFAULTCOLORKEY | 0xFF000000) : (DEFAULTCOLORKEY & ~0xFF000000);
				}
			} else {
				if (!LfbEvaluateAlphaTestFunction (astate.alphaTestFunc, constAlpha, astate.alpharef)) {
					*colorKeyMask = *colorKey = 0x0;
				}
			}
		} /*else if () {
		}*/
	}
}


int LfbLockViaTextures (GrLock_t type, GrLfbWriteMode_t writeMode, FxBool pixelPipeline) {
	
	if (type != GR_LFB_READ_ONLY) {
		if (GetLfbTexureTilingMethod (config.Flags2) != CFG_LFBDISABLEDTEXTURING)	{

			if (pixelPipeline) {
				if (astate.alphaBlendEnable || (astate.fogmode != GR_FOG_DISABLE) ||
					(astate.depthBuffMode != GR_DEPTHBUFFER_DISABLE) ||
					((astate.alphaTestFunc != GR_CMP_ALWAYS) && (writeMode == GR_LFBWRITEMODE_8888)) ) {
					LOG (0, "\n     LockViaTextures: TRUE");
					return 1;
				}
			}
		}

		return (GetLfbTexureTilingMethod (config.Flags2) == CFG_LFBFORCEDTEXTURING);
	}

	return (0);
}


int		LfbLockBuffer (int buffer, LockInfo *pLockInfo) {
_lock					*lockPtr;
_plockinfo				*plockPtr;
unsigned int			lOrigin;
unsigned int			disableMask, requiredState;
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

	DrawFlushPrimitives();
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

	/* Ha a cooperative level nem megfelelõ, akkor minimális lenne az esély, hogy a */
	/* lockolni kivánt puffer nem veszett el, de amúgy sem látjuk a glide-alkalmazás */
	/* képernyõjét, ezért ekkor az átmeneti terület cimét adjuk vissza. Ha már zárolva */
	/* van a puffer úgy, hogy nem az átmeneti terület cimét adtuk vissza, akkor */
	/* a normális módon próbáljuk mégegyszer zárolni, legfeljebb nem sikerül. */
	if (rendererGeneral->GetDeviceStatus () != RendererGeneral::StatusOk) {
		if ((lockPtr->locktype & LOCK_TEMPDISABLE) || (!(lockPtr->locktype & LOCK_LOCKED)))
			lockPtr->locktype |= LOCK_TEMPDISABLE;
	}
	
	disableMask = (pLockInfo->lockType == LI_LOCKTYPEREAD) ? DISABLE_READ : DISABLE_WRITE;
	/* Ha a pufferelérés tiltott, akkor az átmeneti terület címével visszatérünk, és kész. */
	if ( (!(lockPtr->disabled & disableMask)) && (!(lockPtr->locktype & LOCK_TEMPDISABLE)) )	{
		/* Megnézzük, hogy a segédpuffer megvan-e még, ha nem, akkor helyreállitjuk */
		//if (rendererLfbDepth->RestoreBufferIfNeeded (lockPtr->bufferNum, RendererLfbDepth::ScalingBuffer))
		//	lockPtr->locktype &= ~LOCK_CACHEBITS;

		/* A lockolni kivánt puffer esetleges helyreállitása: teljes képernyõn mindig a */
		/* frontbuffert állitjuk helyre, mert egy complex surface tagjait külön-külön */
		/* nem lehet helyreállitani. */
		//if (rendererLfbDepth->RestoreBufferIfNeeded (lockPtr->bufferNum, RendererLfbDepth::ColorBuffer))
		//	lockPtr->locktype &= ~LOCK_CACHEBITS;

		if (buffer == 0) GrabberRestoreFrontBuff();

		/* mely puffert használjuk majd a lockoláshoz */
		if (!(lockPtr->locktype & LOCK_LOCKED))	{
			//if (!depthBuffer)	{
				
				lockPtr->usedBuffer = BUFF_COLOR;
				
				if ( drawresample || /*pLockInfo->lowerOrigin ||*/  //!!!!!!!
					(config.Flags & CFG_LFBREALVOODOO) || (lfbflags & LFB_FORCE_FASTWRITE) ) {

					lockPtr->usedBuffer = BUFF_RESIZING;
				}
				
				if ( (!MMCIsPixfmtTheSame(pLockInfo->lockFormat, &backBufferFormat)) ||
					 (lfbflags & LFB_NO_MATCHING_FORMATS) || pLockInfo->useTextures ) {
					lockPtr->usedBuffer = BUFF_CONVERTING;
				}


				/* További döntés: ha a rescaling buffert colorkey-es blitteléssel kellene használni, de az implementáció */
				/* nem támogatja, kényszerítjük a textúracsempézést és a converting buffert */
				if (GetLfbTexureTilingMethod (config.Flags2) != CFG_LFBDISABLEDTEXTURING &&
					pLockInfo->lockType == LI_LOCKTYPEWRITE) {
				
					if (lockPtr->usedBuffer == BUFF_RESIZING) {
						if (lfbflags & LFB_FORCE_FASTWRITE) {
							if (!rendererLfbDepth->IsBlitTypeSupported (RendererLfbDepth::CopyWithColorKey)) {
								pLockInfo->useTextures = 1;
								lockPtr->usedBuffer = BUFF_CONVERTING;
							}
						}

					} else if (drawresample && (lockPtr->usedBuffer == BUFF_CONVERTING)) {
						if (lfbflags & LFB_RESCALE_CHANGES_ONLY)
							pLockInfo->useTextures |= !rendererLfbDepth->IsBlitTypeSupported (RendererLfbDepth::CopyWithColorKey);
					}
				}
			
			/*} else {
				lockPtr->usedBuffer = BUFF_DEPTH;
			}*/
		}
		
		if ( (lockPtr->locktype & LOCK_DIRTY) && (!(lockPtr->locktype & LOCK_ISLOCKED)) )
			lockPtr->locktype &= ~(LOCK_DIRTY | LOCK_CACHEBITS);

		int negativePitch = 0;

		if (!drawresample)	{
			switch(lockPtr->usedBuffer)	{
				case BUFF_COLOR:
					if (!(lockPtr->locktype & LOCK_ISLOCKED))	{
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
						if (rv) lockPtr->locktype &= ~(LOCK_RCACHED | LOCK_RFASTCACHED);
						if ((rv = LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr)) == -1) return(0);
						if (rv) lockPtr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE);
						if (!rendererLfbDepth->LockBuffer (lockPtr->bufferNum, RendererLfbDepth::ColorBuffer, &lockPtr->ptr, &lockPtr->stride))
							return (0);

						negativePitch = pLockInfo->lowerOrigin;
					}
					break;
				
				case BUFF_RESIZING:
					/* Ha a konvertálóterületet ki kell üríteni, akkor az átméretezõpuffer tartalma érvénytelenné válik */
					if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
					if (rv) lockPtr->locktype &= ~(LOCK_RCACHED | LOCK_RFASTCACHED);
					if (!LfbLockInternalAuxBuffer (BUFF_RESIZING, pLockInfo, lockPtr))
						return(0);

					negativePitch = (pLockInfo->lowerOrigin && !rendererLfbDepth->IsMirroringSupported ());

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
					if (rv) lockPtr->locktype &= ~(LOCK_CCACHED | LOCK_CFASTCACHED | LOCK_CTEXTURE);
					if (!LfbLockInternalAuxBuffer (BUFF_RESIZING, pLockInfo, lockPtr))
						return(0);

					negativePitch = (pLockInfo->lowerOrigin && !rendererLfbDepth->IsMirroringSupported ());
					
					break;
				
				case BUFF_CONVERTING:

					if (!pLockInfo->useTextures) {

						requiredState = (pLockInfo->lockType == LI_LOCKTYPEREAD) ? LOCK_RCACHED :
										 ( (lfbflags & LFB_RESCALE_CHANGES_ONLY) ? LOCK_RFASTCACHED : LOCK_RCACHED);
						if ( (!(lockPtr->locktype & LOCK_RLOWERORIGIN)) && (lockPtr->locktype & LOCK_RCACHED) )	{
							requiredState |= LOCK_RCACHED;
							requiredState &= ~LOCK_RFASTCACHED;
						}

						if (!LfbIsMatchingStatesInScalingBuffer (lockPtr, requiredState, pLockInfo)) {
							if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1) return(0);
						}

						if (!LfbPrepareInternalResizingBuffer (lockPtr, requiredState, pLockInfo, 0)) return(0);

						if (!LfbLockInternalAuxBuffer (BUFF_CONVERTING, pLockInfo, lockPtr))
							return(0);
					
					} else {

						if (!(lockPtr->locktype & LOCK_CTEXTURE)) {
							if ((rv = LfbFlushInternalAuxBuffer (BUFF_CONVERTING, lockPtr)) == -1)
								return(0);
						}

						if ((rv = LfbFlushInternalAuxBuffer (BUFF_RESIZING, lockPtr)) == -1)
							return(0);

						if (!LfbLockInternalAuxBuffer (BUFF_CONVERTING, pLockInfo, lockPtr))
							return(0);

						if (rv) lockPtr->locktype &= ~(LOCK_RCACHED | LOCK_RFASTCACHED);
					}
					break;
			}
		}
		if (!negativePitch) {
			pLockInfo->buffPtr = lockPtr->ptr;
			pLockInfo->buffPitch = lockPtr->stride;
		} else {
			pLockInfo->buffPtr = ((char *) lockPtr->ptr) + (movie.cmiYres-1) * lockPtr->stride;
			pLockInfo->buffPitch = -((int) (lockPtr->stride));
		}
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

	lockPtr = bufflocks + buffer;
	plockPtr = (lockType == LI_LOCKTYPEWRITE) ? &lockPtr->writeLockInfo : &lockPtr->readLockInfo;
	
	if (plockPtr->lockNum == 0) return(0);
	lockSum = lockPtr->readLockInfo.lockNum + lockPtr->writeLockInfo.lockNum;

	disableMask = (lockType == LI_LOCKTYPEWRITE) ? DISABLE_WRITE : DISABLE_READ;
	if ( (!(lockPtr->disabled & disableMask)) && ((--lockSum) == 0) && (!(lockPtr->locktype & LOCK_TEMPDISABLE)) )	{
		switch(lockPtr->usedBuffer)	{
			case BUFF_COLOR:
				if (!rendererLfbDepth->UnlockBuffer (lockPtr->bufferNum, RendererLfbDepth::ColorBuffer))
					return (0);
				break;
			case BUFF_RESIZING:
				if (!rendererLfbDepth->UnlockBuffer (lockPtr->bufferNum, RendererLfbDepth::ScalingBuffer))
					return (0);
				break;
			/*case BUFF_DEPTH:
				lpDDbuff = lockPtr->lpDDbuff;
				if (lpDDbuff->lpVtbl->Unlock(lpDDbuff, NULL) != DD_OK) return(0);
				break;*/
		}
	}
	plockPtr->lockNum--;
	if (!(lockPtr->disabled & disableMask))	{
		if (lockSum) {
			lockPtr->locktype |= LOCK_LOCKED;
		} else {
			lockPtr->locktype &= ~(LOCK_ISLOCKED);
			if ( (lockPtr->locktype & LOCK_PIPELINE) || (config.Flags2 & CFG_LFBALWAYSFLUSHWRITES) ||
				(buffer == GR_BUFFER_FRONTBUFFER) )
				LfbFlushInteralAuxBufferContents (buffer);
			lockPtr->locktype &= ~LOCK_PIPELINE;
		}
		if (buffer == GR_BUFFER_FRONTBUFFER)
			rendererGeneral->ShowContentChangesOnFrontBuffer ();
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

	DEBUG_BEGINSCOPE("grLfbLock", DebugRenderThreadId);
	
	LOG(1,"\ngrLfbLock(type=%s, buffer=%s, writeMode=%s, origin=%s, pixelPipeline=%d, info=%p)", \
		        locktype_t[type], buffer_t[buffer], lfbwritemode_t[(writeMode==GR_LFBWRITEMODE_ANY) ? 0x10 : writeMode], origin_t[origin], \
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
	if ( (buffer != GR_BUFFER_AUXBUFFER) || ( (movie.cmiBuffNum == 3) && (depthbuffering == None) ) )	{
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
	
	if ( (buffer == GR_BUFFER_AUXBUFFER) && (depthbuffering != None))	{
		DEBUGLOG(0,"\n   Warning: grLfbLock: Application tries to lock the depth or alpha buffer. This feature is unsupported in this version.");
		DEBUGLOG(1,"\n   Figyelmeztetés: grLfbLock: Az alkalmazás a mélységi vagy alpha-puffert próbálja meg zárolni. Ezt ez a verzió nem támogatja.");
		depthBuffer = 1;

		info->lfbPtr = bufflocks[2].temparea;
		info->strideInBytes = movie.cmiXres * 2;
		info->origin = GR_ORIGIN_UPPER_LEFT;

		if (type == GR_LFB_WRITE_ONLY) {
			rendererGeneral->ClearBuffer (CLEARFLAG_DEPTHBUFFER, NULL, 0, 1.0f);
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

		if (config.debugFlags & DBG_FORCEFRONTBUFFER)
			buffer = GR_BUFFER_FRONTBUFFER;
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

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}


FxBool EXPORT1 grLfbUnlock( GrLock_t type, GrBuffer_t buffer )	{
FxBool retVal;

	DEBUG_BEGINSCOPE("grLfbUnlock", DebugRenderThreadId);

	LOG(0,"\ngrLfbUnlock(type=%s, buffer=%s)", locktype_t[type], buffer_t[buffer]);
	if (!(runflags & RF_INITOK)) return(FXFALSE);

	//buffer = GR_BUFFER_FRONTBUFFER;

	if ( (buffer != GR_BUFFER_FRONTBUFFER) && (buffer != GR_BUFFER_BACKBUFFER) 
			&& (buffer != GR_BUFFER_AUXBUFFER) ) return(FXFALSE);

	type &= GR_LFB_READ_ONLY | GR_LFB_WRITE_ONLY;

	if ((buffer != GR_BUFFER_AUXBUFFER) && (config.debugFlags & DBG_FORCEFRONTBUFFER))
			buffer = GR_BUFFER_FRONTBUFFER;
	
	retVal = LfbUnlockBuffer (buffer, (type & GR_LFB_WRITE_ONLY) ? LI_LOCKTYPEWRITE : LI_LOCKTYPEREAD);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return retVal;
}


#ifdef GLIDE1

void EXPORT grLfbBypassMode( GrLfbBypassMode_t mode )	{
GrLfbInfo_t info;
FxBool		pixelPipeLine;

	DEBUG_BEGINSCOPE("grLfbBypassMode", DebugRenderThreadId);
	
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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grLfbOrigin(GrOriginLocation_t origin)	{

	DEBUG_BEGINSCOPE("grLfbOrigin", DebugRenderThreadId);
	
	LOG(1,"\ngrLfbOrigin(origin=%s)", origin_t[origin == GR_ORIGIN_ANY ? 2 : origin]);
	glide211LockInfo.origin = origin;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grLfbWriteMode( GrLfbWriteMode_t mode )	{

	DEBUG_BEGINSCOPE("grLfbWriteMode", DebugRenderThreadId);
	
	LOG(1,"\ngrLfbWriteMode(mode=%s)", lfbwritemode_t[mode]);
	glide211LockInfo.writeMode = mode;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grLfbBegin()	{
GrLfbInfo_t info;
int i;

	DEBUG_BEGINSCOPE("grLfbBegin", DebugRenderThreadId);

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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grLfbEnd()	{
int	i;
	
	DEBUG_BEGINSCOPE("grLfbEnd", DebugRenderThreadId);
	
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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


//const FxU32 * EXPORT grLfbGetReadPtr(GrBuffer_t buffer )	{
unsigned int EXPORT grLfbGetReadPtr(GrBuffer_t buffer )	{
GrLfbInfo_t info;
	
	DEBUG_BEGINSCOPE("grLfbGetReadPtr",DebugRenderThreadId);
	
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

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


unsigned int EXPORT grLfbGetWritePtr(GrBuffer_t buffer )	{
GrLfbInfo_t info;

	DEBUG_BEGINSCOPE("grLfbGetWritePtr", DebugRenderThreadId);

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

	DEBUG_ENDSCOPE(DebugRenderThreadId);
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


void LfbBeforeSwap ()	{
int i;

	DEBUG_BEGINSCOPE("LfbBeforeSwap", DebugRenderThreadId);
	
	CopyMemory(templocks, bufflocks, 3 * sizeof(_lock));
	locked = 0;
	for(i = 0; i < 3; i++)	{
		if (bufflocks[i].locktype & LOCK_LOCKED) locked=1;
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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void LfbAfterSwap ()	{
int						i,j;
GrOriginLocation_t		origin;
GrLfbInfo_t				info;
unsigned char			*temparea, *temparea_client;
RECT					convRect;
unsigned int			locktype;

	DEBUG_BEGINSCOPE("LfbAfterSwap", DebugRenderThreadId);

	j = movie.cmiBuffNum;

	int	rotateBuffers = 1;
	/* Ha bufferswap alatt vmelyik buffer zárolva volt, vagy ha a "real voodoo" opció engedélyezve van, */
	/* akkor a fõ szempont, hogy a pufferek "zárolási címei" ne változzanak. Azaz, nem a bufferek címeit */
	/* cserélgetjük, hanem szükség szerint a tartalmukat. */
	if (locked || (config.Flags & CFG_LFBREALVOODOO) )	{
		convRect.left = convRect.top = 0;
		convRect.right = appXRes;
		convRect.bottom = appYRes;

		int dstBuff;
		//Skálázópufferek
		for (dstBuff = 0; dstBuff < j; dstBuff++) {
			if (!(bufflocks[dstBuff].locktype & LOCK_RCACHED))
				break;
		}
		dstBuff = (dstBuff == j) ? 3 : dstBuff;
		int srcBuff, srcBuffIndex;
		int firstBuff = dstBuff;
		do {
			if (dstBuff == 3)
				srcBuff = 0;
			else
				srcBuff = (dstBuff == (j - 1)) ? ((firstBuff == 3) ? 3 : 0) : dstBuff + 1;
			srcBuffIndex = (srcBuff != 3) ? srcBuff : 0;
			if (bufflocks[srcBuffIndex].locktype & LOCK_RCACHED) {
				if (!rendererLfbDepth->CopyBetweenScalingBuffers (srcBuff, dstBuff, &convRect))
					bufflocks[srcBuffIndex].locktype &= ~LOCK_RSTATEMASK;
			} else
				bufflocks[srcBuffIndex].locktype &= ~LOCK_RSTATEMASK;

			dstBuff = srcBuff;
		} while (dstBuff != firstBuff);

		//Szoftveres konvertálópufferek
		for (dstBuff = 0; dstBuff < j; dstBuff++) {
			if (!(bufflocks[dstBuff].locktype & LOCK_CCACHED))
				break;
		}
		firstBuff = dstBuff = (dstBuff == j) ? 3 : dstBuff;
		do {
			if (dstBuff == 3)
				srcBuff = 0;
			else
				srcBuff = (dstBuff == (j - 1)) ? ((firstBuff == 3) ? 3 : 0) : dstBuff + 1;
			srcBuffIndex = (srcBuff != 3) ? srcBuff : 0;
			if ((bufflocks[srcBuffIndex].locktype & LOCK_CCACHED) && rendererLfbDepth->AreHwScalingBuffersUsed ()) {
				unsigned char* srcPtr = (srcBuff == 3) ? (unsigned char *) copyTempArea : bufflocks[srcBuff].temparea;
				unsigned char* dstPtr = (dstBuff == 3) ? (unsigned char *) copyTempArea : bufflocks[dstBuff].temparea;
				GlideCopyTempArea(convbuffxres, appXRes, appYRes, dstPtr, srcPtr);
			} else
				bufflocks[srcBuffIndex].locktype &= ~LOCK_CSTATEMASK;
			dstBuff = srcBuff;
		} while (dstBuff != firstBuff);

		rotateBuffers = 0;
	}
	/* A pufferek jellemzõit rotáljuk */
	if (rotateBuffers) {
		temparea = bufflocks[0].temparea;
		temparea_client = bufflocks[0].temparea_client;
	}
	locktype = bufflocks[0].locktype;
	for(i = 1; i < j; i++)	{
		if (rotateBuffers) {
			bufflocks[i-1].temparea = bufflocks[i].temparea;
			bufflocks[i-1].temparea_client = bufflocks[i].temparea_client;
		}
		bufflocks[i-1].locktype = bufflocks[i].locktype;
	}
	if (rotateBuffers) {
		bufflocks[j-1].temparea = temparea;
		bufflocks[j-1].temparea_client = temparea_client;
	}
	bufflocks[j-1].locktype = locktype;

	rendererLfbDepth->RotateBufferProperties (j, rotateBuffers);

	for(i = 0; i < j; i++)	{
		origin = templocks[i].locktype & LOCK_LOWERORIGIN ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (config.Flags2 & CFG_YMIRROR) origin = (origin == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;
		if (templocks[i].readLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_READ_ONLY, i, GR_LFBWRITEMODE_565, origin, FXFALSE, &info);
			bufflocks[i].readLockInfo.lockNum = templocks[i].readLockInfo.lockNum;
		}
		if (templocks[i].writeLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_WRITE_ONLY, i, templocks[i].writeMode, origin, FXFALSE, &info);
			bufflocks[i].writeLockInfo.lockNum = templocks[i].writeLockInfo.lockNum;
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void LfbCheckLock(int buffer)	{
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
			grLfbLock(GR_LFB_IDLE | GR_LFB_READ_ONLY, i, GR_LFBWRITEMODE_565, origin, FXFALSE, &info);
			bufflocks[i].readLockInfo.lockNum = templocks[i].readLockInfo.lockNum;
		}
		if (templocks[i].writeLockInfo.lockNum)	{
			grLfbLock(GR_LFB_IDLE | GR_LFB_WRITE_ONLY, i, templocks[i].writeMode, origin, FXFALSE, &info);
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
	

	DEBUG_BEGINSCOPE("grLfbWriteRegion",DebugRenderThreadId);

	LOG(0,"\ngrLfbWriteRegion(dst_buffer=%s, dst_x=%d, dst_y=%d, src_format=%s, src_width=%d, src_height=%d, src_stride=%d, src_data=%p)", \
		                     buffer_t[dst_buffer], dst_x, dst_y, lfbsrcfmt_t[src_format], src_width, src_height, \
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
	lockInfo.useTextures = LfbLockViaTextures (GR_LFB_READ_ONLY, src_format, 0);
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
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (FXFALSE);
}								


/* A specifikáció szerint ez az eljárás mindig RGB565 formátumban olvas vissza. */
FxBool EXPORT1 grLfbReadRegion(	GrBuffer_t src_buffer, 
								FxU32 src_x, FxU32 src_y, 
								FxU32 src_width, FxU32 src_height, 
								FxU32 dst_stride, void *dst_data )	{

LockInfo		lockInfo;
void			*buffPtr;
unsigned int	i;

	DEBUG_BEGINSCOPE("grLfbReadRegion", DebugRenderThreadId);

	LOG(0,"\ngrLfbReadRegion(src_buffer=%s, src_x=%d, src_y=%d, src_width=%d, src_height=%d, dst_stride=%d, dst_data=%p)", \
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
		
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (FXFALSE);
}	


#ifdef GLIDE1


void EXPORT guFbReadRegion( int src_x, int src_y, int w, int h, void *dst, const int strideInBytes )	{

	DEBUG_BEGINSCOPE("guFbReadRegion", DebugRenderThreadId);

	if (!(runflags & RF_INITOK)) return;

	grLfbReadRegion(glide211LockInfo.fbRead, src_x, src_y, w, h, strideInBytes, dst);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guFbWriteRegion( int dst_x, int dst_y, int w, int h, const void *src, const int strideInBytes )	{

	DEBUG_BEGINSCOPE("guFbWriteRegion", DebugRenderThreadId);

	if (!(runflags & RF_INITOK)) return;
	
	grLfbWriteRegion(glide211LockInfo.fbWrite, dst_x, dst_y, glide211LockInfo.writeMode, w, h, strideInBytes, (void *) src);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
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

	DEBUG_BEGINSCOPE("grLfbWriteColorFormat", DebugRenderThreadId);

	LOG(0,"\ngrLfbWriteColorFormat(cFormat=%s)",cFormat_t[cFormat]);
	
	if (cFormat == writecFormat) return;
	
	// Az esetleges zárolt puffereket mindenképp elengedjük
	LfbBeforeSwap ();
	
	writecFormat = cFormat;

	LfbAfterSwap ();

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT	grLfbWriteColorSwizzle(FxBool /*swizzleBytes*/, FxBool /*swapWords*/) {
	
	DEBUGLOG(0,"\n   Error: grLfbWriteColorSwizzle: This version of dgVoodoo doesn't support grLfbWriteColorSwizzle !");
	DEBUGLOG(1,"\n   Hiba: grLfbWriteColorSwizzle: A dgVoodoo ezen verziója nem támogatja a grLfbWriteColorSwizzle-t !");
}

/*-------------------------------------------------------------------------------------------*/
/*.............................. Depth buffering függvények .................................*/


void EXPORT grDepthBufferMode( GrDepthBufferMode_t mode )	{
	
	//return;
	//if (auxbuffmode!=AUX_MODE_NONE) return;
	//mode=GR_DEPTHBUFFER_DISABLE;
	DEBUG_BEGINSCOPE("grDepthBufferMode", DebugRenderThreadId);
	
	LOG(0,"\ngrDepthBufferMode(mode=%s)",depthbuffermode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	
	if (mode == (GrDepthBufferMode_t) astate.depthBuffMode) return;
	DrawFlushPrimitives();
	
	if (mode == GR_DEPTHBUFFER_DISABLE)	{
		if ( movie.cmiBuffNum == 3 )	{
			LfbSetBuffDirty(GR_BUFFER_AUXBUFFER);	
			bufflocks[2].disabled = bufflocks[1].disabled;
			bufflocks[2].locktype = 0;
		} else {
			bufflocks[2].disabled = DISABLE_ALL;
		}
	} else {
		bufflocks[2].disabled = DISABLE_ALL;
		bufflocks[2].locktype = 0;
		/* For depth buffer reads - may will be removed */
		if (movie.cmiBuffNum == 3)
			FillMemory (bufflocks[2].temparea, convbuffxres * appYRes * 2, 0xFF);
	}
	rendererLfbDepth->SetDepthBufferingMode (mode);

	astate.depthBuffMode = mode;

	depthbuffering = rendererLfbDepth->GetTrueDepthBufferingMode ();
	astate.vertexmode &= ~(VMODE_ZBUFF | VMODE_WBUFF | VMODE_WBUFFEMU);
	astate.vertexmode |= depthBufferingModeToVertexMode[depthbuffering];

	GlideUpdateUsingTmuWMode ();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDepthBufferFunction( GrCmpFnc_t func )	{
		
	//return;
	//func = GR_CMP_ALWAYS;
	DEBUG_BEGINSCOPE("grDepthBufferFunction", DebugRenderThreadId);
	
	LOG(0,"\ngrDepthBufferFunction(func=%s)",cmpfnc_t[func]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;

	if ((func <= GR_CMP_ALWAYS) && (func != astate.depthFunction)) {

		DrawFlushPrimitives();
		rendererLfbDepth->SetDepthBufferFunction (func);
		astate.depthFunction = func;
		GlideSetVertexWUndefined();
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDepthMask( FxBool enable )	{
	
	//return;
	DEBUG_BEGINSCOPE("grDepthMask", DebugRenderThreadId);
	
	LOG(0,"\ngrDepthMask(enable=%s)",fxbool_t[enable]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	
	if (enable != astate.depthMaskEnable) {
		
		DrawFlushPrimitives();
		rendererLfbDepth->EnableDepthMask (enable);
		astate.depthMaskEnable = enable;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDepthBiasLevel(FxI16 level)	{

	LOG(0,"\ngrDepthBiasLevel(level=%x)",level);
	if (astate.depthbiaslevel != level)	{

		DrawFlushPrimitives();
		rendererLfbDepth->SetDepthBias (level);
		astate.depthbiaslevel = level;
	}
}


/* Inicializálja a depth bufferinget */
int LfbDepthBufferingInit()	{

enum RendererLfbDepth::DepthBufferingDetectMethod detectMethod;
	
	DEBUG_BEGINSCOPE("LfbDepthBufferingInit", DebugRenderThreadId);

	if (config.Flags & CFG_WBUFFERMETHODISFORCED)	{
		detectMethod = (config.Flags & CFG_WBUFFER) ? RendererLfbDepth::AlwaysUseW : RendererLfbDepth::AlwaysUseZ;
	} else {
		if (config.Flags & CFG_RELYONDRIVERFLAGS)	{
			detectMethod = RendererLfbDepth::DetectViaDriver;
		} else {
			DEBUGLOG(0,"\n   Error: LfbDepthBufferingInit: Invalid config options, method of drawing test is removed in this version!");
			DEBUGLOG(1,"\n   Hiba: LfbDepthBufferingInit: Érvénytelen konfig-opció, a rajzolási teszt módszerét ez a verzió nem tartalmazza!");
		}
	}

	if (!rendererLfbDepth->InitDepthBuffering (detectMethod))
		return (0);

	grDepthBufferFunction(GR_CMP_LESS);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


/* Cleanup a depth bufferinghez */
void LfbDepthBufferingCleanUp()	{
	
	DEBUG_BEGINSCOPE("LfbDepthBufferingCleanUp", DebugRenderThreadId);

	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);

	rendererLfbDepth->DeInitDepthBuffering ();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}
