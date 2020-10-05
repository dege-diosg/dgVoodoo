/*--------------------------------------------------------------------------------- */
/* dgVoodooSetupDX7.h - dgVoodooSetup include for base DX7 stuffs                   */
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
/* dgVoodoo: dgVoodooSetupDX7.h																*/
/*			 DX7-specifikus cuccok															*/
/*------------------------------------------------------------------------------------------*/

char*				GetAPINameDX7 ();
void				EnumerateDD7AdapterModes (unsigned short adapter);
void				EnumerateDD7RefreshRates (unsigned short adapter, unsigned int xRes, unsigned int yRes, unsigned int bitDepth);