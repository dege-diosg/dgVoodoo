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
/*			 Konfigur�ci�hoz sz�ks�ges defin�ci�k											*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DGVOODOOCONFIG_H
#define		DGVOODOOCONFIG_H

/*------------------------------------------------------------------------------------------*/
/* Setup: azonos�t� a ki�rt konfig-f�jlokban (.cfg) */
#define		CFGFILEID	"DGVOODOOCFG"

/*------------------------------------------------------------------------------------------*/
/*.................................. Defin�ci�k a konfighoz ................................*/

/* Alap�rtelmezett �rt�kek a config strukt�ra mez�ihez */
#define	TEXTUREMEMSIZE				4*1024*1024			/* Voodoo-text�ramem m�rete */
#define	CFG_DEFFLAGS				0
#define	CFG_DEFFLAGS2				0
#define DEFAULT_DOS_TIMER_BOOST		80

/* Konstansok a konfig strukt�ra flagjeihez */
#define CFG_WBUFFER					0x1					/* W-buffert mindig Z-bufferen kereszt�l emul�l */
#define CFG_RELYONDRIVERFLAGS		CFG_WBUFFER
#define CFG_WINDOWED				0x2					/* Ablakos m�d */
#define CFG_DISPMODE32BIT			0x4					/* 32 bites videom�d */
#define CFG_SETREFRESHRATE			0x8					/* A friss�t�si freq-t az alkalmaz�s */
														/* �ll�tja be (egy�bk�nt az akt. freq) */
#define CFG_PERFECTTEXMEM			0x10				/* T�k�letes text�ramem�ria-emul�ci� */
#define CFG_WBUFFERMETHODISFORCED	0x80				/* A W-bufferel�s m�dszere k�nyszer�tett:*/
														/*	- CFG_WBUFFER flag set: mindig val�di w-bufferel�s */
														/*	- CFG_WBUFFER flag not set: mindig emul�lt w-bufferel�s */
														/* vagy a W-bufferel�s m�dszere detekt�lt, ekkor */
														/*  - CFG_RELYONDRIVERFLAGS set: a driver �ltal a W-bufferel�s t�mogat�s�ra */
														/*								 visszaadott flag alapj�n d�nt */
														/*  - CFG_RELYONDRIVERFLAGS not set: teszt alapj�n d�nt */
#define CFG_VESAENABLE				0x100				/* Be�p�tett VESA-t�mogat�s enged�lyezve */
#define CFG_SETMOUSEFOCUS			0x200               /* Eg�rf�kusz az alkalmaz�sn�l */
#define CFG_SUPPORTMODE13H			0x400               /* 320x200x256 VGA-m�d t�mogat�sa (DOS, VESA emu) */
#define CFG_TEXTURES32BIT			0x800				/* mindig 32 bites text�r�k */
#define CFG_TEXTURES16BIT			0x1000				/* mindig 16 bites text�r�k */
#define CFG_STOREMIPMAP				0x2000				/* Mipmapek m�solat�nak elt�rol�sa */
#define CFG_GRABENABLE				0x4000              /* K�perny�lop�s enged�lyezve */
#define CFG_GRABCLIPBOARD			0x8000              /* K�plop�s a v�g�lapra */
#define CFG_CKMFORCETNTINAUTOMODE	0x10000
#define CFG_TOMBRAIDER				0x20000             /* TR1 �rny�khiba-jav�t�s */
#define CFG_HUNGARIAN				0x40000             /* A nyelv magyar */
#define CFG_HWLFBCACHING			0x80000             /* Hardveres gyors�t�pufferek az LFB-hez */
#define CFG_LFBREALVOODOO			0x100000            /* K�zelebb az igazi hardverhez */
#define CFG_TRIPLEBUFFER			0x200000            /* Mindig triplapufferel�s */
#define CFG_ACTIVEBACKGROUND		0x400000			/* A h�tt�rben is akt�v a DOS-os prg */
#define CFG_LFBFASTUNMATCHWRITE		0x800000			/* Gyors�r�s a nem egyez� LFB form�tumokhoz */
#define CFG_CTRLALT					0x1000000           /* Eg�rf�kusz elenged�se Ctrl-Alt-ra */
#define CFG_VSYNC					0x2000000           /* F�gg�leges visszafut�st mindig megv�rni */
#define CFG_NTVDDMODE				0x4000000           /* a wrapper VDD-m�dban m�k�dik (XP-hez) */
#define CFG_DISABLEMIPMAPPING		0x8000000           /* Mipmapping tilt�sa */
#define CFG_ENABLEGAMMARAMP			0x10000000          /* Glide-gammaramp enged�lyez�se */
#define CFG_ENABLEHWVBUFFER			0x20000000          /* Hardveres vertex pufferek haszn�lata */
#define CFG_BILINEARFILTER			0x40000000          /* Mindig biline�ris sz�r�s a text�r�khoz */
#define CFG_CLIPPINGBYD3D			0x80000000          /* V�g�s a Direct3D-n kereszt�l */

#define CFG_CLOSESTREFRESHFREQ		0x1					/* Friss�t�si frekvencia: az alk. �ltal ig�nyelthez legk�zelebb es� */
#define CFG_FORCECLIPOPF            0x2                 /* CLI/POPF megfelel� kezel�se - rejtett opci� */
#define CFG_LFBRESCALECHANGESONLY	0x4					/* elt�r� felbont�sok eset�n csak a v�ltoz�sok �tm�retez�se, */
														/* �s nem az eg�sz k�p� (gyors�r�st ig�nyel) */
#define CFG_LFBFORCEFASTWRITE		0x8					/* Gyors�r�s haszn�lata egyez� form�tumokhoz is - rejtett opci� */
#define CFG_YMIRROR					0x10				/* K�perny� f�gg�leges t�kr�z�se (orig�-tesztekhez) - rejtett opci� */
#define CFG_FORCETRILINEARMIPMAPS	0x20				/* Triline�ris mipmapping k�nyszer�t�se */
#define CFG_PREFERTMUWIFPOSSIBLE	0x40				/* t�bl�s k�d, z-buffer �s TMUDIFFW hint eset�n a tmu w-t */
														/* haszn�lja w-k�nt a k�dh�z is, �s nem a val�di w-t */
														/* (ez�ltal a k�d perspekt�v�ja romlik el, nem a text�r��) */
#define CFG_LFBALWAYSFLUSHWRITES	0x80				/* A r�gi lfb-unlock dude-m�dja: mindig ki�rja a v�ltoz�sokat
															(nem cache-el) (kompatibilit�si �s teszt-opci�, rejtett) */
#define CFG_DEPTHEQUALTOCOLOR		0x100				/* A k�ppuffer �s a m�lys�gi puffer bitm�lys�ge legyen mindig egyenl� */
														/* (kompatibilit�si �s teszt-opci�, rejtett) */
#define CFG_DISABLELFBREAD			0x200				/* LFB el�r�s�nek tilt�sa olvas�sra */
#define CFG_DISABLELFBWRITE			0x400				/* LFB el�r�s�nek tilt�sa �r�sra */
#define CFG_AUTOGENERATEMIPMAPS		0x800				/* A hi�nyz� mipmap-szintek automatikus gener�l�sa */
#define CFG_LFBNOMATCHINGFORMATS	0x1000				/* Az LFB-form�tumok mindig k�l�nb�z�nek tekintend�k - rejtett opci� */
#define CFG_ALWAYSINDEXEDPRIMS		0x2000
#define CFG_HIDECONSOLE				0x4000				/* Konzol elrejt�se (DOS) */
#define CFG_LFBDISABLETEXTURING		0x8000
#define CFG_CFORMATAFFECTLFB		0x10000
#define CFG_DISABLEVESAFLATLFB		0x20000
#define CFG_PRESERVEWINDOWSIZE		0x40000
#define CFG_PRESERVEASPECTRATIO		0x80000

/* A konfig flagekben lev� bitmez�k �rt�kei */
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
/*....................................... Strukt�r�k .......................................*/


/* A wrapper konfigur�ci�j�t le�r� strukt�ra */
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