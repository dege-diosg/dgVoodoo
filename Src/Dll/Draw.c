/*--------------------------------------------------------------------------------- */
/* Draw.c - Glide implementation, stuffs related to poly/primitive drawing          */
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
/* dgVoodoo: Draw.c																			*/
/*			 Primitívek rajzolásához kapcsolódó függvények									*/
/*------------------------------------------------------------------------------------------*/
#include	"dgVoodooBase.h"

#include	<math.h>
#include	<windows.h>
#include	<winuser.h>
#include	<glide.h>

#include	"dgVoodooConfig.h"
#include	"DDraw.h"
#include	"D3d.h"
#include	"Dgw.h"
#include	"Movie.h"
#include	"Version.h"
#include	"Main.h"
#include	"dgVoodooGlide.h"
#include	"Debug.h"
#include	"Log.h"
#include	"Draw.h"
#include	"LfbDepth.h"
#include	"Texture.h"


/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/

#define MAX_NUM_OF_VERTEXPOINTERS	256

/* Azon primitíveket, amelyeknek nincs szüksége vágásra, egy vertex bufferen keresztül */
/* rajzolja meg, míg a többit a hagyományos módon. Ha a vertex buffer allokációja */
/* sikertelen, akkor minden rajzolás a hagyományos módon zajlik. */

#define DRAW_MAX_VERTEXNUM			16384
#define DRAW_MAX_INDEXNUM			16384
#define DRAW_MAX_POLYINDEXNUM		4096

/* A használt vertextípus definiálása */
//#define D3D_VERTEXTYPE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define D3D_VERTEXTYPE				(D3DFVF_TLVERTEX)

#define DRAW_WFAR					65536.0f

#define WFAR						65536.0f
#define WNEAR						1.0f

#define WCONSTANT					0.05f

#define CLIP_WNEAR					0.01f

#define DRAWMODE_CLIPPING			0
#define	DRAWMODE_NOCLIPPING			1
#define DRAWMODE_ANTIALIAS			2
#define DRAWMODE_ANITALIASCLIP		3

#define VBFLAG_VBUSED				1
#define VBFLAG_VBAVAILABLE			2

#define AAFLAG_LINEAVAILABLE		1
#define AAFLAG_TRIANGLEAVAILABLE	2
#define AAFLAG_LINEENABLED			4
#define AAFLAG_TRIANGLEENABLED		8

#define CF_LEFT						1
#define	CF_RIGHT					2
#define CF_TOP						4
#define CF_BOTTOM					8
#define CF_WNEAR					16

#define RGBcval						(255.0f/256.0f)

void VertexProcessorSSE(GrVertex **vlist, int nVerts, int generateIndices);
void VertexProcessorFPU(GrVertex **vlist, int nVerts, int generateIndices);

//#define CHECKLOCK(buffer) if (bufflocks[buffer].locktype & LOCK_ISLOCKED) GlideLFBSwitchToFastWrite(buffer);

/*------------------------------------------------------------------------------------------*/
/*................................... Globális változók.....................................*/

DWORD					perspective;					/* perspektívakorrektség igen-nem (ez fontos!)*/
D3DTLVERTEX				*actvertex;						/* primitívek csúcsaira mutató pointerek */
D3DTLVERTEX				*memvertex, *vbvertex;
LPDIRECT3DVERTEXBUFFER7 lpD3DVertexBuffer;
LPDIRECT3DVERTEXBUFFER7 lpD3DVertexBufferTile;
D3DTLVERTEX				*vbbuffend;
int						vbflags;
short					*indices, *actindex;
DWORD					primType;						/* gyûjtött primitívek típusa */
DWORD					primclipping;
DWORD					primindexed;
unsigned int			numberOfVertices;
unsigned int			numberOfIndices;
unsigned int			actoffsetvb;
unsigned int			drawmode;
unsigned int			linemode;
unsigned int			antialias;
int						noVertexConverting;
float					linescalex;
float					linescaley;
const int				lineInd1[] = {0, 1, 3, 2};
const int				lineInd2[] = {0, 2, 3, 1};
WORD					tileIndex[6] = {0, 1, 2, 1, 3, 2};
const float				rgbcval = RGBcval;
const float				_Q		= WFAR / (WFAR - WNEAR);
const float				_QZn	= -((WFAR * WNEAR) / (WFAR - WNEAR));
const float				_scale4 = 4.0f;

/*__declspec(align(16))	float				tscale[4];
__declspec(align(16))	float				tconst[4];
__declspec(align(16))	const float			ARGBStreamScaler[4] = {RGBcval, RGBcval, RGBcval, RGBcval};
__declspec(align(16))	const float			StreamConst1_0[4] = {1.0f, 1.0f, 1.0f, 1.0f};
__declspec(align(16))	const float			StreamConst0_5[4] = {0.5f, 0.5f, 0.5f, 0.5f};
__declspec(align(16))	const float			StreamConst0_0[4] = {0.0f, 0.0f, 0.0f, 0.0f};
__declspec(align(16))	const float			StreamConst_DRAW_WFAR[4] = { DRAW_WFAR, DRAW_WFAR, DRAW_WFAR, DRAW_WFAR };
__declspec(align(16))	const float			StreamConstWCONSTANT[4] = { WCONSTANT, WCONSTANT, WCONSTANT, WCONSTANT };
__declspec(align(8))	const unsigned int	snapMask[2] = {0xFFFFF000, 0xFFFFF000};
__declspec(align(8))	const unsigned int	fogAlphaXORConstant[2] = { 0x0, 0xFF000000};*/

void					(*vertexProcessor) (GrVertex **vlist, int nVerts, int generateIndices);

GrVertex				**vertexPointers;
__int64					sse_perf = 0;
__int64					fpu_perf = 0;

/*------------------------------------------------------------------------------------------*/
/*............................Inline függvények a rajzoláshoz...............................*/

void __inline GlideSwitchToVBIfAvailable()	{

	if (vbflags & VBFLAG_VBAVAILABLE)	{
		vbflags|=VBFLAG_VBUSED;			
		vbbuffend=vbvertex+DRAW_MAX_VERTEXNUM;
		actvertex=vbvertex+actoffsetvb;
	}
}


void __inline PrimTypeToClippedTriangleList()	{
	
	if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != FALSE) )	{
		DrawFlushPrimitives(0);
		primType = D3DPT_TRIANGLELIST;
		primindexed = FALSE;
	}
	if (primclipping != FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, FALSE);
		primclipping = FALSE;
		GlideSwitchToVBIfAvailable();
		noVertexConverting = 0;
	}
}


void __inline PrimTypeToClippedIndexedTriangleList()	{
	
	if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != TRUE) )	{	
		DrawFlushPrimitives(0);
		primType = D3DPT_TRIANGLELIST;
		primindexed = TRUE;
	}
	if (primclipping != FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, FALSE);
		primclipping = FALSE;
		GlideSwitchToVBIfAvailable();
		noVertexConverting = 0;
	}
}

/*------------------------------------------------------------------------------------------*/
/*......................... FPU-vertexkonverter, inline függvények .........................*/


/* Egy float-ról visszaadja, hogy az nemvégtelen-e */
int __forceinline IsNotInf(float f)	{
	_asm	{
		mov	eax,f
		and eax,0x7FFFFFFF
		sub eax,0x7F800000
	}
}


void __forceinline ConvertVertex (GrVertex *vertex, D3DTLVERTEX *actvertex)	{
int	x, y;
	
	_asm	{
		mov		ecx,vertex
		fld		_scale4
		mov		edx,actvertex
		fld		DWORD PTR [ecx]GrVertex.x
		fscale
		fistp	x
		fld		DWORD PTR [ecx]GrVertex.y
		mov		eax,x
		fscale
		movsx	eax,ax
		fistp	y
		mov		x,eax
		fchs
		fild	DWORD PTR x
		mov		eax,y
		fscale
		movsx	eax,ax
		fmul	xScale
		mov		y,eax
		fadd	xConst;
		fstp	[edx]D3DTLVERTEX.sx
		fild	DWORD PTR y
		fscale
		fmul	yScale
		fadd	yConst
		fstp	[edx]D3DTLVERTEX.sy
		fstp	st(0)
	}
}


void __forceinline ConvertVertexCustom (GrVertex *vertex, float *toX, float *toY)	{
int	x, y;
	
	_asm	{
		mov		ecx,vertex
		fld		_scale4
		mov		edx,toX
		fld		DWORD PTR [ecx]GrVertex.x
		fscale
		fistp	x
		fld		DWORD PTR [ecx]GrVertex.y
		mov		eax,x
		fscale
		movsx	eax,ax
		fistp	y
		mov		x,eax
		fchs
		fild	DWORD PTR x
		mov		eax,y
		fscale
		movsx	eax,ax
		fstp	[edx]
		mov		y,eax
		fild	DWORD PTR y
		mov		edx,toY
		fscale
		fstp	[edx]
		fstp	st(0)
	}
}


int __forceinline GetFogIndexValue(float f, float *w)	{
int i;
	
	_asm	{
		fld1
		mov		eax,w
		xor		ecx,ecx
		fdiv	f
		fst		DWORD PTR [eax]
		fistp	DWORD PTR i
		mov		eax,DWORD PTR i
		or		eax,eax
		cmovs	eax,ecx
	}
}


/* Inline függvény, az mélységhez (oow) hozzáadja a depth bias-t (W-pufferelés esetén) */
float __forceinline AddWBias(float oow, float *toStore)	{
	_asm	{
		mov		ecx,depthBias_W
		mov		eax,oow
		mov		edx,toStore
		or		ecx,ecx
		mov		[edx],eax
		je		_biasingOK
		fld1
		fdiv	oow
		fstp	oow
		add		DWORD PTR oow,ecx		;egész közvetlen hozzáadása a lebegõpontos értékhez!!
		fld1
		fdiv	oow
		fstp	DWORD PTR [edx]
	_biasingOK:
	}
}


/* Inline függvény, oow-bõl visszaadja a z-t, depth bias beleszámolva (W-pufferelés emulálása esetén) */
float __forceinline AddWEmuBias(float oow)	{
	_asm	{
		fld1
		fld		oow
		fucomi	st,st(1)
		fcmovnbe st,st(1)
		fdiv
		fstp	oow
		mov		eax,depthBias_W
		add		oow,eax
		fld		_QZn
		fdiv	oow
		fadd	_Q
	}
}


float __forceinline ZCLAMP(float ooz)	{

	_asm	{
		fld		ooz
		fldz
		fucomi	st,st(1)
		fcmovb	st,st(1)
		fld1
		fucomi	st,st(1)
		fcmovnb	st,st(1)
		fstp	st(1)
		fstp	st(1)
	}
}


DWORD __forceinline GetARGB(GrVertex *vertex)	{
unsigned int i;

	_asm	{
		mov		edx,vertex
		fld		[edx]vertex.a
		fmul	rgbcval
		fistp	i
		movzx	ecx,BYTE PTR i
		fld		[edx]vertex.r
		shl		ecx,24
		fmul	rgbcval
		mov		eax,ecx
		fistp	i
		movzx	ecx,BYTE PTR i
		fld		[edx]vertex.g
		shl		ecx,16
		fmul	rgbcval
		or		eax,ecx
		fistp	i
		movzx	ecx,BYTE PTR i
		fld		[edx]vertex.b
		shl		ecx,8
		fmul	rgbcval
		or		eax,ecx
		fistp	i
		movzx	ecx,BYTE PTR i
		or		eax,ecx
	}
}


DWORD __forceinline	GetFogIntValue(float fog)	{
DWORD	fogInt;
	_asm	{
		fld		DWORD PTR fog
		fistp	fogInt
	}
	return(fogInt << 24);
}


void __inline GlideSetStencilMode()	{
D3DTLVERTEX v[6];

	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILFUNC,D3DCMP_ALWAYS);
	if (astate.zenable!=FALSE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ZWRITEENABLE,FALSE);
	if (astate.alphaBlendEnable!=TRUE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
	if (astate.srcBlend!=D3DBLEND_ZERO) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_SRCBLEND,D3DBLEND_ZERO);
	if (astate.dstBlend!=D3DBLEND_ONE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE);

	v[0].sx = 0.0f;
	v[0].sy = 0.0f;
	v[1].sx = v[3].sx = (float) movie.cmiXres;
	v[1].sy = v[3].sy = 0.0f;
	v[2].sy = v[4].sy = (float) movie.cmiYres;
	v[5].sx = (float) movie.cmiXres;
	v[5].sy = (float) movie.cmiYres;
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = v[4].sz = v[5].sz = depthBias_cz;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = v[5].rhw = depthBias_cw;
	lpD3Ddevice->lpVtbl->DrawPrimitive(lpD3Ddevice,D3DPT_TRIANGLELIST,
											D3DFVF_TLVERTEX, v, 6, 0);
	
	if (astate.zenable!=FALSE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ZWRITEENABLE,TRUE);
	if (astate.alphaBlendEnable!=TRUE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ALPHABLENDENABLE,astate.alphaBlendEnable);
	if (astate.srcBlend!=D3DBLEND_ZERO) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_SRCBLEND,astate.srcBlend);
	if (astate.dstBlend!=D3DBLEND_ONE) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_DESTBLEND,astate.dstBlend);

	if (astate.zfunc!=D3DCMP_ALWAYS) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ZFUNC,D3DCMP_ALWAYS);
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_STENCILFUNC,D3DCMP_EQUAL);
}


void __inline GlideUnsetStencilMode()	{

	if (astate.zfunc!=D3DCMP_ALWAYS) lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ZFUNC,astate.zfunc);
}


/* Kirajzolja az összegyûjtött primitíveket, ha vannak. */
void DrawFlushPrimitives(int toomanyvertex)	{
DWORD	callf, flags;
//int cop,carg1,carg2, alphaop,aarg1,aarg2;
//DWORD abe, asrc, adst;
DWORD clipping;
		
	DEBUG_BEGIN("DrawFlushPrimitives", DebugRenderThreadId, 1);

	if (numberOfVertices)	{

		LOG(0,"\n    flushing: vertex: %d, index: %d",numberOfVertices, numberOfIndices);
		GlideLFBCheckLock(renderbuffer);
		LfbSetBuffDirty(renderbuffer);
		callf = callflags;
		flags = astate.flags;
		if (callflags & CALLFLAG_SETTEXTURE) GlideTexSource();
		if (callflags & CALLFLAG_RESETTEXTURE) TexUpdateMultitexturingState(astate.acttex);
		if (callflags & CALLFLAG_ALPHACOMBINE) GlideAlphaCombine();
		if (callflags & CALLFLAG_COLORCOMBINE) GlideColorCombine();
		if (callflags & CALLFLAG_RESETTEXTURE) TexSetCurrentTexture(astate.acttex);
		if (callflags & CALLFLAG_SETCOLORKEYSTATE) TexUpdateNativeColorKeyState();
		if (callflags & CALLFLAG_SETPALETTE) TexUpdateTexturePalette();
		if (callflags & CALLFLAG_SETNCCTABLE) TexUpdateTextureNccTable();
		if (callflags & CALLFLAG_SETCOLORKEY) TexUpdateAlphaBasedColorKeyState();
		if (astate.flags & STATE_COMPAREDEPTH) GlideSetStencilMode();

		/*lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_COLOROP,&cop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_COLORARG1,&carg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_COLORARG2,&carg2);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_ALPHAOP,&alphaop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_ALPHAARG1,&aarg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,0,D3DTSS_ALPHAARG2,&aarg2);
		//LOG(0,"\n  stage0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
		//LOG(0,"\n  cache0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[0],astate.colorarg1[0],astate.colorarg2[0],astate.alphaop[0],astate.alphaarg1[0],astate.alphaarg2[0]);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_COLOROP,&cop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_COLORARG1,&carg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_COLORARG2,&carg2);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_ALPHAOP,&alphaop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_ALPHAARG1,&aarg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,1,D3DTSS_ALPHAARG2,&aarg2);
		//LOG(0,"\n  stage1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
		//LOG(0,"\n  cache1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[1],astate.colorarg1[1],astate.colorarg2[1],astate.alphaop[1],astate.alphaarg1[1],astate.alphaarg2[1]);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_COLOROP,&cop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_COLORARG1,&carg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_COLORARG2,&carg2);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_ALPHAOP,&alphaop);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_ALPHAARG1,&aarg1);
		lpD3Ddevice->lpVtbl->GetTextureStageState(lpD3Ddevice,2,D3DTSS_ALPHAARG2,&aarg2);
		//LOG(0,"\n  stage2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
		//LOG(0,"\n  cache2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[2],astate.colorarg1[2],astate.colorarg2[2],astate.alphaop[2],astate.alphaarg1[2],astate.alphaarg2[2]);
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_ALPHABLENDENABLE, &abe);
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_SRCBLEND, &asrc);
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_DESTBLEND, &adst);*/
		
		lpD3Ddevice->lpVtbl->GetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, &clipping);
		if (vbflags & VBFLAG_VBUSED)	{
			lpD3DVertexBuffer->lpVtbl->Unlock(lpD3DVertexBuffer);			
			if (primindexed)	{
				lpD3Ddevice->lpVtbl->DrawIndexedPrimitiveVB(lpD3Ddevice, primType, lpD3DVertexBuffer,
															actoffsetvb, numberOfVertices,
															indices, numberOfIndices, 0);
			} else {
				lpD3Ddevice->lpVtbl->DrawPrimitiveVB(lpD3Ddevice, primType, lpD3DVertexBuffer,
													 actoffsetvb, numberOfVertices, 0);
			}
			if (toomanyvertex)	{
				lpD3DVertexBuffer->lpVtbl->Lock(lpD3DVertexBuffer,DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
												DDLOCK_WAIT | DDLOCK_WRITEONLY |
												DDLOCK_DISCARDCONTENTS, &vbvertex, NULL);
				actoffsetvb = 0;
				actvertex = vbvertex;
			} else {
				lpD3DVertexBuffer->lpVtbl->Lock(lpD3DVertexBuffer,DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
												DDLOCK_WAIT | DDLOCK_WRITEONLY |
												DDLOCK_NOOVERWRITE, &vbvertex, NULL);
				actoffsetvb += numberOfVertices;
			}			
			vbbuffend = vbvertex + DRAW_MAX_VERTEXNUM;

		} else {
			if (primindexed)	{
				lpD3Ddevice->lpVtbl->DrawIndexedPrimitive( lpD3Ddevice, primType,
														D3D_VERTEXTYPE,
														memvertex,
														numberOfVertices,
														indices,
														numberOfIndices,
														0);
			} else lpD3Ddevice->lpVtbl->DrawPrimitive( lpD3Ddevice, primType,
														D3D_VERTEXTYPE,
														memvertex,
														numberOfVertices,
														0);
			actvertex = memvertex;
		}		
		actindex = indices;
		numberOfVertices = numberOfIndices = 0;
		if (astate.flags & STATE_COMPAREDEPTH) GlideUnsetStencilMode();
		/* Az adott primitivek kirajzolásának kényszeritése */
		if (renderbuffer == GR_BUFFER_FRONTBUFFER)	{
			lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
			ShowFrontBuffer();
			lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);
		}
	} else if ( (vbflags & VBFLAG_VBUSED) && toomanyvertex)	{
		lpD3DVertexBuffer->lpVtbl->Unlock(lpD3DVertexBuffer);
		lpD3DVertexBuffer->lpVtbl->Lock(lpD3DVertexBuffer, DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
												DDLOCK_WAIT | DDLOCK_WRITEONLY |
												DDLOCK_DISCARDCONTENTS, &vbvertex, NULL);
		actoffsetvb = 0;
		vbbuffend = (actvertex = vbvertex) + DRAW_MAX_VERTEXNUM;
	}
	/*_asm {
		
		mov ecx,qw; //0x1000000
		ide: loop ide
	}*/
	
	DEBUG_END("DrawFlushPrimitives", DebugRenderThreadId, 1);
}

/*-------------------------------------------------------------------------------------------*/

/*void VertexProcessorSSE(GrVertex **vlist, int nVerts, int generateIndices)	{
float			rcpOOW;
int				j;
unsigned int	mxcsr;

	//DEBUG_BEGIN("VertexProcessorSSE", DebugRenderThreadId, 2);
	
	DEBUG_PERF_MEASURE_BEGIN(&sse_perf);
	if ( (actvertex+nVerts) > vbbuffend) DrawFlushPrimitives(1);
	if ( (numberOfIndices+3*(nVerts-2)) > DRAW_MAX_INDEXNUM) DrawFlushPrimitives(0);
	
	_asm	{
		stmxcsr		mxcsr
		mov			j,9f80h;//0
		ldmxcsr		j
		mov			ebx,vlist
		mov			ecx,nVerts
		mov			edi,actvertex
		lea			ebx,[ebx+4*ecx]
		
		// XY conv
		neg			ecx
		movaps		xmm0,StreamConst0_0
		movaps		xmm7,tscale
		movaps		xmm6,tconst
		movaps		xmm1,xmm0
		movq		mm1,snapMask
	PVnextXY:
		mov			esi,[ebx+4*ecx]
		movlps		xmm0,[esi]GrVertex.x
		cvtps2pi	mm0,xmm0
		pand		mm0,mm1
		cvtpi2ps	xmm1,mm0
		subps		xmm0,xmm1
		add			edi, size D3DTLVERTEX
		mulps		xmm0,xmm7
		inc			ecx
		addps		xmm0,xmm6
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.sx,xmm0		
		jne			PVnextXY

		// ARGB conv
		//mov			eax, astate.vertexmode
		//and			eax,VMODE_DIFFUSE | VMODE_ALPHAFOG
		test		astate.vertexmode, VMODE_DIFFUSE | VMODE_ALPHAFOG
		je			PVnoARGB
		mov			edi,actvertex
		sub			ecx,nVerts
		mov			edx,astate.flags
		//cmp			eax,VMODE_ALPHAFOG
		//je			VPSSE_VMode_OnlyAlpha
		
		//cmp			eax,VMODE_DIFFUSE
		//je			VPSSE_VMode_OnlyDiffuse
		
		movd		mm5,astate.delta0Rgb
		movaps		xmm1,ARGBStreamScaler
		movq		mm6,fogAlphaXORConstant
		punpckldq	mm5,mm5
PVnextARGB:
		movq		mm0,mm5
		and			edx, STATE_DELTA0
		jne			PVARGBdelta0
		mov			esi,[ebx+4*ecx]
		movups		xmm0,[esi]GrVertex.z	;xmm0 = b,g,r,z
		movss		xmm2,[esi]GrVertex.a
		pxor		mm7,mm7
		movss		xmm0,xmm2				;xmm0 = b,g,r,a
		shufps		xmm0,xmm0,00011011b		;xmm0 = a,r,g,b
		mulps		xmm0,xmm1
		cvtps2pi	mm0,xmm0
		movhlps		xmm2,xmm0
		packssdw	mm0,mm7
		cvtps2pi	mm1,xmm2
		packssdw	mm7,mm1
		por			mm0,mm7
		packuswb	mm0,mm0
PVARGBdelta0:
		add			edi, size D3DTLVERTEX
		pxor		mm0,mm6
		inc			ecx
		movq		[edi-size D3DTLVERTEX]D3DTLVERTEX.color,mm0
		jne			PVnextARGB

//VPSSE_VMode_OnlyAlpha:
//		mov			esi,[ebx+4*ecx]
//		movss		xmm2,[esi]GrVertex.a


VPSSE_VMode_OnlyDiffuse:



		// textúra koord.
PVnoARGB:
		test		astate.vertexmode, VMODE_TEXTURE
		je			PVnoTexCoords
		sub			ecx,nVerts
		mov			edi,actvertex
		movlps		xmm7,astate.divx
		movaps		xmm0,StreamConst0_0
		movlhps		xmm7,xmm7
		lea			edx,GrVertex.tmuvtx[0].oow
		test		astate.hints, GR_STWHINT_W_DIFF_TMU0
		jne			PVnextTexCoords
		lea			edx,GrVertex.oow
		//cmovz		edx,GrVertex.oow
PVnextTexCoords:
		mov			esi,[ebx+4*ecx]
		movss		xmm2,[esi+edx]
		movlps		xmm0,[esi]GrVertex.tmuvtx[0].sow
		shufps		xmm2,xmm2,0h
		mulps		xmm2,xmm7			;xmm2 = divy*oow, divx*oow
		add			edi,size D3DTLVERTEX
		divps		xmm0,xmm2			;xmm0 = tow/divy*oow, sow/divx*oow
		inc			ecx
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.tu,xmm0
		jne			PVnextTexCoords
PVnoTexCoords:

		// mélységi puffer
		mov			eax,astate.vertexmode
		sub			ecx,nVerts
		mov			edx,eax
		and			eax,VMODE_DEPTHBUFFMMODESELECT
		mov			edi,actvertex
		cmp			eax,VMODE_WBUFF
		je			VPSSE_WBUFF
		cmp			eax,VMODE_WBUFFEMU
		je			VPSSE_WBUFFEMU

//VPSSE_ZBUFF:
		movaps		xmm0,StreamConst1_0
		movss		xmm7,depthBias_Z
		movss		xmm6,StreamConst_DRAW_WFAR
		movss		xmm5,xmm0
		movaps		xmm4,StreamConst0_0
		lea			eax,GrVertex.tmuvtx[0].oow
		test		astate.flags, STATE_USETMUW
		jne			VPSSE_ZBUFF_Next
		lea			eax,GrVertex.oow
VPSSE_ZBUFF_Next:
		mov			esi,[ebx+4*ecx]
		and			edx, VMODE_TEXTURE
		je			VPSSE_ZBUFF_RHWOK
		movss		xmm0,[esi+eax]
VPSSE_ZBUFF_RHWOK:
		movss		xmm1,[esi]GrVertex.ooz
		addss		xmm1,xmm7
		divss		xmm1,xmm6
		minss		xmm1,xmm5
		add			edi, size D3DTLVERTEX
		maxss		xmm1,xmm4
		unpcklps	xmm1,xmm0
		inc			ecx
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.sz,xmm1
		jne			VPSSE_ZBUFF_Next
		jmp			VPSSE_DepthBuffEnd

VPSSE_WBUFF:
		movaps		xmm7,StreamConst0_5			//0.5
		movaps		xmm0,StreamConstWCONSTANT	//WConstant
		and			edx,VMODE_WUNDEFINED
		jne			PVwBuffNext
		cmp			depthBias_W,0h
		jne			VPSSE_WBUFF_WITH_BIAS
PVwBuffNext:
		and			edx,VMODE_WUNDEFINED
		jne			PVwBuffwUndef
		mov			esi,[ebx+4*ecx]
		movss		xmm0,[esi]GrVertex.oow
PVwBuffwUndef:
		movaps		xmm1,xmm7
		add			edi,size D3DTLVERTEX
		unpcklps	xmm1,xmm0
		inc			ecx
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.sz,xmm1
		jne			PVwBuffNext
		jmp			VPSSE_DepthBuffEnd
VPSSE_WBUFF_WITH_BIAS:
		mov			eax,depthBias_W
VPSSE_WBWB_Next:
		mov			esi,[ebx+4*ecx]
		rcpss		xmm0,[esi]GrVertex.oow
		movss		rcpOOW,xmm0
		add			rcpOOW,eax
		add			edi,size D3DTLVERTEX
		rcpss		xmm0,rcpOOW
		movaps		xmm1,xmm7
		inc			ecx
		unpcklps	xmm1,xmm0
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.sz,xmm1
		jne			VPSSE_WBWB_Next
		jmp			VPSSE_DepthBuffEnd

VPSSE_WBUFFEMU:
		movaps		xmm0,StreamConst0_0
		movaps		xmm7,StreamConst_DRAW_WFAR
		mov			eax,depthBias_W
VPSSE_WBUFFEMU_Next:
		mov			esi,[ebx+4*ecx]
		and			edx,VMODE_WUNDEFINED
		movss		xmm1,[esi]GrVertex.oow
		jne			VPSSE_WBUFFEMU_WUndef
		rcpss		xmm0,xmm1
		movss		rcpOOW,xmm0
		add			rcpOOW,eax
		movss		xmm0,rcpOOW
		divss		xmm0,xmm7
VPSSE_WBUFFEMU_WUndef:			;xmm1=rhw, xmm0=z
		movaps		xmm2,xmm0
		add			edi,size D3DTLVERTEX
		unpcklps	xmm2,xmm1
		inc			ecx
		movlps		[edi - size D3DTLVERTEX]D3DTLVERTEX.sz,xmm2
		jne			VPSSE_WBUFFEMU_Next

VPSSE_DepthBuffEnd:
		// Táblás köd
		cmp			astate.fogmode, GR_FOG_WITH_TABLE
		jne			VPSSE_NoFogWithTable
		sub			ecx,nVerts
		mov			edi,actvertex
		
		mov			esi,WToInd
		movss		xmm7,StreamConst0_0
VPSSE_FogWithTableNext:
		mov			eax,[ebx+4*ecx]
		rcpss		xmm0,[eax]GrVertex.oow
		maxss		xmm0,xmm7
		cvtss2si	eax,xmm0
		movzx		eax, byte ptr [esi+eax]
		movss		xmm1,IndToW[4*eax+4]
		movzx		edx,fogtable[eax]
		movss		xmm5,IndToW[4*eax]
		cvtsi2ss	xmm2,edx
		movzx		edx,fogtable[eax+1]
		movss		xmm6,xmm1
		subss		xmm1,xmm5		;xmm1 = (IndToW[i+1]-IndToW[i])
		cvtsi2ss	xmm3,edx
		subss		xmm6,xmm0		;xmm6 = IndToW[i+1]-w
		subss		xmm0,xmm5		;xmm0 = w-IndToW[i]
		mulss		xmm2,xmm6
		mulss		xmm3,xmm0
		addss		xmm2,xmm3
		add			edi, size D3DTLVERTEX
		divss		xmm2,xmm1
		cvtss2si	eax,xmm2
		shl			eax,24
		inc			ecx
		mov 		[edi - size D3DTLVERTEX]D3DTLVERTEX.specular,eax		
		jne			VPSSE_FogWithTableNext

VPSSE_NoFogWithTable:
		mov			actvertex,edi
		//stmxcsr		j
		ldmxcsr		mxcsr
		emms

	}
	
	if (generateIndices)	{
		for(j = 0; j < nVerts-2; j++)	{
			actindex[0] = numberOfVertices;
			actindex[1] = numberOfVertices + j + 1;
			actindex[2] = numberOfVertices + j + 2;
			actindex += 3;
		}
		numberOfIndices += 3*(nVerts-2);
	}
	numberOfVertices += nVerts;
	
	if (renderbuffer == GR_BUFFER_FRONTBUFFER) DrawFlushPrimitives(0);
	stats.trisProcessed += nVerts-2;

	DEBUG_PERF_MEASURE_END(&sse_perf);
	//DEBUG_END("VertexProcessorSSE", DebugRenderThreadId, 2);
}*/


void VertexProcessorFPU(GrVertex **vlist, int nVerts, int generateIndices)	{
int				i, j;
float			w, tmuoow;
GrVertex		*vertex;
unsigned int	CTW, NEWCTW;

	DEBUG_BEGIN("VertexProcessorFPU", DebugRenderThreadId, 3);

	DEBUG_PERF_MEASURE_BEGIN(&fpu_perf);
	if ( (actvertex + nVerts) > vbbuffend) DrawFlushPrimitives(1);
	if ( (generateIndices) && ((numberOfIndices + 3*(nVerts - 2)) > DRAW_MAX_INDEXNUM)) DrawFlushPrimitives(0);
	
	_asm	{
		fstcw	[CTW]
		mov		eax,CTW
		and		eax,0E0C0h
		or		eax,00C3Fh
		mov		[NEWCTW],eax
		fldcw	[NEWCTW]
	}

	for(j = 0; j < nVerts; j++)	{
		vertex = vlist[j];

		if (noVertexConverting) {
			actvertex->sx = vertex->x * xScale + xConst;
			actvertex->sy = vertex->y * yScale + yConst;
		} else {
			ConvertVertex (vertex, actvertex);
		}

		if (astate.vertexmode & (VMODE_DIFFUSE | VMODE_ALPHAFOG))	{
			actvertex->specular = (
				actvertex->color = (astate.flags & STATE_DELTA0) ? astate.delta0Rgb : GetARGB(vertex)
			) ^ 0xFF000000;
		}

		if (astate.vertexmode & VMODE_TEXTURE)	{
			tmuoow = (astate.hints & GR_STWHINT_W_DIFF_TMU0) ? vertex->tmuvtx[0].oow : vertex->oow;
			actvertex->tu = vertex->tmuvtx[0].sow / (astate.divx * tmuoow );
			actvertex->tv = vertex->tmuvtx[0].tow / (astate.divy * tmuoow );
		}
		switch(astate.vertexmode & VMODE_DEPTHBUFFMMODESELECT)	{
			case (VMODE_ZBUFF):
				if (astate.vertexmode & VMODE_TEXTURE)	{
					actvertex->rhw = (astate.flags & STATE_USETMUW) ? vertex->tmuvtx[0].oow : vertex->oow;
				} else actvertex->rhw = 1.0f;
				actvertex->sz = ZCLAMP((vertex->ooz + depthBias_Z) / DRAW_WFAR);
				break;
			case (VMODE_WBUFFEMU):
				actvertex->sz = (!(astate.vertexmode & VMODE_WUNDEFINED)) ? AddWEmuBias(actvertex->rhw = vertex->oow) : 0.0f;
				break;
			case (VMODE_WBUFF):
				if (!(astate.vertexmode & VMODE_WUNDEFINED))	{
					AddWBias(vertex->oow, &actvertex->rhw);
				} else actvertex->rhw = WCONSTANT;
				actvertex->sz = 0.5f;
				break;			
		}
		if (astate.fogmode == GR_FOG_WITH_TABLE)	{
			i = WToInd[GetFogIndexValue(vertex->oow, &w)];
			actvertex->specular = GetFogIntValue ( ((w-IndToW[i]) * fogtable[i+1] + (IndToW[i+1]-w) * fogtable[i])/(IndToW[i+1]-IndToW[i]) );
		}
		actvertex++;
	}
	
	if (generateIndices)	{
		_asm	{
			mov		edx,nVerts
			mov		ecx,numberOfVertices
			mov		edi,actindex
			lea		edx,[edx-2]
			lea		eax,[ecx+1]
			nop
		_nextTriangleIndices:
			mov		[edi],cx
			lea		edi,[edi+6]
			mov		[edi-4],ax
			inc		eax
			mov		[edi-2],ax
			nop
			dec		edx
			jnz		_nextTriangleIndices
			mov		actindex,edi
		}
		/*for(j = 0; j < nVerts-2; j++)	{
			actindex[0] = numberOfVertices;
			actindex[1] = numberOfVertices + j + 1;
			actindex[2] = numberOfVertices + j + 2;
			actindex += 3;
		}*/
		numberOfIndices += 3 * (nVerts - 2);
	}
	numberOfVertices += nVerts;
	
	if (renderbuffer == GR_BUFFER_FRONTBUFFER) DrawFlushPrimitives(0);
	stats.trisProcessed += nVerts - 2;

	_asm	fldcw	[CTW]

	DEBUG_PERF_MEASURE_END(&fpu_perf);
	
	DEBUG_END("VertexProcessorFPU", DebugRenderThreadId, 3);
}	


/* Háromszög vágás nélkül */
void EXPORT grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )	{
			
	DEBUG_BEGIN("grDrawTriangle",DebugRenderThreadId, 4);
	
	LOG(0,"\ngrDrawTriangle(a, b, c)");
	LOG(0,"\n	a.x=%f, a.y=%f, b.x=%f, b.y=%f, c.x=%f, c.y=%f",a->x, a->y, b->x, b->y, c->x, c->y);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;

	if (config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) PrimTypeToClippedIndexedTriangleList();
	else PrimTypeToClippedTriangleList();

	vertexPointers[0] = (GrVertex *) a;
	vertexPointers[1] = (GrVertex *) b;
	vertexPointers[2] = (GrVertex *) c;
	(*vertexProcessor)(vertexPointers, 3, config.Flags2 & CFG_ALWAYSINDEXEDPRIMS);
	//VertexProcessorFPU(vertexPointers, 3, 0);
	//VertexProcessorSSE(vertexPointers, 3, 0);
	
	DEBUG_END("grDrawTriangle",DebugRenderThreadId, 4);
}


void EXPORT grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] )	{
int	i;

	DEBUG_BEGIN("grDrawPolygonVertexList", DebugRenderThreadId, 5);

	LOG(0,"\ngrDrawPolygonVertexList(nVerts=%d,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);
	
	DEBUG_END("grDrawPolygonVertexList", DebugRenderThreadId, 5);
}


void EXPORT guDrawPolygonVertexListWithClip( int nVerts, const GrVertex vlist[] )	{
int	i;

	DEBUG_BEGIN("guDrawPolygonVertexListWithClip", DebugRenderThreadId, 6);
	
	LOG(0,"\nguDrawPolygonVertexListWithClip(nVerts=%d,vlist)", nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{		
		//GlideDrawTriangleWithClip(a,b,c);
		return;
	} else {
		if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != TRUE) )	{	
			DrawFlushPrimitives(0);
			primType = D3DPT_TRIANGLELIST;
			primindexed = TRUE;
		}
		if (primclipping != TRUE)	{
			DrawFlushPrimitives(0);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, TRUE);
			primclipping = TRUE;
			if (vbflags & VBFLAG_VBAVAILABLE)	{
				vbflags &= ~VBFLAG_VBUSED;
				vbbuffend = (actvertex = memvertex) + DRAW_MAX_VERTEXNUM;
			}
			noVertexConverting = 1;
		}
		for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
		(*vertexProcessor)(vertexPointers, nVerts, 1);
		//VertexProcessorFPU(vertexPointers, nVerts, 1);
		//VertexProcessorSSE(vertexPointers, nVerts, 1);
	}
	
	DEBUG_END("guDrawPolygonVertexListWithClip", DebugRenderThreadId, 6);
}


void EXPORT grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] )	{
int	i;
	
	DEBUG_BEGIN("grDrawPlanarPolygonVertexList", DebugRenderThreadId, 7);
	
	LOG(0,"\ngrDrawPlanarPolygonVertexList(nVerts=%d,vlist)");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);
	
	DEBUG_END("grDrawPlanarPolygonVertexList", DebugRenderThreadId, 7);
}


void EXPORT grDrawPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
int	i;

	DEBUG_BEGIN("grDrawPolygon", DebugRenderThreadId, 8);

	LOG(0,"\ngrDrawPolygon(nVerts=%d,ilist,vlist)", nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);

	DEBUG_END("grDrawPolygon",DebugRenderThreadId, 8);
}


void EXPORT grDrawPlanarPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
int	i;

	DEBUG_BEGIN("grDrawPlanarPolygon",DebugRenderThreadId, 9);
	
	LOG(0,"\ngrDrawPlanarPolygon(nVerts=%d,ilist,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);
	
	DEBUG_END("grDrawPlanarPolygon",DebugRenderThreadId, 9);
}


void EXPORT grAADrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c,
							 FxBool antialiasAB, FxBool antialiasBC, FxBool antialiasCA)	{
	
	DEBUG_BEGIN("grAADrawTriangle",DebugRenderThreadId, 10);
	
	LOG(0,"\ngrAADrawTriangle(a,b,c,aaAB=%s,aaBC=%s,aaCA=%s)",
		fxbool_t[antialiasAB],fxbool_t[antialiasBC],fxbool_t[antialiasCA]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;

	if (config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) PrimTypeToClippedIndexedTriangleList();
	else PrimTypeToClippedTriangleList();
	
	/*if ( (antialias & (AAFLAG_TRIANGLEAVAILABLE | AAFLAG_TRIANGLEENABLED)) == AAFLAG_TRIANGLEAVAILABLE)	{
		DrawFlushPrimitives(0);
		//lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_ANTIALIAS,D3DANTIALIAS_SORTINDEPENDENT);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_EDGEANTIALIAS,TRUE);
		antialias|=AAFLAG_TRIANGLEENABLED;
	}*/
	vertexPointers[0] = a;
	vertexPointers[1] = b;
	vertexPointers[2] = c;
	(*vertexProcessor)(vertexPointers, 3, config.Flags2 & CFG_ALWAYSINDEXEDPRIMS);
	//VertexProcessorFPU(vertexPointers, 3, 0);
	//VertexProcessorSSE(vertexPointers, 3, 0);
	
	DEBUG_END("grAADrawTriangle",DebugRenderThreadId, 10);
}


void EXPORT grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[]) {
int	i;

	DEBUG_BEGIN("grAADrawPolygon",DebugRenderThreadId, 11);

	LOG(0,"\ngrAADrawPolygon(nVerts=%d,ilist,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);
	
	DEBUG_END("grAADrawPolygon",DebugRenderThreadId, 11);
}


void EXPORT grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[]) {
int	i;
	
	DEBUG_BEGIN("grAADrawPolygonVertexList",DebugRenderThreadId, 12);

	LOG(0,"\ngrDrawPolygonVertexList(nVerts=%d,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	PrimTypeToClippedIndexedTriangleList();

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	(*vertexProcessor)(vertexPointers, nVerts, 1);
	//VertexProcessorFPU(vertexPointers, nVerts, 1);
	//VertexProcessorSSE(vertexPointers, nVerts, 1);
	
	DEBUG_END("grAADrawPolygonVertexList",DebugRenderThreadId, 12);
}


/*int clipEdge(GrVertex *inside, GrVertex *outside, GrVertex *output, int clipedge)	{
//float t;
double t;
//float x,y;
double x,y;
double ix,iy;
double ox,oy;
int clip;

	clip=clipedge;
	ix=inside->x;
	iy=inside->y;
	ox=outside->x;
	oy=outside->y;
	switch(clipedge)	{
		case CF_LEFT:
			//t=( (x=astate.minx)-ix )/(ox- ix);
			t=(outside->oow+ox)/(-ix+ox-inside->oow+outside->oow);			
			x=ox+t*(ix-ox);
			y=oy+t*(iy-oy);
			output->oow=-x;
			//output->oow = (double) outside->oow + t*(inside->oow - outside->oow);
			//x=-output->oow;
			//y=iy + t*( oy-iy);
			if (y>output->oow) clip|=CF_BOTTOM;
			if (y<(-output->oow)) clip|=CF_TOP;
			//if (y>=astate.miny) clip|=CF_TOP;
			//if (y<=astate.maxy) clip|=CF_BOTTOM;
			break;
		case CF_RIGHT:
			//t=( (x=astate.maxx)-ix )/(ox- ix);
			t=(outside->oow-ox)/(ix-ox-inside->oow+outside->oow);
			x=ox+t*(ix-ox);
			y=oy+t*(iy-oy);
			output->oow=x;
			//output->oow = (double) outside->oow + t*(inside->oow - outside->oow);
			//x=output->oow;
			//y=oy+t*(iy-oy);
			if (y>output->oow) clip|=CF_BOTTOM;
			if (y<(-output->oow)) clip|=CF_TOP;
			//y=iy + t*(oy-iy);
			//if (y>=astate.miny) clip|=CF_TOP;
			//if (y<=astate.maxy) clip|=CF_BOTTOM;
			break;
		case CF_TOP:
			t=(outside->oow+oy)/(-iy+oy-inside->oow+outside->oow);
			y=oy+t*(iy-oy);
			x=ox+t*(ix-ox);
			output->oow=-y;
			//output->oow = (double) outside->oow + t*(inside->oow - outside->oow);
			//y=-output->oow;
			//t=( (y=astate.miny)-iy )/(oy-iy);			
			
			if (x>output->oow) clip|=CF_RIGHT;
			if (x<(-output->oow)) clip|=CF_LEFT;
			//x=ix + t*(ox-ix);
			//if (x>=astate.minx) clip|=CF_LEFT;
			//if (x<=astate.maxx) clip|=CF_RIGHT;
			break;
		case CF_BOTTOM:
			t=(outside->oow-oy)/(iy-oy-inside->oow+outside->oow);
			y=oy+t*(iy-oy);
			x=ox+t*(ix-ox);
			output->oow=y;

			//output->oow = (double) outside->oow + t*(inside->oow - outside->oow);
			//y=output->oow;			
			//x=ox+t*(ix-ox);
			if (x>output->oow) clip|=CF_RIGHT;
			if (x<(-output->oow)) clip|=CF_LEFT;
			//t=( (y=astate.maxy)-iy )/(oy-iy);
			//x=ix + t*(ox-ix);
			//if (x>=astate.minx) clip|=CF_LEFT;
			//if (x<=astate.maxx) clip|=CF_RIGHT;
			break;
	}
	output->x=x;
	output->y=y;
	output->r = outside->r + t*(inside->r - outside->r);
	output->g = outside->g + t*(inside->g - outside->g);
	output->b = outside->b + t*(inside->b - outside->b);
	output->a = outside->a + t*(inside->a - outside->a);
	output->ooz = outside->ooz + t*(inside->ooz - outside->ooz);	
	output->tmuvtx[0].sow = outside->tmuvtx[0].sow + t*(inside->tmuvtx[0].sow - outside->tmuvtx[0].sow);
	output->tmuvtx[0].tow = outside->tmuvtx[0].tow + t*(inside->tmuvtx[0].tow - outside->tmuvtx[0].tow);
	return(clip);
}*/

/*void GlideDrawTriangleWithClip(const GrVertex *a, const GrVertex *b, const GrVertex *c ) {
GrVertex *list1[8];
GrVertex *list2[8];
GrVertex allocv[4*6];
GrVertex out[6];
GrVertex *v, **inlist, **outlist;
int clipst1[8];
int clipst2[8];
int *inclips, *outclips;
int inlen, outlen;
int clipstatusor, clipstatusand, temp;
int i,j,clip;
int allocindex;

    list1[0]=list1[3]=a;
	list1[1]=b;
	list1[2]=c;	
	allocv[0]=*a;
	allocv[1]=*b;
	allocv[2]=*c;
	list1[0]=list1[3]=allocv;
	list1[1]=allocv+1;
	list1[2]=allocv+2;
	clipstatusor=0;
	clipstatusand=CF_LEFT | CF_RIGHT | CF_BOTTOM | CF_TOP;
	// Clip flagek elõállitása a csúcspontokra
	for(i=0;i<3;i++)	{
		v=list1[i];
		v->x=(v->x-(astate.maxx/2))/(v->oow*(astate.maxx/2));
		v->y=(v->y-(astate.maxy/2))/(v->oow*(astate.maxy/2));
		v->tmuvtx[0].sow/=v->oow;
		v->tmuvtx[0].tow/=v->oow;
		v->oow=1/v->oow;

		temp=0;
		if (v->x > v->oow) temp|=CF_RIGHT;
		if (v->x < -v->oow) temp|=CF_LEFT;
		if (v->y > v->oow) temp|=CF_BOTTOM;
		if (v->y < -v->oow) temp|=CF_TOP;
		clipst1[i]=temp;
		clipstatusor|=temp;
		clipstatusand&=temp;
	}	
	// clipstatusand != 0: a sokszög triviálisan eldobható
	if (clipstatusand!=0) return;
	// clipstatusor == 0: a sokszög triviálisan a vágási régión belül van
	allocindex=3;
	inlist=inclips=NULL;	
	if (clipstatusor)	{
		inlen=3;		
		temp=1;		
		// Mind a négy vágóélen végigmegyûnk
		for(i=0;i<4;i++)	{
			// Az adott vágóéllel kell a sokszög valamelyik élét vágnunk?
			if (clipstatusor & temp)	{				
				if (inlist==list1)	{
					inlist=list2;
					outlist=list1;
					inclips=clipst2;
					outclips=clipst1;
				} else {
					inlist=list1;
					outlist=list2;	
					inclips=clipst1 ;
					outclips=clipst2;
				}
				inclips[inlen]=inclips[0];
				outlen=0;
				// Végigmegyünk az egyes éleken
				if (!(inclips[0] & temp))	{
					outclips[outlen]=inclips[0];
					outlist[outlen++]=inlist[0];
				}
				for(j=0;j<inlen;j++)	{										
					if (!(inclips[j] & temp))	{
						if (!(inclips[j+1] & temp))	{							
							outclips[outlen]=inclips[j+1];
							outlist[outlen++]=inlist[j+1];							
						} else {
							clip=clipEdge(inlist[j], inlist[j+1], allocv+allocindex,temp);							
							outclips[outlen]=inclips[j+1] & (~clip);
							outlist[outlen++]=allocv+allocindex;
							allocindex++;
						}
					} else {
						if (!(inclips[j+1] & temp))	{
							clip=clipEdge(inlist[j+1], inlist[j], allocv+allocindex,temp);
							outclips[outlen]=inclips[j] & (~clip);
							outclips[outlen+1]=inclips[j+1];
							outlist[outlen++]=allocv+allocindex;
							outlist[outlen++]=inlist[j+1];
							allocindex++;
						}
					}					
				}
				if (outlist[0]!=outlist[outlen-1])	{
					outclips[outlen]=outclips[0];
					outlist[outlen++]=outlist[0];
				}
				inlen=outlen-1;
			}
			temp<<=1;			
		}
		//for(i=0;i<outlen-1;i++) if (outlist[i]->oow<0.0f)	{
		//		outlist[i]->tmuvtx[0].sow/=outlist[i]->oow;
		//		outlist[i]->tmuvtx[0].tow/=outlist[i]->oow;
		//		outlist[i]->oow=1.0f/0.01f;
		//		outlist[i]->tmuvtx[0].sow*=outlist[i]->oow;
		//		outlist[i]->tmuvtx[0].tow*=outlist[i]->oow;
		//	}
		for(i=0;i<outlen-1;i++)	{
			out[i]=*(outlist[i]);
			out[i].x=( ((outlist[i])->x/(outlist[i])->oow)+1.0f)*astate.maxx/2;
			out[i].y=( ((outlist[i])->y/(outlist[i])->oow)+1.0f)*astate.maxy/2;			
			out[i].tmuvtx[0].sow/=out[i].oow;
			out[i].tmuvtx[0].tow/=out[i].oow;
			out[i].oow=1/out[i].oow;
		}
		//for(i=0;i<outlen-1;i++) out[i]=*(outlist[i]);
		grDrawPolygonVertexList(outlen-1,&out);
	} else grDrawTriangle(a,b,c);
}*/


int clipEdge(GrVertex *inside, GrVertex *outside, GrVertex *output, int clipedge, int clipflags)	{
float t;
float x,y;
float ix,iy;
float ox,oy;
	
	ix=inside->x;
	iy=inside->y;
	ox=outside->x;
	oy=outside->y;
	clipflags&=~clipedge;
	switch(clipedge)	{
		case CF_LEFT:
			t = (ix - (x = (float) astate.minx)) / (ix - ox);
			clipflags &= ~(CF_TOP | CF_BOTTOM);
			if (t >= 1.0f)	{
				t = 1.0f;
				y = oy;
			} else y = iy + t * (oy - iy);
			if (y <= (float) astate.miny) clipflags |= CF_TOP;
			if (y >= (float) astate.maxy) clipflags |= CF_BOTTOM;
			break;
		case CF_RIGHT:
			t = (ix - (x = (float) astate.maxx)) / (ix - ox);
			clipflags &= ~(CF_TOP | CF_BOTTOM);			
			if (t >= 1.0f)	{
				t = 1.0f;
				y = oy;
			} else y = iy + t * (oy - iy);
			if (y <= (float) astate.miny) clipflags |= CF_TOP;
			if (y >= (float) astate.maxy) clipflags |= CF_BOTTOM;
			break;
		case CF_TOP:
			t = (iy - (y = (float) astate.miny)) / (iy - oy);
			clipflags &= ~(CF_LEFT | CF_RIGHT);
			if (t >= 1.0f)	{
				t = 1.0f;
				x = ox;
			} else x = ix + t * (ox - ix);			
			if (x <= (float) astate.minx) clipflags |= CF_LEFT;
			if (x >= (float) astate.maxx) clipflags |= CF_RIGHT;
			break;
		case CF_BOTTOM:
			t = (iy - (y = (float) astate.maxy)) / (iy - oy);
			clipflags &= ~(CF_LEFT | CF_RIGHT);
			if (t >= 1.0f)	{
				t = 1.0f;
				x = ox;
			} else x = ix + t * (ox - ix);			
			if (x <= (float) astate.minx) clipflags |= CF_LEFT;
			if (x >= (float) astate.maxx) clipflags |= CF_RIGHT;
			break;
		/*case CF_WNEAR:
			t = ((1 / inside->oow) - CLIP_WNEAR ) / ((1 / inside->oow) - (1 / outside->oow));
			x = ix + t * (ox - ix);
			y = ix + t * (ox - ix);
			output->oow = 1 / CLIP_WNEAR;*/
			/*if (x>=astate.minx) clip|=CF_LEFT;
			if (x<=astate.maxx) clip|=CF_RIGHT;
			if (y>=astate.miny) clip|=CF_TOP;
			if (y<=astate.maxy) clip|=CF_BOTTOM;*/
			break;
	}
	output->x = x;
	output->y = y;
	output->r = inside->r + t*(outside->r - inside->r);
	output->g = inside->g + t*(outside->g - inside->g);
	output->b = inside->b + t*(outside->b - inside->b);
	output->a = inside->a + t*(outside->a - inside->a);
	output->ooz = inside->ooz + t*(outside->ooz - inside->ooz);	
	output->oow = inside->oow + t*(outside->oow - inside->oow);
	output->tmuvtx[0].sow = inside->tmuvtx[0].sow + t*(outside->tmuvtx[0].sow - inside->tmuvtx[0].sow);
	output->tmuvtx[0].tow = inside->tmuvtx[0].tow + t*(outside->tmuvtx[0].tow - inside->tmuvtx[0].tow);
	return(clipflags);
}


void GlideDrawTriangleWithClip(const GrVertex *a, const GrVertex *b, const GrVertex *c ) {
GrVertex	*list1[16];
GrVertex	*list2[16];
GrVertex	allocv[5*6];
GrVertex	out[16];
GrVertex	*v, **inlist, **outlist;
int			clipst1[16];
int			clipst2[16];
int			*inclips, *outclips;
int			inlen, outlen;
int			clipstatusor, clipstatusand, temp;
int			i,j,clip;
int			allocindex;

   	list1[0] = list1[3] = (GrVertex *) a;
	list1[1] = (GrVertex *) b;
	list1[2] = (GrVertex *) c;	
	clipstatusor = 0;
	clipstatusand = CF_LEFT | CF_RIGHT | CF_BOTTOM | CF_TOP;
	// Clip flagek elõállitása a csúcspontokra
	for(i = 0; i < 3; i++)	{
		v = list1[i];		
		temp = 0;
		if (!IsNotInf(v->x) || !IsNotInf(v->y) ) return;
		if (v->x > astate.maxx) temp |= CF_RIGHT;
		if (v->x < astate.minx) temp |= CF_LEFT;
		if (v->y > astate.maxy) temp |= CF_BOTTOM;
		if (v->y < astate.miny) temp |= CF_TOP;
		//if ( (1/v->oow) < CLIP_WNEAR) return;//temp|=CF_WNEAR;
		clipst1[i] = temp;
		clipstatusor |= temp;
		clipstatusand &= temp;
	}	
	// clipstatusand != 0: a sokszög triviálisan eldobható
	if (clipstatusand != 0) return;
	// clipstatusor == 0: a sokszög triviálisan a vágási régión belül van
	allocindex = 0;
	inlist = NULL;
	inclips = NULL;	
	if (clipstatusor)	{
		inlen = 3;		
		temp = 1;		
		// Mind a négy vágóélen végigmegyünk
		for(i = 0; i < 4; i++)	{
			// Az adott vágóéllel kell a sokszög valamelyik élét vágnunk?
			if (clipstatusor & temp)	{				
				if (inlist == list1)	{
					inlist = list2;
					outlist = list1;
					inclips = clipst2;
					outclips = clipst1;
				} else {
					inlist = list1;
					outlist = list2;	
					inclips = clipst1 ;
					outclips = clipst2;
				}
				inclips[inlen] = inclips[0];
				outlen = 0;
				// Végigmegyünk az egyes éleken
				if (!(inclips[0] & temp))	{
					outclips[outlen] = inclips[0];
					outlist[outlen++] = inlist[0];
				}
				for(j = 0; j < inlen;j++)	{
					if (!(inclips[j] & temp))	{
						if (!(inclips[j + 1] & temp))	{							
							outclips[outlen] = inclips[j + 1];
							outlist[outlen++] = inlist[j + 1];
						} else {
							clip = clipEdge(inlist[j], inlist[j + 1], allocv + allocindex, temp, inclips[j + 1]);
							//outclips[outlen]=inclips[j+1] & (~clip);
							outclips[outlen] = clip;
							outlist[outlen++] = allocv + allocindex;
							allocindex++;
						}
					} else {
						if (!(inclips[j+1] & temp))	{
							clip = clipEdge(inlist[j + 1], inlist[j], allocv + allocindex, temp, inclips[j]);
							//outclips[outlen]=inclips[j] & (~clip);
							outclips[outlen] = clip;
							outclips[outlen+1] = inclips[j + 1];
							outlist[outlen++] = allocv + allocindex;
							outlist[outlen++] = inlist[j + 1];
							allocindex++;
						}
					}					
				}
				if (outlist[0] != outlist[outlen-1])	{
					outclips[outlen] = outclips[0];
					outlist[outlen++] = outlist[0];
				}
				inlen = outlen - 1;
			}
			temp <<= 1;			
		}		
		for(i = 0; i < outlen - 1; i++)	{
			out[i] = *(outlist[i]);
		}
		grDrawPolygonVertexList(outlen - 1, (const GrVertex *) &out);
	} else grDrawTriangle(a,b,c);
}


/* Háromszög vágással */
void EXPORT guDrawTriangleWithClip( const GrVertex *a, const GrVertex *b, const GrVertex *c )	{
DWORD	reqIndexing;
	
	DEBUG_BEGIN("guDrawTriangleWithClip",DebugRenderThreadId, 13);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	LOG(0,"\nguDrawTriangleWithClip(a,b,c)");

	if (astate.vertexmode & VMODE_NODRAW) return;
	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{
		GlideDrawTriangleWithClip(a, b, c);
	} else {
	//grDrawTriangle(a,b,c);
	//return;			
		reqIndexing = config.Flags2 & CFG_ALWAYSINDEXEDPRIMS ? TRUE : FALSE;

		if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != reqIndexing) )	{
			DrawFlushPrimitives(0);
			primType = D3DPT_TRIANGLELIST;
			primindexed = reqIndexing;
		}
		if (primclipping!=TRUE)	{
			DrawFlushPrimitives(0);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_CLIPPING,TRUE);
			primclipping=TRUE;
			if (vbflags & VBFLAG_VBAVAILABLE)	{
				vbflags&=~VBFLAG_VBUSED;			
				vbbuffend=(actvertex=memvertex)+DRAW_MAX_VERTEXNUM;
			}
			noVertexConverting = 1;
		}
		vertexPointers[0] = (GrVertex *) a;
		vertexPointers[1] = (GrVertex *) b;
		vertexPointers[2] = (GrVertex *) c;
		(*vertexProcessor)(vertexPointers, 3, config.Flags2 & CFG_ALWAYSINDEXEDPRIMS);
		//VertexProcessorFPU(vertexPointers, 3, 0);
		//VertexProcessorSSE(vertexPointers, 3, 0);
	}

	DEBUG_END("guDrawTriangleWithClip",DebugRenderThreadId, 13);
}


void EXPORT guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc )	{
DWORD	reqIndexing;

	DEBUG_BEGIN("guAADrawTriangleWithClip",DebugRenderThreadId, 14);

	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	LOG(0,"\nguAADrawTriangleWithClip(a,b,c)");
	
	if (astate.vertexmode & VMODE_NODRAW) return;
	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{
		GlideDrawTriangleWithClip(va,vb,vc);
		return;
	} else {	
		//grAADrawTriangle(va,vb,vc,0,0,0);
		//return;			
		reqIndexing = config.Flags2 & CFG_ALWAYSINDEXEDPRIMS ? TRUE : FALSE;
		if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != reqIndexing) )	{
			DrawFlushPrimitives(0);
			primType = D3DPT_TRIANGLELIST;
			primindexed = reqIndexing;
		}
		if (primclipping != TRUE)	{
			DrawFlushPrimitives(0);
			lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, TRUE);
			primclipping = TRUE;
			if (vbflags & VBFLAG_VBAVAILABLE)	{
				vbflags &= ~VBFLAG_VBUSED;			
				vbbuffend = (actvertex=memvertex) + DRAW_MAX_VERTEXNUM;
			}
			noVertexConverting = 1;
		}
		vertexPointers[0] = (GrVertex *) va;
		vertexPointers[1] = (GrVertex *) vb;
		vertexPointers[2] = (GrVertex *) vc;
		(*vertexProcessor)(vertexPointers, 3, config.Flags2 & CFG_ALWAYSINDEXEDPRIMS);
		//VertexProcessorFPU(vertexPointers, 3, 0);
		//VertexProcessorSSE(vertexPointers, 3, 0);
	}	
	
	DEBUG_END("guAADrawTriangleWithClip",DebugRenderThreadId, 14);
}

/*-------------------------------------------------------------------------------------------*/
/* Vonalak */

void EXPORT grDrawLine(GrVertex *a, GrVertex *b) {
float		vectx, vecty, len;
GrVertex	v[4];
float		a_x,a_y,b_x,b_y;
int			numOfLineVertices;
DWORD		reqIndexing;

	//return;
	DEBUG_BEGIN("grDrawLine",DebugRenderThreadId, 15);

	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrDrawLine(a,b)");
	if (primclipping != FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, FALSE);
		primclipping = FALSE;
		GlideSwitchToVBIfAvailable();
		noVertexConverting = 0;
	}
	/* Ha a felbontás miatt vastagabb vonalakat kell rajzolnunk háromszögekbôl */
	if (linemode)	{
		if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != TRUE) )	{
			DrawFlushPrimitives(0);
			primType = D3DPT_TRIANGLELIST;
			primindexed = TRUE;
		}
		ConvertVertexCustom (a, &a_x, &a_y);
		ConvertVertexCustom (b, &b_x, &b_y);
		len = (float) (3*sqrt( SQR(a_x - b_x) + SQR(a_y - b_y) ));
		vectx = -linescalex*(a_y - b_y) / len;
		vecty = linescaley*(a_x - b_x) / len;
		v[0].x = a_x + vectx;
		v[0].y = a_y + vecty;
		v[1].x = b_x + vectx;
		v[1].y = b_y + vecty;
		v[2].x = a_x - vectx;
		v[2].y = a_y - vecty;
		v[3].x = b_x - vectx;
		v[3].y = b_y - vecty;
		v[0].r = v[2].r = a->r; v[0].g = v[2].g = a->g; v[0].b = v[2].b = a->b; v[0].a = v[2].a = a->a;
		v[1].r = v[3].r = b->r; v[1].g = v[3].g = b->g; v[1].b = v[3].b = b->b; v[1].a = v[3].a = b->a;
		v[0].ooz = v[2].ooz = a->ooz; v[0].oow = v[2].oow = a->oow;
		v[1].ooz = v[3].ooz = b->ooz; v[1].oow = v[3].oow = b->oow;
		v[0].tmuvtx[0].sow = v[2].tmuvtx[0].sow = a->tmuvtx[0].sow;
		v[0].tmuvtx[0].tow = v[2].tmuvtx[0].tow = a->tmuvtx[0].tow;
		v[1].tmuvtx[0].sow = v[3].tmuvtx[0].sow = b->tmuvtx[0].sow;
		v[1].tmuvtx[0].tow = v[3].tmuvtx[0].tow = b->tmuvtx[0].tow;
		v[0].tmuvtx[1].sow = v[2].tmuvtx[1].sow = a->tmuvtx[1].sow;
		v[0].tmuvtx[1].tow = v[2].tmuvtx[1].tow = a->tmuvtx[1].tow;
		v[1].tmuvtx[1].sow = v[3].tmuvtx[1].sow = b->tmuvtx[1].sow;
		v[1].tmuvtx[1].tow = v[3].tmuvtx[1].tow = b->tmuvtx[1].tow;
		/* Mindkét körüljárási irányban rajzolunk */
		vertexPointers[0] = v + lineInd1[0];
		vertexPointers[1] = v + lineInd1[1];
		vertexPointers[2] = v + lineInd1[2];
		vertexPointers[3] = v + lineInd1[3];
		numOfLineVertices = 4;
		if (astate.gcullmode != GR_CULL_DISABLE)	{
			vertexPointers[4] = v + lineInd2[0];
			vertexPointers[5] = v + lineInd2[1];
			vertexPointers[6] = v + lineInd2[2];
			vertexPointers[7] = v + lineInd2[3];
			numOfLineVertices = 8;
		}
		(*vertexProcessor)(vertexPointers, numOfLineVertices, 1);
		//VertexProcessorFPU(vertexPointers, numOfLineVertices, 1);
		//VertexProcessorSSE(vertexPointers, numOfLineVertices, 1);
		return;
	}
	reqIndexing = FALSE; //config.Flags2 & CFG_ALWAYSINDEXEDPRIMS ? TRUE : FALSE;
	
	if ( (primType != D3DPT_LINELIST) || (primindexed != reqIndexing) )	{
		DrawFlushPrimitives(0);
		primType = D3DPT_LINELIST;
		primindexed = reqIndexing;
	}
	
	if (reqIndexing)	{
		if (numberOfIndices + 2 > DRAW_MAX_INDEXNUM) DrawFlushPrimitives(0);
		actindex[0] = numberOfVertices;
		actindex[1] = numberOfVertices + 1;
		actindex += 2;
		numberOfIndices += 2;
	}

	vertexPointers[0] = a;
	vertexPointers[1] = b;
	(*vertexProcessor)(vertexPointers, 2, 0);
	
	//VertexProcessorFPU(vertexPointers, 2, 0);
	//VertexProcessorSSE(vertexPointers, 2, 0);
	
	DEBUG_END("grDrawLine",DebugRenderThreadId, 15);
}


void EXPORT grAADrawLine(GrVertex *a, GrVertex *b) {
float vectx, vecty, len;
GrVertex v[4];
float a_x,a_y,b_x,b_y;
int numOfLineVertices;
DWORD reqIndexing;

	//return;
	DEBUG_BEGIN("grAADrawLine", DebugRenderThreadId, 16);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrAADrawLine(a,b)");
	if (primclipping!=FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_CLIPPING,FALSE);
		primclipping=FALSE;
		GlideSwitchToVBIfAvailable();
		noVertexConverting = 0;
	}
	/* Ha a felbontás miatt vastagabb vonalakat kell rajzolnunk háromszögekbôl */
	if (linemode)	{
		if ( (primType != D3DPT_TRIANGLELIST) || (primindexed != TRUE) )	{
			DrawFlushPrimitives(0);
			primType = D3DPT_TRIANGLELIST;
			primindexed = TRUE;
		}
		ConvertVertexCustom (a, &a_x, &a_y);
		ConvertVertexCustom (b, &b_x, &b_y);
		
		len = (float) (3*sqrt( SQR(a_x - b_x) + SQR(a_y - b_y) ));
		vectx = -linescalex*(a_y - b_y) / len;
		vecty = linescaley*(a_x - b_x) / len;
		v[0].x = a_x+vectx;
		v[0].y = a_y+vecty;
		v[1].x = b_x+vectx;
		v[1].y = b_y+vecty;
		v[2].x = a_x-vectx;
		v[2].y = a_y-vecty;
		v[3].x = b_x-vectx;
		v[3].y = b_y-vecty;
		v[0].r = v[2].r = a->r; v[0].g = v[2].g = a->g; v[0].b = v[2].b = a->b; v[0].a = v[2].a = a->a;
		v[1].r = v[3].r = b->r; v[1].g = v[3].g = b->g; v[1].b = v[3].b = b->b; v[1].a = v[3].a = b->a;
		v[0].ooz = v[2].ooz = a->ooz; v[0].oow = v[2].oow = a->oow;
		v[1].ooz = v[3].ooz = b->ooz; v[1].oow = v[3].oow = b->oow;
		v[0].tmuvtx[0].sow = v[2].tmuvtx[0].sow = a->tmuvtx[0].sow;
		v[0].tmuvtx[0].tow = v[2].tmuvtx[0].tow = a->tmuvtx[0].tow;
		v[1].tmuvtx[0].sow = v[3].tmuvtx[0].sow = b->tmuvtx[0].sow;
		v[1].tmuvtx[0].tow = v[3].tmuvtx[0].tow = b->tmuvtx[0].tow;
		/* Mindkét körüljárási irányban rajzolunk, ha a backface culling engedélyezve van */
		
		vertexPointers[0] = v + lineInd1[0];
		vertexPointers[1] = v + lineInd1[1];
		vertexPointers[2] = v + lineInd1[2];
		vertexPointers[3] = v + lineInd1[3];
		numOfLineVertices = 4;
		if (astate.gcullmode != GR_CULL_DISABLE)	{
			vertexPointers[4] = v + lineInd2[0];
			vertexPointers[5] = v + lineInd2[1];
			vertexPointers[6] = v + lineInd2[2];
			vertexPointers[7] = v + lineInd2[3];
			numOfLineVertices = 8;
		}
		(*vertexProcessor)(vertexPointers, numOfLineVertices, 1);
		//VertexProcessorFPU(vertexPointers, numOfLineVertices, 1);
		//VertexProcessorSSE(vertexPointers, numOfLineVertices, 1);
		return;
	}
	reqIndexing = FALSE; //config.Flags2 & CFG_ALWAYSINDEXEDPRIMS ? TRUE : FALSE;
	
	if (primType != D3DPT_LINELIST || (primindexed != reqIndexing) )	{
		DrawFlushPrimitives(0);
		primType = D3DPT_LINELIST;
		primindexed = reqIndexing;
	}

	if (reqIndexing)	{
		if (numberOfIndices + 2 > DRAW_MAX_INDEXNUM) DrawFlushPrimitives(0);
		actindex[0] = numberOfVertices;
		actindex[1] = numberOfVertices + 1;
		actindex += 2;
		numberOfIndices += 2;
	}

	vertexPointers[0] = a;
	vertexPointers[1] = b;
	(*vertexProcessor)(vertexPointers, 2, 0);

	//VertexProcessorFPU(vertexPointers, 2, 0);
	//VertexProcessorSSE(vertexPointers, 2, 0);

	DEBUG_END("grAADrawLine", DebugRenderThreadId, 16);
}

/*-------------------------------------------------------------------------------------------*/
/* Pontok */

void EXPORT grDrawPoint( const GrVertex *a ) {

	DEBUG_BEGIN("grDrawPoint", DebugRenderThreadId, 17);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrDrawPoint(a)");
	if (primclipping != FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, FALSE);
		primclipping=FALSE;
		noVertexConverting = 0;
	}
	if (primType != D3DPT_POINTLIST || (primindexed != FALSE) )	{
		DrawFlushPrimitives(0);
		primType = D3DPT_POINTLIST;
		primindexed = FALSE;
	}
	(*vertexProcessor)(&(GrVertex *) a, 1, 0);
	//VertexProcessorFPU(&(GrVertex *) a, 1, 0);
	//VertexProcessorSSE(&(GrVertex *) a, 1, 0);
		
	DEBUG_END("grDrawPoint", DebugRenderThreadId, 17);
}


void EXPORT grAADrawPoint(GrVertex *p) {

	DEBUG_BEGIN("grAADrawPoint", DebugRenderThreadId, 18);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrAADrawPoint(a)");
	if (primclipping!=FALSE)	{
		DrawFlushPrimitives(0);
		lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice,D3DRENDERSTATE_CLIPPING,FALSE);
		primclipping=FALSE;
		noVertexConverting = 0;
	}
	if (primType != D3DPT_POINTLIST || (primindexed != FALSE) )	{
		DrawFlushPrimitives(0);
		primType = D3DPT_POINTLIST;
		primindexed = FALSE;
	}
	(*vertexProcessor)(&p, 1, 0);
	//VertexProcessorFPU(&p, 1, 0);
	//VertexProcessorSSE(&p, 1, 0);
	
	DEBUG_END("grAADrawPoint", DebugRenderThreadId, 18);
}


void DrawComputeScaleCoEffs(GrOriginLocation_t origin)	{
		
	if (config.Flags2 & CFG_YMIRROR)
		origin = (origin == GR_ORIGIN_UPPER_LEFT) ? GR_ORIGIN_LOWER_LEFT : GR_ORIGIN_UPPER_LEFT;

	xScale = yScale = 1.0f;
	if (drawresample)	{
		xScale = ((float) config.dxres) / appXRes;//(config.dxres - 1) / (appXRes - 1);
		yScale = ((float) config.dyres) / appYRes;
	}	
	linescalex = xScale;
	linescaley = yScale;

	if (origin != GR_ORIGIN_UPPER_LEFT)	{
		xScale = xScale;
		yScale = -yScale;
		xConst = - 0.5f;// * xscale;
		yConst = (float) movie.cmiYres - 0.5f;		
	} else {
		xConst = -0.5f;// * xscale;
		yConst = -0.5f;
	}
	/*tscale[0] = tscale[2] = xScale;
	tscale[1] = tscale[3] = yScale;
	tconst[0] = tconst[2] = xConst;
	tconst[1] = tconst[3] = yConst;*/
}


float	DrawGetFloatFromDepthValue (FxU16 depth, enum DepthValueMappingType mappingType) {
float	w;
	
	switch(mappingType)	{
		/* used for z-buffering */
		case DVLinearZ:
			return ((float) depth) / 65535.0f;

		/* used for w-buffer emulation */
		case DVNonlinearZ:
			w = ((float) ((( (unsigned int) depth & 0xFFF) | 0x1000) << (depth >> 12))) / 4096.0f;
			return (_QZn / w) + _Q;

		/* used for w-buffering */
		case DVTrueW:
		default:
			return ((float) ((( (unsigned int) depth & 0xFFF) | 0x1000) << (depth >> 12))) / 4096.0f;
	}
}


void	DrawTile(float left, float top, float right, float bottom, 
				 float tuLeft, float tvTop, float tuRight, float tvBottom,
				 float oow, float ooz) {

D3DTLVERTEX		vertex[4];
D3DTLVERTEX		*tileVertex;
int				vertexBufferUsed;

	vertexBufferUsed = 0;
	if (lpD3DVertexBufferTile != NULL) {
		if (lpD3DVertexBufferTile->lpVtbl->Lock(lpD3DVertexBufferTile, DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
												DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
												&tileVertex, NULL) == D3D_OK) {
			vertexBufferUsed = 1;
		}
	} else {
		tileVertex = vertex;
	}
	
	tileVertex[0].sx = tileVertex[2].sx = left;
	tileVertex[0].sy = tileVertex[1].sy = top;
	tileVertex[1].sx = tileVertex[3].sx = right;
	tileVertex[2].sy = tileVertex[3].sy = bottom;
	tileVertex[0].tu = tileVertex[2].tu = tuLeft;
	tileVertex[0].tv = tileVertex[1].tv = tvTop;
	tileVertex[1].tu = tileVertex[3].tu = tuRight;
	tileVertex[2].tv = tileVertex[3].tv = tvBottom;
	tileVertex[0].rhw = tileVertex[1].rhw = tileVertex[2].rhw = tileVertex[3].rhw = oow;
	tileVertex[0].sz = tileVertex[1].sz = tileVertex[2].sz = tileVertex[3].sz = ooz;
	if (vertexBufferUsed) {
		lpD3Ddevice->lpVtbl->DrawIndexedPrimitiveVB(lpD3Ddevice, D3DPT_TRIANGLELIST, lpD3DVertexBufferTile,
													0, 4,
													tileIndex, 6, 0);
		lpD3DVertexBufferTile->lpVtbl->Unlock(lpD3DVertexBufferTile);
	} else {
		lpD3Ddevice->lpVtbl->DrawIndexedPrimitive(lpD3Ddevice, D3DPT_TRIANGLELIST, D3D_VERTEXTYPE, vertex, 4, tileIndex, 6, 0);
	}

	if (renderbuffer == GR_BUFFER_FRONTBUFFER)	{
		lpD3Ddevice->lpVtbl->EndScene(lpD3Ddevice);
		ShowFrontBuffer();
		lpD3Ddevice->lpVtbl->BeginScene(lpD3Ddevice);
	}
}


/*------------------------------------------------------------------------------------------*/
/*................................... Init / Cleanup .......................................*/


int	DrawInit()	{
D3DVERTEXBUFFERDESC VBDesc;
D3DDEVICEDESC7		devcaps;
	
	DEBUG_BEGIN("DrawInit", DebugRenderThreadId, 19);
	
	lpD3Ddevice->lpVtbl->GetCaps(lpD3Ddevice, &devcaps);
	antialias = 0;
	if (devcaps.dpcLineCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASEDGES) antialias = AAFLAG_LINEAVAILABLE;
	if (devcaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT) antialias |= AAFLAG_TRIANGLEAVAILABLE;
	
	DEBUGCHECK(0, devcaps.dvMaxVertexW < 65528.0f, 
		"\n   Warning: DrawInit: Maximum W-based depth value that device supports is %f, while Glide requires 65528.0!", devcaps.dvMaxVertexW);
	DEBUGCHECK(1, devcaps.dvMaxVertexW < 65528.0f,
		"\n   Figyelmeztetés: DrawInit: Az eszköz által támogatott maximális W-alapú mélységi érték %f, míg a Glide 65528.0-t követel meg!", devcaps.dvMaxVertexW);

	VBDesc.dwSize = sizeof(D3DVERTEXBUFFERDESC);
	VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	if (!(config.Flags & CFG_ENABLEHWVBUFFER)) VBDesc.dwCaps |= D3DVBCAPS_SYSTEMMEMORY;
	VBDesc.dwFVF = D3D_VERTEXTYPE;
	VBDesc.dwNumVertices = DRAW_MAX_VERTEXNUM;
	vbvertex = memvertex = NULL;
	indices = NULL;
	astate.hints = 0;

	if ( (actvertex = memvertex = (D3DTLVERTEX *) _GetMem(DRAW_MAX_VERTEXNUM * sizeof(D3DTLVERTEX))) == NULL)
		return(0);		

	if ( (actindex = indices = (short *) _GetMem(DRAW_MAX_INDEXNUM * sizeof(short))) == NULL )	{
		_FreeMem(memvertex);
		memvertex = NULL;
		return(0);
	}

	if ( (vertexPointers = _GetMem(MAX_NUM_OF_VERTEXPOINTERS * sizeof(GrVertex *))) == NULL)
		return(0);

	//vertexProcessor = (instructionSet & SSE_SUPPORTED) ? VertexProcessorSSE : VertexProcessorFPU;
	vertexProcessor = VertexProcessorFPU;

	numberOfVertices = numberOfIndices = 0;
	astate.vertexmode = VMODE_ZBUFF;
	perspective = TRUE;
	drawmode = DRAWMODE_CLIPPING;
	primType = D3DPT_TRIANGLELIST;	
	primindexed = primclipping = FALSE;
	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CLIPPING, FALSE);
	noVertexConverting = 0;
	DrawComputeScaleCoEffs(astate.locOrig);

	linemode = (appXRes < movie.cmiXres) && (appYRes < movie.cmiYres);

	vbflags = 0;
	if (lpD3D->lpVtbl->CreateVertexBuffer(lpD3D, &VBDesc, &lpD3DVertexBuffer, 0) == DD_OK)	{
	//if (0) {
		vbflags = VBFLAG_VBAVAILABLE | VBFLAG_VBUSED;
		lpD3DVertexBuffer->lpVtbl->Lock(lpD3DVertexBuffer, DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
											DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
											&vbvertex, NULL);
		actvertex = vbvertex;
	}

	VBDesc.dwNumVertices = 16;
	if (lpD3D->lpVtbl->CreateVertexBuffer(lpD3D, &VBDesc, &lpD3DVertexBufferTile, 0) != DD_OK)
		lpD3DVertexBufferTile = NULL;


	lpD3Ddevice->lpVtbl->SetRenderState(lpD3Ddevice, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	astate.cullmode = GR_CULL_DISABLE;
	actoffsetvb = 0;
	vbbuffend = actvertex + DRAW_MAX_VERTEXNUM;
	stats.trisProcessed = 0;

	sse_perf = fpu_perf = 0;
	config.Flags2 |= CFG_ALWAYSINDEXEDPRIMS;
	return(1);
	
	DEBUG_END("DrawInit", DebugRenderThreadId, 19);
}


void DrawCleanUp()	{
//float f;

	DEBUG_BEGIN("DrawCleanUp", DebugRenderThreadId, 20);
	
	DrawFlushPrimitives(0);
	if (vbflags & VBFLAG_VBAVAILABLE)	{	
		if (lpD3DVertexBuffer)	{
			lpD3DVertexBuffer->lpVtbl->Unlock(lpD3DVertexBuffer);
			lpD3DVertexBuffer->lpVtbl->Release(lpD3DVertexBuffer);
		}
	}
	if (lpD3DVertexBufferTile != NULL)
		lpD3DVertexBufferTile->lpVtbl->Release (lpD3DVertexBufferTile);
	
	if (memvertex != NULL) _FreeMem(memvertex);
	if (indices != NULL) _FreeMem(indices);
	if (vertexPointers != NULL) _FreeMem(vertexPointers);

	//f = DEBUG_GET_PERF_QUOTIENT(sse_perf,fpu_perf);
	//DEBUGLOG(2,"\nsse/fpu = %f",f);

	DEBUG_END("DrawCleanUp", DebugRenderThreadId, 20);
}