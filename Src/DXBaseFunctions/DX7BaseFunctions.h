/*--------------------------------------------------------------------------------- */
/* DX7BaseFunctions.h - dgVoodoo base DX7 stuff include                             */
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

#include	<Windows.h>
#include	<string.h>

#include	"DDraw.h"
#include	"D3d.h"


typedef	struct {

	unsigned int	xRes, yRes;
	unsigned int	bitDepth;
	unsigned int	refreshRate;

} DX7DisplayMode;


int					InitDX7 ();
void				UninitDX7 ();
int					EnumDX7Adapters ();
unsigned short		GetNumOfDX7Adapters ();
char*				GetDescriptionOfDX7Adapter (unsigned short adapter);
int					CreateDirectDraw7OnAdapter (unsigned short adapter);
void				DestroyDirectDraw7 ();
LPDIRECTDRAW7		GetDirectDraw7 ();
int					GetCurrentDD7AdapterDisplayMode (DX7DisplayMode* dx7DisplayMode, LPDDPIXELFORMAT lpDDpf);
int					CreateDirect3D7 ();
void				DestroyDirect3D7 ();
LPDIRECT3D7			GetDirect3D7 ();
int					Enum3DDevicesOnDirect3D7 ();
unsigned short		GetNumOf3D7Devices ();
char*				GetDescriptionOf3D7Device (unsigned short device);
int					IsHwDevice (unsigned short device);
const GUID*			GetDeviceGUID (unsigned short device);