/*--------------------------------------------------------------------------------- */
/* LfbDepth.h - Glide implementation, stuffs related to Linear Frame / Depth buffer */
/*                                                                                  */
/* Copyright (C) 2004 Dege                                                          */
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
/* dgVoodoo: LfbDepth.h																		*/
/*			 Linear frame buffer és depth buffering függvények								*/
/*------------------------------------------------------------------------------------------*/

#ifndef		LFBDEPTH_H
#define		LFBDEPTH_H


#include	"MMConv.h"

/* Konstansok a depthbuffering állapotához */
#define DBUFFER_Z		1
#define DBUFFER_W		2

/*------------------------------------------------------------------------------------------*/
/*.................................. Globális változók .....................................*/

extern	DDSURFACEDESC2	lfb;
extern	_pixfmt			backBufferFormat;
extern	unsigned int	depthbuffering;


/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

int						LfbInit();
void					LfbCleanUp();
void					LfbSetBuffDirty(GrBuffer_t buffer);
void					GlideLFBBeforeSwap();
void					GlideLFBAfterSwap();
void					GlideLFBCheckLock(int buffer);
int						LfbGetConvBuffXRes();
void					GlideFlushBufferWrites(int buffer);
int						LfbSetUpLFBDOSBuffers(unsigned char *buff0, unsigned char *buff1, unsigned char *buff2, int onlyclient);
int						LfbDepthBufferingInit();
void					LfbDepthBufferingCleanUp();
void					LfbGetBackBufferFormat();


/*------------------------------------------------------------------------------------------*/
/*............................. Glide-függvények predeklarációja ...........................*/


FxBool			EXPORT1	grLfbLock(GrLock_t type, 
								  GrBuffer_t buffer, 
								  GrLfbWriteMode_t writeMode,
								  GrOriginLocation_t origin, 
								  FxBool pixelPipeline, 
								  GrLfbInfo_t *info);

FxBool			EXPORT1 grLfbUnlock(GrLock_t type, GrBuffer_t buffer);

#ifdef GLIDE1

void			EXPORT	grLfbBypassMode( GrLfbBypassMode_t mode );

void			EXPORT	grLfbOrigin(GrOriginLocation_t origin);

void			EXPORT	grLfbWriteMode( GrLfbWriteMode_t mode );

void			EXPORT	grLfbBegin();

void			EXPORT	grLfbEnd();

unsigned int	EXPORT	grLfbGetReadPtr(GrBuffer_t buffer );

unsigned int	EXPORT	grLfbGetWritePtr(GrBuffer_t buffer );

void			EXPORT	guFbReadRegion( int src_x, int src_y, int w, int h, void *dst, const int strideInBytes );

void			EXPORT	guFbWriteRegion( int dst_x, int dst_y, int w, int h, const void *src, const int strideInBytes );

#endif


FxBool			EXPORT1 grLfbWriteRegion( GrBuffer_t dst_buffer, 
										FxU32 dst_x, FxU32 dst_y, 
										GrLfbSrcFmt_t src_format, 
										FxU32 src_width, FxU32 src_height, 
										FxI32 src_stride, void *src_data );

FxBool			EXPORT1 grLfbReadRegion(	GrBuffer_t src_buffer, 
										FxU32 src_x, FxU32 src_y, 
										FxU32 src_width, FxU32 src_height, 
										FxU32 dst_stride, void *dst_data );

void			EXPORT	grLfbConstantAlpha( GrAlpha_t alpha );

void			EXPORT grLfbConstantDepth( FxU16 depth );

void			EXPORT	grLfbWriteColorFormat( GrColorFormat_t cFormat);

void			EXPORT	grLfbWriteColorSwizzle(FxBool swizzleBytes, FxBool swapWords);

void			EXPORT	grDepthBufferMode( GrDepthBufferMode_t mode );

void			EXPORT	grDepthBufferFunction( GrCmpFnc_t func );

void			EXPORT	grDepthMask( FxBool enable );

void			EXPORT	grDepthBiasLevel(FxI16 level);





void GlideSetVertexWUndefined();

#endif