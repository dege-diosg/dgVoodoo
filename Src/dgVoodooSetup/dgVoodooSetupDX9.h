/*--------------------------------------------------------------------------------- */
/* dgVoodooSetupDX9.h - dgVoodooSetup include for base DX9 stuffs                   */
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
/* dgVoodoo: dgVoodooSetupDX9.h																*/
/*			 DX9-specifikus cuccok															*/
/*------------------------------------------------------------------------------------------*/
int					InitDX9 ();
void				UninitDX9 ();
unsigned short		GetNumOfDX9Adapters ();
char*				GetDescriptionOfDX9Adapter (unsigned short adapter);
int					EnumSupportedD3D9DeviceTypesOnAdapter (unsigned short adapter);
unsigned short		GetNumOfSupportedD3D9DeviceTypes ();
char*				GetDescriptionOfD3D9DeviceType (unsigned short device);

char*				GetAPINameDX9 ();
void				EnumerateD3D9AdapterModes (unsigned short adapter);
void				EnumerateD3D9RefreshRates (unsigned short adapter, unsigned int xRes, unsigned int yRes, unsigned int bitDepth);