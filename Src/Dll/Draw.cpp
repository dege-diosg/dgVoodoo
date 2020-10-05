/*--------------------------------------------------------------------------------- */
/* Draw.cpp - Glide implementation, stuffs related to poly/primitive drawing        */
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

#include	<math.h>
#include	<windows.h>
#include	<winuser.h>


extern "C" {

#include	"dgVoodooBase.h"

#include	"dgVoodooConfig.h"
#include	"Dgw.h"
#include	"Version.h"
#include	"Main.h"
#include	"dgVoodooGlide.h"
#include	"Debug.h"
#include	"Log.h"
#include	"Draw.h"
#include	"LfbDepth.h"
#include	"Texture.h"
#include	<glide.h>

}

#include	"Renderer.hpp"
#include	"RendererDrawing.hpp"


/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/

#define MAX_NUM_OF_VERTEXPOINTERS	256

/* Azon primitíveket, amelyeknek nincs szüksége vágásra, egy vertex bufferen keresztül */
/* rajzolja meg, míg a többit a hagyományos módon. Ha a vertex buffer allokációja */
/* sikertelen, akkor minden rajzolás a hagyományos módon zajlik. */

#define WFAR						65536.0f
#define WNEAR						1.0f

#define CLIP_WNEAR					0.01f

#define DRAWMODE_CLIPPING			0
#define	DRAWMODE_NOCLIPPING			1
#define DRAWMODE_ANTIALIAS			2
#define DRAWMODE_ANITALIASCLIP		3

#define AAFLAG_LINEAVAILABLE		1
#define AAFLAG_TRIANGLEAVAILABLE	2
#define AAFLAG_LINEENABLED			4
#define AAFLAG_TRIANGLEENABLED		8

#define CF_LEFT						1
#define	CF_RIGHT					2
#define CF_TOP						4
#define CF_BOTTOM					8
#define CF_WNEAR					16

//#define CHECKLOCK(buffer) if (bufflocks[buffer].locktype & LOCK_ISLOCKED) GlideLFBSwitchToFastWrite(buffer);

/*------------------------------------------------------------------------------------------*/
/*................................... Globális változók.....................................*/

DWORD					perspective;					/* perspektívakorrektség igen-nem (ez fontos!)*/
unsigned char			cachedVertices;
unsigned int			drawmode;
unsigned int			linemode;
unsigned int			antialias;
float					linescalex;
float					linescaley;
const int				lineInd1[] = {0, 1, 3, 2};
const int				lineInd2[] = {0, 2, 3, 1};
const float				_Q		= WFAR / (WFAR - WNEAR);
const float				_QZn	= -((WFAR * WNEAR) / (WFAR - WNEAR));
const float				_scale4 = 4.0f;

static	enum RendererDrawing::PrimType	currentPrimType;

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

GrVertex				**vertexPointers;
__int64					sse_perf = 0;
__int64					fpu_perf = 0;

//RendererDrawing*		rendererDrawing;


/*------------------------------------------------------------------------------------------*/
/*......................... FPU-vertexkonverter, inline függvények .........................*/

//Disable 'no return value' warning
#pragma warning (disable: 4035)


/* Egy float-ról visszaadja, hogy az nemvégtelen-e */
int __forceinline IsNotInf(float f)	{
	_asm	{
		mov	eax,f
		and eax,0x7FFFFFFF
		sub eax,0x7F800000
	}
}


#pragma warning (default: 4035)


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


/* Kirajzolja az összegyûjtött primitíveket, ha vannak. */
void DrawFlushPrimitives()	{
		
	DEBUG_BEGINSCOPE("DrawFlushPrimitives", DebugRenderThreadId);

	if (cachedVertices)	{
		
		LfbCheckLock(renderbuffer);
		LfbSetBuffDirty(renderbuffer);
		
		//A glideGetState-be is áttenni!!!
		if (updateFlags & UPDATEFLAG_SETTEXTURE) GlideTexSource();
		if (updateFlags & UPDATEFLAG_RESETTEXTURE) TexUpdateMultitexturingState((TexCacheEntry *) astate.acttex);
		if (updateFlags & UPDATEFLAG_SETPALETTE) TexUpdateTexturePalette();
		if (updateFlags & UPDATEFLAG_SETNCCTABLE) TexUpdateTextureNccTable();
		
		if (updateFlags & UPDATEFLAG_COLORKEYSTATE) TexUpdateColorKeyState ();
		if (updateFlags & UPDATEFLAG_AITRGBLIGHTING) GlideAlphaItRGBLighting ();
		if (updateFlags & UPDATEFLAG_ALPHACOMBINE) GlideAlphaCombine();
		if (updateFlags & UPDATEFLAG_COLORCOMBINE) GlideColorCombine();
		if (updateFlags & UPDATEFLAG_TEXCOMBINE) GlideTexCombine();
		GlideRecomposePixelPipeLine ();
		if (updateFlags & (UPDATEFLAG_ALPHATESTFUNC | UPDATEFLAG_ALPHATESTREFVAL)) GlideUpdateAlphaTestState ();
		if (updateFlags & UPDATEFLAG_RESETTEXTURE) TexSetCurrentTexture((TexCacheEntry *)astate.acttex);

		rendererDrawing->FlushCachedVertices ();

		cachedVertices = 0;
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void	ConvertAndCacheVertices (GrVertex **vlist, int nVerts, int numOfPrimitives)
{
	if (!rendererDrawing->ConvertAndCacheVertices (vlist, nVerts, numOfPrimitives)) {
		DrawFlushPrimitives ();
		rendererDrawing->ConvertAndCacheVertices (vlist, nVerts, numOfPrimitives);
	}
	cachedVertices = 1;

	if ((renderbuffer == GR_BUFFER_FRONTBUFFER) && (!(config.debugFlags & DBG_FLUSHONLYWHENNEEDED)))
		DrawFlushPrimitives();
}


/* Háromszög vágás nélkül */
void EXPORT grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )	{

enum RendererDrawing::PrimType primType;

	DEBUG_BEGINSCOPE("grDrawTriangle",DebugRenderThreadId);
	
	LOG(0,"\ngrDrawTriangle(a, b, c)");
	LOG(0,"\n	a.x=%f, a.y=%f, b.x=%f, b.y=%f, c.x=%f, c.y=%f",a->x, a->y, b->x, b->y, c->x, c->y);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;

	primType = ((config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) ? RendererDrawing::ClippedIndexedTriangle : RendererDrawing::ClippedNonIndexedTriangle);
	if (currentPrimType != primType) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = primType);
	}
	
	vertexPointers[0] = (GrVertex *) a;
	vertexPointers[1] = (GrVertex *) b;
	vertexPointers[2] = (GrVertex *) c;
	ConvertAndCacheVertices (vertexPointers, 3, 1);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] )	{
int	i;

	DEBUG_BEGINSCOPE("grDrawPolygonVertexList", DebugRenderThreadId);

	LOG(0,"\ngrDrawPolygonVertexList(nVerts=%d,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	ConvertAndCacheVertices (vertexPointers, nVerts, nVerts-2);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guDrawPolygonVertexListWithClip( int nVerts, const GrVertex vlist[] )	{
int	i;

	DEBUG_BEGINSCOPE("guDrawPolygonVertexListWithClip", DebugRenderThreadId);
	
	LOG(0,"\nguDrawPolygonVertexListWithClip(nVerts=%d,vlist)", nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{		
		//GlideDrawTriangleWithClip(a,b,c);
		return;
	} else {
		if (currentPrimType != RendererDrawing::NonClippedIndexedTriangle) {
			DrawFlushPrimitives ();
			rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::NonClippedIndexedTriangle);
		}
		for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
		ConvertAndCacheVertices (vertexPointers, nVerts, nVerts-2);
	}
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] )	{
int	i;
	
	DEBUG_BEGINSCOPE("grDrawPlanarPolygonVertexList", DebugRenderThreadId);
	
	LOG(0,"\ngrDrawPlanarPolygonVertexList(nVerts=%d,vlist)");
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	ConvertAndCacheVertices (vertexPointers, nVerts, nVerts-2);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDrawPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
int	i;

	DEBUG_BEGINSCOPE("grDrawPolygon", DebugRenderThreadId);

	LOG(0,"\ngrDrawPolygon(nVerts=%d,ilist,vlist)", nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;

	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	ConvertAndCacheVertices(vertexPointers, nVerts, nVerts-2);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grDrawPlanarPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
int	i;

	DEBUG_BEGINSCOPE("grDrawPlanarPolygon",DebugRenderThreadId);
	
	LOG(0,"\ngrDrawPlanarPolygon(nVerts=%d,ilist,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	ConvertAndCacheVertices(vertexPointers, nVerts, nVerts-2);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAADrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c,
							 FxBool antialiasAB, FxBool antialiasBC, FxBool antialiasCA)	{

enum RendererDrawing::PrimType primType;

	DEBUG_BEGINSCOPE("grAADrawTriangle",DebugRenderThreadId);
	
	LOG(0,"\ngrAADrawTriangle(a,b,c,aaAB=%s,aaBC=%s,aaCA=%s)", \
		fxbool_t[antialiasAB],fxbool_t[antialiasBC],fxbool_t[antialiasCA]);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;

	primType = (config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) ? RendererDrawing::ClippedIndexedTriangle : RendererDrawing::ClippedNonIndexedTriangle;
	if (currentPrimType != primType) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = primType);
	}

	vertexPointers[0] = a;
	vertexPointers[1] = b;
	vertexPointers[2] = c;
	ConvertAndCacheVertices(vertexPointers, 3, 1);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[]) {
int	i;

	DEBUG_BEGINSCOPE("grAADrawPolygon",DebugRenderThreadId);

	LOG(0,"\ngrAADrawPolygon(nVerts=%d,ilist,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + ilist[i];
	ConvertAndCacheVertices (vertexPointers, nVerts, nVerts-2);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[]) {
int	i;
	
	DEBUG_BEGINSCOPE("grAADrawPolygonVertexList",DebugRenderThreadId);

	LOG(0,"\ngrDrawPolygonVertexList(nVerts=%d,vlist)",nVerts);
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if ( (nVerts < 3) || (astate.vertexmode & VMODE_NODRAW) ) return;
	
	if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
	}

	for(i = 0; i < nVerts; i++) vertexPointers[i] = (GrVertex *) vlist + i;
	ConvertAndCacheVertices(vertexPointers, nVerts, nVerts-2);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
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
float t = 0.0f;
float x = 0.0f, y = 0.0f;
float ix,iy;
float ox,oy;
	
	ix = inside->x;
	iy = inside->y;
	ox = outside->x;
	oy = outside->y;
	clipflags &= ~clipedge;
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
	if (astate.hints & GR_STWHINT_W_DIFF_TMU0) {
		output->tmuvtx[0].oow = inside->tmuvtx[0].oow + t*(outside->tmuvtx[0].oow - inside->tmuvtx[0].oow);
	}
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
enum RendererDrawing::PrimType primType;
	
	DEBUG_BEGINSCOPE("guDrawTriangleWithClip",DebugRenderThreadId);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	LOG(0,"\nguDrawTriangleWithClip(a,b,c)");

	if (astate.vertexmode & VMODE_NODRAW) return;
	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{
		GlideDrawTriangleWithClip(a, b, c);
	} else {
	//grDrawTriangle(a,b,c);
	//return;			

		primType = (config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) ? RendererDrawing::NonClippedIndexedTriangle : RendererDrawing::NonClippedNonIndexedTriangle;
		if (currentPrimType != primType) {
			DrawFlushPrimitives ();
			rendererDrawing->SetPrimitiveType (currentPrimType = primType);
		}

		vertexPointers[0] = (GrVertex *) a;
		vertexPointers[1] = (GrVertex *) b;
		vertexPointers[2] = (GrVertex *) c;
		ConvertAndCacheVertices(vertexPointers, 3, 1);
	}

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc )	{
enum RendererDrawing::PrimType primType;

	DEBUG_BEGINSCOPE("guAADrawTriangleWithClip",DebugRenderThreadId);

	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	LOG(0,"\nguAADrawTriangleWithClip(a,b,c)");
	
	if (astate.vertexmode & VMODE_NODRAW) return;
	if (!(config.Flags & CFG_CLIPPINGBYD3D))	{
		GlideDrawTriangleWithClip(va,vb,vc);
		return;
	} else {	
		//grAADrawTriangle(va,vb,vc,0,0,0);
		//return;			

		primType = (config.Flags2 & CFG_ALWAYSINDEXEDPRIMS) ? RendererDrawing::NonClippedIndexedTriangle : RendererDrawing::NonClippedNonIndexedTriangle;
		if (currentPrimType != primType) {
			DrawFlushPrimitives ();
			rendererDrawing->SetPrimitiveType (currentPrimType = primType);
		}

		vertexPointers[0] = (GrVertex *) va;
		vertexPointers[1] = (GrVertex *) vb;
		vertexPointers[2] = (GrVertex *) vc;
		ConvertAndCacheVertices(vertexPointers, 3, 1);
	}	
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}

/*-------------------------------------------------------------------------------------------*/
/* Vonalak */

void EXPORT grDrawLine(GrVertex *a, GrVertex *b) {
float		vectx, vecty, len;
GrVertex	v[4];
float		a_x,a_y,b_x,b_y;
int			numOfLineVertices;

	//return;
	DEBUG_BEGINSCOPE("grDrawLine",DebugRenderThreadId);

	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrDrawLine(a,b)");

	/* Ha a felbontás miatt vastagabb vonalakat kell rajzolnunk háromszögekbôl */
	if (linemode)	{
		if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
			DrawFlushPrimitives ();
			rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
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
		ConvertAndCacheVertices(vertexPointers, numOfLineVertices, numOfLineVertices/2);
		return;
	}

	if (currentPrimType != RendererDrawing::ClippedNonIndexedLine) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedNonIndexedLine);
	}

	vertexPointers[0] = a;
	vertexPointers[1] = b;
	ConvertAndCacheVertices(vertexPointers, 2, 1);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAADrawLine(GrVertex *a, GrVertex *b) {
float		vectx, vecty, len;
GrVertex	v[4];
float		a_x,a_y,b_x,b_y;
int			numOfLineVertices;

	//return;
	DEBUG_BEGINSCOPE("grAADrawLine", DebugRenderThreadId);
	
	if ( (runflags & RF_CANFULLYRUN) != RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrAADrawLine(a,b)");

	/* Ha a felbontás miatt vastagabb vonalakat kell rajzolnunk háromszögekbôl */
	if (linemode)	{
		if (currentPrimType != RendererDrawing::ClippedIndexedTriangle) {
			DrawFlushPrimitives ();
			rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);
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
		ConvertAndCacheVertices(vertexPointers, numOfLineVertices, numOfLineVertices/2);
		return;
	}
	if (currentPrimType != RendererDrawing::ClippedNonIndexedLine) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedNonIndexedLine);
	}

	vertexPointers[0] = a;
	vertexPointers[1] = b;
	ConvertAndCacheVertices(vertexPointers, 2, 1);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}

/*-------------------------------------------------------------------------------------------*/
/* Pontok */

void EXPORT grDrawPoint( const GrVertex *a ) {

	DEBUG_BEGINSCOPE("grDrawPoint", DebugRenderThreadId);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrDrawPoint(a)");

	if (currentPrimType != RendererDrawing::ClippedPoint) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedPoint);
	}
	ConvertAndCacheVertices((GrVertex **) &a, 1, 1);
		
	DEBUG_ENDSCOPE(DebugRenderThreadId);
}


void EXPORT grAADrawPoint(GrVertex *p) {

	DEBUG_BEGINSCOPE("grAADrawPoint", DebugRenderThreadId);
	
	if ( (runflags & RF_CANFULLYRUN)!=RF_CANFULLYRUN ) return;
	if (astate.vertexmode & VMODE_NODRAW) return;
	LOG(0,"\ngrAADrawPoint(a)");
	
	if (currentPrimType != RendererDrawing::ClippedPoint) {
		DrawFlushPrimitives ();
		rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedPoint);
	}
	ConvertAndCacheVertices(&p, 1, 1);
	
	DEBUG_ENDSCOPE(DebugRenderThreadId);
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
				 float oow, float ooz, unsigned int constColor) {

	if (!(astate.flags & STATE_NORENDERTARGET))
		rendererDrawing->DrawTile (left, top, right, bottom, tuLeft, tvTop, tuRight, tvBottom, oow, ooz, constColor);
}


void		DrawBeforeSwap ()
{
	rendererDrawing->BeforeBufferSwap ();
}


void		DrawAfterSwap ()
{
	rendererDrawing->AfterBufferSwap ();
}


/*------------------------------------------------------------------------------------------*/
/*................................... Init / Cleanup .......................................*/


int	DrawInit()	{
	
	DEBUG_BEGINSCOPE("DrawInit", DebugRenderThreadId);
	
	//rendererDrawing = renderer.GetRendererDrawing ();
	astate.hints = 0;

	if ( (vertexPointers = (GrVertex **) _GetMem(MAX_NUM_OF_VERTEXPOINTERS * sizeof(GrVertex *))) == NULL)
		return(0);

	//astate.vertexmode = VMODE_ZBUFF;
	drawmode = DRAWMODE_CLIPPING;
	DrawComputeScaleCoEffs(astate.locOrig);

	linemode = (appXRes < movie.cmiXres) && (appYRes < movie.cmiYres);

	cachedVertices = 0;
	astate.gcullmode = GR_CULL_DISABLE;

	if (!rendererDrawing->Init (config.Flags & CFG_ENABLEHWVBUFFER)) {
		_FreeMem (vertexPointers);
		return (0);
	}
	rendererDrawing->SetPrimitiveType (currentPrimType = RendererDrawing::ClippedIndexedTriangle);

	stats.trisProcessed = 0;
	sse_perf = fpu_perf = 0;

	DEBUG_ENDSCOPE(DebugRenderThreadId);

	return(1);
}


void DrawCleanUp()	{

	DEBUG_BEGINSCOPE("DrawCleanUp", DebugRenderThreadId);
	
	DrawFlushPrimitives();

	if (rendererDrawing != NULL)
		rendererDrawing->DeInit ();
	
	if (vertexPointers != NULL) _FreeMem(vertexPointers);

	//f = DEBUG_GET_PERF_QUOTIENT(sse_perf,fpu_perf);
	//DEBUGLOG(2,"\nsse/fpu = %f",f);

	DEBUG_ENDSCOPE(DebugRenderThreadId);
}