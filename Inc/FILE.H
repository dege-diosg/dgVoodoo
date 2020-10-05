/*--------------------------------------------------------------------------------- */
/* FILE.H - C-header for FILE                                                       */
/*          (file related functions ported from my old DOS-extender)                */
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


#ifdef __cplusplus
extern "C"      {
#endif

HANDLE	_stdcall _CreateFile(const char *, int);
HANDLE	_stdcall _OpenFile(const char *);
void	_stdcall _CloseFile(HANDLE);
int		_stdcall _GetFilePos(HANDLE);
int		_stdcall _GetFileSize(HANDLE);
int		_stdcall _SetFilePos(HANDLE, int, int);
int		_stdcall _ReadFile(HANDLE, void *, int);
int		_stdcall _WriteFile(HANDLE, void *, int);

#ifdef __cplusplus
}
#endif
