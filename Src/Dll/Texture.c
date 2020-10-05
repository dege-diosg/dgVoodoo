/*--------------------------------------------------------------------------------- */
/* Texture.c - dgVoodoo Glide implementation, stuffs related to texturing           */
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

#include	"dgVoodooBase.h"
#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>
#include	<stdio.h>

#include	"dgVoodooConfig.h"
#include	"DDraw.h"
#include	"D3d.h"
#include	"Dgw.h"
#include	"Movie.h"
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
#define MAX_STAGETEXTURES		3

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

#define GLIDE_3DF_LINE_LENGTH	1024		/* 3DF file sorainak max. hossza */

/* Flagek a textúra-reload függvényhez */
#define RF_TEXTURE				0x1
#define RF_PALETTE				0x2
#define RF_NOPALETTEMAPCREATE	0x4

/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák  ..................................*/

/* Struktúra, amely egy adott típusú (nevû) pixelformátumot tárol */
typedef struct _NamedFormat {

	int					isValid;					/* formátum támogatott? */
	unsigned char		*missing;					/* pointer más formátumtípusokra, ha ez nem támogatott */
	_pixfmt				pixelFormat;				/* Pixelformátum leírása */
	unsigned int		*constPalettes;
	DDPIXELFORMAT		ddsdPixelFormat;			/* Pixelformátum DirectDraw-féle leírása */
	DDCOLORKEY			colorKey;					/* Az aktuális colorkey textúraformátumban */

} NamedFormat;


/* Palettás és NCC-táblás textúrák esetén egy átmeneti terület, ahol a paletta/NCC tábla is */
/* és a mipmap bitmapje is el van tárolva													*/
struct	PalettedData {

	LPDIRECTDRAWPALETTE lpDDPal;
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
	LPDIRECTDRAWSURFACE7	texBase[9];					/* 9 textúra handle-je a mipmaphez */
	LPDIRECTDRAWSURFACE7	texCkAlphaMask[9];			/* Opcionális árnyék-textúra (alfa alapú colorkeyinghez) */
	LPDIRECTDRAWSURFACE7	texAP88AlphaChannel[9];
	unsigned short			x,y;						/* szélesség, magasság */
														/* (Ne aspect ratiokkal kelljen faszolni) */
	unsigned short			mipMapCount;
	unsigned char			pool, flags;
	struct PalettedData		*palettedData;				/* Táblás textúrák táblái (paletta, ncc tábla) és */
														/* nyers (nem konvertált) bitmapje */
	NamedFormat				*dxFormat;					/* Textúra DirectX-pixelformátuma */
	NamedFormat				*alphaDxFormat;				/* Alpha-channel DirectX-formátuma */
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
static const unsigned char	*textasprats[]		= { "8 1", "4 1", "2 1", "1 1", "1 2", "1 4", "1 8" };

/* és a megfelelô konstansok */
static const GrAspectRatio_t textaspratconst[]	= {	GR_ASPECT_8x1, GR_ASPECT_4x1, GR_ASPECT_2x1, GR_ASPECT_1x1,
													GR_ASPECT_1x2, GR_ASPECT_1x4, GR_ASPECT_1x8 };

/* Egy adott mipmap lehetséges és pillanatnyilag támogatott formátumai */
static const unsigned char	*textfmtstr[]		= { "i8", "a8", "ai44", "yiq", "rgb332", "rgb565", "argb8332",
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
static unsigned char		multiBaseMode = 0;			/* multibase textúrázás */

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

NamedFormat		dxTexFormats[NumberOfDXTexFormats];

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

char			texCkCanGoWithAlphaTesting[9] = {0, 0, 2, 0, 2, 1, 0, 1, 1};	/* D3D-konstansok számozása 1-tõl kezdõdik */

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
void _stdcall	TexConvertNCCToARGBPalette(GuNccTable *, GuTexPalette *);

/* Egy AP88 formátumú textúrából multitextúrát készít */
void _stdcall	TexConvertToMultiTexture(_pixfmt *dstFormat,
										 void *src, void *dstPal, void *dstAlpha,
										 unsigned int x, unsigned int y,
										 int dstPitchPal, int dstPitchAlpha);

/* ARGB->3Dfxformat konverter (alfa alapú colorkeyinghez) */
unsigned long _stdcall TexGetTexelValueFromARGB(unsigned int ck, GrTextureFormat_t format, GuTexPalette *pal, GuNccTable *ncctable);

void _stdcall GlideGenerateMipMap(GrTextureFormat_t format,void *src, void *dst, unsigned int x, unsigned int y);


/*-------------------------------------------------------------------------------------------*/
/*........................... Belsõ függvények predeklarációja ..............................*/

TexCacheEntry*	TexFindTex(unsigned int startAddress, GrTexInfo *info);
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
int GlideHasTempStore(GrTextureFormat_t format)	{
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


int AllocTextureCachePool()	{
unsigned int	i,j;
TexCacheEntry	*tinfo;
		
	DEBUG_BEGIN("AllocTextureCachePool",DebugRenderThreadId, 110);

	for(i = 0; i < TEXPOOL_NUM; i++)	{
		if (texInfo[i] == NULL)	{
			if ((texInfo[i] = _GetMem((MAX_TEXNUM / TEXPOOL_NUM) * sizeof(TexCacheEntry))) == NULL)	{
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
	return(0);
	
	DEBUG_END("AllocTextureCachePool", DebugRenderThreadId, 110);
}


void GlideReleaseTexture(TexCacheEntry *tinfo)	{
unsigned int			i;
LPDIRECTDRAWSURFACE7	lpD3Dtexture;
	
	DEBUG_BEGIN("GlideReleaseTexture", DebugRenderThreadId, 111);
	
	if (tinfo == astate.acttex)	{
		DrawFlushPrimitives(0);
		i = (astate.astagesnum < astate.cstagesnum) ? astate.cstagesnum : astate.astagesnum;
		/* Az esetleges árnyék-textúra kiszedése az utolsó utáni használt stage-rõl */
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, NULL);		
		/* Az összes, nem texture combine-textúra kiszedése a stage-ekrõl */
		for(i = 0; i < MAX_STAGETEXTURES; i++)	{
			if ( (astate.lpDDTCCTex != astate.lpDDTex[i]) && (astate.lpDDTCATex != astate.lpDDTex[i]) )
				lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = NULL);
		}
		astate.acttex = NULL;
	}
	
	/* Normál textúra */
	if ( (lpD3Dtexture = tinfo->texBase[0]) != NULL)	{
		lpD3Dtexture->lpVtbl->SetPalette(lpD3Dtexture, NULL);
		lpD3Dtexture->lpVtbl->Release(lpD3Dtexture);
	}
	/* Az esetlegesen multitextúrázott AP textúra alpha-csatornája */
	if ( (lpD3Dtexture = tinfo->texAP88AlphaChannel[0]) != NULL) lpD3Dtexture->lpVtbl->Release(lpD3Dtexture);
	/* Az esetleges árnyék-textúra */
	if ( (lpD3Dtexture = tinfo->texCkAlphaMask[0]) != NULL) lpD3Dtexture->lpVtbl->Release(lpD3Dtexture);

	if (tinfo->palettedData != NULL) if (tinfo->palettedData->lpDDPal)
		TexDestroyPalette (tinfo->palettedData->lpDDPal);

	for(i = 0; i < 9; i++)
		tinfo->texBase[i] = tinfo->texCkAlphaMask[i] = tinfo->texAP88AlphaChannel[i] = NULL;
	if (tinfo->palettedData) _FreeMem(tinfo->palettedData);
	tinfo->palettedData = NULL;
	texNumInPool[tinfo->pool]--;
	
	DEBUG_END("GlideReleaseTexture", DebugRenderThreadId, 111);
}


int GlideCheckTexIntersect(FxU32 startAddress, GrLOD_t smallLod, GrLOD_t largeLod, GrAspectRatio_t aspectRatio,
						   GrTextureFormat_t format)	{
FxU32	start, end;
FxU32	downloadstart, downloadend;
int		from, to, i, nointersect;
GrLOD_t	slod, llod;

	
	if (!multiBaseMode)	{
		from = to = 0;
		slod = (int) astate.smallLod;
		llod = (int) astate.largeLod;
	} else {
		from = (int) astate.largeLod;
		to = ((GrLOD_t) astate.smallLod > GR_LOD_32) ? GR_LOD_32 : (int) astate.smallLod;
		slod = llod = (int) astate.largeLod;
	}
	
	for(i = from, nointersect = 1; i <= to; i++)	{
		if (astate.startAddress[i] != 0xFFFFFFFF)	{
			start = astate.startAddress[i];
			end = start + grTexCalcMemRequired(slod, llod, (GrAspectRatio_t) astate.aspectRatio, (GrTextureFormat_t) astate.format);
			downloadstart = startAddress;
			downloadend = downloadstart + grTexCalcMemRequired(smallLod, largeLod, aspectRatio, format);
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
TexCacheEntry *TexFindTex(unsigned int startAddress, GrTexInfo *info)	{
unsigned int	len, left, index;
TexCacheEntry	*tinfo;
	
	DEBUG_BEGIN("TexFindTex", DebugRenderThreadId, 112);

	if ( (len = timax - (left = index = timin)) == 0 ) return(NULL);
	while(len != 0)	{
		index = left + (len >> 1);
		if (texindex[index]->startAddress == startAddress) break;
		if (texindex[index]->startAddress > startAddress) len >>= 1;
		else {
			len = (len != 1) ? (len >> 1) + (len & 1) : 0;
			left = index;
		}
	}
	if ( (tinfo = texindex[index])->startAddress == startAddress)	{
		if ( (!perfectEmulating) || (info == NULL) ) return(tinfo);
		else return( GlideCompareInfos(&tinfo->info, info) ? tinfo : NULL );
	}
	return(NULL);
	
	DEBUG_END("TexFindTex", DebugRenderThreadId, 112);
}


/* Allokál egy bejegyzést egy új textúrához a megadott kezdõcímre, majd a megadott jellemzõket (info) */
/* el is tárolja ezen bejegyzésben. Ha nem tud új bejegyzést allokálni, akkor a visszaadott cím NULL. */
TexCacheEntry *TexAllocCacheEntry(FxU32 startAddress, GrTexInfo *info, TexCacheEntry *ontinfo, int onelevel)	{
unsigned int	index, len, left, i;
FxU32			endAddress;
TexCacheEntry	*tinfo;
	
	DEBUG_BEGIN("TexAllocCacheEntry", DebugRenderThreadId, 113);

	index = 0;

#ifdef GLIDE3
	endAddress = startAddress + ((onelevel) ? grTexCalcMemRequired(info->largeLodLog2, info->largeLodLog2, info->aspectRatioLog2, info->format) :
								grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, info));
#else
	endAddress = startAddress + ((onelevel) ? grTexCalcMemRequired(info->largeLod, info->largeLod, info->aspectRatio, info->format) :
								grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, info));
#endif

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
		/* index= max(i) (texInfo[i].endAddress<=startAddress) */
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
		if ( left == timax ) timax = index + 1;
		else {
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
	} else timax = 1;
	if (ontinfo == NULL)	{
		/* Ha az összes idáig allokált pool megtelt */
		if (ethead == NULL)	{
			/* Allokálunk még egy poolt, ha nem sikerül, akkor ezt a bejegyzést töröljük az */
			/* indextáblából, és hiba */
			if (!AllocTextureCachePool())	{
				len = timax-index-1;
				i = index;
				for(i = 0; i < len; i++) texindex[index+i] = texindex[index+i+1];
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
	if (tinfo == astate.acttex) astate.acttex = NULL;
	if (onelevel) tinfo->flags |= TF_STORESONELEVEL;
	return(tinfo);

	DEBUG_END("TexAllocCacheEntry", DebugRenderThreadId, 113);
}


void	TexInvalidateCache(FxU32 startAddress, GrTexInfo *info)	{
unsigned int	index, len, left;
FxU32			endAddress;
TexCacheEntry	*tinfo;
	
	index = 0;
	endAddress = startAddress + grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, info);

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


char	TexGetMatchingDXFormat (GrTextureFormat_t glideTexFormat)
{
char	namedFormat;
char*	tableOfNamedFormats;

	namedFormat = texFmtLookupTable[glideTexFormat];
	tableOfNamedFormats = dxTexFormats[namedFormat].missing;
	while(namedFormat != Pf_Invalid)	{
		if (dxTexFormats[namedFormat].isValid) break;
		namedFormat = *(tableOfNamedFormats++);
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
char			namedFormat;
//char			nf, *tablenf;
PALETTEENTRY	palEntries[256];
LPDIRECTDRAWPALETTE lpDDPal;
	
	DEBUG_BEGIN("GlideCreateTexture", DebugRenderThreadId, 114);

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
		tinfo->mipMapCount = tinfo->info.largeLodLog2 - tinfo->info.smallLodLog2 + 1;
#else
		tinfo->mipMapCount = tinfo->info.smallLod - tinfo->info.largeLod + 1;
#endif

		if (!TexCreateDXTexture (namedFormat, tinfo->x, tinfo->y, mipMapNum, tinfo->texBase))
			return (0);
		
		/* Fekete natív colorkey beállitása (avagy 0-s index a P8 formátum esetén) */
		tinfo->colorKeyNative = 0;
		tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
		TexSetSrcColorKeyForTexture (tinfo->texBase[0], (unsigned int) 0);

		for(llod = len; llod < 9; llod++) tinfo->texBase[llod] = NULL;
	}

	tinfo->palettedData = NULL;
	len = x = 0;
	if ( GlideHasTempStore(tinfo->info.format) ) len = sizeof(struct PalettedData);
	if ( len && (!perfectEmulating) )	{

#ifdef GLIDE3
		len += grTexCalcMemRequired(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLodLog2 : tinfo->info.smallLodLog2, tinfo->info.largeLodLog2,
									tinfo->info.aspectRatioLog2, tinfo->info.format);
#else
		len += grTexCalcMemRequired(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLod : tinfo->info.smallLod, tinfo->info.largeLod,
									tinfo->info.aspectRatio, tinfo->info.format);
#endif
		x = TF_STOREDMIPMAP;
	}
	if ( (config.Flags & CFG_STOREMIPMAP) && (!perfectEmulating) )	{

#ifdef GLIDE3
		len = grTexCalcMemRequired(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLodLog2 : tinfo->info.smallLodLog2, tinfo->info.largeLodLog2,
								   tinfo->info.aspectRatioLog2, tinfo->info.format) + sizeof(struct PalettedData);
#else
		len = grTexCalcMemRequired(tinfo->flags & TF_STORESONELEVEL ? tinfo->info.largeLod : tinfo->info.smallLod, tinfo->info.largeLod,
								   tinfo->info.aspectRatio, tinfo->info.format) + sizeof(struct PalettedData);
#endif
		x = TF_STOREDMIPMAP;
	}
	if (tinfo->startAddress >= RESERVED_ADDRESSSPACE) len = x = 0;
	
	if (len) {
		if ( (tinfo->palettedData = _GetMem(len)) != NULL)	{
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
	
	if ( (apMultiTexturing) && ((tinfo->info.format == GR_TEXFMT_AP_88) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422)) &&
		 tinfo->dxFormat->pixelFormat.ABitCount != 0)	{

		for(llod = 0; llod < 9; llod++) tinfo->texAP88AlphaChannel[llod] = tinfo->texBase[llod];
		
		if (!TexCreateDXTexture (Pf8_P8, tinfo->x, tinfo->y, mipMapNum, tinfo->texBase)) {
	
			for(llod = 0; llod < 9; llod++)
				tinfo->texAP88AlphaChannel[llod] = NULL;
			
			DEBUGLOG(0,"\n   Warning: GlideCreateTexture: AP multitexturing: creating paletted layer has failed");
			DEBUGLOG(1,"\n   Figyelmeztetés: GlideCreateTexture: AP multitexturing: a palettás réteg létrehozása nem sikerült");
			
			LOG(1,"    AP multitexturing: creating paletted layer has failed");
			
			return(1);
		}
	}
	/* Fizikai paletta létrehozása palettás textúrához vagy palettás multitextúrához */
	if ((tinfo->dxFormat->pixelFormat.BitPerPixel == 8)	|| (tinfo->texAP88AlphaChannel[0] != NULL)) {
		
		lpDDPal = NULL;
		if (tinfo->palettedData != NULL)
			lpDDPal = tinfo->palettedData->lpDDPal = TexCreatePalette (palEntries);
		
		TexAssignPaletteToTexture (tinfo->texBase[0], lpDDPal);
		
		DEBUGCHECK(0,(tinfo->palettedData->lpDDPal == NULL),"\n   Error: GlideCreateTexture: Couldn't create palette object for a paletted texture. Using corrupted palette or causing exception during rendering!");
		DEBUGCHECK(1,(tinfo->palettedData->lpDDPal == NULL),"\n   Hiba: GlideCreateTexture: Nem tudtam létrehozni a palettaobjektumot egy palettás textúrához. Helytelen paletta használata vagy kivétel keletkezése várható a megjelenítés közben!");
	}

	return(1);
	DEBUG_END("GlideCreateTexture", DebugRenderThreadId, 114);
}


unsigned int GlideCreateAlphaTexture(TexCacheEntry *tinfo)	{
unsigned int			i;

	DEBUG_BEGIN("GlideCreateAlphaTexture", DebugRenderThreadId, 115);
	
	for(i = 0; i < 9; i++) tinfo->texCkAlphaMask[i] = NULL;

	i = 0;
	while(_AlphaChannel[i] != Pf_Invalid)	{
		if ( dxTexFormats[_AlphaChannel[i]].isValid) break;
		i++;
	}
	if ( (i = _AlphaChannel[i]) == Pf_Invalid) return(0);

	tinfo->alphaDxFormat = dxTexFormats + i;

	if (!TexCreateDXTexture (i, tinfo->x, tinfo->y, tinfo->mipMapCount, tinfo->texCkAlphaMask)) {

		DEBUGLOG(0,"\n   Warning:  GlideCreateAlphaTexture: creating dd alpha texture surface failed");
		DEBUGLOG(1,"\n   Figyelmeztetés: GlideCreateAlphaTexture: a dd alfa-textúra létrehozása nem sikerült");
		LOG(1,"    GlideCreateAlphaTexture failed");
		return(0);
	}

	for(i = tinfo->mipMapCount; i < 9;i++) tinfo->texCkAlphaMask[i] = NULL;
	return(1);
	
	DEBUG_END("GlideCreateAlphaTexture", DebugRenderThreadId, 115);
}


void GlideTexSetTexturesAsLost()	{
unsigned int i;

	for(i = timin; i < timax; i++) texindex[i]->flags |= TF_CONSIDEREDASLOST;
}


/* Egy adott textúra adott szintjére textúraadatot tölt. */
/* tinfo: textúra bejegyzése, src: textúraadat, level: mipmap-szint (0-9), */
/* ystart,yend: kezdõ- és végsor (y), */
/* palmapcreate: 1 - palettás textúra kihasználtsági térképét frissítse, 0 - ne frissítse */
void *_GlideLoadTextureMipMap(TexCacheEntry *tinfo, void *src, int level, int ystart, int yend, int palmapcreate)	{
LPDIRECTDRAWSURFACE7	lpDDTexture, lpDDAlphaTexture;
GuTexPalette			nccPalette;
unsigned char			*palMap;
unsigned int			*palette;
int						lockerror;
unsigned int			x,y;
int						i, palType;
void*					texPtr;
void*					texAlphaPtr;
unsigned int			texPitch, texAlphaPitch;

	DEBUG_BEGIN("_GlideLoadTextureMipMap", DebugRenderThreadId, 116);

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
		TexConvertNCCToARGBPalette (&tinfo->palettedData->ncctable, &nccPalette);
		palette = (unsigned int *) &nccPalette;
	}

	//DEBUGLOG(2,"\nWriting text mipmap with format: %s",tform[tinfo->info.format]);
	
	palMap = ( (tinfo->palettedData) && palmapcreate) ? tinfo->palettedData->palmap : NULL;
	
	lpDDTexture = tinfo->texBase[level];
	if ( (src != NULL) && (lpDDTexture != NULL) )	{
		if ( (tinfo->flags & TF_STOREDMIPMAP) && (!((tinfo->flags & TF_STORESONELEVEL) && level)) && (level < (int) tinfo->mipMapCount))	{

#ifdef GLIDE3
			CopyMemory(tinfo->palettedData->data +
				grTexCalcMemRequired(tinfo->info.largeLodLog2 - level, tinfo->info.largeLodLog2, tinfo->info.aspectRatioLog2, tinfo->info.format) -
				grTexCalcMemRequired(tinfo->info.largeLodLog2 - level, tinfo->info.largeLodLog2 - level, tinfo->info.aspectRatioLog2, tinfo->info.format) +
				ystart*x*GlideGetTexBYTEPP(tinfo->info.format), src, (yend-ystart+1)*x*GlideGetTexBYTEPP(tinfo->info.format));
#else
			CopyMemory(tinfo->palettedData->data +
				grTexCalcMemRequired(tinfo->info.largeLod + level, tinfo->info.largeLod, tinfo->info.aspectRatio, tinfo->info.format) -
				grTexCalcMemRequired(tinfo->info.largeLod + level, tinfo->info.largeLod + level, tinfo->info.aspectRatio, tinfo->info.format) +
				ystart*x*GlideGetTexBYTEPP(tinfo->info.format), src, (yend-ystart+1)*x*GlideGetTexBYTEPP(tinfo->info.format));
#endif
		}

		lockerror = 0;
		if ((texPtr = TexGetPtrToTexture (lpDDTexture, &texPitch)) == NULL) lockerror = 1;
		if ((lpDDAlphaTexture = tinfo->texAP88AlphaChannel[level]) != NULL)
			if ((texAlphaPtr = TexGetPtrToTexture (lpDDAlphaTexture, &texAlphaPitch)) == NULL) lockerror |= 2;

		if (!lockerror)	{
			if (lpDDAlphaTexture == NULL)
				MMCConvertAndTransferData(srcPixFmt[tinfo->info.format],  &tinfo->dxFormat->pixelFormat,
										  0x0, 0x0, 0xFF,
										  src, ((char *) texPtr)+x*ystart*tinfo->dxFormat->pixelFormat.BitPerPixel/8,
										  x, yend - ystart + 1,
										  x * GlideGetTexBYTEPP(tinfo->info.format), texPitch,
										  palette, palMap,
										  MMCIsPixfmtTheSame(srcPixFmt[tinfo->info.format], &tinfo->dxFormat->pixelFormat) ?
										  	CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
										  palType);
			else
				TexConvertToMultiTexture(&tinfo->dxFormat->pixelFormat,
										 src, ((char *) texPtr)+x*ystart,
										 ((char *) texAlphaPtr)+x*ystart*tinfo->dxFormat->pixelFormat.BitPerPixel/8,
										 x, yend-ystart+1,
										 texPitch, texAlphaPitch);
		}
		TexReleasePtrToTexture (lpDDTexture);
		if (lpDDAlphaTexture)
			TexReleasePtrToTexture (lpDDAlphaTexture);
		
		((char *)src) += x * (yend - ystart + 1) * GlideGetTexBYTEPP(tinfo->info.format);
	}
	return(src);

	DEBUG_END("_GlideLoadTextureMipMap", DebugRenderThreadId, 116);
}


void *GlideLoadTextureMipMap(TexCacheEntry *tinfo, void *src, int level, int ystart, int yend, int palmapcreate)	{
unsigned char	*from, *to;
unsigned int	x, y;

	DEBUG_BEGIN("GlideLoadTextureMipMap", DebugRenderThreadId, 117);

	x = tinfo->x >> level;
	y = tinfo->y >> level;
	if (ystart > yend) {
		ystart = 0;
		yend = y - 1;
	}
	from = src;
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
	return(src);
	
	DEBUG_END("GlideLoadTextureMipMap", DebugRenderThreadId, 117);
}


/* újratölti egy tetszôleges textúra bitképét, ha tudja */
void TexReloadTexture(TexCacheEntry *tinfo, unsigned int reloadFlags)	{
unsigned int	i;
void			*src;
unsigned char	textureData[8*8];
PALETTEENTRY	palEntries[256];
unsigned char	textureReloadCanGo;

	DEBUG_BEGIN("TexReloadTexture", DebugRenderThreadId, 118);
	
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
		textureReloadCanGo = 1;
		
		/* Nem valódi palettás textúrát érvénytelen palettával vagy ncc-táblával nem tölt újra */
		if ((tinfo->dxFormat->pixelFormat.BitPerPixel != 8)	&& (tinfo->texAP88AlphaChannel[0] == NULL) &&
			(tinfo->palettedData != NULL) ) {

			switch(tinfo->info.format)	{
				case GR_TEXFMT_P_8:
				case GR_TEXFMT_AP_88:
					if (tinfo->palettedData->palette.data[0] == 0xFFFFFFFF) {
						textureReloadCanGo = 0;
					}
					break;

				case GR_TEXFMT_YIQ_422:
				case GR_TEXFMT_AYIQ_8422:
					if (tinfo->palettedData->ncctable.iRGB[0][0] == ((short) 0x8000)) {
						textureReloadCanGo = 0;
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
					MMCConvARGBPaletteToABGR ((unsigned int *) &tinfo->palettedData->palette.data, (unsigned int *) palEntries);
					//if (tinfo->texBase[0]->lpVtbl->IsLost(tinfo->TexD3DHandle[0]) == DD_OK) {
					if (tinfo->palettedData->lpDDPal)
						TexSetPaletteEntries (tinfo->palettedData->lpDDPal, palEntries);
					//}
				}
				break;
			case GR_TEXFMT_YIQ_422:
			case GR_TEXFMT_AYIQ_8422:
				if (tinfo->palettedData != NULL) {
					TexConvertNCCToABGRPalette(&(tinfo->palettedData->ncctable), palEntries);
					if (tinfo->palettedData->lpDDPal)
						TexSetPaletteEntries (tinfo->palettedData->lpDDPal, palEntries);
				}
				break;
		}
	}
	tinfo->flags &= ~TF_CONSIDEREDASLOST;
	
	DEBUG_END("TexReloadTexture", DebugRenderThreadId, 118);
}


/* Primitívek kiflusholásakor meghívódó rutin: megnézi, hogy az aktuális textúra palettája */
/* megegyezik-e az aktuális palettával, és ha nem, újratölti az egész textúrát */
void TexUpdateTexturePalette()	{
TexCacheEntry	*tinfo;
unsigned int	reloadPalmap;

	DEBUG_BEGIN("TexUpdateTexturePalette", DebugRenderThreadId, 119);

	if ((tinfo = astate.acttex) != NULL)	{
		
		if ( ((tinfo->info.format == GR_TEXFMT_P_8) || (tinfo->info.format == GR_TEXFMT_AP_88)) )	{
			if ( (tinfo->dxFormat->pixelFormat.BitPerPixel != 8) && (tinfo->texAP88AlphaChannel[0] == NULL) )	{
				if (TexComparePalettesWithMap(&(tinfo->palettedData->palette), &palette, tinfo->palettedData->palmap) )	{
					reloadPalmap = tinfo->palettedData->palette.data[0] != 0xFFFFFFFF ? RF_NOPALETTEMAPCREATE : 0;
					TexCopyPalette(&(tinfo->palettedData->palette), &palette, 256);
					TexReloadTexture(tinfo, RF_TEXTURE | reloadPalmap);
				}
			} else {
				if (TexComparePalettes(&(tinfo->palettedData->palette), &palette) )	{
					TexCopyPalette(&(tinfo->palettedData->palette), &palette, 256);
					TexReloadTexture (tinfo, RF_PALETTE);
					colorKeyPal = TexGetTexIndexValue(0, astate.colorkey, &palette);
					callflags |= CALLFLAG_SETCOLORKEY;
				}
			}
		}
	}
	callflags &= ~CALLFLAG_SETPALETTE;

	DEBUG_END("TexUpdateTexturePalette", DebugRenderThreadId, 119);
}


void TexUpdateTextureNccTable()	{
TexCacheEntry	*tinfo;
unsigned int	reloadPalmap;
	
	DEBUG_BEGIN("TexUpdateTextureNccTable", DebugRenderThreadId, 120);
	
	if ((tinfo = astate.acttex) != NULL)	{
		if ( ((tinfo->info.format == GR_TEXFMT_YIQ_422) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422)) )	{
			if ( (tinfo->dxFormat->pixelFormat.BitPerPixel != 8) && (tinfo->texAP88AlphaChannel[0] == NULL) )	{
				if (TexCompareNCCTables(&tinfo->palettedData->ncctable, actncctable))	{
					reloadPalmap = tinfo->palettedData->ncctable.iRGB[0][0] != ((short) 0x8000) ? RF_NOPALETTEMAPCREATE : 0;
					CopyMemory(&tinfo->palettedData->ncctable, actncctable, sizeof(GuNccTable));
					TexReloadTexture(tinfo, RF_TEXTURE | reloadPalmap);
				}
			} else {
				if (TexCompareNCCTables(&tinfo->palettedData->ncctable, actncctable))	{
					CopyMemory(&tinfo->palettedData->ncctable, actncctable, sizeof(GuNccTable));
					TexReloadTexture (tinfo, RF_PALETTE);
					colorKeyPal = TexGetTexIndexValue(0, astate.colorkey, &palette);
					callflags |= CALLFLAG_SETCOLORKEY;
				}
			}
		}
	}
	callflags &= ~CALLFLAG_SETNCCTABLE;
	
	DEBUG_END("TexUpdateTextureNccTable", DebugRenderThreadId, 120);
}


/* Natív colorkeying esetén ténylegesen engedélyezi vagy letiltja a colorkeying-et, és az beállítja */
/* az egyes flageket az alpha combine-hoz (TNT-colorkeying vagy általános) */
void TexUpdateNativeColorKeyState()	{
int ckMethod;

	DEBUG_BEGIN("TexUpdateNativeColorKeyState", DebugRenderThreadId, 121);
	
	astate.flags &= ~STATE_COLORKEYTNT;
	if ( (astate.colorKeyEnable) && (astate.cotherp == GR_COMBINE_OTHER_TEXTURE) && (astate.flags & STATE_NATIVECKMETHOD) )	{
		ckMethod = GetCKMethodValue(config.Flags);
		if ( (ckMethod == CFG_CKMNATIVETNT) || ((ckMethod != CFG_CKMNATIVE) && (config.Flags & CFG_CKMFORCETNTINAUTOMODE)) &&
			 (!astate.alphaBlendEnable) && (astate.alphatestfunc == D3DCMP_ALWAYS) ) {
			astate.flags |= STATE_COLORKEYTNT;
		}
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_COLORKEYENABLE, TRUE);
	} else
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_COLORKEYENABLE, FALSE);
	
	callflags |= CALLFLAG_ALPHACOMBINE;
	callflags &= ~CALLFLAG_SETCOLORKEYSTATE;
	
	DEBUG_END("TexUpdateNativeColorKeyState", DebugRenderThreadId, 121);
}


/* Nem natív (alfa alapú) colorkeying esetén szükség szerint újratölti a textúra árnyék-textúráját, */
/* és beállítja az árnyék-alpha testing-hez használt stage (az utolsó utáni) állapotát. */
void TexUpdateAlphaBasedColorKeyState()	{	
unsigned int			ck,i,j,x,y;
LPDIRECTDRAWSURFACE7	lpDDTexture;
TexCacheEntry			*tinfo;
void					*src;
void*					texPtr;
unsigned int			texPitch;
unsigned int			alphaMask;
DWORD					op,arg,carg2,aarg2,cop;
		
	DEBUG_BEGIN("TexUpdateAlphaBasedColorKeyState", DebugRenderThreadId, 122);
	
	tinfo = astate.acttex;
	LOG(0,"\nGlideTexSetColorKey(), tinfo=astate.acttex: %p",tinfo);
	if ( (tinfo != NULL) && (astate.colorKeyEnable) )	{
		if (astate.flags & STATE_NATIVECKMETHOD)	{
			/* Colorkeying a kártya natív képességével*/
			lpDDTexture = tinfo->texBase[0];
			/* Ha a textúra palettás, vagy ha multitextúrázott */
			if ( (tinfo->dxFormat->pixelFormat.BitPerPixel == 8) || (tinfo->texAP88AlphaChannel[0] != NULL) )	{
				if ( (tinfo->info.format == GR_TEXFMT_YIQ_422) || (tinfo->info.format == GR_TEXFMT_AYIQ_8422) )	{
					if (actncctable == &nccTable0) ck = colorKeyNcc0;
						else ck = colorKeyNcc1;
				} else ck = colorKeyPal;			
			} else ck = tinfo->dxFormat->colorKey.dwColorSpaceLowValue;
			if ((ck != tinfo->colorKeyNative) && (lpDDTexture))
				TexSetSrcColorKeyForTexture (lpDDTexture, ck);
			
			ck = 0;
			callflags &= ~CALLFLAG_SETCOLORKEY;			
		} else {
			/* Alfa alapú colorkeying */
			if ( (ck = TexGetTexelValueFromARGB(astate.colorkey, tinfo->info.format, &palette, actncctable)) !=0 )	{
				if ( (ck != tinfo->colorKeyAlphaBased) )	{
					LOG(0,"\n   old colorkey: %0x, new colorkey: %0x", tinfo->colorKeyAlphaBased, ck);
					if (tinfo->texCkAlphaMask[0] == NULL) GlideCreateAlphaTexture(tinfo);
					if (tinfo->texCkAlphaMask[0] != NULL)	{
						x = tinfo->x;
						y = tinfo->y;
						src = (perfectEmulating) ? src = textureMem + tinfo->startAddress : ((tinfo->flags & TF_STOREDMIPMAP) ? &tinfo->palettedData->data : NULL );
						if (src) {
							alphaMask = ((1L << tinfo->alphaDxFormat->pixelFormat.ABitCount) - 1) << tinfo->alphaDxFormat->pixelFormat.APos;
							for(i = 0; i < tinfo->mipMapCount; i++)	{
								lpDDTexture = tinfo->texCkAlphaMask[i];
								if (lpDDTexture)	{
									if ((texPtr = TexGetPtrToTexture (lpDDTexture, &texPitch)) != NULL) {
										src = TexCreateAlphaMask(src, texPtr,
																 texPitch,
																 GlideGetTexBYTEPP(tinfo->info.format),
																 tinfo->alphaDxFormat->pixelFormat.BitPerPixel/8,
																 x, y, ck,
																 alphaMask);
										TexReleasePtrToTexture (lpDDTexture);
									}
								} else {
									DEBUGLOG (0, "\n   Error: TexUpdateAlphaBasedColorKeyState: Cannot lock and update alpha mask texture!");
									DEBUGLOG (0, "\n   Hiba: TexUpdateAlphaBasedColorKeyState: Nem tudom zárolni és frissíteni az alfa-maszk textúrát!");
								}
								x >>= 1;
								y >>= 1;
							}
						}
					}
				}				
			}
			tinfo->colorKeyAlphaBased = ck;
		}
	}
	if (!(astate.flags & STATE_NATIVECKMETHOD) || ((astate.flags & STATE_NATIVECKMETHOD) && (astate.flags & STATE_ALPHACKUSED)))	{
		tinfo = astate.acttex;
		src = (tinfo != NULL) ? tinfo->texCkAlphaMask[0] : NULL;
		i = j = MAX(astate.astagesnum, astate.cstagesnum);
		/* */
		carg2 = aarg2 = D3DTA_CURRENT;
		cop = D3DTOP_SELECTARG2;
		if ( (astate.astagesnum < i) || ( (astate.astagesnum == i) && (astate.ainvertp) ) )	{
			if ( (astate.cstagesnum < i) || ( (astate.cstagesnum == i) && (astate.cinvertp) ) )	{
				if ( (astate.astagesnum == i) && (astate.ainvertp) ) aarg2 |= D3DTA_COMPLEMENT;
				if ( (astate.cstagesnum == i) && (astate.cinvertp) ) carg2 |= D3DTA_COMPLEMENT;
				i--;
			}
		}
		if ( (src == NULL) || (!texCkCanGoWithAlphaTesting[astate.alphatestfunc])
			  || (!astate.colorKeyEnable) || (ck == 0)
			  || (astate.cotherp != GR_COMBINE_OTHER_TEXTURE) )	{
			lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = NULL);
			if ( (astate.flags & STATE_CCACSTATECHANGED) || (astate.flags & STATE_ALPHACKUSED) )	{
				if (i == j) cop = D3DTOP_DISABLE;
				if (astate.colorOp[i] != cop)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLOROP, astate.colorOp[i] = cop);
				if (astate.alphaOp[i] != D3DTOP_SELECTARG2)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP, astate.alphaOp[i] = D3DTOP_SELECTARG2);
				if (astate.alphaArg2[i] != aarg2)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP, astate.alphaOp[i] = aarg2);
			}
			if (astate.flags & STATE_ALPHACKUSED)	{
				op = (astate.alphatestfunc == D3DCMP_ALWAYS) ? FALSE : TRUE;
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ALPHATESTENABLE, op);
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ALPHAREF, astate.alpharef);
			}		
			astate.flags &= ~STATE_ALPHACKUSED;
		} else {
			LOG(0,"   texture stage: %d",i);
			if ((astate.flags & STATE_CCACSTATECHANGED) || (astate.flags & STATE_ALPHABLENDCHANGED) ||
				(astate.flags & STATE_ALPHATESTCHANGED) || (!(astate.flags & STATE_ALPHACKUSED)) )	{
				if (astate.colorOp[i] != D3DTOP_SELECTARG2)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLOROP, astate.colorOp[i] = D3DTOP_SELECTARG2);
				if (astate.colorArg2[i] != carg2)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLORARG2, astate.colorArg2[i] = carg2);
				if (astate.alphaArg2[i] != aarg2)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG2, astate.alphaArg2[i] = aarg2);
				
				arg = D3DTA_TEXTURE;
				if (texCkCanGoWithAlphaTesting[astate.alphatestfunc] == 2)	{
					op = D3DTOP_ADD;
					arg = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
				} else op = D3DTOP_MODULATE;
				if ((!astate.alphaBlendEnable) && (astate.alphatestfunc == D3DCMP_ALWAYS) ) op = D3DTOP_SELECTARG1;
				
				if (astate.alphaOp[i] != op)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP, astate.alphaOp[i] = op);
				if (astate.alphaArg1[i] != arg)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG1, astate.alphaArg1[i] = arg);
				
				/* Ha egy új stage-et használtunk fel, akkor gondoskodunk arról, hogy a következõ tiltva legyen */
				j = i + 1;
				if (astate.colorOp[j] != D3DTOP_DISABLE)
					lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, j, D3DTSS_COLOROP, astate.colorOp[j] = D3DTOP_DISABLE);

			}
			LOG(0,"   shadow texture handle: %p",tinfo->texCkAlphaMask[0]);
			lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = tinfo->texCkAlphaMask[0]);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MINFILTER, astate.minfilter);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MAGFILTER, astate.magfilter);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ADDRESSU, astate.clampmodeu);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ADDRESSV, astate.clampmodev);
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MIPMAPLODBIAS, *( (LPDWORD) (&astate.lodBias)) );
			if ( (!(astate.flags & STATE_ALPHACKUSED)) && (astate.alphatestfunc == D3DCMP_ALWAYS) )	{
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAREF, (DWORD) config.AlphaCKRefValue);
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
			}
			astate.flags |= STATE_ALPHACKUSED;			
		}
		astate.flags &= ~(STATE_CCACSTATECHANGED | STATE_ALPHABLENDCHANGED | STATE_ALPHATESTCHANGED);
	}
	
	DEBUG_END("TexUpdateAlphaBasedColorKeyState", DebugRenderThreadId, 122);
}


void TexUpdateMultitexturingState(TexCacheEntry *tinfo)	{
LPDIRECTDRAWSURFACE7	lpDDTexture, lpDDAlphaChannel;
unsigned int			flags;

	if (tinfo != NULL)	{
		lpDDTexture = tinfo->texBase[0];
		if( (lpDDAlphaChannel = tinfo->texAP88AlphaChannel[0]) == NULL) lpDDAlphaChannel = lpDDTexture;
		else if (astate.flags & STATE_TCLOCALALPHA) lpDDTexture = lpDDAlphaChannel;

	} else lpDDTexture = lpDDAlphaChannel = NULL;

	if (astate.lpDDTCCTex != NULL)
		lpDDTexture = astate.lpDDTCCTex;

	if (astate.lpDDTCATex != NULL)
		lpDDAlphaChannel = astate.lpDDTCATex;

	if (lpDDTexture == lpDDAlphaChannel) lpDDAlphaChannel = NULL;

	flags = (lpDDAlphaChannel != NULL) ? STATE_APMULTITEXTURING : 0;
	/* Ha a multitextúrázási állapot megváltozott, akkor az alpha combine-t meg kell hívni */
	/* az esetleges stage-csúszás miatt */
	if ((astate.flags & STATE_APMULTITEXTURING) != flags)	{
		astate.flags &= ~STATE_APMULTITEXTURING;
		astate.flags |= flags;
		callflags |= CALLFLAG_ALPHACOMBINE;
	}
}


void TexSetCurrentTexture(TexCacheEntry *tinfo)	{
LPDIRECTDRAWSURFACE7	lpDDTexture, lpDDAlphaChannel;
LPDIRECTDRAWSURFACE7	lpDDTextures[2];
int						i, j, textureUsage, reload;
TexCacheEntry			*tinfo2;

	DEBUG_BEGIN("TexSetCurrentTexture", DebugRenderThreadId, 123);
	
	LOG(0,"\nTexSetCurrentTexture(tinfo=%0x)", tinfo);

	if (!(astate.flags & STATE_TEXTUREUSAGEMASK))
		return;

	tinfo2 = tinfo;
	
	if (tinfo != NULL)	{

#ifdef	WRITETEXTURESTOBMP
		WTEX(tinfo);
#endif

		lpDDTexture = tinfo->texBase[0];
		if( (lpDDAlphaChannel = tinfo->texAP88AlphaChannel[0]) == NULL) lpDDAlphaChannel = lpDDTexture;
			else if (astate.flags & STATE_TCLOCALALPHA) lpDDTexture = lpDDAlphaChannel;

	} else lpDDTexture = lpDDAlphaChannel = NULL;

	if (astate.lpDDTCCTex != NULL)	{
		lpDDTexture = astate.lpDDTCCTex;
		tinfo = nullTexture;
	}
	if (astate.lpDDTCATex != NULL)	{
		lpDDAlphaChannel = astate.lpDDTCATex;
		tinfo = nullTexture;
	}

	if (lpDDTexture == lpDDAlphaChannel) lpDDAlphaChannel = NULL;

	if (tinfo == tinfo2) tinfo2 = NULL;
	
	if (tinfo != NULL)	{
		lpDDTextures[0] = tinfo->texBase[0];
		lpDDTextures[1] = tinfo->texAP88AlphaChannel[0];
		for(reload = i = 0; i < 2; i++)	{
			if (lpDDTextures[i] != NULL) if (lpDDTextures[i]->lpVtbl->IsLost(lpDDTextures[i]) == DDERR_SURFACELOST)
				if (lpDDTextures[i]->lpVtbl->Restore(lpDDTextures[i]) == DD_OK) reload = 1;
		}
		if ( (tinfo->flags & TF_CONSIDEREDASLOST) || reload) TexReloadTexture(tinfo, RF_TEXTURE /*| RF_PALETTE*/);
	}
	
	if (tinfo2 != NULL)	{
		lpDDTextures[0] = tinfo2->texBase[0];
		lpDDTextures[1] = tinfo2->texAP88AlphaChannel[0];
		for(reload = i = 0; i < 2; i++)	{
			if (lpDDTextures[i] != NULL) if (lpDDTextures[i]->lpVtbl->IsLost(lpDDTextures[i]) == DDERR_SURFACELOST)
				if (lpDDTextures[i]->lpVtbl->Restore(lpDDTextures[i]) == DD_OK) reload = 1;
		}
		if ( (tinfo2->flags & TF_CONSIDEREDASLOST) || reload) TexReloadTexture(tinfo2, RF_TEXTURE /*| RF_PALETTE*/);
	}

	callflags |= CALLFLAG_SETCOLORKEY;
	
	if ( (astate.flags & STATE_TEXTURE0) && (lpDDTexture != astate.lpDDTex[0]) )
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, 0, astate.lpDDTex[0] = lpDDTexture);
	j = MAX(astate.astagesnum, astate.cstagesnum);
	textureUsage = STATE_TEXTURE1;
	
	//j=3;
	if (lpDDAlphaChannel != NULL)	{
	
		for(i = 1; i < j; i++)	{
			if ( (astate.flags & textureUsage) && (lpDDAlphaChannel != astate.lpDDTex[i]) )
				lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = lpDDAlphaChannel);
			textureUsage <<= 1;
		}	
	
	} else	{
	
		for(i = 1; i < j; i++)	{
			if ( (astate.flags & textureUsage) && (lpDDTexture != astate.lpDDTex[i]) )
				lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = lpDDTexture);
			textureUsage <<= 1;
		}

	}
	callflags &= ~CALLFLAG_RESETTEXTURE;
	LOG(0,"   set texture, shadow texture handle: %p",(tinfo) ? tinfo->texCkAlphaMask[0] : NULL);
	
	DEBUG_END("TexSetCurrentTexture", DebugRenderThreadId, 123);
}


/* Az astate-ben eltárolt paraméterek alapján beállítja az aktuális textúrát. */
void GlideTexSource()	{
GrTexInfo				info;
TexCacheEntry			*tinfo, *sectinfo;
GrLOD_t					lod;
int						i,j, level;
FxU32					endAddress;
void					*src;
struct PalettedData		*palettedData;

	DEBUG_BEGIN("GlideTexSource", DebugRenderThreadId, 124);

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

	callflags |= CALLFLAG_SETPALETTE | CALLFLAG_SETNCCTABLE;
	
	palettedData = NULL;
	/* Ha nincs multibase texturing: a cache-ben keresett textúra ellenõrzése kétféle módon mehet: */
	/* - tökéletes emu esetén: az adott kezdõcímen pontosan a megadott jellemzõkkel kell rendelkeznie */
	/* - egyébként elég, ha az adott kezdõcímen talál vmilyen textúrát */
	if (!multiBaseMode)	{
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
					endAddress = astate.startAddress[i] + grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, &info);
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
					}
				}
			}
			tinfo->flags &= ~TF_SECRELOAD;
		}
	}
	astate.acttex = tinfo;

	//TexSetCurrentTexture(tinfo);
	callflags |= CALLFLAG_RESETTEXTURE;
	callflags &= ~CALLFLAG_SETTEXTURE;
	
	DEBUG_END("GlideTexSource", DebugRenderThreadId, 124);

}


/*------------------------------------------------------------------------------------------*/
/*.................................. DX7 implementáció  ....................................*/


int		TexIsDXFormatSupported (enum DXTextureFormats format) {

	return (dxTexFormats[format].isValid);
}


void	TexGetDXTexPixFmt (enum DXTextureFormats format, _pixfmt *pixFormat)
{
	*pixFormat = dxTexFormats[format].pixelFormat;
}


int		TexCreateDXTexture(enum DXTextureFormats format, unsigned int x, unsigned int y, int mipMapNum, LPDIRECTDRAWSURFACE7* mipMaps)
{
DDSURFACEDESC2			ddTexDesc;
LPDIRECTDRAWSURFACE7	lpD3DTexture;
LPDIRECTDRAWSURFACE7	lpD3DNext;
LPDIRECTDRAWSURFACE7	textureMipMaps[9];
int						i;
HRESULT					hr;

	ZeroMemory (&ddTexDesc, sizeof(DDSURFACEDESC2));
	ddTexDesc.dwSize = sizeof(DDSURFACEDESC2);

	ddTexDesc.dwWidth = x;
	ddTexDesc.dwHeight = y;
	ddTexDesc.ddpfPixelFormat = dxTexFormats[format].ddsdPixelFormat;

	ddTexDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;// | DDSCAPS_VIDEOMEMORY;
	ddTexDesc.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	ddTexDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS | DDSD_CKSRCBLT;

	if ( (ddTexDesc.dwMipMapCount = mipMapNum) != 1 )	{
		ddTexDesc.dwFlags |= DDSD_MIPMAPCOUNT;
		ddTexDesc.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	}

	lpD3DTexture = NULL;
	hr = lpDD->lpVtbl->CreateSurface(lpDD, &ddTexDesc, &lpD3DTexture, NULL);

	if ((hr != DD_OK) || (lpD3DTexture == NULL)) {
		DEBUGLOG(0,"\n   Warning:  TexCreateDX7Texture: creating dd alpha texture surface has failed");
		DEBUGLOG(1,"\n   Figyelmeztetés: TexCreateDX7Texture: a dd alfa-textúra létrehozása nem sikerült");
		LOG(1,"    TexCreateDX7Texture failed");
		return(0);
	}
	
	ddTexDesc.ddsCaps.dwCaps2 = 0;
	ddTexDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
	textureMipMaps[0] = lpD3DTexture;
	
	for(i = 1; i < mipMapNum; i++)	{
		hr = lpD3DTexture->lpVtbl->GetAttachedSurface(lpD3DTexture, &ddTexDesc.ddsCaps, &lpD3DNext);
		if ((hr != DD_OK) || (lpD3DNext == NULL)) {
			DEBUGLOG(0,"\n   Warning:  TexCreateDX7Texture: obtaining mipmap surface has failed");
			DEBUGLOG(1,"\n   Figyelmeztetés: TexCreateDX7Texture: a mipmap-objektum lekérdezése nem sikerült");

			lpD3DTexture->lpVtbl->Release (lpD3DTexture);
			return (0);
		} 
		textureMipMaps[i] = lpD3DTexture = lpD3DNext;
	}

	CopyMemory (mipMaps, textureMipMaps, mipMapNum * sizeof(LPDIRECTDRAWSURFACE7));
	return (1);
}


void	TexDestroyDXTexture(LPDIRECTDRAWSURFACE7 texture)
{
	texture->lpVtbl->Release (texture);
}


void*	TexGetPtrToTexture (LPDIRECTDRAWSURFACE7 texture, unsigned int *pitch)
{
DDSURFACEDESC2	ddTexDesc = {sizeof(DDSURFACEDESC2)};

	if (texture->lpVtbl->Lock(texture, NULL, &ddTexDesc, DDLOCK_WAIT, NULL) == DD_OK) {
		*pitch = ddTexDesc.lPitch;
		return ddTexDesc.lpSurface;
	}
	return NULL;
}


void	TexReleasePtrToTexture (LPDIRECTDRAWSURFACE7 texture)
{
	texture->lpVtbl->Unlock(texture, NULL);
}


void	TexSetSrcColorKeyForTexture (LPDIRECTDRAWSURFACE7 texture, unsigned int colorKey)
{
DDCOLORKEY	colorkey;

	colorkey.dwColorSpaceLowValue = colorkey.dwColorSpaceHighValue = colorKey;
	texture->lpVtbl->SetColorKey(texture, DDCKEY_SRCBLT, &colorkey);
}

LPDIRECTDRAWPALETTE		TexCreatePalette (PALETTEENTRY*	initialPalEntries)
{
LPDIRECTDRAWPALETTE lpDDPal;

	lpDDPal = NULL;
	if (lpDD->lpVtbl->CreatePalette(lpDD, DDPCAPS_8BIT | DDPCAPS_ALLOW256, initialPalEntries, &lpDDPal, NULL) == DD_OK)
		return lpDDPal;
	else
		return NULL;
}


void	TexDestroyPalette (LPDIRECTDRAWPALETTE palette)
{
	palette->lpVtbl->Release (palette);
}


int		TexAssignPaletteToTexture (LPDIRECTDRAWSURFACE7 texture, LPDIRECTDRAWPALETTE newPalette)
{
	if (texture != NULL) {
		
		if (texture->lpVtbl->SetPalette(texture, newPalette) == DD_OK)
			return (1);
	
	}

	DEBUGLOG (0, "\n	Error: TexAssignPaletteToTexture: Assigning palette to texture has failed");
	DEBUGLOG (1, "\n	Hiba: TexAssignPaletteToTexture: A palettát nem sikerült a textúrához rendelni");
	return (0);
}


void	TexSetPaletteEntries (LPDIRECTDRAWPALETTE palette, PALETTEENTRY* palEntries)
{
	palette->lpVtbl->SetEntries(palette, 0, 0, 256, palEntries);
}

/*------------------------------------------------------------------------------------------*/
/*.......................... Glide függvények implementációja ..............................*/


/* Minimum texmem cím = 0 */
FxU32 EXPORT grTexMinAddress(GrChipID_t tmu) {

	return(0);
}


/* Maximum texmem cím = amennyi a konfigban be van állítva */
FxU32 EXPORT grTexMaxAddress(GrChipID_t tmu) {
	
	return( (tmu == GR_TMU0) ? config.TexMemSize : 0 );
}


/* Mipmap által foglalt texmem mérete */
FxU32 EXPORT grTexCalcMemRequired(GrLOD_t smallLod, GrLOD_t largeLod,
								  GrAspectRatio_t aspect, GrTextureFormat_t format ) {
unsigned int	llod, slod, size, needed;
	
	
	llod = GlideGetLOD(largeLod);
	slod = GlideGetLOD(smallLod);
	needed = GlideGetXY(largeLod,aspect);
	size = ( (needed >> 16) * (needed & 0xFFFF) ) * GlideGetTexBYTEPP(format);
	for(needed = 0; llod >= slod; llod >>= 1)	{
		needed += size;
		if ( (size >>= 2) == 0 ) size = 1;
	}	
	return((needed + 7) & (~7) );
}


/* Szétbontott mipmap texmem mérete */
/* Megjegyz: az 1 TMU miatt a szétbontást nem támogatja! */
FxU32 EXPORT grTexTextureMemRequired( FxU32 evenOdd, GrTexInfo *info )	{
unsigned int llod,slod,size,needed,rshift,rshift2;
	
#ifdef GLIDE3
	llod = GlideGetLOD(info->largeLodLog2);
	slod = GlideGetLOD(info->smallLodLog2);
	needed = GlideGetXY(info->largeLodLog2,info->aspectRatioLog2);
#else
 
	llod = GlideGetLOD(info->largeLod);
	slod = GlideGetLOD(info->smallLod);
	needed = GlideGetXY(info->largeLod,info->aspectRatio);
#endif

	if ( ((evenOdd == GR_MIPMAPLEVELMASK_ODD) && ((llod & 0x5555) == 0)) || 
		 ((evenOdd == GR_MIPMAPLEVELMASK_EVEN) && ((llod & 0xAAAA) == 0)) )	{
		needed=(needed >> 1) & (~0x8000);
		llod >>= 1;
	}
	rshift = (evenOdd == GR_MIPMAPLEVELMASK_BOTH) ? 1 : 2;
	rshift2 = 2*rshift;
   
	
	size = ( (needed >> 16) * (needed & 0xFFFF) ) * GlideGetTexBYTEPP(info->format);
	if (size == 0) size = 1;
	for(needed = 0; llod >= slod; llod >>= rshift)	{
		needed += size;
		if ( (size >>= rshift2) == 0 ) size = 1;
	}	
	return((needed + 7) & (~7) );
}


/* Textúra filtering mód beáll. */
void EXPORT grTexFilterMode(GrChipID_t tmu,
							GrTextureFilterMode_t minFilterMode,
							GrTextureFilterMode_t magFilterMode) {
DWORD	magn, min;
int		i;
	
	//return;
	DEBUG_BEGIN("grTexFilterMode",DebugRenderThreadId, 125);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	min = (minFilterMode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTFN_POINT : D3DTFN_LINEAR;
	magn = (magFilterMode == GR_TEXTUREFILTER_POINT_SAMPLED) ? D3DTFN_POINT : D3DTFN_LINEAR;
	if (config.Flags & CFG_BILINEARFILTER) min = magn = D3DTFN_LINEAR;
	if ( (astate.minfilter != min) || (astate.magfilter != magn) ) {
		DrawFlushPrimitives(0);
		if (astate.minfilter != min)	{
			for(i = 0; i < 3; i++)
				lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MINFILTER ,astate.minfilter = min);
		}
		if (astate.magfilter != magn)	{
			for(i = 0; i < 3; i++)
				lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MAGFILTER, astate.magfilter = magn);
		}
	}
	
	DEBUG_END("grTexFilterMode",DebugRenderThreadId, 125);
}


void EXPORT grTexClampMode(GrChipID_t tmu,
						   GrTextureClampMode_t sClampMode,
						   GrTextureClampMode_t tClampMode)	{
DWORD	addru, addrv;
int		i;
	
	//return;
	DEBUG_BEGIN("grTexClampMode",DebugRenderThreadId, 126);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	if ( sClampMode == GR_TEXTURECLAMP_WRAP ) addru = D3DTADDRESS_WRAP;
		else addru = D3DTADDRESS_CLAMP;
	if ( tClampMode == GR_TEXTURECLAMP_WRAP ) addrv = D3DTADDRESS_WRAP;
		else addrv = D3DTADDRESS_CLAMP;
	if ( (astate.clampmodeu != addru) || (astate.clampmodev != addrv) ) DrawFlushPrimitives(0);
	if (astate.clampmodeu != addru)	{
		for(i = 0; i < MAX_STAGETEXTURES; i++)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice,i,D3DTSS_ADDRESSU,astate.clampmodeu=addru);
	}
	if (astate.clampmodev != addrv)	{
		for(i = 0; i < MAX_STAGETEXTURES; i++)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice,i,D3DTSS_ADDRESSV,astate.clampmodev=addrv);
	}
	
	DEBUG_END("grTexClampMode",DebugRenderThreadId, 126);
}


void EXPORT grTexLodBiasValue(GrChipID_t tmu, float bias) {
int		i;

	DEBUG_BEGIN("grTexLodBiasValue",DebugRenderThreadId, 127);
	
	LOG(0,"\ngrTexLodBiasValue(tmu=%d, bias=%f)",tmu,bias);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	DrawFlushPrimitives(0);
	astate.lodBias = bias;
	for(i = 0; i < MAX_STAGETEXTURES; i++)
		lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice,i,D3DTSS_MIPMAPLODBIAS, *( (LPDWORD) (&bias)) );
	
	DEBUG_END("grTexLodBiasValue",DebugRenderThreadId, 127);
}


/* Egy adott Voodoo texmem-címre textúra feltöltése a megadott formátummal */
/* Megjegyz: gyors textúrafeltöltés */
void EXPORT grTexDownloadMipMap(GrChipID_t tmu, FxU32 startAddress,
								FxU32 evenOdd, GrTexInfo *info ) {

unsigned int	slod, llod, i;
void			*src;
TexCacheEntry	*tinfo;

	//return;
	DEBUG_BEGIN("grTexDownloadMipMap", DebugRenderThreadId, 128);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
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
	if ( (startAddress+(i = grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, info )) > config.TexMemSize)
		 && (startAddress < RESERVED_ADDRESSSPACE) ) return;
	
	/* */
#ifdef GLIDE3
	if (GlideCheckTexIntersect(startAddress, info->smallLodLog2, info->largeLodLog2, info->aspectRatioLog2,info->format))	{
#else
	if (GlideCheckTexIntersect(startAddress, info->smallLod, info->largeLod, info->aspectRatio,info->format))	{
#endif
		DrawFlushPrimitives(0);
		astate.acttex = NULL;
		callflags |= CALLFLAG_SETTEXTURE;
	}

	if (perfectEmulating && (startAddress < RESERVED_ADDRESSSPACE) )	{
		if ( (src = (textureMem + startAddress)) != info->data) CopyMemory(src, info->data, i);
	}
	
	i = 1;
	if ( (tinfo = TexFindTex(startAddress, NULL)) != NULL)	{
#ifdef GLIDE3
		slod = GlideGetLOD(tinfo->info.smallLodLog2);
		llod = GlideGetLOD(info->smallLodLog2);
		if ( (info->format == tinfo->info.format) &&
			 (info->largeLodLog2 == tinfo->info.largeLodLog2) &&
			 (info->aspectRatioLog2 == tinfo->info.aspectRatioLog2) &&
			 (info->smallLodLog2 == tinfo->info.smallLodLog2) )
				i = 0;
			 //(slod <= llod) ) i = 0;
#else
		slod = GlideGetLOD(tinfo->info.smallLod);
		llod = GlideGetLOD(info->smallLod);
		if ( (info->format == tinfo->info.format) &&
			 (info->largeLod == tinfo->info.largeLod) &&
			 (info->aspectRatio == tinfo->info.aspectRatio) &&
			 (info->smallLod == tinfo->info.smallLod) )
				i = 0;
			 //(slod <= llod) ) i = 0;
#endif
	}
	if (perfectEmulating && i && (startAddress < RESERVED_ADDRESSSPACE)) {
		TexInvalidateCache(startAddress, info);
		return;
	}

	if (i)	{
		if ( (tinfo = TexAllocCacheEntry(startAddress, info, NULL, 0)) == NULL ) return;
		if (!GlideCreateTexture(tinfo)) return;
	}
	tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
	src = info->data;
	for(i = 0; i < tinfo->mipMapCount; i++) src = GlideLoadTextureMipMap(tinfo, src, i, 1, 0, 1);
	
	DEBUG_END("grTexDownloadMipMap", DebugRenderThreadId, 128);
}


void EXPORT grTexDownloadMipMapLevelPartial(GrChipID_t tmu, FxU32 startAddress,
											GrLOD_t thisLod, GrLOD_t largeLod,
											GrAspectRatio_t aspectRatio,
											GrTextureFormat_t format,
											FxU32 evenOdd, void *data,
											unsigned int start,
											unsigned int end )	{

unsigned int	slod, tlod, llod, level=0, x, y;
unsigned int	toffset;
GrTexInfo		info;
TexCacheEntry	*tinfo;
int				i;

	//return;
	DEBUG_BEGIN("grTexDownloadMipMapLevelPartial", DebugRenderThreadId, 129);
	
	LOG(0,"\ngrTexDownloadMipMapLevelPartial(tmu=%d, startAddress=%0x, thisLod=%s, largeLod=%p, aspectRatio=%s, format=%s, evenOdd=%s, data=%p, \
											 start=%d, end=%d)", \
		tmu,startAddress,lod_t[thisLod],lod_t[largeLod], aspectr_t[aspectRatio], \
		tformat_t[format],evenOdd_t[evenOdd],data,start,end);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	startAddress &= ~7;
	if (start > end) return;
	x = GlideGetXY(thisLod, aspectRatio);
	y = x & 0xFFFF;
	x = (x >> 16) * GlideGetTexBYTEPP(format);
	toffset = grTexCalcMemRequired(thisLod, largeLod, aspectRatio, format)-grTexCalcMemRequired(thisLod, thisLod, aspectRatio, format);
	if ( (startAddress + toffset + end*x) > config.TexMemSize ) return;
	if (perfectEmulating) CopyMemory(textureMem + startAddress + toffset + (start * x), data, (end-start+1)*x );

	if (GlideCheckTexIntersect(startAddress, thisLod, thisLod, aspectRatio, format))	{
		DrawFlushPrimitives(0);
		astate.acttex = NULL;
		callflags |= CALLFLAG_SETTEXTURE;
	}

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

	i = 1;
	if ( (tinfo = TexFindTex(startAddress, NULL)) != NULL)	{

#ifdef GLIDE3
		//slod = GlideGetLOD(tinfo->info.smallLodLog2);
		if ( (format == tinfo->info.format) &&
			 (largeLod == tinfo->info.largeLodLog2) &&
			 (aspectRatio == tinfo->info.aspectRatioLog2) )
			 i = 0;
#else
		//slod = GlideGetLOD(tinfo->info.smallLod);
		if ( (format == tinfo->info.format) &&
			 (largeLod == tinfo->info.largeLod) &&
			(aspectRatio == tinfo->info.aspectRatio) )
			i = 0;
#endif
	}
	if (perfectEmulating && i) {
		TexInvalidateCache(startAddress, &info);
		return;
	}

	if (i)	{
		if ( (tinfo = TexAllocCacheEntry(startAddress, &info, NULL, 0)) == NULL ) return;
		if (!GlideCreateTexture(tinfo)) return;
	}
#ifdef GLIDE3
	slod = GlideGetLOD(tinfo->info.smallLodLog2);
#else
	slod = GlideGetLOD(tinfo->info.smallLod);
#endif
	tlod = GlideGetLOD(thisLod);
	llod = GlideGetLOD(largeLod);
	if ( (slod > tlod) || (tlod > llod) ) return;
	
	for(level = 0; llod != tlod; llod>>=1) level++;
	
	if ( (start >= y) || (end >= y) ) return;

	tinfo->colorKeyAlphaBased = INVALID_COLORKEY;
	GlideLoadTextureMipMap(tinfo, data, level, start, end, 1);
	
	DEBUG_END("grTexDownloadMipMapLevelPartial", DebugRenderThreadId, 129);
}


void EXPORT grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress,
									  GrLOD_t thisLod, GrLOD_t largeLod,
									  GrAspectRatio_t aspectRatio,
									  GrTextureFormat_t format,
									  FxU32 evenOdd, void *data )	{

	//return;
	DEBUG_BEGIN("grTexDownloadMipMapLevel", DebugRenderThreadId, 130);
	LOG(0,"\ngrTexDownloadMipMapLevel(tmu=%d, startAddress=%0x, thisLod=%s, largeLod=%p, aspectRatio=%s, format=%s, evenOdd=%s, data=%p)",
		tmu,startAddress,lod_t[thisLod],lod_t[largeLod], aspectr_t[aspectRatio],
		tformat_t[format],evenOdd_t[evenOdd],data);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;

	grTexDownloadMipMapLevelPartial(0, startAddress, thisLod, largeLod, aspectRatio,
									format, evenOdd, data, 0, (GlideGetXY(thisLod,aspectRatio) & 0xFFFF)-1);
	
	DEBUG_END("grTexDownloadMipMapLevel", DebugRenderThreadId, 130);
}


/* Egy adott Voodoo texmem-cím mint textúraforrás beállítása */
void EXPORT grTexSource(GrChipID_t tmu, FxU32 startAddress, 
						FxU32 evenOdd, GrTexInfo *info ) {
unsigned int	x,y;

	//return;
	DEBUG_BEGIN("grTexSource", DebugRenderThreadId, 131);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	LOG(0,"\ngrTexSource(tmu=%d, startAddress=%0x, evenOdd=%s, info=%p)",
		tmu,startAddress, evenOdd_t[evenOdd], info);
	LOG(0,"\n   info->smallLod = %s, \
		\n   info->largeLod = %s, \
		\n   info->aspectRatio = %s, \
		\n   info->format = %s, \
		\n   info->data = %0x", lod_t[info->smallLod], lod_t[info->largeLod],
		aspectr_t[info->aspectRatio], tformat_t[info->format], info->data);	
	
	if (tmu != 0) return;

#ifdef GLIDE3
	if ( (astate.smallLod == (unsigned char) info->smallLodLog2) &&
		 (astate.largeLod == (unsigned char) info->largeLodLog2) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatioLog2) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[0] == startAddress )) return;
#else
	if ( (astate.smallLod == (unsigned char) info->smallLod) &&
		 (astate.largeLod==(unsigned char) info->largeLod) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatio) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[0] == startAddress )) return;
#endif
	DrawFlushPrimitives(0);
	astate.startAddress[0] = startAddress;
	astate.format = (unsigned char) info->format;

#ifdef GLIDE3
	astate.smallLod = (unsigned char) info->smallLodLog2;
	astate.largeLod = (unsigned char) info->largeLodLog2;
	astate.aspectRatio = (unsigned char) info->aspectRatioLog2;
	x = GlideGetXY(info->largeLodLog2,info->aspectRatioLog2);
#else
	astate.smallLod = (unsigned char) info->smallLod;
	astate.largeLod = (unsigned char) info->largeLod;
	astate.aspectRatio = (unsigned char) info->aspectRatio;
	x = GlideGetXY(info->largeLod,info->aspectRatio);
#endif

	y = x & 0xFFFF;
	x >>= 16;
	if (x >= y)	{
		astate.divx = 256.0f;
		astate.divy = (float) ((256*y)/x);
	} else {
		astate.divy = 256.0f;
		astate.divx = (float) ((256*x)/y);
	}

	callflags |= CALLFLAG_SETTEXTURE;
	
	DEBUG_END("grTexSource", DebugRenderThreadId, 131);
}


/* megjegyz: ditherelt mipmappinget nem kezel. Helyette trilineárisra állítja be. */
void EXPORT grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend ) {

DWORD val;
int i;

	//return;
	DEBUG_BEGIN("grTexMipMapMode", DebugRenderThreadId, 132);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	switch(mode)	{
		case GR_MIPMAP_NEAREST:
			val = (lodBlend == FXTRUE) ? D3DTFP_LINEAR : D3DTFP_POINT;
			break;
		case GR_MIPMAP_NEAREST_DITHER:
			val = D3DTFP_LINEAR;
			break;
		default:
			val = (config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ? D3DTFP_LINEAR : D3DTFP_NONE;
	}
	if ( (config.Flags2 & CFG_FORCETRILINEARMIPMAPS) && (val != D3DTFP_NONE) ) val = D3DTFP_LINEAR;
	if (astate.mipMapMode != val)	{
		DrawFlushPrimitives(0);
		for(i = 0; i <= 3; i++)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MIPFILTER, astate.mipMapMode = val);
	}
	
	DEBUG_END("grTexMipMapMode",DebugRenderThreadId, 132);
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
						 FxBool alpha_invert)	{

LPDIRECTDRAWSURFACE7	lpDDTexture,lpDDAlpha;
unsigned int			testc,testa;

	DEBUG_BEGIN("grTexCombine", DebugRenderThreadId, 161);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	LOG(0,"\ngrTexCombine(tmu=%d, rgb_func=%s, rgb_fact=%s, alpha_func=%s, alpha_fact=%s, rgb_inv=%s, alpha_inv=%s",
		tmu, combfunc_t[rgb_function], combfact_t[rgb_factor], 
		combfunc_t[alpha_function], combfact_t[alpha_factor],
		fxbool_t[rgb_invert], fxbool_t[alpha_invert]);
	if ( (rgb_function == astate.rgb_function) && (rgb_factor == astate.rgb_factor) && (rgb_invert == astate.rgb_invert) &&
		 (alpha_function == astate.alpha_function) && (alpha_factor == astate.alpha_factor) && (alpha_invert == astate.alpha_invert) )
		return;
	DrawFlushPrimitives(0);
	astate.rgb_function = (unsigned char) rgb_function;
	astate.rgb_factor = (unsigned char) rgb_factor;
	astate.rgb_invert = (unsigned char) rgb_invert;
	astate.alpha_function = (unsigned char) alpha_function;
	astate.alpha_factor = (unsigned char) alpha_factor;
	astate.alpha_invert = (unsigned char) alpha_invert;
	lpDDTexture = lpDDAlpha = NULL;
	if (texCombineFuncImp[rgb_function] == TCF_ZERO) lpDDTexture = nullTexture->texBase[0];
	if (texCombineFuncImp[alpha_function] == TCF_ZERO) lpDDAlpha = nullTexture->texBase[0];
	if (lpDDTexture == lpDDAlpha) lpDDAlpha = NULL;
	astate.lpDDTCCTex = lpDDTexture;
	astate.lpDDTCATex = lpDDAlpha;
	if (texCombineFuncImp[rgb_function] == TCF_ALOCAL) astate.flags |= STATE_TCLOCALALPHA;
		else astate.flags &= ~STATE_TCLOCALALPHA;
	callflags |= CALLFLAG_COLORCOMBINE;
	testc = testa = 0;
	if (rgb_invert) testc = STATE_TCCINVERT;
	if (alpha_invert) testa = STATE_TCAINVERT;
	if ( (astate.flags & STATE_TCCINVERT) != testc)	{
		astate.flags ^= STATE_TCCINVERT;
		callflags |= CALLFLAG_COLORCOMBINE;
	}
	if ( (astate.flags & STATE_TCAINVERT) != testa)	{
		astate.flags ^= STATE_TCAINVERT;
		callflags |= CALLFLAG_ALPHACOMBINE;
	}
	GlideColorCombineUpdateVertexMode();
	GlideAlphaCombineUpdateVertexMode();
	callflags |= CALLFLAG_RESETTEXTURE;

	DEBUG_END("grTexCombine", DebugRenderThreadId, 161);
}


void EXPORT grTexDownloadTable(GrChipID_t tmu, GrTexTable_t type, void *data )	{
//unsigned int	i;

	//return;
	DEBUG_BEGIN("grTexDownloadTable", DebugRenderThreadId, 133);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;	
	LOG(1,"\ngrTexDownloadTable(tmu=%d, type=%s, data=%p)",tmu, textable_t[type], data);
	DrawFlushPrimitives(0);
	switch(type)	{
		case GR_TEXTABLE_PALETTE:
			if (TexCopyPalette(&palette, data, 256)) return;
			/*#if LOGGING
				LOG(0,"\nPalette:");
				for(i = 0;i < 16; i++) LOG(0,"\n\t0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x",
										palette.data[i*16+0],palette.data[i*16+1],palette.data[i*16+2],palette.data[i*16+3],
										palette.data[i*16+4],palette.data[i*16+5],palette.data[i*16+6],palette.data[i*16+7],
										palette.data[i*16+8],palette.data[i*16+9],palette.data[i*16+10],palette.data[i*16+11],
										palette.data[i*16+12],palette.data[i*16+13],palette.data[i*16+14],palette.data[i*16+15]);
			#endif*/
			if (!(astate.flags & STATE_NATIVECKMETHOD)) callflags |= CALLFLAG_SETCOLORKEY;
			callflags |= CALLFLAG_SETPALETTE;
			break;
		case GR_NCCTABLE_NCC0:
			TexCopyNCCTable(&nccTable0, data);
			if (!(astate.flags & STATE_NATIVECKMETHOD)) callflags |= CALLFLAG_SETCOLORKEY;
			callflags |= CALLFLAG_SETNCCTABLE;
			break;
		case GR_NCCTABLE_NCC1:
			TexCopyNCCTable(&nccTable1, data);
			if (!(astate.flags & STATE_NATIVECKMETHOD)) callflags |= CALLFLAG_SETCOLORKEY;
			callflags |= CALLFLAG_SETNCCTABLE;
			break;
	}		
	
	DEBUG_END("grTexDownloadTable", DebugRenderThreadId, 133);
}


void EXPORT grTexDownloadTablePartial(GrChipID_t tmu, GrTexTable_t type, void *data,
									  int start, int end)	{
FxU32	*pdata = data;
	
	//return;
	DEBUG_BEGIN("grTexDownloadTablePartial", DebugRenderThreadId, 134);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (tmu != 0) || (type != GR_TEXTABLE_PALETTE) ) return;
	LOG(0,"\ngrTexDownloadTablePartial(tmu=%d, type=%s, data=%p, start=%d, end=%d)",tmu, textable_t[type], data, start, end);
	DrawFlushPrimitives(0);
	if (TexCopyPalette((GuTexPalette *) (&(palette.data[start])), (GuTexPalette *) &(((GuTexPalette *) pdata)->data[start]), (end-start+1))) return;
	if (!(astate.flags & STATE_NATIVECKMETHOD)) callflags |= CALLFLAG_SETCOLORKEY;
	callflags |= CALLFLAG_SETPALETTE;

	DEBUG_END("grTexDownloadTablePartial", DebugRenderThreadId, 134);
}


void EXPORT grTexNCCTable(GrChipID_t tmu, GrNCCTable_t table) {
GuNccTable	*ncctable;

	DEBUG_BEGIN("grTexNCCTable", DebugRenderThreadId, 135);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (tmu != 0) return;
	LOG(0,"\ngrTexNCCTable(tmu=%d, table=%s)",tmu,ncctable_t[table]);
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
		DrawFlushPrimitives(0);
		actncctable = ncctable;
		callflags |= CALLFLAG_SETNCCTABLE;
	}
	
	DEBUG_END("grTexNCCTable", DebugRenderThreadId, 135);
}


void EXPORT grTexMultibase(GrChipID_t tmu, FxBool enable)	{
unsigned char	mode;

	LOG(0,"\ngrTexMultibase(tmu=%d,enable=%s)",tmu,fxbool_t[enable]);
	
	mode = (enable) ? 1 : 0;
	if (mode == multiBaseMode) return;
	DrawFlushPrimitives(0);
	multiBaseMode = mode;
	callflags |= CALLFLAG_SETTEXTURE;
}


void EXPORT grTexMultibaseAddress(GrChipID_t tmu, GrTexBaseRange_t range, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info)	{
unsigned int	x,y;

	LOG(0,"\ngrTexMultibaseAddress(tmu=%d, range=%s, startAddress=%0x, evenOdd=%s, info=%p)",tmu,texbaserange_t[range],startAddress,evenOdd_t[evenOdd],info);
	LOG(0,"\n     info->smallLod=%s,\n     info->largeLod=%s,\n     info->aspectRatio=%s,\n     info->format=%s",
			lod_t[info->smallLod], lod_t[info->largeLod], aspectr_t[info->aspectRatio], tformat_t[info->format]);
	if (tmu != 0) return;
	switch(range)	{
		case GR_TEXBASE_256:
			x = 0;
			break;
		case GR_TEXBASE_128:
			x = 1;
			break;
		case GR_TEXBASE_64:
			x = 2;
			break;
		case GR_TEXBASE_32_TO_1:
			x = 3;
			break;
	}

#ifdef GLIDE3
	if ( (astate.smallLod == (unsigned char) info->smallLodLog2) &&
		 (astate.largeLod == (unsigned char) info->largeLodLog2) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatioLog2) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[x] == startAddress )) return;
#else
	if ( (astate.smallLod == (unsigned char) info->smallLod) &&
		 (astate.largeLod == (unsigned char) info->largeLod) &&
		 (astate.aspectRatio == (unsigned char) info->aspectRatio) &&
		 (astate.format == (unsigned char) info->format) &&
		 (astate.startAddress[x] == startAddress )) return;
#endif
	
	DrawFlushPrimitives(0);
	astate.startAddress[x] = startAddress;
	astate.format = (unsigned char) info->format;

#ifdef GLIDE3
	astate.smallLod = (unsigned char) info->smallLodLog2;
	astate.largeLod = (unsigned char) info->largeLodLog2;
	astate.aspectRatio = (unsigned char) info->aspectRatioLog2;
	x = GlideGetXY(info->largeLodLog2,info->aspectRatioLog2);
#else
	astate.smallLod = (unsigned char) info->smallLod;
	astate.largeLod = (unsigned char) info->largeLod;
	astate.aspectRatio = (unsigned char) info->aspectRatio;
	x = GlideGetXY(info->largeLod,info->aspectRatio);
#endif

	y = x & 0xFFFF;
	x >>= 16;
	if (x >= y)	{
		astate.divx = 256.0f;
		astate.divy = (float) ((256*y)/x);
	} else {
		astate.divy = 256.0f;
		astate.divx = (float) ((256*x)/y);
	}
	callflags |= CALLFLAG_SETTEXTURE;
}


/*------------------------------------------------------------------------------------------*/
/*------------------------ A két fájlkezel¾rutin a textúrázáshoz ---------------------------*/

/* segédrutin: a line pufferben visszaadja a fájl egy nemkomment sorát */
int GlideGet3DFline(HANDLE file, unsigned char *line, unsigned char *buffer,
					unsigned int *pos, unsigned int *read, unsigned int *asciiend)	{
unsigned char	*linechar, *actchar = buffer + *pos;
unsigned int	comment = 1, linelength;
	
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
	if (!GlideComp3DFstring(line, "3df v", 5)) return(0);
	if (!(len=GlideGet3DFnumber(line + 5, &number))) return(0);
	if (*(line + (linepos = len)+5) != '.') return(0);
	if (!(len=GlideGet3DFnumber(line+6+len, &number))) return(0);
	if (*(line+linepos+len+6) != 0x0) return(0);
/* Második sor beolvasása, és a textúraformátum kiderítése */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	for(number = 0; number < 13; number++)	{
		if (GlideComp3DFstring(line, textfmtstr[number], 0xFFFFFFFF))	{
			info->header.format = textfmtconst[number];
			break;
		}
	}
	if (number == 13) return(0);

/* Harmadik sor beolv, és a LOD range megállapítása */
	if (!GlideGet3DFline(file, line, buffer, &buffpos, &read, &asciiend)) return(0);
	if (!GlideComp3DFstring(line, "lod range: ", 11)) return(0);
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
	if (!GlideComp3DFstring(line, "aspect ratio: ", 14)) return(0);
	for(number = 0; number < 7; number++)	{
		if (GlideComp3DFstring(line+14, textasprats[number], 0xFFFFFFFF))	{
			info->header.aspect_ratio = textaspratconst[number];
			break;
		}
	}
	if (number == 7) return(0);
	linepos = GlideGetXY(info->header.large_lod, info->header.aspect_ratio);
	info->header.width = linepos >> 16;
	info->header.height = linepos & 0xFFFF;
	info->mem_required = grTexCalcMemRequired(info->header.small_lod,
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
	
 	DEBUG_BEGIN("gu3dfGetInfo", DebugRenderThreadId, 136);
	
	LOG(0,"\ngu3dfGetInfo(filename=\"%s\", info=%p)",filename,info);
	if ((file = _OpenFile((unsigned char *) filename)) == INVALID_HANDLE_VALUE) return(FXFALSE);
	r = GlideGet3DFinfo(file,info);
	_CloseFile(file);
	return(r ? FXTRUE : FXFALSE);
	
	DEBUG_END("gu3dfGetInfo", DebugRenderThreadId, 136);
}


FxBool EXPORT gu3dfLoad(const char *filename, Gu3dfInfo *info)	{
HANDLE			file;
unsigned int	imagepos;
void			*image;

	DEBUG_BEGIN("gu3dfLoad", DebugRenderThreadId, 137);
	
	LOG(0,"\ngu3dfLoad(filename=\"%s\", info=%p)",filename,info);
	if ((file = _OpenFile((unsigned char*)filename)) == INVALID_HANDLE_VALUE) return(FXFALSE);
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
	return(FXTRUE);

	DEBUG_END("gu3dfLoad", DebugRenderThreadId, 137);
}

/*-------------------------------------------------------------------------------------------*/
/*------------------------------- Chroma keying függvények ----------------------------------*/

void EXPORT grChromakeyMode(GrChromakeyMode_t mode)	{
DWORD	ckstate = FALSE;

	DEBUG_BEGIN("grChromakeyMode", DebugRenderThreadId, 138);
	
	LOG(0,"\ngrChromakeyMode(mode=%s)",chromakeymode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( mode == GR_CHROMAKEY_ENABLE ) ckstate = TRUE;
	if (ckstate != (DWORD) astate.colorKeyEnable)	{
		DrawFlushPrimitives(0);
		astate.colorKeyEnable = (unsigned char) ckstate;
		
		if (astate.flags & STATE_NATIVECKMETHOD) callflags |= CALLFLAG_SETCOLORKEYSTATE;
		callflags |= CALLFLAG_SETCOLORKEY;
	}
	GlideUpdateConstantColorKeyingState();
	
	DEBUG_END("grChromakeyMode", DebugRenderThreadId, 138);
}


void EXPORT grChromakeyValue(GrColor_t value)	{
DWORD colorKey;
	
	DEBUG_BEGIN("grChromakeyValue", DebugRenderThreadId, 139);
	
	LOG(0,"\ngrChromakeyValue(value=%0x)",value);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN) return;
	if ( (colorKey = GlideGetColor(value)) == astate.colorkey) return;
	LOG(0,"\n	grChromakeyValue(ARGB value=%0x)", colorKey);

	astate.colorkey = colorKey;
	if (astate.colorKeyEnable) DrawFlushPrimitives(0);

	/* A chromakey comparing 24 biten történik, ezért az alfát nem veszi figyelembe. */
	/* A natív DirectX-es colorkeying viszont minden komponenst figyelembe vesz: */
	/* - ha a textúra alfa nélküli, akkor mindegy */
	/* - ha a textúra alfás, de alfa nélkülirõl lett konvertálva, akkor a konstans alfa 0xFF, ezért ezt itt is beállítjuk */
	/* - ha alfás 3dfx-es textúráról van szó, akkor az esetleg eltérõ alfa értékek miatt a natív ck nem fog mûködni :( */
	colorKey |= 0xFF000000;

	dxTexFormats[Pf16_ARGB4444].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf16_ARGB4444].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_ARGB4444].pixelFormat);
	dxTexFormats[Pf16_ARGB1555].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf16_ARGB1555].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_ARGB1555].pixelFormat);
	dxTexFormats[Pf16_RGB555].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf16_RGB555].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_RGB555].pixelFormat);
	dxTexFormats[Pf16_RGB565].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf16_RGB565].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf16_RGB565].pixelFormat);
	dxTexFormats[Pf32_ARGB8888].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf32_ARGB8888].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf32_ARGB8888].pixelFormat);
	dxTexFormats[Pf32_RGB888].colorKey.dwColorSpaceHighValue = 
		dxTexFormats[Pf32_RGB888].colorKey.dwColorSpaceLowValue = MMCGetPixelValueFromARGB(colorKey, &dxTexFormats[Pf32_RGB888].pixelFormat);

	if (dxTexFormats[Pf8_P8].isValid)	{
		colorKeyPal = TexGetTexIndexValue(0, colorKey, &palette);
		colorKeyNcc0 = TexGetTexIndexValue(1, colorKey, &nccTable0);
		colorKeyNcc1 = TexGetTexIndexValue(1, colorKey, &nccTable1);
	}
	GlideUpdateConstantColorKeyingState();
	callflags |= CALLFLAG_SETCOLORKEY;
	
	DEBUG_END("grChromakeyValue", DebugRenderThreadId, 139);
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
										FxBool lodBlend )	{

unsigned int	xy;
DDSURFACEDESC2	textdesc={sizeof(DDSURFACEDESC2)};
GrMipMapInfo	*_mipmapinfo;
		
	DEBUG_BEGIN("guTexAllocateMemory",DebugRenderThreadId, 140);
	
	LOG(0,"\nguTexAllocateMemory(tmu=%d, evenOddMask=%s, width=%d, height=%d, format=%s, mmMode=%s, \
		   smallLod=%s, largeLod=%s, aspectRatio=%s, sClampMode=%s, tClampMode=%s, \
		   minFilterMode=%s, magFilterMode=%s, lodBias=%f, lodBlend=%s)",
		   tmu, evenOdd_t[evenOddMask], width, height, tformat_t[format],
		   mipmapmode_t[mmMode], lod_t[smallLod], lod_t[largeLod],
		   aspectr_t[aspectRatio], clampmode_t[sClampMode], clampmode_t[tClampMode],
		   filtermode_t[minFilterMode], filtermode_t[magFilterMode],
		   lodBias, fxbool_t[lodBlend]);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return(GR_NULL_MIPMAP_HANDLE);
	if (tmu) return(GR_NULL_MIPMAP_HANDLE);

	if (mipmapinfos == NULL) if ( (mipmapinfos = (GrMipMapInfo *) _GetMem(MAX_UTIL_TEXTURES * sizeof(GrMipMapInfo))) == NULL) return(GR_NULL_MIPMAP_HANDLE);
	if (mmidIndex == (MAX_UTIL_TEXTURES-1)) return(GR_NULL_MIPMAP_HANDLE);
	_mipmapinfo = mipmapinfos + mmidIndex;

	xy = GlideGetXY(largeLod, aspectRatio);
	if ( (xy >> 16) != (unsigned int) width ) return(GR_NULL_MIPMAP_HANDLE);
	if ( (xy & 0xFFFF) != (unsigned int) height ) return(GR_NULL_MIPMAP_HANDLE);
	
	_mipmapinfo->sst = 0;
	_mipmapinfo->valid = FXTRUE;
	_mipmapinfo->width = width;
	_mipmapinfo->height = height;
	_mipmapinfo->aspect_ratio = aspectRatio;
	_mipmapinfo->data = NULL;
	_mipmapinfo->format = format;
	_mipmapinfo->mipmap_mode = mmMode;
	_mipmapinfo->magfilter_mode = magFilterMode;
	_mipmapinfo->minfilter_mode = minFilterMode;
	_mipmapinfo->s_clamp_mode = sClampMode;
	_mipmapinfo->t_clamp_mode = tClampMode;
	_mipmapinfo->tLOD = 'DEGE';
	_mipmapinfo->tTextureMode = 'DEGE';
	_mipmapinfo->lod_bias = *((FxU32 *) &lodBias);
	_mipmapinfo->lod_min = smallLod;
	_mipmapinfo->lod_max = largeLod;
	_mipmapinfo->tmu = 0;
	_mipmapinfo->odd_even_mask = evenOddMask;
	_mipmapinfo->tmu_base_address = utilMemFreeBase;
	_mipmapinfo->trilinear = lodBlend;
	utilMemFreeBase += grTexCalcMemRequired(smallLod, largeLod, aspectRatio, format);
	
	return(mmidIndex++);
	
	DEBUG_END("guTexAllocateMemory",DebugRenderThreadId, 140);
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
									GrTextureFilterMode_t magFilterMode )	{
GrMipMapInfo *_mipmapinfo;

	DEBUG_BEGIN("guTexChangeAttributes", DebugRenderThreadId, 141);
	
	LOG(0,"\nguTexChangeAttributes(mmid=%0x, width=%d, height=%d, format=%s, mmMode=%s, \
		   smallLod=%s, largeLod=%s, aspectRatio=%s, sClampMode=%s, tClampMode=%s, \
		   minFilterMode=%s, magFilterMode=%s)",
		   mmid, width, height, tformat_t[format],
		   mipmapmode_t[mmMode], lod_t[smallLod], lod_t[largeLod],
		   aspectr_t[aspectRatio], clampmode_t[sClampMode], clampmode_t[tClampMode],
		   filtermode_t[minFilterMode], filtermode_t[magFilterMode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return(FXFALSE);
	if ( (mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex) ) return(FXFALSE);
	_mipmapinfo = mipmapinfos + mmid;
	if (width != -1) _mipmapinfo->width=width;
	if (height != -1) _mipmapinfo->height=height;
	if (format != -1) _mipmapinfo->format=format;
	if (mmMode != -1) _mipmapinfo->mipmap_mode=mmMode;
	if (smallLod != -1) _mipmapinfo->lod_min=smallLod;
	if (largeLod != -1) _mipmapinfo->lod_max=largeLod;
	if (aspectRatio != -1) _mipmapinfo->aspect_ratio=aspectRatio;
	if (sClampMode != -1) _mipmapinfo->s_clamp_mode=sClampMode;
	if (tClampMode != -1) _mipmapinfo->t_clamp_mode=tClampMode;
	if (minFilterMode != -1) _mipmapinfo->minfilter_mode=minFilterMode;
	if (magFilterMode != -1) _mipmapinfo->magfilter_mode=magFilterMode;

	return(FXTRUE);
	
	DEBUG_END("guTexChangeAttributes", DebugRenderThreadId, 141);
}


void EXPORT guTexDownloadMipMap(GrMipMapId_t mmid, const void *src,
								const GuNccTable *nccTable )	{
GrMipMapInfo	*_mipmapinfo;
GrTexInfo		info;

	DEBUG_BEGIN("guTexDownloadMipMap",DebugRenderThreadId, 142);
	
	LOG(0,"\nguTexDownloadMipMap(mmid=%0x, src=%p, nccTable=%p)",mmid,src,nccTable);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex) ) return;
	
	if (mmid == astate.actmmid) DrawFlushPrimitives(0);
	_mipmapinfo = mipmapinfos + mmid;
	
	info.smallLod = _mipmapinfo->lod_min;
	info.largeLod = _mipmapinfo->lod_max;
	info.format = _mipmapinfo->format;
	info.aspectRatio = _mipmapinfo->aspect_ratio;
	info.data = _mipmapinfo->data = (void *) src;

	grTexDownloadMipMap(0, _mipmapinfo->tmu_base_address, GR_MIPMAPLEVELMASK_BOTH, &info);

	if ( ((_mipmapinfo->format == GR_TEXFMT_YIQ_422) || (_mipmapinfo->format == GR_TEXFMT_AYIQ_8422)) && (nccTable != NULL) )
		TexCopyNCCTable(&_mipmapinfo->ncc_table, (GuNccTable *) nccTable);

	DEBUG_END("guTexDownloadMipMap",DebugRenderThreadId, 142);
}


void EXPORT guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src )	{
unsigned int lodsize;
GrMipMapInfo *_mipmapinfo;

	DEBUG_BEGIN("guTexDownloadMipMapLevel",DebugRenderThreadId, 143);

	LOG(0,"\nguTexDownloadMipMapLevel(mmid=%0x, lod=%s, src=%p)",mmid,lod_t[lod],src);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex) ) return;

	if (mmid==astate.actmmid) DrawFlushPrimitives(0);
	_mipmapinfo=mipmapinfos+mmid;
	
	DEBUGCHECK(0,( (lod < _mipmapinfo->lod_min) || (lod > _mipmapinfo->lod_max) ),
		         "\n   Warning: guTexDownloadMipMapLevel: specified lod is out of range!");
	DEBUGCHECK(1,( (lod < _mipmapinfo->lod_min) || (lod > _mipmapinfo->lod_max) ),
		         "\n   Figyelmeztetés: guTexDownloadMipMapLevel: a megadott lod nem esik a megfelelõ tartományba!");

	lodsize=grTexCalcMemRequired(lod,lod,_mipmapinfo->aspect_ratio,_mipmapinfo->format);
	grTexDownloadMipMapLevel(0,_mipmapinfo->tmu_base_address,lod,_mipmapinfo->lod_max,_mipmapinfo->lod_min,
								_mipmapinfo->format, _mipmapinfo->odd_even_mask, (void *) *src);

	((char *) (*src))+=lodsize;
	
	DEBUG_END("guTexDownloadMipMapLevel",DebugRenderThreadId, 143);
}


GrMipMapId_t EXPORT guTexGetCurrentMipMap ( GrChipID_t tmu )	{
	
	LOG(0,"\nguTexGetCurrentMipMap(tmu=%d)",tmu);
	return tmu == 0 ? astate.actmmid : GR_NULL_MIPMAP_HANDLE;
}


unsigned int EXPORT guTexGetMipMapInfo( GrMipMapId_t mmid ) {

	LOG(0,"\nguTexGetMipMapInfo(mmid=%0x)",mmid);
	if (mmid >= mmidIndex) return((unsigned int) NULL);
	return((unsigned int) (mipmapinfos + mmid));
}


int GlideCopyMipMapInfo(GrMipMapInfo *dst, GrMipMapId_t mmid)	{
	
	if ( (mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex) ) return(0);
	CopyMemory(dst, mipmapinfos + mmid, sizeof(GrMipMapInfo));
	return(1);
}


void EXPORT guTexMemReset( void )	{

	DEBUG_BEGIN("guTexMemReset", DebugRenderThreadId, 144);
	
	LOG(0,"\nguTexMemReset()");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	utilMemFreeBase = 0;
	mmidIndex = 0;
	astate.actmmid = GR_NULL_MIPMAP_HANDLE;
	
	DEBUG_END("guTexMemReset", DebugRenderThreadId, 144);
}


FxU32 EXPORT guTexMemQueryAvail( GrChipID_t tmu )	{

	if (tmu!=0) return(0);
		else return(config.TexMemSize - utilMemFreeBase);
}


void EXPORT guTexSource( GrMipMapId_t mmid )	{
GrMipMapInfo *_mipmapinfo;
GrTexInfo info;

	DEBUG_BEGIN("guTexSource",DebugRenderThreadId, 145);
	
	LOG(0,"\nguTexSource(mmid=%0x)",mmid);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;

	if ( (mmid == GR_NULL_MIPMAP_HANDLE) || (mmid >= mmidIndex) ) return;
	DrawFlushPrimitives(0);
	_mipmapinfo=mipmapinfos+mmid;

	info.smallLod = _mipmapinfo->lod_min;
	info.largeLod = _mipmapinfo->lod_max;
	info.format = _mipmapinfo->format;
	info.aspectRatio = _mipmapinfo->aspect_ratio;
	info.data = _mipmapinfo->data;
	grTexSource(0, _mipmapinfo->tmu_base_address, _mipmapinfo->odd_even_mask, &info);

	astate.actmmid = mmid;
	
	if ( (_mipmapinfo->format == GR_TEXFMT_YIQ_422) || (_mipmapinfo->format == GR_TEXFMT_AYIQ_8422) )	{
		grTexDownloadTable(0, (actncctable == &nccTable0) ? GR_TEXTABLE_NCC0 : GR_TEXTABLE_NCC1, &_mipmapinfo->ncc_table);
	}
	grTexClampMode(0, _mipmapinfo->s_clamp_mode, _mipmapinfo->t_clamp_mode);
	grTexFilterMode(0, _mipmapinfo->minfilter_mode, _mipmapinfo->magfilter_mode);
	grTexMipMapMode(0, _mipmapinfo->mipmap_mode, _mipmapinfo->trilinear);
	grTexLodBiasValue(0, *((float *) &_mipmapinfo->lod_bias) );
	callflags |= CALLFLAG_SETPALETTE;

	DEBUG_END("guTexSource",DebugRenderThreadId, 145);
}


unsigned int GlideGetUTextureSize(GrMipMapId_t mmid, GrLOD_t lod, int lodvalid)	{
GrMipMapInfo *_mipmapinfo;

	DEBUG_BEGIN("GlideGetUTextureSize",DebugRenderThreadId, 146);

	if ( (mmid >= mmidIndex) || (mmid == GR_NULL_MIPMAP_HANDLE) ) return(0);
	if (mmid == GR_NULL_MIPMAP_HANDLE) return(0);
	_mipmapinfo = mipmapinfos + mmid;
	if (lodvalid) return( grTexCalcMemRequired(lod,lod,_mipmapinfo->aspect_ratio,_mipmapinfo->format) );
		else return ( grTexCalcMemRequired(_mipmapinfo->lod_min, _mipmapinfo->lod_max,_mipmapinfo->aspect_ratio,_mipmapinfo->format) );
	
	DEBUG_END("GlideGetUTextureSize",DebugRenderThreadId, 146);
}

#endif


void EXPORT guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func ) {
	
	LOG(0,"\nguTexCombineFunction(tmu=%0x, func=%s)",tmu,texturecombinefnc_t[func]);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (tmu) return;
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

/* A következô initrutin callback-függvénye */
HRESULT CALLBACK TexEnumFormatsCallback(LPDDPIXELFORMAT lpDDpf, void *constPalettes)	{
_pixfmt			enumPixFmt;
NamedFormat		*destNamedFormat;
int				i;

	DEBUG_BEGIN("TexEnumFormatsCallback", DebugRenderThreadId, 147);
	
	if (lpDDpf->dwFourCC != 0) return(D3DENUMRET_OK);
	/*if ( (lpDDpf->dwFlags & (DDPF_LUMINANCE | DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) || 
		 (lpDDpf->dwFourCC != 0) ) return(D3DENUMRET_OK);*/
	
	i = Pf_Invalid;
	
	/* Ha a kártya esetleg támogatja a 8 bites palettás textúrákat */
	if ( (lpDDpf->dwRGBBitCount==8) && (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) &&
		(lpDDpf->dwFlags & DDPF_PALETTEINDEXED8) &&
		(!(config.Flags & (CFG_TEXTURES16BIT | CFG_TEXTURES32BIT))) )	{
		i = Pf8_P8;
		ZeroMemory(&enumPixFmt, sizeof(_pixfmt));
		enumPixFmt.BitPerPixel = 8;

	} else {

		if ( ((lpDDpf->dwFlags & DDPF_BUMPDUDV) == DDPF_BUMPDUDV) ||
			((lpDDpf->dwFlags & (DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) == (DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) ) {
			_asm nop
			return(D3DENUMRET_OK);
		}

		/* Egyébként megvizsgáljuk a 16 és 32 bites formátumokat */
		if ( (lpDDpf->dwRGBBitCount != 16) && (lpDDpf->dwRGBBitCount != 32) ) return(D3DENUMRET_OK);
		MMCConvertToPixFmt(lpDDpf, &enumPixFmt, 0);

		switch(lpDDpf->dwRGBBitCount)	{
			case 32:				
				if (config.Flags & CFG_TEXTURES16BIT) return(D3DENUMRET_OK);
				if ( (enumPixFmt.RBitCount != 8) || (enumPixFmt.GBitCount != 8) || (enumPixFmt.BBitCount != 8) ) return(D3DENUMRET_OK);
				if (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS ))	{
					i = Pf32_RGB888;
				} else {
					if (enumPixFmt.ABitCount == 8) i = Pf32_ARGB8888;
				}
				break;
			case 16:
				if (config.Flags & CFG_TEXTURES32BIT) return(D3DENUMRET_OK);
				/* Ellenôrzés az ARGB 4444 formátumhoz */
				if ( (enumPixFmt.RBitCount == 4) && (enumPixFmt.GBitCount == 4) && (enumPixFmt.BBitCount == 4) && (enumPixFmt.ABitCount == 4) &&
					 (lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) {
					i = Pf16_ARGB4444;
				}
				/* Ellenôrzés a ARGB 1555 formátumhoz */
				if ( (enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5) && (enumPixFmt.ABitCount == 1) &&
				     (lpDDpf->dwFlags & DDPF_ALPHAPIXELS)) {
					i = Pf16_ARGB1555;
				}
				/* Ellenôrzés a RGB 555 formátumhoz */
				if ( (enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5) &&
					 (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS))) {
					i = Pf16_RGB555;
				}
				/* Ellenôrzés a RGB 565 formátumhoz */
				if ( ((enumPixFmt.RBitCount == 6) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 5)) ||
					 ((enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 6) && (enumPixFmt.BBitCount == 5)) ||
					 ((enumPixFmt.RBitCount == 5) && (enumPixFmt.GBitCount == 5) && (enumPixFmt.BBitCount == 6)) &&
					 (!(lpDDpf->dwFlags & DDPF_ALPHAPIXELS))) {

					i = Pf16_RGB565;
				}
				break;
			default:
				break;
		}
	}
	
	if (i == Pf_Invalid) return(D3DENUMRET_OK);

	destNamedFormat = dxTexFormats + i;
	if (enumPixFmt.BitPerPixel != 8)	{
		if ( (destNamedFormat->constPalettes = _GetMem(6 * 256 * sizeof(unsigned int))) != NULL)	{
			for(i = 0; i < 6; i++)
				MMCConvARGBPaletteToPixFmt(((unsigned int *)constPalettes) + i*256,
											destNamedFormat->constPalettes + i*256,
											0, 256, &enumPixFmt);
		}
	}
	
	CopyMemory(&destNamedFormat->ddsdPixelFormat, lpDDpf, sizeof(DDPIXELFORMAT));
	CopyMemory(&destNamedFormat->pixelFormat, &enumPixFmt, sizeof(_pixfmt));
	destNamedFormat->isValid = 1;
	
	return(D3DENUMRET_OK);

	DEBUG_END("TexEnumFormatsCallback", DebugRenderThreadId, 147);
}


/* Ez a rutin lekérdezi a hardver által támogatott textúraformátumokat, létrehozza */
/* a konstans palettákat. */
int TexQuerySupportedFormats()	{
unsigned int constARGBPalettes[6*256];
unsigned int i, val;
	
	DEBUG_BEGIN("TexQuerySupportedFormats", DebugRenderThreadId, 148);

	ZeroMemory (&dxTexFormats, NumberOfDXTexFormats * sizeof(NamedFormat));
		
	dxTexFormats[Pf16_ARGB4444].missing = _16_ARGB4444;
	dxTexFormats[Pf16_ARGB1555].missing = _16_ARGB1555;
	dxTexFormats[Pf16_RGB555].missing = _16_RGB555;
	dxTexFormats[Pf16_RGB565].missing = _16_RGB565;
	dxTexFormats[Pf32_ARGB8888].missing = _32_ARGB8888;
	dxTexFormats[Pf32_RGB888].missing = _32_RGB888;
	dxTexFormats[Pf8_P8].missing = _8_P8;

	/* Generating constant palettes */

	/* RGB332, paletta max alphával */
	for(i = 0; i < 256; i++)
		constARGBPalettes[i] = 0xFF000000 | ((i & 0xE0) << 16) | ((i & 0xE0) << 13) | ((i & 0xC0) << 10) |
								((i & 0x1C) << 11) | ((i & 0x1C) << 8) | ((i & 0x18) << 5) |
								((i & 3) << 6) | ((i & 3) << 4) | ((i & 3) << 2) | (i & 3);
	
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

	lpD3Ddevice->lpVtbl->EnumTextureFormats(lpD3Ddevice, &TexEnumFormatsCallback, constARGBPalettes);

	DEBUGCHECK (0, !dxTexFormats[Pf8_P8].isValid, "\n   Warning: TexQuerySupportedFormats: Paletted textures are not supported! \
												   \n                                      Is it an nVidia card? If yes, why does not it support them??? Argh...");
	DEBUGCHECK (1, !dxTexFormats[Pf8_P8].isValid, "\n   Figyelmeztetés: TexQuerySupportedFormats: A palettás textúrákat a hardver nem támogatja! \
												   \n                                             Ez egy nVidia kártya? Ha igen, miért nem támogatja õket??? Ahh...");

	return(1);

	DEBUG_END("TexQuerySupportedFormats", DebugRenderThreadId, 148);
}


int TexInitAtGlideInit()	{

	DEBUG_BEGIN("TexInitAtGlideInit",DebugRenderThreadId, 149);
	
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
	return(1);
	
	DEBUG_END("TexInitAtGlideInit",DebugRenderThreadId, 149);
}


void TexCleanUpAtShutDown()	{

	DEBUG_BEGIN("TexCleanUpAtShutDown", DebugRenderThreadId, 150);

	if ( (config.Flags & CFG_PERFECTTEXMEM) && (textureMem) )	{
		_FreeMem(textureMem);
		textureMem = NULL;
	}
	
	DEBUG_END("TexCleanUpAtShutDown", DebugRenderThreadId, 150);
}


/* A grWinSStOpen-bôl init közben meghívódó init */	
int TexInit()	{
unsigned int		i;
unsigned char		textureData[8*8];
D3DDEVICEDESC7		devdesc;
GrTexInfo info =	{GR_LOD_8, GR_LOD_8, GR_ASPECT_1x1, GR_TEXFMT_8BIT, textureData};
	
	DEBUG_BEGIN("TexInit", DebugRenderThreadId, 151);

	texindex = NULL;
	mipMapTempArea = NULL;
	for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, astate.lpDDTex[i] = NULL);

	if (!TexQuerySupportedFormats()) return(0);
	
	/* textúraindextáblázatnak hely foglalása */	
	if ( (texindex = (TexCacheEntry **) _GetMem(MAX_TEXNUM * sizeof(TexCacheEntry *))) == NULL) return(0);
	
	/* */
	perfectEmulating = (config.Flags & CFG_PERFECTTEXMEM) ? 1 : 0;
	
	if (config.Flags2 & CFG_AUTOGENERATEMIPMAPS)	{
		if ( (mipMapTempArea = (unsigned char *) _GetMem(128*128*2+64*64*2)) == NULL ) return(0);
	}
		
	ZeroMemory(texInfo, TEXPOOL_NUM * sizeof(TexCacheEntry *));
	ZeroMemory(texNumInPool, TEXPOOL_NUM * sizeof(unsigned int));

	timin = timax = mmidIndex = 0;
	astate.acttex = ethead = NULL;
	astate.actmmid = GR_NULL_MIPMAP_HANDLE;
	lpD3Ddevice->lpVtbl->GetCaps(lpD3Ddevice, &devdesc);
	apMultiTexturing = (devdesc.wMaxTextureBlendStages >= 2) && (dxTexFormats[Pf8_P8].isValid) ? 1 : 0;

	i = GetCKMethodValue(config.Flags);
	if ( (i == CFG_CKMNATIVE) || (i == CFG_CKMNATIVETNT) ) astate.flags |= STATE_NATIVECKMETHOD;

	/* Nincs aktuális textúra (NULL) */
	texMemFree = config.TexMemSize;
	colorKeyPal = colorKeyNcc0 = colorKeyNcc1 = 0;

	/* A speciális nulltextúra létrehozása és bitmapjének feltöltése */
	grTexDownloadMipMap(0, TEXTURE_C0A0, GR_MIPMAPLEVELMASK_BOTH, &info);
	TexReloadTexture(nullTexture = TexFindTex(TEXTURE_C0A0, NULL), RF_TEXTURE);

	grTexFilterMode(0, GR_TEXTUREFILTER_POINT_SAMPLED, GR_TEXTUREFILTER_POINT_SAMPLED);
	grTexClampMode(0, GR_TEXTURECLAMP_WRAP, GR_TEXTURECLAMP_WRAP);	
	grTexCombine(0,GR_COMBINE_FUNCTION_LOCAL,GR_COMBINE_FACTOR_NONE,
				 GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE);

#ifndef GLIDE3
	mipmapinfos = NULL;
	utilMemFreeBase = 0;
#endif

	return(1);

	DEBUG_END("TexInit", DebugRenderThreadId, 151);
}


/* A grWinSStClose-ból meghívódó cleanup */
void TexCleanUp()	{
unsigned int	i;
	
	DEBUG_BEGIN("TexCleanUp", DebugRenderThreadId, 152);

	/* A textúrák eldobálása */
	for(i = timin; i < timax; i++)	{

#ifdef	WRITETEXTURESTOBMP
		WTEX(texindex[i]);
#endif

		GlideReleaseTexture(texindex[i]);
	}
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
	
	DEBUG_END("TexCleanUp", DebugRenderThreadId, 152);
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
DDSURFACEDESC2			lfb = {sizeof(DDSURFACEDESC)};
int						failed;
LPDIRECTDRAWSURFACE7	texture;
unsigned char			tname[256];
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
	sprintf(tname,"e:\\dgvt\\%0x.bmp",tinfo->startAddress);
	bitmap = _GetMem(sizeof(BITMAPINFO)+1024+tinfo->x*tinfo->y*4);
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
	
	texture = tinfo->texBase[0];
	if (texture)	{
		if ((texPtr = TexGetPtrToTexture (lpDDTexture, &texPitch)) != NULL) {
			MMCConvertAndTransferData(tinfo->texAP88AlphaChannel[0] == NULL ? &tinfo->dxformat->pixelformat : &srcPixFmt,
									  &pixelf,
									  0, 0, 0,
									  texPtr, q,
									  tinfo->x, tinfo->y,
									  texPitch, tinfo->x*4,
									  &tinfo->palettedData->palette, NULL,
									  MMCIsPixfmtTheSame(&tinfo->dxformat->pixelformat, &pixelf) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
									  0);
			TexReleasePtrToTexture (lpDDTexture);
		} else {
			DEBUGLOG (0, "\n   Error: WTEX: Cannot lock and read texture!");
			DEBUGLOG (1, "\n   Error: WTEX: Nem tudom zárolni és olvasni a textúrát!");
		}
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

	if (tinfo->texCkAlphaMask[0])	{
		sprintf(tname,"e:\\dgvt\\%0x_mask.bmp",tinfo->startAddress);
		
		texture=tinfo->texCkAlphaMask[0];
		if (texture)	{
			texture->lpVtbl->Lock(texture,NULL,&ddtextdesc,DDLOCK_WAIT,NULL);
			MMCConvertAndTransferData(&tinfo->alphadxformat->pixelformat, &pixelf,
							  0, 0, 0,
							  ddtextdesc.lpSurface, q,
							  tinfo->x, tinfo->y,
							  -ddtextdesc.lPitch, tinfo->x*4,
							  NULL, NULL,
							  MMCIsPixfmtTheSame(&tinfo->alphadxformat->pixelformat, &pixelf) ? CONVERTTYPE_RAWCOPY : CONVERTTYPE_COPY,
							  0);
			texture->lpVtbl->Unlock(texture,NULL);
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