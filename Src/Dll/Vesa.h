/*--------------------------------------------------------------------------------- */
/* Vesa.h - dgVoodoo VESA rendering implementation                                  */
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
/* dgVoodoo: Vesa.h																			*/
/*			 VESA implementáció																*/
/*------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

unsigned int			VesaSetVBEMode();
void					VesaUnsetVBEMode(int flag);
HANDLE					VESAFreshThreadHandle;
int						InitVESA();
void					CleanupVESA();
int						VESAInit (void *driverHeader);
unsigned long			VESAHandler (VESARegisters *vesaRegisters);
LRESULT					VESALocalClientCommandHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT					VESAServerCommandHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int			VESAGetXRes ();
unsigned int			VESAGetYRes ();

/*------------------------------------------------------------------------------------------*/
/*..................................... Egyéb adatok .......................................*/

extern	int				VESAModeWillBeSet;

/* Default VGA/VESA paletta 3 bájtos RGB formátumban */
extern	unsigned char	vesaDefaultPalette[];
