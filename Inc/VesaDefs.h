/*--------------------------------------------------------------------------------- */
/* VesaDefs.h - C-header for general VESA things                                    */
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
/* dgVoodoo: VesaDefs.h																		*/
/*			 Általános dolgok a VESA interfészhez											*/
/*------------------------------------------------------------------------------------------*/

#ifndef		VESADEFS_H
#define		VESADEFS_H

/*------------------------------------------------------------------------------------------*/
/*................................. Szükséges struktúrák  ..................................*/

/* VBE Információ */
typedef struct {

unsigned int	vbeSignature;
unsigned short	vbeVersion;
unsigned int	oemStringPtr;
unsigned int	capabilities;
unsigned int	videoModePtr;
unsigned short	totalMemory;

unsigned short	oemSoftwareRev;
unsigned int	oemVendorNamePtr;
unsigned int	oemProductNamePtr;
unsigned int	oemProductRevPtr;
unsigned char	reserved[222];

unsigned char	oemData[256];

} VBEInfoBlock;


/* VESA-módot leíró struktúra */
typedef struct {

unsigned short	ModeAttributes;
unsigned char	WinAAttributes;
unsigned char	WinBAttributes;
unsigned short	WinGranularity;
unsigned short	WinSize;
unsigned short	WinASegment;
unsigned short	WinBSegment;
unsigned int	WinFuncPtr;
unsigned short	BytesPerScanLineVESA;

unsigned short	XResolution;
unsigned short	YResolution;
unsigned char	XCharSize;
unsigned char	YCharSize;
unsigned char	NumberOfPlanes;
unsigned char	BitsPerPixel;
unsigned char	NumberOfBanks;
unsigned char	MemoryModel;
unsigned char	BankSize;
unsigned char	NumberOfImagePages;
unsigned char	MIReserved;

unsigned char	RedMaskSize;
unsigned char	RedFieldPosition;
unsigned char	GreenMaskSize;
unsigned char	GreenFieldPosition;
unsigned char	BlueMaskSize;
unsigned char	BlueFieldPosition;
unsigned char	RsvdMaskSize;
unsigned char	RsvdFieldPosition;
unsigned char	DirectColorModeInfo;

unsigned int	PhysBasePtr;
unsigned int	OffScreenMemOffset;
unsigned short	OffScreenMemSize;
unsigned char	MIReserved2[206];

} ModeInfoBlock;


/* A belsõleg használt, állapot elmentésére szolgáló struktúra */
typedef struct {

	unsigned int palette[256];
	unsigned int bytesPerScanline;
	unsigned int displayOffset;
	unsigned char vgaRAMDACSize;

} StateBuffer;

#endif		/* VESADEFS_H */