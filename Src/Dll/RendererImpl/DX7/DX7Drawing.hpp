/*--------------------------------------------------------------------------------- */
/* DX7Drawing.hpp - dgVoodoo 3D polygon rendering DX7 implementation                */
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
/* dgVoodoo: DX7Drawing.hpp																	*/
/*			 A Drawing interfésze, DX7 imp													*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "DX7RendererBase.hpp"
#include "RendererDrawing.hpp"


// ------------------------------------------------------------------------------------------
// ----- DX7 primitive drawing class --------------------------------------------------------

class	DX7Drawing: public DX7RendererBase,
					public RendererDrawing
{
private:
	D3DTLVERTEX				*actvertex;						/* primitívek csúcsaira mutató pointerek */
	D3DTLVERTEX				*memvertex, *vbvertex;
	D3DTLVERTEX				*vbbuffend;
	unsigned short			*indices, *actindex;
	int						vbflags;
	LPDIRECT3DVERTEXBUFFER7 lpD3DVertexBuffer;
	LPDIRECT3DVERTEXBUFFER7 lpD3DVertexBufferTile;
	D3DPRIMITIVETYPE		primType;						/* gyûjtött primitívek típusa */
	DWORD					primClipping;
	DWORD					primIndexed;
	unsigned int			numberOfVertices;
	unsigned int			numberOfIndices;
	unsigned int			actoffsetvb;
	unsigned int			drawmode;
	unsigned int			linemode;
	unsigned int			antialias;
	int						noVertexConverting;
	int						tooManyVertex;
	int						useHwBuffers;

private:
	void					SwitchToVertexBufferIfAvailable ();
	void					SwitchToNormalBuffer ();
	void					EnableStencilMode (int enable);

	void					CreateVertexBuffers ();
	void					ReleaseVertexBuffers ();

public:
	virtual	int				ConvertAndCacheVertices (GrVertex **vlist, int nVerts, int numOfPrimitives);
	virtual void			FlushCachedVertices ();
	virtual void			SetPrimitiveType (enum PrimType primType);
	virtual void			DrawTile (float left, float top, float right, float bottom, 
									  float tuLeft, float tvTop, float tuRight, float tvBottom,
									  float oow, float ooz, unsigned int constColor);
	virtual void			BeforeBufferSwap ();
	virtual void			AfterBufferSwap ();

	virtual int				Init (int useHardwareBuffers);
	virtual void			DeInit ();

	void					SetDepthBias (FxI16 level, int wBuffEmuUsed, int stencilCompareDepth);

	void					ResourcesMayLost ();
	int						RestoreResources ();
};
