/*--------------------------------------------------------------------------------- */
/* DX7RendererBase.hpp - dgVoodoo 3D rendering interface                            */
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
/* dgVoodoo: DX7RendererBase.hpp															*/
/*			 DX7 renderer base class														*/
/*------------------------------------------------------------------------------------------*/
#ifndef	DX7RENDERERBASE_HPP
#define	DX7RENDERERBASE_HPP


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

extern "C" {

#include	"MMConv.h"

}

#include "DDraw.h"
#include "D3d.h"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define		MAX_TEXSTAGES				4				/* Direct3D-ben a használt textúra stage-ek max száma */


// ------------------------------------------------------------------------------------------
// ----- Predeclarations --------------------------------------------------------------------

class	DX7General;
class	DX7Drawing;
class	DX7LfbDepth;


// ------------------------------------------------------------------------------------------
// ----- DX7 base renderer class ------------------------------------------------------------

class	DX7RendererBase
{
protected:
	static  DWORD					cmpFuncLookUp[8];
	static	unsigned short			adapter, device;
	static	LPDIRECTDRAW7			lpDD;
	static	LPDIRECT3D7				lpD3D;
	static	LPDIRECT3DDEVICE7		lpD3Ddevice;
	static	LPDIRECTDRAWSURFACE7	lpDDdepth;
	static	LPDIRECTDRAWSURFACE7	renderBuffers[3];
	static  unsigned int			zBitDepth, stencilBitDepth;

	static	DX7General*				dx7General;
	static	DX7Drawing*				dx7Drawing;
	static	DX7LfbDepth*			dx7LfbDepth;

protected:
	static	void	ConvertDDPixelFormatToPixFmt (LPDDPIXELFORMAT ddpf, _pixfmt *pf, int createMissingAlpha);
	static	int		RestoreSurface (LPDIRECTDRAWSURFACE7 lpSurface);
};


#endif