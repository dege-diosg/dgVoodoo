/*--------------------------------------------------------------------------------- */
/* Debug.h - Glide general debugging stuffs used for developing                     */
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
/* dgVoodoo: Debug.h																		*/
/*			 Függvények, definíciók a debug log-hoz és a teljesítményméréshez				*/
/*------------------------------------------------------------------------------------------*/

#ifndef		DEBUG_H
#define		DEBUG_H

/* Ezzel engedélyezhetjük, hogy bizonyos hibák naplózásra kerüljenek, beleértve az esetleges */
/* kivételeket is. */
#define DEBUG

#ifdef	DEBUG

/*------------------------------------------------------------------------------------------*/
/*........................... Definíciók debug-támogatás esetén ............................*/

#define DEBUG_LANG_ENGLISH						0
#define DEBUG_LANG_HUNGARIAN					1

#define MAX_THREADNUM							12
#define MAX_DEBUGNESTLEVEL						64
#define MAX_LISTELEMENTS						10

#define DEBUG_THREAD_REGISTER(id)				id = debug_thread_register();
#define DEBUG_BEGIN(inFunction, threadid, num)	handle_history_strings(get_funcdataptr(inFunction, num), threadid); __try { __try {
#define DEBUG_END(inFunction, threadid, num)	} __finally { handle_history_strings_out(get_funcdataptr(inFunction, num), threadid); } } __except(_exception(_exception_info(),get_funcdataptr(inFunction,num),threadid),0)	{ }
#define DEBUG_PERF_MEASURE_BEGIN(counter)		debug_perf_measure_begin(counter);
#define DEBUG_PERF_MEASURE_END(counter)			debug_perf_measure_end(counter);
#define	DEBUG_GET_PERF_QUOTIENT(counter1, counter2) debug_get_perf_quotient(counter1, counter2);
#define DEBUG_GLOBALTIMEBEGIN					debug_global_time_begin();
#define DEBUG_GLOBALTIMEEND						debug_global_time_end();
#define DEBUG_LOGMAXTIMES						debug_log_maxtimes();
#define DEBUG_SETLANGUAGE(language)				debug_set_language(language);
#define DEBUG_SETMODULEINSTANCE(hInstance)		debug_set_module_instance(hInstance);


/*------------------------------------------------------------------------------------------*/
/*....................................... Struktúrák .......................................*/


typedef	struct	{
	char			*functionName;
	unsigned int	functionTime[2];
} FunctionData;


typedef struct	{
	unsigned int	pureFunctionTime[2];
} ProfileTimes;


/*------------------------------------------------------------------------------------------*/
/*....................................... Függvények .......................................*/


void __cdecl	DEBUGLOG(int lang, char *message, ... );
void __cdecl	DEBUGCHECK(int lang, int exp, char *message, ...);
int				debug_thread_register();
FunctionData*	get_funcdataptr(char *str, int num);
float			getdiv64(int *int64);
float			debug_get_perf_quotient(__int64 counter1, __int64 counter2);
void			debug_global_time_begin();
void			debug_global_time_end();
void			_exception(EXCEPTION_POINTERS *excp, FunctionData *functionData, int threadid);
void			handle_history_strings(FunctionData *functionData, int threadID);
void			handle_history_strings_out(FunctionData *functionData, int threadID);
void			debug_perf_measure_begin(__int64 *counter);
void			debug_perf_measure_end(__int64 *counter);
void			debug_log_maxtimes();
void			debug_set_language(int language);
void			debug_set_module_instance(HINSTANCE hInstance);


#else

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*....................... Definíciók debug-támogatás nélküli esetben .......................*/


#define DEBUG_THREAD_REGISTER(id)
#define DEBUG_BEGIN(functionData, threadid, num)
#define DEBUG_END(functionData, threadid, num)
#define DEBUG_PERF_MEASURE_BEGIN(counter)
#define DEBUG_PERF_MEASURE_END(counter)
#define	DEBUG_GET_PERF_QUOTIENT(counter1, counter2)
#define DEBUG_GLOBALTIMEBEGIN
#define DEBUG_GLOBALTIMEEND
#define DEBUG_LOGMAXTIMES
#define DEBUG_SETLANGUAGE(language)
#define DEBUG_SETMODULEINSTANCE(hInstance)
#define DEBUGLOG
#define DEBUGCHECK


#endif

#endif