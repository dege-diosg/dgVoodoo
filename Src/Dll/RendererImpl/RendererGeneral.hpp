/*--------------------------------------------------------------------------------- */
/* RendererGeneral.hpp - dgVoodoo 3D general rendering abstract interface           */
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
/* dgVoodoo: RendererGeneral.hpp															*/
/*			 A General interfésze															*/
/*------------------------------------------------------------------------------------------*/

// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "dgVoodooGlide.h"

// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

/* Flags for working mode */

#define		SWM_WINDOWED				1
#define		SWM_3D						2
#define		SWM_USECLOSESTFREQ			4
#define		SWM_MULTITHREADED			8


/* Flags for buffer clearing */

#define		CLEARFLAG_COLORBUFFER		1
#define		CLEARFLAG_DEPTHBUFFER		2


/* Flags of general renderer caps */

#define		CAP_FRONTBUFFAVAILABLE		1


// ------------------------------------------------------------------------------------------
// ----- Interfaces -------------------------------------------------------------------------

/* Interface of renderer general functions */

class	RendererGeneral
{
public:
	typedef	struct {

		unsigned int	xRes, yRes;
		//unsigned int	bitDepth;
		unsigned int	refreshRate;
		_pixfmt			pixFmt;

	} DisplayMode;


	typedef struct {
	
		unsigned short	red[256];
		unsigned short	green[256];
		unsigned short	blue[256];
	
	} GammaRamp;

	
	enum DeviceStatus
	{
		StatusOk			=	0,
		StatusLostCanBeReset=	1,
		StatusLost			=	2
	};

	
	enum ColorKeyMethod
	{
		Disabled			=	0,
		Native				=	1,
		AlphaBased			=	2,
		NativeTNT			=	3
	};

	
	enum AlphaCkModeInPipeline
	{
		AlphaDisabled		=	0,
		AlphaForFncGreater  =	1,
		AlphaForFncLess		=	2
	};


	enum RenderStateSaveType {
		ScreenShotLabel		=	0,
		LfbNoPipeline		=	1,
		LfbPipeline			=	2,
		GammaCorrection		=	3,
		GrayScaleRendering	=	4
	};


	enum DestAlphaBlendFactor {
		DestAlphaFactorZero	=	0,
		DestAlphaFactorOne	=	1
	};

	
	typedef				void	(*DISPLAYMODEENUMCALLBACK) (struct RendererGeneral::DisplayMode* displayMode);

public:
	virtual void		PrepareForInit (HWND rendererWnd) = 0;
	virtual	int			InitRendererApi () = 0;
	virtual void		UninitRendererApi () = 0;
	virtual int			GetApiCaps () = 0;
	virtual int			SetWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										unsigned int bufferNum, HWND hWnd, unsigned short adapter, unsigned short device,
										unsigned int flags) = 0;

	virtual int			ModifyWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										   unsigned int bufferNum, unsigned int flags) = 0;

	virtual void		RestoreWorkingMode () = 0;

	virtual int			GetCurrentDisplayMode (unsigned short adapter, unsigned short device, DisplayMode* displayMode) = 0;
	virtual int			EnumDisplayModes (unsigned short adapter, unsigned short device, unsigned int byRefreshRates,
										  DISPLAYMODEENUMCALLBACK callbackFunc) = 0;
	virtual void		WriteOutRendererCaps () = 0;

	virtual int			CreateFullScreenGammaControl (GammaRamp* originalGammaRamp) = 0;
	virtual void		DestroyFullScreenGammaControl () = 0;
	virtual void		SetGammaRamp (GammaRamp* gammaRamp) = 0;

	virtual DeviceStatus GetDeviceStatus () = 0;
	virtual void		PrepareForDeviceReset () = 0;
	virtual int			DeviceReset () = 0;
	virtual int			ReallocResources () = 0;

	virtual int 		BeginScene () = 0;
	virtual void		EndScene () = 0;
	virtual void		ShowContentChangesOnFrontBuffer () = 0;
	
	virtual int			RefreshDisplayByBuffer (int buffer) = 0;
	virtual void		SwitchToNextBuffer (unsigned int swapInterval, unsigned int renderBuffer) = 0;
	virtual int			WaitUntilBufferSwitchDone () = 0;
	virtual int			GetNumberOfPendingBufferFlippings () = 0;
	virtual int			GetVerticalBlankStatus () = 0;
	virtual unsigned int GetCurrentScanLine () = 0;

	virtual int			SetRenderTarget (int buffer) = 0;

	virtual void		ClearBuffer (unsigned int flags, RECT* rect, unsigned int color, float depth) = 0;
	virtual	void		SetClipWindow (RECT* clipRect) = 0;

	virtual void		SetCullMode (GrCullMode_t mode) = 0;

	virtual void		SetColorAndAlphaWriteMask (FxBool rgb, FxBool alpha) = 0;

	virtual void		EnableAlphaBlending (int enable) = 0;
	virtual void		SetRgbAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc) = 0;
	virtual void		SetAlphaAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc) = 0;
	virtual void		SetDestAlphaBlendFactorDirectly (DestAlphaBlendFactor destAlphaBlendFactor) = 0;

	virtual	void		EnableAlphaTesting (int enable) = 0;
	virtual void		SetAlphaTestFunction (GrCmpFnc_t function) = 0;
	virtual	void		SetAlphaTestRefValue (GrAlpha_t value) = 0;

	virtual void		SetFogColor (unsigned int fogColor) = 0;
	virtual void		EnableFog (int enable) = 0;

	virtual void		SetDitheringMode (GrDitherMode_t mode) = 0;

	virtual void		SetConstantColor (unsigned int constantColor) = 0;

	virtual void		ConfigureTexelPixelPipeLine (GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor, 
													 GrCombineFunction_t alpha_function, GrCombineFactor_t alpha_factor,
													 FxBool rgb_invert, FxBool alpha_invert) = 0;
	virtual void		ConfigureColorPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert) = 0;
	virtual void		ConfigureAlphaPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert) = 0;

	virtual void		ComposeConcretePixelPipeLine () = 0;

	virtual void		GetCkMethodPreferenceOrder (ColorKeyMethod*	ckMethodPreference) = 0;
	virtual int			IsAlphaTestUsedForAlphaBasedCk () = 0;

	virtual int			EnableAlphaColorkeying (AlphaCkModeInPipeline alphaCkModeInPipeline, int alphaOutputUsed) = 0;
	virtual int 		EnableNativeColorKeying (int enable, int forTnt) = 0;

	virtual int			EnableAlphaItRGBLighting (int enable) = 0;
    
	virtual int			GetTextureUsageFlags () = 0;
	virtual int			GetNumberOfTextureStages () = 0;

	virtual void		SaveRenderState (RenderStateSaveType renderStateSaveType) = 0;
	virtual void		RestoreRenderState (RenderStateSaveType renderStateSaveType) = 0;
	virtual void		SetRenderState (RenderStateSaveType renderStateSaveType, int lfbEnableNativeCk, int lfbUseConstAlpha,
										unsigned int lfbConstAlpha) = 0;

	virtual int			UsesPixelShaders () = 0;

	virtual int			Is3DNeeded () = 0;
};
