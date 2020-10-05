/*--------------------------------------------------------------------------------- */
/* RendererDX7.cpp - dgVoodoo 3D renderer, DX7 implementation factory               */
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
/* dgVoodoo: RendererDX7.cpp																*/
/*			 DirectDraw7 & Direct3D7-rendererhez kapcsolódó függvények						*/
/*------------------------------------------------------------------------------------------*/
extern "C" {

#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"

}

#include	"DX7General.hpp"
#include	"DX7Texturing.hpp"
#include	"DX7Drawing.hpp"
#include	"DX7LfbDepth.hpp"


extern		RendererGeneral*	rendererGeneral;
extern		RendererTexturing*	rendererTexturing;
extern		RendererDrawing*	rendererDrawing;
extern		RendererLfbDepth*	rendererLfbDepth;


/*------------------------------------------------------------------------------------------*/
/*..........................................................................................*/

int		CreateDX7Renderer ()
{

	rendererGeneral = new DX7General;
	rendererTexturing = new DX7Texturing;
	rendererDrawing = new DX7Drawing;
	rendererLfbDepth = new DX7LfbDepth;
	return (1);
}


void	DestroyDX7Renderer ()
{
	delete rendererGeneral;
	delete rendererTexturing;
	delete rendererDrawing;
	delete rendererLfbDepth;
}