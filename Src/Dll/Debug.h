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

#define		_CRT_SECURE_NO_DEPRECATE

/* Ezzel engedélyezhetjük, hogy bizonyos hibák naplózásra kerüljenek, beleértve az esetleges */
/* kivételeket is. */
//#define DEBUG

#ifdef	DEBUG

#include	<windows.h>
#include	"excpt.h"

/*------------------------------------------------------------------------------------------*/
/*........................... Definíciók debug-támogatás esetén ............................*/

#define DEBUG_LANG_ENGLISH						0
#define DEBUG_LANG_HUNGARIAN					1

#define MAX_THREADNUM							12
#define MAX_DEBUGNESTLEVEL						64

#define DEBUG_THREAD_REGISTER(id)				id = debug_thread_register();
#define DEBUG_THREAD_UNREGISTER(id)				debug_thread_unregister(id);
#define DEBUG_BEGINSCOPE(scopeName, threadId)	debug_begincritsection (); \
												__asm { call  $+9 }; \
												__asm { _emit 0x0 }; \
												__asm { _emit 0x0 }; \
												__asm { _emit 0x0 }; \
												__asm { _emit 0x0 }; \
												__asm { pop lastLocation };	 \
												debug_beginscope(lastLocation, scopeName, threadId); \
												debug_endcritsection (); \
												 __try { __try {
#define DEBUG_ENDSCOPE(threadId)				} __finally { debug_endscope(threadId); } } __except(_exception(GetExceptionInformation(),threadId),0)	{ }
#define DEBUG_PERF_MEASURE_BEGIN(counter)		debug_perf_measure_begin(counter);
#define DEBUG_PERF_MEASURE_END(counter)			debug_perf_measure_end(counter);
#define	DEBUG_GET_PERF_QUOTIENT(counter1, counter2) debug_get_perf_quotient(counter1, counter2);
#define DEBUG_GLOBALTIMEBEGIN					debug_global_time_begin();
#define DEBUG_GLOBALTIMEEND						debug_global_time_end();
#define DEBUG_LOGMAXTIMES						debug_log_maxtimes();
#define DEBUG_SETLANGUAGE(language)				debug_set_language(language);
#define DEBUG_SETMODULEINSTANCE(hInstance)		debug_set_module_instance(hInstance);

#define DEBUG_ENABLEPROFILING					enabe_profiling (1);
#define DEBUG_DISABLEPROFILING					enabe_profiling (0);


/*------------------------------------------------------------------------------------------*/
/*....................................... Struktúrák .......................................*/


typedef	struct  {
	char			*functionName;
	unsigned int	functionTime[2];
} FunctionData;


typedef struct	{
	unsigned int	pureFunctionTime[2];
} ProfileTimes;


/*------------------------------------------------------------------------------------------*/
/*....................................... Függvények .......................................*/

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl	DEBUGLOG(int lang, char *message, ... );
void __cdecl	DEBUGCHECK(int lang, int exp, char *message, ...);
int				debug_thread_register();
void			debug_thread_unregister(int threadId);
FunctionData*	get_funcdataptr(char *str, int num);
float			getdiv64(unsigned int *int64);
float			debug_get_perf_quotient(__int64 counter1, __int64 counter2);
void			debug_global_time_begin();
void			debug_global_time_end();
void			_exception(EXCEPTION_POINTERS *excp, int threadid);
void			debug_beginscope(void* location, char* scopeName, int threadId);
void			debug_endscope(int threadId);
void			debug_begincritsection ();
void			debug_endcritsection ();
void			handle_history_strings(FunctionData *functionData, int threadID);
void			handle_history_strings_out(int threadID);
void			debug_perf_measure_begin(__int64 *counter);
void			debug_perf_measure_end(__int64 *counter);
void			debug_log_maxtimes();
void			debug_set_language(int language);
void			debug_set_module_instance(HINSTANCE hInstance);
void			enabe_profiling (int enable);

#ifdef __cplusplus
}
#endif

extern			void*			lastLocation;

#else

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*....................... Definíciók debug-támogatás nélküli esetben .......................*/


#define DEBUG_THREAD_REGISTER(id)
#define DEBUG_THREAD_UNREGISTER(id)
#define DEBUG_BEGINSCOPE(functionData, threadid)
#define DEBUG_ENDSCOPE(threadid)
#define DEBUG_PERF_MEASURE_BEGIN(counter)
#define DEBUG_PERF_MEASURE_END(counter)
#define	DEBUG_GET_PERF_QUOTIENT(counter1, counter2)
#define DEBUG_GLOBALTIMEBEGIN
#define DEBUG_GLOBALTIMEEND
#define DEBUG_LOGMAXTIMES
#define DEBUG_SETLANGUAGE(language)
#define DEBUG_SETMODULEINSTANCE(hInstance)
#define DEBUGLOG									;##/##/
#define DEBUGCHECK									;##/##/

#define DEBUG_ENABLEPROFILING
#define DEBUG_DISABLEPROFILING



#endif

#endif