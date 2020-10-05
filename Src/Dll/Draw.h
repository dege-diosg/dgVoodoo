/*--------------------------------------------------------------------------------- */
/* Draw.h - Glide implementation, stuffs related to poly/primitive drawing          */
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
/* dgVoodoo: Draw.h																			*/
/*			 Primitívek rajzolásához kapcsolódó függvények									*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DRAW_H
#define		DRAW_H


#include <Glide.h>

/*------------------------------------------------------------------------------------------*/
/*....................................... Definíciók .......................................*/


/* Konstansok egy vertex Glide-ról Direct3D-re való konvertálásának módjáról */
#define VMODE_ZBUFF					0				/* Z-buffer használata */
#define VMODE_WBUFFEMU				1				/* W-buffer emuláció: ooz kiszámítása */
#define VMODE_WBUFF					2				/* W-buffer használata */
#define VMODE_TEXTURE				8				/* textúrakoordináták kellenek */
#define VMODE_DIFFUSE				16				/* diffuse színinfo kell */
#define VMODE_ALPHAFOG				32				/* köd a diffuse alphájában */
#define VMODE_WUNDEFINED			64				/* A W-érték meghatározatlan (W-pufferelés, de mélységi függvény=ALWAYS) */
#define VMODE_NODRAW				128				/* Egyáltalán nincs rajzolás */
#define VMODE_DEPTHBUFFMMODESELECT	(VMODE_ZBUFF | VMODE_WBUFFEMU | VMODE_WBUFF)

extern unsigned int qw;

enum DepthValueMappingType {
	DVLinearZ = 0,
	DVNonlinearZ,
	DVTrueW
};


/*------------------------------------------------------------------------------------------*/
/*.................................. Globális változók .....................................*/

/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

void		DrawFlushPrimitives();
void		DrawComputeScaleCoEffs(GrOriginLocation_t origin);
float		DrawGetFloatFromDepthValue (FxU16 depth, enum DepthValueMappingType mappingType);
void		DrawTile(float left, float top, float right, float bottom, 
					 float tuLeft, float tvTop, float tuRight, float tvBottom,
					 float oow, float ooz, unsigned int constColor);
void		DrawBeforeSwap ();
void		DrawAfterSwap ();
int			DrawInit();
void		DrawCleanUp();

/*------------------------------------------------------------------------------------------*/
/*............................. Glide-függvények predeklarációja ...........................*/

void EXPORT	grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c );

void EXPORT grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] );

void EXPORT guDrawPolygonVertexListWithClip( int nVerts, const GrVertex vlist[] );

void EXPORT grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] );

void EXPORT grDrawPolygon( int nVerts, int ilist[], const GrVertex vlist[] );

void EXPORT grDrawPlanarPolygon( int nVerts, int ilist[], const GrVertex vlist[] );

void EXPORT grAADrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c, FxBool antialiasAB, FxBool antialiasBC, FxBool antialiasCA);

void EXPORT grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[]);

void EXPORT grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[]);

void EXPORT guDrawTriangleWithClip( const GrVertex *a, const GrVertex *b, const GrVertex *c );

void EXPORT guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc );

void EXPORT grDrawLine(GrVertex *a, GrVertex *b);

void EXPORT grAADrawLine(GrVertex *a, GrVertex *b);

void EXPORT grDrawPoint( const GrVertex *a );

void EXPORT grAADrawPoint(GrVertex *p);

#endif