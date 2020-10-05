/*--------------------------------------------------------------------------------- */
/* DX9RendererBase.cpp - dgVoodoo 3D rendering interface                            */
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
/* dgVoodoo: DX9RendererBase.cpp															*/
/*			 DX9 renderer base class implementation											*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "DX9RendererBase.hpp"


// ------------------------------------------------------------------------------------------
// ----- Static members ---------------------------------------------------------------------

unsigned int			DX9RendererBase::adapter;
D3DDEVTYPE				DX9RendererBase::deviceType;
D3DFORMAT				DX9RendererBase::deviceDisplayFormat;
D3DFORMAT				DX9RendererBase::renderTargetFormat;

LPDIRECT3D9				DX9RendererBase::lpD3D9;
LPDIRECT3DDEVICE9		DX9RendererBase::lpD3Ddevice9;
LPDIRECT3DSURFACE9		DX9RendererBase::lpD3Ddepth9;
D3DCAPS9				DX9RendererBase::d3dCaps9;

LPDIRECT3DSURFACE9		DX9RendererBase::renderBuffers[3];
int						DX9RendererBase::actRenderTarget;
int						DX9RendererBase::usePixelShaders;

DX9General*				DX9RendererBase::dx9General;
DX9Drawing*				DX9RendererBase::dx9Drawing;
DX9LfbDepth*			DX9RendererBase::dx9LfbDepth;
DX9Texturing*			DX9RendererBase::dx9Texturing;


DWORD					DX9RendererBase::cmpFuncLookUp[8] = { D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL,
															  D3DCMP_GREATER, D3DCMP_NOTEQUAL, D3DCMP_GREATEREQUAL, 
															  D3DCMP_ALWAYS };


// ------------------------------------------------------------------------------------------
// ----- Member functions -------------------------------------------------------------------

DX9RendererBase::DX9RendererBase ()
{
	dx9General = NULL;
	dx9Drawing = NULL;
	dx9LfbDepth = NULL;
}


DX9RendererBase::~DX9RendererBase ()
{
}


void	DX9RendererBase::ConvertToPixFmt (D3DFORMAT d3dFormat, _pixfmt* pixFmt)
{
	switch (d3dFormat) {
		case D3DFMT_A8R8G8B8:
			pixFmt->BitPerPixel = 32;
			pixFmt->RBitCount = pixFmt->GBitCount = pixFmt->BBitCount = pixFmt->ABitCount = 8;
			pixFmt->RPos = 16;
			pixFmt->GPos = 8;
			pixFmt->BPos = 0;
			pixFmt->APos = 24;
			break;

		case D3DFMT_X8R8G8B8:
			pixFmt->BitPerPixel = 32;
			pixFmt->RBitCount = pixFmt->GBitCount = pixFmt->BBitCount = 8;
			pixFmt->ABitCount = 0;
			pixFmt->RPos = 16;
			pixFmt->GPos = 8;
			pixFmt->BPos = 0;
			pixFmt->APos = 0;
			break;

		case D3DFMT_R5G6B5:
			pixFmt->BitPerPixel = 16;
			pixFmt->RBitCount = pixFmt->BBitCount = 5;
			pixFmt->GBitCount = 6;
			pixFmt->ABitCount = 0;
			pixFmt->RPos = 11;
			pixFmt->GPos = 5;
			pixFmt->BPos = 0;
			pixFmt->APos = 0;
			break;

		case D3DFMT_X1R5G5B5:
			pixFmt->BitPerPixel = 16;
			pixFmt->RBitCount = pixFmt->BBitCount = pixFmt->GBitCount = 5;
			pixFmt->ABitCount = 0;
			pixFmt->RPos = 10;
			pixFmt->GPos = 5;
			pixFmt->BPos = 0;
			pixFmt->APos = 0;
			break;

		case D3DFMT_A1R5G5B5:
			pixFmt->BitPerPixel = 16;
			pixFmt->RBitCount = pixFmt->BBitCount = pixFmt->GBitCount = 5;
			pixFmt->ABitCount = 1;
			pixFmt->RPos = 10;
			pixFmt->GPos = 5;
			pixFmt->BPos = 0;
			pixFmt->APos = 15;
			break;
		
		case D3DFMT_A4R4G4B4:
			pixFmt->BitPerPixel = 16;
			pixFmt->RBitCount = pixFmt->BBitCount = pixFmt->GBitCount = pixFmt->ABitCount = 4;
			pixFmt->RPos = 8;
			pixFmt->GPos = 4;
			pixFmt->BPos = 0;
			pixFmt->APos = 12;
			break;

		case D3DFMT_P8:
		case D3DFMT_L8:
			ZeroMemory (pixFmt, sizeof (_pixfmt));
			pixFmt->BitPerPixel = 8;
			break;

		case D3DFMT_A8L8:
			ZeroMemory (pixFmt, sizeof (_pixfmt));
			pixFmt->BitPerPixel = 8;
			pixFmt->ABitCount = 8;
			break;

		default:
			break;
	}
}