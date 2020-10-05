/*--------------------------------------------------------------------------------- */
/* Main.h - Glide implementation, DLL Main                                          */
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
/* dgVoodoo: Main.h																			*/
/*			 DLL belépési pontja és a fõ globális változók									*/
/*------------------------------------------------------------------------------------------*/

#ifndef		MAIN_H
#define		MAIN_H


#include <windows.h>
#include "dgVoodooConfig.h"

//#define	 GLIDE1
//#define	 GLIDE2

/*------------------------------------------------------------------------------------------*/
/*.....................................  ...... ..................................*/

#ifdef	GLIDE2

#define	DOS_SUPPORT
#define VESA_SUPPORT

#endif

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók ...... ..................................*/

#define	MMX_SUPPORTED		1
#define	SSE_SUPPORTED		2
#define	SSE2_SUPPORTED		4


/* Az egyes platformok kódjai */
#define	PLATFORM_WINDOWS	0
#define	PLATFORM_DOSWIN9X	1
#define PLATFORM_DOSWINNT	2

#define LFBMAXPIXELSIZE		4

#define	MAXSTRINGLENGTH		1024

/*------------------------------------------------------------------------------------------*/
/*................................. Exportált függvények ...... ............................*/

void	GetString (LPSTR lpBuffer, UINT uID);

/*------------------------------------------------------------------------------------------*/
/*................................. Exportált globálisok ...... ............................*/

extern HINSTANCE		hInstance;
extern unsigned char	moduleName[256];
extern int				platform;
extern unsigned int		instructionSet;
extern dgVoodooConfig	config;
extern dgVoodooConfig	*pConfigsInHeader;
extern char				productName[];
extern int				DebugRenderThreadId;


#endif