/*--------------------------------------------------------------------------------- */
/* Grabber.h - dgVoodoo internal header for screen capturing                        */
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


#ifndef		GRABBER_H
#define		GRABBER_H


#include <windows.h>

extern	UINT	timerHandle;

void			GrabberRestoreFrontBuff();

void			GrabberDrawLabel(int savelabelarea);
void			GrabberCleanUnvisibleBuff();
int				GrabberInit();
void			GrabberCleanUp();
void			GrabberHookDos();
void			GrabberGrabLFB(unsigned char *label, PALETTEENTRY *lpPalette);
void CALLBACK	GrabberTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void			GrabberCreateLabel(unsigned char *label);
void			GrabberDeleteLabel();

#endif