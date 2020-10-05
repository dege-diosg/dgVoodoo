/*--------------------------------------------------------------------------------- */
/* DX9BaseFunctions.h - dgVoodoo base DX9 stuff include                             */
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

#include	"D3d9.h"


typedef	struct {

	unsigned int	xRes, yRes;
	unsigned int	bitDepth;
	unsigned int	refreshRate;

} DX9DisplayMode;

int					InitDX9 ();
void				UninitDX9 ();
unsigned short		GetNumOfDX9Adapters ();
char*				GetDescriptionOfDX9Adapter (unsigned short adapter);
int					EnumSupportedD3D9DeviceTypesOnAdapter (unsigned short adapter);
unsigned short		GetNumOfSupportedD3D9DeviceTypes (unsigned short adapter);
D3DDEVTYPE			GetDeviceType (unsigned short device);
char*				GetDescriptionOfD3D9DeviceType (unsigned short adapter, unsigned short device);
LPDIRECT3D9			GetDirect3D9 ();
bool				GetDisplayModeDX9 (DX9DisplayMode& mode, unsigned short adapter);