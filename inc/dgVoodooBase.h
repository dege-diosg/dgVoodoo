/*--------------------------------------------------------------------------------- */
/* dgVoodooBase.h - Glide very basic defs used for compilation                      */
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
/* dgVoodoo: dgVoodooBase.h																	*/
/*			 Alapdefiníciók																	*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DGVOODOOBASE_H
#define		DGVOODOOBASE_H

#define		i386
#ifndef		_WIN32_WINNT
#define		_WIN32_WINNT	0x0500
#endif

#define		__MSC__

/* A glide headerekben nem kellenek a függvények prototípusai */
#define		FX_GLIDE_NO_FUNC_PROTO

/* Az exportálandó függvények tárolási osztálya */
#define		EXPORT			__declspec(dllexport) _stdcall

#ifdef		GLIDE1

#define		EXPORT1			_stdcall

#else

#define		EXPORT1			__declspec(dllexport) _stdcall

#endif

#define		_CRT_SECURE_NO_DEPRECATE

#define		SQR(x)			( (x) * (x) )
#define		ABS(x)			( ((x) > 0) ? (x) : (-(x)) )
#define		MAX(x, y)		( (x) < (y) ? (y) : (x) )
#define		MIN(x, y)		( (x) < (y) ? (x) : (y) )

#define	_CRT_SECURE_NO_WARNINGS

#pragma warning (disable: 4200)		//Disable 'nonstandard extension used : zero-sized array in struct/union'

#endif