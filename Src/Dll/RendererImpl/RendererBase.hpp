/*--------------------------------------------------------------------------------- */
/* Renderer.hpp - dgVoodoo 3D renderer base abstract interface                      */
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


#include "dgVoodooGlide.h"

class	RendererBase
{
public:
	virtual void		ClearBuffer (unsigned int flags, RECT* rect, unsigned int color, float depth) = 0;
	virtual	void		SetClipWindow (RECT* clipRect) = 0;

	virtual void		SetCullMode (GrCullMode_t mode) = 0;

	virtual void		SetColorAndAlphaWriteMask (FxBool rgb, FxBool alpha) = 0;

	virtual void		EnableAlphaBlending (int enable) = 0;
	virtual void		SetRgbAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc) = 0;
	virtual void		SetAlphaAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc) = 0;

	virtual	void		EnableAlphaTesting (int enable) = 0;
	virtual void		SetAlphaTestFunction (GrCmpFnc_t function) = 0;
	virtual	void		SetAlphaTestRefValue (GrAlpha_t value) = 0;

	virtual void		SetFogColor (unsigned int fogColor) = 0;
	virtual void		EnableFog (int enable) = 0;

	virtual void		SetDitheringMode (GrDitherMode_t mode) = 0;

	virtual void		SetConstantColor (unsigned int constantColor) = 0;

	virtual void		ConfigureColorPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert) = 0;
	virtual void		ConfigureAlphaPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert) = 0;
};
