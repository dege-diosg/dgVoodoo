/*--------------------------------------------------------------------------------- */
/* DX9LfbDepth.hpp - dgVoodoo 3D rendering interface for                            */
/*                        Linear Frame/Depth buffers, DX9 implementation            */
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

#include "DX9RendererBase.hpp"
#include "RendererLfbDepth.hpp"


class	DX9LfbDepth: public DX9RendererBase,
					 public RendererLfbDepth
{
private:
	LPDIRECT3DTEXTURE9			lpD3DBackBufferTexture;
	LPDIRECT3DSURFACE9			lpD3DBackBufferTextureSurface;
	unsigned int				depthCaps;
	enum DepthBufferingMode		depthBufferingMode;
	D3DTEXTUREFILTERTYPE		magStretchFilter;
	D3DTEXTUREFILTERTYPE		minStretchFilter;
	unsigned int				auxBuffWidth;
	unsigned int				auxBuffHeight;
	LfbUsage					lfbUsage;
	int							realVoodoo;
	char						depthBufferCreated;
	char						compareStencilModeUsed;
	char						grayScaleRendering;

private:
	void						LfbDetermineWBufferingMethod (enum DepthBufferingDetectMethod detectMethod);
	int							LfbAllocInternalBuffers (LfbUsage lfbUsage, int realVoodoo);
	void						LfbSetupDefaultRenderStates ();
	void						LfbReleaseInternalBuffers ();
	void						LfbAllocOptionalResources ();
	void						LfbReleaseOptionalResources ();
	int							CreateDepthBuffer (unsigned int width, unsigned int height, unsigned int bitDepth,
												   int createWithStencil, unsigned int* zBitDepth,
												   unsigned int* stencilBitDepth);
	void						DestroyDepthBuffer ();

public:
	DX9LfbDepth ();
	~DX9LfbDepth ();

	int							InitLfb (LfbUsage lfbUsage, int useHwScalingBuffers, int realVoodoo);
	void						DeInitLfb ();

	int							InitDepthBuffering (enum DepthBufferingDetectMethod detectMethod);
	void						DeInitDepthBuffering ();

	virtual int					GetLfbFormat (_pixfmt* format);
	virtual void				ClearBuffers ();

	virtual void				FillScalingBuffer (int buffer, RECT* rect, unsigned int colorKey, int mirrored,
												   int pointerIsInUse);
	virtual void				BlitToScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, int mirrored, int scaled,
													 int pointerIsInUse);
	virtual void				BlitFromScalingBuffer (int buffer, RECT* srcRect, RECT* dstRect, BlitType blitType,
													   int mirrored, int scaled);
	virtual int					IsBlitTypeSupported (BlitType blitType);
	virtual int					IsMirroringSupported ();
	virtual int					CopyBetweenScalingBuffers (int srcBuffer, int dstBuffer, RECT* rect);
	virtual int					AreHwScalingBuffersUsed ();

	virtual int					CreateAuxBuffers (unsigned int width, unsigned int height);
	virtual void				DestroyAuxBuffers ();
	virtual void				BlitToAuxBuffer (int buffer, RECT* srcRect);
	virtual void				BlitFromAuxBuffer (int buffer, unsigned int x, unsigned y);

	virtual void				RotateBufferProperties (int numOfBuffers, int rotateBuffers);
	
	virtual int					LockBuffer (int buffer, BufferType bufferType, void** buffPtr, unsigned int* pitch);
	virtual int					UnlockBuffer (int buffer, BufferType bufferType);

	virtual int					CreatePrimaryPalette ();
	virtual void				DestroyPrimaryPalette ();
	virtual void				SetPrimaryPaletteEntries (PALETTEENTRY* entries);

	virtual void				SetDepthBufferingMode (GrDepthBufferMode_t mode);
	virtual void				SetDepthBufferFunction (GrCmpFnc_t function);
	virtual void				EnableDepthMask (int enable);
	virtual void				SetDepthBias (FxI16 level);
	virtual DepthBufferingMode  GetTrueDepthBufferingMode ();

	int							IsCompareStencilModeUsed () { return compareStencilModeUsed; }
	void						ReleaseDefaultPoolResources ();
	int							ReallocDefaultPoolResources ();

	int							Is3DNeeded ();
	void						ConvertToGrayScale (LPDIRECT3DSURFACE9 buffer);
};
