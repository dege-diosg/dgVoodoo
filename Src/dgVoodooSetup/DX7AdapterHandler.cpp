/*--------------------------------------------------------------------------------- */
/* DX7AdapterHandler.cpp - dgVoodooSetup DX7 video adapter handling interface impl  */
/*                                                                                  */
/* Copyright (C) 2005 Dege                                                          */
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


#include "AdapterHandler.hpp"
#include "DX7BaseFunctions.h"

class	DX7AdapterHandler:	public IAdapterHandler
{
private:
	struct DD7RefreshRateCallbackParameters
	{
		IRefreshRateEnumCallback&	callback;
		unsigned int				xRes;
		unsigned int				yRes;
		unsigned int				bitDepth;
	};

public:
	virtual const char*		GetApiName ()
	{
		return "DirectDraw7 & Direct3D7";
	}

	virtual	bool			Init ()
	{
		return InitDX7 () != 0;
	}

	virtual	void			ShutDown ()
	{
		UninitDX7 ();
	}
	
	virtual	unsigned int	GetNumOfAdapters ()
	{
		return GetNumOfDX7Adapters ();
	}

	virtual	const char*		GetDescriptionOfAdapter (unsigned int index)
	{
		return GetDescriptionOfDX7Adapter ((unsigned short) index);
	}

	virtual	unsigned int	GetNumOfDeviceTypes (unsigned int /*adapter*/)
	{
		return GetNumOf3D7Devices ();
	}

	virtual	const char*		GetDescriptionOfDeviceType (unsigned int /*adapter*/, unsigned int index)
	{
		return GetDescriptionOf3D7Device ((unsigned short) index);
	}

	static HRESULT WINAPI EnumerateResolutionsCallbackDX7 (LPDDSURFACEDESC2 lpddsd, LPVOID lpContext)
	{
		if ( (lpddsd->ddpfPixelFormat.dwRGBBitCount != 16) && (lpddsd->ddpfPixelFormat.dwRGBBitCount != 32) )
			return DDENUMRET_OK;

		IResolutionEnumCallback& callback = *((IResolutionEnumCallback*) (lpContext));
		callback.AddResolution (lpddsd->dwWidth, lpddsd->dwHeight);

		return DDENUMRET_OK;
	}

	void					EnumResolutions (unsigned int adapter, IResolutionEnumCallback& callback)
	{
		if (CreateDirectDraw7OnAdapter (adapter)) {
			LPDIRECTDRAW7 lpDD = GetDirectDraw7 ();

			lpDD->EnumDisplayModes (0, NULL, (LPVOID) &callback, EnumerateResolutionsCallbackDX7);

			DestroyDirectDraw7 ();
		}
	}

	static HRESULT WINAPI	EnumerateRefreshRatesCallbackDX7 (LPDDSURFACEDESC2 lpddsd, LPVOID lpContext)
	{
		struct DD7RefreshRateCallbackParameters* params = (struct DD7RefreshRateCallbackParameters*) lpContext;
		
		if ( (lpddsd->ddpfPixelFormat.dwRGBBitCount == params->bitDepth) &&
			 (lpddsd->dwWidth == params->xRes) && (lpddsd->dwHeight == params->yRes) &&
			 (lpddsd->dwRefreshRate != 0) )	{

				 params->callback.AddRefreshRate (lpddsd->dwRefreshRate);
		}
		return DDENUMRET_OK;
	}

	void					EnumRefreshRates (unsigned int adapter, unsigned int xRes, unsigned int yRes,
											  unsigned int bitDepth, IRefreshRateEnumCallback& callback)
	{
		if (CreateDirectDraw7OnAdapter (adapter)) {
			LPDIRECTDRAW7 lpDD = GetDirectDraw7 ();

			struct DD7RefreshRateCallbackParameters params = {callback, xRes, yRes, bitDepth};

			lpDD->EnumDisplayModes (DDEDM_REFRESHRATES, NULL, &params, EnumerateRefreshRatesCallbackDX7);

			DestroyDirectDraw7 ();
		}
	}

	bool					GetCurrentDisplayMode (unsigned int& xRes, unsigned int& yRes,
												   unsigned int& bitDepth, unsigned int& refRate)
	{
		DX7DisplayMode	dx7DisplayMode;
		if (CreateDirectDraw7OnAdapter (0)) {
			if (GetCurrentDD7AdapterDisplayMode (&dx7DisplayMode, NULL)) {
				xRes = dx7DisplayMode.xRes;
				yRes = dx7DisplayMode.yRes;
				bitDepth = dx7DisplayMode.bitDepth;
				refRate = dx7DisplayMode.refreshRate;
			}
			DestroyDirectDraw7 ();
			return true;
		}
		return false;
	}
};


IAdapterHandler*	GetDX7AdapterHandler ()
{
	static	DX7AdapterHandler	handler;

	return &handler;
}
