/*--------------------------------------------------------------------------------- */
/* DX9BaseFunctions.cpp - dgVoodoo base DX9 stuff implmenetation                    */
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


#include	"DX9BaseFunctions.h"

#define		NUMOFDEVTYPES	3

typedef		IDirect3D9* (WINAPI *DIRECT3DCREATE9) (UINT SDKVersion);

															
HINSTANCE					hD3D9Dll;
DIRECT3DCREATE9				pDirect3DCreate9;

LPDIRECT3D9					lpD3D9;
LPDIRECT3DDEVICE9			lpD3Ddevice9;

D3DADAPTER_IDENTIFIER9 		d3dAdapterInfo;

static unsigned short		deviceNum;
typedef struct {

	char			driverDescription[128];
	D3DDEVTYPE		deviceType;
	bool			isSupported;

} DeviceDatas;


DeviceDatas	deviceDatas[] = {	{"Direct3D9 HAL", D3DDEVTYPE_HAL, 0},
								{"Direct3D9 Reference Renderer", D3DDEVTYPE_REF, 0},
								{"Direct3D9 Software Renderer", D3DDEVTYPE_SW, 0}};

int				InitDX9 ()
{
	if ((hD3D9Dll = LoadLibrary ("D3D9.DLL")) != NULL) {
		pDirect3DCreate9 = (DIRECT3DCREATE9) GetProcAddress (hD3D9Dll, "Direct3DCreate9");

		if (pDirect3DCreate9 != NULL) {
			lpD3D9 = pDirect3DCreate9 (D3D_SDK_VERSION);
			if (lpD3D9 != NULL)
				return (1);
		}

		UninitDX9 ();
	}
	return (0);
}


void			UninitDX9 ()
{
	if (lpD3D9 != NULL)
		lpD3D9->Release ();

	FreeLibrary (hD3D9Dll);
	pDirect3DCreate9 = NULL;
}


unsigned short		GetNumOfDX9Adapters ()
{
	return (unsigned short) lpD3D9->GetAdapterCount ();
}


char*				GetDescriptionOfDX9Adapter (unsigned short adapter)
{
	if (adapter >= lpD3D9->GetAdapterCount ())
		return NULL;

	ZeroMemory (&d3dAdapterInfo, sizeof (D3DADAPTER_IDENTIFIER9));
	lpD3D9->GetAdapterIdentifier (adapter, 0, &d3dAdapterInfo);

	return d3dAdapterInfo.Description;
}


int					EnumSupportedD3D9DeviceTypesOnAdapter (unsigned short adapter)
{
	if (adapter >= lpD3D9->GetAdapterCount ())
		return (0);

	int	i, j;
	D3DFORMAT	displayFormats[3] = {D3DFMT_X8R8G8B8, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5};
	D3DFORMAT	backBuffFormats[3] = {D3DFMT_A8R8G8B8, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5};
	
	for (i = 0; i < NUMOFDEVTYPES; i++) {
		for (j = 0; j < 3; j++) {
			if (lpD3D9->CheckDeviceType (adapter, deviceDatas[i].deviceType, displayFormats[j], backBuffFormats[j], FALSE) == D3D_OK) {
				deviceDatas[i].isSupported = true;
				break;
			}
		}
		if (j == 3) {
			deviceDatas[i].isSupported = false;
		}
	}
	return (1);
}


unsigned short		GetNumOfSupportedD3D9DeviceTypes (unsigned short adapter)
{
	int i = 0;
	if (EnumSupportedD3D9DeviceTypesOnAdapter (adapter)) {
		int j;
		for (j = 0; j < NUMOFDEVTYPES; j++) {
			if (deviceDatas[i].isSupported) 
				i++;
		}
	}
	return ((unsigned short) i);
}


D3DDEVTYPE			GetDeviceType (unsigned short device)
{
	return deviceDatas[device].deviceType;
}


char*				GetDescriptionOfD3D9DeviceType (unsigned short /*adapter*/, unsigned short device)
{
	int		i = 0;
	int		j;
	char*	deviceTypeDescription = NULL;

	for (j = 0; j < NUMOFDEVTYPES; j++) {
		if (deviceDatas[i].isSupported) {
			if (i == device) {
				deviceTypeDescription = deviceDatas[i].driverDescription;
				break;
			}
			i++;
		}
	}
	return (deviceTypeDescription);
}


LPDIRECT3D9			GetDirect3D9 ()
{
	return lpD3D9;
}


bool				GetDisplayModeDX9 (DX9DisplayMode& mode, unsigned short adapter)
{
	if (lpD3D9 != NULL) {
		D3DDISPLAYMODE dMode;
		lpD3D9->GetAdapterDisplayMode (adapter, &dMode);
		mode.xRes = dMode.Width;
		mode.yRes = dMode.Height;
		switch (dMode.Format) {
			case D3DFMT_R8G8B8:
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
			case D3DFMT_A8B8G8R8:
			case D3DFMT_X8B8G8R8:
				mode.bitDepth = 32;
				break;
			
			case D3DFMT_R5G6B5:
			case D3DFMT_X1R5G5B5:
			case D3DFMT_A1R5G5B5:
				mode.bitDepth = 16;
				break;

			default:
				mode.bitDepth = 0;
		}
		mode.refreshRate = dMode.RefreshRate;
		return true;
	}
	return false;
}