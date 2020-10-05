/*--------------------------------------------------------------------------------- */
/* DX9Drawing.hpp - dgVoodoo 3D polygon rendering DX9 implementation                */
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
/* dgVoodoo: DX9Drawing.hpp																	*/
/*			 A Drawing interfésze, DX9 imp													*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "DX9RendererBase.hpp"
#include "RendererDrawing.hpp"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

typedef	struct {

	float		x, y, z;
	float		rhw;
	D3DCOLOR	diffuse, specular;
	float		tu, tv;

} D3DTLVERTEX;


// ------------------------------------------------------------------------------------------
// ----- DX9 primitive drawing class --------------------------------------------------------

class	DX9Drawing: public DX9RendererBase,
					public RendererDrawing
{

private:
	unsigned int			maxPrimitiveCount;
	LPDIRECT3DVERTEXBUFFER9 lpD3DVertexBuffer9;
	LPDIRECT3DINDEXBUFFER9	lpD3DIndexBuffer9;
	LPDIRECT3DVERTEXBUFFER9 lpD3DTileVertexBuffer9;
	LPDIRECT3DINDEXBUFFER9	lpD3DTileIndexBuffer9;
	LPDIRECT3DVERTEXDECLARATION9	lpD3D9VertexDecl;
	LPDIRECT3DVERTEXSHADER9			lpD3D9VertexShader;

	unsigned int			indexSizeInBytes;
	D3DTLVERTEX				*memVertexBuffPtr;
	unsigned int			vbVertexBuffSize;
	D3DTLVERTEX				*actVertexPtr;						// primitívek csúcsaira mutató pointerek
	void					*memIndexBuffPtr;
	unsigned int			ibIndexBuffSize;
	void					*actIndexPtr;						// primitívek indexeire mutató pointerek
	unsigned int			actOffsetInVB;
	unsigned int			actOffsetInIB;
	unsigned int			vibFlags;
	D3DPOOL					vibPool;
	unsigned int			tileNumInBuffers;


	enum PrimType			primitiveType;
	D3DPRIMITIVETYPE		primType;						// gyûjtött primitívek típusa
	DWORD					primClipping;
	DWORD					primIndexed;
	unsigned int			numberOfVertices;
	unsigned int			numberOfIndices;
	unsigned int			numberOfPrimitives;
	/*unsigned int			drawmode;
	unsigned int			linemode;
	unsigned int			antialias;*/
	int						noVertexConverting;
	int						tooManyVertex;

private:
	void					SwitchToVertexBufferIfAvailable ();
	void					SwitchToNormalBuffer ();
	int						CreateVertexAndIndexBuffer ();
	void					EnableStencilMode (int enable);

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
	void					ReleaseDefaultPoolResources ();
	int						ReallocDefaultPoolResources ();
};
