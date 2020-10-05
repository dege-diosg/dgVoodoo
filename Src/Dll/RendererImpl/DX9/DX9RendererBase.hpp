/*--------------------------------------------------------------------------------- */
/* DX9RendererBase.hpp - dgVoodoo 3D rendering interface                            */
/*                       base class of each renderer class                          */
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
/* dgVoodoo: DX9RendererBase.hpp															*/
/*			 DX9 renderer base class														*/
/*------------------------------------------------------------------------------------------*/
#ifndef	DX9RENDERERBASE_HPP
#define	DX9RENDERERBASE_HPP


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

extern "C" {

#include	"MMConv.h"

}

#include "D3d9.h"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define		MAX_TEXSTAGES			4				/* Direct3D-ben a használt textúra stage-ek max száma */


// ------------------------------------------------------------------------------------------
// ----- Predeclarations --------------------------------------------------------------------

class	DX9General;
class	DX9Drawing;
class	DX9LfbDepth;
class	DX9Texturing;


// ------------------------------------------------------------------------------------------
// ----- DX9 base renderer class ------------------------------------------------------------

class	DX9RendererBase
{
protected:
	static	unsigned int			adapter;
	static  D3DDEVTYPE				deviceType;
	static  D3DFORMAT				deviceDisplayFormat;
	static  D3DFORMAT				renderTargetFormat;

	static	LPDIRECT3D9				lpD3D9;
	static	LPDIRECT3DDEVICE9		lpD3Ddevice9;
	static	LPDIRECT3DSURFACE9		lpD3Ddepth9;
	static	D3DCAPS9				d3dCaps9;
	static  DWORD					cmpFuncLookUp[8];
	static	LPDIRECT3DSURFACE9		renderBuffers[3];
	static  int						actRenderTarget;

	static	int						usePixelShaders;

	static	DX9General*				dx9General;
	static	DX9Drawing*				dx9Drawing;
	static	DX9LfbDepth*			dx9LfbDepth;
	static	DX9Texturing*			dx9Texturing;

protected:
	void	ConvertToPixFmt (D3DFORMAT d3dFormat, _pixfmt* pixFmt);

public:
	DX9RendererBase ();
	~DX9RendererBase ();
};


#endif