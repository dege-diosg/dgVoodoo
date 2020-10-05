/*--------------------------------------------------------------------------------- */
/* DosMouseNt.c - Glide virtual mouse emulated by DirectInput7 for WinNT            */
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
/* dgVoodoo: Mouse.h																		*/
/*			 DOS-egértámogatás																*/
/*------------------------------------------------------------------------------------------*/

#include <windows.h>


typedef struct	{

short			irqevent;
short			irqmove_x;
short			irqmove_y;
short			res_x;
short			res_y;
short			clientarea_x_l;
short			clientarea_x_r;
short			clientarea_y_l;
short			clientarea_y_r;
short			pointer_x;
short			pointer_x_mickey;
short			pointer_y;
short			pointer_y_mickey;
short			sumofmove_x;
short			sumofmove_y;
short			buttonstate;
unsigned short	ratio_x;
unsigned short	ratio_y;
short			ds_threshold;
short			presscount[3];
short			releasecount[3];
short			lastpressx[3];
short			lastpressy[3];
short			lastreleasex[3];
short			lastreleasey[3];
short			eventmask;
unsigned long	eventhandler;
short			eventmask_ex;
unsigned long	eventhandler_ex[3];
	
} MouseDriverInfo;



int		NTInstallMouse(void *handle);
int		NTUnInstallMouse();
int		NTIsMouseInstalled ();
void	NTInitMouse();
void	DIMouseAcquire();
void	DIMouseUnacquire();

LRESULT NTLocalMouseWinHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

