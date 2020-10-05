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
/*			 Primit�vek rajzol�s�hoz kapcsol�d� f�ggv�nyek									*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DRAW_H
#define		DRAW_H


#include <Glide.h>

/*------------------------------------------------------------------------------------------*/
/*....................................... Defin�ci�k .......................................*/


/* Konstansok egy vertex Glide-r�l Direct3D-re val� konvert�l�s�nak m�dj�r�l */
#define VMODE_ZBUFF					0				/* Z-buffer haszn�lata */
#define VMODE_WBUFFEMU				1				/* W-buffer emul�ci�: ooz kisz�m�t�sa */
#define VMODE_WBUFF					2				/* W-buffer haszn�lata */
#define VMODE_TEXTURE				8				/* text�rakoordin�t�k kellenek */
#define VMODE_DIFFUSE				16				/* diffuse sz�ninfo kell */
#define VMODE_ALPHAFOG				32				/* k�d a diffuse alph�j�ban */
#define VMODE_WUNDEFINED			64				/* A W-�rt�k meghat�rozatlan (W-pufferel�s, de m�lys�gi f�ggv�ny=ALWAYS) */
#define VMODE_NODRAW				128				/* Egy�ltal�n nincs rajzol�s */
#define VMODE_DEPTHBUFFMMODESELECT	(VMODE_ZBUFF | VMODE_WBUFFEMU | VMODE_WBUFF)

extern unsigned int qw;

enum DepthValueMappingType {
	DVLinearZ = 0,
	DVNonlinearZ,
	DVTrueW
};


/*------------------------------------------------------------------------------------------*/
/*.................................. Glob�lis v�ltoz�k .....................................*/

/*------------------------------------------------------------------------------------------*/
/*............................. Bels� f�ggv�nyek predeklar�ci�ja ...........................*/

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
/*............................. Glide-f�ggv�nyek predeklar�ci�ja ...........................*/

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