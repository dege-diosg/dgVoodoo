/*--------------------------------------------------------------------------------- */
/* VxDComm.h - Include for communication between Server and VxD component           */
/*             Used only with Win9x                                                 */
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
/* dgVoodoo: VxdComm.h																		*/
/*			 Include for communication between Server and VxD component						*/
/*			 (only for Win9x/Me)															*/
/*------------------------------------------------------------------------------------------*/

#ifndef	VXDCOMM_H
#define VXDCOMM_H

#include "Dos.h"

/*------------------------------------------------------------------------------------------*/
/* Adatok, amelyeket a szerver átad a kernelmodulnak, amikor regisztrálja magát				*/
typedef struct	{

	HANDLE			workerThread;			/* Szerverszál kezelôje */
	CommArea		**commAreaPtr;			/* pointer címe, ami a kommunikációs területet címzi */
	unsigned int	videoMemSize;			/* Emulált videokártya memóriája (VESA) */
	dgVoodooConfig	*configPtr;				/* Konfig struktúra címe */
	unsigned int	errorCode;				/* Hibakód */

} VxdRegInfo;


/*------------------------------------------------------------------------------------------*/
/* A dgVoodoo VXD által visszaadott hibakódok (regisztráláskor)								*/

#define DGSERVERR_NONE				0
#define DGSERVERR_ALREADYREGISTERED 1
#define DGSERVERR_INITVESAFAILED	2


/*------------------------------------------------------------------------------------------*/
/* A szerver által hívott szolgáltatások (a VXD-hez, csak Win9x/Me alatt)                     */

#define	DGSERVER_SETFOCUSONVM			0x1
#define DGSERVER_REGSERVER				0x2
#define DGSERVER_UNREGSERVER			0x3
#define DGSERVER_RETURNVM				0x4
#define	DGSERVER_SETFOCUSONSYSVM		0x5
#define	DGSERVER_SETALLFOCUSONSYSVM		0x6
#define	DGSERVER_SETALLFOCUSONVM		0x7
#define DGSERVER_SETORIGFOCUSONVM		0x8
#define DGSERVER_BLOCKWORKERTHREAD		0x9
#define DGSERVER_WAKEUPWORKERTHREAD		0xA
#define DGSERVER_RELEASEKBDMOUSEFOCUS	0x10

#define DGSERVER_REGISTERMODE			0xB
#define DGSERVER_REGISTERACTMODE		0xC
#define DGSERVER_MOUSEEVENTINVM			0xD
#define DGSERVER_SUSPENDVM				0xE
#define DGSERVER_RESUMEVM				0xF

#endif /* VXDCOMM_H */