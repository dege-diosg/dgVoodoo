/*--------------------------------------------------------------------------------- */
/* DX7General.hpp - dgVoodoo 3D general rendering interface, DX7 implementation     */
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
/* dgVoodoo: DX7General.hpp																	*/
/*			 A General interfésze, DX7 imp													*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

#include "DX7RendererBase.hpp"
#include "DX7FFPixelShaders.h"
#include "RendererGeneral.hpp"


// ------------------------------------------------------------------------------------------
// ----- DX7 class of general functionality -------------------------------------------------

class	DX7General: public DX7RendererBase,
					public RendererGeneral
{
private:
	enum
	{
		FrontBuffer = 0,
		BackBuffer = 1,
		ThirdBuffer = 2
	} Buffers;

private:
	LPDIRECTDRAWCLIPPER				lpDDClipper;
	LPDIRECTDRAWGAMMACONTROL		lpDDGamma;
	
	//Egyes bufferek a láncban (full screen)
	LPDIRECTDRAWSURFACE7			lpDDSPrimary;
	LPDIRECTDRAWSURFACE7			lpDDSBack;
	LPDIRECTDRAWSURFACE7			lpDDSThird;

	HWND							hWnd;
	unsigned short					adapter, device;
	unsigned int					bufferNum;
	unsigned int					createFlags;
	DisplayMode						displayMode;
	unsigned int					renderTarget;

	DWORD							alphaBlendEnabled;
	DWORD							srcBlendFunc;
	DWORD							dstBlendFunc;
	
	unsigned int					getScanLineCount;

	static	unsigned char			grfunctranslating[];
	static	CPixelShader3Stages		pixelShadersf1[];
	static	CPixelShader3Stages		pixelShadersf0[];
	static	CPixelShader3Stages		pixelShadersfloc[];
	static	CPixelShader3Stages		pixelShadersfa[];
	static	APixelShader2Stages		alphaPixelShaders[];
	static	APixelShader2Stages		alphaPixelShadersf0[];
	static	APixelShader2Stages		alphaPixelShadersf1[];
	
	DWORD							alphaLocal, alphaOther;

	DWORD							alphaOp[MAX_TEXSTAGES];
	DWORD							alphaArg1[MAX_TEXSTAGES];
	DWORD							alphaArg2[MAX_TEXSTAGES];
	DWORD							colorOp[MAX_TEXSTAGES];
	DWORD							colorArg1[MAX_TEXSTAGES];
	DWORD							colorArg2[MAX_TEXSTAGES];
	DWORD							alphaCkAlphaOp[1];
	DWORD							alphaCkAlphaArg1[1];

	DWORD							cachedColorOp[MAX_TEXSTAGES];
	DWORD							cachedAlphaOp[MAX_TEXSTAGES];
	DWORD							cachedColorArg1[MAX_TEXSTAGES];
	DWORD							cachedAlphaArg1[MAX_TEXSTAGES];
	DWORD							cachedColorArg2[MAX_TEXSTAGES];
	DWORD							cachedAlphaArg2[MAX_TEXSTAGES];
	unsigned int					astagesnum, cstagesnum, maxUsedStageNum;

	int								nativeTntCkMethodUsed;

	DWORD							savedState[128];

private:
	static	HRESULT WINAPI	BackbufferEnumSurfacesCallBack(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDDSURFACEDESC2 lpDDSurfaceDesc,
														   LPVOID lpContext);
	static	HRESULT WINAPI  DisplayModeEnumCallBack (LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);
	
	int						Create (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
									unsigned int bufferNum, unsigned int flags);

	void					Destroy ();
	void					FlipBuffer (int atNextVerticalRetrace);
	
	static	HRESULT WINAPI	SetWorkingModeEnumRefreshRatesCallback(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);
	static	DWORD /*_inline*/ _fastcall	GlideGetStageArg(DWORD type, DWORD d3dFactor, DWORD d3dLocal, DWORD d3dOther, int texinvert);


public:
	virtual void		PrepareForInit (HWND /*rendererWnd*/) { }
	virtual	int			InitRendererApi ();
	virtual void		UninitRendererApi ();
	virtual int			GetApiCaps ();

	virtual int			SetWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										unsigned int bufferNum, HWND hWnd, unsigned short adapter, unsigned short device,
										unsigned int flags);

	virtual int			ModifyWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										   unsigned int bufferNum, unsigned int flags);

	virtual void		RestoreWorkingMode ();

	virtual int			GetCurrentDisplayMode (unsigned short adapter, unsigned short device, DisplayMode* displayMode);
	virtual int			EnumDisplayModes (unsigned short adapter, unsigned short device, unsigned int byRefreshRates,
										  DISPLAYMODEENUMCALLBACK callbackFunc);
	virtual void		WriteOutRendererCaps ();

	virtual int			CreateFullScreenGammaControl (GammaRamp* originalGammaRamp);
	virtual void		DestroyFullScreenGammaControl ();
	virtual void		SetGammaRamp (GammaRamp* gammaRamp);

	virtual DeviceStatus GetDeviceStatus ();
	virtual void		PrepareForDeviceReset ();
	virtual int			DeviceReset ();
	virtual int			ReallocResources ();

	virtual int 		BeginScene ();
	virtual void		EndScene ();
	virtual void		ShowContentChangesOnFrontBuffer ();

	virtual int 		RefreshDisplayByBuffer (int buffer);
	virtual void		SwitchToNextBuffer (unsigned int swapInterval, unsigned int renderBuffer);
	virtual int			WaitUntilBufferSwitchDone ();
	virtual int			GetNumberOfPendingBufferFlippings ();
	virtual int			GetVerticalBlankStatus ();
	virtual unsigned int GetCurrentScanLine ();

	virtual int			SetRenderTarget (int buffer);
	
	virtual void		ClearBuffer (unsigned int flags, RECT* rect, unsigned int color, float depth);
	virtual	void		SetClipWindow (RECT* clipRect);

	virtual void		SetCullMode (GrCullMode_t mode);

	virtual void		SetColorAndAlphaWriteMask (FxBool rgb, FxBool alpha);

	virtual void		EnableAlphaBlending (int enable);
	virtual void		SetRgbAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc);
	virtual void		SetAlphaAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc);
	virtual void		SetDestAlphaBlendFactorDirectly (DestAlphaBlendFactor destAlphaBlendFactor);

	virtual	void		EnableAlphaTesting (int enable);
	virtual void		SetAlphaTestFunction (GrCmpFnc_t function);
	virtual	void		SetAlphaTestRefValue (GrAlpha_t value);

	virtual void		SetFogColor (unsigned int fogColor);
	virtual void		EnableFog (int enable);

	virtual void		SetDitheringMode (GrDitherMode_t mode);

	virtual void		SetConstantColor (unsigned int constantColor);

	virtual void		ConfigureTexelPixelPipeLine (GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor, 
													 GrCombineFunction_t alpha_function, GrCombineFactor_t alpha_factor,
													 FxBool rgb_invert, FxBool alpha_invert);
	virtual void		ConfigureColorPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert);
	virtual void		ConfigureAlphaPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert);

	virtual void		ComposeConcretePixelPipeLine ();

	virtual void		GetCkMethodPreferenceOrder (ColorKeyMethod*	ckMethodPreference);
	virtual int			IsAlphaTestUsedForAlphaBasedCk ();

	virtual int			EnableAlphaColorkeying (AlphaCkModeInPipeline alphaCkModeInPipeline, int alphaOutputUsed);
	virtual int 		EnableNativeColorKeying (int enable, int forTnt);

	virtual int			EnableAlphaItRGBLighting (int enable);
	
	virtual int			GetTextureUsageFlags ();
	virtual int			GetNumberOfTextureStages ();

	virtual void		SaveRenderState (RenderStateSaveType renderStateSaveType);
	virtual void		RestoreRenderState (RenderStateSaveType renderStateSaveType);
	virtual void		SetRenderState (RenderStateSaveType renderStateSaveType, int lfbEnableNativeCk, int lfbUseConstAlpha,
										unsigned int lfbConstAlpha);

	virtual int			UsesPixelShaders ();

	virtual int			Is3DNeeded ();
};