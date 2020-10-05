/*--------------------------------------------------------------------------------- */
/* GlideImp.c - Glide main DLL implementation (general stuffs)                      */
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
/* dgVoodoo: Glide.c																		*/
/*			 Glide-függvények																*/
/*------------------------------------------------------------------------------------------*/

#include "dgVoodooBase.h"
#include <windows.h>
#include <glide.h>
#include <string.h>
#include "Dgw.h"
#include "Main.h"

#include <math.h>
#include "Debug.h"
#include "Log.h"
#include "Dos.h"
#include "DosCommon.h"
#include "VxdComm.h"

#include "dgVoodooConfig.h"
#include "pshaders.h"
#include "draw.h"
#include "lfbdepth.h"
#include "dgVoodooGlide.h"
#include "Texture.h"
#include "Grabber.h"
#include "Resource.h"

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

MOVIEDATA					movie;						/* Movie kommunikációs struktúra */
LPDIRECTDRAW7				lpDD = NULL;				/* fõ DirectDraw objektum */
LPDIRECT3D7					lpD3D = NULL;				/* fõ Direct3D objektum */
LPDIRECT3DDEVICE7			lpD3Ddevice = NULL;			/* Direct3D device */
LPDIRECTDRAWGAMMACONTROL	lpDDGamma = NULL;			/* Gamma control: a primary surface-hez csatolva,*/
														/* ha támogatott (csak full scr) */
LPDIRECTDRAWSURFACE7		lpDDFrontBuff = NULL;		/* Frontbuffer */
LPDIRECTDRAWSURFACE7		lpDDBackBuff = NULL;		/* Backbuffer */

DWORD						d3dlocal;
DWORD						d3dother;
DWORD						d3dfactor;					/* local, other és factor paraméterek átkonvertálva D3D-paraméterekre */

GrColorFormat_t				cFormat;						/* Glide színformátum */
unsigned int				buffswapnum;					/* összesen hányszor váltottak puffert */
float						IndToW[GR_FOG_TABLE_SIZE+1];	/* W meghatározása egy indexbõl (ködtáblázathoz) */
GrFog_t						fogtable[GR_FOG_TABLE_SIZE+1];	/* Ködtábla */
unsigned char				*WToInd;						/* index meghatározása W-bõl (ködtáblához) */

DDGAMMARAMP					origGammaRamp;				/* eredeti gammaértékek */
unsigned int				callflags;
unsigned int				runflags = 0;
int							renderbuffer;				/* melyik pufferbe megy a rajzolás */

_GlideActState				astate;
_stat						stats;
_winopenstruc				winopenstruc;
HWND						threadCommandWindow;
LRESULT						(CALLBACK *oldwndproc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = NULL;

unsigned int				appXRes, appYRes;			/* Alk. által megadott felbontás */
int							drawresample;				/* A megadott felbontás eltér az alkalmazásétól */

unsigned short				_xresolution[] = {320, 320, 400, 512, 640, 640, 640, 640, 800, 960, 856, 512, 1024, 1280, 1600, 400};
unsigned short				_yresolution[] = {200, 240, 256, 384, 200, 350, 400, 480, 600, 720, 480, 256, 768, 1024, 1200, 300};
unsigned char				_refreshrate[] = {60, 70, 72, 75, 80, 90, 100, 85, 120};
int							_3dnum, _3dtype;

float						depthBias_Z;
int							depthBias_W;
float						depthBias_cz, depthBias_cw;

float						xScale, yScale;
float						xConst, yConst;

/*-----------------------------------------------------------------------------------------	*/
/*....................................... Táblák ...........................................*/

/* A Glide alpha blending módok (faktorok) és a D3D módjai (faktorai)között leképezô táblázat */
/* Megtehetjük, mert a Glide-konstansok 0-tól valameddig vannak definiálva ilyen sorrendben. */

DWORD alphamodelookupsrc[] = {D3DBLEND_ZERO,			/* GR_BLEND_ZERO */
							  D3DBLEND_SRCALPHA,		/* GR_BLEND_SRC_ALPHA */
							  D3DBLEND_DESTCOLOR,		/* GR_BLEND_DST_COLOR */
							  D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
							  D3DBLEND_ONE,				/* GR_BLEND_ONE */
							  D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
							  D3DBLEND_INVDESTCOLOR,	/* GR_BLEND_ONE_MINUS_DST_COLOR */													   
							  D3DBLEND_INVDESTALPHA,	/* GR_BLEND_ONE_MINUS_DST_ALPHA */
							  0,0,0,0,0,0,0,			/* ezek fenntartottak */						  
							  D3DBLEND_SRCALPHASAT};	/* GR_BLEND_ALPHA_SATURE */

/* Magyarázat a GR_BLEND_PREFOG_COLOR-hoz: ez a blendelési mód a forrásszínt (SRCCOLOR) */
/* használja blending factorként, ua. mint a GR_BLEND_SRC_COLOR, annyi különbséggel, hogy */
/* ezen blending factorra nincs hatással a köd (az ugyanis még az alpha blending elõtt */
/* történik). */
DWORD alphamodelookupdst[] = {D3DBLEND_ZERO,			/* GR_BLEND_ZERO */
							  D3DBLEND_SRCALPHA,		/* GR_BLEND_SRC_ALPHA */
							  D3DBLEND_SRCCOLOR,		/* GR_BLEND_SRC_COLOR */
							  D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
							  D3DBLEND_ONE,				/* GR_BLEND_ONE */
							  D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
							  D3DBLEND_INVSRCCOLOR,		/* GR_BLEND_ONE_MINUS_SRC_COLOR */													
							  D3DBLEND_INVDESTALPHA,	/* GR_BLEND_ONE_MINUS_DST_ALPHA */
							  0,0,0,0,0,0,0,			/* ezek fenntartottak */
							  D3DBLEND_SRCCOLOR};		/* GR_BLEND_PREFOG_COLOR */

/* Glide alpha testing függvények */
DWORD cmpFuncLookUp[] = {	D3DCMP_NEVER,
							D3DCMP_LESS,
							D3DCMP_EQUAL,
							D3DCMP_LESSEQUAL,
							D3DCMP_GREATER,
							D3DCMP_NOTEQUAL,
							D3DCMP_GREATEREQUAL,
							D3DCMP_ALWAYS};

/* combine local paramétert konvertáló táblázat */
DWORD combinelocallookup[] = {D3DTA_DIFFUSE, D3DTA_TFACTOR, D3DTA_DIFFUSE};

/* combine other paramétert konvertáló táblázat */
DWORD combineotherlookup[] = {D3DTA_DIFFUSE, D3DTA_TEXTURE, D3DTA_TFACTOR};

DWORD combineargd3dlookup[] = {D3DTOP_BLENDDIFFUSEALPHA, 0, D3DTOP_BLENDTEXTUREALPHA, D3DTOP_BLENDFACTORALPHA};

/* lookup táblázat ahhoz, hogy egy combine function használja-e az other paramétert */
char combinefncotherusedlookup[] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter a textúra egy adott komponensét jelöli-e */
char combinefactortexusedlookup[] = {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0};

/* lookup táblázat ahhoz, hogy a factor paraméter a local, other vagy alocal paramétert jelöli-e  */
/* 0 - egyik sem, 1 - local, 2 - aother, 4 - alocal */
char combinefactorwhichparam[] = {0, 1, 2, 4, 0, 0, 0, 0, 0, 1, 2, 4, 0, 0};

/* lookup táblázat ahhoz, hogy az adott combine függvény mely paramétereket használja */
/* 001b = local, 010b = other, 100b = alocal, 1000b = factor */
char combinefncwhichparams[] = {0x0, 0x1, 0x4, 0xA, 0xB, 0x0E, 0xB, 0xB, 0x0F, 0x9, 0, 0, 0, 0, 0, 0, 0x0D };

DWORD D3DStates[] = {D3DRENDERSTATE_ALPHATESTENABLE, D3DRENDERSTATE_ALPHABLENDENABLE,
					 D3DRENDERSTATE_SRCBLEND, D3DRENDERSTATE_DESTBLEND, D3DRENDERSTATE_ZENABLE,
					 D3DRENDERSTATE_CULLMODE, D3DRENDERSTATE_ZWRITEENABLE, 
					 D3DRENDERSTATE_TEXTUREPERSPECTIVE, D3DRENDERSTATE_COLORKEYENABLE,
					 D3DRENDERSTATE_FOGENABLE};

DWORD TSStates[] = {D3DTSS_COLOROP,  D3DTSS_COLORARG1,  D3DTSS_COLORARG2,
					D3DTSS_ALPHAOP, D3DTSS_ALPHAARG1, D3DTSS_ALPHAARG2,
					D3DTSS_ADDRESSU, D3DTSS_ADDRESSV, D3DTSS_MAGFILTER, D3DTSS_MINFILTER};

/*-------------------------------------------------------------------------------------------*/
/*.............................. DirectX callback-függvények.................................*/

/* Ez a függvény a 3D driverek felsorolásakor hívódik meg. */
/* Vagy a konfigban megadott meghajtót választja ki, vagy az elsõ hardvereset. */
HRESULT WINAPI GlideEnum3DDeviceCallback(LPSTR lpDevDesc,
										 LPSTR lpDevName,
										 LPD3DDEVICEDESC7 lpDevCaps,
										 LPVOID lpContext) {
int	type;

	DEBUG_BEGIN("GlideEnum3DDeviceCallback", DebugRenderThreadId, 29);
	
	if (config.dispdriver != 0)	{
		if ( (config.dispdriver - 1) == _3dnum ) CopyMemory(&movie.cmiDeviceGUID,&lpDevCaps->deviceGUID,sizeof(GUID));
	} else {
		type = (lpDevCaps->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) ? 2 : 1;
		if (type > _3dtype)	{
			CopyMemory(&movie.cmiDeviceGUID, &lpDevCaps->deviceGUID, sizeof(GUID));
			_3dtype = type;
		}
	}
	_3dnum++;
    return(D3DENUMRET_OK);
	
	DEBUG_END("GlideEnum3DDeviceCallback", DebugRenderThreadId, 29);
}

/* Ez a függvény az egyes megjelenítési módok felsorolásakor hívódik meg. */
/* A megadott (beállított) felbontáshoz tartozó frekvenciákból kiválasztja az */
/* alkalmazás által megadotthoz legközelebb levõt. */
HRESULT WINAPI GlideEnumRefreshRatesCallback(LPDDSURFACEDESC2 lpDDsd, LPVOID lpContext)	{

	DEBUG_BEGIN("GlideEnumRefreshRatesCallback", DebugRenderThreadId, 30);

	if ( (lpDDsd->ddpfPixelFormat.dwRGBBitCount==movie.cmiBitPP) &&
		(lpDDsd->dwWidth==movie.cmiXres) && (lpDDsd->dwHeight==movie.cmiYres) ) {
		if ( ABS((int) lpDDsd->dwRefreshRate - (int)movie.cmiFreq) < ABS((* (int *) lpContext) - (int) movie.cmiFreq) ) * (int *) lpContext = lpDDsd->dwRefreshRate;
	}
	return(D3DENUMRET_OK);
	
	DEBUG_END("GlideEnumRefreshRatesCallback", DebugRenderThreadId, 30);
}

/*-------------------------------------------------------------------------------------------*/
/*HRESULT WINAPI GlideAlhpaChannelCallback(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDDSURFACEDESC2 lpDDSurfaceDesc,  LPVOID lpContext)	{
	return(DDENUMRET_OK);
}

int CreateAlphaChannel()	{
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpDDAlpha;
LPDDPIXELFORMAT lpPf;

	ZeroMemory(&ddsd,sizeof(DDSURFACEDESC2));
	ddsd.dwSize=sizeof(DDSURFACEDESC2);
	ddsd.dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;// | DDSD_ALPHABITDEPTH;
	ddsd.dwAlphaBitDepth=16;
	ddsd.dwWidth=movie.cmiXres;
	ddsd.dwHeight=movie.cmiYres;
	ddsd.ddsCaps.dwCaps=DDSCAPS_3DDEVICE;//DDSCAPS_ALPHA | 
	ddsd.ddpfPixelFormat.dwSize=sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags=DDPF_ALPHA;
	ddsd.ddpfPixelFormat.dwAlphaBitDepth=16;
	ddsd.ddpfPixelFormat.dwRGBAlphaBitMask=0xFF00;
	_asm jkl: jmp jkl
	lpDD->lpVtbl->CreateSurface(lpDD,&ddsd,&lpDDAlpha,NULL);
	lpDDAlpha->lpVtbl->GetPixelFormat(lpDDAlpha,&ddsd.ddpfPixelFormat);
	lpPf=&ddsd.ddpfPixelFormat;
	//lpDD->lpVtbl->EnumSurfaces(lpDD,DDENUMSURFACES_ALL,NULL,NULL,GlideAlhpaChannelCallback);
}*/


/*-------------------------------------------------------------------------------------------*/
/*.............................. Általános segédfüggvények ..................................*/

void intToStr(unsigned char *dst, int i)	{
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


/* Ez a függvény visszaadja a movie.cmiDeviceGUID mezõben a kiválasztott meghajtó GUID-ját, */
/* majd létre is hozza ezt a meghajtót. */
int GlideSelectAndCreate3DDevice()	{
HRESULT					hr;
LPDIRECTDRAWSURFACE7	lpRenderBuffer;
DWORD					initialTickCount;
	
	DEBUG_BEGIN("GlideSelectAndCreate3DDevice", DebugRenderThreadId, 31);

	_3dnum = _3dtype = 0;
	if ((hr = lpD3D->lpVtbl->EnumDevices(lpD3D, &GlideEnum3DDeviceCallback, 0)) != D3D_OK)	{

		DEBUGLOG(0,"\n   Error: GlideSelectAndCreate3DDevice failed, cannot enumerate 3D devices, error code: %0x", hr);
		DEBUGLOG(1,"\n   Hiba: GlideSelectAndCreate3DDevice hiba, nem tudom lekérdezni a 3D eszközöket, hibakód: %0x", hr);
		
		return(0);
	}
	
	/* 4 mp timeouttal puffert allokálunk */
	initialTickCount = GetTickCount ();
	while ( ((lpRenderBuffer = GABA ()) == NULL) && ((GetTickCount () - initialTickCount) < 4000) ) Sleep (1);
	
	if ((hr = lpD3D->lpVtbl->CreateDevice(lpD3D, (GUID *) &movie.cmiDeviceGUID, lpRenderBuffer, &lpD3Ddevice)) != D3D_OK)	{

		DEBUGLOG(0,"\n   Error: GlideSelectAndCreate3DDevice failed, cannot create selected 3D device, error code: %0x", hr);
		DEBUGLOG(1,"\n   Hiba: GlideSelectAndCreate3DDevice hiba, nem tudom létrehozni az adott 3D eszközt, hibakód: %0x", hr);
		
		return(0);
	}

	return(1);
	
	DEBUG_END("GlideSelectAndCreate3DDevice", DebugRenderThreadId, 31);
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

	DEBUG_BEGIN("GlideCreateFogTables", DebugRenderThreadId, 32);

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
	
	return(1);
	
	DEBUG_END("GlideCreateFogTables",DebugRenderThreadId, 32);
}


/* Eldobja a köd segédtábláit */
void GlideDestroyFogTables()	{

	DEBUG_BEGIN("GlideDestroyFogTables", DebugRenderThreadId, 33);
	
	if (WToInd)
		_FreeMem(WToInd);
	
	WToInd = NULL;
	
	DEBUG_END("GlideDestroyFogTables", DebugRenderThreadId, 33);
}


/* Az ItoW táblát egy adott helyre másolja (ezt a függvényt csak DOS-ból hívjuk meg, mert ott */
/* nem használunk semmilyen lebegõpontos aritmetikát, így ezt a táblát sem tudjuk legenerálni). */
void GlideGetIndToWTable(float *table)	{
	
	DEBUG_BEGIN("GlideGetIndToWTable", DebugRenderThreadId, 34);
	
	CopyMemory(table, IndToW, GR_FOG_TABLE_SIZE*sizeof(float));
	
	DEBUG_END("GlideGetIndToWTable", DebugRenderThreadId, 34);
}


void GlideGetD3DState(DWORD *state)	{
int i, pos;

	for(i = 0; i < 10; i++)
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice,D3DStates[i],state+i);
	pos = i;
	for(i = 0; i < 10; i++)	{
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 0, TSStates[i], state+(pos++));
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 1, TSStates[i], state+(pos++));
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 2, TSStates[i], state+(pos++));
	}
	lpD3Ddevice->lpVtbl->GetTexture(lpD3Ddevice, 0, (LPDIRECTDRAWSURFACE7 *) state + pos);
}


void GlideSetD3DState(DWORD *state)	{
int i, pos;

	for(i = 0; i < 10; i++)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DStates[i], state[i]);
	pos = i;
	for(i = 0; i < 10; i++)	{
		lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, TSStates[i], state[pos++]);
		lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, TSStates[i], state[pos++]);
		lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 2, TSStates[i], state[pos++]);
	}
	lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, 0, (LPDIRECTDRAWSURFACE7) state[pos]);
	lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, 1, (LPDIRECTDRAWSURFACE7) state[pos]);
}


void	GlideSave3DStates (DWORD *states, DWORD *saveArea)	{
int		i;

	for(i = 0; states[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, states[i], saveArea + i);
}


void	GlideSave3DStageStates (DWORD *stateDesc, DWORD *saveArea)	{
int		i;
	
	for(i = 0; stateDesc[2*i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, stateDesc[2*i], stateDesc[2*i+1], saveArea + i);
}


void	GlideSave3DTextures (DWORD *stageDesc, DWORD *texHandles)	{
int		i;

	for(i = 0; stageDesc[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->GetTexture(lpD3Ddevice, stageDesc[i], (LPDIRECTDRAWSURFACE7 *) (texHandles + i));
}


void	GlideRestore3DStates (DWORD *states, DWORD *saveArea)	{
int		i;

	for(i = 0; states[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, states[i], saveArea[i]);
}


void	GlideRestore3DStageStates (DWORD *stateDesc, DWORD *saveArea)	{
int		i;
	
	for(i = 0; stateDesc[2*i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, stateDesc[2*i], stateDesc[2*i+1], saveArea[i]);
}


void	GlideRestore3DTextures (DWORD *stageDesc, DWORD *texHandles)	{
int		i;

	for(i = 0; stageDesc[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, stageDesc[i], (LPDIRECTDRAWSURFACE7) texHandles[i]);
}


void	GlideApplyGammaCorrection()	{
DWORD			abe,absrc,abdst, abts1, abts0, abarg;
DWORD			zenable, cull, ateste;
unsigned int	percent,color;
D3DTLVERTEX		v[6];
	
	/* Ha az eszköz támogatja a gamma rampot és az be is van kapcsolva, akkor megoldva */
	if ( (lpDDGamma != NULL) || (config.Gamma == 100) ) return;	
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, &ateste);
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, &abe);
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, &absrc);
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, &abdst);
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, &zenable);
	lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, &cull);

	lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, &abts1);
	lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, &abts0);
	lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, &abarg);
	
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, D3DBLEND_DESTCOLOR);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	v[0].sx = v[0].sy = v[0].sz = 0.0f;
	v[1].sx = v[3].sx = (float) movie.cmiXres;
	v[1].sy = v[3].sy = 0.0f;
	v[1].sz = v[3].sz = 0.0f;
	v[2].sx = v[4].sx = 0.0f;
	v[2].sy = v[4].sy = (float) movie.cmiYres;
	v[2].sz = v[4].sz = 0.0f;
	v[5].sx = (float) movie.cmiXres;
	v[5].sy = (float) movie.cmiYres;
	v[5].sz = 0.0f;

	percent = 100;
	
	while(percent != config.Gamma)	{
		color = (config.Gamma*100)/percent;		
		if (color < 100)	{
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO);
			percent = config.Gamma;
		}
		else	{
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
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
		v[0].color = v[1].color = v[2].color = v[3].color = v[4].color = v[5].color = color;
		lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice, D3DPT_TRIANGLELIST,
											D3DFVF_TLVERTEX, v, 6, 0);
	}
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, abe);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, absrc);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, abdst);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZENABLE, zenable);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, cull);

	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 1, D3DTSS_COLOROP, abts1);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLOROP, abts0);
	lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, 0, D3DTSS_COLORARG1, abarg);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, ateste);
}


/* Ez a függvény csak a naplózó verzióban szerepel: kiírja a meghajtó képességeit. */
void GlideWriteOutCaps()	{
D3DDEVICEDESC7 devdesc;
DDCAPS ddcaps, helcaps;

	ZeroMemory(&ddcaps, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);
	CopyMemory(&helcaps, &ddcaps, sizeof(ddcaps));
	lpDD->lpVtbl->GetCaps(lpDD, &ddcaps, &helcaps);
	DEBUGLOG(2,"\n\n------------------------- dgVoodoo: DirectDraw caps ----------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n\
----------------------- dgVoodoo: DirectDraw HEL caps --------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n",
				ddcaps.dwCaps,
				ddcaps.dwCaps2,
				ddcaps.dwCKeyCaps,
				ddcaps.dwFXCaps,
				helcaps.dwCaps,
				helcaps.dwCaps2,
				helcaps.dwCKeyCaps,
				helcaps.dwFXCaps);

	lpD3Ddevice->lpVtbl->GetCaps(lpD3Ddevice, &devdesc);
	DEBUGLOG(2,"\n------------------------- dgVoodoo: driver description -------------------------\n \
				dwDevCaps: %x\n\
			dpcLineCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
			dpcTriCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
				dwDeviceRenderBitDepth: %0x\n\
				dwDeviceZBufferBitDepth: %0x\n\
				dwMinTextureWidth: %0x\n\
				dwMinTextureHeight: %0x\n\
				dwMaxTextureWidth: %0x\n\
				dwMaxTextureHeight: %0x\n\
				dwMaxTextureRepeat: %0x\n\
				dwMaxTextureAspectRatio: %0x\n\
				dwMaxAnisotropy: %0x\n\
				dvGuardBandLeft: %f\n\
				dvGuardBandTop: %f\n\
				dvGuardBandRight: %f\n\
				dvGuardBandBottom: %f\n\
				dvExtentsAdjust: %f\n\
				dwStencilCaps: %0x\n\
				dwFVFCaps: %0x\n\
				dwTextureOpCaps: %0x\n\
				wMaxTextureBlendStages: %0x\n\
				wMaxSimultaneousTextures: %0x\n\
				dwMaxActiveLights: %0x\n\
				dvMaxVertexW: %f\n\
				wMaxUserClipPlanes: %0x\n\
				wMaxVertexBlendMatrices: %0x\n\
				dwVertexProcessingCaps: %0x\n", \
				devdesc.dwDevCaps, \
				devdesc.dpcLineCaps.dwMiscCaps, \
				devdesc.dpcLineCaps.dwRasterCaps, \
				devdesc.dpcLineCaps.dwZCmpCaps, \
				devdesc.dpcLineCaps.dwSrcBlendCaps, \
				devdesc.dpcLineCaps.dwDestBlendCaps, \
				devdesc.dpcLineCaps.dwAlphaCmpCaps, \
				devdesc.dpcLineCaps.dwShadeCaps, \
				devdesc.dpcLineCaps.dwTextureCaps, \
				devdesc.dpcLineCaps.dwTextureFilterCaps, \
				devdesc.dpcLineCaps.dwTextureBlendCaps, \
				devdesc.dpcLineCaps.dwTextureAddressCaps, \
				devdesc.dpcLineCaps.dwStippleWidth, \
				devdesc.dpcLineCaps.dwStippleHeight, \
				devdesc.dpcTriCaps.dwMiscCaps, \
				devdesc.dpcTriCaps.dwRasterCaps, \
				devdesc.dpcTriCaps.dwZCmpCaps, \
				devdesc.dpcTriCaps.dwSrcBlendCaps, \
				devdesc.dpcTriCaps.dwDestBlendCaps, \
				devdesc.dpcTriCaps.dwAlphaCmpCaps, \
				devdesc.dpcTriCaps.dwShadeCaps, \
				devdesc.dpcTriCaps.dwTextureCaps, \
				devdesc.dpcTriCaps.dwTextureFilterCaps, \
				devdesc.dpcTriCaps.dwTextureBlendCaps, \
				devdesc.dpcTriCaps.dwTextureAddressCaps, \
				devdesc.dpcTriCaps.dwStippleWidth, \
				devdesc.dpcTriCaps.dwStippleHeight, \
				devdesc.dwDeviceRenderBitDepth, \
				devdesc.dwDeviceZBufferBitDepth, \
				devdesc.dwMinTextureWidth, \
				devdesc.dwMinTextureHeight, \
				devdesc.dwMaxTextureWidth, \
				devdesc.dwMaxTextureHeight, \
				devdesc.dwMaxTextureRepeat, \
				devdesc.dwMaxTextureAspectRatio, \
				devdesc.dwMaxAnisotropy, \
				devdesc.dvGuardBandLeft, \
				devdesc.dvGuardBandTop, \
				devdesc.dvGuardBandRight, \
				devdesc.dvGuardBandBottom, \
				devdesc.dvExtentsAdjust, \
				devdesc.dwStencilCaps, \
				devdesc.dwFVFCaps, \
				devdesc.dwTextureOpCaps, \
				devdesc.wMaxTextureBlendStages, \
				devdesc.wMaxSimultaneousTextures, \
				devdesc.dwMaxActiveLights, \
				devdesc.dvMaxVertexW, \
				devdesc.wMaxUserClipPlanes, \
				devdesc.wMaxVertexBlendMatrices, \
				devdesc.dwVertexProcessingCaps);
}


/*-------------------------------------------------------------------------------------------*/
/* Ez a függvény kinullázza az összes rajzbuffert. */
void GlideNullSurfaces()	{
DDSURFACEDESC2			ddsd;
LPDIRECTDRAWSURFACE7	lpdds[3];
LPDIRECTDRAWSURFACE7	lpDDSurface;
unsigned int			i,j;
char					*p;

	DEBUG_BEGIN("GlideNullSurfaces", DebugRenderThreadId, 35);
	
	lpdds[0] = GABA();
	lpdds[1] = GetFrontBuffer();
	lpdds[2] = GetThirdBuffer();
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	for(i = 0; i < 3; i++)	{
		lpDDSurface = lpdds[i];
		if (lpDDSurface != NULL)	{
			lpDDSurface->lpVtbl->GetSurfaceDesc(lpDDSurface, &ddsd);
			if (lpDDSurface->lpVtbl->Lock(lpDDSurface, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL) == DD_OK)	{
				p = ddsd.lpSurface;
				for(j = 0; j < movie.cmiYres; j++)	{
					ZeroMemory(p, movie.cmiXres*movie.cmiByPP);
					p += ddsd.lPitch;
				}
				lpDDSurface->lpVtbl->Unlock(lpDDSurface, NULL);
			}
		}
	}
	
	DEBUG_END("GlideNullSurfaces", DebugRenderThreadId, 35);
}


/* A VMODE_WUNDEFINED vertex flag-et update-eli. */
void GlideSetVertexWUndefined()	{

	if ( (!(astate.vertexmode & VMODE_TEXTURE)) && (astate.zfunc == D3DCMP_ALWAYS) ) astate.vertexmode |= VMODE_WUNDEFINED;
		else astate.vertexmode &= ~VMODE_WUNDEFINED;
}


/* Ez a függvény eldönti, hogy a vertexek információi közül szükség van-e a textúrakoordinátákra */
/* és az interpolált színre (diffuse). Ennek megfelelõen állítja be a VMODE_TEXTURE és VMODE_DIFFUSE */
/* biteket a vertex átalakítását megadó állapotinfóban. */
/* Ezt két infóból dönti el: az alfa és color combine használja-e az adott komponenst, és hogy */
/* az alpha blending felhasználja-e a forrásszínt (és alfát). */
void _inline GlideSetVertexTexDiffuseMode()	{
int		col,alpha;

	astate.vertexmode &= ~(VMODE_DIFFUSE | VMODE_TEXTURE);
	if (astate.colorMask)	{
		col = alpha = 0;
		if (astate.alphaBlendEnable == TRUE)	{
			if ( (astate.dstBlend == (unsigned char) D3DBLEND_SRCCOLOR) || (astate.dstBlend == (unsigned char) D3DBLEND_INVSRCCOLOR) ||
				  (astate.dstBlend == (unsigned char) D3DBLEND_ONE) || (astate.srcBlend != (unsigned char) D3DBLEND_ZERO) )
				  col = 1;
			if ( (astate.srcBlend == (unsigned char) D3DBLEND_SRCALPHA) || (astate.srcBlend == (unsigned char) D3DBLEND_INVSRCALPHA) ||
				(astate.dstBlend == (unsigned char) D3DBLEND_SRCALPHA) || (astate.dstBlend == (unsigned char) D3DBLEND_INVSRCALPHA) ||
				(astate.srcBlend == (unsigned char) D3DBLEND_SRCALPHASAT) )
				alpha = 1;
		} else col = 1;
		
		if ( ( (astate.flags & STATE_COLORTEXTUREUSED) && col) ||
			( (astate.flags & STATE_ALPHATEXTUREUSED) && alpha ) ) astate.vertexmode |= VMODE_TEXTURE;
		if ( ( (astate.flags & STATE_COLORDIFFUSEUSED) && col) ||
			( (astate.flags & STATE_ALPHADIFFUSEUSED) && alpha ) ) astate.vertexmode |= VMODE_DIFFUSE;
	}
	GlideSetVertexWUndefined();
	/*LOG(1,"\nGlideSetVertexTexDiffuseMode: astate.flags & STATE_COLORDIFFUSEUSED: %d, col: %d, astate.flags & STATE_ALPHADIFFUSEUSED: %d, alpha: %d",
			(astate.flags & STATE_COLORDIFFUSEUSED) ? 1 : 0, col, (astate.flags & STATE_ALPHADIFFUSEUSED) ? 1 : 0, alpha);
	LOG(1,"\n astate.vertexmode=%0x",astate.vertexmode);*/
}


/* A VMODE_NODRAW vertex-flaget frissíti: ha a konstans szín a colorkey, és a colorkeying engedélyezve van, akkor  */
/* nem rajzol */
void GlideUpdateConstantColorKeyingState()	{

	astate.vertexmode &= ~VMODE_NODRAW;
	if ( (astate.colorKeyEnable) && (astate.cotherp == GR_COMBINE_OTHER_CONSTANT) && 
		((astate.constColorValue & 0x00FFFFFF) == (astate.colorkey & 0x00FFFFFF) ) )
		astate.vertexmode |= VMODE_NODRAW;
}


/* Megállapítja, hogy a color combine használja-e a textúrát és a diffuse színt. */
void GlideColorCombineUpdateVertexMode()	{
char	usage;

	DEBUG_BEGIN("GlideColorCombineUpdateVertexMode", DebugRenderThreadId, 36);
	
	astate.flags &= ~(STATE_COLORTEXTUREUSED | STATE_COLORDIFFUSEUSED);
	if ( (astate.cotherp == GR_COMBINE_OTHER_TEXTURE) && (combinefncotherusedlookup[astate.cfuncp]) )
		astate.flags |= STATE_COLORTEXTUREUSED;
	if ( (astate.cfuncp >= GR_COMBINE_FUNCTION_SCALE_OTHER) && (combinefactortexusedlookup[astate.cfactorp]) )
		astate.flags |= STATE_COLORTEXTUREUSED;
	if (texCombineFuncImp[astate.rgb_function] == TCF_ZERO)
		astate.flags &= ~STATE_COLORTEXTUREUSED;
	usage = 0;
	if (astate.clocalp == GR_COMBINE_LOCAL_ITERATED) usage = 1;
	if (astate.cotherp == GR_COMBINE_OTHER_ITERATED) usage |= 2;
	if (astate.alocalp == GR_COMBINE_LOCAL_ITERATED) usage |= 4;
	if (astate.aotherp == GR_COMBINE_OTHER_ITERATED) usage |= 16;
	if (combinefactorwhichparam[astate.cfactorp]==1) usage |= (usage & 1) << 3;
	if (combinefactorwhichparam[astate.cfactorp]==2) usage |= (usage & 16) >> 1;
	if (combinefactorwhichparam[astate.cfactorp]==4) usage |= (usage & 4) << 1;
	if (combinefncwhichparams[astate.cfuncp] & usage) astate.flags |= STATE_COLORDIFFUSEUSED;
	LOG(1,"\n\tcolor, usage=%d, combinefncwhichparams[astate.cfuncp]=%d", usage,combinefncwhichparams[astate.cfuncp]);
	GlideSetVertexTexDiffuseMode();
	
	DEBUG_END("GlideColorCombineUpdateVertexMode", DebugRenderThreadId, 36);
}


/* Megállapítja, hogy az alpha combine használja-e a textúrát és a diffuse színt. */
void GlideAlphaCombineUpdateVertexMode()	{
char	usage;

	astate.flags &= ~(STATE_ALPHATEXTUREUSED | STATE_ALPHADIFFUSEUSED);
	if ( (astate.aotherp == GR_COMBINE_OTHER_TEXTURE) && (combinefncotherusedlookup[astate.afuncp]) )
		astate.flags |= STATE_ALPHATEXTUREUSED;
	if ( (astate.afuncp >= GR_COMBINE_FUNCTION_SCALE_OTHER) && (combinefactortexusedlookup[astate.afactorp]) )
		astate.flags |= STATE_ALPHATEXTUREUSED;
	if (texCombineFuncImp[astate.alpha_function] == TCF_ZERO)
		astate.flags &= ~STATE_ALPHATEXTUREUSED;
	usage = 0;
	if (astate.alocalp == GR_COMBINE_LOCAL_ITERATED) usage = 1 | 4;
	if (astate.aotherp == GR_COMBINE_OTHER_ITERATED) usage |= 2;
	if ( (combinefactorwhichparam[astate.afactorp] == 1) || (combinefactorwhichparam[astate.afactorp] == 4) )
		usage |= (usage & 1) << 3;
	if (combinefactorwhichparam[astate.afactorp] == 2)
		usage |= (usage & 2) << 2;
	if (combinefncwhichparams[astate.afuncp] & usage)
		astate.flags |= STATE_ALPHADIFFUSEUSED;
	LOG(1,"\n\talpha, usage=%d, combinefncwhichparams[astate.afuncp]=%d",usage,combinefncwhichparams[astate.afuncp]);
}


void _inline GlideSetUseTmuWMode()	{
	
	if ( (astate.hints & GR_STWHINT_W_DIFF_TMU0) && ( (astate.fogmode != GR_FOG_WITH_TABLE) || ((config.Flags2 & CFG_PREFERTMUWIFPOSSIBLE)) ) )
	  astate.flags |= STATE_USETMUW;
	else astate.flags &= ~STATE_USETMUW;

}


void GlideUpdateTextureUsageFlags()	{
int				i, j, textureUsage, flags;
unsigned int	oldUsageMask;

	j = MAX(astate.astagesnum, astate.cstagesnum);
	oldUsageMask = astate.flags & STATE_TEXTUREUSAGEMASK;
	flags = astate.flags & ~STATE_TEXTUREUSAGEMASK;
	textureUsage = STATE_TEXTURE0;
	for(i = 0; i < j; i++)	{
		if ( (astate.alphaArg1[i] == D3DTA_TEXTURE) || (astate.alphaArg2[i] == D3DTA_TEXTURE) ||
			 (astate.colorArg1[i] == D3DTA_TEXTURE) || (astate.colorArg2[i] == D3DTA_TEXTURE) )
			 flags |= textureUsage;
		textureUsage <<= 1;
	}
	astate.flags = flags;
	if ((astate.flags & STATE_TEXTUREUSAGEMASK) != oldUsageMask) callflags |= CALLFLAG_RESETTEXTURE;
}


/* Egy pixel shaderben megadott argumentumot átalakít D3DTA formátumra.  */
/* Figyelembe veszi, ha a texture combine egység a kimenetet invertálja. */
DWORD _inline _fastcall GlideGetStageArg(DWORD type, int texinvert)	{
register DWORD	modifier = 0;
	
	if (type & PS_INVERT)
		modifier ^= D3DTA_COMPLEMENT;
	
	if (type & PS_ALPHA)
		modifier |= D3DTA_ALPHAREPLICATE;
	
	switch(type & (~(PS_ALPHA | PS_INVERT)) )	{
		case PS_LOCAL:
			return(d3dlocal ^ modifier);
		case PS_OTHER:
			if ( texinvert && ((d3dother & D3DTA_SELECTMASK) == D3DTA_TEXTURE) )
				modifier ^= D3DTA_COMPLEMENT;
			return(d3dother ^ modifier);
		case PS_FACTOR:
			return(d3dfactor ^ modifier);
		default:
			return(type ^ modifier);
	}
}


/*-------------------------------------------------------------------------------------------*/
/*.................................... Glide-függvények .....................................*/


void EXPORT grGlideGetVersion(char version[80])	{
int	i = 0;

	DEBUG_BEGIN("grGlideGetVersion", DebugRenderThreadId, 37);
	
	LOG(0,"\ngrGlideGetVersion(versionbuff=%p)", version);	
	while(version[i] = GLIDEVERSIONSTRING[i]) i++;
	
	DEBUG_END("grGlideGetVersion", DebugRenderThreadId, 37);
}


FxBool EXPORT grSstQueryBoards(GrHwConfiguration *hwConfig)	{

	DEBUG_BEGIN("grSstQueryBoards", DebugRenderThreadId, 38);

	LOG(0,"\ngrSstQueryBoards(hwConfig=%p)",hwConfig);
	ZeroMemory(hwConfig, sizeof(GrHwConfiguration));
	hwConfig->num_sst = 1;
	return(FXTRUE);
	
	DEBUG_END("grSstQueryBoards", DebugRenderThreadId, 38);
}


FxBool EXPORT grSstQueryHardware(GrHwConfiguration *hwConfig)	{
	
	DEBUG_BEGIN("grSstQueryHardware", DebugRenderThreadId, 39);
	
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
	return(FXTRUE);
	
	DEBUG_END("grSstQueryHardware", DebugRenderThreadId, 39);
}


void EXPORT grGlideInit()	{

	DEBUG_GLOBALTIMEBEGIN;
	
	DEBUG_BEGIN("grGlideInit", DebugRenderThreadId, 40);
 	
	LOG(0,"\ngrGlideInit()");
	TexInitAtGlideInit();
	
	DEBUG_END("grGlideInit", DebugRenderThreadId, 40);
}


void EXPORT grSstSelect(int whichSST)	{
	
	LOG(0,"\ngrSstSelect(whichSST=%d)",whichSST);	
}


void EXPORT grBufferClear(GrColor_t color, GrAlpha_t alpha, FxU16 depth)	{
DWORD				flags;
GrOriginLocation_t	origin;
D3DVALUE			z;
D3DRECT				d3drect;

	DEBUG_BEGIN("grBufferClear", DebugRenderThreadId, 41);

	LOG(0,"\ngrBufferClear(color=%0x, alpha=%0x, depth=%0x)",color,alpha,depth);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	GlideLFBCheckLock(renderbuffer);
	DrawFlushPrimitives(0);
	color = (GlideGetColor(color) & 0x00FFFFFF) | (((unsigned int) alpha) << 24);
	flags = (astate.colorMask == FXTRUE) ? D3DCLEAR_TARGET : 0;
	if (astate.zenable == FXTRUE)	{
		flags |= D3DCLEAR_ZBUFFER;
		switch(depthbuffering)	{
			case DBUFFER_Z:	
				z = DrawGetFloatFromDepthValue (depth, DVLinearZ);
				break;
			case DBUFFER_W:
				z = (astate.vertexmode & VMODE_WBUFFEMU) ? DrawGetFloatFromDepthValue (depth, DVNonlinearZ) : 
														   DrawGetFloatFromDepthValue (depth, DVTrueW) / 65528.0f;
				break;
			default:;
		}
	}
	if (movie.cmiStencilBitDepth) flags |= D3DCLEAR_STENCIL;
	d3drect.x1 = (LONG) (astate.minx * xScale);
	d3drect.x2 = (LONG) (astate.maxx * xScale);
	origin = (config.Flags2 & CFG_YMIRROR) ? ((astate.locOrig == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT) : astate.locOrig;
	if (origin == GR_ORIGIN_LOWER_LEFT)	{
		d3drect.y1 = (LONG) (movie.cmiYres + (((float) astate.maxy) * yScale));
		d3drect.y2 = (LONG) (movie.cmiYres + (((float) astate.miny) * yScale));
	} else {
		d3drect.y1 = (LONG) (((float) astate.miny) * yScale);
		d3drect.y2 = (LONG) (((float) astate.maxy) * yScale);
	}
	lpD3Ddevice->lpVtbl->Clear(lpD3Ddevice, 1, &d3drect, flags, color, z, 0);

	if (renderbuffer == GR_BUFFER_FRONTBUFFER)	{
		lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
		lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);
	}
	LfbSetBuffDirty(renderbuffer);
	
	DEBUG_END("grBufferClear", DebugRenderThreadId, 41);
}


void EXPORT grBufferSwap(int swapinterval)	{
LPDIRECTDRAWSURFACE7	temp = NULL;
int						vbbool,i,x;
	
	DEBUG_BEGIN("grBufferSwap", DebugRenderThreadId, 42);
	
	LOG(1,"\n---- grBufferSwap(swapinterval=%d) ----", swapinterval);
	if (!(runflags & RF_INITOK)) return;
	if (runflags & RF_SCENECANBEDRAWN)	{
		GlideLFBBeforeSwap();		
		DrawFlushPrimitives(0);
		GlideApplyGammaCorrection();
		GrabberDrawLabel(1);
		lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
	}
	if (lpDD->lpVtbl->TestCooperativeLevel(lpDD))	{		
		runflags &= ~RF_SCENECANBEDRAWN;
		return;
	}
	lpDDFrontBuff->lpVtbl->Restore(lpDDFrontBuff);
	if (!(runflags & RF_SCENECANBEDRAWN))	{
		GlideTexSetTexturesAsLost();
		GlideNullSurfaces();
	}
	runflags |= RF_SCENECANBEDRAWN;		
	EndDraw();

	if ((config.Flags & CFG_VSYNC) && (swapinterval == 0)) swapinterval = 1;
	if (swapinterval == 0) CallConsumer(0);
	else {
		/* ha a tényleges frissítési freq-hoz igazítjuk magunkat */
		if (!(config.Flags & CFG_SETREFRESHRATE))	{
			
			/* ha full screen-en futunk, akkor csak eggyel kevesebb visszafutást várunk meg */
			if (!(config.Flags & CFG_WINDOWED)) swapinterval--;
			
			/* megvárjuk a visszafutásokat */
			for(i = 0; i < swapinterval; i++)	{
				while(1)	{
					lpDD->lpVtbl->GetVerticalBlankStatus(lpDD, &vbbool);
					if (!vbbool) break;
				}
				while(1)	{
					lpDD->lpVtbl->GetVerticalBlankStatus(lpDD, &vbbool);
					if (vbbool) break;
				}
			}
			x = GetBufferNumPending();
			CallConsumer(1);
			if (x == GetBufferNumPending()) return;

		} else {
		/* ha a timer-hez igazítjuk magunkat */
			movie.cmiFrames = 0;
			while((volatile) movie.cmiFrames < (swapinterval - 1));
		}
	}
	
	while (temp == NULL) temp = BeginDraw();
	buffswapnum++;
	lpDDBackBuff = temp;
	lpDDFrontBuff = GetFrontBuffer();	

	if (renderbuffer == GR_BUFFER_FRONTBUFFER) temp = lpDDFrontBuff;
	lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, temp, 0);	
	
	if (lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice) != DD_OK) runflags &= ~RF_SCENECANBEDRAWN;
		else runflags |= RF_SCENECANBEDRAWN;
	GrabberCleanUnvisibleBuff();
	GlideLFBAfterSwap();
	
	DEBUG_END("grBufferSwap", DebugRenderThreadId, 42);
}


int EXPORT grBufferNumPending( void )	{

	return(GetBufferNumPending());

}


void EXPORT grConstantColorValue( GrColor_t color )	{
DWORD temp;

	DEBUG_BEGIN("grConstantColorValue", DebugRenderThreadId, 43);

	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (temp = GlideGetColor(color)) == astate.constColorValue) return;
	LOG(0,"\ngrConstantColorValue(color=%0x), in ARGB: %0x",color,temp);
	DrawFlushPrimitives(0);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, 
										D3DRENDERSTATE_TEXTUREFACTOR,
										(astate.constColorValue = temp));
	GlideUpdateConstantColorKeyingState();
	
	DEBUG_END("grConstantColorValue", DebugRenderThreadId, 43);
}


void EXPORT grConstantColorValue4(float a, float r, float g, float b)	{
	
//	return;
	DEBUG_BEGIN("grConstantColorValue4",DebugRenderThreadId, 44);

	LOG(0,"\ngrConstantColorValue4(a=%f, r=%f, g=%f, b=%f)",a,r,g,b);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	astate.delta0Rgb =	(((unsigned long) ((unsigned char) (a*(255.0f/256.0f)))) <<24) +
						(((unsigned long) ((unsigned char) (r*(255.0f/256.0f)))) <<16) +
						(((unsigned long) ((unsigned char) (g*(255.0f/256.0f)))) <<8) +
						((unsigned long) ((unsigned char) (b*(255.0f/256.0f))));
	
	DEBUG_END("grConstantColorValue4",DebugRenderThreadId, 44);
}


void EXPORT grColorMask( FxBool rgb, FxBool alpha )	{
DWORD abe;

//	return;
	DEBUG_BEGIN("grColorMask", DebugRenderThreadId, 45);
	
	LOG(1,"\ngrColorMask(rgb=%s,alpha=%s)",fxbool_t[rgb],fxbool_t[alpha]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;	
	if (astate.colorMask == (DWORD) rgb) return;
	DrawFlushPrimitives(0);
	if ( astate.colorMask = rgb )	{
			abe = (DWORD) astate.alphaBlendEnable;
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, (DWORD) astate.alphaBlendEnable);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
												D3DRENDERSTATE_SRCBLEND, (DWORD) astate.srcBlend);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
												D3DRENDERSTATE_DESTBLEND, (DWORD) astate.dstBlend);
	} else {
			abe = TRUE;
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, 
												D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
												D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	}
	GlideSetVertexTexDiffuseMode();
	
	DEBUG_END("grColorMask",DebugRenderThreadId, 45);
}


void EXPORT grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy ) {
D3DVIEWPORT7		viewport;
GrOriginLocation_t	origin;

//	return;
	DEBUG_BEGIN("grClipWindow",DebugRenderThreadId, 46);

	LOG(0,"\ngrClipWindow(minx=%d, miny=%d, maxx=%d, maxy=%d)",minx,miny,maxx,maxy);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	DrawFlushPrimitives(0);
	origin = (config.Flags2 & CFG_YMIRROR) ? ((astate.locOrig == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT) : astate.locOrig;
	astate.minx = minx;
	astate.miny = miny;
	astate.maxx = maxx;
	astate.maxy = maxy;	
	viewport.dwX = (DWORD) (minx * xScale);
	viewport.dwWidth = (DWORD) ((maxx - minx) * xScale);
	if (origin == GR_ORIGIN_LOWER_LEFT)	{
		viewport.dwY = (DWORD) (movie.cmiYres + (((float) maxy) * yScale));
		viewport.dwHeight = (DWORD) ((maxy - miny) * (-yScale));
	} else {
		viewport.dwY = (DWORD) (((float) miny) * yScale);
		viewport.dwHeight = (DWORD) ((maxy - miny) * yScale);
	}
	viewport.dvMinZ = 0.0f;
	viewport.dvMaxZ = 1.0f;
	lpD3Ddevice->lpVtbl->SetViewport(lpD3Ddevice, &viewport);
	
	DEBUG_END("grClipWindow",DebugRenderThreadId, 46);
}


void EXPORT grGlideGetState( _GrState *state ) {

	//return;
	DEBUG_BEGIN("grGlideGetState",DebugRenderThreadId, 47);

	LOG(0,"\ngrGlideGetState(*state=%p)",state);
	DrawFlushPrimitives(0);
	
	/* A megfelelõ állapotok frissítése mentés elõtt */
	if (callflags & CALLFLAG_ALPHACOMBINE) GlideAlphaCombine();
	if (callflags & CALLFLAG_COLORCOMBINE) GlideColorCombine();
	if (callflags & CALLFLAG_SETTEXTURE) GlideTexSource();
	if (callflags & CALLFLAG_RESETTEXTURE) TexSetCurrentTexture(astate.acttex);
	if (callflags & CALLFLAG_SETCOLORKEYSTATE) TexUpdateNativeColorKeyState();
	if (callflags & CALLFLAG_SETPALETTE) TexUpdateTexturePalette();
	if (callflags & CALLFLAG_SETNCCTABLE) TexUpdateTextureNccTable();
	if (callflags & CALLFLAG_SETCOLORKEY) TexUpdateAlphaBasedColorKeyState();
	
	CopyMemory(state, &astate, sizeof(_GrState));
	return;
	
	DEBUG_END("grGlideGetState",DebugRenderThreadId, 47);
}


void EXPORT grGlideSetState( _GrState *state ) {
int	i;

	//return;
	DEBUG_BEGIN("grGlideSetState",DebugRenderThreadId, 48);

	LOG(0,"\ngrGlideSetState(*state=%p)",state);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;	
	DrawFlushPrimitives(0);
	for(i = 0; i < MAX_TEXSTAGES; i++)	{
		if (state->colorOp[i] != astate.colorOp[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLOROP,
														state->colorOp[i]);
		if (state->colorArg1[i] != astate.colorArg1[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLORARG1,
														state->colorArg1[i]);
		if ( (state->colorArg2[i] != astate.colorArg2[i]) && (i != 2) )
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLORARG2,
														state->colorArg2[i]);

		if (state->alphaOp[i] != astate.alphaOp[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP,
														state->alphaOp[i]);
		if (state->alphaArg1[i] != astate.alphaArg1[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG1,
														state->alphaArg1[i]);
		if ( (state->alphaArg2[i] != astate.alphaArg2[i]) && (i != 2) )
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG2,
														state->alphaArg2[i]);
	}	
	
	for(i = 0; i < 3; i++)
		if (state->lpDDTex[i] != astate.lpDDTex[i]) lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice, i, state->lpDDTex[i]);

	if (state->constColorValue != astate.constColorValue) 
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_TEXTUREFACTOR, state->constColorValue);

	if (state->magfilter != astate.magfilter)	{
		for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MAGFILTER, state->magfilter);
	}
	if (state->minfilter != astate.minfilter) {
		for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MINFILTER, state->minfilter);
	}
	if (state->clampmodeu != astate.clampmodeu)	{
		for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ADDRESSU, state->clampmodeu);
	}
	if (state->clampmodev != astate.clampmodev)	{
		for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ADDRESSV, state->clampmodev);
	}
	for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MIPMAPLODBIAS, *( (LPDWORD) (&state->lodBias)) );
	if (state->mipMapMode != astate.mipMapMode)	{
		for(i = 0; i < 3; i++) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_MIPFILTER, state->mipMapMode);
	}
	if (state->zfunc != astate.zfunc)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZFUNC, state->zfunc);
	if (state->zenable != astate.zenable)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ZWRITEENABLE, state->zenable);
	if (state->cullmode != astate.cullmode)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, state->cullmode);

	if (state->alphaBlendEnable != astate.alphaBlendEnable)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, (DWORD) state->alphaBlendEnable);
	if (state->srcBlend != astate.srcBlend)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, (DWORD) state->srcBlend);
	if (state->dstBlend != astate.dstBlend)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, (DWORD) state->dstBlend);

	if (state->alphatestfunc != astate.alphatestfunc)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAFUNC, state->alphatestfunc);
	if (state->alpharef != astate.alpharef)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAREF, state->alpharef);
	/* itt kihasználtuk, hogy a GR_FOG_DISABLE == 0 ! */
	if (state->fogmode != astate.fogmode)	{
		i = (state->fogmode == GR_FOG_DISABLE) ? FALSE : TRUE;
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGENABLE, i);
	}
	if (state->fogcolor != astate.fogcolor)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGCOLOR, state->fogcolor);
	
	if (astate.fogmodifier & GR_FOG_ADD2)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGCOLOR, 0x0);

	if ( (state->minx != astate.minx) || (state->maxx != astate.maxx) ||
		 (state->miny != astate.miny) || (state->maxy != astate.maxy) )
			grClipWindow(state->minx, state->miny, state->maxx, state->maxy);
	if (state->colorMask != astate.colorMask)
		grColorMask(state->colorMask, 0);
	if (state->perspective != astate.perspective)		
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
											D3DRENDERSTATE_TEXTUREPERSPECTIVE,
											astate.perspective = state->perspective);
	if (state->locOrig != astate.locOrig) DrawComputeScaleCoEffs(state->locOrig);
	
	CopyMemory(&astate, state, sizeof(_GrState));	
	grDepthBiasLevel( (FxI16) astate.depthbiaslevel);
	return;
	
	DEBUG_END("grGlideSetState",DebugRenderThreadId, 48);
}


void EXPORT grAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df,
								 GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df )	{
DWORD	abe, asrc, adst;

	DEBUG_BEGIN("grAlphaBlendFunction", DebugRenderThreadId, 49);
	
	LOG(1,"\nAlphaBlendFunction(rgb_sf=%s, rgb_df=%s, alpha_sf=%s, alpha_df=%s",
		alphabfnc_srct[rgb_sf], alphabfnc_dstt[rgb_df],
		alphabfnc_srct[alpha_sf], alphabfnc_dstt[alpha_df]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	abe = ( (rgb_sf == GR_BLEND_ONE) && (rgb_df == GR_BLEND_ZERO) ) ? FALSE : TRUE;
	DrawFlushPrimitives(0);
	if (astate.colorMask)	{
		if (abe != (DWORD) astate.alphaBlendEnable)
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, (DWORD) (astate.alphaBlendEnable = (unsigned char) abe));
		if (abe)	{
			if ((asrc = alphamodelookupsrc[rgb_sf]) != (DWORD) astate.srcBlend)
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, 
													D3DRENDERSTATE_SRCBLEND, (DWORD) (astate.srcBlend = (unsigned char) asrc));
			if ((adst = alphamodelookupdst[rgb_df]) != (DWORD) astate.dstBlend)
				lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
													D3DRENDERSTATE_DESTBLEND, (DWORD) (astate.dstBlend = (unsigned char) adst));
		}
	}
	GlideSetVertexTexDiffuseMode();
	callflags |= (astate.flags & STATE_NATIVECKMETHOD) ? CALLFLAG_SETCOLORKEYSTATE : CALLFLAG_SETCOLORKEY;
	astate.flags |= STATE_ALPHABLENDCHANGED;

	DEBUG_END("grAlphaBlendFunction", DebugRenderThreadId, 49);
}


void EXPORT grColorCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert )	{
		
	DEBUG_BEGIN("grColorCombine", DebugRenderThreadId, 50);

	LOG(1,"\ngrColorCombine(func=%s, factor=%s, local=%s, other=%s, invert=%s",
		combfunc_t[func], combfact_t[factor], local_t[local], other_t[other], fxbool_t[invert]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( ( (unsigned char) func == astate.cfuncp) && ((unsigned char) factor == astate.cfactorp)
		&& ((unsigned char) local == astate.clocalp) && ((unsigned char) other == astate.cotherp)
		&& ((unsigned char) invert == astate.cinvertp) ) return;
	
	DrawFlushPrimitives(0);
	LOG(1,"\n\tastate.flags=%0x",astate.flags);
	astate.cfuncp = (unsigned char) func;
	astate.cfactorp = (unsigned char) factor;
	astate.clocalp = (unsigned char) local;
	astate.cotherp = (unsigned char) other;
	astate.cinvertp = (unsigned char) invert;
	GlideColorCombineUpdateVertexMode();
	GlideUpdateConstantColorKeyingState();
	callflags |= CALLFLAG_COLORCOMBINE | ((astate.flags & STATE_NATIVECKMETHOD) ? CALLFLAG_SETCOLORKEYSTATE : CALLFLAG_SETCOLORKEY);
	
	DEBUG_END("grColorCombine", DebugRenderThreadId, 50);
}


void		GlideColorCombine()	{
DWORD				colorOp[4];
DWORD				colorArg1[4];
DWORD				colorArg2[4];
CPixelShader3Stages *setting;
DWORD				rpersp;
unsigned int		i,j,cfuncpindex;

//return;
	DEBUG_BEGIN("GlideColorCombine", DebugRenderThreadId, 51);

	d3dlocal = combinelocallookup[astate.clocalp];
	d3dother = combineotherlookup[astate.cotherp];
	if ( (d3dother == D3DTA_TEXTURE) && (astate.flags & STATE_TCLOCALALPHA) ) d3dother |= D3DTA_ALPHAREPLICATE;

	setting = pixelShadersfa;
	switch(astate.cfactorp)	{		
		case GR_COMBINE_FACTOR_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_RGB:
			setting = pixelShadersfloc;
			d3dfactor = D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
			break;
		
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = astate.aother;
			break;

		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
			d3dfactor = astate.alocal;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
			
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
			d3dfactor = astate.alocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = astate.aother | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ZERO:
			setting = pixelShadersf0;
			break;

		case GR_COMBINE_FACTOR_ONE:
			setting = pixelShadersf1;
			break;

		default:							/* ha nem valamelyik konstans */
			setting = pixelShadersf0;
	}
	
	cfuncpindex = grfunctranslating[astate.cfuncp];

	/* Argumentumok és opkód kiolvasása a shaderbõl */
	astate.cstagesnum = 1;
	i = astate.flags & STATE_TCCINVERT;
	colorOp[0] = setting[cfuncpindex].colorop_0;
	colorArg1[0] = GlideGetStageArg(setting[cfuncpindex].colorarg1_0, i);
	colorArg2[0] = GlideGetStageArg(setting[cfuncpindex].colorarg2_0, i);
	if ( (colorOp[1] = setting[cfuncpindex].colorop_1) != D3DTOP_DISABLE )	{
		astate.cstagesnum = 2;
		colorArg1[1] = GlideGetStageArg(setting[cfuncpindex].colorarg1_1, i);
		colorArg2[1] = GlideGetStageArg(setting[cfuncpindex].colorarg2_1, i);
		if ( (colorOp[2] = setting[cfuncpindex].colorop_2) != D3DTOP_DISABLE )	{
			astate.cstagesnum = 3;
			colorArg1[2] = GlideGetStageArg(setting[cfuncpindex].colorarg1_2, i);
			colorArg2[2] = GlideGetStageArg(setting[cfuncpindex].colorarg2_2, i);
		}
	}
		
	/* A speciális opkódok feldolgozása (implicit argumentumokkal) */
	/* Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ*/
	switch(colorOp[1])	{		
		case PS_BLENDFACTORALPHA:
			colorOp[1] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = colorArg1[1];
				colorArg1[1] = colorArg2[1];
				colorArg2[1] = i;
			}
			break;
	}	

	/*colorOp[0]=D3DTOP_SELECTARG1;
	colorArg1[0]=D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
	colorOp[1]=D3DTOP_DISABLE;*/
	
	if (astate.cinvertp)	{
		colorOp[astate.cstagesnum] = D3DTOP_SELECTARG2;
		colorArg1[astate.cstagesnum] = D3DTA_CURRENT;
		colorArg2[astate.cstagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		astate.cstagesnum++;
	}
	
	for(j = 0; j < astate.cstagesnum; j++)	{
		if (astate.colorOp[j] != colorOp[j])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, j, D3DTSS_COLOROP, astate.colorOp[j] = colorOp[j]);
		if (astate.colorArg1[j] != colorArg1[j])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, j, D3DTSS_COLORARG1, astate.colorArg1[j] = colorArg1[j]);
		if (astate.colorArg2[j] != colorArg2[j])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, j, D3DTSS_COLORARG2, astate.colorArg2[j] = colorArg2[j]);
	}

	for(i = astate.cstagesnum; i < astate.astagesnum; i++)	{
		if (astate.colorOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLOROP, astate.colorOp[i] = D3DTOP_SELECTARG1);
		if (astate.colorArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLORARG1, astate.colorArg1[i] = D3DTA_CURRENT);
	}
	if (i != MAX_TEXSTAGES)
		if (astate.colorOp[i] != D3DTOP_DISABLE) lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_COLOROP, astate.colorOp[i] = D3DTOP_DISABLE);
	
	if ( ( (astate.flags & STATE_COLORTEXTUREUSED) || (astate.flags & STATE_ALPHATEXTUREUSED) ) ||
		( (depthbuffering == DBUFFER_W) && (!(astate.vertexmode & VMODE_WBUFFEMU))) ) rpersp = TRUE;
		else rpersp = FALSE;
	if (rpersp != astate.perspective)
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,
											D3DRENDERSTATE_TEXTUREPERSPECTIVE,
											astate.perspective = rpersp);
	if (!(astate.flags & STATE_NATIVECKMETHOD))	{
		callflags |= CALLFLAG_SETCOLORKEY;
		astate.flags |= STATE_CCACSTATECHANGED;
	}
	callflags &= ~CALLFLAG_COLORCOMBINE;
	GlideUpdateTextureUsageFlags();

	DEBUG_END("GlideColorCombine", DebugRenderThreadId, 51);
}


void EXPORT grAlphaCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert)	{
		
	DEBUG_BEGIN("grAlphaCombine", DebugRenderThreadId, 159);
	
	LOG(1,"\ngrAlphaCombine(func=%s, factor=%s, local=%s, other=%s, invert=%s",
		combfunc_t[func], combfact_t[factor], local_t[local], other_t[other], fxbool_t[invert]);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (func == astate.afuncp) && (factor == astate.afactorp) && (local == astate.alocalp)
		&& (other == astate.aotherp) && (invert == astate.ainvertp) && (astate.flags == astate.flagsold) )
		return;	
	DrawFlushPrimitives(0);
	LOG(1,"\n\tastate.flags=%0x", astate.flags);
	astate.afuncp = (unsigned char) func;
	astate.afactorp = (unsigned char) factor;
	astate.alocalp = (unsigned char) local;
	astate.aotherp = (unsigned char) other;
	astate.ainvertp = (unsigned char) invert;
	astate.flagsold = astate.flags;
	GlideAlphaCombineUpdateVertexMode();
	GlideColorCombineUpdateVertexMode();
	callflags |= CALLFLAG_ALPHACOMBINE;
	return;
	
	DEBUG_END("grAlphaCombine", DebugRenderThreadId, 159);
}


void		GlideAlphaCombine()	{
DWORD					alphaOp[4];
DWORD					alphaArg1[4];
DWORD					alphaArg2[4];
APixelShader2Stages		*setting;
int						i, invert, afuncpindex;
	
	DEBUG_BEGIN("GlideAlphaCombine", DebugRenderThreadId, 52);
	
	d3dlocal = combinelocallookup[astate.alocalp];
	d3dother = combineotherlookup[astate.aotherp];
	
	switch(astate.afactorp)	{
		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_LOCAL:
			d3dfactor = d3dlocal;
			break;
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = d3dother;
			break;
		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = d3dother | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;
	}
	setting = alphaPixelShaders;
	afuncpindex = grfunctranslating[astate.afuncp];
	
	if (astate.afactorp == GR_COMBINE_FACTOR_ZERO) setting = alphaPixelShadersf0;
	if (astate.afactorp == GR_COMBINE_FACTOR_ONE) setting = alphaPixelShadersf1;
	
	i=0;
	if (astate.flags & STATE_APMULTITEXTURING)	{
		if ( (d3dother == D3DTA_TEXTURE) &&
			( (setting[astate.afuncp].alphaarg1_0 == PS_OTHER) || (setting[astate.afuncp].alphaarg2_0 == PS_OTHER) ) )	{
			alphaOp[0] = D3DTOP_SELECTARG1;
			alphaArg1[0] = D3DTA_DIFFUSE;
			alphaArg2[0] = D3DTA_DIFFUSE;
			i++;
		}
	}

	invert = astate.flags & STATE_TCAINVERT;
	astate.astagesnum = i + 1;
	alphaOp[i] = setting[afuncpindex].alphaop_0;
	alphaArg1[i] = GlideGetStageArg(setting[afuncpindex].alphaarg1_0, invert);
	alphaArg2[i] = GlideGetStageArg(setting[afuncpindex].alphaarg2_0, invert);
	if ( (alphaOp[i+1] = setting[afuncpindex].alphaop_1) != D3DTOP_DISABLE )	{
		astate.astagesnum = i+2;
		alphaArg1[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg1_1, invert);
		alphaArg2[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg2_1, invert);
	}

	/* A speciális opkódok feldolgozása (implicit argumentumokkal) */
	/* Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ */
	switch(alphaOp[i])	{
		case PS_BLENDFACTORALPHA:
			alphaOp[i] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = alphaArg1[i];
				alphaArg1[i] = alphaArg2[i];
				alphaArg2[i] = i;
			}
			break;
	}		
	if (astate.ainvertp)	{
		alphaOp[astate.astagesnum] = D3DTOP_SELECTARG2;
		alphaArg2[astate.astagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		astate.astagesnum++;
	}
	if (astate.flags & STATE_COLORKEYTNT)	{
		alphaOp[0] = D3DTOP_SELECTARG1;
		alphaArg1[0] = D3DTA_TEXTURE;
		astate.astagesnum = 1;
	}	

	for(i = 0; i < astate.astagesnum; i++)	{
		if (astate.alphaOp[i] != alphaOp[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP, astate.alphaOp[i] = alphaOp[i]);
		if (astate.alphaArg1[i] != alphaArg1[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG1, astate.alphaArg1[i] = alphaArg1[i]);
		if (astate.alphaArg2[i] != alphaArg2[i])
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG2, astate.alphaArg2[i] = alphaArg2[i]);
	}

	for (;i < MAX_TEXSTAGES; i++)	{
		if (astate.alphaOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAOP, astate.alphaOp[i] = D3DTOP_SELECTARG1);
		if (astate.alphaArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice->lpVtbl->SetTextureStageState(lpD3Ddevice, i, D3DTSS_ALPHAARG1, astate.alphaArg1[i] = D3DTA_CURRENT);
	}
	astate.alocal = d3dlocal;
	astate.aother = d3dother;
	callflags |= CALLFLAG_COLORCOMBINE;
	callflags &= ~CALLFLAG_ALPHACOMBINE;	
	if (!(astate.flags & STATE_NATIVECKMETHOD))	{
		callflags |= CALLFLAG_SETCOLORKEY;		
		astate.flags |= STATE_CCACSTATECHANGED;
	}
	
	DEBUG_END("GlideAlphaCombine", DebugRenderThreadId, 52);
}


void EXPORT guColorCombineFunction( GrColorCombineFnc_t func )	{

//	return;
	DEBUG_BEGIN("guColorCombineFunction", DebugRenderThreadId, 53);

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
	
	DEBUG_END("guColorCombineFunction", DebugRenderThreadId, 53);
}


void EXPORT guAlphaSource( GrAlphaSource_t mode )	{
	
	DEBUG_BEGIN("guAlphaSource", DebugRenderThreadId, 54);
	
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
	DEBUG_END("guAlphaSource", DebugRenderThreadId, 54);
}


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


void EXPORT grGammaCorrectionValue( float value )	{
unsigned int	i, t;
float			corr;
DDGAMMARAMP		gammaRamp;
	
	DEBUG_BEGIN("grGammaCorrectionValue", DebugRenderThreadId, 55);

	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	corr = 256 * ((float) config.Gamma)/100;
	if (lpDDGamma != NULL)	{	
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
		lpDDGamma->lpVtbl->SetGammaRamp(lpDDGamma, 0, &gammaRamp);
	}

	DEBUG_END("grGammaCorrectionValue", DebugRenderThreadId, 55);
}


void EXPORT grAlphaTestFunction( GrCmpFnc_t function )	{
DWORD			temp;
unsigned int	flag;

	DEBUG_BEGIN("grAlphaTestFunction",DebugRenderThreadId, 56);
	
	//function = GR_CMP_ALWAYS;
	LOG(0,"\ngrAlphaTestFunction(function=%s)",cmpfnc_t[function]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;	
	if (function > GR_CMP_ALWAYS) return;
	if ((temp = cmpFuncLookUp[function]) == astate.alphatestfunc) return;
	DrawFlushPrimitives(0);
	if (temp == D3DCMP_ALWAYS)	{
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	} else {
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAREF, astate.alpharef);
	}
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAFUNC, astate.alphatestfunc = temp);

	if (GetCKMethodValue(config.Flags) == CFG_CKMAUTOMATIC) {
		flag = (temp == D3DCMP_EQUAL) || (temp == D3DCMP_NOTEQUAL) ? STATE_NATIVECKMETHOD : 0;
		if ((astate.flags & STATE_NATIVECKMETHOD) != flag) {
			astate.flags &= ~STATE_NATIVECKMETHOD;
			astate.flags |= flag;
			callflags |= CALLFLAG_SETCOLORKEYSTATE | CALLFLAG_SETCOLORKEY;
		}
	}

	/* colorkeying extra work */
	callflags |= (astate.flags & STATE_NATIVECKMETHOD) ? CALLFLAG_SETCOLORKEYSTATE : CALLFLAG_SETCOLORKEY;

	astate.flags |= STATE_ALPHATESTCHANGED;
	
	DEBUG_END("grAlphaTestFunction",DebugRenderThreadId, 56);
}


void EXPORT grAlphaTestReferenceValue( GrAlpha_t value )	{
	
//		return;
	DEBUG_BEGIN("grAlphaTestReferenceValue", DebugRenderThreadId, 57);

	LOG(0,"\ngrAlphaTestReferenceValue(value=%0x)", value);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (value == astate.alpharef) return;
	DrawFlushPrimitives(0);
	/* colorkeying extra work */
	astate.alpharef = (DWORD) value;
	if ( (astate.flags & STATE_ALPHACKUSED) && (astate.alphatestfunc == D3DCMP_ALWAYS) ) return;
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHAREF, astate.alpharef);
	
	DEBUG_END("grAlphaTestReferenceValue", DebugRenderThreadId, 57);
}


void EXPORT grDitherMode( GrDitherMode_t mode )	{
DWORD	val;
	
	DEBUG_BEGIN("grDitherMode", DebugRenderThreadId, 58);

	LOG(0,"\ngrDitherMode(mode=%s)", dithermode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	val = (mode == GR_DITHER_2x2) || (mode == GR_DITHER_4x4) ? TRUE : FALSE; 
	DrawFlushPrimitives(0);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_DITHERENABLE, val);
	
	DEBUG_END("grDitherMode", DebugRenderThreadId, 58);
}


void EXPORT grDisableAllEffects( void )	{

	DEBUG_BEGIN("grDisableAllEffects",DebugRenderThreadId, 59);

	LOG(0,"\ngrDisableAllEffects()");
	grAlphaBlendFunction(GR_BLEND_ONE,GR_BLEND_ZERO, 0, 0);
	grAlphaTestFunction(GR_CMP_ALWAYS);
	grChromakeyMode(GR_CHROMAKEY_DISABLE);
	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
	
	DEBUG_END("grDisableAllEffects",DebugRenderThreadId, 59);
}


void EXPORT grCullMode( GrCullMode_t mode ) {
DWORD	dxcmode;
	
	//mode=GR_CULL_DISABLE;
	DEBUG_BEGIN("grCullMode", DebugRenderThreadId, 60);
	
	LOG(0,"\ngrCullMode(mode=%s)",cullmode_t[mode]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	astate.gcullmode = mode;
	dxcmode = D3DCULL_NONE;
	if (mode != GR_CULL_DISABLE)	{
		if (astate.locOrig == GR_ORIGIN_UPPER_LEFT) dxcmode = (mode == GR_CULL_POSITIVE) ? D3DCULL_CW : D3DCULL_CCW;
		else dxcmode = (mode == GR_CULL_POSITIVE) ? D3DCULL_CCW : D3DCULL_CW;
		if (config.Flags2 & CFG_YMIRROR) dxcmode = (dxcmode == D3DCULL_CCW) ? D3DCULL_CW : D3DCULL_CCW;
	}
	if (dxcmode == astate.cullmode) return;
	DrawFlushPrimitives(0);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE,
										astate.cullmode = dxcmode);
	
	DEBUG_END("grCullMode", DebugRenderThreadId, 60);
}


void EXPORT grHints( GrHint_t type, FxU32 hintMask ) {

	DEBUG_BEGIN("grHints", DebugRenderThreadId, 61);
	
	LOG(1,"\ngrHints(type=%s, hintMask=%0x)", hint_t[type], hintMask);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (type == GR_HINT_STWHINT)	{
		if (hintMask != astate.hints) DrawFlushPrimitives(0);
		astate.hints = hintMask;
		GlideSetUseTmuWMode();
	}
	
	DEBUG_END("grHints", DebugRenderThreadId, 61);
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
unsigned int	vbool;

	DEBUG_BEGIN("grSstVRetraceOn",DebugRenderThreadId, 62);

	LOG(0,"\ngrSstVRetraceOn()");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return(FXFALSE);
	if (!(config.Flags & CFG_SETREFRESHRATE))	{
		lpDD->lpVtbl->GetVerticalBlankStatus(lpDD, &vbool);
		if (vbool) return(FXTRUE);
	}
	return(FXFALSE);
	
	DEBUG_END("grSstVRetraceOn",DebugRenderThreadId, 62);
}


FxU32 EXPORT grSstStatus( void ) {
FxU32			temp;
unsigned int	vbool;
		
	DEBUG_BEGIN("grSstStatus",DebugRenderThreadId, 63);
	
	LOG(0,"\ngrSstStatus()");
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return(0);
	temp = 0x0FFFF040; // 0000 1111 1111 1111 1111 0000 0100 0000;
	temp |= (GetBufferNumPending() & 0x7) << 28;	
	temp |= (buffswapnum % movie.cmiBuffNum) << 10;
	if (!(config.Flags & CFG_SETREFRESHRATE))	{
		lpDD->lpVtbl->GetVerticalBlankStatus(lpDD, &vbool);
		if (vbool) temp |= 0x40;
	}
	return(temp);
	
	DEBUG_END("grSstStatus",DebugRenderThreadId, 63);
}


void EXPORT grResetTriStats( void )	{
	
	stats.trisProcessed = 0;
}


void EXPORT grTriStats(FxU32 *trisProcessed, FxU32 *trisDrawn)	{
	
	*trisProcessed = *trisDrawn = stats.trisProcessed;
}


FxU32 EXPORT grSstVideoLine( void )	{
DWORD	line;
	
	DEBUG_BEGIN("grSstVideoLine",DebugRenderThreadId, 64);

	LOG(0,"\ngrSstVideoLine()");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return(3);
	if (lpDD->lpVtbl->GetScanLine(lpDD, &line) == DD_OK) return(line);
	return(3);
	
	DEBUG_END("grSstVideoLine",DebugRenderThreadId, 64);
}


FxBool EXPORT grSstControlMode( FxU32 mode) {

	DEBUG_BEGIN("grSstControlMode",DebugRenderThreadId, 65);

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
	return(FXTRUE);
	
	DEBUG_END("grSstControlMode",DebugRenderThreadId, 65);
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

	return( (float) floor(pow(2.0f, 3.0f + (double)(i >> 2)) / (8 - (i & 3))) );
}


void EXPORT guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
int		i;
double	max;
float	w;
	
	DEBUG_BEGIN("guFogGenerateExp", DebugRenderThreadId, 66);
	
	max = 1.0f - exp(-density * guFogTableIndexToW(GR_FOG_TABLE_SIZE-1));
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		w = guFogTableIndexToW(i);
		fogTable[i] = (GrFog_t) ((255.0f - (255.0f * exp(-density * w))) / max);
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_END("guFogGenerateExp", DebugRenderThreadId, 66);
}


void EXPORT guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
int		i;
double	max;
float	w;
	
	DEBUG_BEGIN("grFogGenerateExp2", DebugRenderThreadId, 67);
	
	max = 1.0f - exp(-(density * SQR(guFogTableIndexToW(GR_FOG_TABLE_SIZE))));
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		w = density*guFogTableIndexToW(i);
		fogTable[i] = (GrFog_t) ((255.0f - (255.0f*exp(-SQR(w)))) / max);
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_END("grFogGenerateExp2",DebugRenderThreadId, 67);
}


void EXPORT guFogGenerateLinear( GrFog_t fogTable[GR_FOG_TABLE_SIZE],
						  float nearW, float farW ) {
int		i,j;
	
	DEBUG_BEGIN("grFogGenerateLinear", DebugRenderThreadId, 68);
	
	for(i = 0; i < GR_FOG_TABLE_SIZE; i++)	{
		j = (int) (255.0f * (guFogTableIndexToW(i) - nearW) / (farW - nearW));
		if (j < 0) j = 0;
		if (j > 255) j = 255;
		fogTable[i] = j;
	}
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_END("grFogGenerateLinear", DebugRenderThreadId, 68);
}


void EXPORT grFogColorValue( GrColor_t value ) {
DWORD fogcolor;
		
	DEBUG_BEGIN("grFogColorValue", DebugRenderThreadId, 69);
	
	LOG(0, "\ngrFogColorValue (value=%x ", value);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (fogcolor = GlideGetColor(value)) != astate.fogcolor)	{
		astate.fogcolor = fogcolor;
		if (astate.fogmodifier & GR_FOG_ADD2) fogcolor = 0;
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGCOLOR, fogcolor);
	}
	
	DEBUG_END("grFogColorValue", DebugRenderThreadId, 69);
}


void EXPORT grFogMode( GrFogMode_t mode ) {
DWORD fogColor;
DWORD fogEnable;
	
	//mode=GR_FOG_DISABLE;	
	DEBUG_BEGIN("grFogMode", DebugRenderThreadId, 70);

	LOG(0, "\ngrFogMode(mode=%s ", fogmode_t[mode & 0xFF]);
	LOG(0, fogmodemod_t[mode >> 8]);
	LOG(0, ")");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	DrawFlushPrimitives(0);
	
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
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGENABLE, fogEnable);

	if ((astate.fogmodifier = (mode & (GR_FOG_MULT2 | GR_FOG_ADD2))) & GR_FOG_ADD2) fogColor = 0;
		else fogColor = astate.fogcolor;
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGCOLOR, fogColor);
	GlideSetUseTmuWMode();
	
	DEBUG_END("grFogMode", DebugRenderThreadId, 70);
}


void EXPORT grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] )	{
unsigned int i, *src, *dst;
	
	DEBUG_BEGIN("grFogTable\n", DebugRenderThreadId, 71);
	
	//QLOG("\ngrFogtable");
	//for(i=0;i<GR_FOG_TABLE_SIZE;i++) QLOG("%0x,",table[i]);
	src = (unsigned int *) table;
	dst = (unsigned int *) fogtable;
	for(i = 0; i < GR_FOG_TABLE_SIZE/4; i++) dst[i] = src[i] ^ 0xFFFFFFFF;
	fogtable[GR_FOG_TABLE_SIZE] = fogtable[GR_FOG_TABLE_SIZE-1];
	
	DEBUG_END("grFogTable", DebugRenderThreadId, 71);
}


/* Origo (és a koordinátarendszer) beállítása */
void EXPORT grSstOrigin( GrOriginLocation_t origin )	{
	
	DEBUG_BEGIN("grSstOrigin", DebugRenderThreadId, 72);
	
	LOG(0,"\ngrSstOrigin( origin=%s )",origin_t[origin]);	
	if (astate.locOrig == origin) return;	
	astate.locOrig = origin;
	DrawComputeScaleCoEffs(origin);
	if (astate.cullmode != D3DCULL_NONE)	{
		DrawFlushPrimitives(0);
		grCullMode(astate.gcullmode);
	}
	return;
	
	DEBUG_END("grSstOrigin", DebugRenderThreadId, 72);
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
LPDIRECTDRAWSURFACE7	lpDDRenderTarget = NULL;
	
	DEBUG_BEGIN("grRenderBuffer", DebugRenderThreadId, 73);

	LOG(0,"\ngrRenderBuffer(buffer=%s)",buffer_t[buffer]);

	//buffer=GR_BUFFER_FRONTBUFFER;
	
	if (renderbuffer == buffer)
		return;

	DrawFlushPrimitives(0);
	
	if (runflags & RF_SCENECANBEDRAWN)
		lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
	
	lpDDRenderTarget = (buffer == GR_BUFFER_FRONTBUFFER) ? lpDDFrontBuff : lpDDBackBuff;
	lpD3Ddevice->lpVtbl->SetRenderTarget(lpD3Ddevice, lpDDRenderTarget, 0);
	
	if (lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice) != DD_OK) runflags &= ~RF_SCENECANBEDRAWN;
		else runflags |= RF_SCENECANBEDRAWN;

	renderbuffer = buffer;

	DEBUG_END("grRenderBuffer", DebugRenderThreadId, 73);
}


/*-------------------------------------------------------------------------------------------*/
/* grSstWinOpen, grSstWinClose és grGlideShutDown implementáció								 */
/*-------------------------------------------------------------------------------------------*/

LRESULT CALLBACK GlideConsumerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
	switch(uMsg)	{
		case WM_USER:
			if (lParam==0)	{
				winopenstruc.ret = grSstWinOpen(winopenstruc.hwnd,winopenstruc.res,winopenstruc.refresh,winopenstruc.l_cFormat,winopenstruc.locateOrigin,
												winopenstruc.numBuffers,winopenstruc.numAuxBuffers);
			} else {
				grSstWinClose();
			}
			return(0);
		case WM_CREATE:
			return(0);
		case WM_TIMER:
			CallConsumer(1);
			return(0);
		case WM_DESTROY:
			//PostQuitMessage(0);
			return(0);
		default:
			return(DefWindowProc(hwnd,uMsg,wParam,lParam));
	}
}


void	CleanUpWindow()	{

	DEBUG_BEGIN("CleanUpWindow",DebugRenderThreadId, 80);

#ifdef	DOS_SUPPORT

	if (platform == PLATFORM_WINDOWS) {

#endif
		if (runflags & RF_WINDOWCREATED)	{	
			DestroyWindow(movie.cmiWinHandle);
			UnregisterClass("DGVOODOO",hInstance);
		}

#ifdef	DOS_SUPPORT
	} else {
		
		DestroyRenderingWindow ();
	
	}
#endif
	
	DEBUG_END("CleanUpWindow",DebugRenderThreadId, 80);
}


LRESULT CALLBACK newwndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	{

	if ( (uMsg == WM_USER) && (wParam == 'DEGE') )	{
		if (lParam == 0) threadCommandWindow = CreateWindowEx(0, "DGVOODOO", "", WS_OVERLAPPED, 0, 0, 100, 100, NULL, NULL, hInstance, NULL);
			else DestroyWindow(threadCommandWindow);
		return(0);
	} else return(CallWindowProc(oldwndproc, hwnd, uMsg, wParam, lParam));
}


int		DoThreadCall(HWND hwnd, int command)	{

	if ( (oldwndproc = ( LRESULT (CALLBACK *)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
						 SetWindowLong(hwnd, GWL_WNDPROC,(LONG) newwndproc)) == NULL) return(GetLastError());
	SendMessage(hwnd, WM_USER, 'DEGE', 0);
	if (threadCommandWindow == NULL) return(GetLastError());
	SendMessage(threadCommandWindow, WM_USER, 'DEGE', command);
	SendMessage(hwnd, WM_USER, 'DEGE', 1);
	if ( (SetWindowLong(hwnd, GWL_WNDPROC, (LONG) oldwndproc)) == (LONG) NULL) return(GetLastError());
	return(0);
}


FxBool EXPORT1 grSstWinOpen(FxU32 _hwnd, GrScreenResolution_t res, \
							GrScreenRefresh_t refresh, GrColorFormat_t l_cFormat, \
							GrOriginLocation_t locateOrigin, int numBuffers, \
							int numAuxBuffers)	{
unsigned int	i;
DDCAPS			ddcaps = {sizeof(DDCAPS)};
WNDCLASS		wndc = {0, GlideConsumerProc, 0, 0, 0, 0, 0, 0, NULL, "DGVOODOO"};
HWND			hwnd = (HWND) _hwnd;

#ifdef	DOS_SUPPORT

unsigned char	windowTitle[256];
unsigned char	temp[16];

#endif

	
	DEBUG_BEGIN("grSstWinOpen", DebugRenderThreadId, 81);

	//l_cFormat = GR_COLORFORMAT_ABGR;

	if (runflags & RF_INITOK) return(FXFALSE);
	LOG(1,"\ngrSstWinOpen(hwnd=%0x, res=%s, ref=%s, cformat=%s, org_loc=%s, num_buffers=%d, num_aux=%d)",
						hwnd, res_t[res], ref_t[refresh], cFormat_t[l_cFormat],
						locOrig_t[locateOrigin], numBuffers, numAuxBuffers);
	
	if (sizeof(_GrState) >= 320) MessageBox(0, "_GrState túl nagy!", 0, 0);

	FillMemory(&astate, sizeof(_GlideActState), 0xFF);
	ZeroMemory(&movie, sizeof(MOVIEDATA));
	runflags = 0;

	DEBUGLOG (0,"\ngrSstWinopen: required resolution: %dx%d", _xresolution[res], _yresolution[res]);
	DEBUGLOG (1,"\ngrSstWinopen: megkívánt felbontás: %dx%d", _xresolution[res], _yresolution[res]);
	
	DEBUGLOG (0,"\ngrSstWinopen: required refresh rate: %dHz",_refreshrate[refresh]);
	DEBUGLOG (1,"\ngrSstWinopen: megkívánt frissítés: %dHz",_refreshrate[refresh]);
	
	cFormat = l_cFormat;
	astate.locOrig = locateOrigin;

	if ( (res > MAX_RES_CONSTANT) || (refresh > MAX_REFRATE_CONSTANT) ) return(FXFALSE);
	movie.cmiFlags = MV_3D | MV_ANTIALIAS;

#ifdef	DOS_SUPPORT

	if (platform != PLATFORM_WINDOWS) {
		CreateRenderingWindow();

		if (platform == PLATFORM_DOSWINNT) {
			ShowWindow(warningBoxHwnd, SW_HIDE);	
			ShowWindow(renderingWinHwnd, SW_SHOW);
			SetForegroundWindow(renderingWinHwnd);
		} else if (platform == PLATFORM_DOSWIN9X) {
			if (config.Flags & CFG_WINDOWED) DeviceIoControl(hDevice, DGSERVER_SETALLFOCUSONSYSVM, NULL, 0, NULL, 0, NULL, NULL);
		}
		
		if (!InitDraw(&movie, NULL))	{
			DEBUGLOG(0,"\n   Error: grSstWinopen: InitDraw() failed");
			DEBUGLOG(1,"\n   Hiba: grSstWinopen: InitDraw() sikertelen");
			return(FXFALSE);
		}
		DEBUGLOG(2, "\ngrSstWinopen: InitDraw() OK");
	
	} else {

#endif
		
		if (hwnd == NULL) hwnd = GetActiveWindow();

		wndc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wndc.hInstance = hInstance;
		RegisterClass(&wndc);
		if ( ((HWND) hwnd) == NULL)	{
			if ( (movie.cmiWinHandle = CreateWindowEx(0, "DGVOODOO", "dgVoodoo", WS_OVERLAPPEDWINDOW,
												CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
												NULL, NULL, hInstance, NULL)) == NULL) return(FXFALSE);
			if (movie.cmiWinHandle != NULL) {
				runflags |= RF_WINDOWCREATED;
				ShowWindow(movie.cmiWinHandle, SW_SHOWNORMAL);
				if (!InitDraw(&movie, NULL))	{
					DEBUGLOG(0,"\n   Error: grSstWinopen: InitDraw() failed");
					DEBUGLOG(1,"\n   Hiba: grSstWinopen: InitDraw() hiba");
					CleanUpWindow();
					return(FXFALSE);
				}
			} else {
				return(FXFALSE);
			}
		} else {

			if ( GetCurrentThreadId() != GetWindowThreadProcessId( (HWND) hwnd, NULL) )	{
				winopenstruc.hwnd = _hwnd;
				winopenstruc.res = res;
				winopenstruc.refresh = refresh;
				winopenstruc.l_cFormat = l_cFormat;
				winopenstruc.locateOrigin = locateOrigin;
				winopenstruc.numBuffers = numBuffers;
				winopenstruc.numAuxBuffers = numAuxBuffers;
				if (i = DoThreadCall((HWND) hwnd, 0))	{
				  DEBUGLOG(0,"\n   Error: grSstWinOpen: Multithreading issue: calling grSstWinopen in the context of the thread of the Glide-window has failed.");
				  DEBUGLOG(0,"\n          Error code returned by GetLastError is: %d", i);
				  DEBUGLOG(1,"\n   Hiba: grSstWinOpen: Többszálúság kezelése: a grSstWinOpen függvényt nem sikerült meghívni a Glide-ablak szálából.");
				  DEBUGLOG(1,"\n          A hibakód, amit a GetLastError visszaadott: %d", i);
				}
				return(winopenstruc.ret);
			}
		}
		movie.cmiWinHandle = (HWND) hwnd;
		if (!InitDraw(&movie, NULL))	{
		  DEBUGLOG(0,"\n   Error: grSstWinopen: InitDraw() failed");
		  DEBUGLOG(1,"\n   Hiba: grSstWinopen: InitDraw() hiba");
		  return(FXFALSE);
		}
		DEBUGLOG(2,"\ngrSstWinopen: InitDraw() OK");

#ifdef	DOS_SUPPORT
	
	}

#endif
	
	lpDD = movie.cmiDirectDraw;
	lpD3D = movie.cmiDirect3D;	
	movie.cmiXres = appXRes = _xresolution[res];
	movie.cmiYres = appYRes = _yresolution[res];
	movie.cmiDispFreq = config.RefreshRate;
	movie.cmiFreq = _refreshrate[refresh];
	movie.cmiBuffNum = (config.Flags & CFG_TRIPLEBUFFER) ? 3 : ( (numBuffers != 0) ? numBuffers : (numBuffers = numAuxBuffers));
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

	movie.cmiBitPP = (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16;	
	movie.cmiPixelFormat = NULL;
	movie.cmiFlags |= ((config.Flags & CFG_WINDOWED) ? MV_WINDOWED : MV_FULLSCR) |
		              ((config.Flags & CFG_SETREFRESHRATE) ? 0 : MV_NOCONSUMERCALL) | MV_VIDMEM;	
	if (config.Flags2 & CFG_CLOSESTREFRESHFREQ)	{
		i = 0;
		lpDD->lpVtbl->EnumDisplayModes(lpDD, DDEDM_REFRESHRATES, NULL, &i, GlideEnumRefreshRatesCallback);
		movie.cmiDispFreq = i;

		DEBUGLOG (0,"\ngrSstWinOpen:   closest supported refrate: %dHz", i);
		DEBUGLOG (1,"\ngrSstWinOpen:   legközelebbi támogatott frissítési frekvencia: %dHz", i);
	}
	if (!InitMovie())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: InitMovie() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: InitMovie() hiba");
		UninitDraw();
		CleanUpWindow();
		return(FXFALSE);
	}
	DEBUGLOG(2,"\ngrSstWinopen: InitMovie() OK");
	
	if (!GlideSelectAndCreate3DDevice())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: lpD3D->CreateDevice() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: lpD3D->CreateDevice() nem sikerült");
		DeInitMovie();
		UninitDraw();
		CleanUpWindow();
		return(FXFALSE);
	}
	DEBUGLOG(2,"\ngrSstWinopen: lpD3D->CreateDevice() OK");

	lpDDBackBuff = GABA();
	lpDDFrontBuff = GetFrontBuffer();

#ifdef	DOS_SUPPORT

	if (platform != PLATFORM_WINDOWS) {

		//HideMouseCursor(0);
	    if (platform == PLATFORM_DOSWIN9X) {
	    	if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow( (HWND) c->WinOldApHWND,SW_HIDE);
	    } else {
			if (config.Flags2 & CFG_HIDECONSOLE) ShowWindow(consoleHwnd, SW_HIDE);
	    }
	    ShowWindow(warningBoxHwnd, SW_HIDE);
	    ShowWindow(renderingWinHwnd, SW_SHOW);
	    SetForegroundWindow(movie.cmiWinHandle);
	}

#endif

	runflags |= RF_INITOK | RF_SCENECANBEDRAWN;
	callflags = 0;
	astate.flags = 0;

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
	if (!GrabberInit())	{
		DEBUGLOG(0,"\n   Error: grSstWinopen: GlideGrabberInit() failed");
		DEBUGLOG(1,"\n   Hiba: grSstWinopen: GlideGrabberInit() hiba");	
		grSstWinClose ();
		return(FXFALSE);
	}
	
	lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);

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
	GlideColorCombine();
	grCullMode(GR_CULL_DISABLE);
	grClipWindow(0, 0, appXRes, appYRes);
	grAlphaBlendFunction(GR_BLEND_ONE, GR_BLEND_ZERO, GR_BLEND_ONE, GR_BLEND_ZERO);
	grAlphaTestFunction(GR_CMP_ALWAYS);
	grAlphaTestReferenceValue(0x0);
	grDitherMode(GR_DITHER_4x4);	
	grTexMipMapMode(0, GR_MIPMAP_DISABLE, FXFALSE);
	grTexLodBiasValue(0, 0.0f);
	grDepthBiasLevel(0);

	GlideWriteOutCaps();
	
	grColorMask(FXTRUE, FXTRUE);
	grFogMode(GR_FOG_DISABLE);
	grFogColorValue (0x0);

	grLfbConstantAlpha (0xFF);
	grLfbConstantDepth (0x0);

	/* Az elõre elcache-elt színek miatt (az astate struktúrában) */
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_FOGCOLOR, 0xFFFFFFFF);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_TEXTUREFACTOR, 0xFFFFFFFF);
	
	/* GammaControl-t támogatja a hardver? */
	lpDD->lpVtbl->GetCaps(lpDD, &ddcaps, NULL);
	lpDDGamma = NULL;
	if ( (ddcaps.dwCaps2 & DDCAPS2_PRIMARYGAMMA) && (movie.cmiFlags & MV_FULLSCR) )	{		
		if (!lpDDFrontBuff->lpVtbl->QueryInterface(lpDDFrontBuff, &IID_IDirectDrawGammaControl, &lpDDGamma))	{
			lpDDGamma->lpVtbl->GetGammaRamp(lpDDGamma, 0, &origGammaRamp);
			grGammaCorrectionValue(1.0f);
		}
	}
	
	GlideNullSurfaces();
	renderbuffer = GR_BUFFER_BACKBUFFER;

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
	
	return(FXTRUE);

	DEBUG_END("grSstWinOpen", DebugRenderThreadId, 81);
}


/* Glide lelövése: leállítjuk az idôzítôt, visszaváltunk eredeti felbontásba, stb. */
void EXPORT grSstWinClose( void )	{
int i;
	
	DEBUG_BEGIN("grSstWinClose", DebugRenderThreadId, 82);
	
	if (runflags & RF_INITOK)	{
		DEBUG_LOGMAXTIMES;

#ifdef	DOS_SUPPORT

		if (platform != PLATFORM_WINDOWS) {
			if (config.Flags & CFG_SETMOUSEFOCUS) RestoreMouseCursor(1);
		} else {

#endif
			
			if (GetCurrentThreadId() != GetWindowThreadProcessId(movie.cmiWinHandle, NULL) )	{
				if (i = DoThreadCall(movie.cmiWinHandle, 1))	{
					DEBUGLOG(0,"\n   Error: grSstWinClose: Multithreading issue: calling grSstWinClose in the context of the thread of the Glide-window has failed.");
					DEBUGLOG(0,"\n          Error code returned by GetLastError is: %d",i);
					DEBUGLOG(1,"\n   Hiba: grSstWinClose: Többszálúság kezelése: a grSstWinClose függvényt nem sikerült meghívni a Glide-ablak szálából.");
					DEBUGLOG(1,"\n         A hibakód, amit a GetLastError visszaadott: %d",i);
				}
				return;
			}
#ifdef	DOS_SUPPORT
		
		}

#endif
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice,0,NULL);
		lpD3Ddevice->lpVtbl->SetTexture(lpD3Ddevice,1,NULL);		
		DrawCleanUp ();
		GrabberCleanUp ();
		TexCleanUp ();
		LfbCleanUp ();
		LfbDepthBufferingCleanUp ();		
		runflags = 0;
		GlideDestroyFogTables ();
		lpD3Ddevice->lpVtbl->EndScene (lpD3Ddevice);
		lpD3Ddevice->lpVtbl->Release (lpD3Ddevice);
		if (lpDDGamma != NULL) lpDDGamma->lpVtbl->Release (lpDDGamma);
		lpDDGamma = NULL;
		lpD3Ddevice = NULL;
		lpD3D->lpVtbl->EvictManagedTextures (lpD3D);
		DeInitMovie ();
		UninitDraw ();

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

	DEBUG_END("grSstWinClose", DebugRenderThreadId, 82);
}


/* Glide könyvtár kilövése: véglegesen eldobunk minden objektumot a DirectX-bôl, */
/* a becsatolt DLL-eket (DDRAW és WINMM) eldobjuk								 */
void EXPORT grGlideShutdown()	{

	DEBUG_BEGIN("grGlideShutdown", DebugRenderThreadId, 83);
	
	grSstWinClose();
	TexCleanUpAtShutDown();

#ifdef	DOS_SUPPORT
	
	if (platform != PLATFORM_WINDOWS) {
		ShowWindow(warningBoxHwnd, SW_SHOW);
	}

#endif

	DEBUG_END("grGlideShutdown", DebugRenderThreadId, 83);

	DEBUG_GLOBALTIMEEND;
}


#ifdef GLIDE1

FxBool EXPORT grSstOpen(GrScreenResolution_t res, GrScreenRefresh_t ref, GrColorFormat_t cformat,
				 GrOriginLocation_t org_loc, GrSmoothingMode_t smoothing_mode, int num_buffers )	{

	DEBUG_BEGIN("grSstOpen", DebugRenderThreadId, 84);

	config.Flags |= CFG_LFBREALVOODOO;
	config.Flags2 |= CFG_LFBNOMATCHINGFORMATS;
	return(grSstWinOpen((FxU32) NULL, res, ref, cformat, org_loc,num_buffers, 0));
	
	DEBUG_END("grSstOpen", DebugRenderThreadId, 84);
}

#endif