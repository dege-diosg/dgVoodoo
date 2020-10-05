/*--------------------------------------------------------------------------------- */
/* RMDriver.h - C-header for Real Mode Driver definitions (used by me)              */
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


/*----------------------------------------------------------------------*/
/* dgVoodoo: RMDriver.h													*/
/*			 Include for real mode parts/drivers of dgVoodoo			*/
/*----------------------------------------------------------------------*/

#define	DRIVERID	'EGED'


/* General realmode driver header */

typedef struct {

unsigned int		driverId;
unsigned short		entryPoint;             /* A belépési pont offszetje */
unsigned short		driverSize;
unsigned short		driverExtramemory;		/* az igényelt többletmemória */
unsigned short		driverSelector;

} RealModeDriverHeader;


/* Header of VESA drivers (Fake and dgVesa) */

typedef struct {

RealModeDriverHeader driverHeader;
unsigned short		vddHandle;
unsigned int		origInt10Handler;
unsigned short		psp;
unsigned short		env;
unsigned short		protTable;
unsigned short		protTableLen;
unsigned char		semafor;

} VesaDriverHeader;


/* Header of mouse driver */

typedef struct {

RealModeDriverHeader driverHeader;
unsigned short		driverInfoStruct;		/* A driverinfo struktúra offszetje */
unsigned int		origInt33Handler;		/* Az eredeti kezelõ címét tárolja */
unsigned int		origIrq3Handler;

} MouseDriverHeader;
