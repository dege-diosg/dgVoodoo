/*--------------------------------------------------------------------------------- */
/* RendererDrawing.hpp - dgVoodoo 3D polygon rendering abstract interface           */
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
/* dgVoodoo: RendererDrawing.hpp															*/
/*			 A Drawing interfésze															*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "Draw.h"


// ------------------------------------------------------------------------------------------
// ----- Interface of primitive drawing -----------------------------------------------------

class	RendererDrawing
{
public:
	enum PrimType {
	
		ClippedIndexedTriangle			=	0,
		NonClippedIndexedTriangle		=	1,
		ClippedNonIndexedTriangle		=	2,
		NonClippedNonIndexedTriangle	=	3,

		ClippedIndexedLine				=	4,
		ClippedNonIndexedLine			=	5,

		ClippedPoint					=	6
	
	};

public:
	virtual	int			ConvertAndCacheVertices (GrVertex **vlist, int nVerts, int numOfPrimitives) = 0;
	virtual void		FlushCachedVertices () = 0;
	virtual void		SetPrimitiveType (enum PrimType primType) = 0;
	virtual void		DrawTile (float left, float top, float right, float bottom, 
								  float tuLeft, float tvTop, float tuRight, float tvBottom,
								  float oow, float ooz, unsigned int constColor) = 0;
	virtual void		BeforeBufferSwap () = 0;
	virtual void		AfterBufferSwap () = 0;

	virtual int			Init (int useHardwareBuffers) = 0;
	virtual void		DeInit () = 0;
};
