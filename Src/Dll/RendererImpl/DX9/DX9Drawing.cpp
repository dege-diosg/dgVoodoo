/*------------------------------------------------------------------------------------------*/
/* dgVoodoo: DX9Drawing.cpp																	*/
/*			 DX9 renderer drawing class implementation										*/
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

#include "Resource.h"

}

#include	"DX9Drawing.hpp"
#include	"DX9General.hpp"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define DRAW_MAX_VERTEXNUM			16384
#define DRAW_MAX_INDEXNUM			16384

#define	MAXTILENUMINBUFFERS			16

#define WFAR						65536.0f
#define WNEAR						1.0f

#define VIB_ISINUSE					1
#define VIB_AVAILABLE				2

/* A használt vertextípus definiálása */
#define D3D_FVFVERTEXTYPE			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

#define	RGBcval						(255.0f/256.0f)

/*------------------------------------------------------------------------------------------*/
/*............................ DX9 implementáció a rajzoláshoz .............................*/

static	const float		_scale4			= 4.0f;
static	const float		rgbcval			= RGBcval;
static  const int		lineInd1[]		= {0, 1, 3, 2};
static  const int		lineInd2[]		= {0, 2, 3, 1};
static  WORD			tileIndex16[6]	= {0, 1, 2, 1, 3, 2};
static  DWORD			tileIndex32[6]	= {0, 1, 2, 1, 3, 2};
static  const float		_Q				= WFAR / (WFAR - WNEAR);
static  const float		_QZn			= -((WFAR * WNEAR) / (WFAR - WNEAR));

float					d3d9_depthBias_Z;
int						d3d9_depthBias_W;
float					d3d9_depthBias_cz, d3d9_depthBias_cw;

static	const D3DPRIMITIVETYPE	primitiveTypesLookUp[] =	{ D3DPT_TRIANGLELIST, D3DPT_TRIANGLELIST, D3DPT_TRIANGLELIST,
															  D3DPT_TRIANGLELIST, D3DPT_LINELIST, D3DPT_LINELIST,
															  D3DPT_POINTLIST };
static	const DWORD		indexedTypeLookUp[]	=		{ TRUE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE };
static	const DWORD		clippedTypeLookUp[] =		{ FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE};

static	const D3DVERTEXELEMENT9 vertexDecl[] = {{0, 0, D3DDECLTYPE_FLOAT4, 0, D3DDECLUSAGE_POSITIONT, 0},
												{0, 16, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 0},
												{0, 20, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 1},
												{0, 24, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0},
												D3DDECL_END () };

/*------------------------------------------------------------------------------------------*/
/*......................... FPU-vertexkonverter, inline függvények .........................*/

//Disable 'no return value' warning
#pragma warning (disable: 4035)

static void __forceinline ConvertVertex (GrVertex *vertex, D3DTLVERTEX *actvertex)	{
int	x, y;
	
	_asm	{
		mov		ecx,vertex
		fld		DWORD PTR [ecx]GrVertex.x
		mov		edx,actvertex
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
		fstp	[edx]D3DTLVERTEX.x
		fild	DWORD PTR y
		fscale
		fmul	yScale
		fadd	yConst
		fstp	[edx]D3DTLVERTEX.y
		fchs
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
		mov		ecx,d3d9_depthBias_W
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
		mov		eax,d3d9_depthBias_W
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

void  __inline DX9Drawing::SwitchToVertexBufferIfAvailable ()
{
	if (vibFlags & VIB_AVAILABLE)
		vibFlags |= VIB_ISINUSE;
}


void  __inline DX9Drawing::SwitchToNormalBuffer ()
{
	if (vibFlags & VIB_AVAILABLE)
		vibFlags &= ~VIB_ISINUSE;			
}


int		DX9Drawing::CreateVertexAndIndexBuffer ()
{
	HRESULT hr = lpD3Ddevice9->CreateVertexBuffer (vbVertexBuffSize * sizeof (D3DTLVERTEX),
												   D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | ((vibPool == D3DPOOL_SYSTEMMEM) ? D3DUSAGE_SOFTWAREPROCESSING : 0),
												   D3D_FVFVERTEXTYPE, vibPool, &lpD3DVertexBuffer9, NULL);

	if (!FAILED (hr)) {
		hr = lpD3Ddevice9->CreateIndexBuffer (ibIndexBuffSize * indexSizeInBytes, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											  (indexSizeInBytes == 4) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, vibPool,
											  &lpD3DIndexBuffer9, NULL);

		if (!FAILED (hr)) {
			
			lpD3Ddevice9->SetFVF (D3D_FVFVERTEXTYPE);
			lpD3Ddevice9->SetStreamSource (0, lpD3DVertexBuffer9, 0, sizeof (D3DTLVERTEX));
			lpD3Ddevice9->SetIndices (lpD3DIndexBuffer9);
			
			lpD3DVertexBuffer9->Lock (actOffsetInVB = 0, 0, (void**) &actVertexPtr, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
			lpD3DIndexBuffer9->Lock (actOffsetInIB = 0, 0, (void**) &actIndexPtr, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
			
			HRESULT hr = lpD3Ddevice9->CreateVertexBuffer (MAXTILENUMINBUFFERS * 4 * sizeof (D3DTLVERTEX),
														   D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | ((vibPool == D3DPOOL_SYSTEMMEM) ? D3DUSAGE_SOFTWAREPROCESSING : 0),
														   D3D_FVFVERTEXTYPE, vibPool, &lpD3DTileVertexBuffer9, NULL);

			if (!FAILED (hr)) {
				hr = lpD3Ddevice9->CreateIndexBuffer (ibIndexBuffSize * indexSizeInBytes, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
													  (indexSizeInBytes == 4) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, vibPool,
													   &lpD3DTileIndexBuffer9, NULL);
				if (!FAILED (hr)) {
					vibFlags = VIB_AVAILABLE | VIB_ISINUSE;

					tileNumInBuffers = 0;
					return (1);
				}

				lpD3DTileVertexBuffer9->Release ();
			}
			lpD3DIndexBuffer9->Release ();
		}
		lpD3DVertexBuffer9->Release ();
	}
	
	return (0);
}


void	DX9Drawing::EnableStencilMode (int enable)	{

	if (enable) {
	
		D3DTLVERTEX v[6];
		DWORD		srcBlend, dstBlend;

		lpD3Ddevice9->SetRenderState (D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		if (astate.depthMaskEnable != FXFALSE)
			lpD3Ddevice9->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		if (astate.alphaBlendEnable != TRUE)
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		lpD3Ddevice9->GetRenderState (D3DRS_SRCBLEND, &srcBlend);
		lpD3Ddevice9->GetRenderState (D3DRS_DESTBLEND, &dstBlend);
		if (astate.rgbSrcBlend != GR_BLEND_ZERO)
			lpD3Ddevice9->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_ZERO);
		if (astate.rgbDstBlend != GR_BLEND_ONE)
			lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		v[0].x = 0.0f;
		v[0].y = 0.0f;
		v[1].x = v[3].x = (float) movie.cmiXres;
		v[1].y = v[3].y = 0.0f;
		v[2].y = v[4].y = (float) movie.cmiYres;
		v[5].x = (float) movie.cmiXres;
		v[5].y = (float) movie.cmiYres;
		v[0].z = v[1].z = v[2].z = v[3].z = v[4].z = v[5].z = d3d9_depthBias_cz;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = v[5].rhw = d3d9_depthBias_cw;
		lpD3Ddevice9->DrawPrimitiveUP (D3DPT_TRIANGLELIST, 2, v, sizeof(D3DTLVERTEX));
		
		if (astate.depthMaskEnable != FXFALSE)
			lpD3Ddevice9->SetRenderState (D3DRS_ZWRITEENABLE, TRUE);
		if (astate.alphaBlendEnable != TRUE)
			lpD3Ddevice9->SetRenderState (D3DRS_ALPHABLENDENABLE, astate.alphaBlendEnable);
		if (astate.rgbSrcBlend != D3DBLEND_ZERO)
			lpD3Ddevice9->SetRenderState (D3DRS_SRCBLEND, srcBlend);
		if (astate.rgbDstBlend != D3DBLEND_ONE)
			lpD3Ddevice9->SetRenderState (D3DRS_DESTBLEND, dstBlend);

		if (astate.depthFunction != D3DCMP_ALWAYS)
			lpD3Ddevice9->SetRenderState (D3DRS_ZFUNC, D3DCMP_ALWAYS);
		lpD3Ddevice9->SetRenderState (D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	} else {
		
		if (astate.depthFunction != D3DCMP_ALWAYS)
			lpD3Ddevice9->SetRenderState(D3DRS_ZFUNC, cmpFuncLookUp[astate.depthFunction]);
	}
}


int			DX9Drawing::ConvertAndCacheVertices (GrVertex **vlist, int nVerts, int numOfPrimitives)
{
int				i, j;
float			w, tmuoow;
GrVertex		*vertex;
unsigned int	CTW, NEWCTW;

	DEBUG_BEGINSCOPE("VertexProcessorFPU", DebugRenderThreadId);

	//DEBUG_PERF_MEASURE_BEGIN(&fpu_perf);

	if ( ((actOffsetInVB + nVerts + numberOfVertices) > vbVertexBuffSize) ||
		  (primIndexed && ((actOffsetInIB + 3*numOfPrimitives + numberOfIndices) > ibIndexBuffSize)) ||
		  ((numberOfPrimitives + numOfPrimitives) > maxPrimitiveCount) ) {
		
		tooManyVertex = 1;
		if (!numberOfVertices) {
			FlushCachedVertices ();
		} else
			return (0);
	}
	
	_asm	{
		fstcw	[CTW]
		mov		eax,CTW
		and		eax,0E0C0h
		or		eax,00C3Fh
		mov		[NEWCTW],eax
		fldcw	[NEWCTW]
		fld		_scale4
	}

	for(j = 0; j < nVerts; j++)	{
		vertex = vlist[j];

		if (noVertexConverting) {
			actVertexPtr->x = vertex->x * xScale + xConst;
			actVertexPtr->y = vertex->y * yScale + yConst;
		} else {
			ConvertVertex (vertex, actVertexPtr);
		}

		if (astate.vertexmode & (VMODE_DIFFUSE | VMODE_ALPHAFOG))	{
			actVertexPtr->specular = (
				actVertexPtr->diffuse = (astate.flags & STATE_DELTA0) ? astate.delta0Rgb : GetARGB(vertex)
			) ^ 0xFF000000;
		}

		if (astate.vertexmode & VMODE_TEXTURE)	{
			tmuoow = (astate.hints & GR_STWHINT_W_DIFF_TMU0) ? vertex->tmuvtx[0].oow : vertex->oow;
			actVertexPtr->tu = vertex->tmuvtx[0].sow / (astate.divx * tmuoow);
			actVertexPtr->tv = vertex->tmuvtx[0].tow / (astate.divy * tmuoow);
		}
		switch(astate.vertexmode & VMODE_DEPTHBUFFMMODESELECT)	{
			case (VMODE_ZBUFF):
				if (astate.vertexmode & VMODE_TEXTURE)	{
					actVertexPtr->rhw = (astate.flags & STATE_USETMUW) ? vertex->tmuvtx[0].oow : vertex->oow;
				} else actVertexPtr->rhw = 1.0f;
				actVertexPtr->z = ZCLAMP((vertex->ooz + d3d9_depthBias_Z) / WFAR);
				break;
			case (VMODE_WBUFFEMU):
				{
					float* pOow = (astate.flags & STATE_USETMUW) ? &vertex->tmuvtx[0].oow : &vertex->oow;
					actVertexPtr->z = AddWEmuBias(actVertexPtr->rhw = (astate.vertexmode & VMODE_WUNDEFINED) ? GetClampedWValue(*pOow) : *pOow);
					LOG (1, "\n		actVertexPtr->rhw = %f", actVertexPtr->rhw);
				}
				break;
			case (VMODE_WBUFF):
				{
					float* pOow = (astate.flags & STATE_USETMUW) ? &vertex->tmuvtx[0].oow : &vertex->oow;
					AddWBias((astate.vertexmode & VMODE_WUNDEFINED) ? GetClampedWValue(*pOow) : *pOow, &actVertexPtr->rhw);
					actVertexPtr->z = 0.5f;
				}
				break;
		}
		if (astate.fogmode == GR_FOG_WITH_TABLE)	{
			i = WToInd[GetFogIndexValue(vertex->oow, &w)];
			actVertexPtr->specular = GetFogIntValue ( ((w-IndToW[i]) * fogtable[i+1] + (IndToW[i+1]-w) * fogtable[i])/(IndToW[i+1]-IndToW[i]) );
		}
		actVertexPtr++;
	}
	
	if (primIndexed)	{
		_asm	{
			mov		edx,nVerts
			mov		esi,this
			mov		edi,[esi]DX9Drawing.actIndexPtr
			mov		ecx,[esi]DX9Drawing.numberOfVertices
			lea		edx,[edx-2]
			lea		eax,[ecx+1]
#ifndef DEBUG
			cmp		[esi]DX9Drawing.indexSizeInBytes,4
			je		_32bitIndices
#endif
		_nextTriangleIndices16:
			mov		[edi],cx
			lea		edi,[edi+6]
			mov		[edi-4],ax
			inc		eax
			mov		[edi-2],ax
			nop
			dec		edx
			jnz		_nextTriangleIndices16
#ifndef DEBUG
			jmp		_indexGenOk

		_32bitIndices:
			mov		[edi],ecx
			lea		edi,[edi+12]
			mov		[edi-8],eax
			inc		eax
			mov		[edi-4],eax
			nop
			dec		edx
			jnz		_32bitIndices

		_indexGenOk:
#endif
			mov		[esi]DX9Drawing.actIndexPtr,edi
		}
		numberOfIndices += 3 * numOfPrimitives;
	}
	numberOfVertices += nVerts;
	numberOfPrimitives += numOfPrimitives;
	
	stats.trisProcessed += numOfPrimitives;

	_asm	{
		fstp	st(0)
		fldcw	[CTW]
	}

	return (1);
	//DEBUG_PERF_MEASURE_END(&fpu_perf);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
	return 1;
}


void		DX9Drawing::FlushCachedVertices ()
{
//unsigned long cop,carg1,carg2, alphaop,aarg1,aarg2;
//DWORD abe, asrc, adst;
//DWORD clipping;

	//lpD3Ddevice->GetRenderState(D3DRENDERSTATE_CLIPPING, &clipping);
	/*lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(0,D3DTSS_ALPHAARG2,&aarg2);
	//LOG(0,"\n  stage0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache0: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[0],astate.colorarg1[0],astate.colorarg2[0],astate.alphaop[0],astate.alphaarg1[0],astate.alphaarg2[0]);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(1,D3DTSS_ALPHAARG2,&aarg2);
	//LOG(0,"\n  stage1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache1: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[1],astate.colorarg1[1],astate.colorarg2[1],astate.alphaop[1],astate.alphaarg1[1],astate.alphaarg2[1]);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLOROP,&cop);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLORARG1,&carg1);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_COLORARG2,&carg2);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAOP,&alphaop);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAARG1,&aarg1);
	lpD3Ddevice->GetTextureStageState(2,D3DTSS_ALPHAARG2,&aarg2);
	//LOG(0,"\n  stage2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",cop,carg1,carg2,alphaop,aarg1,aarg2);
	//LOG(0,"\n  cache2: cop: %d, carg1: %d, carg2: %d,  alphaop: %d, aarg1: %d, aarg2: %d",astate.colorop[2],astate.colorarg1[2],astate.colorarg2[2],astate.alphaop[2],astate.alphaarg1[2],astate.alphaarg2[2]);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &abe);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &asrc);
	lpD3Ddevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &adst);*/

	//dx7General->WaitUntilBufferSwitchDone ();

//	if (dx9LfbDepth->IsCompareStencilModeUsed ())
//		EnableStencilMode (1);

	if (vibFlags & VIB_ISINUSE)	{
		lpD3DVertexBuffer9->Unlock ();
		lpD3DIndexBuffer9->Unlock ();
		if (numberOfVertices) {
			if (primIndexed)	{
				lpD3Ddevice9->DrawIndexedPrimitive (primType, actOffsetInVB, 0, numberOfVertices, actOffsetInIB, numberOfPrimitives);
			} else {
				lpD3Ddevice9->DrawPrimitive(primType, actOffsetInVB, numberOfPrimitives);
			}
		}
		actOffsetInVB += numberOfVertices;
		actOffsetInIB += numberOfIndices;
		if (tooManyVertex || (vbVertexBuffSize == actOffsetInVB) || (ibIndexBuffSize == actOffsetInIB))	{
			lpD3DVertexBuffer9->Lock (actOffsetInVB = 0, 0, (void**) &actVertexPtr, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
			lpD3DIndexBuffer9->Lock (actOffsetInIB = 0, 0, (void**) &actIndexPtr, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
		} else {
			lpD3DVertexBuffer9->Lock (actOffsetInVB * sizeof(D3DTLVERTEX), (vbVertexBuffSize - actOffsetInVB) * sizeof(D3DTLVERTEX), (void**) &actVertexPtr, D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
			lpD3DIndexBuffer9->Lock (actOffsetInIB * indexSizeInBytes, (ibIndexBuffSize - actOffsetInIB) * indexSizeInBytes, (void**) &actIndexPtr, D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
		}
		tooManyVertex = 0;

	} else {
		if (primIndexed)	{
			lpD3Ddevice9->DrawIndexedPrimitiveUP (primType, 0, numberOfVertices, numberOfPrimitives, memIndexBuffPtr,
												  (indexSizeInBytes == 2) ? D3DFMT_INDEX16 : D3DFMT_INDEX32, memVertexBuffPtr,
												  sizeof(D3DTLVERTEX));
		} else
			lpD3Ddevice9->DrawPrimitiveUP (primType, numberOfPrimitives, memVertexBuffPtr, sizeof(D3DTLVERTEX));
		
		actVertexPtr = memVertexBuffPtr;
		actIndexPtr = memIndexBuffPtr;
		actOffsetInVB = actOffsetInIB = 0;
	}		
	numberOfVertices = numberOfIndices = numberOfPrimitives = 0;
	
//	if (dx9LfbDepth->IsCompareStencilModeUsed ())
//		EnableStencilMode (0);

	// Az adott primitivek kirajzolásának kényszeritése
	if (renderbuffer == GR_BUFFER_FRONTBUFFER)
		dx9General->ShowContentChangesOnFrontBuffer ();
}


void		DX9Drawing::SetPrimitiveType (enum PrimType primitiveType)
{

	D3DPRIMITIVETYPE d3dPrimType = primitiveTypesLookUp[primitiveType];
	DWORD			 d3dIndexed = indexedTypeLookUp[primitiveType];
	DWORD			 d3dClipped = clippedTypeLookUp[primitiveType];

	this->primitiveType = primitiveType;
	primType = d3dPrimType;
	primIndexed = d3dIndexed;
	if (primClipping != d3dClipped) {
		lpD3Ddevice9->SetRenderState(D3DRS_CLIPPING, primClipping = d3dClipped);
		if (primClipping) {
			SwitchToNormalBuffer ();
			noVertexConverting = 1;
		} else {
			SwitchToVertexBufferIfAvailable ();
			noVertexConverting = 0;
		}
	}
}


void		DX9Drawing::DrawTile(float left, float top, float right, float bottom, 
								 float tuLeft, float tvTop, float tuRight, float tvBottom,
								 float oow, float ooz, unsigned int constColor) {


	D3DTLVERTEX* vertexPtr;
	DWORD lockFlags = ((tileNumInBuffers != 0) ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD) | D3DLOCK_NOSYSLOCK;
	lpD3DTileVertexBuffer9->Lock (tileNumInBuffers * 4 * sizeof(D3DTLVERTEX),
								  4 * sizeof(D3DTLVERTEX), (void**) &vertexPtr, lockFlags);
	
	
	void* indexPtr;
	lpD3DTileIndexBuffer9->Lock (tileNumInBuffers * 6 * indexSizeInBytes,
								 6 * indexSizeInBytes, (void**) &indexPtr, lockFlags);
	
	vertexPtr[0].x = vertexPtr[2].x = left;
	vertexPtr[0].y = vertexPtr[1].y = top;
	vertexPtr[1].x = vertexPtr[3].x = right;
	vertexPtr[2].y = vertexPtr[3].y = bottom;
	vertexPtr[0].tu = vertexPtr[2].tu = tuLeft;
	vertexPtr[0].tv = vertexPtr[1].tv = tvTop;
	vertexPtr[1].tu = vertexPtr[3].tu = tuRight;
	vertexPtr[2].tv = vertexPtr[3].tv = tvBottom;
	vertexPtr[0].rhw = vertexPtr[1].rhw = vertexPtr[2].rhw = vertexPtr[3].rhw = oow;
	vertexPtr[0].z = vertexPtr[1].z = vertexPtr[2].z = vertexPtr[3].z = ooz;
	vertexPtr[0].diffuse = vertexPtr[1].diffuse = vertexPtr[2].diffuse = vertexPtr[3].diffuse = constColor;


	void* indexSrc = (indexSizeInBytes == 2) ? ((void*) tileIndex16) : ((void*) tileIndex32);
	CopyMemory (indexPtr, indexSrc, 6 * indexSizeInBytes);

	lpD3DTileIndexBuffer9->Unlock ();
	lpD3DTileVertexBuffer9->Unlock ();

	lpD3Ddevice9->SetStreamSource (0, lpD3DTileVertexBuffer9, 0, sizeof (D3DTLVERTEX));
	lpD3Ddevice9->SetIndices (lpD3DTileIndexBuffer9);

	lpD3Ddevice9->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, tileNumInBuffers * 4, 0, 4, tileNumInBuffers * 6, 2);

	lpD3Ddevice9->SetStreamSource (0, lpD3DVertexBuffer9, 0, sizeof (D3DTLVERTEX));
	lpD3Ddevice9->SetIndices (lpD3DIndexBuffer9);

	if ((++tileNumInBuffers) == MAXTILENUMINBUFFERS)
		tileNumInBuffers = 0;
}


void		DX9Drawing::BeforeBufferSwap ()
{
	/*if (vibFlags & VIB_ISINUSE) {
		lpD3DVertexBuffer9->Unlock ();
		lpD3DIndexBuffer9->Unlock ();
	}*/
}


void		DX9Drawing::AfterBufferSwap ()
{
	/*if (vibFlags & VIB_ISINUSE) {
		lpD3DVertexBuffer9->Lock (actOffsetInVB * sizeof(D3DTLVERTEX), (vbVertexBuffSize - actOffsetInVB) * sizeof(D3DTLVERTEX), (void**) &actVertexPtr, D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
		lpD3DIndexBuffer9->Lock (actOffsetInIB * indexSizeInBytes, (ibIndexBuffSize - actOffsetInIB) * indexSizeInBytes, (void**) &actIndexPtr, D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
	}*/
}


int			DX9Drawing::Init (int useHardwareBuffers)
{
	dx9Drawing = this;

	DEBUGCHECK(0, d3dCaps9.MaxVertexW < 65528.0f, \
		"\n   Warning: DX9Drawing::Init: Maximum W-based depth value that device supports is %f, while Glide requires 65528.0!", d3dCaps9.MaxVertexW);
	DEBUGCHECK(1, d3dCaps9.MaxVertexW < 65528.0f, \
		"\n   Figyelmeztetés: DX9Drawing::Init: Az eszköz által támogatott maximális W-alapú mélységi érték %f, míg a Glide 65528.0-t követel meg!", d3dCaps9.MaxVertexW);


	unsigned int maxVertexCount = DRAW_MAX_VERTEXNUM;

	if ((maxPrimitiveCount = d3dCaps9.MaxPrimitiveCount) == 0xFFFF) {
		maxVertexCount = MIN (d3dCaps9.MaxPrimitiveCount, DRAW_MAX_VERTEXNUM);
	}

	ibIndexBuffSize = MIN (d3dCaps9.MaxVertexIndex, DRAW_MAX_INDEXNUM);

#ifndef DEBUG
	indexSizeInBytes = (d3dCaps9.MaxVertexIndex > 0xFFFF) ? 4 : 2;
#else
	indexSizeInBytes = 2;
#endif


	memVertexBuffPtr = NULL;
	memIndexBuffPtr = NULL;

	numberOfVertices = numberOfIndices = numberOfPrimitives = 0;

	//primType = D3DPT_TRIANGLELIST;
	//primIndexed = primClipping = FALSE;

	lpD3Ddevice9->SetRenderState(D3DRS_CLIPPING, FALSE);

	noVertexConverting = 0;

	vbVertexBuffSize = 3 * maxVertexCount;

	if ( (actVertexPtr = memVertexBuffPtr = (D3DTLVERTEX *) _GetMem(vbVertexBuffSize * sizeof(D3DTLVERTEX))) == NULL)
		return(0);		

	if ( (actIndexPtr = memIndexBuffPtr = (unsigned short *) _GetMem(ibIndexBuffSize * sizeof(short))) == NULL )	{
		_FreeMem(memVertexBuffPtr);
		memVertexBuffPtr = NULL;
		return(0);
	}

	vibFlags = 0;
	vibPool = useHardwareBuffers ? D3DPOOL_DEFAULT : D3DPOOL_SYSTEMMEM;

	CreateVertexAndIndexBuffer ();

	lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int i = 0; i < MAX_TEXSTAGES; i++)
		lpD3Ddevice9->SetTextureStageState (i, D3DTSS_TEXCOORDINDEX, 0);

	tooManyVertex = 0;

	SetPrimitiveType (primitiveType = ClippedIndexedTriangle);

	return (1);
}
	

void		DX9Drawing::DeInit ()
{
	if (vibFlags & VIB_AVAILABLE)	{	
		if (lpD3DVertexBuffer9)	{
			lpD3DVertexBuffer9->Unlock();
		}
		if (lpD3DIndexBuffer9) {
			lpD3DIndexBuffer9->Unlock();
		}
		ReleaseDefaultPoolResources ();
	}
	
	if (memVertexBuffPtr != NULL)
		_FreeMem(memVertexBuffPtr);
	
	if (memIndexBuffPtr != NULL)
		_FreeMem(memIndexBuffPtr);
}


void		DX9Drawing::SetDepthBias (FxI16 level, int wBuffEmuUsed, int stencilCompareDepth)
{
	if (!stencilCompareDepth)	{
		d3d9_depthBias_W = ((int) level) << 11;
		d3d9_depthBias_Z = (float) level;
	} else {
		d3d9_depthBias_W = 0;
		d3d9_depthBias_Z = 0.0f;
		d3d9_depthBias_cz = DrawGetFloatFromDepthValue (level, wBuffEmuUsed ? DVNonlinearZ : DVLinearZ);
		d3d9_depthBias_cw = DrawGetFloatFromDepthValue (level, DVTrueW);
	}
}


void		DX9Drawing::ReleaseDefaultPoolResources ()
{
	if (vibFlags & VIB_AVAILABLE) {
		if (lpD3DVertexBuffer9)
			lpD3DVertexBuffer9->Release();
		if (lpD3DIndexBuffer9)
			lpD3DIndexBuffer9->Release ();

		if (lpD3DTileVertexBuffer9)
			lpD3DTileVertexBuffer9->Release();
		if (lpD3DTileIndexBuffer9)
			lpD3DTileIndexBuffer9->Release ();

		lpD3DVertexBuffer9 = lpD3DTileVertexBuffer9 = NULL;
		lpD3DIndexBuffer9 = lpD3DTileIndexBuffer9 = NULL;
	}
}


int			DX9Drawing::ReallocDefaultPoolResources ()
{
	return CreateVertexAndIndexBuffer ();
}