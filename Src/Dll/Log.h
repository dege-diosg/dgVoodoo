/*--------------------------------------------------------------------------------- */
/* Log.h - Glide implementation, stuffs related to logging                          */
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


#define		LOGGING		0

#if			LOGGING

extern	char *res_t[];

extern	char *ref_t[];

extern	char *cFormat_t[];

extern	char *locOrig_t[];

extern	char *evenOdd_t[];

extern	char *lod_t[];

extern	char *texbaserange_t[];

extern	char *aspectr_t[];

extern	char *tformat_t[];

extern	char *combfunc_t[];

extern	char *combfact_t[];

extern	char *local_t[];

extern	char *other_t[];

extern	char *alphabfnc_srct[];

extern	char *alphabfnc_dstt[];

extern	char *cmpfnc_t[];

extern	char *locktype_t[];

extern	char *buffer_t[];

extern	char *lfbwritemode_t[];

extern	char *bypassmode_t[];

extern	char *origin_t[];

extern	char *textable_t[];

extern	char *ncctable_t[];

extern	char *depthbuffermode_t[];

extern	char *lfbsrcfmt_t[];

extern	char *colorcombinefunc_t[];

extern	char *alphasource_t[];

extern	char *dithermode_t[];

extern	char *cullmode_t[];

extern	char *hint_t[];

extern	char *sstmode_t[];

extern	char *fogmode_t[];

extern	char *fogmodemod_t[];

extern	char *fxbool_t[];

extern	char *chromakeymode_t[];

extern	char *d3dzbuffmode_t[];

extern	char *mipmapmode_t[];

extern	char *clampmode_t[];

extern	char *filtermode_t[];

extern	char *texturecombinefnc_t[];


void	__cdecl LOG(int flush, char *message, ... );


#define TOGGLELOGWIN logenabled^=1; \
					 if (logenabled) \
						LOG(1,"\n-*******--- Logging by user has begun ---******-"); \
					 else LOG(1,"\n-*******--- Logging by user has ended ---******-"); \
						return(0);


#define TOGGLELOGDOS logenabled^=1; \
					 return;


#else


#define	res_t				((char *) 0x0)

#define	ref_t				((char *) 0x0)

#define	cFormat_t			((char *) 0x0)

#define	locOrig_t			((char *) 0x0)

#define	evenOdd_t			((char *) 0x0)

#define	lod_t				((char *) 0x0)

#define	texbaserange_t		((char *) 0x0)

#define	aspectr_t			((char *) 0x0)

#define	tformat_t			((char *) 0x0)

#define	combfunc_t			((char *) 0x0)

#define	combfact_t			((char *) 0x0)

#define	local_t				((char *) 0x0)

#define	other_t				((char *) 0x0)

#define	alphabfnc_srct		((char *) 0x0)

#define	alphabfnc_dstt		((char *) 0x0)

#define	cmpfnc_t			((char *) 0x0)

#define	locktype_t			((char *) 0x0)

#define	buffer_t			((char *) 0x0)

#define	lfbwritemode_t		((char *) 0x0)

#define	bypassmode_t		((char *) 0x0)

#define	origin_t			((char *) 0x0)

#define	textable_t			((char *) 0x0)

#define	ncctable_t			((char *) 0x0)

#define	depthbuffermode_t	((char *) 0x0)

#define	lfbsrcfmt_t			((char *) 0x0)

#define	colorcombinefunc_t	((char *) 0x0)

#define	alphasource_t		((char *) 0x0)

#define	dithermode_t		((char *) 0x0)

#define	cullmode_t			((char *) 0x0)

#define	hint_t				((char *) 0x0)

#define	sstmode_t			((char *) 0x0)

#define	fogmode_t			((char *) 0x0)

#define	fogmodemod_t		((char *) 0x0)

#define	fxbool_t			((char *) 0x0)

#define	chromakeymode_t		((char *) 0x0)

#define	d3dzbuffmode_t		((char *) 0x0)

#define	mipmapmode_t		((char *) 0x0)

#define	clampmode_t			((char *) 0x0)

#define	filtermode_t		((char *) 0x0)

#define	texturecombinefnc_t ((char *) 0x0)


/* Ha nincs naplózás, a LOG-bejegyzéseket kommentnek tekintjük */
#define LOG				//
#define TOGGLELOGWIN	//
#define TOGGLELOGDOS	//


#endif