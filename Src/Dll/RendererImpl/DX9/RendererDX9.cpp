/*--------------------------------------------------------------------------------- */
/* RendererDX9.cpp - dgVoodoo 3D renderer, DX9 implementation factory               */
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
/* dgVoodoo: RendererDX9.cpp																*/
/*			 Direct3D9-rendererhez kapcsolódó függvények									*/
/*------------------------------------------------------------------------------------------*/
extern "C" {

#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"

}

#include	"DX9General.hpp"
#include	"DX9Texturing.hpp"
#include	"DX9Drawing.hpp"
#include	"DX9LfbDepth.hpp"


extern		RendererGeneral*	rendererGeneral;
extern		RendererTexturing*	rendererTexturing;
extern		RendererDrawing*	rendererDrawing;
extern		RendererLfbDepth*	rendererLfbDepth;


/*------------------------------------------------------------------------------------------*/
/*..........................................................................................*/

int		CreateD3D9Renderer ()
{

	rendererGeneral = new DX9General;
	rendererTexturing = new DX9Texturing;
	rendererDrawing = new DX9Drawing;
	rendererLfbDepth = new DX9LfbDepth;
	return (1);
}


void	DestroyD3D9Renderer ()
{
	delete rendererGeneral;
	delete rendererTexturing;
	delete rendererDrawing;
	delete rendererLfbDepth;
}