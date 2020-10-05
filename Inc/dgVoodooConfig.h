/*--------------------------------------------------------------------------------- */
/* dgVoodooConfig.h - Config structure and flags bits                               */
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
/* dgVoodoo: dgVoodooConfig.h																*/
/*			 Konfigurációhoz szükséges definíciók											*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DGVOODOOCONFIG_H
#define		DGVOODOOCONFIG_H

/*------------------------------------------------------------------------------------------*/
/* Setup: azonosító a kiírt konfig-fájlokban (.cfg) */
#define		CFGFILEID	"DGVOODOOCFG"

/*------------------------------------------------------------------------------------------*/
/*.................................. Definíciók a konfighoz ................................*/

/* Alapértelmezett értékek a config struktúra mezôihez */
#define	TEXTUREMEMSIZE				4*1024*1024			/* Voodoo-textúramem mérete */
#define	CFG_DEFFLAGS				0
#define	CFG_DEFFLAGS2				0
#define DEFAULT_DOS_TIMER_BOOST		80

/* Konstansok a konfig struktúra flagjeihez */
#define CFG_WBUFFER					0x1					/* W-buffert mindig Z-bufferen keresztül emulál */
#define CFG_RELYONDRIVERFLAGS		CFG_WBUFFER
#define CFG_WINDOWED				0x2					/* Ablakos mód */
#define CFG_DISPMODE32BIT			0x4					/* 32 bites videomód */
#define CFG_SETREFRESHRATE			0x8					/* A frissítési freq-t az alkalmazás */
														/* állítja be (egyébként az akt. freq) */
#define CFG_PERFECTTEXMEM			0x10				/* Tökéletes textúramemória-emuláció */
#define CFG_WBUFFERMETHODISFORCED	0x80				/* A W-bufferelés módszere kényszerített:*/
														/*	- CFG_WBUFFER flag set: mindig valódi w-bufferelés */
														/*	- CFG_WBUFFER flag not set: mindig emulált w-bufferelés */
														/* vagy a W-bufferelés módszere detektált, ekkor */
														/*  - CFG_RELYONDRIVERFLAGS set: a driver által a W-bufferelés támogatására */
														/*								 visszaadott flag alapján dönt */
														/*  - CFG_RELYONDRIVERFLAGS not set: teszt alapján dönt */
#define CFG_VESAENABLE				0x100				/* Beépített VESA-támogatás engedélyezve */
#define CFG_SETMOUSEFOCUS			0x200               /* Egérfókusz az alkalmazásnál */
#define CFG_SUPPORTMODE13H			0x400               /* 320x200x256 VGA-mód támogatása (DOS, VESA emu) */
#define CFG_TEXTURES32BIT			0x800				/* mindig 32 bites textúrák */
#define CFG_TEXTURES16BIT			0x1000				/* mindig 16 bites textúrák */
#define CFG_STOREMIPMAP				0x2000				/* Mipmapek másolatának eltárolása */
#define CFG_GRABENABLE				0x4000              /* Képernyõlopás engedélyezve */
#define CFG_GRABCLIPBOARD			0x8000              /* Képlopás a vágólapra */
#define CFG_CKMFORCETNTINAUTOMODE	0x10000
#define CFG_TOMBRAIDER				0x20000             /* TR1 árnyékhiba-javítás */
#define CFG_HUNGARIAN				0x40000             /* A nyelv magyar */
#define CFG_HWLFBCACHING			0x80000             /* Hardveres gyorsítópufferek az LFB-hez */
#define CFG_LFBREALVOODOO			0x100000            /* Közelebb az igazi hardverhez */
#define CFG_TRIPLEBUFFER			0x200000            /* Mindig triplapufferelés */
#define CFG_ACTIVEBACKGROUND		0x400000			/* A háttérben is aktív a DOS-os prg */
#define CFG_LFBFASTUNMATCHWRITE		0x800000			/* Gyorsírás a nem egyezõ LFB formátumokhoz */
#define CFG_CTRLALT					0x1000000           /* Egérfókusz elengedése Ctrl-Alt-ra */
#define CFG_VSYNC					0x2000000           /* Függõleges visszafutást mindig megvárni */
#define CFG_NTVDDMODE				0x4000000           /* a wrapper VDD-módban mûködik (XP-hez) */
#define CFG_DISABLEMIPMAPPING		0x8000000           /* Mipmapping tiltása */
#define CFG_ENABLEGAMMARAMP			0x10000000          /* Glide-gammaramp engedélyezése */
#define CFG_ENABLEHWVBUFFER			0x20000000          /* Hardveres vertex pufferek használata */
#define CFG_BILINEARFILTER			0x40000000          /* Mindig bilineáris szûrés a textúrákhoz */
#define CFG_CLIPPINGBYD3D			0x80000000          /* Vágás a Direct3D-n keresztül */

#define CFG_CLOSESTREFRESHFREQ		0x1					/* Frissítési frekvencia: az alk. által igényelthez legközelebb esõ */
#define CFG_FORCECLIPOPF            0x2                 /* CLI/POPF megfelelõ kezelése - rejtett opció */
#define CFG_LFBRESCALECHANGESONLY	0x4					/* eltérõ felbontások esetén csak a változások átméretezése, */
														/* és nem az egész képé (gyorsírást igényel) */
#define CFG_LFBFORCEFASTWRITE		0x8					/* Gyorsírás használata egyezõ formátumokhoz is - rejtett opció */
#define CFG_YMIRROR					0x10				/* Képernyõ függõleges tükrözése (origó-tesztekhez) - rejtett opció */
#define CFG_FORCETRILINEARMIPMAPS	0x20				/* Trilineáris mipmapping kényszerítése */
#define CFG_PREFERTMUWIFPOSSIBLE	0x40				/* táblás köd, z-buffer és TMUDIFFW hint esetén a tmu w-t */
														/* használja w-ként a ködhöz is, és nem a valódi w-t */
														/* (ezáltal a köd perspektívája romlik el, nem a textúráé) */
#define CFG_LFBALWAYSFLUSHWRITES	0x80				/* A régi lfb-unlock dude-módja: mindig kiírja a változásokat
															(nem cache-el) (kompatibilitási és teszt-opció, rejtett) */
#define CFG_DEPTHEQUALTOCOLOR		0x100				/* A képpuffer és a mélységi puffer bitmélysége legyen mindig egyenlõ */
														/* (kompatibilitási és teszt-opció, rejtett) */
#define CFG_DISABLELFBREAD			0x200				/* LFB elérésének tiltása olvasásra */
#define CFG_DISABLELFBWRITE			0x400				/* LFB elérésének tiltása írásra */
#define CFG_AUTOGENERATEMIPMAPS		0x800				/* A hiányzó mipmap-szintek automatikus generálása */
#define CFG_LFBNOMATCHINGFORMATS	0x1000				/* Az LFB-formátumok mindig különbözõnek tekintendõk - rejtett opció */
#define CFG_ALWAYSINDEXEDPRIMS		0x2000
#define CFG_HIDECONSOLE				0x4000				/* Konzol elrejtése (DOS) */
#define CFG_LFBDISABLETEXTURING		0x8000
#define CFG_CFORMATAFFECTLFB		0x10000
#define CFG_DISABLEVESAFLATLFB		0x20000
#define CFG_PRESERVEWINDOWSIZE		0x40000
#define CFG_PRESERVEASPECTRATIO		0x80000

/* A konfig flagekben levõ bitmezõk értékei */
#define CFG_CKMAUTOMATIC			0x00
#define CFG_CKMALPHABASED			0x01
#define CFG_CKMNATIVE				0x02
#define CFG_CKMNATIVETNT			0x03

#define GetCKMethodValue(flags)				((flags >> 5) & 0x3)
#define SetCKMethodValue(flags, method)		flags = (flags & ~(0x3 << 5)) | (method << 5)

#define CFG_WDETFORCEZ				(CFG_WBUFFERMETHODISFORCED)
#define CFG_WDETFORCEW				(CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)
#define CFG_WDETONDRIVERFLAG		(CFG_RELYONDRIVERFLAGS)
#define CFG_WDETDRAWINGTEST			0

#define GetLfbAccessValue(flags)			(((flags & CFG_DISABLELFBWRITE) ? 2 : 0) | ((flags & CFG_DISABLELFBREAD) ? 1 : 0))

#define GetWBuffMethodValue(flags)			(((flags & CFG_WBUFFERMETHODISFORCED) ? 2 : 0) | ((flags & CFG_WBUFFER) ? 1 : 0))
#define SetWBuffMethodValue(flags, method)	flags = (flags & ~(CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) | method


enum Languages	{

	English				= 0,
	Hungarian			= 1,
	numberOfLanguages	= 2
};

/*------------------------------------------------------------------------------------------*/
/*....................................... Struktúrák .......................................*/


/* A wrapper konfigurációját leíró struktúra */
typedef struct	{

	unsigned short	ProductVersion;
	unsigned short	GlideVersion;
	unsigned int	TexMemSize, Flags;
	unsigned int	Gamma, Version;
	unsigned int	VideoMemSize;
	unsigned int	VideoRefreshFreq;
	unsigned short	dxres, dyres;
	unsigned short	dispdev, dispdriver;
	unsigned int	RefreshRate;
	unsigned int	Flags2;
	unsigned short  dosTimerBoostPeriod;
	unsigned char	AlphaCKRefValue;
	unsigned char	language;
	unsigned char	reserved[4];

} dgVoodooConfig;


extern	dgVoodooConfig	config;


#endif