/*--------------------------------------------------------------------------------- */
/* DX7BaseFunctions.cpp - dgVoodoo base DX7 stuff implmenetation                    */
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


#include	"dgVoodooBase.h"
#include	"DX7BaseFunctions.h"

typedef		HRESULT (WINAPI *DIRECTDRAWCREATEEXTYPE) (GUID FAR *, LPVOID *, REFIID iid, IUnknown FAR *);
typedef		HRESULT (WINAPI *DIRECTDRAWENUMERATEEXTYPE) (LPDDENUMCALLBACKEX, LPVOID, DWORD);
															
HINSTANCE					hDDrawDll;
DIRECTDRAWCREATEEXTYPE		pDirectDrawCreateEx;
DIRECTDRAWENUMERATEEXTYPE	pDirectDrawEnumerateEx;


LPDIRECTDRAW7				lpDD;
LPDIRECT3D7					lpD3D;

unsigned short	adapterNum;
struct {

	GUID	guid;
	int		nullGUID;
	char	driverDescription[128];

} adapterDatas[8];


unsigned short	deviceNum;
struct {

	GUID	guid;
	char	driverDescription[128];
	int		isHardware;

} deviceDatas[8];


int				InitDX7 ()
{
	if ((hDDrawDll = LoadLibrary ("DDRAW.DLL")) != NULL) {
		pDirectDrawCreateEx = (DIRECTDRAWCREATEEXTYPE) GetProcAddress (hDDrawDll, "DirectDrawCreateEx");
		pDirectDrawEnumerateEx = (DIRECTDRAWENUMERATEEXTYPE) GetProcAddress (hDDrawDll, "DirectDrawEnumerateExA");

		if ((pDirectDrawCreateEx != NULL) && (pDirectDrawEnumerateEx != NULL))
			return (1);

		UninitDX7 ();
	}
	return (0);
}


void			UninitDX7 ()
{
	FreeLibrary (hDDrawDll);
	pDirectDrawCreateEx = NULL;
	pDirectDrawEnumerateEx = NULL;
}



static BOOL WINAPI	DX7AdapterEnumCallback(GUID FAR *lpGUID, LPSTR lpDriverDescription, LPSTR /*lpDriverName*/, LPVOID /*lpContext*/,
										   HMONITOR /*hm*/)
{
	if (lpGUID == NULL) {
		adapterDatas[adapterNum].nullGUID = 1;
	} else {
		adapterDatas[adapterNum].nullGUID = 0;
		adapterDatas[adapterNum].guid = *lpGUID;
	}
	strcpy (adapterDatas[adapterNum].driverDescription, lpDriverDescription);

	if (++adapterNum == 8)
		return (0);

	return (1);
}


int				EnumDX7Adapters ()
{
	adapterNum = 0;
	HRESULT hr = (*pDirectDrawEnumerateEx) (DX7AdapterEnumCallback, NULL, DDENUM_ATTACHEDSECONDARYDEVICES);

	if (FAILED (hr))
		return (0);

	return (1);
}


unsigned short	GetNumOfDX7Adapters ()
{
	if (adapterNum == 0) {
		EnumDX7Adapters ();
	}
	return adapterNum;
}


char*			GetDescriptionOfDX7Adapter (unsigned short adapter)
{
	if (adapterNum == 0) {
		EnumDX7Adapters ();
	}
	if (adapter < adapterNum) {
		return adapterDatas[adapter].driverDescription;	
	} else {
		return NULL;
	}
}


int				CreateDirectDraw7OnAdapter (unsigned short adapter)
{
	lpDD = NULL;
	HRESULT hr = (*pDirectDrawCreateEx) (adapterDatas[adapter].nullGUID ? NULL : &adapterDatas[adapter].guid,
										 (LPVOID*) &lpDD, IID_IDirectDraw7, NULL);
	return FAILED (hr) ? 0 : 1;
}


void			DestroyDirectDraw7 ()
{
	lpDD->Release ();
	lpDD = NULL;
}


LPDIRECTDRAW7	GetDirectDraw7 ()
{
	return lpDD;
}


int				GetCurrentDD7AdapterDisplayMode (DX7DisplayMode* dx7DisplayMode, LPDDPIXELFORMAT lpDDpf)
{
DDSURFACEDESC2	ddsd;

	ZeroMemory (&ddsd, sizeof (DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);

	HRESULT hr = lpDD->GetDisplayMode (&ddsd);
	if (FAILED (hr))
		return (0);

	if ((ddsd.dwFlags & (DDSD_WIDTH | DDSD_HEIGHT)) != (DDSD_WIDTH | DDSD_HEIGHT))
		return (0);

	dx7DisplayMode->xRes = ddsd.dwWidth;
	dx7DisplayMode->yRes = ddsd.dwHeight;
	
	dx7DisplayMode->bitDepth = (ddsd.dwFlags & DDSD_PIXELFORMAT) ? ddsd.ddpfPixelFormat.dwRGBBitCount : 0;
	dx7DisplayMode->refreshRate = (ddsd.dwFlags & DDSD_REFRESHRATE) ? ddsd.dwRefreshRate : 0;

	if (lpDDpf != NULL)
		*lpDDpf = ddsd.ddpfPixelFormat;

	return (1);
}


int				CreateDirect3D7 ()
{
	return (lpDD->QueryInterface (IID_IDirect3D7, (LPVOID*) &lpD3D) == S_OK);
}


void			DestroyDirect3D7 ()
{
	lpD3D->Release ();
	lpD3D = NULL;
}


LPDIRECT3D7		GetDirect3D7 ()
{
	return lpD3D;
}


static HRESULT CALLBACK DX73DDeviceEnumCallback (LPSTR /*lpDeviceDescription*/, LPSTR lpDeviceName, LPD3DDEVICEDESC7 lpD3DDeviceDesc,
												 LPVOID /*lpContext*/)
{
	deviceDatas[deviceNum].guid = lpD3DDeviceDesc->deviceGUID;
	deviceDatas[deviceNum].isHardware = (lpD3DDeviceDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) ? 1 : 0;
	strcpy (deviceDatas[deviceNum].driverDescription, lpDeviceName);

	if (++deviceNum == 8)
		return (D3DENUMRET_CANCEL);

	return (D3DENUMRET_OK);
}


int				Enum3DDevicesOnDirect3D7 ()
{
	deviceNum = 0;
	if (lpD3D != NULL) {
		HRESULT hr = lpD3D->EnumDevices (DX73DDeviceEnumCallback, NULL);
		if (FAILED (hr)) {
			lpD3D->Release ();
			return (0);
		}
		return (1);
	}
	return (0);
}


unsigned short	GetNumOf3D7Devices ()
{
	if (deviceNum == 0) {
		if (CreateDirectDraw7OnAdapter (0)) {
			if (CreateDirect3D7 ()) {
				Enum3DDevicesOnDirect3D7 ();
				DestroyDirect3D7 ();
			}
			DestroyDirectDraw7 ();
		}
	}
	return deviceNum;
}


char*			GetDescriptionOf3D7Device (unsigned short device)
{
	if (deviceNum == 0) {
		if (CreateDirectDraw7OnAdapter (0)) {
			if (CreateDirect3D7 ()) {
				Enum3DDevicesOnDirect3D7 ();
				DestroyDirect3D7 ();
			}
			DestroyDirectDraw7 ();
		}
	}
	if (device < deviceNum) {
		return deviceDatas[device].driverDescription;
	}
	return NULL;
}


int				IsHwDevice (unsigned short device)
{
	return deviceDatas[device].isHardware;
}


const GUID*		GetDeviceGUID (unsigned short device)
{
	return &deviceDatas[device].guid;
}
