/*--------------------------------------------------------------------------------- */
/* Renderer.cpp - dgVoodoo 3D renderer factory                                      */
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
/* dgVoodoo: Renderer.cpp																	*/
/*			 A rendererhez kapcsolódó függvények											*/
/*------------------------------------------------------------------------------------------*/

#include	<windows.h>

extern "C" {

#include	"Debug.h"
#include	"Main.h"
#include	"dgVoodooBase.h"
#include	"dgVoodooConfig.h"

}

#include	"Renderer.hpp"

/*------------------------------------------------------------------------------------------*/
/*......................... A tényleges rendererek külsõ függvényei ........................*/


extern		int		CreateD3D9Renderer ();
extern		void	DestroyD3D9Renderer ();

extern		int		CreateDX7Renderer ();
extern		void	DestroyDX7Renderer ();

/*------------------------------------------------------------------------------------------*/
/*.................................. A bridge-osztályok ....................................*/


Renderer			renderer;
RendererGeneral*	rendererGeneral;
RendererTexturing*	rendererTexturing;
RendererDrawing*	rendererDrawing;
RendererLfbDepth*	rendererLfbDepth;


/*------------------------------------------------------------------------------------------*/


int Renderer::CreateRenderer ()
{
	switch (config.rendererApi) {
		case DirectX7:
			
			//DEBUG_BEGIN ("Renderer::CreateRenderer, path DX7", DebugRenderThreadId, ??);
			CreateDX7Renderer ();
			//DEBUG_END ("Renderer::CreateRenderer, path DX7", DebugRenderThreadId, ??);
			break;

		case DirectX9:
			//DEBUG_BEGIN ("Renderer::CreateRenderer, path DX9", DebugRenderThreadId, ??);
			CreateD3D9Renderer ();
			//DEBUG_END ("Renderer::CreateRenderer, path DX9", DebugRenderThreadId, ??);
			break;

		default:
			break;
	}
	
	return (1);
}


void Renderer::DestroyRenderer ()
{

	switch (config.rendererApi) {
		case DirectX7:
			DestroyDX7Renderer ();
			break;

		case DirectX9:
			DestroyD3D9Renderer ();
			break;

		default:
			break;
	}
	
	rendererGeneral = NULL;
	rendererTexturing = NULL;
	rendererDrawing = NULL;
	rendererLfbDepth = NULL;
}
