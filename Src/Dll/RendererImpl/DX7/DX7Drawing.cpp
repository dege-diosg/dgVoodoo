/*--------------------------------------------------------------------------------- */
/* DX7Drawing.cpp - dgVoodoo 3D polygon rendering DX7 implementation                */
/*                                                                                  */
/* Copyright (C) 2007 Dege                                                          */
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
/* dgVoodoo: DX7Drawing.cpp																	*/
/*			 DX7 renderer drawing class implementation										*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

extern "C" {

#include	"Debug.h"
#include	"dgVoodooGlide.h"
#include	"Log.h"
#include	"dgVoodooBase.h"
#include	"Dgw.h"
#include	"Draw.h"
#include	"Main.h"

}

#include	"DX7Drawing.hpp"
#include	"DX7LfbDepth.hpp"
#include	"DX7General.hpp"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define DRAW_MAX_VERTEXNUM			16384
#define DRAW_MAX_INDEXNUM			16384

#define WFAR						65536.0f
#define WNEAR						1.0f

#define VBFLAG_VBUSED				1
#define VBFLAG_VBAVAILABLE			2

/* A használt vertextípus definiálása */
//#define D3D_VERTEXTYPE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define D3D_VERTEXTYPE				(D3DFVF_TLVERTEX)

#define	RGBcval		(255.0f/256.0f)

//void VertexProcessorSSE(GrVertex **vlist, int nVerts);
//void VertexProcessorFPU(GrVertex **vlist, int nVerts);


/*------------------------------------------------------------------------------------------*/
/*............................ DX7 implementáció a rajzoláshoz .............................*/

static	const float		_scale4			= 4.0f;
static	const float		rgbcval			= RGBcval;
static  const int		lineInd1[]		= {0, 1, 3, 2};
static  const int		lineInd2[]		= {0, 2, 3, 1};
static  WORD			tileIndex[6]	= {0, 1, 2, 1, 3, 2};
static  const float		_Q				= WFAR / (WFAR - WNEAR);
static  const float		_QZn			= -((WFAR * WNEAR) / (WFAR - WNEAR));

float					depthBias_Z;
int						depthBias_W;
float					depthBias_cz, depthBias_cw;

static	const D3DPRIMITIVETYPE	primitiveTypesLookUp[] =	{ D3DPT_TRIANGLELIST, D3DPT_TRIANGLELIST, D3DPT_TRIANGLELIST,
															  D3DPT_TRIANGLELIST, D3DPT_LINELIST, D3DPT_LINELIST,
															  D3DPT_POINTLIST };
static	const DWORD		indexedTypeLookUp[]	=		{ TRUE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE };
static	const DWORD		clippedTypeLookUp[] =		{ FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE};

//void					(*vertexProcessor) (GrVertex **vlist, int nVerts, int generateIndices);

/*------------------------------------------------------------------------------------------*/
/*......................... FPU-vertexkonverter, inline függvények .........................*/

//Disable 'no return value' warning
#pragma warning (disable: 4035)

static void __forceinline ConvertVertex (GrVertex *vertex, D3DTLVERTEX *actvertex)	{
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


static float __forceinline GetClampedWValue (float oow)
{
float	w;
int		mantissa, exponent;

	_asm	{
		mov		eax,oow
		fld1
		fdiv	oow
		fstp	w
		mov		eax,w
		fld1
		mov		ecx,eax
/*		test	eax,80000000h
		je		_positive
		fchs
_positive:*/
		shr		eax,23
		or		ecx,0800000h
		sub		al,0x7F
		js		_clampedTo1
		and		ecx,0FFFFFFh
		and		eax,0x0F
		
		mov		mantissa,ecx
		sub		eax,23
		mov		exponent,eax
		fild	exponent
		fild	mantissa
		fscale
		fstp	st(1)
		fdivp	st(1),st(0)
_clampedTo1:
	}
}


static int __forceinline GetFogIndexValue(float f, float *w)	{
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
static void __forceinline AddWBias(float oow, float *toStore)	{
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
static float __forceinline AddWEmuBias(float oow)	{
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


static float __forceinline ZCLAMP(float ooz)	{

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


static DWORD __forceinline GetARGB(GrVertex *vertex)	{
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


static DWORD __forceinline	GetFogIntValue(float fog)	{
DWORD	fogInt;
	_asm	{
		fld		DWORD PTR fog
		fistp	fogInt
	}
	return(fogInt << 24);
}

#pragma warning (default: 4035)


// ------------------------------------------------------------------------------------------
// ----- Member functions -------------------------------------------------------------------


void  __inline DX7Drawing::SwitchToVertexBufferIfAvailable ()
{
	if (vbflags & VBFLAG_VBAVAILABLE)	{
		vbflags |= VBFLAG_VBUSED;			
		vbbuffend = vbvertex + DRAW_MAX_VERTEXNUM;
		actvertex = vbvertex + actoffsetvb;
	}
}


void  __inline DX7Drawing::SwitchToNormalBuffer ()
{
	if (vbflags & VBFLAG_VBAVAILABLE)	{
		vbflags &= ~VBFLAG_VBUSED;			
		vbbuffend = (actvertex = memvertex) + DRAW_MAX_VERTEXNUM;
	}
}


void	DX7Drawing::EnableStencilMode (int enable)	{

	if (enable) {
	
		D3DTLVERTEX v[6];
		DWORD		srcBlend, dstBlend;

		lpD3Ddevice->SetRenderState (D3DRENDERSTATE_STENCILFUNC, D3DCMP_ALWAYS);
		if (astate.depthMaskEnable!=FXFALSE) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		if (astate.alphaBlendEnable!=TRUE) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		lpD3Ddevice->GetRenderState (D3DRENDERSTATE_SRCBLEND, &srcBlend);
		lpD3Ddevice->GetRenderState (D3DRENDERSTATE_DESTBLEND, &dstBlend);
		if (astate.rgbSrcBlend!=GR_BLEND_ZERO) lpD3Ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_ZERO);
		if (astate.rgbDstBlend!=GR_BLEND_ONE) lpD3Ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE);

		v[0].sx = 0.0f;
		v[0].sy = 0.0f;
		v[1].sx = v[3].sx = (float) movie.cmiXres;
		v[1].sy = v[3].sy = 0.0f;
		v[2].sy = v[4].sy = (float) movie.cmiYres;
		v[5].sx = (float) movie.cmiXres;
		v[5].sy = (float) movie.cmiYres;
		v[0].sz = v[1].sz = v[2].sz = v[3].sz = v[4].sz = v[5].sz = depthBias_cz;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = v[5].rhw = depthBias_cw;
		lpD3Ddevice->DrawPrimitive (D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, v, 6, 0);
		
		if (astate.depthMaskEnable != FXFALSE) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_ZWRITEENABLE, TRUE);
		if (astate.alphaBlendEnable != TRUE) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, astate.alphaBlendEnable);
		if (astate.rgbSrcBlend != D3DBLEND_ZERO) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_SRCBLEND, srcBlend);
		if (astate.rgbDstBlend != D3DBLEND_ONE) lpD3Ddevice->SetRenderState (D3DRENDERSTATE_DESTBLEND, dstBlend);

		if (astate.depthFunction != D3DCMP_ALWAYS) lpD3Ddevice->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
		lpD3Ddevice->SetRenderState (D3DRENDERSTATE_STENCILFUNC, D3DCMP_EQUAL);

	} else {
		
		if (astate.depthFunction != D3DCMP_ALWAYS)
			lpD3Ddevice->SetRenderState (D3DRENDERSTATE_ZFUNC, cmpFuncLookUp[astate.depthFunction]);	
	}
}


void		DX7Drawing::CreateVertexBuffers ()
{
	D3DVERTEXBUFFERDESC vbDesc;
	vbDesc.dwSize = sizeof(D3DVERTEXBUFFERDESC);
	vbDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	
	if (!useHwBuffers)
		vbDesc.dwCaps |= D3DVBCAPS_SYSTEMMEMORY;
	
	vbDesc.dwFVF = D3D_VERTEXTYPE;
	vbDesc.dwNumVertices = DRAW_MAX_VERTEXNUM;

	lpD3DVertexBuffer = NULL;
	lpD3DVertexBufferTile = NULL;
	lpD3D->CreateVertexBuffer(&vbDesc, &lpD3DVertexBuffer, 0);

	vbDesc.dwNumVertices = 16;
	lpD3D->CreateVertexBuffer(&vbDesc, &lpD3DVertexBufferTile, 0);
}


void		DX7Drawing::ReleaseVertexBuffers ()
{
	if (lpD3DVertexBuffer != NULL) {
		lpD3DVertexBuffer->Release ();
		lpD3DVertexBuffer = NULL;
	}
	if (lpD3DVertexBufferTile != NULL) {
		lpD3DVertexBufferTile->Release ();
		lpD3DVertexBufferTile = NULL;
	}
}


int			DX7Drawing::ConvertAndCacheVertices (GrVertex **vlist, int nVerts, int /*numOfPrimitives*/)
{
int				i, j;
float			w, tmuoow;
GrVertex		*vertex;
unsigned int	CTW, NEWCTW;

	DEBUG_BEGINSCOPE("VertexProcessorFPU", DebugRenderThreadId);

	//DEBUG_PERF_MEASURE_BEGIN(&fpu_perf);
	if ( (actvertex + nVerts) > vbbuffend) {
		tooManyVertex = 1;
		if (!numberOfVertices) {
			FlushCachedVertices ();
		} else
			return (0);
	}
	
	if ( (primIndexed) && ((numberOfIndices + 3*(nVerts - 2)) > DRAW_MAX_INDEXNUM))
		return (0);
	
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
			actvertex->tu = vertex->tmuvtx[0].sow / (astate.divx * tmuoow);
			actvertex->tv = vertex->tmuvtx[0].tow / (astate.divy * tmuoow);
		}
		switch(astate.vertexmode & VMODE_DEPTHBUFFMMODESELECT)	{
			case (VMODE_ZBUFF):
				if (astate.vertexmode & VMODE_TEXTURE)	{
					actvertex->rhw = (astate.flags & STATE_USETMUW) ? vertex->tmuvtx[0].oow : vertex->oow;
				} else actvertex->rhw = 1.0f;
				actvertex->sz = ZCLAMP((vertex->ooz + depthBias_Z) / WFAR);
				break;
			case (VMODE_WBUFFEMU):
				{
					float* pOow = (astate.flags & STATE_USETMUW) ? &vertex->tmuvtx[0].oow : &vertex->oow;
					actvertex->sz = AddWEmuBias(actvertex->rhw = (astate.vertexmode & VMODE_WUNDEFINED) ? GetClampedWValue(*pOow) : *pOow);
					LOG (1, "\n		actvertex->rhw = %f", actvertex->rhw);
				}
				break;
			case (VMODE_WBUFF):
				{
					float* pOow = (astate.flags & STATE_USETMUW) ? &vertex->tmuvtx[0].oow : &vertex->oow;
					AddWBias((astate.vertexmode & VMODE_WUNDEFINED) ? GetClampedWValue(*pOow) : *pOow, &actvertex->rhw);
					actvertex->sz = 0.5f;
				}
				break;
		}
		if (astate.fogmode == GR_FOG_WITH_TABLE)	{
			i = WToInd[GetFogIndexValue(vertex->oow, &w)];
			actvertex->specular = GetFogIntValue ( ((w-IndToW[i]) * fogtable[i+1] + (IndToW[i+1]-w) * fogtable[i])/(IndToW[i+1]-IndToW[i]) );
		}
		actvertex++;
	}
	
	if (primIndexed)	{
		_asm	{
			mov		edx,nVerts
			mov		esi,this
			mov		edi,[esi]DX7Drawing.actindex
			mov		ecx,[esi]DX7Drawing.numberOfVertices
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
			mov		[esi]DX7Drawing.actindex,edi
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

	stats.trisProcessed += nVerts - 2;

	_asm	fldcw	[CTW]

	//DEBUG_PERF_MEASURE_END(&fpu_perf);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return (1);
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


void		DX7Drawing::FlushCachedVertices ()
{
/*unsigned long cop,carg1,carg2, alphaop,aarg1,aarg2;
DWORD abe, asrc, adst;
DWORD clipping;*/

	/*//lpD3Ddevice->GetRenderState(D3DRENDERSTATE_CLIPPING, &clipping);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAARG2,&aarg2);
	LOG(0,"\n  stage0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cachedColorOp[0],cachedColorArg1[0],cachedColorArg2[0],cachedAlphaOp[0],cachedAlphaArg1[0],cachedAlphaArg2[0]);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAARG2,&aarg2);
	LOG(0,"\n  stage1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cachedColorOp[1],cachedColorArg1[1],cachedColorArg2[1],cachedAlphaOp[1],cachedAlphaArg1[1],cachedAlphaArg2[1]);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAARG2,&aarg2);
	LOG(0,"\n  stage2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cachedColorOp[2],cachedColorArg1[2],cachedColorArg2[2],cachedAlphaOp[2],cachedAlphaArg1[2],cachedAlphaArg2[2]);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &abe);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &asrc);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &adst);*/

	//dx7General->WaitUntilBufferSwitchDone ();

	if (dx7LfbDepth->IsCompareStencilModeUsed ())
		EnableStencilMode (1);

	if (vbflags & VBFLAG_VBUSED)	{
		lpD3DVertexBuffer->Unlock();
		if (numberOfVertices) {
			if (primIndexed)	{
				lpD3Ddevice->DrawIndexedPrimitiveVB(primType, lpD3DVertexBuffer,
													actoffsetvb, numberOfVertices,
													indices, numberOfIndices, 0);
			} else {
				lpD3Ddevice->DrawPrimitiveVB(primType, lpD3DVertexBuffer,
											 actoffsetvb, numberOfVertices, 0);
			}
		}
		HRESULT hr;
		if (tooManyVertex || (actvertex == vbbuffend))	{
			hr = lpD3DVertexBuffer->Lock(DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
										 DDLOCK_WAIT | DDLOCK_WRITEONLY |
										 DDLOCK_DISCARDCONTENTS, (LPVOID *) &vbvertex, NULL);
			actoffsetvb = 0;
			actvertex = vbvertex;
		} else {
			hr = lpD3DVertexBuffer->Lock(DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
										 DDLOCK_WAIT | DDLOCK_WRITEONLY |
										 DDLOCK_NOOVERWRITE, (LPVOID *) &vbvertex, NULL);
			
			actoffsetvb += numberOfVertices;
		}
		tooManyVertex = 0;
		vbbuffend = vbvertex + DRAW_MAX_VERTEXNUM;
		if (FAILED (hr)) {
			vbflags &= ~(VBFLAG_VBAVAILABLE | VBFLAG_VBUSED);
			actvertex = memvertex;
		}

	} else {
		if (primIndexed)	{
			lpD3Ddevice->DrawIndexedPrimitive(primType, D3D_VERTEXTYPE, memvertex, numberOfVertices, indices, numberOfIndices,
											  0);
		} else lpD3Ddevice->DrawPrimitive(primType, D3D_VERTEXTYPE, memvertex, numberOfVertices, 0);
		actvertex = memvertex;
	}		
	actindex = indices;
	numberOfVertices = numberOfIndices = 0;

	if (dx7LfbDepth->IsCompareStencilModeUsed ())
		EnableStencilMode (0);

	/* Az adott primitivek kirajzolásának kényszeritése */
	if (renderbuffer == GR_BUFFER_FRONTBUFFER)
		dx7General->ShowContentChangesOnFrontBuffer ();
}


void		DX7Drawing::SetPrimitiveType (enum PrimType primitiveType)
{

	D3DPRIMITIVETYPE d3dPrimType = primitiveTypesLookUp[primitiveType];
	DWORD			 d3dIndexed = indexedTypeLookUp[primitiveType];
	DWORD			 d3dClipped = clippedTypeLookUp[primitiveType];

	primType = d3dPrimType;
	primIndexed = d3dIndexed;
	if (primClipping != d3dClipped) {
		lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CLIPPING, primClipping = d3dClipped);
		if (primClipping) {
			SwitchToNormalBuffer ();
			noVertexConverting = 1;
		} else {
			SwitchToVertexBufferIfAvailable ();
			noVertexConverting = 0;
		}
	}
}


void		DX7Drawing::DrawTile(float left, float top, float right, float bottom, 
								 float tuLeft, float tvTop, float tuRight, float tvBottom,
								 float oow, float ooz, unsigned int constColor) {

D3DTLVERTEX		vertex[4];
D3DTLVERTEX		*tileVertex;
int				vertexBufferUsed;

	vertexBufferUsed = 0;
	tileVertex = vertex;
	if (lpD3DVertexBufferTile != NULL) {
		if (lpD3DVertexBufferTile->Lock(DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
										DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
										(LPVOID *) &tileVertex, NULL) == D3D_OK) {
			vertexBufferUsed = 1;
		}
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
	tileVertex[0].color = tileVertex[1].color = tileVertex[2].color = tileVertex[3].color = constColor;
	if (vertexBufferUsed) {
		
		lpD3DVertexBufferTile->Unlock();
		
		lpD3Ddevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, lpD3DVertexBufferTile, 0, 4, tileIndex, 6, 0);
	} else {
		lpD3Ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3D_VERTEXTYPE, vertex, 4, tileIndex, 6, 0);
	}

	//Ez csak debuggoláshoz kell!!
	//if (renderbuffer == GR_BUFFER_FRONTBUFFER)
	//	dx7General->ShowContentChangesOnFrontBuffer ();
}


void		DX7Drawing::BeforeBufferSwap ()
{
	if (vbflags & VBFLAG_VBAVAILABLE)
		lpD3DVertexBuffer->Unlock();
}


void		DX7Drawing::AfterBufferSwap ()
{
	if (lpD3DVertexBuffer != NULL && (!(vbflags & VBFLAG_VBAVAILABLE))) {
		if (!FAILED (lpD3DVertexBuffer->Lock(DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
											 DDLOCK_WAIT | DDLOCK_WRITEONLY |
											 DDLOCK_NOOVERWRITE, (LPVOID *) &vbvertex, NULL))) {
			vbflags = VBFLAG_VBAVAILABLE;
		}
	}
}


int			DX7Drawing::Init (int useHardwareBuffers)
{
D3DDEVICEDESC7		devcaps;

	dx7Drawing = this;

	useHwBuffers = useHardwareBuffers;

	lpD3Ddevice->GetCaps(&devcaps);

	//antialias = 0;
	//if (devcaps.dpcLineCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASEDGES) antialias = AAFLAG_LINEAVAILABLE;
	//if (devcaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT) antialias |= AAFLAG_TRIANGLEAVAILABLE;

	DEBUGCHECK(0, devcaps.dvMaxVertexW < 65528.0f, \
		"\n   Warning: DrawInit: Maximum W-based depth value that device supports is %f, while Glide requires 65528.0!", devcaps.dvMaxVertexW);
	DEBUGCHECK(1, devcaps.dvMaxVertexW < 65528.0f, \
		"\n   Figyelmeztetés: DrawInit: Az eszköz által támogatott maximális W-alapú mélységi érték %f, míg a Glide 65528.0-t követel meg!", devcaps.dvMaxVertexW);

	vbvertex = memvertex = NULL;
	indices = NULL;

	if ( (actvertex = memvertex = (D3DTLVERTEX *) _GetMem(DRAW_MAX_VERTEXNUM * sizeof(D3DTLVERTEX))) == NULL)
		return(0);		

	if ( (actindex = indices = (unsigned short *) _GetMem(DRAW_MAX_INDEXNUM * sizeof(short))) == NULL )	{
		_FreeMem(memvertex);
		memvertex = NULL;
		return(0);
	}


	numberOfVertices = numberOfIndices = 0;

	primType = D3DPT_TRIANGLELIST;
	primIndexed = primClipping = FALSE;

	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CLIPPING, FALSE);
	noVertexConverting = 0;

	vbflags = 0;

	CreateVertexBuffers ();
	if (lpD3DVertexBuffer != NULL) {
		vbflags = VBFLAG_VBAVAILABLE | VBFLAG_VBUSED;
		lpD3DVertexBuffer->Lock(DDLOCK_NOSYSLOCK | DDLOCK_SURFACEMEMORYPTR |
								DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID *) &vbvertex, NULL);
		actvertex = vbvertex;
	}

	lpD3Ddevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	actoffsetvb = 0;
	vbbuffend = actvertex + DRAW_MAX_VERTEXNUM;

	//vertexProcessor = (instructionSet & SSE_SUPPORTED) ? VertexProcessorSSE : VertexProcessorFPU;
	//vertexProcessor = VertexProcessorFPU;

	tooManyVertex = 0;

	return (1);
}
	

void		DX7Drawing::DeInit ()
{
	//if (vbflags & VBFLAG_VBAVAILABLE)	{
		if (lpD3DVertexBuffer != NULL)	{
			lpD3DVertexBuffer->Unlock();
		}
		/*if (lpD3DVertexBuffer)	{
			lpD3DVertexBuffer->Unlock();
			lpD3DVertexBuffer->Release();
		}*/
	/*}
	if (lpD3DVertexBufferTile != NULL)
		lpD3DVertexBufferTile->Release ();*/

	ReleaseVertexBuffers ();

	if (memvertex != NULL) _FreeMem(memvertex);
	if (indices != NULL) _FreeMem(indices);
}


void		DX7Drawing::SetDepthBias (FxI16 level, int wBuffEmuUsed, int stencilCompareDepth)
{
	if (!stencilCompareDepth)	{
		depthBias_W = ((int) level) << 11;
		depthBias_Z = (float) level;
	} else {
		depthBias_W = 0;
		depthBias_Z = 0.0f;
		depthBias_cz = DrawGetFloatFromDepthValue (level, wBuffEmuUsed ? DVNonlinearZ : DVLinearZ);
		depthBias_cw = DrawGetFloatFromDepthValue (level, DVTrueW);
	}
}


void		DX7Drawing::ResourcesMayLost ()
{
	FlushCachedVertices ();
}


int			DX7Drawing::RestoreResources ()
{
	ReleaseVertexBuffers ();
	CreateVertexBuffers ();

	if (vbflags & VBFLAG_VBUSED) {
		actoffsetvb = 0;
		actvertex = vbvertex;
		vbbuffend = vbvertex + DRAW_MAX_VERTEXNUM;
	}
	tooManyVertex = 0;
	actindex = indices;
	numberOfVertices = numberOfIndices = 0;

	return !((vbflags & VBFLAG_VBAVAILABLE) && (lpD3DVertexBuffer == NULL));
}