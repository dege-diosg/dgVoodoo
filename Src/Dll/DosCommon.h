/*--------------------------------------------------------------------------------- */
/* DosCommon.h - Header for general DOS-serving stuffs                              */
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
/* dgVoodoo: DosCommon.h																	*/
/*			 DOS-kommunikáció, általános dolgok												*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DOSCOMMON_H
#define		DOSCOMMON_H


extern	HANDLE				threadSyncEvent;
extern	HANDLE				ntSharedMemHandle;
								
extern	char				SharedMemName[];
extern	char				MutexEventName[];

extern	unsigned char		ErrorTitleEng[];
extern	unsigned char		ErrorTitleHun[];
extern	unsigned char		*errorTitle;

extern	DWORD				foregroundFlashCount;
extern	DWORD				foregroundLockTimeOut;



void	RegisterMainClass();
void	CreateWarningBox();
void	ExitDosNt ();

#endif