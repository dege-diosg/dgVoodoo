/*--------------------------------------------------------------------------------- */
/* DGW.H - C-header for DGW (some functions ported from my old DOS-extender)        */
/*                                                                                  */
/* Copyright (C) 2003 Dege                                                          */
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

#ifdef __cplusplus
extern "C"      {
#endif

void * _stdcall _GetMem(int);
int _stdcall _FreeMem(void *);
int _stdcall _FreeMemWithMarker(int);
void _stdcall  _SetBlockMarker(int);
int _stdcall _SetBlockID(void *, int);
void _stdcall _FreeAllMem();

#ifdef __cplusplus
}
#endif

