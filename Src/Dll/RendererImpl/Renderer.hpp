/*--------------------------------------------------------------------------------- */
/* Renderer.hpp - dgVoodoo 3D renderer abstract interfaces                          */
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
/* dgVoodoo: Renderer.hpp																	*/
/*			 A rendererhez kapcsolódó cuccok												*/
/*------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------*/
/*.......................... A bridge classok predeklarációja ..............................*/

class	RendererBase;
class	RendererGeneral;
class	RendererTexturing;
class	RendererDrawing;
class	RendererLfbDepth;

extern	RendererBase*		rendererBase;
extern	RendererGeneral*	rendererGeneral;
extern	RendererTexturing*	rendererTexturing;
extern	RendererDrawing*	rendererDrawing;
extern	RendererLfbDepth*	rendererLfbDepth;

/*------------------------------------------------------------------------------------------*/
/*................................... A renderer class .....................................*/


class	Renderer
{
public:
	int					CreateRenderer ();
	void				DestroyRenderer ();
};


extern	Renderer renderer;