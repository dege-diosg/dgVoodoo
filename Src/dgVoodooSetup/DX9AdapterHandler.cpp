/*--------------------------------------------------------------------------------- */
/* DX9AdapterHandler.cpp - dgVoodooSetup DX9 video adapter handling interface impl  */
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
#include "DX9BaseFunctions.h"

class	DX9AdapterHandler:	public IAdapterHandler
{
public:
	virtual const char*		GetApiName ()
	{
		return "Direct3D9";
	}

	virtual	bool			Init ()
	{
		return InitDX9 () != 0;
	}

	virtual	void			ShutDown ()
	{
		UninitDX9 ();
	}
	
	virtual	unsigned int	GetNumOfAdapters ()
	{
		return GetNumOfDX9Adapters ();
	}

	virtual	const char*		GetDescriptionOfAdapter (unsigned int index)
	{
		return GetDescriptionOfDX9Adapter ((unsigned short) index);
	}

	unsigned int			GetNumOfDeviceTypes (unsigned int adapter)
	{
		return GetNumOfSupportedD3D9DeviceTypes (adapter);
	}

	const char*				GetDescriptionOfDeviceType (unsigned int adapter, unsigned int index) 
	{
		return GetDescriptionOfD3D9DeviceType (adapter, (unsigned short) index);
	}

	void					EnumResolutions (unsigned int adapter, IResolutionEnumCallback& callback)
	{
		LPDIRECT3D9		lpD3D9 = NULL;
		D3DFORMAT		displayFormatsD3D9[3] = {D3DFMT_X8R8G8B8, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5};

		lpD3D9 = GetDirect3D9 ();
		for (int i = 0; i < 3; i++) {
			unsigned int modeCount = lpD3D9->GetAdapterModeCount (adapter, displayFormatsD3D9[i]);

			D3DDISPLAYMODE pMode;

			for (unsigned int j = 0; j < modeCount; j++) {
				lpD3D9->EnumAdapterModes (adapter, displayFormatsD3D9[i], j, &pMode);
				callback.AddResolution (pMode.Width, pMode.Height);
			}
		}
	}

	void					EnumRefreshRates (unsigned int adapter, unsigned int xRes, unsigned int yRes,
											  unsigned int bitDepth, IRefreshRateEnumCallback& callback)
	{
		LPDIRECT3D9		lpD3D9 = NULL;
		D3DFORMAT		displayFormatsD3D9[3] = {D3DFMT_X8R8G8B8, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5};
		unsigned int	displayFormatBitDepth[3] = {32, 16, 16};

		lpD3D9 = GetDirect3D9 ();
		for (unsigned int i = 0; i < 3; i++) {
			if (displayFormatBitDepth[i] == bitDepth) {
				unsigned int modeCount = lpD3D9->GetAdapterModeCount (adapter, displayFormatsD3D9[i]);

				D3DDISPLAYMODE pMode;

				for (unsigned int j = 0; j < modeCount; j++) {
					lpD3D9->EnumAdapterModes (adapter, displayFormatsD3D9[i], j, &pMode);

					if (xRes == pMode.Width && yRes == pMode.Height) {
						callback.AddRefreshRate (pMode.RefreshRate);
					}
				}
			}
		}
	}


	bool					GetCurrentDisplayMode (unsigned int& xRes, unsigned int& yRes,
												   unsigned int& bitDepth, unsigned int& refRate)
	{
		DX9DisplayMode mode;
		if (GetDisplayModeDX9 (mode, 0)) {
			xRes = mode.xRes;
			yRes = mode.yRes;
			bitDepth = mode.bitDepth;
			refRate = mode.refreshRate;
			return true;
		}
		return false;
	}
};


IAdapterHandler*	GetDX9AdapterHandler ()
{
	static	DX9AdapterHandler	handler;

	return &handler;
}
