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


/*------------------------------------------------------------------------------------------*/
/* dgVoodoo: Log.h																			*/
/*			 General logging control header													*/
/*------------------------------------------------------------------------------------------*/

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


#define TOGGLELOGWIN logenabled ^= 1;													\
					 if (logenabled)													\
						LOG(1,"\n-*******--- Logging by user has begun ---******-");	\
					 else																\
						LOG(1,"\n-*******--- Logging by user has ended ---******-");	\
					 return(0);


#define TOGGLELOGDOS logenabled^=1; \
					 return;


#else


/* Ha nincs naplózás, a LOG-bejegyzéseket kommentnek tekintjük */
#define LOG				;##/##/
#define TOGGLELOGWIN	;##/##/
#define TOGGLELOGDOS	;##/##/

#pragma warning (disable : 4010)

#endif