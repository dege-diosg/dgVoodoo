/*--------------------------------------------------------------------------------- */
/* GlideImp.cpp - Glide main DLL implementation (general stuffs)                    */
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
/* dgVoodoo: Glide.cpp																		*/
/*			 Glide-függvények																*/
/*------------------------------------------------------------------------------------------*/

#include	"dgVoodooBase.h"

#include	<windows.h>
#include	<string.h>
#include	<math.h>


extern "C" {

#include	"Dgw.h"
#include	"Main.h"

#include	"dgVoodooConfig.h"
#include	"Debug.h"
#include	"Log.h"
#include	"Dos.h"
#include	"DosCommon.h"
#include	"VxdComm.h"


#include	"Draw.h"
#include	"LfbDepth.h"
#include	"dgVoodooGlide.h"
#include	"Texture.h"
#include	"Grabber.h"
#include	"Resource.h"

}

#include	<glide.h>

#include	"Renderer.hpp"
#include	"RendererGeneral.hpp"
#include	"RendererLfbDepth.hpp"
#include	"RendererTexturing.hpp"

/*------------------------------------------------------------------------------------------*/
/*....................................... Definíciók .......................................*/

#ifndef GLIDE1

#define	GLIDEVERSIONSTRING			"Glide 2.43"

#else

#define	GLIDEVERSIONSTRING			"Glide 2.11"

#endif

#define MAX_RES_CONSTANT			GR_RESOLUTION_400x300
#define MAX_REFRATE_CONSTANT		GR_REFRESH_120Hz

#define CheckAndReturn				if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;


/*------------------------------------------------------------------------------------------*/
/*....................................... Struktúrák .......................................*/

#define DGCOMMAND_GRSSTWINOPEN		1
#define DGCOMMAND_GRSSTWINCLOSE		2
#define DGCOMMAND_GETDEVICESTATUS	3
#define DGCOMMAND_RESETDEVICE		4


int		DoThreadCall(/*HWND hwnd,*/ int command);

/* A grSstWinOpen paramétereit és visszatérési értékét tartalmazó struktúra. */
/* Erre akkor van szükség, amikor a grSstWinOpen-t és a grSstWinClose-t hook-ból hívjuk. */
typedef struct {

	FxU32					hwnd;
	GrScreenResolution_t	res;
	GrScreenRefresh_t		refresh;
	GrColorFormat_t			l_cFormat;
	GrOriginLocation_t		locateOrigin;
	int						numBuffers;
	int						numAuxBuffers;
	FxBool					ret;

} _winopenstruc;


/*-----------------------------------------------------------------------------------------	*/
/*................................... Globális változók.....................................*/

GrColorFormat_t				cFormat;						/* Glide színformátum */
unsigned int				buffswapnum;					/* összesen hányszor váltottak puffert */
float						IndToW[GR_FOG_TABLE_SIZE+1];	/* W meghatározása egy indexbõl (ködtáblázathoz) */
GrFog_t						fogtable[GR_FOG_TABLE_SIZE+1];	/* Ködtábla */
unsigned char				*WToInd;						/* index meghatározása W-bõl (ködtáblához) */

RendererGeneral::GammaRamp	origGammaRamp;					/* eredeti gammaértékek */
unsigned int				fullScreenGammaSupported;

unsigned int				updateFlags;
unsigned int				needRecomposePixelPipeLine;
unsigned int				runflags = 0;
int							renderbuffer;					/* melyik pufferbe megy a rajzolás */
int							rendererGenCaps;

_GlideActState				astate;
_stat						stats;
_winopenstruc				winopenstruc;
HWND						threadCommandWindow = NULL;
HWND						glideHwnd;
LRESULT						(CALLBACK *oldwndproc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int							isWindowMaximized;
int							isWindowMinimized;


unsigned int				appXRes, appYRes;				/* Alk. által megadott felbontás */
int							drawresample;					/* A megadott felbontás eltér az alkalmazásétól */

unsigned short				_xresolution[] = {320, 320, 400, 512, 640, 640, 640, 640, 800, 960, 856, 512, 1024, 1280, 1600, 400};
unsigned short				_yresolution[] = {200, 240, 256, 384, 200, 350, 400, 480, 600, 720, 480, 256, 768, 1024, 1200, 300};
unsigned char				_refreshrate[] = {60, 70, 72, 75, 80, 90, 100, 85, 120};
int							_3dnum, _3dtype;

float						xScale, yScale;
float						xConst, yConst;

/*-----------------------------------------------------------------------------------------	*/
/*....................................... Táblák ...........................................*/

/* lookup táblázat ahhoz, hogy egy combine function használja-e az other paramétert */
char combinefncotherusedlookup[] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter az AOther-t jelöli-e */
char combineFactorOtherAlphaUsedLookUp[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter a textúra RGB komponensét jelöli-e */
char combineFactorTexRgbUsedLookUp[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter a textúra Alpha komponensét jelöli-e */
char combineFactorTexAlphaUsedLookUp[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter a local, other vagy alocal paramétert jelöli-e  */
/* 0 - egyik sem, 1 - local, 2 - aother, 4 - alocal */
char combinefactorwhichparam[] = {0, 1, 2, 4, 0, 0, 0, 0, 0, 1, 2, 4, 0, 0};

/* lookup táblázat ahhoz, hogy az adott combine függvény mely paramétereket használja */
/* 001b = local, 010b = other, 100b = alocal, 1000b = factor */
char combinefncwhichparams[] = {0x0, 0x1, 0x4, 0xA, 0xB, 0x0E, 0xB, 0xB, 0x0F, 0x9, 0, 0, 0, 0, 0, 0, 0x0D };


/*-------------------------------------------------------------------------------------------*/
/*.............................. Általános segédfüggvények ..................................*/

void intToStr(char *dst, int i)	{
	_asm	{
		push ebx
		xor	ecx,ecx
		mov	eax,i
		mov ebx,10
	inttostrnextd_:
		xor edx,edx
		div ebx
		add dl,'0'
		push edx
		inc ecx
		or eax,eax
		jne inttostrnextd_
		mov edx,dst
	inttostrnextd2_:
		pop eax
		mov [edx],al
		inc	edx
		loop inttostrnextd2_
		mov byte ptr [edx],0h
		pop ebx
	}
}


int		GetCurrentDisplayModeBitDepth (unsigned short adapter, unsigned short device)
{
RendererGeneral::DisplayMode	displayMode;

	rendererGeneral->GetCurrentDisplayMode (adapter, device, &displayMode);
	return displayMode.pixFmt.BitPerPixel;
}


int		IsRendererApiAvailable ()
{
	if (rendererGeneral->InitRendererApi ()) {
		rendererGeneral->UninitRendererApi ();
		return (1);
	}
	return (0);
}


int		RefreshDisplayByFrontBuffer ()
{
	return rendererGeneral->RefreshDisplayByBuffer (0);
}


void	GlideResetActState ()
{
	FillMemory(&astate, sizeof(_GlideActState), 0xEE);
	astate.acttex = NULL;
	astate.actmmid = GR_NULL_MIPMAP_HANDLE;
	astate.flags = 0;
	astate.vertexmode = VMODE_ZBUFF;
	for (int i = 0; i < 4/*MAX_STAGETEXTURES*/; i++)
		astate.lpDDTex[i] = NULL;
}


/* A megadott színformátum alapján egy adott színt (color) ARGB-ben ad vissza. */
unsigned long GlideGetColor(GrColor_t color)	{

	switch(cFormat)	{
		case GR_COLORFORMAT_RGBA:
			_asm	{
				mov eax,color
				ror eax,8h
				mov color,eax
			}
			break;		
		case GR_COLORFORMAT_BGRA:
			_asm	{
				mov eax,color
				bswap eax
				mov color,eax
			}
			break;
		case GR_COLORFORMAT_ABGR:
			_asm	{
				mov	eax,color
				bswap eax
				ror eax,8h
				mov color,eax
			}
			break;
		case GR_COLORFORMAT_ARGB:;
			break;
	}
	return(color);
}


/* Elkészíti a segédtáblákat a ködhöz: az indexbõl W-t, és W-bõl indexet megadó lookup táblázatokat */
int GlideCreateFogTables()	{
unsigned int	i,j;

	DEBUG_BEGINSCOPE("GlideCreateFogTables", DebugRenderThreadId);

	if ( (WToInd = (unsigned char *) _GetMem(65536+16)) == NULL)
		return(0);
	for(i = 0; i <= GR_FOG_TABLE_SIZE; i++)
		IndToW[i] = guFogTableIndexToW(i);
	IndToW[0] = 0.0f;
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		for(j = (unsigned int) IndToW[i]; j <= (unsigned int) IndToW[i+1]; j++)
			WToInd[j] = (unsigned char) i;
	}
	fogtable[GR_FOG_TABLE_SIZE] = 0;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


/* Eldobja a köd segédtábláit */
void GlideDestroyFogTables()	{

	DEBUG_BEGINSCOPE("GlideDestroyFogTables", DebugRenderThreadId);
	
	if (WToInd)
		_FreeMem(WToInd);
	
	WToInd = NULL;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Az ItoW táblát egy adott helyre másolja (ezt a függvényt csak DOS-ból hívjuk meg, mert ott */
/* nem használunk semmilyen lebegõpontos aritmetikát, így ezt a táblát sem tudjuk legenerálni). */
void GlideGetIndToWTable(float *table)	{
	
	DEBUG_BEGINSCOPE("GlideGetIndToWTable", DebugRenderThreadId);
	
	CopyMemory(table, IndToW, GR_FOG_TABLE_SIZE*sizeof(float));
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void	GlideApplyGammaCorrection()	{
unsigned int	percent,color;
	
	/* Ha az eszköz támogatja a gamma rampot és az be is van kapcsolva, akkor megoldva */
	if ( (fullScreenGammaSupported) || (config.Gamma == 100) ) return;

	rendererGeneral->SaveRenderState (RendererGeneral::GammaCorrection);
	rendererGeneral->SetRenderState (RendererGeneral::GammaCorrection, 0, 0, 0);

	percent = 100;
	
	while(percent != config.Gamma)	{
		color = (config.Gamma*100)/percent;		
		if (color < 100)	{
			rendererGeneral->SetDestAlphaBlendFactorDirectly (RendererGeneral::DestAlphaFactorZero);
			percent = config.Gamma;
		} else {
			rendererGeneral->SetDestAlphaBlendFactorDirectly (RendererGeneral::DestAlphaFactorOne);
			if (config.Gamma > 2*percent) percent += 100;
				else percent = config.Gamma;
			if (color > 200) color = 200;
			color -= 100;
		}
		color = (color * 255) / 100;
		_asm	{
			mov		eax,color			
			mov		ah,al
			mov		edx,eax
			shl		eax,10h
			or		eax,edx
			mov		color,eax
		}

		DrawTile (0.0f, 0.0f, (float) movie.cmiXres, (float) movie.cmiYres, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, color);
	}

	rendererGeneral->RestoreRenderState (RendererGeneral::GammaCorrection);
}


/*-------------------------------------------------------------------------------------------*/

/* A VMODE_WUNDEFINED vertex flag-et update-eli. */
void GlideSetVertexWUndefined()	{

	if ( (!(astate.vertexmode & VMODE_TEXTURE)) && (astate.depthFunction == GR_CMP_ALWAYS) ) astate.vertexmode |= VMODE_WUNDEFINED;
		else astate.vertexmode &= ~VMODE_WUNDEFINED;
}


/* Ez a függvény eldönti, hogy a vertexek információi közül szükség van-e a textúrakoordinátákra */
/* és az interpolált színre (diffuse). Ennek megfelelõen állítja be a VMODE_TEXTURE és VMODE_DIFFUSE */
/* biteket a vertex átalakítását megadó állapotinfóban. */
/* Ezt két infóból dönti el: az alfa és color combine használja-e az adott komponenst, és hogy */
/* az alpha blending felhasználja-e a forrásszínt (és alfát). */
void _inline GlideUpdateVertexTexDiffuseMode()	{
int		col,alpha;

	astate.vertexmode &= ~(VMODE_DIFFUSE | VMODE_TEXTURE);
	if (astate.colorMask)	{
		col = alpha = 0;
		if (astate.alphaBlendEnable)	{
			if ( (astate.rgbDstBlend == (unsigned char) GR_BLEND_SRC_COLOR) || (astate.rgbDstBlend == (unsigned char) GR_BLEND_ONE_MINUS_SRC_COLOR) ||
				 (astate.rgbDstBlend == (unsigned char) GR_BLEND_ONE) || (astate.rgbSrcBlend != (unsigned char) GR_BLEND_ZERO) ||
				 (astate.rgbDstBlend == (unsigned char) GR_BLEND_PREFOG_COLOR))
				  col = 1;
			if ( (astate.rgbSrcBlend == (unsigned char) GR_BLEND_SRC_ALPHA) || (astate.rgbSrcBlend == (unsigned char) GR_BLEND_ONE_MINUS_SRC_ALPHA) ||
				(astate.rgbDstBlend == (unsigned char) GR_BLEND_SRC_ALPHA) || (astate.rgbDstBlend == (unsigned char) GR_BLEND_ONE_MINUS_SRC_ALPHA) ||
				(astate.rgbSrcBlend == (unsigned char) GR_BLEND_ALPHA_SATURATE) )
				alpha = 1;
		} else col = 1;
		
		if ( ( (astate.flags & STATE_COLORCOMBINEUSETEXTURE) && col) ||
			( (astate.flags & STATE_ALPHACOMBINEUSETEXTURE) && alpha ) ) astate.vertexmode |= VMODE_TEXTURE;
		if ( ( (astate.flags & STATE_COLORDIFFUSEUSED) && col) ||
			( (astate.flags & STATE_ALPHADIFFUSEUSED) && alpha ) ) astate.vertexmode |= VMODE_DIFFUSE;
	}
	GlideSetVertexWUndefined();
	LOG(1,"\n   col = %d, astate.vertexmode & VMODE_TEXTURE = %d", col, astate.vertexmode & VMODE_TEXTURE);
	LOG(1,"\n   alpha = %d, astate.vertexmode & VMODE_TEXTURE = %d", alpha, astate.vertexmode & VMODE_TEXTURE);

	/*LOG(1,"\GlideUpdateVertexTexDiffuseMode: astate.flags & STATE_COLORDIFFUSEUSED: %d, col: %d, astate.flags & STATE_ALPHADIFFUSEUSED: %d, alpha: %d",
			(astate.flags & STATE_COLORDIFFUSEUSED) ? 1 : 0, col, (astate.flags & STATE_ALPHADIFFUSEUSED) ? 1 : 0, alpha);
	LOG(1,"\n astate.vertexmode=%0x",astate.vertexmode);*/
}


/* A VMODE_NODRAW vertex-flaget frissíti: ha a konstans szín a colorkey, és a colorkeying engedélyezve van, akkor  */
/* nem rajzol */
void GlideUpdateConstantColorKeyingState()	{

	if (astate.flags & STATE_NORENDERTARGET) {
	
		astate.vertexmode |= VMODE_NODRAW;
	
	} else {
	
		astate.vertexmode &= ~VMODE_NODRAW;
		if ( (astate.colorKeyEnable) && (astate.cotherp == GR_COMBINE_OTHER_CONSTANT) && 
			((GlideGetColor(astate.constColorValue) & 0x00FFFFFF) == (GlideGetColor(astate.colorkey) & 0x00FFFFFF) ) )
			astate.vertexmode |= VMODE_NODRAW;
	}
}


/* Megállapítja, hogy a color combine használja-e a textúrát és a diffuse színt. */
void GlideColorCombineUpdateVertexMode()	{

	DEBUG_BEGINSCOPE("GlideColorCombineUpdateVertexMode", DebugRenderThreadId);
	
	astate.flags &= ~(STATE_COLORCOMBINEUSETEXTURE | STATE_TEXTURERGBUSED | STATE_COLORDIFFUSEUSED);
	
	unsigned int flags = 0;
	if ( (astate.cotherp == GR_COMBINE_OTHER_TEXTURE) && (astate.cfactorp != GR_COMBINE_FACTOR_ZERO) &&
		 (combinefncotherusedlookup[astate.cfuncp]) )
		flags = STATE_TEXTURERGBUSED;

	/* ha az alpha ItRGB engedélyezve, és a color combine függvény használja a clocal-t */
	int alphaItRGB = (astate.aItRGBLightingMode && (combinefncwhichparams[astate.cfuncp] & 0x001));
	if (alphaItRGB)
		flags |= STATE_TEXTUREALPHAUSED;

	if (astate.cfuncp >= GR_COMBINE_FUNCTION_SCALE_OTHER) {
		if (combineFactorTexRgbUsedLookUp[astate.cfactorp])
			flags |= STATE_TEXTURERGBUSED;

		if (combineFactorTexAlphaUsedLookUp[astate.cfactorp])
			flags |= STATE_TEXTUREALPHAUSED;

		if (combineFactorOtherAlphaUsedLookUp[astate.cfactorp] && (astate.aotherp == GR_COMBINE_OTHER_TEXTURE))
			flags |= STATE_TEXTUREALPHAUSED;
	}

	if (flags & (STATE_TEXTURERGBUSED | STATE_TEXTUREALPHAUSED))
		flags |= STATE_COLORCOMBINEUSETEXTURE;

	if (texCombineFuncImp[astate.rgb_function] == TCF_ZERO)
		flags &= ~STATE_COLORCOMBINEUSETEXTURE;

	astate.flags |= flags;
	
	if (!alphaItRGB) {
		char usage = 0;
		if (astate.clocalp == GR_COMBINE_LOCAL_ITERATED) usage = 1;
		if (astate.cotherp == GR_COMBINE_OTHER_ITERATED) usage |= 2;
		if (astate.alocalp == GR_COMBINE_LOCAL_ITERATED) usage |= 4;
		if (astate.aotherp == GR_COMBINE_OTHER_ITERATED) usage |= 16;
		if (combinefactorwhichparam[astate.cfactorp] == 1) usage |= (usage & 1) << 3;
		if (combinefactorwhichparam[astate.cfactorp] == 2) usage |= (usage & 16) >> 1;
		if (combinefactorwhichparam[astate.cfactorp] == 4) usage |= (usage & 4) << 1;
		if (combinefncwhichparams[astate.cfuncp] & usage) astate.flags |= STATE_COLORDIFFUSEUSED;

		LOG(1,"\n\tcolor, usage=%d, combinefncwhichparams[astate.cfuncp]=%d", usage,combinefncwhichparams[astate.cfuncp]);

	} else
		astate.flags |= STATE_COLORDIFFUSEUSED;
	
	GlideUpdateVertexTexDiffuseMode ();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Megállapítja, hogy az alpha combine használja-e a textúrát és a diffuse színt. */
void GlideAlphaCombineUpdateVertexMode()	{
char	usage;

	astate.flags &= ~(STATE_ALPHACOMBINEUSETEXTURE | STATE_TEXTUREALPHAUSED | STATE_ALPHADIFFUSEUSED);

	if ((astate.aotherp == GR_COMBINE_OTHER_TEXTURE) && (astate.afactorp != GR_COMBINE_FACTOR_ZERO) &&
		(combinefncotherusedlookup[astate.afuncp]))
		astate.flags |= STATE_TEXTUREALPHAUSED | STATE_ALPHACOMBINEUSETEXTURE;
	
	if (astate.afuncp >= GR_COMBINE_FUNCTION_SCALE_OTHER) {
		if (combineFactorTexRgbUsedLookUp[astate.afactorp] || combineFactorTexAlphaUsedLookUp[astate.cfactorp])
			astate.flags |= STATE_TEXTUREALPHAUSED | STATE_ALPHACOMBINEUSETEXTURE;

		if (combineFactorOtherAlphaUsedLookUp[astate.afactorp] && (astate.aotherp == GR_COMBINE_OTHER_TEXTURE))
			astate.flags |= STATE_TEXTUREALPHAUSED | STATE_ALPHACOMBINEUSETEXTURE;
	}

	if (texCombineFuncImp[astate.alpha_function] == TCF_ZERO)
		astate.flags &= ~STATE_ALPHACOMBINEUSETEXTURE;

	usage = 0;
	if (astate.alocalp == GR_COMBINE_LOCAL_ITERATED) usage = 1 | 4;
	if (astate.aotherp == GR_COMBINE_OTHER_ITERATED) usage |= 2;
	if ( (combinefactorwhichparam[astate.afactorp] == 1) || (combinefactorwhichparam[astate.afactorp] == 4) )
		usage |= (usage & 1) << 3;
	if (combinefactorwhichparam[astate.afactorp] == 2)
		usage |= (usage & 2) << 2;
	if (combinefncwhichparams[astate.afuncp] & usage)
		astate.flags |= STATE_ALPHADIFFUSEUSED;
	
	LOG (1, "\n  astate.flags & STATE_TEXTUREALPHAUSED == %d", astate.flags & STATE_TEXTUREALPHAUSED);
	//LOG(1,"\n\talpha, usage=%d, combinefncwhichparams[astate.afuncp]=%d",usage,combinefncwhichparams[astate.afuncp]);
}


void GlideUpdateUsingTmuWMode ()	{
	
	if ( (astate.hints & GR_STWHINT_W_DIFF_TMU0) &&
		 ( ((astate.depthBuffMode != GR_DEPTHBUFFER_WBUFFER) && (astate.depthBuffMode != GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS)) ||
		 (config.Flags2 & CFG_PREFERTMUWIFPOSSIBLE) ))
	  astate.flags |= STATE_USETMUW;
	else astate.flags &= ~STATE_USETMUW;

}


void GlideUpdateTextureUsageFlags()	{
	
	unsigned int flags = rendererGeneral->GetTextureUsageFlags ();

	if (flags != (astate.flags & STATE_TEXTUREUSAGEMASK))
		updateFlags |= UPDATEFLAG_RESETTEXTURE;

	astate.flags = (astate.flags & ~STATE_TEXTUREUSAGEMASK) | flags;
}


void _fastcall GetScaledClipWindow (RECT* rect)	{

	GrOriginLocation_t origin = (config.Flags2 & CFG_YMIRROR) ? ((astate.locOrig == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT) : astate.locOrig;
	
	rect->left = (LONG) (astate.minx * xScale);
	rect->right = (LONG) (astate.maxx * xScale);
	if (origin == GR_ORIGIN_LOWER_LEFT)	{
		rect->top = (LONG) (movie.cmiYres + (((float) astate.maxy) * yScale));
		rect->bottom = (LONG) (movie.cmiYres + (((float) astate.miny) * yScale));
	} else {
		rect->top = (LONG) (((float) astate.miny) * yScale);
		rect->bottom = (LONG) (((float) astate.maxy) * yScale);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*.................................... Glide-függvények .....................................*/


void EXPORT grGlideGetVersion(char version[80])	{
int	i = 0;

	DEBUG_BEGINSCOPE("grGlideGetVersion", DebugRenderThreadId);
	
	LOG(0,"\ngrGlideGetVersion(versionbuff=%p)", version);	
	while(version[i] = GLIDEVERSIONSTRING[i]) i++;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


FxBool EXPORT grSstQueryBoards(GrHwConfiguration *hwConfig)	{

	DEBUG_BEGINSCOPE("grSstQueryBoards", DebugRenderThreadId);

	LOG(0,"\ngrSstQueryBoards(hwConfig=%p)",hwConfig);
	ZeroMemory(hwConfig, sizeof(GrHwConfiguration));
	hwConfig->num_sst = 1;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}


FxBool EXPORT grSstQueryHardware(GrHwConfiguration *hwConfig)	{
	
	DEBUG_BEGINSCOPE("grSstQueryHardware", DebugRenderThreadId);
	
	LOG(0,"\ngrSstQueryHardware(hwConfig=%p)",hwConfig);
	ZeroMemory(hwConfig, sizeof(GrHwConfiguration));
	hwConfig->num_sst = 1;
	hwConfig->SSTs[0].type = GR_SSTTYPE_VOODOO;
	hwConfig->SSTs[0].sstBoard.VoodooConfig.fbRam = config.TexMemSize/(1024*1024);
	hwConfig->SSTs[0].sstBoard.VoodooConfig.fbiRev = 0x1000;
	hwConfig->SSTs[0].sstBoard.VoodooConfig.nTexelfx = 1;
	hwConfig->SSTs[0].sstBoard.VoodooConfig.sliDetect = FXFALSE;
	hwConfig->SSTs[0].sstBoard.VoodooConfig.tmuConfig[0].tmuRev = 1;
	hwConfig->SSTs[0].sstBoard.VoodooConfig.tmuConfig[0].tmuRam = config.TexMemSize/(1024*1024);

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}


void EXPORT grGlideInit()	{

	DEBUG_GLOBALTIMEBEGIN;
	
	DEBUG_BEGINSCOPE("grGlideInit", DebugRenderThreadId);

	LOG(0,"\ngrGlideInit()");
	TexInitAtGlideInit();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grSstSelect(int whichSST)	{
	
	LOG(0,"\ngrSstSelect(whichSST=%d)", whichSST);
}


void EXPORT grBufferClear(GrColor_t color, GrAlpha_t alpha, FxU16 depth)	{
float	z = 0.0f;

	DEBUG_BEGINSCOPE("grBufferClear", DebugRenderThreadId);

	LOG(0,"\ngrBufferClear(color=%0x, alpha=%0x, depth=%0x)",color,alpha,depth);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	LfbCheckLock(renderbuffer);
	DrawFlushPrimitives ();
	color = (GlideGetColor(color) & 0x00FFFFFF) | (((unsigned int) alpha) << 24);
	
	unsigned int flags = (astate.colorMask == FXTRUE) ? CLEARFLAG_COLORBUFFER : 0;
	if (astate.depthMaskEnable == FXTRUE)	{
		flags |= CLEARFLAG_DEPTHBUFFER;
		switch(depthbuffering)	{
			case ZBuffering:	
				z = DrawGetFloatFromDepthValue (depth, DVLinearZ);
				break;
			case WBufferingEmu:
				z = DrawGetFloatFromDepthValue (depth, DVNonlinearZ);
				break;
			case WBuffering:
				z = DrawGetFloatFromDepthValue (depth, DVTrueW) / 65528.0f;
				break;
			default:;
		}
	}

	RECT clipRect;
	GetScaledClipWindow (&clipRect);
	
	rendererGeneral->ClearBuffer (flags, &clipRect, color, z);
	
	if (renderbuffer == GR_BUFFER_FRONTBUFFER)
		rendererGeneral->ShowContentChangesOnFrontBuffer ();

	LfbSetBuffDirty(renderbuffer);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grBufferSwap(int swapinterval)	{
	
	DEBUG_BEGINSCOPE("grBufferSwap", DebugRenderThreadId);
	
	LOG(1,"\n---- grBufferSwap(swapinterval=%d) ----", swapinterval);

	if (!(runflags & RF_INITOK)) return;
	if (runflags & RF_SCENECANBEDRAWN)	{
		DrawBeforeSwap ();
		LfbBeforeSwap ();		
		DrawFlushPrimitives ();
		GlideApplyGammaCorrection();
		GrabberDrawLabel(1);
		rendererGeneral->EndScene ();
	}

	RendererGeneral::DeviceStatus deviceStatus = (RendererGeneral::DeviceStatus) DoThreadCall (DGCOMMAND_GETDEVICESTATUS);
		//rendererGeneral->GetDeviceStatus ();

	if (deviceStatus == RendererGeneral::StatusLost) {
		
		runflags &= ~RF_SCENECANBEDRAWN;
		return;
	}

	if (deviceStatus == RendererGeneral::StatusLostCanBeReset || (!(runflags & RF_SCENECANBEDRAWN))) {
		runflags &= ~RF_SCENECANBEDRAWN;
	
		if (!rendererTexturing->CanRestoreTextures ()) {
			DEBUGCHECK (0, !(config.Flags & CFG_PERFECTTEXMEM), \
				"\n   Warning: grBufferSwap: Resetting device without perfect texmem emu, \
				 \n                          renderer API doesn't support restoring texture restoring. All cached textures will be lost!");
			DEBUGCHECK (1, !(config.Flags & CFG_PERFECTTEXMEM), \
				"\n   Warning: grBufferSwap: Eszköz helyreállítása teljes texmem emu nélkül, \
				 \n                          a megjelenítõ API nem támogatja a textúrák helyreállítását. Az összes tárolt textúra el fog veszni!");
			TexReleaseAllCachedTextures ();
			LfbDestroyTileTextures ();
		}

		if (!rendererTexturing->CanPreserveTextures ()) {
			GlideTexSetTexturesAsLost();
		}

		LfbInvalidateCachedContent ();

		rendererGeneral->PrepareForDeviceReset ();
		//if (!rendererGeneral->DeviceReset ())
		//	return;
		if (!DoThreadCall (DGCOMMAND_RESETDEVICE))
			return;
		if (!rendererGeneral->ReallocResources ())
			return;

		runflags |= RF_SCENECANBEDRAWN;

		if (!rendererTexturing->CanRestoreTextures ()) {
			TexCreateSpecialTextures ();
			if (!LfbCreateTileTextures ()) {
				runflags &= ~RF_SCENECANBEDRAWN;
				return;
			}
		}

		rendererLfbDepth->ClearBuffers ();

		_GlideActState curState;
		grGlideGetState (&curState);
		GlideResetActState ();
		grGlideSetState (&curState);
		//grRenderBuffer (renderbuffer++);
		grGlideGetState (&curState);
	}

	if ((config.Flags & CFG_VSYNC) && (swapinterval == 0)) swapinterval = 1;
	rendererGeneral->SwitchToNextBuffer (swapinterval, renderbuffer);

	buffswapnum++;

	if (!rendererGeneral->BeginScene()) runflags &= ~RF_SCENECANBEDRAWN;
		else runflags |= RF_SCENECANBEDRAWN;
	LfbAfterSwap ();
	GrabberCleanUnvisibleBuff ();
	DrawAfterSwap ();

	//grRenderBuffer (1); //////////
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


int EXPORT grBufferNumPending( void )	{

	return rendererGeneral->GetNumberOfPendingBufferFlippings ();
}


void EXPORT grConstantColorValue( GrColor_t color )	{

	DEBUG_BEGINSCOPE("grConstantColorValue", DebugRenderThreadId);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	LOG(0,"\ngrConstantColorValue(color=%0x), in ARGB: %0x", color, GlideGetColor(color));
	if (color != astate.constColorValue) {
		DrawFlushPrimitives ();
		astate.constColorValue = color;
		rendererGeneral->SetConstantColor (GlideGetColor(color));
		GlideUpdateConstantColorKeyingState();
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grConstantColorValue4(float a, float r, float g, float b)	{
	
//	return;
	DEBUG_BEGINSCOPE("grConstantColorValue4",DebugRenderThreadId);

	LOG(0,"\ngrConstantColorValue4(a=%f, r=%f, g=%f, b=%f)",a,r,g,b);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	astate.delta0Rgb =	(((unsigned long) ((unsigned char) (a*(255.0f/256.0f)))) <<24) +
						(((unsigned long) ((unsigned char) (r*(255.0f/256.0f)))) <<16) +
						(((unsigned long) ((unsigned char) (g*(255.0f/256.0f)))) <<8) +
						((unsigned long) ((unsigned char) (b*(255.0f/256.0f))));
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grColorMask( FxBool rgb, FxBool alpha )	{

//	return;
	DEBUG_BEGINSCOPE("grColorMask", DebugRenderThreadId);
	
	LOG(1,"\ngrColorMask(rgb=%s,alpha=%s)",fxbool_t[rgb],fxbool_t[alpha]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;	
	
	if ((astate.colorMask != (DWORD) rgb) || (astate.alphaWriteMask != (DWORD) alpha)) {
	
		DrawFlushPrimitives ();
		astate.colorMask = rgb;
		astate.alphaWriteMask = alpha;
		rendererGeneral->SetColorAndAlphaWriteMask (rgb, alpha);

		GlideUpdateVertexTexDiffuseMode ();

	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy ) {

//	return;
	DEBUG_BEGINSCOPE("grClipWindow",DebugRenderThreadId);

	LOG(0,"\ngrClipWindow(minx=%d, miny=%d, maxx=%d, maxy=%d)", minx, miny, maxx, maxy);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;

	if ((minx != astate.minx) || (miny != astate.miny) || (maxx != astate.maxx) || (maxy != astate.maxy)) {
		DrawFlushPrimitives();
		astate.minx = minx;
		astate.miny = miny;
		astate.maxx = maxx;
		astate.maxy = maxy;

		RECT clipRect;
		GetScaledClipWindow (&clipRect);
		if (clipRect.left < 0)
			clipRect.left = 0;
		if (clipRect.top < 0)
			clipRect.top = 0;
		if (clipRect.right > (long) movie.cmiXres)
			clipRect.right = movie.cmiXres;
		if (clipRect.bottom > (long) movie.cmiYres)
			clipRect.bottom = movie.cmiYres;
		rendererGeneral->SetClipWindow (&clipRect);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grGlideGetState( _GrState *state ) {

	//return;
	DEBUG_BEGINSCOPE("grGlideGetState",DebugRenderThreadId);

	LOG(0,"\ngrGlideGetState(*state=%p)",state);
	DrawFlushPrimitives();
	
	/* A megfelelõ állapotok frissítése mentés elõtt */
	if (updateFlags & UPDATEFLAG_SETTEXTURE) GlideTexSource();
	if (updateFlags & UPDATEFLAG_RESETTEXTURE) TexUpdateMultitexturingState((TexCacheEntry *) astate.acttex);
	if (updateFlags & UPDATEFLAG_SETPALETTE) TexUpdateTexturePalette();
	if (updateFlags & UPDATEFLAG_SETNCCTABLE) TexUpdateTextureNccTable();
	//if (updateFlags & UPDATEFLAG_SETCOLORKEYSTATE) TexUpdateNativeColorKeyState();
	
	if (updateFlags & UPDATEFLAG_COLORKEYSTATE) TexUpdateColorKeyState ();
	if (updateFlags & UPDATEFLAG_ALPHACOMBINE) GlideAlphaCombine();
	if (updateFlags & UPDATEFLAG_COLORCOMBINE) GlideColorCombine();
	if (updateFlags & UPDATEFLAG_TEXCOMBINE) GlideTexCombine();
	GlideRecomposePixelPipeLine ();
	if (updateFlags & (UPDATEFLAG_ALPHATESTFUNC | UPDATEFLAG_ALPHATESTREFVAL)) GlideUpdateAlphaTestState ();
	if (updateFlags & UPDATEFLAG_RESETTEXTURE) TexSetCurrentTexture((TexCacheEntry *)astate.acttex);
	
	CopyMemory(state, &astate, sizeof(_GrState));
	return;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grGlideSetState( _GrState *state ) {
GrTexInfo	texInfo;

	//return;
	DEBUG_BEGINSCOPE("grGlideSetState",DebugRenderThreadId);

	LOG(0,"\ngrGlideSetState(*state=%p)",state);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;

	grSstOrigin (state->locOrig);
	grHints (GR_HINT_STWHINT, state->hints);
	grAlphaControlsITRGBLighting (state->aItRGBLightingMode);

	grTexCombine (0, state->rgb_function, state->rgb_factor,
					 state->alpha_function, state->alpha_factor,
					 state->rgb_invert, state->alpha_invert);
	grAlphaCombine (state->afuncp, state->afactorp, state->alocalp, state->aotherp, state->ainvertp);
	grColorCombine (state->cfuncp, state->cfactorp, state->clocalp, state->cotherp, state->cinvertp);
	
	texInfo.aspectRatio = state->aspectRatio;
	texInfo.format = state->format;
	texInfo.smallLod = state->smallLod;
	texInfo.largeLod = state->largeLod;

	grTexMultibase (0, state->multiBaseTexturing);
	grTexMultibaseAddress (0, GR_TEXBASE_256, state->startAddress[0], GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_128, state->startAddress[1], GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_64, state->startAddress[2], GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_32_TO_1, state->startAddress[3], GR_MIPMAPLEVELMASK_BOTH, &texInfo);

	grConstantColorValue (state->constColorValue);

	grTexFilterMode (0, state->minFilter, state->magFilter);
	grTexClampMode (0, state->clampModeU, state->clampModeV);
	grTexLodBiasValue (0, state->lodBias);
	grTexMipMapMode (0, state->mipMapMode, state->lodBlend);

	grDepthBufferMode (state->depthBuffMode);
	grDepthBufferFunction (state->depthFunction);
	grDepthMask (state->depthMaskEnable);
	grDepthBiasLevel( (FxI16) astate.depthbiaslevel);
	grCullMode (state->gcullmode);

	grAlphaBlendFunction (state->rgbSrcBlend, state->rgbDstBlend, state->alphaSrcBlend, state->alphaDstBlend);

	grAlphaTestFunction (state->alphaTestFunc);
	grAlphaTestReferenceValue (state->alpharef);

	grFogMode (state->fogmode | state->fogmodifier);
	grFogColorValue (state->fogcolor);

	grChromakeyMode (state->colorKeyEnable ? GR_CHROMAKEY_ENABLE : GR_CHROMAKEY_DISABLE);
	grChromakeyValue (state->colorkey);

	grClipWindow(state->minx, state->miny, state->maxx, state->maxy);
	grColorMask(state->colorMask, 0);

	grGammaCorrectionValue (state->gammaCorrectionValue);

	astate.delta0Rgb = state->delta0Rgb;
	astate.flags = (astate.flags & ~STATE_DELTA0) | (state->flags & STATE_DELTA0);

	return;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df,
								 GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df )	{


	DEBUG_BEGINSCOPE("grAlphaBlendFunction", DebugRenderThreadId);
	
	LOG(1,"\nAlphaBlendFunction(rgb_sf=%s, rgb_df=%s, alpha_sf=%s, alpha_df=%s", \
		alphabfnc_srct[rgb_sf], alphabfnc_dstt[rgb_df], \
		alphabfnc_srct[alpha_sf], alphabfnc_dstt[alpha_df]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	
	if ((rgb_sf != astate.rgbSrcBlend) || (rgb_df != astate.rgbDstBlend) /*||
		(alpha_sf != astate.alphaSrcBlend) || (alpha_df != astate.alphaDstBlend)*/) {
	
		DrawFlushPrimitives();
		
		char alphaBlendEnabled = ((rgb_sf != GR_BLEND_ONE) || (rgb_df != GR_BLEND_ZERO));/* ||
								  ((alpha_sf != GR_BLEND_ONE) || (alpha_df != GR_BLEND_ZERO));*/
		alphaBlendEnabled &= 1;
	
		if (alphaBlendEnabled != astate.alphaBlendEnable) {
			rendererGeneral->EnableAlphaBlending (alphaBlendEnabled);
			astate.alphaBlendEnable = alphaBlendEnabled;
		}

		if (alphaBlendEnabled && ((rgb_sf != astate.rgbSrcBlend) || (rgb_df != astate.rgbDstBlend))) {
			rendererGeneral->SetRgbAlphaBlendingFunction (rgb_sf, rgb_df);
		}
		astate.rgbSrcBlend = (unsigned char) rgb_sf;
		astate.rgbDstBlend = (unsigned char) rgb_df;

		if (alphaBlendEnabled && ((alpha_sf != astate.alphaSrcBlend) || (alpha_df != astate.alphaDstBlend))) {
			//rendererGeneral->SetAlphaAlphaBlendingFunction (alpha_sf, alpha_df);
		}
		astate.alphaSrcBlend = (unsigned char) alpha_sf;
		astate.alphaDstBlend = (unsigned char) alpha_df;
	}
	GlideUpdateVertexTexDiffuseMode ();
	updateFlags |= UPDATEFLAG_COLORKEYSTATE;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grColorCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert )	{
		
	DEBUG_BEGINSCOPE("grColorCombine", DebugRenderThreadId);

	LOG(1,"\ngrColorCombine(func=%s, factor=%s, local=%s, other=%s, invert=%s", \
		combfunc_t[func], combfact_t[factor], local_t[local], other_t[other], fxbool_t[invert]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( ( (unsigned char) func == astate.cfuncp) && ((unsigned char) factor == astate.cfactorp)
		&& ((unsigned char) local == astate.clocalp) && ((unsigned char) other == astate.cotherp)
		&& ((unsigned char) invert == astate.cinvertp) ) return;
	
	DrawFlushPrimitives();
	LOG(1,"\n\tastate.flags=%0x",astate.flags);
	astate.cfuncp = (unsigned char) func;
	astate.cfactorp = (unsigned char) factor;
	astate.clocalp = (unsigned char) local;
	astate.cotherp = (unsigned char) other;
	astate.cinvertp = (unsigned char) invert;
	GlideColorCombineUpdateVertexMode();
	GlideUpdateConstantColorKeyingState();

	updateFlags |= UPDATEFLAG_COLORCOMBINE | UPDATEFLAG_COLORKEYSTATE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void		GlideColorCombine()	{

	DEBUG_BEGINSCOPE("GlideColorCombine", DebugRenderThreadId);

	needRecomposePixelPipeLine = 1;
	rendererGeneral->ConfigureColorPixelPipeLine (astate.cfuncp, astate.cfactorp, astate.clocalp, astate.cotherp, astate.cinvertp);

	updateFlags &= ~UPDATEFLAG_COLORCOMBINE;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAlphaCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert)	{
		
	DEBUG_BEGINSCOPE("grAlphaCombine", DebugRenderThreadId);
	
	LOG(1,"\ngrAlphaCombine(func=%s, factor=%s, local=%s, other=%s, invert=%s", \
		combfunc_t[func], combfact_t[factor], local_t[local], other_t[other], fxbool_t[invert]);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (func == astate.afuncp) && (factor == astate.afactorp) && (local == astate.alocalp)
		&& (other == astate.aotherp) && (invert == astate.ainvertp) /*&& (astate.flags == astate.flagsold)*/ )
		return;	
	DrawFlushPrimitives();
	LOG(1,"\n\tastate.flags=%0x", astate.flags);
	astate.afuncp = (unsigned char) func;
	astate.afactorp = (unsigned char) factor;
	astate.alocalp = (unsigned char) local;
	astate.aotherp = (unsigned char) other;
	astate.ainvertp = (unsigned char) invert;

	GlideAlphaCombineUpdateVertexMode();
	GlideColorCombineUpdateVertexMode();
	updateFlags |= UPDATEFLAG_ALPHACOMBINE | UPDATEFLAG_COLORKEYSTATE;
	return;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void		GlideAlphaCombine()	{
	
	DEBUG_BEGINSCOPE("GlideAlphaCombine", DebugRenderThreadId);
	
	needRecomposePixelPipeLine = 1;
	rendererGeneral->ConfigureAlphaPixelPipeLine (astate.afuncp, astate.afactorp, astate.alocalp, astate.aotherp, astate.ainvertp);

	updateFlags |= UPDATEFLAG_COLORCOMBINE;
	updateFlags &= ~UPDATEFLAG_ALPHACOMBINE;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void		GlideTexCombine()
{
	needRecomposePixelPipeLine = 1;
	rendererGeneral->ConfigureTexelPixelPipeLine (astate.rgb_function, astate.rgb_factor, astate.alpha_function, astate.alpha_factor,
												  astate.rgb_invert, astate.alpha_invert);
	updateFlags &= ~UPDATEFLAG_TEXCOMBINE;
}


void		GlideRecomposePixelPipeLine ()
{
	if (needRecomposePixelPipeLine) {
		rendererGeneral->ComposeConcretePixelPipeLine ();
		GlideUpdateTextureUsageFlags();
		needRecomposePixelPipeLine = 0;
	}
}


void EXPORT grAlphaControlsITRGBLighting( FxBool enable ) {

	if ((runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	
	if (enable != astate.aItRGBLightingMode) {
		DrawFlushPrimitives();

		astate.aItRGBLightingMode = (unsigned char) enable;
		updateFlags |= UPDATEFLAG_AITRGBLIGHTING;

		GlideColorCombineUpdateVertexMode();
	}
}


void		GlideAlphaItRGBLighting ()
{
	needRecomposePixelPipeLine |= rendererGeneral->EnableAlphaItRGBLighting (astate.aItRGBLightingMode);

	updateFlags &= ~UPDATEFLAG_AITRGBLIGHTING;
}


void EXPORT guColorCombineFunction( GrColorCombineFnc_t func )	{

//	return;
	DEBUG_BEGINSCOPE("guColorCombineFunction", DebugRenderThreadId);

	LOG(0,"\nguColorCombineFunction(func=%s)",colorcombinefunc_t[func]);
	astate.flags &= ~STATE_DELTA0;
	switch(func)	{
		case GR_COLORCOMBINE_ZERO:				/* 0 */
			grColorCombine(GR_COMBINE_FUNCTION_ZERO,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_NONE,
						   GR_COMBINE_OTHER_NONE, FXFALSE);
			break;
		
		case GR_COLORCOMBINE_ONE:				/* 1 */
			grColorCombine(GR_COMBINE_FUNCTION_ZERO,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_NONE,
						   GR_COMBINE_OTHER_NONE, FXTRUE);
			break;
		
		case GR_COLORCOMBINE_ITRGB_DELTA0:
			astate.flags |= STATE_DELTA0;
		
		case GR_COLORCOMBINE_ITRGB:				/* iterált RGB */
			grColorCombine(GR_COMBINE_FUNCTION_LOCAL,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_NONE, FXFALSE);
			break;
		
		case GR_COLORCOMBINE_DECAL_TEXTURE:		/* adott texel */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_ONE,
						   GR_COMBINE_LOCAL_NONE,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;
			
		case GR_COLORCOMBINE_TEXTURE_TIMES_CCRGB:	/* texel * konstans */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_LOCAL,
						   GR_COMBINE_LOCAL_CONSTANT,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_DELTA0:
			astate.flags |= STATE_DELTA0;

		case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB:	/* texel * interpolált RGB */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_LOCAL,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ALPHA: /* texel * interpolált RGB + alfa */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA,
						   GR_COMBINE_FACTOR_LOCAL,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA:	/* texel * alfa */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_LOCAL_ALPHA,
						   GR_COMBINE_LOCAL_NONE,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA_ADD_ITRGB:
			grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL,
							GR_COMBINE_FACTOR_LOCAL_ALPHA,
							GR_COMBINE_LOCAL_ITERATED,
							GR_COMBINE_OTHER_TEXTURE, FXFALSE );
			break;

		case GR_COLORCOMBINE_DIFF_SPEC_A:			/* texel * alfa + interpolált RGB */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL,
						   GR_COMBINE_FACTOR_LOCAL_ALPHA,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;
		
		case GR_COLORCOMBINE_DIFF_SPEC_B:			/* texel * interpolált RGB + alfa */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA,
						   GR_COMBINE_FACTOR_LOCAL,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE );
			break;
		
		case GR_COLORCOMBINE_TEXTURE_ADD_ITRGB:		/* texel + interpolált RGB */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL,
						   GR_COMBINE_FACTOR_ONE,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_TEXTURE_SUB_ITRGB:		/* texel - interpolált RGB */
			grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL,
						   GR_COMBINE_FACTOR_ONE,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE, FXFALSE);
			break;

		case GR_COLORCOMBINE_CCRGB:					/* konstans */
			grColorCombine(GR_COMBINE_FUNCTION_LOCAL,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_CONSTANT,
						   GR_COMBINE_OTHER_NONE, FXFALSE);
			if ( (platform != PLATFORM_WINDOWS) && (config.Flags & CFG_TOMBRAIDER) ) grConstantColorValue(0x7f000000);
			break;
		
		case GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA:
			grColorCombine(GR_COMBINE_FUNCTION_BLEND,
						   GR_COMBINE_FACTOR_TEXTURE_ALPHA,
						   GR_COMBINE_LOCAL_CONSTANT,
						   GR_COMBINE_OTHER_ITERATED, FXFALSE);
			break;
		default:;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guAlphaSource( GrAlphaSource_t mode )	{
	
	DEBUG_BEGINSCOPE("guAlphaSource", DebugRenderThreadId);
	
	LOG(0,"\nguAlphaSource(mode=%s)",alphasource_t[mode]);
	switch(mode)	{
		case GR_ALPHASOURCE_CC_ALPHA:
			grAlphaCombine(GR_COMBINE_FUNCTION_LOCAL_ALPHA,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_CONSTANT,
						   GR_COMBINE_OTHER_NONE,
						   0);
			break;
		case GR_ALPHASOURCE_ITERATED_ALPHA:
			grAlphaCombine(GR_COMBINE_FUNCTION_LOCAL_ALPHA,
						   GR_COMBINE_FACTOR_NONE,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_NONE,
						   0);
			break;
		case GR_ALPHASOURCE_TEXTURE_ALPHA:
			grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_ONE,
						   GR_COMBINE_LOCAL_NONE,
						   GR_COMBINE_OTHER_TEXTURE,
						   0);
			break;
			
		case GR_ALPHASOURCE_TEXTURE_ALPHA_TIMES_ITERATED_ALPHA:
			grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
						   GR_COMBINE_FACTOR_LOCAL,
						   GR_COMBINE_LOCAL_ITERATED,
						   GR_COMBINE_OTHER_TEXTURE,
						   0);
			break;
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}

//Disable 'no return value' warning
#pragma warning (disable: 4035)


FxU32 EXPORT guEndianSwapWords(FxU32 value)	{
	_asm	{
		mov		eax,value
		ror		eax,16
	}
}


FxU16 EXPORT guEndianSwapBytes(FxU16 value)	{
	_asm	{
		mov		ax,value
		xchg	al,ah
	}
}


#pragma warning (default: 4035)

void EXPORT grGammaCorrectionValue( float value )	{
unsigned int				i, t;
float						corr;
RendererGeneral::GammaRamp	gammaRamp;
	
	DEBUG_BEGINSCOPE("grGammaCorrectionValue", DebugRenderThreadId);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	astate.gammaCorrectionValue = value;
	corr = 256 * ((float) config.Gamma)/100;
	if (fullScreenGammaSupported) {
		if (config.Flags & CFG_ENABLEGAMMARAMP)	{			
			for(i = 0; i < 256; i++)	{
				t = ((unsigned int) (corr * pow( ((float) i)/255,  1/value)) ) << 8;
				if (t > 0xFFFF) t = 0xFFFF;
				gammaRamp.red[i] = gammaRamp.green[i] = gammaRamp.blue[i] = (WORD) t;
			}
		} else {			
			for(i = 0; i < 256; i++)	{
				t = ((unsigned int) (corr * origGammaRamp.red[i])) / 256;
				if (t > 0xFFFF) t = 0xFFFF;
				gammaRamp.red[i] = (WORD) t;
				t = ((unsigned int) (corr * origGammaRamp.green[i])) / 256;
				if (t > 0xFFFF) t = 0xFFFF;
				gammaRamp.green[i] = (WORD) t;
				t = ((unsigned int) (corr * origGammaRamp.blue[i])) / 256;
				if (t > 0xFFFF) t = 0xFFFF;
				gammaRamp.blue[i] = (WORD) t;
			}
		}
		rendererGeneral->SetGammaRamp (&gammaRamp);
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAlphaTestFunction( GrCmpFnc_t function )	{

	DEBUG_BEGINSCOPE("grAlphaTestFunction",DebugRenderThreadId);
	
	//function = GR_CMP_ALWAYS;
	LOG(0,"\ngrAlphaTestFunction(function=%s)",cmpfnc_t[function]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;	
	if ((function > GR_CMP_ALWAYS) || function == astate.alphaTestFunc) return;
	DrawFlushPrimitives();

	astate.alphaTestFunc = function;
	
	if (rendererGeneral->IsAlphaTestUsedForAlphaBasedCk ()) {
		updateFlags |= UPDATEFLAG_ALPHATESTFUNC | UPDATEFLAG_COLORKEYSTATE;
	} else {
		rendererGeneral->EnableAlphaTesting ((function == GR_CMP_ALWAYS) ? FALSE : TRUE);
		rendererGeneral->SetAlphaTestFunction (astate.alphaTestFunc = function);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAlphaTestReferenceValue( GrAlpha_t value )	{
	
//		return;
	DEBUG_BEGINSCOPE("grAlphaTestReferenceValue", DebugRenderThreadId);

	LOG(0,"\ngrAlphaTestReferenceValue(value=%0x)", value);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (value == astate.alpharef) return;
	DrawFlushPrimitives();
	
	astate.alpharef = value;

	if (rendererGeneral->IsAlphaTestUsedForAlphaBasedCk ()) {
		updateFlags |= UPDATEFLAG_ALPHATESTREFVAL;
	} else {
		rendererGeneral->SetAlphaTestRefValue (value);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void	GlideUpdateAlphaTestState ()
{
	if (((astate.flags & STATE_CKMMASK) != STATE_CKMALPHABASED) || (astate.alphaTestFunc != GR_CMP_ALWAYS)) {
		if (updateFlags & UPDATEFLAG_ALPHATESTREFVAL)
			rendererGeneral->SetAlphaTestRefValue (astate.alpharef);

		if (updateFlags & UPDATEFLAG_ALPHATESTFUNC) {		
			rendererGeneral->EnableAlphaTesting ((astate.alphaTestFunc == GR_CMP_ALWAYS) ? FALSE : TRUE);
			rendererGeneral->SetAlphaTestFunction (astate.alphaTestFunc);
		}
	}
	updateFlags &= ~(UPDATEFLAG_ALPHATESTREFVAL | UPDATEFLAG_ALPHATESTFUNC);
}


void EXPORT grDitherMode( GrDitherMode_t mode )	{
	
	DEBUG_BEGINSCOPE("grDitherMode", DebugRenderThreadId);

	LOG(0,"\ngrDitherMode(mode=%s)", dithermode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	DrawFlushPrimitives();
	rendererGeneral->SetDitheringMode (mode);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDisableAllEffects( void )	{

	DEBUG_BEGINSCOPE("grDisableAllEffects",DebugRenderThreadId);

	LOG(0,"\ngrDisableAllEffects()");
	grAlphaBlendFunction(GR_BLEND_ONE,GR_BLEND_ZERO, 0, 0);
	grAlphaTestFunction(GR_CMP_ALWAYS);
	grChromakeyMode(GR_CHROMAKEY_DISABLE);
	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grCullMode( GrCullMode_t mode ) {
int	origin;

	//mode=GR_CULL_DISABLE;
	DEBUG_BEGINSCOPE("grCullMode", DebugRenderThreadId);
	
	LOG(0,"\ngrCullMode(mode=%s)",cullmode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	
	origin = (astate.locOrig != GR_ORIGIN_UPPER_LEFT) ? 0x10 : 0x0;
	mode &= 0x0F;
	if ((mode | origin) != astate.gcullmode) {
		DrawFlushPrimitives();
		astate.gcullmode = mode | origin;
		if (mode != GR_CULL_DISABLE)	{
			if (astate.locOrig != GR_ORIGIN_UPPER_LEFT)
				mode = (mode == GR_CULL_POSITIVE) ? GR_CULL_NEGATIVE : GR_CULL_POSITIVE;

			if (config.Flags2 & CFG_YMIRROR)
				mode = (mode == GR_CULL_POSITIVE) ? GR_CULL_NEGATIVE : GR_CULL_POSITIVE;
		}
		rendererGeneral->SetCullMode (mode);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grHints( GrHint_t type, FxU32 hintMask ) {

	DEBUG_BEGINSCOPE("grHints", DebugRenderThreadId);
	
	LOG(1,"\ngrHints(type=%s, hintMask=%0x)", hint_t[type], hintMask);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (type == GR_HINT_STWHINT)	{
		if (hintMask != astate.hints) DrawFlushPrimitives();
		astate.hints = hintMask;
		GlideUpdateUsingTmuWMode ();
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* A Voodoo Graphics subsystem soha nem foglalt (ha igen, akkor azt a DirectX úgyis kivárja */
/* a rajzoláskor) */
FxBool EXPORT grSstIsBusy( void )	{

	LOG(0,"\ngrSstIsBusy()");
	return(FXFALSE);
}


/* Vertical blank status: ha a frekvenciát az alkalmazás állítja be, akkor a státusz szerint */
/* soha nincs visszafutás. Ez veszélyes lehet. */
FxBool EXPORT grSstVRetraceOn( void )	{

	DEBUG_BEGINSCOPE("grSstVRetraceOn",DebugRenderThreadId);

	LOG(0,"\ngrSstVRetraceOn()");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return(FXFALSE);
	if (!(config.Flags & CFG_SETREFRESHRATE))	{
		if (rendererGeneral->GetVerticalBlankStatus ())
			return(FXTRUE);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXFALSE);
}


FxU32 EXPORT grSstStatus( void ) {
FxU32	temp;
		
	DEBUG_BEGINSCOPE("grSstStatus",DebugRenderThreadId);
	
	LOG(0,"\ngrSstStatus()");
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return(0);
	temp = 0x0FFFF040; // 0000 1111 1111 1111 1111 0000 0100 0000;
	temp |= (rendererGeneral->GetNumberOfPendingBufferFlippings () & 0x7) << 28;	
	temp |= (buffswapnum % movie.cmiBuffNum) << 10;
	if (!(config.Flags & CFG_SETREFRESHRATE))	{
		if (rendererGeneral->GetVerticalBlankStatus ())
			temp |= 0x40;
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(temp);
}


void EXPORT grResetTriStats( void )	{
	
	stats.trisProcessed = 0;
}


void EXPORT grTriStats(FxU32 *trisProcessed, FxU32 *trisDrawn)	{
	
	*trisProcessed = *trisDrawn = stats.trisProcessed;
}


FxU32 EXPORT grSstVideoLine( void )	{
FxU32 scanLine;
	
	DEBUG_BEGINSCOPE("grSstVideoLine",DebugRenderThreadId);

	LOG(0,"\ngrSstVideoLine()");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN )
		return(3);
	
	scanLine = rendererGeneral->GetCurrentScanLine ();
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return scanLine;
}


FxBool EXPORT grSstControlMode( FxU32 mode) {

	DEBUG_BEGINSCOPE("grSstControlMode",DebugRenderThreadId);

	LOG(0,"\ngrSstControlMode(mode=%s)",sstmode_t[mode-1]);
#ifndef GLIDE3

#ifdef	DOS_SUPPORT

	if (mode == GR_CONTROL_ACTIVATE)	{
		DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONSYSVM, NULL, 0, NULL, 0, NULL, NULL);
		DeviceIoControl(hDevice, DGSERVER_SETFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
		return(FXTRUE);
	}
	if (mode == GR_CONTROL_DEACTIVATE)	{
		DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
		return(FXTRUE);
	}

#endif

#endif
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(FXTRUE);
}


FxBool EXPORT grSstControl( FxU32 mode) {

	return(grSstControlMode(mode));
}


#ifdef GLIDE1

//void EXPORT grSstPassthruMode( GrSstPassthruMode_t mode)	{
void EXPORT grSstPassthruMode( int mode)	{

}

#endif


float EXPORT guFogTableIndexToW( int i )	{

	return( (float) floor(pow(2.0f, 3.0f + (float)(i >> 2)) / (8 - (i & 3))) );
}


void EXPORT guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
int		i;
double	max;
float	w;
	
	DEBUG_BEGINSCOPE("guFogGenerateExp", DebugRenderThreadId);
	
	max = 1.0f - exp(-density * guFogTableIndexToW(GR_FOG_TABLE_SIZE-1));
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		w = guFogTableIndexToW(i);
		fogTable[i] = (GrFog_t) ((255.0f - (255.0f * exp(-density * w))) / max);
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
int		i;
double	max;
float	w;
	
	DEBUG_BEGINSCOPE("grFogGenerateExp2", DebugRenderThreadId);
	
	max = 1.0f - exp(-(density * SQR(guFogTableIndexToW(GR_FOG_TABLE_SIZE))));
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		w = density*guFogTableIndexToW(i);
		fogTable[i] = (GrFog_t) ((255.0f - (255.0f*exp(-SQR(w)))) / max);
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guFogGenerateLinear( GrFog_t fogTable[GR_FOG_TABLE_SIZE],
						  float nearW, float farW ) {
int		i,j;
	
	DEBUG_BEGINSCOPE("grFogGenerateLinear", DebugRenderThreadId);
	
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		j = (int) (255.0f * (guFogTableIndexToW(i) - nearW) / (farW - nearW));
		if (j < 0) j = 0;
		if (j > 255) j = 255;
		fogTable[i] = (GrFog_t) j;
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grFogColorValue( GrColor_t value ) {
		
	DEBUG_BEGINSCOPE("grFogColorValue", DebugRenderThreadId);
	
	LOG(0, "\ngrFogColorValue (value=%x), in ARGB: %0x", value, GlideGetColor(value));
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (value != astate.fogcolor) {
		astate.fogcolor = value;
		if (astate.fogmodifier & GR_FOG_ADD2) value = 0;
		DrawFlushPrimitives();
		rendererGeneral->SetFogColor (GlideGetColor(value));
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grFogMode( GrFogMode_t mode ) {
DWORD fogColor;
DWORD fogEnable;
	
	//mode=GR_FOG_DISABLE;	
	DEBUG_BEGINSCOPE("grFogMode", DebugRenderThreadId);

	LOG(0, "\ngrFogMode(mode=%s | %s) ", fogmode_t[mode & 0xFF], fogmodemod_t[mode >> 8]);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	DrawFlushPrimitives();
	
	astate.vertexmode &= ~VMODE_ALPHAFOG;
	switch (astate.fogmode = (mode & 0xFF)) {
		case GR_FOG_WITH_ITERATED_ALPHA:
			astate.vertexmode |= VMODE_ALPHAFOG;	//no break;

		case GR_FOG_WITH_TABLE:
			fogEnable = TRUE;
			break;

		default:
			fogEnable = FALSE;
			break;
	}
	rendererGeneral->EnableFog (fogEnable);

	if ((astate.fogmodifier = (mode & (GR_FOG_MULT2 | GR_FOG_ADD2))) & GR_FOG_ADD2) fogColor = 0;
		else fogColor = astate.fogcolor;

	rendererGeneral->SetFogColor (GlideGetColor(fogColor));
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] )	{
unsigned int i, *src, *dst;
	
	DEBUG_BEGINSCOPE("grFogTable\n", DebugRenderThreadId);
	
	//QLOG("\ngrFogtable");
	//for(i=0;i<GR_FOG_TABLE_SIZE;i++) QLOG("%0x,",table[i]);
	src = (unsigned int *) table;
	dst = (unsigned int *) fogtable;
	for(i = 0; i < GR_FOG_TABLE_SIZE/4; i++) dst[i] = src[i] ^ 0xFFFFFFFF;
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Origo (és a koordinátarendszer) beállítása */
void EXPORT grSstOrigin( GrOriginLocation_t origin )	{
	
	DEBUG_BEGINSCOPE("grSstOrigin", DebugRenderThreadId);
	
	LOG(0,"\ngrSstOrigin( origin=%s )",origin_t[origin]);	
	if (astate.locOrig != origin) {
		astate.locOrig = origin;
		DrawComputeScaleCoEffs(origin);
		if (astate.gcullmode != GR_CULL_DISABLE)	{
			DrawFlushPrimitives();
			grCullMode(astate.gcullmode);
		}
	}
	return;
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Aktuális X felbontás lekérdezése */
FxU32 EXPORT grSstScreenWidth( void )	{
	
	return(appXRes);
}


/* Aktuális Y felbontás lekérdezése */
FxU32 EXPORT grSstScreenHeight( void )	{
	
	return(appYRes);
}


void EXPORT grRenderBuffer(GrBuffer_t buffer)	{
	
	DEBUG_BEGINSCOPE("grRenderBuffer", DebugRenderThreadId);

	LOG(0,"\ngrRenderBuffer(buffer=%s)",buffer_t[buffer]);

	if (config.debugFlags & DBG_FORCEFRONTBUFFER)
		buffer = GR_BUFFER_FRONTBUFFER;
	
	if (renderbuffer == buffer)
		return;

	DrawFlushPrimitives();
	
	if (runflags & RF_SCENECANBEDRAWN)
		rendererGeneral->EndScene();

	rendererGeneral->SetRenderTarget (buffer);

	if ((buffer == GR_BUFFER_FRONTBUFFER) && (!(rendererGenCaps & CAP_FRONTBUFFAVAILABLE)))
		astate.flags |= STATE_NORENDERTARGET;
	else
		astate.flags &= ~STATE_NORENDERTARGET;

	GlideUpdateConstantColorKeyingState ();
	
	if (rendererGeneral->BeginScene()) {
		runflags |= RF_SCENECANBEDRAWN;
	} else {
		runflags &= ~RF_SCENECANBEDRAWN;
	}

	renderbuffer = buffer;

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/*-------------------------------------------------------------------------------------------*/
/* grSstWinOpen, grSstWinClose és grGlideShutDown implementáció								 */
/*-------------------------------------------------------------------------------------------*/

static	LRESULT	ProcessThreadCommand (DWORD command)
{
	switch (command) {
		case DGCOMMAND_GRSSTWINOPEN:
			winopenstruc.ret = grSstWinOpen(winopenstruc.hwnd,winopenstruc.res,winopenstruc.refresh,winopenstruc.l_cFormat,winopenstruc.locateOrigin,
											winopenstruc.numBuffers,winopenstruc.numAuxBuffers);
			return (0);
		
		case DGCOMMAND_GRSSTWINCLOSE:
			grSstWinClose();
			return (0);
		
		case DGCOMMAND_GETDEVICESTATUS:
		{
			RendererGeneral::DeviceStatus deviceStatus = rendererGeneral->GetDeviceStatus ();
			return deviceStatus;
		}

		case DGCOMMAND_RESETDEVICE:
			return rendererGeneral->DeviceReset ();

		default:
			return 0;
	}
}


LRESULT CALLBACK GlideConsumerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)	{
		case WM_USER:
			return ProcessThreadCommand (lParam);
			/*if (lParam==0)	{
				winopenstruc.ret = grSstWinOpen(winopenstruc.hwnd,winopenstruc.res,winopenstruc.refresh,winopenstruc.l_cFormat,winopenstruc.locateOrigin,
												winopenstruc.numBuffers,winopenstruc.numAuxBuffers);
			} else {
				grSstWinClose();
			}
			return(0);*/
		case WM_CREATE:
			return(0);
		/*case WM_TIMER:
			CallConsumer(1);
			return(0);*/
		case WM_DESTROY:
			//PostQuitMessage(0);
			return(0);
		default:
			return(DefWindowProc(hwnd,uMsg,wParam,lParam));
	}
}


LRESULT CALLBACK newwndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	if ( (uMsg == WM_USER) && (wParam == 'DEGE') )	{
		if (lParam == 0) {
			threadCommandWindow = CreateWindowEx(0, "DGVOODOO", "", WS_OVERLAPPED, 0, 0, 100, 100, NULL, NULL, hInstance, NULL);
		} else {
			DestroyWindow(threadCommandWindow);
			threadCommandWindow = NULL;
		}
		
		return(0);
	} else
		return(CallWindowProc(oldwndproc, hwnd, uMsg, wParam, lParam));
}


int		CreateOrRemoveThreadCommandWindow (HWND hwnd, int remove)
{
	//_asm jjj: jmp jjj
	if (!remove && (threadCommandWindow != NULL))
		return (0);

	if (remove && (threadCommandWindow == NULL))
		return (0);

	if ( (oldwndproc = ( LRESULT (CALLBACK *)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
						 SetWindowLong(hwnd, GWL_WNDPROC, (LONG) newwndproc)) == NULL)
		return(GetLastError());

	SendMessage(hwnd, WM_USER, 'DEGE', !remove ? 0 : 1);

	if ( (SetWindowLong(hwnd, GWL_WNDPROC, (LONG) oldwndproc)) == (LONG) NULL)
		return(GetLastError());

	return(0);
}


void	CleanUpWindow()	{

	DEBUG_BEGINSCOPE("CleanUpWindow",DebugRenderThreadId);

#ifdef	DOS_SUPPORT

	if (platform == PLATFORM_WINDOWS) {

#endif
		CreateOrRemoveThreadCommandWindow (movie.cmiWinHandle, 1);
		
		if (runflags & RF_WINDOWCREATED)	{	
			DestroyWindow(movie.cmiWinHandle);
			UnregisterClass("DGVOODOO",hInstance);
		}

#ifdef	DOS_SUPPORT
	} else {
		
		DestroyRenderingWindow ();
	
	}
#endif
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


int		DoThreadCall (int command)	{

	if (threadCommandWindow != NULL) {
		return SendMessage(threadCommandWindow, WM_USER, 'DEGE', command);
	} else {
		return ProcessThreadCommand (command);
	}

}


/*int		DoThreadCall(HWND hwnd, int command)	{

	if ( (oldwndproc = ( LRESULT (CALLBACK *)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
						 SetWindowLong(hwnd, GWL_WNDPROC, (LONG) newwndproc)) == NULL)
		return(GetLastError());

	SendMessage(hwnd, WM_USER, 'DEGE', 0);
	
	if ( (SetWindowLong(hwnd, GWL_WNDPROC, (LONG) oldwndproc)) == (LONG) NULL)
		return(GetLastError());

	if (threadCommandWindow == NULL)
		return(GetLastError());

	SendMessage(threadCommandWindow, WM_USER, 'DEGE', command);

	if ( (oldwndproc = ( LRESULT (CALLBACK *)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
						 SetWindowLong(hwnd, GWL_WNDPROC, (LONG) newwndproc)) == NULL)
		return(GetLastError());

	SendMessage(hwnd, WM_USER, 'DEGE', 1);
	
	if ( (SetWindowLong(hwnd, GWL_WNDPROC, (LONG) oldwndproc)) == (LONG) NULL)
		return(GetLastError());

	return(0);
}*/


FxBool EXPORT1 grSstWinOpen(FxU32 _hwnd, GrScreenResolution_t res, \
							GrScreenRefresh_t refresh, GrColorFormat_t l_cFormat, \
							GrOriginLocation_t locateOrigin, int numBuffers, \
							int numAuxBuffers)	{
unsigned int	i;
WNDCLASS		wndc = {0, GlideConsumerProc, 0, 0, 0, 0, 0, 0, NULL, "DGVOODOO"};
HWND			hwnd = (HWND) _hwnd;

#ifdef	DOS_SUPPORT

/*unsigned*/ char	windowTitle[256];
/*unsigned*/ char	temp[16];

#endif

	
	DEBUG_BEGINSCOPE("grSstWinOpen", DebugRenderThreadId);

	//l_cFormat = GR_COLORFORMAT_ABGR;

	if (runflags & RF_INITOK) return(FXFALSE);
	LOG(1,"\ngrSstWinOpen(hwnd=%0x, res=%s, ref=%s, cformat=%s, org_loc=%s, num_buffers=%d, num_aux=%d)", \
						hwnd, res_t[res], ref_t[refresh], cFormat_t[l_cFormat], \
						locOrig_t[locateOrigin], numBuffers, numAuxBuffers);

	GlideResetActState ();

	ZeroMemory(&movie, sizeof(MovieData));
	runflags = 0;

	DEBUGLOG (0,"\ngrSstWinopen: required resolution: %dx%d", _xresolution[res], _yresolution[res]);
	DEBUGLOG (1,"\ngrSstWinopen: megadott felbontás: %dx%d", _xresolution[res], _yresolution[res]);
	
	DEBUGLOG (0,"\ngrSstWinopen: required refresh rate: %dHz",_refreshrate[refresh]);
	DEBUGLOG (1,"\ngrSstWinopen: megadott frissítés: %dHz",_refreshrate[refresh]);
	
	cFormat = l_cFormat;
	astate.locOrig = locateOrigin;

	if ( (res > MAX_RES_CONSTANT) || (refresh > MAX_REFRATE_CONSTANT) ) return(FXFALSE);

	//if (!rendererGeneral->InitRendererApi ())
	//eturn(FXFALSE);

#ifdef	DOS_SUPPORT

	if (platform != PLATFORM_WINDOWS) {
		hwnd = CreateRenderingWindow();

		if (platform == PLATFORM_DOSWINNT) {
			ShowWindow(warningBoxHwnd, SW_HIDE);	
			ShowWindow(renderingWinHwnd, SW_SHOW);
			SetForegroundWindow(renderingWinHwnd);
		} else if (platform == PLATFORM_DOSWIN9X) {
			if (config.Flags & CFG_WINDOWED)
				DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONSYSVM, NULL, 0, NULL, 0, NULL, NULL);
		}
	
	} else {

#endif
		
		if (hwnd == NULL)
			hwnd = GetActiveWindow();

		wndc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wndc.hInstance = hInstance;
		RegisterClass(&wndc);
		if ( ((HWND) hwnd) == NULL)	{
			if ( (movie.cmiWinHandle = CreateWindowEx(0, "DGVOODOO", "dgVoodoo", WS_OVERLAPPEDWINDOW,
													  CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
													  NULL, NULL, hInstance, NULL)) == NULL)
				return(FXFALSE);
			
			if (movie.cmiWinHandle != NULL) {
				runflags |= RF_WINDOWCREATED;
				ShowWindow(movie.cmiWinHandle, SW_SHOWNORMAL);

				CreateOrRemoveThreadCommandWindow (hwnd, 0);

			} else {
				return(FXFALSE);
			}
		} else {
			
			CreateOrRemoveThreadCommandWindow (hwnd, 0);

			if ( GetCurrentThreadId() != GetWindowThreadProcessId( (HWND) hwnd, NULL) )	{
				winopenstruc.hwnd = _hwnd;
				winopenstruc.res = res;
				winopenstruc.refresh = refresh;
				winopenstruc.l_cFormat = l_cFormat;
				winopenstruc.locateOrigin = locateOrigin;
				winopenstruc.numBuffers = numBuffers;
				winopenstruc.numAuxBuffers = numAuxBuffers;
				if (i = DoThreadCall(/*(HWND) hwnd,*/ DGCOMMAND_GRSSTWINOPEN))	{
				  DEBUGLOG(0,"\n   Error: grSstWinOpen: Multithreading issue: calling grSstWinopen in the context of the thread of the Glide-window has failed.");
				  DEBUGLOG(0,"\n          Error code returned by GetLastError is: %d", i);
				  DEBUGLOG(1,"\n   Hiba: grSstWinOpen: Többszálúság kezelése: a grSstWinOpen függvényt nem sikerült meghívni a Glide-ablak szálából.");
				  DEBUGLOG(1,"\n          A hibakód, amit a GetLastError visszaadott: %d", i);
				}
				return(winopenstruc.ret);
			}
		}
		movie.cmiWinHandle = (HWND) hwnd;

#ifdef	DOS_SUPPORT
	
	}

	rendererGeneral->PrepareForInit (renderingWinHwnd);

	if (!(config.Flags & CFG_WINDOWED)) {
		LONG exStyle = GetWindowLong (renderingWinHwnd, GWL_EXSTYLE);
		SetWindowLong (renderingWinHwnd, GWL_EXSTYLE, exStyle | WS_EX_TOPMOST);
	}

#endif

	if (!rendererGeneral->InitRendererApi ()) {
		CleanUpWindow();
		return(FXFALSE);
	}

	movie.cmiXres = appXRes = _xresolution[res];
	movie.cmiYres = appYRes = _yresolution[res];
	movie.cmiBuffNum = numBuffers = (config.Flags & CFG_TRIPLEBUFFER) ? 3 : ( (numBuffers != 0) ? numBuffers : numAuxBuffers);
	if ( (numBuffers != 2) && (numBuffers != 3) ) return(FXFALSE);
	
	if ( (config.dxres != 0) && (config.dyres != 0) )	{
		if ( (appXRes != config.dxres) || (appYRes != config.dyres) )	{			
			movie.cmiXres = config.dxres;
			movie.cmiYres = config.dyres;
			drawresample = 1;
		} else drawresample = 0;
	}


#ifdef	DOS_SUPPORT
	
	if (platform != PLATFORM_WINDOWS) {
		/* Ablak fejléc szövege */
		windowTitle[0] = 0;
		strcat(windowTitle, productName);
		if (c->progname[0] != 0 )	{
			strcat(windowTitle, " - ");
			strcat(windowTitle, c->progname + 1);
		}
		if ( actResolution != ((movie.cmiXres << 16) + movie.cmiYres) )	{
			i = GetIdealWindowSizeForResolution (movie.cmiXres, movie.cmiYres);
			if (IsWindowSizeIsEqualToResolution (renderingWinHwnd)) {	
				SetWindowClientArea(renderingWinHwnd, i >> 16, i & 0xFFFF);
			} else {
				AdjustAspectRatio (renderingWinHwnd, i >> 16, i & 0xFFFF);
			}
		}
		actResolution = (movie.cmiXres << 16) + movie.cmiYres;

		strcat(windowTitle," (GLIDE ");
		intToStr(temp, movie.cmiXres);
		strcat(windowTitle, temp);
		strcat(windowTitle, "x");
		intToStr(temp, movie.cmiYres);
		strcat(windowTitle, temp);
		strcat(windowTitle, ")");
		SetWindowText(renderingWinHwnd, windowTitle);
	}

#endif

	rendererGenCaps = rendererGeneral->GetApiCaps ();

	movie.cmiBitPP = (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16;	
	movie.cmiByPP = movie.cmiBitPP / 8;

	unsigned int swmFlags = ((config.Flags & CFG_WINDOWED) ? SWM_WINDOWED : 0) |
							//((config.Flags & CFG_SETREFRESHRATE) ? 0 : MV_NOCONSUMERCALL)
							((config.Flags2 & CFG_CLOSESTREFRESHFREQ) ? SWM_USECLOSESTFREQ : 0);

	if (platform == PLATFORM_WINDOWS)
		swmFlags |= SWM_MULTITHREADED;

	if (!rendererGeneral->SetWorkingMode (movie.cmiXres, movie.cmiYres, (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16,
										  (config.Flags2 & CFG_CLOSESTREFRESHFREQ) ? _refreshrate[refresh] : config.RefreshRate,
										  numBuffers, hwnd, config.dispdev, config.dispdriver,
										  swmFlags | SWM_3D)) {
		rendererGeneral->UninitRendererApi ();
		CleanUpWindow();
		return(FXFALSE);
	}


	isWindowMaximized = IsZoomed (hwnd);
	isWindowMinimized = IsIconic (hwnd);
	winopenstruc.hwnd = (FxU32) hwnd;

#ifdef	DOS_SUPPORT

	if (platform != PLATFORM_WINDOWS) {

		//HideMouseCursor(0);
	    if (platform == PLATFORM_DOSWIN9X) {
	    	if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow( (HWND) c->WinOldApHWND, SW_HIDE);
	    } else {
			if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow(consoleHwnd, SW_HIDE);
	    }
	    ShowWindow(warningBoxHwnd, SW_HIDE);
	    ShowWindow(renderingWinHwnd, (config.Flags & CFG_WINDOWED) ? SW_SHOWNORMAL : SW_SHOWMAXIMIZED);
	    SetForegroundWindow(movie.cmiWinHandle);
	} else {

#endif

		if (config.Flags & CFG_WINDOWED) {
			RECT rect;
			RECT clientRect;
			GetWindowRect (hwnd, &rect);
			GetClientRect (hwnd, &clientRect);
			SetWindowPos (hwnd, NULL, 0, 0, movie.cmiXres + rect.right-rect.left-(clientRect.right-clientRect.left),
						  movie.cmiYres + rect.bottom-rect.top-(clientRect.bottom-clientRect.top),
						  SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

			ShowWindow (hwnd, SW_SHOWNORMAL);
		}

#ifdef	DOS_SUPPORT
	}
#endif

	runflags |= RF_INITOK | RF_SCENECANBEDRAWN;
	updateFlags = 0;
	//astate.flags = 0;
	//rendererGeneral->GetCkMethodPreferenceOrder (&ckmPreferenceOrder);

	if (!GlideCreateFogTables())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: GlideCreateFogTables() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: GlideCreateFogTables() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
 	if (!TexInit())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: TexInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: TexInit() hiba");
		grSstWinClose ();
		return(FXFALSE);
	}
	if (!LfbInit())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: LfbInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: LfbInit() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
	if (!LfbDepthBufferingInit())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: LfbDepthBufferingInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: LfbDepthBufferingInit() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
	if (!DrawInit())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: DrawInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: DrawInit() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
	if (!GrabberInit(1))	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: GlideGrabberInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: GlideGrabberInit() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
	
	if (!rendererGeneral->BeginScene()) {
		DEBUGLOG(0,"\n   Error: grSstWinopen: BeginScene() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: BeginScene() hiba");
		grSstWinClose ();
		return(FXFALSE);
	}

	rendererGeneral->WriteOutRendererCaps ();

	grChromakeyMode(GR_CHROMAKEY_DISABLE);
	grChromakeyValue(0x0);
	grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
				   GR_COMBINE_FACTOR_ONE,
				   GR_COMBINE_LOCAL_NONE,
				   GR_COMBINE_OTHER_CONSTANT,
				   FXFALSE);
	GlideAlphaCombine();
	grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
				   GR_COMBINE_FACTOR_ONE,
				   GR_COMBINE_LOCAL_ITERATED,
				   GR_COMBINE_OTHER_ITERATED,
				   FXFALSE);
	GlideColorCombine ();
	grConstantColorValue (0x0FFFFFFFF);
	grConstantColorValue4 (0.0f, 0.0f, 0.0f, 0.0f);
	grCullMode (GR_CULL_DISABLE);
	grClipWindow (0, 0, appXRes, appYRes);
	grAlphaBlendFunction (GR_BLEND_ONE, GR_BLEND_ZERO, GR_BLEND_ONE, GR_BLEND_ZERO);
	grAlphaTestFunction (GR_CMP_ALWAYS);
	grAlphaTestReferenceValue (0x0);
	grAlphaControlsITRGBLighting (FXFALSE);
	grDitherMode (GR_DITHER_4x4);	
	grTexMipMapMode (0, GR_MIPMAP_DISABLE, FXFALSE);
	grTexLodBiasValue (0, 0.0f);
	grDepthBufferMode (GR_DEPTHBUFFER_DISABLE);
	grDepthMask (FXFALSE);
	grDepthBiasLevel (0);
	
	grColorMask (FXTRUE, FXTRUE);
	grFogMode (GR_FOG_DISABLE);
	grFogColorValue (0x0);

	grLfbConstantAlpha (0xFF);
	grLfbConstantDepth (0x0);

	grTexMultibase (0, FXFALSE);
	GrTexInfo texInfo = {GR_LOD_1, GR_LOD_1, GR_ASPECT_1x1, GR_TEXFMT_RGB_332, NULL};
	grTexMultibaseAddress (0, GR_TEXBASE_256, 0x0, GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_128, 0x0, GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_64, 0x0, GR_MIPMAPLEVELMASK_BOTH, &texInfo);
	grTexMultibaseAddress (0, GR_TEXBASE_32_TO_1, 0x0, GR_MIPMAPLEVELMASK_BOTH, &texInfo);

	/* GammaControl-t támogatja a hardver? */
	fullScreenGammaSupported = (!(swmFlags & SWM_WINDOWED)) && rendererGeneral->CreateFullScreenGammaControl (&origGammaRamp);

 	grGammaCorrectionValue(1.0f);

	rendererLfbDepth->ClearBuffers ();
	renderbuffer = GR_BUFFER_BACKBUFFER;

	grRenderBuffer (GR_BUFFER_BACKBUFFER);

#ifdef	DOS_SUPPORT
	
	if (platform != PLATFORM_WINDOWS) {
	    HideMouseCursor(0);
	    if (platform == PLATFORM_DOSWIN9X) {
	        DeviceIoControl(hDevice, DGSERVER_SETFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
	    } else {
			if (config.Flags & CFG_SETMOUSEFOCUS) HideMouseCursor(1);
	    }
	    c->kernelflag |= KFLAG_GLIDEACTIVE;
	} else {

#endif
		SetWindowPos(movie.cmiWinHandle, HWND_TOP, 0, 0, movie.cmiXres, movie.cmiYres, SWP_NOMOVE);
		if (!(config.Flags & CFG_WINDOWED))	{
			ShowCursor(FALSE);
			ShowCursor(FALSE);
		}
#ifdef	DOS_SUPPORT	

	}

#endif

	//renderbuffer = GR_BUFFER_FRONTBUFFER;
	buffswapnum = 0;
	LOG(0,"\n--- end of GrSstWinOpen ---");
	
	//return(FXTRUE);

	DEBUG_ENDSCOPE(DebugRenderThreadId);

 	//DEBUG_GLOBALTIMEEND;
	//SetWindowPos (hwnd, HWND_TOP, 0, 0, 10, 10, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOREDRAW);

	return(FXTRUE);
}


/* Glide lelövése: leállítjuk az idôzítôt, visszaváltunk eredeti felbontásba, stb. */
void EXPORT grSstWinClose( void )	{
int i;
	
	DEBUG_BEGINSCOPE("grSstWinClose", DebugRenderThreadId);

	if (runflags & RF_INITOK)	{

#ifdef	DOS_SUPPORT

		if (platform != PLATFORM_WINDOWS) {
			if (config.Flags & CFG_SETMOUSEFOCUS) RestoreMouseCursor(1);
		} else {

#endif		
			
			if (GetCurrentThreadId() != GetWindowThreadProcessId(movie.cmiWinHandle, NULL) )	{
				if (i = DoThreadCall(/*movie.cmiWinHandle,*/ DGCOMMAND_GRSSTWINCLOSE))	{
					DEBUGLOG(0,"\n   Error: grSstWinClose: Multithreading issue: calling grSstWinClose in the context of the thread of the Glide-window has failed.");
					DEBUGLOG(0,"\n          Error code returned by GetLastError is: %d",i);
					DEBUGLOG(1,"\n   Hiba: grSstWinClose: Többszálúság kezelése: a grSstWinClose függvényt nem sikerült meghívni a Glide-ablak szálából.");
					DEBUGLOG(1,"\n         A hibakód, amit a GetLastError visszaadott: %d",i);
				}

				if (oldwndproc != NULL)
					SetWindowLong(movie.cmiWinHandle, GWL_WNDPROC, (LONG) oldwndproc);
				
				return;
			}
#ifdef	DOS_SUPPORT
		
		}

#endif

		LfbCleanUp ();
		DrawCleanUp ();
		GrabberCleanUp ();
		TexCleanUp ();
		LfbDepthBufferingCleanUp ();
		runflags = 0;
		GlideDestroyFogTables ();

		rendererGeneral->EndScene();

		if (fullScreenGammaSupported)
			rendererGeneral->DestroyFullScreenGammaControl ();

		if (isWindowMaximized) {
			ShowWindow ((HWND) winopenstruc.hwnd, SW_MAXIMIZE);
		} else if (isWindowMinimized) {
			ShowWindow ((HWND) winopenstruc.hwnd, SW_MINIMIZE);
		} else {
			ShowWindow ((HWND) winopenstruc.hwnd, SW_SHOWNA);
		}

		rendererGeneral->RestoreWorkingMode ();
		rendererGeneral->UninitRendererApi ();

#ifdef	DOS_SUPPORT

		if (platform != PLATFORM_WINDOWS) {
			
			RestoreMouseCursor (0);
			ShowWindow (renderingWinHwnd, SW_HIDE);
			DestroyRenderingWindow ();
			
			switch (platform) {
				case PLATFORM_DOSWIN9X:
					if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow( (HWND) c->WinOldApHWND,SW_SHOW);
						DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONVM, NULL, 0, NULL, 0, NULL, NULL);
					break;
				
				case PLATFORM_DOSWINNT:
					if (!(config.Flags & CFG_WINDOWED)) Sleep(500);
					if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow(consoleHwnd, SW_SHOW);	
					SetActiveWindow(consoleHwnd);
					SetForegroundWindow(consoleHwnd);
					ShowWindow(consoleHwnd, SW_MAXIMIZE);
					break;
			}		
			c->kernelflag &= ~KFLAG_GLIDEACTIVE;
		} else {
#endif
			CleanUpWindow ();

#ifdef	DOS_SUPPORT
		}
#endif
		if (!(config.Flags & CFG_WINDOWED))	{
			ShowCursor(TRUE);
			ShowCursor(TRUE);
		}
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


/* Glide könyvtár kilövése: véglegesen eldobunk minden objektumot a DirectX-bôl, */
/* a becsatolt DLL-eket (DDRAW és WINMM) eldobjuk								 */
void EXPORT grGlideShutdown()	{

	DEBUG_BEGINSCOPE("grGlideShutdown", DebugRenderThreadId);
	
	grSstWinClose();
	TexCleanUpAtShutDown();

#ifdef	DOS_SUPPORT
	
	if (platform != PLATFORM_WINDOWS) {
		ShowWindow(warningBoxHwnd, SW_SHOW);
	}

#endif

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	DEBUG_GLOBALTIMEEND;
}


#ifdef GLIDE1

FxBool EXPORT grSstOpen(GrScreenResolution_t res, GrScreenRefresh_t ref, GrColorFormat_t cformat,
				 GrOriginLocation_t org_loc, GrSmoothingMode_t smoothing_mode, int num_buffers )	{

	DEBUG_BEGINSCOPE("grSstOpen", DebugRenderThreadId);

	config.Flags |= CFG_LFBREALVOODOO;
	config.Flags2 |= CFG_LFBNOMATCHINGFORMATS;
	return(grSstWinOpen((FxU32) NULL, res, ref, cformat, org_loc,num_buffers, 0));
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}

#endif