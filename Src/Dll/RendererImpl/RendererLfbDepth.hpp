/*--------------------------------------------------------------------------------- */
/* RendererLfbDepth.hpp - dgVoodoo 3D rendering abstract interface for              */
/*                        Linear/Depth buffers                                      */
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
/* dgVoodoo: RendererLfbDepth.hpp															*/
/*			 Az LfbDepth interfésze															*/
/*------------------------------------------------------------------------------------------*/

#include	"RendererTypes.hpp"

class	RendererLfbDepth
{
public:
	enum LfbUsage {
		ColorBuffersOnly	=	0,			// No scaling buffers
		OneScalingBuffer	=	1,			// Scaling buffer only for backbuffer
		Complex				=	2			// Scaling buffer for each buffers
	};

	enum BufferType {
		ColorBuffer			=	0,
		ScalingBuffer		=	1
	};

	enum BlitType {
		Copy				=	0,
		CopyWithColorKey	=	1
	};

	enum DepthBufferingMode {
	
		ZBuffering						=	0,
		WBufferingEmu					=	1,
		WBuffering						=	2,
		None							=	3,
		Unknown							=	4
	};

	enum DepthBufferingDetectMethod {

		AlwaysUseZ						=	0,
		AlwaysUseW						=	1,
		DetectViaDriver					=	2,
		DrawingTest						=	3		//Obsolete!!
	};

public:
	virtual int					InitLfb (LfbUsage lfbUsage, int useHwScalingBuffers, int realVoodoo) = 0;
	virtual void				DeInitLfb () = 0;

	virtual int					InitDepthBuffering (enum DepthBufferingDetectMethod detectMethod) = 0;
	virtual void				DeInitDepthBuffering () = 0;

	virtual int					GetLfbFormat (_pixfmt* format) = 0;
	virtual void				ClearBuffers () = 0;
	
	virtual void				FillScalingBuffer (int buffer, RECT* rect, unsigned int colorKey, int mirrored,
												   int pointerIsInUse) = 0;
	virtual void				BlitToScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, int mirrored, int scaled,
													 int pointerIsInUse) = 0;
	virtual void				BlitFromScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, BlitType blitType,
													   int mirrored, int scaled) = 0;
	virtual int					IsBlitTypeSupported (BlitType blitType) = 0;
	virtual int					IsMirroringSupported () = 0;
	virtual int					CopyBetweenScalingBuffers (int srcBuffer, int dstBuffer, RECT* rect) = 0;
	virtual int					AreHwScalingBuffersUsed () = 0;

	virtual int					CreateAuxBuffers (unsigned int width, unsigned int height) = 0;
	virtual void				DestroyAuxBuffers () = 0;
	virtual void				BlitToAuxBuffer (int buffer, RECT* srcRect) = 0;
	virtual void				BlitFromAuxBuffer (int buffer, unsigned int x, unsigned y) = 0;

	virtual void				RotateBufferProperties (int numOfBuffers, int rotateBuffers) = 0;

	virtual int					LockBuffer (int buffer, BufferType bufferType, void** buffPtr, unsigned int* pitch) = 0;
	virtual int					UnlockBuffer (int buffer, BufferType bufferType) = 0;

	virtual int					CreatePrimaryPalette () = 0;
	virtual void				DestroyPrimaryPalette () = 0;
	virtual void				SetPrimaryPaletteEntries (PALETTEENTRY* entries) = 0;

	virtual void				SetDepthBufferingMode (GrDepthBufferMode_t mode) = 0;
	virtual void				SetDepthBufferFunction (GrCmpFnc_t function) = 0;
	virtual void				EnableDepthMask (int enable) = 0;
	virtual void				SetDepthBias (FxI16 level) = 0;
	virtual DepthBufferingMode  GetTrueDepthBufferingMode () = 0;
};