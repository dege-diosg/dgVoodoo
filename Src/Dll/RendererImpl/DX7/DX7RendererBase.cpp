/*--------------------------------------------------------------------------------- */
/* DX7RendererBase.cpp - dgVoodoo 3D rendering interface                            */
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
/* dgVoodoo: DX7RendererBase.cpp															*/
/*			 DX7 renderer base class implementation											*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "DX7RendererBase.hpp"


// ------------------------------------------------------------------------------------------
// ----- Static members ---------------------------------------------------------------------


LPDIRECTDRAW7			DX7RendererBase::lpDD = NULL;
LPDIRECT3D7				DX7RendererBase::lpD3D = NULL;
LPDIRECT3DDEVICE7		DX7RendererBase::lpD3Ddevice = NULL;
unsigned short			DX7RendererBase::adapter;
unsigned short			DX7RendererBase::device;
LPDIRECTDRAWSURFACE7	DX7RendererBase::lpDDdepth = NULL;
LPDIRECTDRAWSURFACE7	DX7RendererBase::renderBuffers[3];
unsigned int			DX7RendererBase::zBitDepth;
unsigned int			DX7RendererBase::stencilBitDepth;


DX7General*				DX7RendererBase::dx7General;
DX7Drawing*				DX7RendererBase::dx7Drawing;
DX7LfbDepth*			DX7RendererBase::dx7LfbDepth;


DWORD					DX7RendererBase::cmpFuncLookUp[8] = { D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL,
															  D3DCMP_GREATER, D3DCMP_NOTEQUAL, D3DCMP_GREATEREQUAL, 
															  D3DCMP_ALWAYS };


// ------------------------------------------------------------------------------------------
// ----- Member functions -------------------------------------------------------------------

void	DX7RendererBase::ConvertDDPixelFormatToPixFmt (LPDDPIXELFORMAT ddpf, _pixfmt *pf, int createMissingAlpha)
{
	unsigned int	pos, count, mask, fullMask;

	pf->BitPerPixel = ddpf->dwRGBBitCount;
	fullMask = mask = ddpf->dwRBitMask;
	for (pos = count = 0; mask != 0x0;) {
		while (!(mask & 0x1)) { pos++; mask >>= 1; }
		while (mask != 0x0) { count++; mask >>= 1; }
	}
	pf->RPos = pos;
	pf->RBitCount = count;

	fullMask |= mask = ddpf->dwGBitMask;
	for (pos = count = 0; mask != 0x0;) {
		while (!(mask & 0x1)) { pos++; mask >>= 1; }
		while (mask != 0x0) { count++; mask >>= 1; }
	}
	pf->GPos = pos;
	pf->GBitCount = count;

	fullMask |= mask = ddpf->dwBBitMask;
	for (pos = count = 0; mask != 0x0;) {
		while (!(mask & 0x1)) { pos++; mask >>= 1; }
		while (mask != 0x0) { count++; mask >>= 1; }
	}
	pf->BPos = pos;
	pf->BBitCount = count;

	if ((mask = ddpf->dwRGBAlphaBitMask) == 0x0 && createMissingAlpha) {
		mask = ~fullMask;
		if (ddpf->dwRGBBitCount == 16)
			mask &= 0xFFFF;
	}
	for (pos = count = 0; mask != 0x0;) {
		while (!(mask & 0x1)) { pos++; mask >>= 1; }
		while (mask != 0x0) { count++; mask >>= 1; }
	}
	pf->APos = pos;
	pf->ABitCount = count;
}


int		DX7RendererBase::RestoreSurface (LPDIRECTDRAWSURFACE7 lpSurface)
{
	switch (lpSurface->IsLost ()) {
		case DD_OK:
			break;
		case DDERR_SURFACELOST:
			if (lpSurface->Restore () != DD_OK) {
				return (0);
			}
			break;
		default:
			return (0);
	}
	return (1);
}