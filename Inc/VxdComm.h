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
/* Adatok, amelyeket a szerver �tad a kernelmodulnak, amikor regisztr�lja mag�t				*/
typedef struct	{

	HANDLE			workerThread;			/* Szerversz�l kezel�je */
	CommArea		**commAreaPtr;			/* pointer c�me, ami a kommunik�ci�s ter�letet c�mzi */
	unsigned int	videoMemSize;			/* Emul�lt videok�rtya mem�ri�ja (VESA) */
	dgVoodooConfig	*configPtr;				/* Konfig strukt�ra c�me */
	unsigned int	errorCode;				/* Hibak�d */

} VxdRegInfo;


/*------------------------------------------------------------------------------------------*/
/* A dgVoodoo VXD �ltal visszaadott hibak�dok (regisztr�l�skor)								*/

#define DGSERVERR_NONE				0
#define DGSERVERR_ALREADYREGISTERED 1
#define DGSERVERR_INITVESAFAILED	2


/*------------------------------------------------------------------------------------------*/
/* A szerver �ltal h�vott szolg�ltat�sok (a VXD-hez, csak Win9x/Me alatt)                     */

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