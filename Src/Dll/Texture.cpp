/*--------------------------------------------------------------------------------- */
/* Texture.cpp - dgVoodoo Glide implementation, stuffs related to texturing         */
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
/* dgVoodoo: Texture.c																		*/
/*			 Függvények a textúrázáshoz														*/
/*------------------------------------------------------------------------------------------*/

extern "C" {

#include	"dgVoodooBase.h"
#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>
#include	<stdio.h>

#include	"dgVoodooConfig.h"
#include	"Dgw.h"
#include	"File.h"
#include	"dgVoodooGlide.h"
#include	"Debug.h"
#include	"Log.h"
#include	"LfbDepth.h"
#include	"Draw.h"
#include	"Version.h"
#include	"Main.h"

#include	"Texture.h"
#include	"mmconv.h"

}

#include	"Renderer.hpp"
#include	"RendererTexturing.hpp"
#include	"RendererGeneral.hpp"

/*------------------------------------------------------------------------------------------*/
/*........................... Makrók a régi függvények helyett .............................*/

#define	GlideGetTexBYTEPP(format)		formatBYTEPP[format]
#define GlideGetLOD(lod)				lodToSize[lod]
#define GlideHasAlphaComponent(format)	hasAlphaComponent[format] 
#define GlideIsTableFormat(format)		isTableFormat[format]

#define HasDxFormatAlphaComponent (enum DXTextureFormats format)	hasDxFormatAlphaComponent[format]

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók ...... ..................................*/

//#define	WRITETEXTURESTOBMP

/* Az elõre elkészített speciális textúrák címei */
#define RESERVED_ADDRESSSPACE	0xFFFF0000
#define TEXTURE_C0A0			RESERVED_ADDRESSSPACE


#define MAX_TEXNUM				32768
#define TEXPOOL_NUM				64

/* Utility-textúrák max száma */
#define MAX_UTIL_TEXTURES		1024

/* Max hány stage-en használunk textúrákat */
/* Textúrák kiosztása: */
/* Normál textúrázás esetén: 0 - MAX_STAGETEXTURES-1 normál textúra */
/* Multitextúrázás esetén:	 0	 color textúra */
/*							 1 - MAX_STAGETEXTURES-1 alpha textúra */
#define MAX_STAGETEXTURES		4

/* Az érvénytelen colorkey-t csak az alfa-alapú colorkeying esetén használjuk: */
/* (ilyenkor a colorkey: (3dfx colormask<<16) | (3dfx texel color), azaz  */
/* a colorkey nem DX-textúraformátumban van, hanem 3Dfx textúraformátumban */
/* egy maszkkal együtt, ami a texel színének maszkja. ) */
/* Ha egy textúrát létrehoztunk, vagy újratöltöttünk, ezzel az értékkel */
/* kényszerítjük ki, hogy a hozzá tartozó maszk-textúra is újratöltõdjön. */
#define INVALID_COLORKEY		0xEEEEEEEE

/* Textúra-flagek */
#define TF_SECONDARY			0x1			/* Multibase: Másodlagos textúra (fizikailag nem tárol textúrát, csak a */
											/* szülõmipmap egyik szintjének paramétereit tárolja) */
#define TF_STORESONELEVEL		0x2			/* Multibase: Jelzi, hogy az elsõdleges textúra hossz paramétere (endAddress) */
											/* csak a legelsõ szintet veszi figyelembe */
#define TF_SECRELOAD			0x4			/* Multibase: újonnan allokált elsõdleges textúra: a másodlagos szinteket */
											/* mindenképpen újra kell tölteni (nem elég, ha azok szülõként ezt a bejegyzést jelölik) */
#define TF_STOREDMIPMAP			0x8			/* A letöltött mipmap másolata eltárolva (legacy) */
#define TF_CONSIDEREDASLOST		0x10		/* A textúrát úgy tekintjük, mintha elveszett volna: újra kell tölteni */
#define TF_BITMAPCHANGED		0x20

#define GLIDE_3DF_LINE_LENGTH	1024		/* 3DF file sorainak max. hossza */

/* Flagek a textúra-reload függvényhez */
#define RF_TEXTURE				0x1
#define RF_PALETTE				0x2
#define RF_NOPALETTEMAPCREATE	0x4

/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák  ..................................*/

/* Palettás és NCC-táblás textúrák esetén egy átmeneti terület, ahol a paletta/NCC tábla is */
/* és a mipmap bitmapje is el van tárolva													*/
struct	PalettedData {

	PaletteType lpDDPal;
	union	{
		GuTexPalette palette;
		GuNccTable ncctable;
	};
	unsigned char palmap[256];
	unsigned char data[];
};


/* Egy adott Voodoo-tárcímen levô textúra adatai a könnyebb elérhetôséghez */
typedef struct TexCacheEntry {

	FxU32					startAddress, endAddress;	/* Voodoo textúramembeli kezdô-, végcím */
	TextureType				texBase[9];					/* 9 textúra handle-je a mipmaphez */
	TextureType				texCkAlphaMask[9];			/* Opcionális árnyék-textúra (alfa alapú colorkeyinghez) */
	TextureType				texAP88AlphaChannel[9];
	TextureType				texAuxPTextures[9];
	unsigned short			x,y;						/* szélesség, magasság */
														/* (Ne aspect ratiokkal kelljen faszolni) */
	unsigned short			mipMapCount;
	unsigned char			pool, flags;
	struct PalettedData		*palettedData;				/* Táblás textúrák táblái (paletta, ncc tábla) és */
														/* nyers (nem konvertált) bitmapje */
	NamedFormat				*dxFormat;					/* Textúra DirectX-pixelformátuma */
	NamedFormat				*alphaDxFormat;				/* Alpha-channel DirectX-formátuma */
	NamedFormat				*auxPFormat;				/* Aux palettatextúra DirectX-formátuma */
	GrTexInfo				info;						/* Mipmap infója */
	
	unsigned int			colorKeyNative;				/* A textúrához beállított colorkey textúraformátumban */
	unsigned int			colorKeyAlphaBased;
	union	{
		struct TexCacheEntry *parentMipMap;
		struct TexCacheEntry *next;						/* nem haszn. textúra, köv. üres elem: NULL = lánc vége */
	};
											
} TexCacheEntry;


/*-------------------------------------------------------------------------------------------*/

/* Egy adott mipmap lehetséges aspect ratio-i (ASCII) */
static const /*unsigned*/ char	*textasprats[]		= { "8 1", "4 1", "2 1", "1 1", "1 2", "1 4", "1 8" };

/* és a megfelelô konstansok */
static const GrAspectRatio_t textaspratconst[]	= {	GR_ASPECT_8x1, GR_ASPECT_4x1, GR_ASPECT_2x1, GR_ASPECT_1x1,
													GR_ASPECT_1x2, GR_ASPECT_1x4, GR_ASPECT_1x8 };

/* Egy adott mipmap lehetséges és pillanatnyilag támogatott formátumai */
static const /*unsigned*/ char	*textfmtstr[]		= { "i8", "a8", "ai44", "yiq", "rgb332", "rgb565", "argb8332",
													"argb1555", "ayiq8422", "argb4444", "ai88", "p8", "ap88" };

/* és a megfelelô konstansok */
static const GrTextureFormat_t textfmtconst[]	= { GR_TEXFMT_INTENSITY_8, GR_TEXFMT_ALPHA_8,
													GR_TEXFMT_ALPHA_INTENSITY_44, GR_TEXFMT_YIQ_422,
													GR_TEXFMT_RGB_332, GR_TEXFMT_RGB_565,
													GR_TEXFMT_ARGB_8332, GR_TEXFMT_ARGB_1555,
													GR_TEXFMT_AYIQ_8422, GR_TEXFMT_ARGB_4444,
													GR_TEXFMT_ALPHA_INTENSITY_88,
													GR_TEXFMT_P_8, GR_TEXFMT_AP_88 };

static const GrLOD_t	textlods[]				= { GR_LOD_1, GR_LOD_2, GR_LOD_4, GR_LOD_8,
													GR_LOD_16, GR_LOD_32, GR_LOD_64, GR_LOD_128, GR_LOD_256};


/*-------------------------------------------------------------------------------------------*/

static TexCacheEntry		*texInfo[TEXPOOL_NUM];		/* mutatók a textúraadat-poolokra */
static unsigned int			texNumInPool[TEXPOOL_NUM];
static TexCacheEntry		**texindex;					/* textúrák indexe (voodoo-cím szerint növekedve) */
static TexCacheEntry		*ethead;					/* nem használt bejegyzések láncának a feje */
static unsigned char		*mipMapTempArea;
static unsigned int			utilMemFreeBase;
static unsigned int			mmidIndex;
static TexCacheEntry		*nullTexture;
static unsigned int			timin, timax;				/* texture index min és max */
static unsigned int			texMemFree;					/* szabad memória bájtokban */
static GuTexPalette			palette;					/* aktuális beállított paletta a palettás textúrákhoz */
static GuNccTable			nccTable0;					/* aktuális beállított NCC 0 tábla */
static GuNccTable			nccTable1;					/* aktuális beállított NCC 1 tábla */
static unsigned int			colorKeyPal;
static unsigned int			colorKeyNcc0;
static unsigned int			colorKeyNcc1;
static GuNccTable			*actncctable = &nccTable0;	/* Aktív NCC táblázat */
static unsigned char		*textureMem = NULL;			/* virtuális textúriamemória - csak a tökéletes emulációhoz */
static unsigned char		perfectEmulating = 1;		/* tökéletes emuláció ki-be */
static unsigned char		apMultiTexturing = 0;		/* AP multitexturing engedélyezése (az AP88 és AYIQ8422 textúrákhoz) */
static unsigned int			texMemScale = 0;
static unsigned int			texMemScalePwr = 0;

RendererGeneral::ColorKeyMethod	ckmPreferenceOrder[3];

#ifndef GLIDE3

static GrMipMapInfo			*mipmapinfos;				/* Az egyes utility-textúrák mipmapinfói */

#endif

/*-------------------------------------------------------------------------------------------*/
/* A 3Dfx-es textúraformátumokhoz tartozó dolgok */

_pixfmt			srcPixFmt_P8		= {8, 0,0,0,0, 0,0,0,0};
_pixfmt			srcPixFmt_AP88		= {8, 0,0,0,8, 0,0,0,0};
_pixfmt			srcPixFmt_RGB565	= {16, 5,6,5,0, 11,5,0,0};
_pixfmt			srcPixFmt_ARGB1555	= {16, 5,5,5,1, 10,5,0,15};
_pixfmt			srcPixFmt_ARGB4444	= {16, 4,4,4,4, 8,4,0,12};

/* Tábla, hogy az egyes 3Dfx-es formátumok milyen formátumoknak felelnek meg a MMC-hez */
_pixfmt			*srcPixFmt[] = {&srcPixFmt_P8,		&srcPixFmt_P8,		&srcPixFmt_P8,		&srcPixFmt_P8,
								&srcPixFmt_P8,		&srcPixFmt_P8,		NULL,				NULL,
								&srcPixFmt_AP88,	&srcPixFmt_AP88,	&srcPixFmt_RGB565,	&srcPixFmt_ARGB1555,
								&srcPixFmt_ARGB4444,&srcPixFmt_AP88,	&srcPixFmt_AP88,	NULL};

char			constPaletteIndex[]	= {1, 0, 2, 3, 4, 0, 0, 0, 5, 0, 0, 0, 0, 6, 0, 0};

/*-------------------------------------------------------------------------------------------*/

extern "C" {

NamedFormat		dxTexFormats[NumberOfDXTexFormats];

}

/* Lookup table ahhoz, hogy egy Voodoo-textúraformátumhoz melyik DirectX-formátum felel meg */
/* a legjobban. */
unsigned char	texFmtLookupTable[] = { Pf16_RGB555,	Pf8_P8,			Pf32_ARGB8888,
										Pf32_RGB888,	Pf16_ARGB4444,	Pf8_P8,
										Pf16_RGB555,	Pf16_RGB555,	Pf32_ARGB8888,
										Pf32_ARGB8888,	Pf16_RGB565,	Pf16_ARGB1555,
										Pf16_ARGB4444,	Pf32_ARGB8888,	Pf32_ARGB8888,
										Pf16_RGB555 };

/* Ha az adott pixelformátum nem támogatott, akkor prioritás szerint milyen választások */
/* lehetségesek. */
unsigned char	_8_P8[]			=	{	Pf32_RGB888,	Pf16_RGB565,	Pf16_RGB555,
										Pf32_ARGB8888,	Pf16_ARGB1555,	Pf16_ARGB4444,
										Pf_Invalid };
unsigned char	_16_RGB565[]	=	{	Pf16_RGB555,	Pf32_RGB888,	Pf16_ARGB1555,
										Pf32_ARGB8888,	Pf16_ARGB4444,	Pf_Invalid };
unsigned char	_16_RGB555[]	=	{	Pf16_RGB565,	Pf32_RGB888,	Pf16_ARGB1555,
										Pf32_ARGB8888,	Pf16_ARGB4444,	Pf_Invalid };
unsigned char	_16_ARGB1555[]	=	{	Pf32_ARGB8888,	Pf16_ARGB4444,	Pf16_RGB555,
										Pf16_RGB565,	Pf32_RGB888,	Pf_Invalid };
unsigned char	_16_ARGB4444[]	=	{	Pf32_ARGB8888,	Pf16_ARGB1555,	Pf16_RGB555,
										Pf16_RGB565,	Pf32_RGB888,	Pf_Invalid };
unsigned char	_32_RGB888[]	=	{	Pf16_RGB565,	Pf16_RGB555,	Pf32_ARGB8888,
										Pf16_ARGB1555,	Pf16_ARGB4444,	Pf_Invalid };
unsigned char	_32_ARGB8888[]	=	{	Pf16_ARGB4444,	Pf16_ARGB1555,	Pf32_RGB888,
										Pf16_RGB565,	Pf16_RGB555,	Pf_Invalid };
unsigned char	_AlphaChannel[]	=	{	Pf16_ARGB1555,	Pf16_ARGB4444,	Pf32_ARGB8888,
										Pf_Invalid };

char			texCkCanGoWithAlphaTesting[8] = {0, 2, 0, 2, 1, 0, 1, 1};

char			texCombineFuncImp[] = { TCF_ZERO, TCF_CLOCAL, TCF_ALOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL,
										TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL,
										TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL, TCF_CLOCAL };

unsigned int	lodToSize[]					= { 256, 128, 64, 32, 16, 8, 4, 2, 1 };
unsigned char	aspectRatioScaler[]			= { 3, 2, 1, 0, 1, 2, 3 };
char			formatBYTEPP[]				= { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
char			isTableFormat[]				= { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };
char			hasAlphaComponent[]			= { 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0 };
char			baseRegister[]				= { 0, 1, 2, 3, 3, 3, 3, 3, 3 };
char			hasDXFormatAlphaComponent[]	= { 0, 1, 1, 0, 0, 1, 0 };

/*-------------------------------------------------------------------------------------------*/
/*.................... Az assembly-ben megírt függvények prototípusai .......................*/

extern "C" {

/* ARGB->indexérték texel konverter palettás vagy NCC-textúrák esetén (natív colorkeyinghez) */
unsigned int _stdcall TexGetTexIndexValue(int isNCC, DWORD color, void *table);

/* Adott 3Dfx formátumú textúrabitmaphez legyártja az alpha-textúrát (maszkot) a colorkey alapján */
void * _stdcall TexCreateAlphaMask(void *src, void *dst, unsigned int pitch,							
								   unsigned int byteppsrc, unsigned int byteppdst,
								   unsigned int x, unsigned int y,
								   unsigned int colorkey,
								   unsigned int alphamask);

/* Palettaadatot másol (alpha kinullázva) */
int _stdcall	TexCopyPalette(GuTexPalette *dst, GuTexPalette *src, int size);

/* NCC-táblát másol (az iRGB és qRGB mezõket 9 bites elõjeles egészekként kezelve és átalakítva õket 16 bitesre) */
void _stdcall	TexCopyNCCTable(GuNccTable *dst, GuNccTable *src);

/* két palettát összehasonlít */
int _stdcall	TexComparePalettes(GuTexPalette *src, GuTexPalette *dst);

/* két palettát hasonlít össze egy kihasználtsági térkép alapján */
int _stdcall	TexComparePalettesWithMap(GuTexPalette *src, GuTexPalette *dst, unsigned char *map);

/* két NCC-táblát összehasonlít */
int _stdcall	TexCompareNCCTables(GuNccTable *src, GuNccTable *dst);

/* Egy NCC-táblát konvertál ABGR palettára */
void _stdcall	TexConvertNCCToABGRPalette(GuNccTable *, PALETTEENTRY *);

/* Egy NCC-táblát konvertál ARGB palettára */
void _stdcall	TexConvertNCCToARGBPalette(GuNccTable *, unsigned int *);

/* Egy AP88 formátumú textúrából multitextúrát készít */
void _stdcall	TexConvertToMultiTexture(_pixfmt *dstFormat,
										 void *src, void *dstPal, void *dstAlpha,
										 unsigned int x, unsigned int y,
										 int dstPitchPal, int dstPitchAlpha);

/* ARGB->3Dfxformat konverter (alfa alapú colorkeyinghez) */
unsigned long _stdcall TexGetTexelValueFromARGB(unsigned int ck, GrTextureFormat_t format, GuTexPalette *pal, GuNccTable *ncctable);

void _stdcall GlideGenerateMipMap(GrTextureFormat_t format,void *src, void *dst, unsigned int x, unsigned int y);
FxU32 grTexCalcMemRequiredInternal(GrLOD_t smallLod, GrLOD_t largeLod, GrAspectRatio_t aspect, GrTextureFormat_t format);
FxU32 grTexTextureMemRequiredInternal (FxU32 evenOdd, GrTexInfo *info);

}

/*-------------------------------------------------------------------------------------------*/
/*........................... Belsõ függvények predeklarációja ..............................*/

TexCacheEntry*	TexFindTex(unsigned int startAddress, GrTexInfo *info);
void			TexReloadTexture(TexCacheEntry *tinfo, unsigned int reloadFlags);
void			WTEX(TexCacheEntry *tinfo);


/*-------------------------------------------------------------------------------------------*/
/*.................................... Segédfüggvények ......................................*/

/* Egy LOD-konstansból és egy aspect ratio-ból visszaadja a textúra méreteit */
unsigned int _inline GlideGetXY(GrLOD_t Lod, GrAspectRatio_t ar)	{
unsigned int x;

	//ez a vizsgálat csak Glide2-ben érvényes
	x = GlideGetLOD(Lod);
	return (ar > GR_ASPECT_1x1) ? ((x >> aspectRatioScaler[ar]) << 16) + x : (x << 16) + (x >> (aspectRatioScaler[ar]));
}


/* A formátumból visszaadja, hogy a textúrának van-e átmeneti tárolása (palettás textúrák */
/* és colorkeying) */
static int GlideHasTempStore (GrTextureFormat_t format)	{
int	i;

	i = GetCKMethodValue(config.Flags);
	if ( (i == CFG_CKMAUTOMATIC) || (i == CFG_CKMALPHABASED)) return(1);
	switch(format)	{
		case GR_TEXFMT_P_8:
		case GR_TEXFMT_AP_88:
		case GR_TEXFMT_YIQ_422:
		case GR_TEXFMT_AYIQ_8422:
			return(1);
		default:
			return(0);
	}
}


static int AllocTextureCachePool ()	{
unsigned int	i,j;
TexCacheEntry	*tinfo;
		
	DEBUG_BEGINSCOPE("AllocTextureCachePool",DebugRenderThreadId);

	for(i = 0; i < TEXPOOL_NUM; i++)	{
		if (texInfo[i] == NULL)	{
			if ((texInfo[i] = (TexCacheEntry *) _GetMem((MAX_TEXNUM / TEXPOOL_NUM) * sizeof(TexCacheEntry))) == NULL)	{
				
				DEBUGLOG(0,"\n   Error: AllocTextureCachePool: Allocating a texture cache pool: FAILED, cannot allocate memory");
				DEBUGLOG(1,"\n   Hiba: AllocTextureCachePool: Textúra-cache terület allokálása: HIBA, nem tudok memóriát foglalni");
				
				return(0);
			}
			tinfo = texInfo[i];
			tinfo[(MAX_TEXNUM/TEXPOOL_NUM)-1].next = NULL;
			ethead = tinfo;
			for(j = 0; j < (MAX_TEXNUM/TEXPOOL_NUM)-1; j++)	{
				tinfo->pool = i;
				tinfo->next = tinfo+1;
				tinfo++;
			}
			return(1);
		}
	}

	DEBUGLOG(0,"\n   Error: AllocTextureCachePool: Allocating a texture cache pool: FAILED, too many cached textures");
	DEBUGLOG(1,"\n   Hiba: AllocTextureCachePool: Textúra-cache terület allokálása: HIBA, túl sok tárolt (cache-elt) textúra");
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(0);
}


void GlideReleaseTexture(TexCacheEntry *tinfo)	{
unsigned int	i;
	
	DEBUG_BEGINSCOPE("GlideReleaseTexture", DebugRenderThreadId);
	
	if (tinfo == astate.acttex) {
		DrawFlushPrimitives();
		astate.acttex = NULL;
	}

	/* Az esetleges bennragadt textúra handle-k kiszedése */
	if (astate.lpDDTex[0] == tinfo->texBase[0])
		rendererTexturing->SetTextureAtStage (0, astate.lpDDTex[0] = NULL);

	if (astate.lpDDTex[1] == tinfo->texAP88AlphaChannel[0])
		rendererTexturing->SetTextureAtStage (1, astate.lpDDTex[1] = NULL);

	for(i = 1; i < MAX_STAGETEXTURES; i++)	{
		if (astate.lpDDTex[i] == tinfo->texCkAlphaMask[0])
			rendererTexturing->SetTextureAtStage (i, astate.lpDDTex[i] = NULL);
	}
	
	/* Normál textúra */
	if ( (tinfo->texBase[0]) != NULL)	{
		rendererTexturing->DestroyDXTexture (tinfo->texBase[0]);
	}
	/* Az esetlegesen multitextúrázott AP textúra alpha-csatornája */
	if ( (tinfo->texAP88AlphaChannel[0]) != NULL)
		rendererTexturing->DestroyDXTexture (tinfo->texAP88AlphaChannel[0]);
	
	/* Az esetleges árnyék-textúra */
	if ( (tinfo->texCkAlphaMask[0]) != NULL)
		rendererTexturing->DestroyDXTexture (tinfo->texCkAlphaMask[0]);

	if ( (tinfo->texAuxPTextures[0]) != NULL)
		rendererTexturing->DestroyDXTexture (tinfo->texAuxPTextures[0]);

	if (tinfo->palettedData != NULL) if (tinfo->palettedData->lpDDPal)
		rendererTexturing->DestroyPalette (tinfo->palettedData->lpDDPal);

	for(i = 0; i < 9; i++)
		tinfo->texBase[i] = tinfo->texCkAlphaMask[i] = tinfo->texAP88AlphaChannel[i] = tinfo->texAuxPTextures[i] = NULL;
	
	if (tinfo->palettedData) _FreeMem(tinfo->palettedData);
	tinfo->palettedData = NULL;
	texNumInPool[tinfo->pool]--;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


static int GlideCheckTexIntersect(FxU32 startAddress, GrLOD_t smallLod, GrLOD_t largeLod, GrAspectRatio_t aspectRatio,
								  GrTextureFormat_t format)
{
FxU32	start, end;
FxU32	downloadstart, downloadend;
int		from, to, i, nointersect;
GrLOD_t	slod, llod;

	
	if (!astate.multiBaseTexturing)	{
		from = to = 0;
		slod = (int) astate.smallLod;
		llod = (int) astate.largeLod;
	} else {
		from = (int) astate.largeLod;
		to = ((GrLOD_t) astate.smallLod > GR_LOD_32) ? GR_LOD_32 : (int) astate.smallLod;
		slod = llod = (int) astate.largeLod;
	}
	
	for(i = from, nointersect = 1; i <= to; i++)	{
		if (astate.startAddress[i] != 0xEEEEEEEE)	{
			start = astate.startAddress[i];
			end = start + grTexCalcMemRequiredInternal (slod, llod, (GrAspectRatio_t) astate.aspectRatio, (GrTextureFormat_t) astate.format);
			downloadstart = startAddress;
			downloadend = downloadstart + grTexCalcMemRequiredInternal (smallLod, largeLod, aspectRatio, format);
			nointersect = nointersect && (((downloadstart < start) && (downloadend < start)) || ((downloadstart >= end) && (downloadend >= end)));
		}
		llod++;
		slod++;
		if (slod == GR_LOD_32) slod = smallLod;
	}
	
	return(!nointersect);
}


#ifdef GLIDE3

int _inline GlideCompareInfos(GrTexInfo *info1, GrTexInfo *info2)	{
	return( (info1->smallLodLog2 == info2->smallLodLog2) &&
			(info1->largeLodLog2 == info2->largeLodLog2) &&
			(info1->aspectRatioLog2 == info2->aspectRatioLog2) &&
			(info1->format == info2->format) );
}

#else

int _inline GlideCompareInfos(GrTexInfo *info1, GrTexInfo *info2)	{
	return( (info1->smallLod == info2->smallLod) &&
			(info1->largeLod == info2->largeLod) &&
			(info1->aspectRatio == info2->aspectRatio) &&
			(info1->format == info2->format) );
}

#endif


/* A voodoo-videómemóriabeli kezdôcímbôl és a kívánt textúratulajdonságokból */
/* visszaadja a textúra blokkjának címét. Ha azon a címen nincs cache-elt textúra, */
/* vagy nem a megadott tulajdonságokkal rendelkezik, akkor a visszatérési érték NULL. */
/* Megjegyz: a kívánt tulajdonságokat állíthatjuk NULL-ra is, ekkor ezzel nem foglalkozik. */
TexCacheEntry *TexFindTex(unsigned int startAddress, GrTexInfo *info)
{
	
	DEBUG_BEGINSCOPE("TexFindTex", DebugRenderThreadId);

	unsigned int len, left, index;

	if ((len = timax - (left = index = timin)) == 0)
		return(NULL);
	
	while(len != 0)	{
		index = left + (len >> 1);
		if (texindex[index]->startAddress == startAddress)
			break;
		if (texindex[index]->startAddress < startAddress) {
			len = (len != 1) ? (len >> 1) + (len & 1) : 0;
			left = index;
		} else {
			len >>= 1;
		}
	}
	
	TexCacheEntry* tinfo;
	if ( (tinfo = texindex[index])->startAddress == startAddress)	{
		if ((!perfectEmulating) || (info == NULL))
			return (tinfo);
		else
			return (GlideCompareInfos(&tinfo->info, info) ? tinfo : NULL);
	}
		
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(NULL);
}


/* Allokál egy bejegyzést egy új textúrához a megadott kezdõcímre, majd a megadott jellemzõket (info) */
/* el is tárolja ezen bejegyzésben. Ha nem tud új bejegyzést allokálni, akkor a visszaadott cím NULL. */
TexCacheEntry*	TexAllocCacheEntry(FxU32 startAddress, GrTexInfo *info, TexCacheEntry *ontinfo, int onelevel)
{
unsigned int	index, len, left, i;
FxU32			endAddress;
TexCacheEntry	*tinfo;
	
	DEBUG_BEGINSCOPE("TexAllocCacheEntry", DebugRenderThreadId);

	index = 0;

#ifdef GLIDE3
	endAddress = startAddress + ((onelevel) ? grTexCalcMemRequiredInternal(info->largeLodLog2, info->largeLodLog2, info->aspectRatioLog2, info->format) :
								grTexTextureMemRequiredInternal(GR_MIPMAPLEVELMASK_BOTH, info));
#else
	endAddress = startAddress + ((onelevel) ? grTexCalcMemRequiredInternal(info->largeLod, info->largeLod, info->aspectRatio, info->format) :
								grTexTextureMemRequiredInternal(GR_MIPMAPLEVELMASK_BOTH, info));
#endif

	if ( (len = timax - (left = timin)) != 0 )	{
		while(len != 0)	{
			index = left + (len >> 1);
			if (texindex[index]->startAddress == startAddress)
				break;
			if (texindex[index]->startAddress < startAddress) {
				len = (len != 1) ? (len >> 1)+(len & 1) : 0;
				left = index;
			} else {
				len >>= 1;
			}
		}		
		/* index= max(i) (texInfo[i].endAddress<=startAddress) */
		if (texindex[index]->endAddress <= startAddress)
			index++;
		
		/* összeszámoljuk, hogy mennyit kell törölni, és ezeket töröljük is */
		len = 0;
		left = index;
		while( (left < timax) )	{
			if ((tinfo = texindex[left])->startAddress < endAddress)	{
				GlideReleaseTexture(tinfo);
				tinfo->next = ethead;
				ethead = tinfo;
				left++;
				len++;				
			} else
				break;
		}
		/* Ha törlés után nem kell mozgatni */
		if (left == timax) {
			timax = index + 1;
		} else {
			/* bejegyzés beszúrása */
			if (len == 0)	{
				if ( (timax - timin + 1) == MAX_TEXNUM) return(NULL);
				i = timax;
				len = timax - left;
				left = timax - 1;
				timax++;
				for(; len>0; len--) texindex[i--] = texindex[left--];
			}
			/* len-1 bejegyzés törlése */
			if (len > 1)	{
				len = timax - left;
				timax -= left - index - 1;
				i = index + 1;
				for(; len>0; len--) texindex[i++] = texindex[left++];
			}
		}
	} else {
		timax = 1;
	}
	if (ontinfo == NULL)	{
		/* Ha az összes idáig allokált pool megtelt */
		if (ethead == NULL)	{
			/* Allokálunk még egy poolt, ha nem sikerül, akkor ezt a bejegyzést töröljük az */
			/* indextáblából, és hiba */
			if (!AllocTextureCachePool())	{
				len = timax-index-1;
				i = index;
				for(i = 0; i < len; i++)
					texindex[index+i] = texindex[index+i+1];
				
				timax--;
				return(NULL);
			}
		}
		texindex[index] = tinfo = ethead;
		ethead = tinfo->next;
		tinfo->parentMipMap = NULL;
		texNumInPool[tinfo->pool]++;
	} else {
		texindex[index] = tinfo = ontinfo;
	}
	tinfo->info = *info;
	tinfo->startAddress = startAddress;
	tinfo->endAddress = endAddress;
	if (tinfo == astate.acttex)
		astate.acttex = NULL;
	
	if (onelevel)
		tinfo->flags |= TF_STORESONELEVEL;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(tinfo);
}


static void		TexInvalidateCache (FxU32 startAddress, GrTexInfo *info)	{
unsigned int	index, len, left;
FxU32			endAddress;
TexCacheEntry	*tinfo;
	
	index = 0;
	endAddress = startAddress + grTexTextureMemRequiredInternal (GR_MIPMAPLEVELMASK_BOTH, info);

	if ( (len = timax - (left = timin)) != 0 )	{
		while(len != 0)	{
			index = left + (len >> 1);
			if (texindex[index]->startAddress == startAddress) break;
			if (texindex[index]->startAddress > startAddress) len >>= 1;
			else {
				len = (len != 1) ? (len >> 1)+(len & 1) : 0;
				left = index;
			}
		}		
		/* index= max(i)=(texInfo[i].startAddress<=startAddress) */
		if (texindex[index]->endAddress <= startAddress) index++;
		/* összeszámoljuk, hogy mennyit kell törölni, és ezeket töröljük is */
		len = 0;
		left = index;
		while( (left < timax) )	{
			if ((tinfo = texindex[left])->startAddress < endAddress)	{				
				GlideReleaseTexture(tinfo);
				tinfo->next = ethead;
				ethead = tinfo;
				left++;
				len++;				
			} else break;
		}
		/* Ha törlés után nem kell mozgatni */
		if ( left == timax ) timax = index;
		else {
			/* len bejegyzés törlése */
			if (len > 0)	{
				len = timax - left;
				timax -= left - index;
				for(; len > 0; len--) texindex[index++] = texindex[left++];
			}
		}
	}
}


void	TexReleaseAllCachedTextures ()
{
	while (timax) {
		TexInvalidateCache (texindex[0]->startAddress, &texindex[0]->info);
	}
}


void	TexCreateSpecialTextures ()
{
unsigned char		textureData[8*8];
GrTexInfo info =	{GR_LOD_8, GR_LOD_8, GR_ASPECT_1x1, GR_TEXFMT_8BIT, textureData};

	grTexDownloadMipMap(0, TEXTURE_C0A0, GR_MIPMAPLEVELMASK_BOTH, &info);
	TexReloadTexture(nullTexture = TexFindTex(TEXTURE_C0A0, NULL), RF_TEXTURE);
}


enum DXTextureFormats	TexGetMatchingDXFormat (GrTextureFormat_t glideTexFormat)
{
enum DXTextureFormats	namedFormat;
unsigned char*			tableOfNamedFormats;

	namedFormat = (enum DXTextureFormats) texFmtLookupTable[glideTexFormat];
	tableOfNamedFormats = dxTexFormats[namedFormat].missing;
	while(namedFormat != Pf_Invalid)	{
		if (dxTexFormats[namedFormat].isValid)
			break;
		namedFormat = (enum DXTextureFormats) *(tableOfNamedFormats++);
	}
	return namedFormat;
}


unsigned int	TexGetFormatConstPaletteIndex (GrTextureFormat_t glideTexFormat)
{
	return constPaletteIndex[glideTexFormat];
}


_pixfmt*		TexGetFormatPixFmt (GrTextureFormat_t glideTexFormat)
{
	return srcPixFmt[glideTexFormat];
}


_pixfmt*		TexGetDXFormatPixelFormat (enum DXTextureFormats format)
{
	return &dxTexFormats[format].pixelFormat;
}


unsigned int*	TexGetDXFormatConstPalette (enum DXTextureFormats format, unsigned int paletteIndex)
{
	return paletteIndex ? dxTexFormats[format].constPalettes + (paletteIndex - 1) * 256 : NULL;
}


/* Az adott bejegyzésre a paraméterek alapján fizikailag létrehozza mipmap-et. */
/* Másodlagos textúra estén csak a tempstore-t hozza létre szükség szerint. */
/* Return: 0 - nem sikerült, 1 - sikerült */
unsigned int GlideCreateTexture(TexCacheEntry *tinfo)	{
unsigned int	len, x, y;
unsigned int	llod, slod;
unsigned int	mipMapNum;
enum DXTextureFormats	namedFormat;
PALETTEENTRY	palEntries[256];
PaletteType		lpDDPal;
	
	DEBUG_BEGINSCOPE("GlideCreateTexture", DebugRenderThreadId);

	DXTextureFormats auxPTexFormat = Pf_Invalid;
	/* Secondary bejegyzésekre nem hozunk létre fizikai textúrákat, csak a paletteData-t allokáljuk szükség szerint. */
	if (!(tinfo->flags & TF_SECONDARY))	{
		/* AP multitextúrázás esetén az alpha réteg formátuma ugyanaz, mint */
		/* ami normál esetben a teljes textúráé. */

#ifdef GLIDE3
		len = GlideGetXY(tinfo->info.largeLodLog2, tinfo->info.aspectRatioLog2);
#else
		len = GlideGetXY(tinfo->info.largeLod, tinfo->info.aspectRatio);
#endif
		x = tinfo->x = len >> 16;
		y = tinfo->y = len & 0xFFFF;

		if ((namedFormat = TexGetMatchingDXFormat (tinfo->info.format)) == Pf_Invalid)
			return (0);

		tinfo->dxFormat = dxTexFormats + namedFormat;
	
		mipMapNum = 1;

		//Fizikai mipmapek száma
#ifdef GLIDE3
		if ( ( (slod = GlideGetLOD((config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ? GR_LOD_1 : tinfo->info.smallLodLog2)) !=
			(llod = GlideGetLOD(tinfo->info.largeLodLog2)) ) && (!(config.Flags & CFG_DISABLEMIPMAPPING)) )	{
#else
		if ( ( (slod = GlideGetLOD((config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ? GR_LOD_1 : tinfo->info.smallLod)) !=
			(llod = GlideGetLOD(tinfo->info.largeLod)) ) && (!(config.Flags & CFG_DISABLEMIPMAPPING)) )	{
#endif
			while( (llod != slod) && (x != 1) && (y != 1) ) {			
				mipMapNum++;
				llod >>= 1;
				x >>= 1;
				y >>= 1;			
			}
		}

		//Logikai mipmapek száma
#ifdef GLIDE3
		tinfo->mipMapCount = (unsigned short) (tinfo->info.largeLodLog2 - tinfo->info.smallLodLog2 + 1);
#else
		tinfo->mipMapCount = (unsigned short) (tinfo->info.smallLod - tinfo->info.largeLod + 1);
#endif


		/* */
		//DXTextureFormats auxPTexFormat = Pf_Invalid;

		if (namedFormat != Pf8_P8) {
			switch (tinfo->info.format) {
				case GR_TEXFMT_AP_88:
				case GR_TEXFMT_AYIQ_8422:
					if (dxTexFormats[Pf16_AuxP8A8].isValid) {
						auxPTexFormat = Pf16_AuxP8A8;
						break;
					}						// no break

				case GR_TEXFMT_P_8:
				case GR_TEXFMT_YIQ_422:
					if (dxTexFormats[Pf8_AuxP8].isValid) {
						auxPTexFormat = Pf8_AuxP8;
					}
					break;

				default:
					break;
			}
		}

		if (auxPTexFormat != Pf_Invalid) {
			if (!rendererTexturing->CreateDXTexture (auxPTexFormat, tinfo->x, tinfo->y, mipMapNum, tinfo->texAuxPTextures, 0)) {
				auxPTexFormat = Pf_Invalid;
				DEBUGLOG (0, "\n   Error: GlideCreateTexture: Cannot create aux palette texture, no hw accelerated regenerating!");
				DEBUGLOG (1, "\n   Hiba: TexUpdateAlphaBasedColorKeyState: Nem tudom létrehozni a segéd-palettatextúrát, nincs hw-es regenerálás!");
			}
		}

		tinfo->auxPFormat = (auxPTexFormat != Pf_Invalid) ? (dxTexFormats + auxPTexFormat) : NULL;
		
		/* */

		if (!rendererTexturing->CreateDXTexture (namedFormat, tinfo->x, tinfo->y, mipMapNum, tinfo->texBase,
												 auxPTexFormat != Pf_Invalid)) {
			if (auxPTexFormat != Pf_Invalid) {
				rendererTexturing->DestroyDXTexture (tinfo->texAuxPTextures[0]);
				ZeroMemory (tinfo->texAuxPTextures, sizeof (tinfo->texAuxPTextures));
			}
			return (0);
		}
		
		/* Fekete natív colorkey beállitása (avagy 0-s index a P8 formátum esetén) */
		tinfo->colorKeyNative = 0;
		tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
		rendererTexturing->SetSrcColorKeyForTexture (tinfo->texBase[0], (unsigned int) 0);

		for(llod = len; llod < 9; llod++) tinfo->texBase[llod] = NULL; ///len ?????
	}

	tinfo->palettedData = NULL;
	len = x = 0;
	if (GlideHasTempStore(tinfo->info.format))
		len = sizeof(struct PalettedData);
	
	if ( len && (!perfectEmulating) )	{

#ifdef GLIDE3
		len += grTexCalcMemRequiredInternal(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLodLog2 : tinfo->info.smallLodLog2,
											tinfo->info.largeLodLog2, tinfo->info.aspectRatioLog2, tinfo->info.format);
#else
		len += grTexCalcMemRequiredInternal(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLod : tinfo->info.smallLod,
											tinfo->info.largeLod, tinfo->info.aspectRatio, tinfo->info.format);
#endif
		x = TF_STOREDMIPMAP;
	}
	if ( (config.Flags & CFG_STOREMIPMAP) && (!perfectEmulating) )	{

#ifdef GLIDE3
		len = grTexCalcMemRequiredInternal(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLodLog2 : tinfo->info.smallLodLog2,
										   tinfo->info.largeLodLog2, tinfo->info.aspectRatioLog2, tinfo->info.format) + sizeof(struct PalettedData);
#else
		len = grTexCalcMemRequiredInternal(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLod : tinfo->info.smallLod,
										   tinfo->info.largeLod, tinfo->info.aspectRatio, tinfo->info.format) + sizeof(struct PalettedData);
#endif
		x = TF_STOREDMIPMAP;
	}
	if (tinfo->startAddress >= RESERVED_ADDRESSSPACE) len = x = 0;
	
	if (len) {
		if ( (tinfo->palettedData = (PalettedData *) _GetMem(len)) != NULL)	{
			ZeroMemory(tinfo->palettedData, sizeof(struct PalettedData));
			tinfo->flags |= x;
			
			/* Érvénytelen paletta és ncc-tábla beállítása a kihasználtsági térképen */
			tinfo->palettedData->palette.data[0] = 0xFFFFFFFF;
			tinfo->palettedData->ncctable.iRGB[0][0] = (short) 0x8000;
			tinfo->palettedData->palmap[0] = 1;
		}
		DEBUGCHECK(0,(tinfo->palettedData == NULL),"\n   Error: GlideCreateTexture: Couldn't allocate additional memory needed for a particular texture. If it's a paletted texture, palette will be corrupted!");
		DEBUGCHECK(1,(tinfo->palettedData == NULL),"\n   Hiba: GlideCreateTexture: Nem tudtam járulékos memóriát foglalni egy textúrának. Ha ez egy palettás textúra, a paletta helytelen adatokat fog tartalmazni!");
	}
	if (tinfo->flags & TF_SECONDARY) return(1);

	/* AP multitexturing esetén ha AP88 vagy AYIQ8422 textúráról van szó, */
	/* és az eredetileg választott formátumnak van alpha komponense, akkor a tényleges palettás textúra létrehozása */
	/* Ha ezt nem sikerül létrehozni, akkor a textúrát hagyományos módon kezeljük, */
	/* és a mûvelet sikeresnek minõsül!! */
	for(llod = 0; llod < 9; llod++)
		tinfo->texAP88AlphaChannel[llod] = tinfo->texCkAlphaMask[llod] = NULL;
	
	if (((tinfo->info.format == GR_TEXFMT_AP_88) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422)) &&
		tinfo->dxFormat->pixelFormat.ABitCount != 0)	{

		if (/*apMultiTexturing*/namedFormat == Pf8_P8) {
		
			for(llod = 0; llod < 9; llod++) tinfo->texAP88AlphaChannel[llod] = tinfo->texBase[llod];
		
			//DXTextureFormats pTexFormat = (namedFormat == Pf8_P8) ? Pf8_P8 : namedFormat;

			if (!rendererTexturing->CreateDXTexture (Pf8_P8, tinfo->x, tinfo->y, mipMapNum, tinfo->texBase, 0)) {

				for(llod = 0; llod < 9; llod++)
					tinfo->texAP88AlphaChannel[llod] = NULL;
				
				DEBUGLOG(0,"\n   Warning: GlideCreateTexture: AP multitexturing: creating paletted layer has failed");
				DEBUGLOG(1,"\n   Figyelmeztetés: GlideCreateTexture: AP multitexturing: a palettás réteg létrehozása nem sikerült");
				
				LOG(1,"    AP multitexturing: creating paletted layer has failed");
				
				return(1);
			}
		} else if (auxPTexFormat == Pf8_AuxP8) {
			if (!rendererTexturing->CreateDXTexture (namedFormat, tinfo->x, tinfo->y, mipMapNum, tinfo->texAP88AlphaChannel, 0)) {
				rendererTexturing->DestroyDXTexture (tinfo->texBase[0]);
				rendererTexturing->DestroyDXTexture (tinfo->texAuxPTextures[0]);
				ZeroMemory (tinfo->texBase, sizeof (tinfo->texBase));
				ZeroMemory (tinfo->texBase, sizeof (tinfo->texAuxPTextures));

				return (0);
			}
		}
	}
	/* Fizikai paletta létrehozása palettás textúrához vagy palettás multitextúrához */
	if ((tinfo->dxFormat->pixelFormat.BitPerPixel == 8)	|| (tinfo->texAP88AlphaChannel[0] != NULL)) {
		
		lpDDPal = NULL;
		if (tinfo->palettedData != NULL)
			lpDDPal = tinfo->palettedData->lpDDPal = rendererTexturing->CreatePalette (palEntries);
		
		rendererTexturing->AssignPaletteToTexture (tinfo->texBase[0], lpDDPal);
		
		DEBUGCHECK(0,(tinfo->palettedData->lpDDPal == NULL),"\n   Error: GlideCreateTexture: Couldn't create palette object for a paletted texture. Using corrupted palette or causing exception during rendering!");
		DEBUGCHECK(1,(tinfo->palettedData->lpDDPal == NULL),"\n   Hiba: GlideCreateTexture: Nem tudtam létrehozni a palettaobjektumot egy palettás textúrához. Helytelen paletta használata vagy kivétel keletkezése várható a megjelenítés közben!");
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


unsigned int GlideCreateAlphaTexture(TexCacheEntry *tinfo)	{
unsigned int	i;

	DEBUG_BEGINSCOPE("GlideCreateAlphaTexture", DebugRenderThreadId);
	
	for(i = 0; i < 9; i++) tinfo->texCkAlphaMask[i] = NULL;

	i = 0;
	while(_AlphaChannel[i] != Pf_Invalid)	{
		if ( dxTexFormats[_AlphaChannel[i]].isValid) break;
		i++;
	}
	if ( (i = _AlphaChannel[i]) == Pf_Invalid) return(0);

	tinfo->alphaDxFormat = dxTexFormats + i;

	if (!rendererTexturing->CreateDXTexture ((enum DXTextureFormats)i, tinfo->x, tinfo->y, tinfo->mipMapCount, tinfo->texCkAlphaMask, 0)) {

		DEBUGLOG(0,"\n   Warning:  GlideCreateAlphaTexture: creating dd alpha texture surface failed");
		DEBUGLOG(1,"\n   Figyelmeztetés: GlideCreateAlphaTexture: a dd alfa-textúra létrehozása nem sikerült");
		LOG(1,"    GlideCreateAlphaTexture failed");
		return(0);
	}

	for(i = tinfo->mipMapCount; i < 9;i++) tinfo->texCkAlphaMask[i] = NULL;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


void GlideTexSetTexturesAsLost ()
{
unsigned int i;

	for(i = timin; i < timax; i++) {
		texindex[i]->flags |= TF_CONSIDEREDASLOST;
		texindex[i]->colorKeyAlphaBased = INVALID_COLORKEY;
	}
}


/* Egy adott textúra adott szintjére textúraadatot tölt. */
/* tinfo: textúra bejegyzése, src: textúraadat, level: mipmap-szint (0-9), */
/* ystart,yend: kezdõ- és végsor (y), */
/* palmapcreate: 1 - palettás textúra kihasználtsági térképét frissítse, 0 - ne frissítse */
void* _GlideLoadTextureMipMap (TexCacheEntry *tinfo, void *src, int level, int ystart, int yend, int palmapcreate)
{
TextureType				lpDDTexture, lpDDAlphaTexture;
GuTexPalette			nccPalette;
unsigned char			*palMap;
unsigned int			*palette;
int						lockerror;
unsigned int			x,y;
int						i, palType;
void*					texPtr;
void*					texAlphaPtr;
unsigned int			texPitch, texAlphaPitch;

	DEBUG_BEGINSCOPE("_GlideLoadTextureMipMap", DebugRenderThreadId);

	/* Ha valamelyik dimenzió nulla lesz, akkor ez a mipmapszint elvileg nem létezhet (NULL) */
	x = tinfo->x >> level;
	y = tinfo->y >> level;
	//if (x==0 || y==0) _asm mov eax,ds:[0]
	if (ystart > yend) {
		ystart = 0;
		yend = y - 1;
	}
	
	i = constPaletteIndex[tinfo->info.format];
	palette = i ? tinfo->dxFormat->constPalettes + (i - 1) * 256 : NULL;
	palType = i ? PALETTETYPE_CONVERTED : PALETTETYPE_UNCONVERTED;

	if (GlideIsTableFormat(tinfo->info.format)) palette = (unsigned int *) &tinfo->palettedData->palette.data;

	if ( (tinfo->info.format == GR_TEXFMT_YIQ_422) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422) )	{
		TexConvertNCCToARGBPalette (&tinfo->palettedData->ncctable, (unsigned int*) &nccPalette.data);
		palette = (unsigned int *) &nccPalette;
	}

	//DEBUGLOG(2,"\nWriting text mipmap with format: %s",tform[tinfo->info.format]);
	
	palMap = ( (tinfo->palettedData) && palmapcreate) ? tinfo->palettedData->palmap : NULL;
	
	_pixfmt* dxFormat = &tinfo->dxFormat->pixelFormat;
	unsigned int bitPP = dxFormat->BitPerPixel;
	if (tinfo->texAuxPTextures[0] == NULL) {
		lpDDTexture = tinfo->texBase[level];
	} else {
		lpDDTexture = tinfo->texAuxPTextures[level];
		dxFormat = &tinfo->auxPFormat->pixelFormat;
		bitPP =	(dxFormat->ABitCount == 8) ? 16 : 8;
	}
	
	if ( (src != NULL) && (lpDDTexture != NULL) )	{
		if ( (tinfo->flags & TF_STOREDMIPMAP) && (!((tinfo->flags & TF_STORESONELEVEL) && level)) && (level < (int) tinfo->mipMapCount))	{

#ifdef GLIDE3
			CopyMemory(tinfo->palettedData->data +
				grTexCalcMemRequiredInternal(tinfo->info.largeLodLog2 - level, tinfo->info.largeLodLog2, tinfo->info.aspectRatioLog2, tinfo->info.format) -
				grTexCalcMemRequiredInternal(tinfo->info.largeLodLog2 - level, tinfo->info.largeLodLog2 - level, tinfo->info.aspectRatioLog2, tinfo->info.format) +
				ystart*x*GlideGetTexBYTEPP(tinfo->info.format), src, (yend-ystart+1)*x*GlideGetTexBYTEPP(tinfo->info.format));
#else
			CopyMemory(tinfo->palettedData->data +
				grTexCalcMemRequiredInternal(tinfo->info.largeLod + level, tinfo->info.largeLod, tinfo->info.aspectRatio, tinfo->info.format) -
				grTexCalcMemRequiredInternal(tinfo->info.largeLod + level, tinfo->info.largeLod + level, tinfo->info.aspectRatio, tinfo->info.format) +
				ystart*x*GlideGetTexBYTEPP(tinfo->info.format), src, (yend-ystart+1)*x*GlideGetTexBYTEPP(tinfo->info.format));
#endif
		}

		lockerror = 0;
		if ((texPtr = rendererTexturing->GetPtrToTexture (lpDDTexture, level, &texPitch)) == NULL)
			lockerror = 1;
		
		if ((lpDDAlphaTexture = tinfo->texAP88AlphaChannel[level]) != NULL) {
			if ((texAlphaPtr = rendererTexturing->GetPtrToTexture (lpDDAlphaTexture, level, &texAlphaPitch)) == NULL)
				lockerror |= 2;
		}

		if (!lockerror)	{
			if (lpDDAlphaTexture == NULL)
				MMCConvertAndTransferData(srcPixFmt[tinfo->info.format], dxFormat,
										  0x0, 0x0, 0xFF,
										  src, ((char *) texPtr)+x*ystart*(bitPP>>3),
										  x, yend - ystart + 1,
										  x * GlideGetTexBYTEPP(tinfo->info.format), texPitch,
										  palette, palMap,
										  MMCIsPixfmtTheSame(srcPixFmt[tinfo->info.format], dxFormat) ?
										  	CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
										  palType);
			else
				TexConvertToMultiTexture(&tinfo->dxFormat->pixelFormat,
										 src, ((char *) texPtr)+x*ystart,
										 ((char *) texAlphaPtr)+x*ystart*tinfo->dxFormat->pixelFormat.BitPerPixel/8,
										 x, yend-ystart+1,
										 texPitch, texAlphaPitch);
		}
		if (texPtr != NULL)
			rendererTexturing->ReleasePtrToTexture (lpDDTexture, level);
		if ((lpDDAlphaTexture != NULL) && (texAlphaPtr != NULL))
			rendererTexturing->ReleasePtrToTexture (lpDDAlphaTexture, level);
		
		src = ((char *)src) + (x * (yend - ystart + 1) * GlideGetTexBYTEPP(tinfo->info.format));
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(src);
}


void* GlideLoadTextureMipMap (TexCacheEntry *tinfo, void *src, int level, int ystart, int yend, int palmapcreate)
{
unsigned char	*from, *to;
unsigned int	x, y;

	DEBUG_BEGINSCOPE("GlideLoadTextureMipMap", DebugRenderThreadId);

	x = tinfo->x >> level;
	y = tinfo->y >> level;
	if (ystart > yend) {
		ystart = 0;
		yend = y - 1;
	}
	from = (unsigned char *) src;
	to = mipMapTempArea;
	src = _GlideLoadTextureMipMap(tinfo, src, level, ystart, yend, palmapcreate);

#ifdef GLIDE3
	if ( (tinfo->info.largeLodLog2 == tinfo->info.smallLodLog2) && (config.Flags2 & CFG_AUTOGENERATEMIPMAPS) &&
		(ystart == 0) && (yend == ((int) y - 1) ) )
#else
	if ( (tinfo->info.largeLod == tinfo->info.smallLod) && (config.Flags2 & CFG_AUTOGENERATEMIPMAPS) &&
		(ystart == 0) && (yend == ((int) y - 1) ) )
#endif
		while( (x >= 2) && (y >= 2) )	{
			GlideGenerateMipMap(tinfo->info.format, from, to, x, y);
			_GlideLoadTextureMipMap(tinfo, to, ++level, 1, 0, palmapcreate);
			/* pointerek cseréje */
			if (to == mipMapTempArea)	{
				from = mipMapTempArea;
				to = mipMapTempArea + 128*128*2;
			} else {
				from = mipMapTempArea + 128*128*2;
				to = mipMapTempArea;
			}
			x >>= 1;
			y >>= 1;
		}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(src);
}


/* újratölti egy tetszôleges textúra bitképét, ha tudja */
void TexReloadTexture (TexCacheEntry *tinfo, unsigned int reloadFlags)
{
unsigned int	i;
void			*src;
unsigned char	textureData[8*8];
PALETTEENTRY	palEntries[256];

	DEBUG_BEGINSCOPE("TexReloadTexture", DebugRenderThreadId);
	
	if (tinfo == NULL) return;
	src = (perfectEmulating) ? src = textureMem + tinfo->startAddress : ((tinfo->flags & TF_STOREDMIPMAP) ? &tinfo->palettedData->data : NULL );
	if (tinfo->startAddress >= RESERVED_ADDRESSSPACE)	{
		src = textureData;
		switch(tinfo->startAddress)	{
			case TEXTURE_C0A0:
				ZeroMemory(textureData, 64);
				break;
			default:
				break;
		}
	}
	if ((reloadFlags & RF_TEXTURE) && (src != NULL)) {
		bool textureReloadCanGo = true;
		
		/* Nem valódi palettás textúrát érvénytelen palettával vagy ncc-táblával nem tölt újra */
		if ((tinfo->dxFormat->pixelFormat.BitPerPixel != 8)	&&
			(tinfo->texAP88AlphaChannel[0] == NULL) && (tinfo->texAuxPTextures[0] == NULL) &&
			(tinfo->palettedData != NULL) ) {

			switch(tinfo->info.format)	{
				case GR_TEXFMT_P_8:
				case GR_TEXFMT_AP_88:
					if (tinfo->palettedData->palette.data[0] == 0xFFFFFFFF) {
						textureReloadCanGo = false;
					}
					break;

				case GR_TEXFMT_YIQ_422:
				case GR_TEXFMT_AYIQ_8422:
					if (tinfo->palettedData->ncctable.iRGB[0][0] == ((short) 0x8000)) {
						textureReloadCanGo = false;
					}
					break;
			}
		}
		if (textureReloadCanGo) {
			for(i = 0; i < tinfo->mipMapCount; i++) {
				src = GlideLoadTextureMipMap(tinfo, src, i, 1, 0, !(reloadFlags & RF_NOPALETTEMAPCREATE));
			}
		}
	}

	if (reloadFlags & RF_PALETTE) {
		switch(tinfo->info.format)	{
			case GR_TEXFMT_P_8:
			case GR_TEXFMT_AP_88:
				if (tinfo->palettedData != NULL) {
					if (tinfo->auxPFormat == NULL) {
						MMCConvARGBPaletteToABGR ((unsigned int *) &tinfo->palettedData->palette.data, (unsigned int *) palEntries);
						if (tinfo->palettedData->lpDDPal)
							rendererTexturing->SetPaletteEntries (tinfo->palettedData->lpDDPal, palEntries);
					} else {
						//renderer auxconverter
						rendererTexturing->GeneratePalettedTexture (tinfo->texBase, tinfo->texAuxPTextures,
																	tinfo->x, tinfo->y, 0, tinfo->mipMapCount,
																	((unsigned int *) &tinfo->palettedData->palette.data));
					}
				}
				break;
			case GR_TEXFMT_YIQ_422:
			case GR_TEXFMT_AYIQ_8422:
				if (tinfo->palettedData != NULL) {
					if (tinfo->auxPFormat == NULL) {
						TexConvertNCCToABGRPalette(&(tinfo->palettedData->ncctable), palEntries);
						if (tinfo->palettedData->lpDDPal)
							rendererTexturing->SetPaletteEntries (tinfo->palettedData->lpDDPal, palEntries);
					} else {
						//renderer auxconverter
						unsigned int palette[256];
						TexConvertNCCToARGBPalette (&tinfo->palettedData->ncctable, palette);
						rendererTexturing->GeneratePalettedTexture (tinfo->texBase, tinfo->texAuxPTextures,
																	tinfo->x, tinfo->y, 0, tinfo->mipMapCount, palette);
					}
				}
				break;
		}
	}
	tinfo->flags &= ~TF_CONSIDEREDASLOST;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Primitívek kiflusholásakor meghívódó rutin: megnézi, hogy az aktuális textúra palettája */
/* megegyezik-e az aktuális palettával, és ha nem, újratölti az egész textúrát */
void TexUpdateTexturePalette ()
{

	DEBUG_BEGINSCOPE("TexUpdateTexturePalette", DebugRenderThreadId);

	TexCacheEntry* tinfo;
	if ((tinfo = (TexCacheEntry *) astate.acttex) != NULL)	{
		
		if ( ((tinfo->info.format == GR_TEXFMT_P_8) || (tinfo->info.format == GR_TEXFMT_AP_88)) )	{
			
			if ((tinfo->dxFormat->pixelFormat.BitPerPixel == 8) || (tinfo->auxPFormat != NULL))	{
				if ((tinfo->flags & TF_BITMAPCHANGED && tinfo->auxPFormat != NULL) ||
					TexComparePalettes(&(tinfo->palettedData->palette), &palette) )	{
					
					TexCopyPalette(&(tinfo->palettedData->palette), &palette, 256);
					TexReloadTexture (tinfo, RF_PALETTE);
					if (tinfo->auxPFormat == NULL) {
						colorKeyPal = TexGetTexIndexValue(0, GlideGetColor(astate.colorkey), &palette);
						updateFlags |= UPDATEFLAG_COLORKEYSTATE;
					}
				}
			} else {
				if (TexComparePalettesWithMap(&(tinfo->palettedData->palette), &palette, tinfo->palettedData->palmap) )	{
					unsigned int reloadPalmap = tinfo->palettedData->palette.data[0] != 0xFFFFFFFF ? RF_NOPALETTEMAPCREATE : 0;
					TexCopyPalette(&(tinfo->palettedData->palette), &palette, 256);
					TexReloadTexture(tinfo, RF_TEXTURE | reloadPalmap);
				}
			}
			tinfo->flags &= ~TF_BITMAPCHANGED;
		}
	}
	updateFlags &= ~UPDATEFLAG_SETPALETTE;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void TexUpdateTextureNccTable ()
{
	
	DEBUG_BEGINSCOPE("TexUpdateTextureNccTable", DebugRenderThreadId);
	
	TexCacheEntry* tinfo;
	if ((tinfo = (TexCacheEntry *) astate.acttex) != NULL)	{
		if ( ((tinfo->info.format == GR_TEXFMT_YIQ_422) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422)) )	{
			if ((tinfo->dxFormat->pixelFormat.BitPerPixel == 8) || (tinfo->auxPFormat != NULL))	{
				if ((tinfo->flags & TF_BITMAPCHANGED && tinfo->auxPFormat != NULL) ||
					TexCompareNCCTables(&tinfo->palettedData->ncctable, actncctable))	{
					CopyMemory(&tinfo->palettedData->ncctable, actncctable, sizeof(GuNccTable));
					TexReloadTexture (tinfo, RF_PALETTE);
					if (tinfo->auxPFormat == NULL) {
						colorKeyNcc0 = TexGetTexIndexValue(1, GlideGetColor(astate.colorkey), &nccTable0);
						colorKeyNcc1 = TexGetTexIndexValue(1, GlideGetColor(astate.colorkey), &nccTable1);
						updateFlags |= UPDATEFLAG_COLORKEYSTATE;//UPDATEFLAG_SETCOLORKEY;
					}
				}
			} else {
				if (TexCompareNCCTables(&tinfo->palettedData->ncctable, actncctable))	{
					unsigned int reloadPalmap = tinfo->palettedData->ncctable.iRGB[0][0] != ((short) 0x8000) ? RF_NOPALETTEMAPCREATE : 0;
					CopyMemory(&tinfo->palettedData->ncctable, actncctable, sizeof(GuNccTable));
					TexReloadTexture(tinfo, RF_TEXTURE | reloadPalmap);
				}
			}
			tinfo->flags &= ~TF_BITMAPCHANGED;
		}
	}
	updateFlags &= ~UPDATEFLAG_SETNCCTABLE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void		TexUpdateColorKeyState ()
{
	int i = 0;
	RendererGeneral::ColorKeyMethod ckMethod;

	TexCacheEntry	*tInfo = astate.acttex;

	unsigned int ck = 0;
	int	ckDisabled = 0;
	int disableAlphaTesting = 0;

	if (astate.colorKeyEnable) {
		if ((astate.cotherp == GR_COMBINE_OTHER_TEXTURE) &&
			((tInfo == NULL) ||
			 ((ck = TexGetTexelValueFromARGB(GlideGetColor(astate.colorkey), tInfo->info.format, &palette, actncctable)) == 0))) {

			ckDisabled = 1;
		}
	} else
		ckDisabled = 1;


	if ((astate.flags & STATE_CKMMASK) == STATE_CKMUNDEFINED) {
		disableAlphaTesting = needRecomposePixelPipeLine = 1;
		rendererGeneral->EnableAlphaColorkeying (RendererGeneral::AlphaDisabled, 0);
		rendererGeneral->EnableNativeColorKeying (FALSE, 1);
	}
	
	while (1) {
		ckMethod = ckDisabled ? RendererGeneral::Disabled : ckmPreferenceOrder[i++];
		switch (ckMethod) {

			case RendererGeneral::Native:
			case RendererGeneral::NativeTNT:

				//TODO: A natív iterált bemenetet is figyelembe vehet!!!
				if (astate.cotherp == GR_COMBINE_OTHER_TEXTURE) {

					if ((ckMethod == RendererGeneral::NativeTNT) &&
						((astate.alphaBlendEnable) || (astate.alphaTestFunc != GR_CMP_ALWAYS)))
						break;

					TextureType lpDDTexture = tInfo->texBase[0];
					if (lpDDTexture == NULL)
						break;

					/* Ha a textúra palettás, vagy ha multitextúrázott */
					if ( (tInfo->dxFormat->pixelFormat.BitPerPixel == 8) || (tInfo->texAP88AlphaChannel[0] != NULL) )	{
						if ( (tInfo->info.format == GR_TEXFMT_YIQ_422) || (tInfo->info.format == GR_TEXFMT_AYIQ_8422) )	{
							ck = (actncctable == &nccTable0) ? colorKeyNcc0 : colorKeyNcc1;
						} else ck = colorKeyPal;			
					} else ck = tInfo->dxFormat->colorKeyInTexFmt;

					unsigned int prevMode = (astate.flags & STATE_CKMMASK);
					unsigned int newMode = (ckMethod == RendererGeneral::Native) ? STATE_CKMNATIVE : STATE_CKMNATIVETNT;

					if (prevMode != newMode) {
						if (prevMode == STATE_CKMALPHABASED) {
							disableAlphaTesting = rendererGeneral->IsAlphaTestUsedForAlphaBasedCk ();
							needRecomposePixelPipeLine |= rendererGeneral->EnableAlphaColorkeying (RendererGeneral::AlphaDisabled, 0);
						}
						
						LOG (0,"\n   RendererGeneral::Native, enable colorkeying");
						needRecomposePixelPipeLine |= rendererGeneral->EnableNativeColorKeying (TRUE, newMode == STATE_CKMNATIVETNT);
						
						astate.flags = (astate.flags & ~STATE_CKMMASK) | newMode;
					}

					if (ck != tInfo->colorKeyNative) {
						LOG (0,"\n   RendererGeneral::Native, ck = %0x",ck);
						rendererTexturing->SetSrcColorKeyForTexture (lpDDTexture, ck);
						tInfo->colorKeyNative = ck;
					}
					rendererTexturing->SetSrcColorKeyForTextureFormat (tInfo->dxFormat->dxTexFormatIndex, ck);

					i = 0;
				}
				break;

			case RendererGeneral::AlphaBased:
				if (astate.cotherp == GR_COMBINE_OTHER_TEXTURE) {
					if (rendererGeneral->IsAlphaTestUsedForAlphaBasedCk ()) {
						if (!texCkCanGoWithAlphaTesting[astate.alphaTestFunc])
							break;
					}
					if ( (ck != tInfo->colorKeyAlphaBased) )	{
						if (tInfo->texCkAlphaMask[0] == NULL) {
							GlideCreateAlphaTexture(tInfo);
							
							if (tInfo->texCkAlphaMask[0] != NULL)
								updateFlags |= UPDATEFLAG_RESETTEXTURE;
							else
								break;
							
						}

						void* src = (perfectEmulating) ? src = textureMem + tInfo->startAddress : ((tInfo->flags & TF_STOREDMIPMAP) ? &tInfo->palettedData->data : NULL );
						if (src == NULL)
							break;

						unsigned int x = tInfo->x;
						unsigned int y = tInfo->y;
						unsigned int alphaMask = ((1L << tInfo->alphaDxFormat->pixelFormat.ABitCount) - 1) << tInfo->alphaDxFormat->pixelFormat.APos;


						LOG (0,"\n   RendererGeneral::AlphaBased, src = %0x, lpddtex: %0x",tInfo->startAddress, tInfo->texCkAlphaMask[0]);

						rendererTexturing->RestoreTexturePhysicallyIfNeeded (tInfo->texCkAlphaMask[0]);
						int j;
						for(j = 0; j < tInfo->mipMapCount; j++)	{
							TextureType lpDDTexture = tInfo->texCkAlphaMask[j];
							if (lpDDTexture)	{
								void* texPtr;
								unsigned int texPitch;
								if ((texPtr = rendererTexturing->GetPtrToTexture (lpDDTexture, j, &texPitch)) != NULL) {
									src = TexCreateAlphaMask(src, texPtr,
															 texPitch,
															 GlideGetTexBYTEPP(tInfo->info.format),
															 tInfo->alphaDxFormat->pixelFormat.BitPerPixel/8,
															 x, y, ck,
															 alphaMask);
									rendererTexturing->ReleasePtrToTexture (lpDDTexture, j);
								} else {
									break;
								}
							} else {
								break;
							}
							x >>= 1;
							y >>= 1;
						}
						if (j != tInfo->mipMapCount) {
							DEBUGLOG (0, "\n   Error: TexUpdateAlphaBasedColorKeyState: Cannot lock and update alpha mask texture!");
							DEBUGLOG (1, "\n   Hiba: TexUpdateAlphaBasedColorKeyState: Nem tudom zárolni és frissíteni az alfa-maszk textúrát!");
							break;
						}
						tInfo->colorKeyAlphaBased = ck;
					}
					unsigned int prevMode = astate.flags & STATE_CKMMASK;
					
					if (prevMode != STATE_CKMALPHABASED) {
						if (rendererGeneral->IsAlphaTestUsedForAlphaBasedCk () && (astate.alphaTestFunc == GR_CMP_ALWAYS)) {
							rendererGeneral->EnableAlphaTesting (TRUE);
							rendererGeneral->SetAlphaTestRefValue ((GrAlpha_t) config.AlphaCKRefValue);
							rendererGeneral->SetAlphaTestFunction (GR_CMP_GREATER);
						}
						if (prevMode != STATE_CKMDISABLED)
							needRecomposePixelPipeLine |= rendererGeneral->EnableNativeColorKeying (FALSE, prevMode == STATE_CKMNATIVETNT);
						
						astate.flags = (astate.flags & ~STATE_CKMMASK) | STATE_CKMALPHABASED;
					}

					RendererGeneral::AlphaCkModeInPipeline alphaCkModeInPipeline =
					 (texCkCanGoWithAlphaTesting[astate.alphaTestFunc] == 2) ? RendererGeneral::AlphaForFncLess : RendererGeneral::AlphaForFncGreater;

					//Ezen a feltételen lehetne szigorítani...
					int alphaOutputUsed = ((astate.alphaBlendEnable) || (astate.alphaTestFunc != GR_CMP_ALWAYS));
					needRecomposePixelPipeLine |= rendererGeneral->EnableAlphaColorkeying (alphaCkModeInPipeline, alphaOutputUsed);

					i = 0;
				}
				break;
			
			default:
				i = 0;
				LOG (0,"\n   RendererGeneral::Disable");
				unsigned int prevMode = astate.flags & STATE_CKMMASK;
				switch (prevMode) {
					case STATE_CKMALPHABASED:
						disableAlphaTesting = rendererGeneral->IsAlphaTestUsedForAlphaBasedCk ();
						needRecomposePixelPipeLine |= rendererGeneral->EnableAlphaColorkeying (RendererGeneral::AlphaDisabled, 0);
						break;

					case STATE_CKMNATIVE:
					case STATE_CKMNATIVETNT:
						needRecomposePixelPipeLine |= rendererGeneral->EnableNativeColorKeying (FALSE, prevMode == STATE_CKMNATIVETNT);
						break;

					case STATE_CKMDISABLED:
					default:
						break;

				}
				astate.flags = (astate.flags & ~STATE_CKMMASK) | STATE_CKMDISABLED;
				break;
		}

		if (disableAlphaTesting)	{
			if (astate.alphaTestFunc == GR_CMP_ALWAYS)
				rendererGeneral->EnableAlphaTesting (FALSE);
			rendererGeneral->SetAlphaTestRefValue ((GrAlpha_t) astate.alpharef);
		}

		if (i == 0)
			break;
	}

	updateFlags &= ~UPDATEFLAG_COLORKEYSTATE;
}


void TexUpdateMultitexturingState (TexCacheEntry *tinfo)
{
TextureType		lpDDTexture, lpDDAlphaChannel;

	if (tinfo != NULL)	{
		lpDDTexture = tinfo->texBase[0];
		if( (lpDDAlphaChannel = tinfo->texAP88AlphaChannel[0]) == NULL) {
			lpDDAlphaChannel = lpDDTexture;
		} else if (astate.flags & STATE_TCLOCALALPHA) {
			lpDDTexture = lpDDAlphaChannel;
		}

	} else {
		lpDDTexture = lpDDAlphaChannel = NULL;
	}

	if (astate.lpDDTCCTex != NULL)
		lpDDTexture = astate.lpDDTCCTex;

	if (astate.lpDDTCATex != NULL)
		lpDDAlphaChannel = astate.lpDDTCATex;

	if (lpDDTexture == lpDDAlphaChannel) lpDDAlphaChannel = NULL;

	unsigned int flags = (lpDDAlphaChannel != NULL) ? STATE_APMULTITEXTURING : 0;
	/* Ha a multitextúrázási állapot megváltozott, akkor az alpha combine-t meg kell hívni */
	/* az esetleges stage-csúszás miatt */
	if ((astate.flags & STATE_APMULTITEXTURING) != flags)	{
		astate.flags &= ~STATE_APMULTITEXTURING;
		astate.flags |= flags;
		updateFlags |= UPDATEFLAG_ALPHACOMBINE;
	}
}


void TexSetCurrentTexture (TexCacheEntry *tinfo)
{
TextureType				lpDDTexture, lpDDAlphaChannel, lpDDCkMask;
TextureType				lpDDTextures[2];
unsigned int			i, textureUsage, reload;
TexCacheEntry			*tinfo2;

	DEBUG_BEGINSCOPE("TexSetCurrentTexture", DebugRenderThreadId);
	
	LOG(0,"\nTexSetCurrentTexture(tinfo=%0x)", tinfo);

	if (!(astate.flags & STATE_TEXTUREUSAGEMASK))
		return;

	tinfo2 = NULL;

	if (tinfo != NULL)	{

#ifdef	WRITETEXTURESTOBMP
		WTEX(tinfo);
#endif

		lpDDTexture = tinfo->texBase[0];
		lpDDCkMask = tinfo->texCkAlphaMask[0];
		if( (lpDDAlphaChannel = tinfo->texAP88AlphaChannel[0]) == NULL) lpDDAlphaChannel = lpDDTexture;
			else if (astate.flags & STATE_TCLOCALALPHA) lpDDTexture = lpDDAlphaChannel;

	} else lpDDTexture = lpDDAlphaChannel = lpDDCkMask = NULL;

	if (astate.lpDDTCCTex != NULL)	{
		lpDDTexture = astate.lpDDTCCTex;
		tinfo2 = nullTexture;
	}
	if (astate.lpDDTCATex != NULL)	{
		lpDDAlphaChannel = astate.lpDDTCATex;
		tinfo2 = nullTexture;
	}
	
	if (tinfo != NULL)	{
		lpDDTextures[0] = tinfo->texBase[0];
		lpDDTextures[1] = tinfo->texAP88AlphaChannel[0];
		for(reload = i = 0; i < 2; i++)
			reload |= rendererTexturing->RestoreTexturePhysicallyIfNeeded (lpDDTextures[i]);

		if ( (tinfo->flags & TF_CONSIDEREDASLOST) || reload) TexReloadTexture(tinfo, RF_TEXTURE | RF_PALETTE);
	}
	
	if (tinfo2 != NULL)	{
		lpDDTextures[0] = tinfo2->texBase[0];
		lpDDTextures[1] = tinfo2->texAP88AlphaChannel[0];
		for(reload = i = 0; i < 2; i++)
			reload |= rendererTexturing->RestoreTexturePhysicallyIfNeeded (lpDDTextures[i]);
		
		if ( (tinfo2->flags & TF_CONSIDEREDASLOST) || reload) TexReloadTexture(tinfo2, RF_TEXTURE | RF_PALETTE);
	}

	if ( (astate.flags & STATE_TEXTURE0) && (lpDDTexture != astate.lpDDTex[0]) )
		rendererTexturing->SetTextureAtStage (0, astate.lpDDTex[0] = lpDDTexture);

	textureUsage = STATE_TEXTURE1;
	
	unsigned int lastUsedTexture = 0;

	if ((astate.flags & STATE_CKMMASK) != STATE_CKMDISABLED) {
		lastUsedTexture = STATE_TEXTURE3;

		for (i = 0; i < MAX_STAGETEXTURES; i++) {
			if (astate.flags & lastUsedTexture)
				break;
			lastUsedTexture >>= 1;
		}

		if ((astate.flags & STATE_CKMMASK) != STATE_CKMALPHABASED) {
			TextureType lpDDNativeCkAux = rendererTexturing->GetAuxTextureForNativeCk (tinfo->dxFormat->dxTexFormatIndex);
			if (lpDDNativeCkAux != NULL)
				lpDDCkMask = lpDDNativeCkAux;
			else
				lastUsedTexture = 0;
		}
	}
	
	lpDDTexture = lpDDAlphaChannel;
	
	for(i = 1; i < MAX_STAGETEXTURES; i++)	{
		if (/*((astate.flags & STATE_CKMMASK) == STATE_CKMALPHABASED) &&*/ (textureUsage == lastUsedTexture))
			lpDDTexture = lpDDCkMask;

		if ( (astate.flags & textureUsage) && (lpDDTexture != astate.lpDDTex[i]) )
			rendererTexturing->SetTextureAtStage (i, astate.lpDDTex[i] = lpDDTexture);
		
		textureUsage <<= 1;
	}

	updateFlags &= ~UPDATEFLAG_RESETTEXTURE;
	LOG(0,"   set texture, shadow texture handle: %p",(tinfo) ? tinfo->texCkAlphaMask[0] : NULL);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Az astate-ben eltárolt paraméterek alapján beállítja az aktuális textúrát. */
void GlideTexSource ()
{
GrTexInfo				info;
TexCacheEntry			*tinfo, *sectinfo;
GrLOD_t					lod;
int						i,j, level;
FxU32					endAddress;
void					*src;
struct PalettedData		*palettedData;

	DEBUG_BEGINSCOPE("GlideTexSource", DebugRenderThreadId);

	if (!(astate.vertexmode & VMODE_TEXTURE))
		return;

#ifdef GLIDE3
	info.smallLodLog2 = (GrLOD_t) astate.smallLod;
	info.largeLodLog2 = (GrLOD_t) astate.largeLod;
	info.aspectRatioLog2 = (GrAspectRatio_t) astate.aspectRatio;
	info.format = (GrTextureFormat_t) astate.format;
#else
	info.smallLod = (GrLOD_t) astate.smallLod;
	info.largeLod = (GrLOD_t) astate.largeLod;
	info.aspectRatio = (GrAspectRatio_t) astate.aspectRatio;
	info.format = (GrTextureFormat_t) astate.format;
#endif

	updateFlags |= UPDATEFLAG_SETPALETTE | UPDATEFLAG_SETNCCTABLE | UPDATEFLAG_COLORKEYSTATE;
	
	palettedData = NULL;
	/* Ha nincs multibase texturing: a cache-ben keresett textúra ellenõrzése kétféle módon mehet: */
	/* - tökéletes emu esetén: az adott kezdõcímen pontosan a megadott jellemzõkkel kell rendelkeznie */
	/* - egyébként elég, ha az adott kezdõcímen talál vmilyen textúrát */
	if (!astate.multiBaseTexturing)	{
		if ( ((tinfo = TexFindTex(astate.startAddress[0], &info)) == astate.acttex) && (astate.acttex != NULL) ) return;
		astate.acttex = NULL;
		if (tinfo != NULL)	{
#ifdef GLIDE3
			if ( (tinfo->flags & TF_SECONDARY) ||
				( (tinfo->flags & TF_STORESONELEVEL) && (tinfo->info.smallLodLog2 != tinfo->info.largeLodLog2) ) )	{
#else
			if ( (tinfo->flags & TF_SECONDARY) ||
				( (tinfo->flags & TF_STORESONELEVEL) && (tinfo->info.smallLod != tinfo->info.largeLod) ) )	{
#endif
				if ((tinfo->flags & TF_STOREDMIPMAP) && GlideCompareInfos(&info, &tinfo->info) &&
					(!(tinfo->flags & TF_STORESONELEVEL)) )	{
					palettedData = tinfo->palettedData;
					tinfo->palettedData = NULL;
				}
				tinfo = NULL;
			}
		}
		if (tinfo == NULL)	{
			if ( (tinfo = TexAllocCacheEntry(astate.startAddress[0], &info, NULL, 0)) != NULL )	{
				if (GlideCreateTexture(tinfo))	{
					TexReloadTexture (tinfo, RF_TEXTURE);
					if (palettedData) _FreeMem(palettedData);
				} else tinfo = NULL;
			}
		}
	/* Multibase eset: kétféle ellenõrzési mód: */
	/* - tökéletes emu: az adott kezdõcímen pontosan a megadott jellemzõkkel, beleértve a másodlagos szinteket is */
	/* - nem tök. emu, mipmap tárolásával: elég ha az elsõdeges textúra kezdõcímén talál vmit */
	/* - nem tök. emu, mipmap tárolása nélkül: elég ha az elsõdeges textúra kezdõcímén talál vmit */
	} else {
		astate.acttex = NULL;
		/* Elsõdleges textúra (szint) ellenõrzése */
		tinfo = TexFindTex(astate.startAddress[i = baseRegister[astate.largeLod]], &info);
		if (tinfo != NULL)	{

#ifdef GLIDE3
			if ( (tinfo->flags & TF_SECONDARY) ||
				( (!(tinfo->flags & TF_STORESONELEVEL)) && (tinfo->info.smallLodLog2 != tinfo->info.largeLodLog2) && (tinfo->info.largeLodLog2 > GR_LOD_32)) )	{
#else
			if ( (tinfo->flags & TF_SECONDARY) ||
				( (!(tinfo->flags & TF_STORESONELEVEL)) && (tinfo->info.smallLod != tinfo->info.largeLod) && (tinfo->info.largeLod < GR_LOD_32)) )	{
#endif
				if ((tinfo->flags & TF_STOREDMIPMAP) && GlideCompareInfos(&info, &tinfo->info)) {
					palettedData = tinfo->palettedData;
					tinfo->palettedData = NULL;
				}
				tinfo = NULL;
			}
		}
		if (tinfo == NULL)	{
#ifdef GLIDE3
			if ( (tinfo = TexAllocCacheEntry(astate.startAddress[i], &info, NULL, (info.largeLodLog2 > GR_LOD_32))) != NULL )	{
#else
			if ( (tinfo = TexAllocCacheEntry(astate.startAddress[i], &info, NULL, (info.largeLod < GR_LOD_32))) != NULL )	{
#endif
				if (GlideCreateTexture(tinfo))	{
					src = (perfectEmulating) ? textureMem + astate.startAddress[i] : NULL;
					GlideLoadTextureMipMap(tinfo, src, 0, 1, 0, 1);
					if (palettedData) _FreeMem(palettedData);
				} else tinfo = NULL;
			}
		}
		/* Másodlagos textúrák (szintek) ellenõrzése */
		if (tinfo != NULL)	{
			level = 1;
#ifdef GLIDE3
			lod = info.largeLodLog2;
			while( (lod != (GrLOD_t) astate.smallLod) && (lod > GR_LOD_32) )	{
				info.largeLodLog2 = --lod;
				info.smallLodLog2 = (lod == GR_LOD_32) ? (GrLOD_t) astate.smallLod : lod;
#else
			lod = info.largeLod;
			while( (lod != (GrLOD_t) astate.smallLod) && (lod < GR_LOD_32) )	{
				info.largeLod = ++lod;
				info.smallLod = (lod == GR_LOD_32) ? (GrLOD_t) astate.smallLod : lod;
#endif
				sectinfo = TexFindTex(astate.startAddress[++i], &info);
				
				if (sectinfo != NULL) if (!(sectinfo->flags & TF_SECONDARY)) sectinfo = NULL;

				src = (perfectEmulating) ? textureMem + astate.startAddress[i] : NULL;
				if (sectinfo == NULL)	{
					endAddress = astate.startAddress[i] + grTexTextureMemRequiredInternal (GR_MIPMAPLEVELMASK_BOTH, &info);
					/* Ha ennek másodlagos textúrának nincs közös része a szülõtextúrával, */
					/* akkor OK, egyébként nem hozzuk létre, mert az törölné a szülõtextúrát. */
					if ( ((astate.startAddress[i] < tinfo->startAddress) && (endAddress <= tinfo->startAddress)) || 
						((astate.startAddress[i] >= tinfo->endAddress) && (endAddress >= tinfo->endAddress)) )	{
						sectinfo = TexAllocCacheEntry(astate.startAddress[i], &info, NULL, 0);
						sectinfo->flags |= TF_SECONDARY | TF_SECRELOAD;
					} else {
#ifdef GLIDE3
						for(j = info.smallLodLog2; j <= info.largeLodLog2; j++)
#else
						for(j = info.largeLod; j <= info.smallLod; j++)
#endif
							src = GlideLoadTextureMipMap(tinfo, src, level++, 1, 0, 1);
							tinfo->flags |= TF_BITMAPCHANGED;
					}
				}
				if (sectinfo != NULL)	{
					if ( (sectinfo->parentMipMap != tinfo) || (tinfo->flags & TF_SECRELOAD) )	{
						sectinfo->parentMipMap = tinfo;
#ifdef GLIDE3
						for(j = info.smallLodLog2; j <= info.largeLodLog2; j++)
#else
						for(j = info.largeLod;j <= info.smallLod; j++)
#endif
							src = GlideLoadTextureMipMap(tinfo, src, level++, 1, 0, 1);
							tinfo->flags |= TF_BITMAPCHANGED;
					}
				}
			}
			tinfo->flags &= ~TF_SECRELOAD;
		}
	}
	astate.acttex = tinfo;

	//TexSetCurrentTexture(tinfo);
	updateFlags |= UPDATEFLAG_RESETTEXTURE;
	updateFlags &= ~UPDATEFLAG_SETTEXTURE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

}


int		TexIsDXFormatSupported (enum DXTextureFormats format)
{
	return (dxTexFormats[format].isValid);
}


void	TexGetDXTexPixFmt (enum DXTextureFormats format, _pixfmt *pixFormat)
{
	*pixFormat = dxTexFormats[format].pixelFormat;
}


void	SetTexMemScale ()
{
	unsigned int scale = config.TexMemSize / TEXMEMSCALE_MAX;
	unsigned int bit = 1;
	texMemScalePwr = 0;
	for (; scale & (~bit); scale &= (~bit), bit<<=1, texMemScalePwr++);
	texMemScale = scale;
	if (texMemScalePwr == 0) {
		config.Flags2 &= ~CFG_SCALETEXMEM;
	}
}


FxU32 _inline GetScaledSize (FxU32 size)
{
	if (config.Flags2 & CFG_SCALETEXMEM) {
		if (texMemScale == 0) {
			SetTexMemScale ();
		}
		return (size >> texMemScalePwr) + ((size & (texMemScale-1)) ? 1 : 0);
	}
	return size;
}


FxU32 _inline GetScaledAddress (FxU32 address)
{
	if (address >= RESERVED_ADDRESSSPACE)
		return address;

	if ((config.Flags2 & CFG_SCALETEXMEM) && (texMemScale == 0)) {
		SetTexMemScale ();
	}
	return address << texMemScalePwr;
}


/*------------------------------------------------------------------------------------------*/
/*.......................... Glide függvények implementációja ..............................*/


/* Minimum texmem cím = 0 */
FxU32 EXPORT grTexMinAddress(GrChipID_t /*tmu*/)
{
	return(0);
}


/* Maximum texmem cím = amennyi a konfigban be van állítva */
FxU32 EXPORT grTexMaxAddress(GrChipID_t tmu)
{
	FxU32 texMem = config.TexMemSize;
	if (config.Flags2 & CFG_SCALETEXMEM) {
		texMem = TEXMEMSCALE_MAX;
	}
	return ((tmu == GR_TMU0) ? texMem : 0);
}


/* Mipmap által foglalt texmem mérete */
static FxU32 grTexCalcMemRequiredInternal(GrLOD_t smallLod, GrLOD_t largeLod, GrAspectRatio_t aspect, GrTextureFormat_t format)
{
	unsigned int llod = GlideGetLOD(largeLod);
	unsigned int slod = GlideGetLOD(smallLod);
	unsigned int needed = GlideGetXY(largeLod,aspect);
	unsigned int size = ( (needed >> 16) * (needed & 0xFFFF) ) * GlideGetTexBYTEPP(format);

	for(needed = 0; llod >= slod; llod >>= 1)	{
		needed += size;
		if ( (size >>= 2) == 0 ) size = 1;
	}	
	return((needed + 7) & (~7) );
}


FxU32 EXPORT grTexCalcMemRequired(GrLOD_t smallLod, GrLOD_t largeLod,
								  GrAspectRatio_t aspect, GrTextureFormat_t format)
{
	return GetScaledSize (grTexCalcMemRequiredInternal (smallLod, largeLod, aspect, format));
}


/* Szétbontott mipmap texmem mérete */
/* Megjegyz: az 1 TMU miatt a szétbontást nem támogatja! */
static FxU32 grTexTextureMemRequiredInternal (FxU32 evenOdd, GrTexInfo *info)
{
#ifdef GLIDE3
	
	unsigned int llod = GlideGetLOD(info->largeLodLog2);
	unsigned int slod = GlideGetLOD(info->smallLodLog2);
	unsigned int needed = GlideGetXY(info->largeLodLog2, info->aspectRatioLog2);

#else
 
	unsigned int llod = GlideGetLOD(info->largeLod);
	unsigned int slod = GlideGetLOD(info->smallLod);
	unsigned int needed = GlideGetXY(info->largeLod, info->aspectRatio);

#endif

	if ( ((evenOdd == GR_MIPMAPLEVELMASK_ODD) && ((llod & 0x5555) == 0)) || 
		 ((evenOdd == GR_MIPMAPLEVELMASK_EVEN) && ((llod & 0xAAAA) == 0)) )	{
		needed=(needed >> 1) & (~0x8000);
		llod >>= 1;
	}
	unsigned int rshift = (evenOdd == GR_MIPMAPLEVELMASK_BOTH) ? 1 : 2;
	unsigned int rshift2 = 2*rshift;
   
	
	unsigned int size = ( (needed >> 16) * (needed & 0xFFFF) ) * GlideGetTexBYTEPP(info->format);
	if (size == 0) size = 1;
	for(needed = 0; llod >= slod; llod >>= rshift)	{
		needed += size;
		if ( (size >>= rshift2) == 0 ) size = 1;
	}	
	return((needed + 7) & (~7) );
}


FxU32 EXPORT grTexTextureMemRequired (FxU32 evenOdd, GrTexInfo *info)
{
	return GetScaledSize (grTexTextureMemRequiredInternal (evenOdd, info));
}


/* Textúra filtering mód beáll. */
void EXPORT grTexFilterMode(GrChipID_t tmu,
							GrTextureFilterMode_t minFilterMode,
							GrTextureFilterMode_t magFilterMode)
{
	
	DEBUG_BEGINSCOPE("grTexFilterMode",DebugRenderThreadId);
	
 	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN )
		return;
	
	if (tmu != 0)
		return;
	
	if (config.Flags & CFG_BILINEARFILTER)
		minFilterMode = magFilterMode = GR_TEXTUREFILTER_BILINEAR;
	
	if ( (astate.minFilter != minFilterMode) || (astate.magFilter != magFilterMode) ) {
		DrawFlushPrimitives();
		int i;
		if (astate.minFilter != minFilterMode)	{
			for(i = 0; i < 4; i++)
				rendererTexturing->SetTextureStageMinFilter (i, astate.minFilter = minFilterMode);
		}
		if (astate.magFilter != magFilterMode)	{
			for(i = 0; i < 4; i++)
				rendererTexturing->SetTextureStageMagFilter (i, astate.magFilter = magFilterMode);
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexClampMode(GrChipID_t tmu,
						   GrTextureClampMode_t sClampMode,
						   GrTextureClampMode_t tClampMode)
{
	
	DEBUG_BEGINSCOPE("grTexClampMode",DebugRenderThreadId);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if (tmu != 0)
		return;

	if ( (astate.clampModeU != sClampMode) || (astate.clampModeV != tClampMode) ) {
		DrawFlushPrimitives();
		int i;
		if (astate.clampModeU != sClampMode)	{
			for(i = 0; i < MAX_STAGETEXTURES; i++)
				rendererTexturing->SetTextureStageClampModeU (i, astate.clampModeU = sClampMode);
		}
		if (astate.clampModeV != tClampMode)	{
			for(i = 0; i < MAX_STAGETEXTURES; i++)
				rendererTexturing->SetTextureStageClampModeV (i, astate.clampModeV = tClampMode);
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexLodBiasValue(GrChipID_t tmu, float bias)
{

	DEBUG_BEGINSCOPE("grTexLodBiasValue",DebugRenderThreadId);
	
	LOG(0,"\ngrTexLodBiasValue(tmu=%d, bias=%f)",tmu,bias);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if (tmu != 0)
		return;
	
	DrawFlushPrimitives();
	if (bias != astate.lodBias) {
		astate.lodBias = bias;
		for(int i = 0; i < MAX_STAGETEXTURES; i++)
			rendererTexturing->SetTextureStageLodBias (i, bias);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Egy adott Voodoo texmem-címre textúra feltöltése a megadott formátummal */
/* Megjegyz: gyors textúrafeltöltés */
void EXPORT grTexDownloadMipMap(GrChipID_t tmu, FxU32 startAddress,
								FxU32 evenOdd, GrTexInfo *info )
{
	DEBUG_BEGINSCOPE("grTexDownloadMipMap", DebugRenderThreadId);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	if (tmu != 0)
		return;
	
	startAddress = GetScaledAddress (startAddress);

	LOG(0,"\ngrTexDownloadMipMap(tmu=%d, startAddress=%0x, evenOdd=%s, info=%p)", \
		tmu,startAddress,evenOdd_t[evenOdd],info);
	LOG(0,"\n   info->smallLod = %s, \
		\n   info->largeLod = %s, \
		\n   info->aspectRatio = %s, \
		\n   info->format = %s, \
		\n   info->data = %0x", lod_t[info->smallLod], lod_t[info->largeLod], \
		aspectr_t[info->aspectRatio], tformat_t[info->format], info->data);

	/* kezdôcím csonkolása 8 byte-os határra */
	startAddress &= ~7;

	/* Ha a textúra kilógna a textúramemóriából, akkor nem csinálunk semmit */
	unsigned int texSize = grTexTextureMemRequiredInternal (GR_MIPMAPLEVELMASK_BOTH, info);
	if ((startAddress + texSize > config.TexMemSize) && (startAddress < RESERVED_ADDRESSSPACE) ) {
		return;
	}
	
	/* */
#ifdef GLIDE3
	if (GlideCheckTexIntersect(startAddress, info->smallLodLog2, info->largeLodLog2, info->aspectRatioLog2,info->format))	{
#else
	if (GlideCheckTexIntersect(startAddress, info->smallLod, info->largeLod, info->aspectRatio,info->format))	{
#endif
		DrawFlushPrimitives();
		astate.acttex = NULL;
		updateFlags |= UPDATEFLAG_SETTEXTURE;
	}

	if (perfectEmulating && (startAddress < RESERVED_ADDRESSSPACE))	{
		void* src = textureMem + startAddress;
		if (src != info->data)
			CopyMemory (src, info->data, texSize);
	}
	
	bool recreateTexCacheEntry = true;
	TexCacheEntry* tinfo = TexFindTex(startAddress, NULL);
	if (tinfo != NULL)	{
#ifdef GLIDE3
		if ( (info->format == tinfo->info.format) &&
			 (info->largeLodLog2 == tinfo->info.largeLodLog2) &&
			 (info->aspectRatioLog2 == tinfo->info.aspectRatioLog2) &&
			 (info->smallLodLog2 == tinfo->info.smallLodLog2) )
				recreateTexCacheEntry = false;
#else
		if ( (info->format == tinfo->info.format) &&
			 (info->largeLod == tinfo->info.largeLod) &&
			 (info->aspectRatio == tinfo->info.aspectRatio) &&
			 (info->smallLod == tinfo->info.smallLod) )
				recreateTexCacheEntry = false;
#endif
	}
	if (perfectEmulating && recreateTexCacheEntry && (startAddress < RESERVED_ADDRESSSPACE)) {
		TexInvalidateCache(startAddress, info);
		return;
	}

	if (recreateTexCacheEntry)	{
		if ((tinfo = TexAllocCacheEntry (startAddress, info, NULL, 0)) == NULL)
			return;
		if (!GlideCreateTexture (tinfo))
			return;
	}
	tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
	void* src = info->data;
	for(int i = 0; i < tinfo->mipMapCount; i++) {
		src = GlideLoadTextureMipMap(tinfo, src, i, 1, 0, 1);
	}
	tinfo->flags |= TF_BITMAPCHANGED;
	updateFlags |= UPDATEFLAG_SETPALETTE;
	updateFlags |= UPDATEFLAG_SETNCCTABLE;	
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexDownloadMipMapLevelPartial(GrChipID_t tmu, FxU32 startAddress,
											GrLOD_t thisLod, GrLOD_t largeLod,
											GrAspectRatio_t aspectRatio,
											GrTextureFormat_t format,
											FxU32 evenOdd, void *data,
											unsigned int start,
											unsigned int end )
{
	DEBUG_BEGINSCOPE("grTexDownloadMipMapLevelPartial", DebugRenderThreadId);
	
	startAddress = GetScaledAddress (startAddress);

	LOG(0,"\ngrTexDownloadMipMapLevelPartial(tmu=%d, startAddress=%0x, thisLod=%s, largeLod=%p, aspectRatio=%s, format=%s, evenOdd=%s, data=%p, \
											 start=%d, end=%d)", \
		tmu,startAddress,lod_t[thisLod],lod_t[largeLod], aspectr_t[aspectRatio], \
		tformat_t[format],evenOdd_t[evenOdd],data,start,end);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	startAddress &= ~7;
	
	if ((tmu != 0) || (start > end))
		return;

	unsigned int x = GlideGetXY(thisLod, aspectRatio);
	unsigned int y = x & 0xFFFF;
	x = (x >> 16) * GlideGetTexBYTEPP(format);
	unsigned int toffset = grTexCalcMemRequiredInternal (thisLod, largeLod, aspectRatio, format)-grTexCalcMemRequired(thisLod, thisLod, aspectRatio, format);
	if ((startAddress + toffset + end*x) > config.TexMemSize)
		return;
	
	if (perfectEmulating)
		CopyMemory(textureMem + startAddress + toffset + (start * x), data, (end-start+1)*x);

	if (GlideCheckTexIntersect(startAddress, thisLod, thisLod, aspectRatio, format))	{
		DrawFlushPrimitives();
		astate.acttex = NULL;
		updateFlags |= UPDATEFLAG_SETTEXTURE;
	}

	GrTexInfo info;
#ifdef GLIDE3
	info.largeLodLog2 = largeLod;
	info.smallLodLog2 = thisLod;
	info.aspectRatioLog2 = aspectRatio;
#else
	info.largeLod = largeLod;
	info.smallLod = thisLod;
	info.aspectRatio = aspectRatio;
#endif
	info.format = format; 
	info.data = data;

	bool recreateTexCacheEntry = true;
	TexCacheEntry* tinfo = TexFindTex(startAddress, NULL);
	if (tinfo != NULL)	{

#ifdef GLIDE3
		
		if ( (format == tinfo->info.format) &&
			 (largeLod == tinfo->info.largeLodLog2) &&
			 (aspectRatio == tinfo->info.aspectRatioLog2) )
			 recreateTexCacheEntry = false;

#else
		
		if ( (format == tinfo->info.format) &&
			 (largeLod == tinfo->info.largeLod) &&
			(aspectRatio == tinfo->info.aspectRatio) )
			recreateTexCacheEntry = false;

#endif

	}
	if (perfectEmulating && recreateTexCacheEntry) {
		TexInvalidateCache(startAddress, &info);
		return;
	}

	if (recreateTexCacheEntry)	{
		if ( (tinfo = TexAllocCacheEntry(startAddress, &info, NULL, 0)) == NULL ) return;
		if (!GlideCreateTexture(tinfo)) return;
	}
#ifdef GLIDE3
	unsigned int slod = GlideGetLOD(tinfo->info.smallLodLog2);
#else
	unsigned int slod = GlideGetLOD(tinfo->info.smallLod);
#endif
	unsigned int tlod = GlideGetLOD(thisLod);
	unsigned int llod = GlideGetLOD(largeLod);
	
	if ((slod > tlod) || (tlod > llod) || (start >= y) || (end >= y))
		return;

	tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
	
	unsigned int level;
	for (level = 0; llod != tlod; llod>>=1) level++;
	GlideLoadTextureMipMap(tinfo, data, level, start, end, 1);
	tinfo->flags |= TF_BITMAPCHANGED;
	updateFlags |= UPDATEFLAG_SETPALETTE;
	updateFlags |= UPDATEFLAG_SETNCCTABLE;	
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress,
									  GrLOD_t thisLod, GrLOD_t largeLod,
									  GrAspectRatio_t aspectRatio,
									  GrTextureFormat_t format,
									  FxU32 evenOdd, void *data )
{
	DEBUG_BEGINSCOPE("grTexDownloadMipMapLevel", DebugRenderThreadId);
	
	LOG(0,"\ngrTexDownloadMipMapLevel(tmu=%d, startAddress=%0x, thisLod=%s, largeLod=%p, aspectRatio=%s, format=%s, evenOdd=%s, data=%p)", \
		tmu,GetScaledAddress(startAddress),lod_t[thisLod],lod_t[largeLod], aspectr_t[aspectRatio], \
		tformat_t[format],evenOdd_t[evenOdd],data);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	grTexDownloadMipMapLevelPartial(tmu, startAddress, thisLod, largeLod, aspectRatio,
									format, evenOdd, data, 0, (GlideGetXY(thisLod,aspectRatio) & 0xFFFF)-1);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Egy adott Voodoo texmem-cím mint textúraforrás beállítása */
void EXPORT grTexSource(GrChipID_t tmu, FxU32 startAddress, 
						FxU32 evenOdd, GrTexInfo *info )
{
	DEBUG_BEGINSCOPE("grTexSource", DebugRenderThreadId);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	LOG(0,"\ngrTexSource(tmu=%d, startAddress=%0x, evenOdd=%s, info=%p)", \
		tmu,GetScaledAddress (startAddress), evenOdd_t[evenOdd], info);
	LOG(0,"\n   info->smallLod = %s, \
		\n   info->largeLod = %s, \
		\n   info->aspectRatio = %s, \
		\n   info->format = %s, \
		\n   info->data = %0x", lod_t[info->smallLod], lod_t[info->largeLod], \
		aspectr_t[info->aspectRatio], tformat_t[info->format], info->data);	
	
	grTexMultibaseAddress (tmu, GR_TEXBASE_256, startAddress, evenOdd, info);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* megjegyz: ditherelt mipmappinget nem kezel. Helyette trilineárisra állítja be. */
void EXPORT grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend )
{
	DEBUG_BEGINSCOPE("grTexMipMapMode", DebugRenderThreadId);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	if (tmu != 0)
		return;
	
	if (astate.mipMapMode != mode || astate.lodBlend != lodBlend) {
		astate.mipMapMode = mode;
		astate.lodBlend = lodBlend;
		DrawFlushPrimitives();
		if (((config.Flags2 & CFG_FORCETRILINEARMIPMAPS) && (mode != GR_MIPMAP_DISABLE)) ||
			(config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ) {
			
			mode = GR_MIPMAP_NEAREST;
			lodBlend = FXTRUE;
		}
		
		for(int i = 0; i <= 3; i++)
			rendererTexturing->SetTextureStageMipMapMode (i, mode, lodBlend);
			
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Texture combine unit függvények megadása a texel színének és alfájának kiszámítására: */
/* Mivel a wrapper csak egy TMU-t támogat, ezért az olyan függvények, amelyek a cotherre */
/* vagy aotherre hivatkoznak, definiálatlan eredményt adnak, ezért nem kell implementálni. */
/* */
/* Két függvény került implementálásra: a 0 és a clocal, de ugyanaz lesz alfa függvénye is, */
/* mint a színé. */
void EXPORT grTexCombine(GrChipID_t tmu,
						 GrCombineFunction_t rgb_function,
						 GrCombineFactor_t rgb_factor, 
						 GrCombineFunction_t alpha_function,
						 GrCombineFactor_t alpha_factor,
			             FxBool rgb_invert,
						 FxBool alpha_invert)
{
	DEBUG_BEGINSCOPE("grTexCombine", DebugRenderThreadId);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	if (tmu != 0)
		return;

	LOG(0,"\ngrTexCombine(tmu=%d, rgb_func=%s, rgb_fact=%s, alpha_func=%s, alpha_fact=%s, rgb_inv=%s, alpha_inv=%s", \
		tmu, combfunc_t[rgb_function], combfact_t[rgb_factor], \
		combfunc_t[alpha_function], combfact_t[alpha_factor], \
		fxbool_t[rgb_invert], fxbool_t[alpha_invert]);
	
	if ( (rgb_function == astate.rgb_function) && (rgb_factor == astate.rgb_factor) && (rgb_invert == astate.rgb_invert) &&
		 (alpha_function == astate.alpha_function) && (alpha_factor == astate.alpha_factor) && (alpha_invert == astate.alpha_invert) )
		return;
	
	DrawFlushPrimitives();
	astate.rgb_function = (unsigned char) rgb_function;
	astate.rgb_factor = (unsigned char) rgb_factor;
	astate.rgb_invert = (unsigned char) rgb_invert;
	astate.alpha_function = (unsigned char) alpha_function;
	astate.alpha_factor = (unsigned char) alpha_factor;
	astate.alpha_invert = (unsigned char) alpha_invert;
	
	if (rendererGeneral->UsesPixelShaders ()) {
		updateFlags |= UPDATEFLAG_TEXCOMBINE;
		astate.lpDDTCCTex = astate.lpDDTCATex = NULL;
	} else {
		TextureType lpDDTexture = NULL;
		TextureType lpDDAlpha = NULL;

		if (texCombineFuncImp[rgb_function] == TCF_ZERO) lpDDTexture = nullTexture->texBase[0];
		if (texCombineFuncImp[alpha_function] == TCF_ZERO) lpDDAlpha = nullTexture->texBase[0];
		if (lpDDTexture == lpDDAlpha) lpDDAlpha = NULL;
		
		astate.lpDDTCCTex = lpDDTexture;
		astate.lpDDTCATex = lpDDAlpha;
		if (texCombineFuncImp[rgb_function] == TCF_ALOCAL) astate.flags |= STATE_TCLOCALALPHA;
			else astate.flags &= ~STATE_TCLOCALALPHA;
		updateFlags |= UPDATEFLAG_COLORCOMBINE;
		unsigned int testc = 0;
		unsigned int testa = 0;
		if (rgb_invert) testc = STATE_TCCINVERT;
		if (alpha_invert) testa = STATE_TCAINVERT;
		if ( (astate.flags & STATE_TCCINVERT) != testc)	{
			astate.flags ^= STATE_TCCINVERT;
			updateFlags |= UPDATEFLAG_COLORCOMBINE;
		}
		if ( (astate.flags & STATE_TCAINVERT) != testa)	{
			astate.flags ^= STATE_TCAINVERT;
			updateFlags |= UPDATEFLAG_ALPHACOMBINE;
		}
		//GlideColorCombineUpdateVertexMode();
		//GlideAlphaCombineUpdateVertexMode();
		updateFlags |= UPDATEFLAG_RESETTEXTURE;
	}
	GlideAlphaCombineUpdateVertexMode();
	GlideColorCombineUpdateVertexMode();

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexDownloadTable(GrChipID_t tmu, GrTexTable_t type, void *data )
{
	DEBUG_BEGINSCOPE("grTexDownloadTable", DebugRenderThreadId);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if (tmu != 0)
		return;
	
	LOG(1,"\ngrTexDownloadTable(tmu=%d, type=%s, data=%p)",tmu, textable_t[type], data);
	
	DrawFlushPrimitives();
	switch(type)	{
		case GR_TEXTABLE_PALETTE:
			if (TexCopyPalette(&palette, (GuTexPalette *) data, 256))
				return;
			/*#if LOGGING
				LOG(0,"\nPalette:");
				for(i = 0;i < 16; i++) LOG(0,"\n\t0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x",
										palette.data[i*16+0],palette.data[i*16+1],palette.data[i*16+2],palette.data[i*16+3],
										palette.data[i*16+4],palette.data[i*16+5],palette.data[i*16+6],palette.data[i*16+7],
										palette.data[i*16+8],palette.data[i*16+9],palette.data[i*16+10],palette.data[i*16+11],
										palette.data[i*16+12],palette.data[i*16+13],palette.data[i*16+14],palette.data[i*16+15]);
			#endif*/
			updateFlags |= UPDATEFLAG_SETPALETTE;
			break;

		case GR_NCCTABLE_NCC0:
			TexCopyNCCTable(&nccTable0, (GuNccTable *) data);
			updateFlags |= UPDATEFLAG_SETNCCTABLE;
			break;
		
		case GR_NCCTABLE_NCC1:
			TexCopyNCCTable(&nccTable1, (GuNccTable *) data);
			updateFlags |= UPDATEFLAG_SETNCCTABLE;
			break;
	}		
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexDownloadTablePartial(GrChipID_t tmu, GrTexTable_t type, void *data, int start, int end)
{
FxU32	*pdata = (FxU32 *) data;
	
	DEBUG_BEGINSCOPE("grTexDownloadTablePartial", DebugRenderThreadId);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if ((tmu != 0) || (type != GR_TEXTABLE_PALETTE))
		return;
	
	LOG(0,"\ngrTexDownloadTablePartial(tmu=%d, type=%s, data=%p, start=%d, end=%d)",tmu, textable_t[type], data, start, end);
	
	DrawFlushPrimitives();
	if (TexCopyPalette((GuTexPalette *) (&(palette.data[start])), (GuTexPalette *) &(((GuTexPalette *) pdata)->data[start]), (end-start+1))) return;
	updateFlags |= UPDATEFLAG_SETPALETTE;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexNCCTable(GrChipID_t tmu, GrNCCTable_t table)
{

	DEBUG_BEGINSCOPE("grTexNCCTable", DebugRenderThreadId);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if (tmu != 0)
		return;
	
	LOG(0,"\ngrTexNCCTable(tmu=%d, table=%s)",tmu,ncctable_t[table]);

	GuNccTable	*ncctable;
	switch(table)	{		
		case GR_TEXTABLE_NCC1:		
			ncctable = &nccTable1;
			break;
		case GR_TEXTABLE_NCC0:
		default:
			ncctable = &nccTable0;
			break;
	}
	if (ncctable != actncctable)	{
		DrawFlushPrimitives();
		actncctable = ncctable;
		updateFlags |= UPDATEFLAG_SETNCCTABLE;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grTexMultibase(GrChipID_t tmu, FxBool enable)
{

	LOG(0,"\ngrTexMultibase(tmu=%d,enable=%s)",tmu,fxbool_t[enable]);

	if (tmu != 0)
		return;

	unsigned char mode = (enable) ? 1 : 0;
	if (mode == astate.multiBaseTexturing) return;
	DrawFlushPrimitives();
	astate.multiBaseTexturing = mode;
	updateFlags |= UPDATEFLAG_SETTEXTURE;
}


void EXPORT grTexMultibaseAddress(GrChipID_t tmu, GrTexBaseRange_t range, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info)
{
	startAddress = GetScaledAddress (startAddress);

	LOG(0,"\ngrTexMultibaseAddress(tmu=%d, range=%s, startAddress=%0x, evenOdd=%s, info=%p)",tmu,texbaserange_t[range],startAddress,evenOdd_t[evenOdd],info);
	LOG(0,"\n     info->smallLod=%s,\n     info->largeLod=%s,\n     info->aspectRatio=%s,\n     info->format=%s", \
			lod_t[info->smallLod], lod_t[info->largeLod], aspectr_t[info->aspectRatio], tformat_t[info->format]);
	
	if (tmu != 0)
		return;

	int mipMapLevel;
	GrTexInfo rangeInfo = *info;

	switch(range)	{
		case GR_TEXBASE_256:
			mipMapLevel = 0;
			rangeInfo.largeLod = rangeInfo.smallLod = GR_LOD_256;
			break;
		case GR_TEXBASE_128:
			mipMapLevel = 1;
			rangeInfo.largeLod = rangeInfo.smallLod = GR_LOD_128;
			break;
		case GR_TEXBASE_64:
			mipMapLevel = 2;
			rangeInfo.largeLod = rangeInfo.smallLod = GR_LOD_64;
			break;
		case GR_TEXBASE_32_TO_1:
			mipMapLevel = 3;
			rangeInfo.largeLod = GR_LOD_32;
			rangeInfo.smallLod = GR_LOD_1;
			break;
		default:
			return;
	}

	if ((startAddress + grTexTextureMemRequiredInternal (evenOdd, &rangeInfo)) > config.TexMemSize)
		return;

#ifdef GLIDE3
	if ( (astate.smallLod == (unsigned char) info->smallLodLog2) &&
		 (astate.largeLod == (unsigned char) info->largeLodLog2) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatioLog2) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[mipMapLevel] == startAddress )) return;
#else
	if ( (astate.smallLod == (unsigned char) info->smallLod) &&
		 (astate.largeLod == (unsigned char) info->largeLod) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatio) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[mipMapLevel] == startAddress )) return;
#endif
	
	DrawFlushPrimitives();
	astate.startAddress[mipMapLevel] = startAddress;
	astate.format = (unsigned char) info->format;

#ifdef GLIDE3
	astate.smallLod = (unsigned char) info->smallLodLog2;
	astate.largeLod = (unsigned char) info->largeLodLog2;
	astate.aspectRatio = (unsigned char) info->aspectRatioLog2;
	unsigned int x = GlideGetXY(info->largeLodLog2, info->aspectRatioLog2);
#else
	astate.smallLod = (unsigned char) info->smallLod;
	astate.largeLod = (unsigned char) info->largeLod;
	astate.aspectRatio = (unsigned char) info->aspectRatio;
	unsigned int x = GlideGetXY(info->largeLod, info->aspectRatio);
#endif

	unsigned int y = x & 0xFFFF;
	x >>= 16;
	if (x >= y)	{
		astate.divx = 256.0f;
		astate.divy = (float) ((256*y)/x);
	} else {
		astate.divy = 256.0f;
		astate.divx = (float) ((256*x)/y);
	}
	updateFlags |= UPDATEFLAG_SETTEXTURE;
}


/*------------------------------------------------------------------------------------------*/
/*------------------------ A két fájlkezel¾rutin a textúrázáshoz ---------------------------*/

/* segédrutin: a line pufferben visszaadja a fájl egy nemkomment sorát */
int GlideGet3DFline(HANDLE file, unsigned char *line, unsigned char *buffer,
					unsigned int *pos, unsigned int *read, unsigned int *asciiend)	{
unsigned char	*linechar = NULL, *actchar = buffer + *pos;
unsigned int	comment = 1, linelength = 0;
	
	while(comment)	{
		linechar = line;
		linelength = 0;
		do {
			if( (*pos) == (*read) )	{
				*pos = 0;
				if(( (*read) = _ReadFile(file,buffer,1024)) == 0) return(0);
				actchar = buffer;
				*asciiend += *read;
			}
			if (linelength++ == GLIDE_3DF_LINE_LENGTH) return(0);
			(*pos)++;
		} while((*(linechar++) = *(actchar++)) != 0x0A);
		if(*line != '#') comment = 0;
	}
	*(linechar-1) = 0x0;
	return(linelength);
}


/* segédrutin: két len hosszú vagy 0-ra végzôdô stringet hasonlit össze */
int GlideComp3DFstring(const unsigned char *src, const unsigned char *dst, unsigned int len)	{
	while(len--)	{
		if ( (*src == *dst) && (*src == 0x0) ) break;
		if ( *(src++) != *(dst++) ) return(0);
	}
	return(1);
}


/* segédrutin: egy adott pozíciótól kezdve egy számot olvas be */
int GlideGet3DFnumber(unsigned char *src, unsigned int *number)	{
unsigned int	len = 0;
	
	*number = 0;
	while( ((*src) >= '0') && ((*src) <= '9') )	{
		*number = (*number)*10 + (*(src++) - '0');
		len++;
	}
	return(len);
}


/* segédrutin: visszaadja, hogy egy szám 2-nek hányadik hatványa, vagy ha nem 2 hatvány, */
/* akkor 0-t.																				 */
unsigned int _inline GlideGet3DFpower(unsigned int number)	{
int		i=1;
	
	while(!(number & 1)) {
		number >>= 1;
		i++;
	}
	if (number != 1) return(0);
		else return(i);
}


/* segédrutin: egy fileból kiszedi az infót */
unsigned int GlideGet3DFinfo(HANDLE file, Gu3dfInfo *info)	{
unsigned char	buffer[1024], line[GLIDE_3DF_LINE_LENGTH];
unsigned int	buffpos=0, read=0, number, len, linepos, asciiend;
short			ncc[2*12+16];

	asciiend = 0;
/* Elsô sor beolvasása, és a "3df v%d.%d" formátum ellenôrzése */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	if (!GlideComp3DFstring(line, (unsigned char *) "3df v", 5)) return(0);
	if (!(len=GlideGet3DFnumber(line + 5, &number))) return(0);
	if (*(line + (linepos = len)+5) != '.') return(0);
	if (!(len=GlideGet3DFnumber(line+6+len, &number))) return(0);
	if (*(line+linepos+len+6) != 0x0) return(0);
/* Második sor beolvasása, és a textúraformátum kiderítése */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	for(number = 0; number < 13; number++)	{
		if (GlideComp3DFstring(line, (unsigned char *) textfmtstr[number], 0xFFFFFFFF))	{
			info->header.format = textfmtconst[number];
			break;
		}
	}
	if (number == 13) return(0);

/* Harmadik sor beolv, és a LOD range megállapítása */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	if (!GlideComp3DFstring(line, (unsigned char *) "lod range: ", 11)) return(0);
	if (!(len = GlideGet3DFnumber(line+11, &number))) return(0);
	if (!(linepos = GlideGet3DFpower(number))) return(0);
	info->header.small_lod = textlods[linepos-1];
	if (*(line+len+11) != ' ') return(0);
	linepos = len;
	if (!(len = GlideGet3DFnumber(line+len+12, &number))) return(0);
	if (*(line+linepos+len+12) != 0x0) return(0);
	if (!(linepos = GlideGet3DFpower(number))) return(0);
	info->header.large_lod = textlods[linepos-1];
	
/* Negyedik sor beolv, és az aspect ratio megállapítása */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	if (!GlideComp3DFstring(line, (unsigned char *) "aspect ratio: ", 14)) return(0);
	for(number = 0; number < 7; number++)	{
		if (GlideComp3DFstring(line+14, (unsigned char *) textasprats[number], 0xFFFFFFFF))	{
			info->header.aspect_ratio = textaspratconst[number];
			break;
		}
	}
	if (number == 7) return(0);
	linepos = GlideGetXY(info->header.large_lod, info->header.aspect_ratio);
	info->header.width = linepos >> 16;
	info->header.height = linepos & 0xFFFF;
	info->mem_required = grTexCalcMemRequired (info->header.small_lod,
						   					   info->header.large_lod,
											   info->header.aspect_ratio,
											   info->header.format);
	switch(info->header.format)	{
		case GR_TEXFMT_P_8:
		case GR_TEXFMT_AP_88:
			_SetFilePos(file, asciiend-1024+buffpos, FILE_BEGIN);
			if (_ReadFile(file, &info->table, sizeof(GuTexPalette)) != sizeof(GuTexPalette)) return(0);
			_asm	{
				mov edx,info
				lea edx,[edx].table
				mov ecx,256
		_gu3dfLoadPaletteConvert:
				mov eax,[edx]
				bswap eax
				add edx,4h				
				mov [edx-4],eax
				dec ecx
				jne _gu3dfLoadPaletteConvert
			}
			asciiend += sizeof(GuTexPalette);
			break;
		case GR_TEXFMT_YIQ_422:
		case GR_TEXFMT_AYIQ_8422:
			_SetFilePos(file, asciiend-1024+buffpos, FILE_BEGIN);
			if (_ReadFile(file, ncc, 80) != 80) return(0);
			_asm	{
				mov edx,info
				lea edx,[edx].table
				lea	ebx,ncc
				mov ecx,16
		_gu3dfLoadNCCConvertY:
				mov ax,[ebx]
				mov [edx],ah
				inc edx
				add ebx,2
				loop _gu3dfLoadNCCConvertY
				mov ecx,2*12
		_gu3dfLoadNCCConvertIQ:
				mov ax,[ebx]
				xchg al,ah
				mov [edx],ax
				add ebx,2
				add edx,2
				loop _gu3dfLoadNCCConvertIQ
			}
			asciiend += 80;
			break;
		default:
			break;
	}
	return(asciiend-1024+buffpos);
}


FxBool EXPORT gu3dfGetInfo(const char *filename, Gu3dfInfo *info) {
HANDLE	file;
int		r;
	
 	DEBUG_BEGINSCOPE("gu3dfGetInfo", DebugRenderThreadId);
	
	LOG(0,"\ngu3dfGetInfo(filename=\"%s\", info=%p)",filename,info);
	if ((file = _OpenFile(/*(unsigned char *)*/ filename)) == INVALID_HANDLE_VALUE) return(FXFALSE);
	r = GlideGet3DFinfo (file, info);
	_CloseFile(file);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(r ? FXTRUE : FXFALSE);
}


FxBool EXPORT gu3dfLoad(const char *filename, Gu3dfInfo *info)	{
HANDLE			file;
unsigned int	imagepos;
void			*image;

	DEBUG_BEGINSCOPE("gu3dfLoad", DebugRenderThreadId);
	
	LOG(0,"\ngu3dfLoad(filename=\"%s\", info=%p)",filename,info);
	if ((file = _OpenFile(/*(unsigned char*)*/ filename)) == INVALID_HANDLE_VALUE) return(FXFALSE);
	if (!(imagepos = GlideGet3DFinfo(file, info)))	{
		_CloseFile(file);
		return(FXFALSE);
	}
	_SetFilePos(file, imagepos, FILE_BEGIN);
	if (_ReadFile(file, info->data, info->mem_required) < (((signed int) info->mem_required) - 7))	{
		GetLastError();
		_CloseFile(file);
		return(FXFALSE);
	}
	image = info->data;
	switch(info->header.format)	{
		case GR_TEXFMT_ARGB_8332:
		case GR_TEXFMT_AYIQ_8422:
		case GR_TEXFMT_RGB_565:
		case GR_TEXFMT_ARGB_1555:
		case GR_TEXFMT_ARGB_4444:
		case GR_TEXFMT_ALPHA_INTENSITY_88:
		case GR_TEXFMT_AP_88:
			_asm	{
				mov eax,info
				mov ecx,[eax].mem_required
				shr	ecx,2
				mov edx,image
		_gu3dfLoadConvert:
				mov eax,[edx]
				bswap eax
				add edx,4h
				ror eax,16
				mov [edx-4],eax
				dec ecx
				jne _gu3dfLoadConvert
			}
		default:;
	}
	_CloseFile(file);

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*------------------------------- Chroma keying függvények ----------------------------------*/

void EXPORT grChromakeyMode(GrChromakeyMode_t mode)	{

	DEBUG_BEGINSCOPE("grChromakeyMode", DebugRenderThreadId);
	
	LOG(0,"\ngrChromakeyMode(mode=%s)",chromakeymode_t[mode]);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	DWORD ckstate = (mode == GR_CHROMAKEY_ENABLE) ? TRUE : FALSE;
	if (ckstate != (DWORD) astate.colorKeyEnable)	{
		DrawFlushPrimitives();
		astate.colorKeyEnable = (unsigned char) ckstate;
		
		updateFlags |= UPDATEFLAG_COLORKEYSTATE;
	}
	GlideUpdateConstantColorKeyingState();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grChromakeyValue(GrColor_t value)	{
	
	DEBUG_BEGINSCOPE("grChromakeyValue", DebugRenderThreadId);
	
	LOG(0,"\ngrChromakeyValue(value=%0x)",value);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	if (value == astate.colorkey)
		return;
	
	LOG(0,"\n	grChromakeyValue(value=%0x), in ARGB:", value, GlideGetColor (value));

	astate.colorkey = value;
	if (astate.colorKeyEnable)
		DrawFlushPrimitives();

	/*	DirectX7 probléma native colorkeying esetén:
		A chromakey comparing 24 biten történik, ezért az alfát nem veszi figyelembe.
		A natív DirectX-es colorkeying viszont minden komponenst figyelembe vesz:
		- ha a textúra alfa nélküli, akkor mindegy
		- ha a textúra alfás, de alfa nélkülirõl lett konvertálva, akkor a konstans alfa 0xFF, ezért ezt itt is beállítjuk
		- ha alfás 3dfx-es textúráról van szó, akkor az esetleg eltérõ alfa értékek miatt a natív ck nem fog mûködni :( */
	DWORD colorKey = GlideGetColor (value) | 0xFF000000;

	dxTexFormats[Pf16_ARGB4444].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_ARGB4444].pixelFormat);
	dxTexFormats[Pf16_ARGB1555].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_ARGB1555].pixelFormat);
	dxTexFormats[Pf16_RGB555].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_RGB555].pixelFormat);
	dxTexFormats[Pf16_RGB565].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_RGB565].pixelFormat);
	dxTexFormats[Pf32_ARGB8888].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf32_ARGB8888].pixelFormat);
	dxTexFormats[Pf32_RGB888].colorKeyInTexFmt = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf32_RGB888].pixelFormat);

	if (dxTexFormats[Pf8_P8].isValid)	{
		colorKeyPal = TexGetTexIndexValue(0, colorKey, &palette);
		colorKeyNcc0 = TexGetTexIndexValue(1, colorKey, &nccTable0);
		colorKeyNcc1 = TexGetTexIndexValue(1, colorKey, &nccTable1);
	}
	GlideUpdateConstantColorKeyingState();
	updateFlags |= UPDATEFLAG_COLORKEYSTATE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/*-------------------------------------------------------------------------------------------*/
/*----------------------------- Textúra-utililty függvények ---------------------------------*/

#ifndef GLIDE3

/* Memóriát allokál egy adott textúrának. Az érvénytelen handle 0xFFFFFFFF */
GrMipMapId_t EXPORT guTexAllocateMemory(GrChipID_t tmu,
										FxU8 evenOddMask,
										int width, int height,
										GrTextureFormat_t format,
										GrMipMapMode_t mmMode,
										GrLOD_t smallLod, GrLOD_t largeLod,
										GrAspectRatio_t aspectRatio,
										GrTextureClampMode_t sClampMode,
										GrTextureClampMode_t tClampMode,
										GrTextureFilterMode_t minFilterMode,
										GrTextureFilterMode_t magFilterMode,
										float lodBias,
										FxBool lodBlend )
{
	DEBUG_BEGINSCOPE("guTexAllocateMemory",DebugRenderThreadId);
	
	LOG(0,"\nguTexAllocateMemory(tmu=%d, evenOddMask=%s, width=%d, height=%d, format=%s, mmMode=%s, \
		   smallLod=%s, largeLod=%s, aspectRatio=%s, sClampMode=%s, tClampMode=%s, \
		   minFilterMode=%s, magFilterMode=%s, lodBias=%f, lodBlend=%s)", \
		   tmu, evenOdd_t[evenOddMask], width, height, tformat_t[format], \
		   mipmapmode_t[mmMode], lod_t[smallLod], lod_t[largeLod], \
		   aspectr_t[aspectRatio], clampmode_t[sClampMode], clampmode_t[tClampMode], \
		   filtermode_t[minFilterMode], filtermode_t[magFilterMode], \
		   lodBias, fxbool_t[lodBlend]);

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return (GR_NULL_MIPMAP_HANDLE);
	
	if (tmu)
		return (GR_NULL_MIPMAP_HANDLE);

	if (mipmapinfos == NULL) {
		if ((mipmapinfos = (GrMipMapInfo *) _GetMem(MAX_UTIL_TEXTURES * sizeof(GrMipMapInfo))) == NULL)
			return(GR_NULL_MIPMAP_HANDLE);
	}
	if (mmidIndex == (MAX_UTIL_TEXTURES-1))
		return(GR_NULL_MIPMAP_HANDLE);
	
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmidIndex;

	unsigned int xy = GlideGetXY(largeLod, aspectRatio);
	if ((xy >> 16) != (unsigned int) width)
		return (GR_NULL_MIPMAP_HANDLE);
	if ((xy & 0xFFFF) != (unsigned int) height)
		return (GR_NULL_MIPMAP_HANDLE);
	
	mipmapInfo->sst = 0;
	mipmapInfo->valid = FXTRUE;
	mipmapInfo->width = width;
	mipmapInfo->height = height;
	mipmapInfo->aspect_ratio = aspectRatio;
	mipmapInfo->data = NULL;
	mipmapInfo->format = format;
	mipmapInfo->mipmap_mode = mmMode;
	mipmapInfo->magfilter_mode = magFilterMode;
	mipmapInfo->minfilter_mode = minFilterMode;
	mipmapInfo->s_clamp_mode = sClampMode;
	mipmapInfo->t_clamp_mode = tClampMode;
	mipmapInfo->tLOD = 'DEGE';
	mipmapInfo->tTextureMode = 'DEGE';
	mipmapInfo->lod_bias = *((FxU32 *) &lodBias);
	mipmapInfo->lod_min = smallLod;
	mipmapInfo->lod_max = largeLod;
	mipmapInfo->tmu = 0;
	mipmapInfo->odd_even_mask = evenOddMask;
	mipmapInfo->tmu_base_address = utilMemFreeBase;
	mipmapInfo->trilinear = lodBlend;
	utilMemFreeBase += grTexCalcMemRequired (smallLod, largeLod, aspectRatio, format);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(mmidIndex++);
}


FxBool EXPORT guTexChangeAttributes(GrMipMapId_t mmid,
									int width, int height,
									GrTextureFormat_t format,
									GrMipMapMode_t mmMode,
									GrLOD_t smallLod, GrLOD_t largeLod,
									GrAspectRatio_t aspectRatio,
									GrTextureClampMode_t sClampMode,
									GrTextureClampMode_t tClampMode,
									GrTextureFilterMode_t minFilterMode,
									GrTextureFilterMode_t magFilterMode)
{
	DEBUG_BEGINSCOPE("guTexChangeAttributes", DebugRenderThreadId);
	
	LOG(0,"\nguTexChangeAttributes(mmid=%0x, width=%d, height=%d, format=%s, mmMode=%s, \
		   smallLod=%s, largeLod=%s, aspectRatio=%s, sClampMode=%s, tClampMode=%s, \
		   minFilterMode=%s, magFilterMode=%s)", \
		   mmid, width, height, tformat_t[format], \
		   mipmapmode_t[mmMode], lod_t[smallLod], lod_t[largeLod], \
		   aspectr_t[aspectRatio], clampmode_t[sClampMode], clampmode_t[tClampMode], \
		   filtermode_t[minFilterMode], filtermode_t[magFilterMode]);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return (FXFALSE);
	
	if ((mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex))
		return (FXFALSE);
	
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmid;
	if (width != -1) mipmapInfo->width = width;
	if (height != -1) mipmapInfo->height = height;
	if (format != -1) mipmapInfo->format = format;
	if (mmMode != -1) mipmapInfo->mipmap_mode = mmMode;
	if (smallLod != -1) mipmapInfo->lod_min = smallLod;
	if (largeLod != -1) mipmapInfo->lod_max = largeLod;
	if (aspectRatio != -1) mipmapInfo->aspect_ratio = aspectRatio;
	if (sClampMode != -1) mipmapInfo->s_clamp_mode = sClampMode;
	if (tClampMode != -1) mipmapInfo->t_clamp_mode = tClampMode;
	if (minFilterMode != -1) mipmapInfo->minfilter_mode = minFilterMode;
	if (magFilterMode != -1) mipmapInfo->magfilter_mode = magFilterMode;

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}


void EXPORT guTexDownloadMipMap(GrMipMapId_t mmid, const void *src, const GuNccTable *nccTable )
{
	DEBUG_BEGINSCOPE("guTexDownloadMipMap",DebugRenderThreadId);
	
	LOG(0,"\nguTexDownloadMipMap(mmid=%0x, src=%p, nccTable=%p)",mmid,src,nccTable);

	if ((runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN)
		return;
	
	if ((mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex))
		return;
	
	if (mmid == astate.actmmid)
		DrawFlushPrimitives();
	
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmid;
	
	GrTexInfo	info;
	info.smallLod = mipmapInfo->lod_min;
	info.largeLod = mipmapInfo->lod_max;
	info.format = mipmapInfo->format;
	info.aspectRatio = mipmapInfo->aspect_ratio;
	info.data = mipmapInfo->data = (void *) src;

	grTexDownloadMipMap (0, mipmapInfo->tmu_base_address, GR_MIPMAPLEVELMASK_BOTH, &info);

	if (((mipmapInfo->format == GR_TEXFMT_YIQ_422) || (mipmapInfo->format == GR_TEXFMT_AYIQ_8422)) && (nccTable != NULL))
		TexCopyNCCTable(&mipmapInfo->ncc_table, (GuNccTable *) nccTable);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src )
{
	DEBUG_BEGINSCOPE("guTexDownloadMipMapLevel",DebugRenderThreadId);

	LOG(0,"\nguTexDownloadMipMapLevel(mmid=%0x, lod=%s, src=%p)",mmid,lod_t[lod],src);

	if ((runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN)
		return;
	
	if ((mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex))
		return;

	if (mmid == astate.actmmid)
		DrawFlushPrimitives();
	
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmid;
	
	DEBUGCHECK(0,( (lod < mipmapInfo->lod_min) || (lod > mipmapInfo->lod_max) ), \
		         "\n   Warning: guTexDownloadMipMapLevel: specified lod is out of range!");
	DEBUGCHECK(1,( (lod < mipmapInfo->lod_min) || (lod > mipmapInfo->lod_max) ), \
		         "\n   Figyelmeztetés: guTexDownloadMipMapLevel: a megadott lod nem esik a megfelelõ tartományba!");

	unsigned int lodsize = grTexCalcMemRequired (lod, lod, mipmapInfo->aspect_ratio, mipmapInfo->format);
	grTexDownloadMipMapLevel (0, mipmapInfo->tmu_base_address, lod,mipmapInfo->lod_max, mipmapInfo->lod_min,
							  mipmapInfo->format, mipmapInfo->odd_even_mask, (void *) *src);

	*src = ((char *) (*src)) + lodsize;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


GrMipMapId_t EXPORT guTexGetCurrentMipMap ( GrChipID_t tmu )
{
	LOG(0,"\nguTexGetCurrentMipMap(tmu=%d)",tmu);

	return (tmu == 0) ? astate.actmmid : GR_NULL_MIPMAP_HANDLE;
}


unsigned int EXPORT guTexGetMipMapInfo( GrMipMapId_t mmid ) {

	LOG(0,"\nguTexGetMipMapInfo(mmid=%0x)",mmid);
	
	return (mmid >= mmidIndex) ? (unsigned int) NULL : (unsigned int) (mipmapinfos + mmid);
}


//DOS version of guTexGetMipMapInfo
int GlideCopyMipMapInfo(GrMipMapInfo *dst, GrMipMapId_t mmid)
{
	if ((mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex))
		return(0);
	
	CopyMemory(dst, mipmapinfos + mmid, sizeof(GrMipMapInfo));
	return(1);
}


void EXPORT guTexMemReset( void )
{

	DEBUG_BEGINSCOPE("guTexMemReset", DebugRenderThreadId);
	
	LOG(0,"\nguTexMemReset()");

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	utilMemFreeBase = 0;
	mmidIndex = 0;
	astate.actmmid = GR_NULL_MIPMAP_HANDLE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


FxU32 EXPORT guTexMemQueryAvail( GrChipID_t tmu )
{

	return (tmu == 0) ? (grTexMaxAddress (0) - utilMemFreeBase) : 0;
}


void EXPORT guTexSource( GrMipMapId_t mmid )
{
	DEBUG_BEGINSCOPE("guTexSource",DebugRenderThreadId);
	
	LOG(0,"\nguTexSource(mmid=%0x)",mmid);
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;

	if ((mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex))
		return;

	DrawFlushPrimitives();
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmid;

	GrTexInfo info;
	info.smallLod = mipmapInfo->lod_min;
	info.largeLod = mipmapInfo->lod_max;
	info.format = mipmapInfo->format;
	info.aspectRatio = mipmapInfo->aspect_ratio;
	info.data = mipmapInfo->data;
	grTexSource(0, mipmapInfo->tmu_base_address, mipmapInfo->odd_even_mask, &info);

	astate.actmmid = mmid;
	
	if ( (mipmapInfo->format == GR_TEXFMT_YIQ_422) || (mipmapInfo->format == GR_TEXFMT_AYIQ_8422) )	{
		grTexDownloadTable(0, (actncctable == &nccTable0) ? GR_TEXTABLE_NCC0 : GR_TEXTABLE_NCC1, &mipmapInfo->ncc_table);
	}
	grTexClampMode(0, mipmapInfo->s_clamp_mode, mipmapInfo->t_clamp_mode);
	grTexFilterMode(0, mipmapInfo->minfilter_mode, mipmapInfo->magfilter_mode);
	grTexMipMapMode(0, mipmapInfo->mipmap_mode, mipmapInfo->trilinear);
	grTexLodBiasValue(0, *((float *) &mipmapInfo->lod_bias) );
	updateFlags |= UPDATEFLAG_SETPALETTE;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


unsigned int GlideGetUTextureSize(GrMipMapId_t mmid, GrLOD_t lod, int lodvalid)
{
	unsigned int retVal;

	DEBUG_BEGINSCOPE("GlideGetUTextureSize",DebugRenderThreadId);

	if ((mmid >= mmidIndex) || (mmid == GR_NULL_MIPMAP_HANDLE))
		return(0);
	
	GrMipMapInfo* mipmapInfo = mipmapinfos + mmid;

	retVal = lodvalid ? grTexCalcMemRequired (lod, lod ,mipmapInfo->aspect_ratio,mipmapInfo->format) :
					    grTexCalcMemRequired (mipmapInfo->lod_min, mipmapInfo->lod_max, mipmapInfo->aspect_ratio, mipmapInfo->format);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return retVal;
}

#endif


void EXPORT guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )
{
	
	LOG(0,"\nguTexCombineFunction(tmu=%0x, func=%s)",tmu,texturecombinefnc_t[func]);
	
	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN)
		return;
	
	if (tmu != 0)
		return;
	
	switch(func)	{
		case GR_TEXTURECOMBINE_ZERO:
			grTexCombine(0, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
						 GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
						 FXFALSE, FXFALSE);
			break;
		case GR_TEXTURECOMBINE_ONE:
			grTexCombine(0,GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
						 GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
						 FXTRUE, FXTRUE);
			break;
		default:
			grTexCombine(0,GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
		 				 GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
						 FXFALSE, FXFALSE);
			break;
	}	
}


void EXPORT grTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func ) {
	guTexCombineFunction(tmu,func);
}

/*-------------------------------------------------------------------------------------------*/
/*------------------------------------- Init rutinok ----------------------------------------*/


/* Ez a rutin lekérdezi a hardver által támogatott textúraformátumokat, létrehozza */
/* a konstans palettákat. */
int TexQuerySupportedFormats()
{
	DEBUG_BEGINSCOPE("TexQuerySupportedFormats", DebugRenderThreadId);

	ZeroMemory (&dxTexFormats, NumberOfDXTexFormats * sizeof(NamedFormat));
		
	dxTexFormats[Pf16_ARGB4444].missing = _16_ARGB4444;
	dxTexFormats[Pf16_ARGB1555].missing = _16_ARGB1555;
	dxTexFormats[Pf16_RGB555].missing = _16_RGB555;
	dxTexFormats[Pf16_RGB565].missing = _16_RGB565;
	dxTexFormats[Pf32_ARGB8888].missing = _32_ARGB8888;
	dxTexFormats[Pf32_RGB888].missing = _32_RGB888;
	dxTexFormats[Pf8_P8].missing = _8_P8;

	unsigned int i;
	for (i = 0; i < NumberOfDXTexFormats; i++)
		dxTexFormats[i].dxTexFormatIndex = (DXTextureFormats) i;

	/* Generating constant palettes */
	unsigned int constARGBPalettes[6*256];

	/* RGB332, paletta max alphával */
	for(i = 0; i < 256; i++)
		constARGBPalettes[i] = 0xFF000000 | ((i & 0xE0) << 16) | ((i & 0xE0) << 13) | ((i & 0xC0) << 10) |
								((i & 0x1C) << 11) | ((i & 0x1C) << 8) | ((i & 0x18) << 5) |
								((i & 3) << 6) | ((i & 3) << 4) | ((i & 3) << 2) | (i & 3);
	
	unsigned int val;
	/* A8 */
	for(val = 0, i = 256; i < 512; i++) {
		constARGBPalettes[i] = val;
		val += 0x01010101;
	}
	
	/* I8, paletta max alphával */
	for(val = 0xFF000000, i = 512; i < 768; i++) {
		constARGBPalettes[i] = val;
		val += 0x00010101;
	}
	
	/* AI44 */
	for(i = 768; i < 1024; i++) {
		val = ((i & 0xF0) << 8) | ((i & 0xF0) << 4) | ((i & 0x0F) << 4) | (i & 0x0F);
		constARGBPalettes[i] = ((val & 0xFF00) << 16) | ((val & 0x00FF) << 16) | ((val & 0x00FF) << 8) | (val & 0x00FF);
	}
	
	/* ARGB8332, paletta nulla alphával */
	for(i = 1024; i < 1280; i++)
		constARGBPalettes[i] = constARGBPalettes[i - 1024] & 0xFFFFFF;
	
	/* AI88, paletta nulla alphával */
	for(i = 1280; i < 1536; i++)
		constARGBPalettes[i] = constARGBPalettes[i - (1280 - 512)] & 0x00FFFFFF;

	rendererTexturing->GetSupportedTextureFormats (dxTexFormats);

	if (config.Flags & CFG_TEXTURES16BIT) {
		dxTexFormats[Pf8_P8].isValid = dxTexFormats[Pf32_ARGB8888].isValid = dxTexFormats[Pf32_RGB888].isValid = 0;
	}

	if (config.Flags & CFG_TEXTURES32BIT) {
		dxTexFormats[Pf8_P8].isValid = dxTexFormats[Pf16_ARGB1555].isValid = dxTexFormats[Pf16_ARGB4444].isValid =
		dxTexFormats[Pf16_RGB555].isValid = dxTexFormats[Pf16_RGB565].isValid = 0;
	}
	
  	for (i = 0; i < NumberOfDXTexFormats; i++) {
		if (dxTexFormats[i].isValid && (i != Pf8_P8)) {
			if ( (dxTexFormats[i].constPalettes = (unsigned int *) _GetMem(6 * 256 * sizeof(unsigned int))) != NULL)	{
				unsigned int j;
				for(j = 0; j < 6; j++)
					MMCConvARGBPaletteToPixFmt(constARGBPalettes + j*256,
											   dxTexFormats[i].constPalettes + j*256,
											   0, 256, &dxTexFormats[i].pixelFormat);
			}
		}
	}

	DEBUGCHECK (0, !dxTexFormats[Pf8_P8].isValid, "\n   Warning: TexQuerySupportedFormats: Paletted textures are not supported! \
												   \n                                      Is it an nVidia card? If yes, why does not it support them??? Argh...");
	DEBUGCHECK (1, !dxTexFormats[Pf8_P8].isValid, "\n   Figyelmeztetés: TexQuerySupportedFormats: A palettás textúrákat a hardver nem támogatja! \
												   \n                                             Ez egy nVidia kártya? Ha igen, miért nem támogatja õket??? Ahh...");

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


int TexInitAtGlideInit()
{

	DEBUG_BEGINSCOPE("TexInitAtGlideInit",DebugRenderThreadId);
	
	if (config.Flags & CFG_PERFECTTEXMEM)	{
		if (textureMem == NULL) {
			if ( (textureMem = (unsigned char *) _GetMem(config.TexMemSize)) == NULL ) {

				DEBUGCHECK (0, textureMem == NULL, "\n   Error: TexInit: Initializing Glide: Cannot alloc virtual texture memory!");
				DEBUGCHECK (1, textureMem == NULL, "\n   Hiba: TexInit: Nem tudom leallokálni a virtuális textúramemóriát!");
				
				return(0);
			}
			ZeroMemory(textureMem, config.TexMemSize);
		}
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


void TexCleanUpAtShutDown()
{

	DEBUG_BEGINSCOPE("TexCleanUpAtShutDown", DebugRenderThreadId);

	if ( (config.Flags & CFG_PERFECTTEXMEM) && (textureMem) )	{
		_FreeMem(textureMem);
		textureMem = NULL;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* A grWinSStOpen-bôl init közben meghívódó init */	
int TexInit()
{
	
	DEBUG_BEGINSCOPE("TexInit", DebugRenderThreadId);

	//rendererTexturing = renderer.GetRendererTexturing ();

	rendererTexturing->Init (config.Flags2 & CFG_MANAGEDTEXTURES);

	texindex = NULL;
	mipMapTempArea = NULL;

	unsigned int i;
	for(i = 0; i < 3; i++)
		rendererTexturing->SetTextureAtStage (i, astate.lpDDTex[i] = NULL);

	if (!TexQuerySupportedFormats())
		return(0);
	
	/* textúraindextáblázatnak hely foglalása */	
	if ( (texindex = (TexCacheEntry **) _GetMem(MAX_TEXNUM * sizeof(TexCacheEntry *))) == NULL)
		return(0);
	
	/* */
	perfectEmulating = (config.Flags & CFG_PERFECTTEXMEM) ? 1 : 0;
	
	if (config.Flags2 & CFG_AUTOGENERATEMIPMAPS)	{
		if ( (mipMapTempArea = (unsigned char *) _GetMem(128*128*2+64*64*2)) == NULL )
			return(0);
	}
		
	ZeroMemory(texInfo, TEXPOOL_NUM * sizeof(TexCacheEntry *));
	ZeroMemory(texNumInPool, TEXPOOL_NUM * sizeof(unsigned int));

	timin = timax = mmidIndex = 0;
	/*astate.acttex =*/ ethead = NULL;
	//astate.actmmid = GR_NULL_MIPMAP_HANDLE;
	apMultiTexturing = (dxTexFormats[Pf8_P8].isValid) && (rendererGeneral->GetNumberOfTextureStages () >= 2) ? 1 : 0;

	/* Nincs aktuális textúra (NULL) */
	texMemFree = config.TexMemSize;
	colorKeyPal = colorKeyNcc0 = colorKeyNcc1 = 0;

	/* A speciális nulltextúra létrehozása és bitmapjének feltöltése */
	TexCreateSpecialTextures ();

	grTexFilterMode(0, GR_TEXTUREFILTER_POINT_SAMPLED, GR_TEXTUREFILTER_POINT_SAMPLED);
	grTexClampMode(0, GR_TEXTURECLAMP_WRAP, GR_TEXTURECLAMP_WRAP);	
	grTexCombine(0,GR_COMBINE_FUNCTION_LOCAL,GR_COMBINE_FACTOR_NONE,
				 GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE);

#ifndef GLIDE3
	mipmapinfos = NULL;
	utilMemFreeBase = 0;
#endif

	switch (GetCKMethodValue (config.Flags)) {
		case CFG_CKMAUTOMATIC:
			rendererGeneral->GetCkMethodPreferenceOrder (ckmPreferenceOrder);
			break;

		case CFG_CKMALPHABASED:
			ckmPreferenceOrder[0] = RendererGeneral::AlphaBased;
			ckmPreferenceOrder[1] = RendererGeneral::Disabled;
			break;

		case CFG_CKMNATIVE:
			ckmPreferenceOrder[0] = RendererGeneral::Native;
			ckmPreferenceOrder[1] = RendererGeneral::Disabled;
			break;

		case CFG_CKMNATIVETNT:
			ckmPreferenceOrder[0] = RendererGeneral::NativeTNT;
			ckmPreferenceOrder[1] = RendererGeneral::Disabled;
			break;
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


/* A grWinSStClose-ból meghívódó cleanup */
void TexCleanUp()
{
	
	DEBUG_BEGINSCOPE("TexCleanUp", DebugRenderThreadId);

	unsigned int i;
	for (i = 0; i <=3; i++)
		rendererTexturing->SetTextureAtStage (i, NULL);

	/* A textúrák eldobálása */
	for(i = timin; i < timax; i++)	{

#ifdef	WRITETEXTURESTOBMP
		WTEX(texindex[i]);
#endif

		GlideReleaseTexture(texindex[i]);
	}

	rendererTexturing->DeInit ();

	if (texindex != NULL) _FreeMem(texindex);

#ifndef GLIDE3
	if (mipmapinfos != NULL) _FreeMem(mipmapinfos);
	mipmapinfos = NULL;
#endif

	if (mipMapTempArea) _FreeMem(mipMapTempArea);

	for(i = 0; i < NumberOfDXTexFormats; i++)	{
		if (dxTexFormats[i].constPalettes != NULL) _FreeMem(dxTexFormats[i].constPalettes);
	}
	
	for(i = 0; i < TEXPOOL_NUM; i++) if (texInfo[i] != NULL) _FreeMem(texInfo[i]);
	texindex = NULL;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


#ifdef	WRITETEXTURESTOBMP

/* Csak fejlesztéshez: ez a függvény egy adott textúrát ír ki bmp-be */
void WTEX(TexCacheEntry *tinfo)	{
BITMAPINFO				*bitmap;
BITMAPFILEHEADER		bmpheader;
unsigned char			*p;
unsigned short			*q;
unsigned int			*shadow;
HANDLE					hfile;
int						failed;
TextureType				texture;
char					tname[256];
_pixfmt					pixelf;
_pixfmt					srcPixFmt;
int						i;
void*					texPtr;
unsigned int			texPitch;

	pixelf.BitPerPixel = 32;
	pixelf.RPos = 16;
	pixelf.RBitCount = 8;
	pixelf.GPos = 8;
	pixelf.GBitCount = 8;
	pixelf.BPos = 0;
	pixelf.BBitCount = 8;
	pixelf.ABitCount = pixelf.APos = 0;
	sprintf((char *) tname, "e:\\dgvt\\%0x.bmp",tinfo->startAddress);
	bitmap = (BITMAPINFO*) _GetMem(sizeof(BITMAPINFO)+1024+tinfo->x*tinfo->y*4);
	ZeroMemory(bitmap, sizeof(BITMAPINFO));
	bitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap->bmiHeader.biWidth = tinfo->x;
	bitmap->bmiHeader.biHeight = tinfo->y;
	bitmap->bmiHeader.biPlanes = 1;
	bitmap->bmiHeader.biBitCount = (unsigned short) 32;
	bitmap->bmiHeader.biCompression = BI_RGB;
	bitmap->bmiHeader.biSizeImage = 0;
	bitmap->bmiHeader.biClrUsed = 0;
	bitmap->bmiHeader.biClrImportant = 0;
	p = (unsigned char *) bitmap;
	p += sizeof(BITMAPINFO); //+1024;
	/*if (tinfo->x*tinfo->dxformat->pixelformat.BitPerPixel == 8) {
		CopyMemory (p, &tinfo->palettedData->palette, 1024);
		TexConvertNCCToABGRPalette((GuNccTable *) p, (PALETTEENTRY *) p);
		p+=1024;
	}*/

	q = (unsigned short *) p;
	ZeroMemory (&srcPixFmt, sizeof (_pixfmt));
	srcPixFmt.BitPerPixel = 8;

	_pixfmt* convSrcPixFmt = &tinfo->dxFormat->pixelFormat;
	if (tinfo->auxPFormat != NULL) {
		texture = tinfo->texAuxPTextures[0];
		convSrcPixFmt = &tinfo->auxPFormat->pixelFormat;
	} else {
		texture = tinfo->texBase[0];
		if (tinfo->texAP88AlphaChannel[0] != NULL) {
			convSrcPixFmt = &srcPixFmt;
		}
	}

	if (texture)	{
		if ((texPtr = rendererTexturing->GetPtrToTexture (texture, 0, &texPitch)) != NULL) {
			MMCConvertAndTransferData(convSrcPixFmt,
									  &pixelf,
									  0, 0, 0,
									  texPtr, q,
									  tinfo->x, tinfo->y,
									  -((int) texPitch), tinfo->x*4,
									  (unsigned int *) &tinfo->palettedData->palette.data, NULL,
									  MMCIsPixfmtTheSame(&tinfo->dxFormat->pixelFormat, &pixelf) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
									  0);
			rendererTexturing->ReleasePtrToTexture (texture, 0);
		} else {
			DEBUGLOG (0, "\n   Error: WTEX: Cannot lock and read texture!");
			DEBUGLOG (1, "\n   Error: WTEX: Nem tudom zárolni és olvasni a textúrát!");
		}
	}

	hfile = _CreateFile(tname, FILE_ATTRIBUTE_NORMAL);
	if (hfile==INVALID_HANDLE_VALUE) failed=1; else failed=0;
	bmpheader.bfType='MB';
	bmpheader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+tinfo->x*tinfo->y*4;
	bmpheader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	
	bmpheader.bfReserved1 = bmpheader.bfReserved2 = 0;	
	if (_WriteFile(hfile, &bmpheader, sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER)) {
		failed=1;
	}
	if (_WriteFile(hfile, bitmap, sizeof(BITMAPINFOHEADER))!=sizeof(BITMAPINFOHEADER))	{
		failed=1;
	}
	if (_WriteFile(hfile, q, tinfo->x*tinfo->y*4)!= (int) (tinfo->x*tinfo->y*4) )	{
		failed=1;
	}

	_CloseFile(hfile);

	if (tinfo->texCkAlphaMask[0])	{
		sprintf((char*) tname,"e:\\dgvt\\%0x_mask.bmp",tinfo->startAddress);
		
		texture = tinfo->texCkAlphaMask[0];
		pixelf.ABitCount = 8;
		pixelf.APos = 24;
		if (texture)	{
			if ((texPtr = rendererTexturing->GetPtrToTexture (texture, 0, &texPitch)) != NULL) {
				MMCConvertAndTransferData(&tinfo->alphaDxFormat->pixelFormat, &pixelf,
								  0, 0, 0,
								  texPtr, q,
								  tinfo->x, tinfo->y,
								  -((int) texPitch), tinfo->x*4,
								  NULL, NULL,
								  MMCIsPixfmtTheSame(&tinfo->alphaDxFormat->pixelFormat, &pixelf) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
								  0);
				rendererTexturing->ReleasePtrToTexture (texture, 0);
			}
		}
		shadow=(unsigned int *) q;
		for(i=0;i<tinfo->x*tinfo->y;i++)	{
			if (shadow[i]!=0) shadow[i]=0xFFFFFFFF;
		}
		hfile = _CreateFile(tname,FILE_ATTRIBUTE_NORMAL);
		if (hfile==INVALID_HANDLE_VALUE) failed=1; else failed=0;
		bmpheader.bfType='MB';
		bmpheader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+tinfo->x*tinfo->y*4;
		bmpheader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	
		bmpheader.bfReserved1 = bmpheader.bfReserved2 = 0;	
		if (_WriteFile(hfile, &bmpheader, sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER)) {
			failed=1;
		}
		if (_WriteFile(hfile, bitmap, sizeof(BITMAPINFOHEADER))!=sizeof(BITMAPINFOHEADER))	{
			failed=1;
		}
		if (_WriteFile(hfile, q, tinfo->x*tinfo->y*4)!= (int) (tinfo->x*tinfo->y*4) )	{
			failed=1;
		}
		_CloseFile(hfile);
	}
	_FreeMem(bitmap);
}

#endif