/*--------------------------------------------------------------------------------- */
/* Debug.c - Glide general debugging stuffs used for developing                     */
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
/* dgVoodoo: Debug.c																		*/
/*			 Függvények, definíciók a debug log-hoz és a teljesítményméréshez				*/
/*------------------------------------------------------------------------------------------*/

#include "debug.h"
#include <windows.h>
#include <stdio.h>
#include "dgw.h"
#include "Main.h"

#ifdef	DEBUG

#pragma	message ( "Compiling with built-in debug support" )

/*------------------------------------------------------------------------------------------*/
/*........................ Globális cuccok debug-támogatás esetén ..........................*/


DWORD	exceptionCodes[]	= { EXCEPTION_ACCESS_VIOLATION, EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
								EXCEPTION_BREAKPOINT, EXCEPTION_DATATYPE_MISALIGNMENT,
								EXCEPTION_FLT_DENORMAL_OPERAND, EXCEPTION_FLT_DIVIDE_BY_ZERO,
								EXCEPTION_FLT_INEXACT_RESULT, EXCEPTION_FLT_INVALID_OPERATION,
								EXCEPTION_FLT_OVERFLOW, EXCEPTION_FLT_STACK_CHECK,
								EXCEPTION_FLT_UNDERFLOW, EXCEPTION_ILLEGAL_INSTRUCTION,
								EXCEPTION_IN_PAGE_ERROR, EXCEPTION_INT_DIVIDE_BY_ZERO,
								EXCEPTION_INT_OVERFLOW, EXCEPTION_INVALID_DISPOSITION,
								EXCEPTION_NONCONTINUABLE_EXCEPTION, EXCEPTION_PRIV_INSTRUCTION,
								EXCEPTION_SINGLE_STEP, EXCEPTION_STACK_OVERFLOW };
char	*exceptionStrings[] = { "EXCEPTION_ACCESS_VIOLATION", "EXCEPTION_ARRAY_BOUNDS_EXCEEDED",
								"EXCEPTION_BREAKPOINT", "EXCEPTION_DATATYPE_MISALIGNMENT",
								"EXCEPTION_FLT_DENORMAL_OPERAND", "EXCEPTION_FLT_DIVIDE_BY_ZERO",
								"EXCEPTION_FLT_INEXACT_RESULT", "EXCEPTION_FLT_INVALID_OPERATION",
								"EXCEPTION_FLT_OVERFLOW", "EXCEPTION_FLT_STACK_CHECK",
								"EXCEPTION_FLT_UNDERFLOW", "EXCEPTION_ILLEGAL_INSTRUCTION",
								"EXCEPTION_IN_PAGE_ERROR", "EXCEPTION_INT_DIVIDE_BY_ZERO",
								"EXCEPTION_INT_OVERFLOW", "EXCEPTION_INVALID_DISPOSITION",
								"EXCEPTION_NONCONTINUABLE_EXCEPTION", "EXCEPTION_PRIV_INSTRUCTION",
								"EXCEPTION_SINGLE_STEP", "EXCEPTION_STACK_OVERFLOW" };

int					debugInited = 0;
FunctionData**		history_data;
int					history_ptr[MAX_THREADNUM];
int					history_num[MAX_THREADNUM];
int					threadUsage[MAX_THREADNUM];
int					exc_logged = 0;
ProfileTimes		profileTimes[MAX_DEBUGNESTLEVEL];
ProfileTimes		sumOfProfileTimes[MAX_DEBUGNESTLEVEL];
FunctionData		functionDatas[512];
unsigned int		funcDataNum = 0;
int					debugLanguage;
HINSTANCE			hModInstance;
unsigned int		totalTime[2] = {0,0};
int					profilingEnabled = 1;
void*				lastLocation;
CRITICAL_SECTION	debugCritSection;

/*------------------------------------------------------------------------------------------*/
/*....................................... Függvények .......................................*/


void DebugStrCat(unsigned char *dst, unsigned char *src)	{

	while( (*dst) != 0x0 ) dst++;
	while( (*src) != 0x0 ) *(dst++) = *(src++);
	*dst=0x0;

}


void __cdecl DEBUGLOG(int lang, char *message, ... )	{
FILE	*logFileHandle;

	va_list(arg);
	if (lang != 2) if (lang != debugLanguage) return;
    va_start(arg, message);

	logFileHandle = fopen("dgVoodoo.log", "a");
	if (logFileHandle != NULL)	{
		vfprintf(logFileHandle, message, arg);
		fclose(logFileHandle);
	}	
	va_end(arg);
}


void __cdecl DEBUGCHECK(int lang, int exp, char *message, ...)	{

	va_list(arg);
	va_start(arg, message);
	if (exp) DEBUGLOG(lang, message, arg);
	va_end(arg);
}


void	enabe_profiling (int enable) {
	profilingEnabled = enable;
}


int debug_thread_register()	{
int i;
	
	if (!debugInited)	{
		if ( (history_data = _GetMem(MAX_THREADNUM*MAX_DEBUGNESTLEVEL*sizeof(FunctionData))) == NULL) return(-1);
		for(i = 0; i < MAX_THREADNUM; i++) history_ptr[i] = history_num[i] = threadUsage[i] = 0;
		ZeroMemory(profileTimes, MAX_DEBUGNESTLEVEL * sizeof(__int64));
		ZeroMemory(functionDatas, sizeof(functionDatas));
		debugInited = 1;
		InitializeCriticalSection (&debugCritSection);
	}
	for (i=0; (threadUsage[i] != 0) && (i < MAX_THREADNUM); i++);

	threadUsage[i] = 1;
	history_ptr[i] = i*MAX_DEBUGNESTLEVEL*32;
	return(i);
}


void debug_thread_unregister (int threadid) {
	
	threadUsage[threadid] = 0;
}


float	getdiv64(unsigned int *int64)	{
double	f, f2;

	f = ((double) int64[1]) * ((double) (2<<16)) * ((double) (2<<16));
	f += (double) int64[0];
	
	f2 = ((double) totalTime[1]) * ((double) (2<<16)) * ((double) (2<<16));
	f2 += (double) totalTime[0];
	return (float) (100.0*f/f2);
}


float	debug_get_perf_quotient(__int64 counter1, __int64 counter2)	{
	
	return( ((float) counter1) / ((float) counter2) );
}


void debug_global_time_begin()	{
	_asm	{
		xor		eax,eax
		mov		totalTime[0],eax
		mov		totalTime[4],eax
	}
}


void debug_global_time_end()	{
unsigned int		i,j;
float				f;
float				sum;
char				fill[128];

	sum = 0.0f;
	
	for(i = 0; i < funcDataNum; i++)	{
		f = getdiv64(functionDatas[i].functionTime);
		//if ( (f = getdiv64(functionDatas[i].functionTime)) > 0.001f ) {
			sum += f;
			
			for (j = 0; j < (int) ((64 - strlen (functionDatas[i].functionName))); j++)
				fill[j] = ' ';

			fill[j] = 0x0;
			DEBUGLOG(2,"\n%s: %s%f", functionDatas[i].functionName, fill, f);
		//}
	}
	DEBUGLOG (2, "\n\nSum of times: %f", sum);
}



void _exception(EXCEPTION_POINTERS *excp, int threadid)	{
char	history[10*1024];
int		i,ptr;
char	*exceptionString;
char	*infunction;

	if (exc_logged) return;
	
	infunction = (history_data)[history_ptr[threadid]-1]->functionName;

	for(i = 0; i < sizeof(exceptionCodes); i++) if (excp->ExceptionRecord->ExceptionCode == exceptionCodes[i]) break;
	exceptionString = (i != sizeof(exceptionCodes)) ? exceptionStrings[i] : "UNKNOWN";
	
	DEBUGLOG(0,"\n\nException occured in function (or debug scope): %s\nException: %s",infunction, exceptionString);
	DEBUGLOG(1,"\n\nKivétel keletkezett a következõ függvény végrehajtásakor: %s\nKivétel: %s",infunction, exceptionString);
	DEBUGLOG(2,"\n\nEAX: %0p  EBX: %0p  ECX: %0p  EDX: %0p", excp->ContextRecord->Eax, excp->ContextRecord->Ebx,
			 excp->ContextRecord->Ecx, excp->ContextRecord->Edx);
	DEBUGLOG(2,"\nESI: %0p  EDI: %0p  EBP: %0p  ESP: %0p", excp->ContextRecord->Esi, excp->ContextRecord->Edi,
			 excp->ContextRecord->Ebp, excp->ContextRecord->Esp);
	DEBUGLOG(2,"\nEIP: %0p  EFlags: %0p, DLL Base=%0p", excp->ContextRecord->Eip, excp->ContextRecord->EFlags,
			 hModInstance);
	if (threadid!=-1)	{
		history[0]=0;
		ptr = history_ptr[threadid] - history_num[threadid];
		for(i = 0; i < history_num[threadid]; i++)	{
			if ((history_data)[ptr+i] != NULL) {
				DebugStrCat(history, (history_data)[ptr+i]->functionName);
			} else {
				DebugStrCat(history, "(unknown)");
			}
			
			if (i != (history_num[threadid]-1)) DebugStrCat(history, "->");
		}
		DEBUGLOG(0,"\n\nCallstack: %s",history);
		DEBUGLOG(1,"\n\nHívási verem: %s",history);
	} else {
		DEBUGLOG(0,"\n   Error: Callstack is not available due to unsuccessful debug support initialization");
		DEBUGLOG(1,"\n   Hiba: A hívási verem nem elérhetõ a debug-támogatás sikertelen inicializációja miatt");
	}
	exc_logged++;

	CrashCallback ();
}


void	debug_beginscope(void* location, char* scopeName, int threadId)
{
	DWORD oldProtect;
	FunctionData* funcDPtr = *((FunctionData**) location);
	if (funcDPtr == NULL) {
		if (VirtualProtect (location, 4, PAGE_READWRITE, &oldProtect)) {
			funcDPtr = functionDatas + (funcDataNum++);
			funcDPtr->functionName = scopeName;
			funcDPtr->functionTime[0] = funcDPtr->functionTime[1] = 0;
			*((FunctionData**) location) = funcDPtr;
			VirtualProtect (location, 4, oldProtect, &oldProtect);
		} else {
			DEBUGLOG(0,"\nRegistering function/debugscope %s has failed", scopeName);
			DEBUGLOG(1,"\nA %s függvény bejegyzése sikertelen", scopeName);
		}
	}

	if (threadId != -1)	{
		(history_data)[history_ptr[threadId]++] = funcDPtr;
		
		if (profilingEnabled) {
			_asm	{
				xor		eax,eax
				rdtsc
				mov		ecx,threadId
				mov		ecx,history_num[4*ecx]
				imul	ecx,1 * 8
				or		ecx,ecx
				jne		_not_toplevel
				sub		totalTime[0],eax
				sbb		totalTime[4],edx
			/*	jmp		_toplevel
			_not_toplevel:
				add		profileTimes[ecx-8].pureFunctionTime[0],eax
				adc		profileTimes[ecx-8].pureFunctionTime[4],edx	;elõzõ szint tiszta idejéhez: -(vége-eleje)
			_toplevel:*/
			_not_toplevel:
				mov		dword ptr sumOfProfileTimes[ecx].pureFunctionTime[0],0
				mov		dword ptr sumOfProfileTimes[ecx].pureFunctionTime[4],0

				mov		dword ptr profileTimes[ecx].pureFunctionTime[0],0
				mov		dword ptr profileTimes[ecx].pureFunctionTime[4],0
				sub		profileTimes[ecx].pureFunctionTime[0],eax
				sbb		profileTimes[ecx].pureFunctionTime[4],edx	;jelenlegi szint tiszta idejéhez: (vége-eleje)		
			}
		}
		history_num[threadId]++;
	}
}


void	debug_endscope(int threadId)
{
FunctionData	*functionData;
	
	if (threadId != -1)	{
		functionData = (history_data)[history_ptr[threadId]-1];
		history_num[threadId]--;
		history_ptr[threadId]--;
		if (profilingEnabled) {
			_asm	{
				xor		eax,eax
				rdtsc
				mov		ecx,threadId
				mov		ecx,history_num[4*ecx]
				imul	ecx,1 * 8
				or		ecx,ecx
				jnz		_not_toplevel
				add		totalTime[0],eax
				adc		totalTime[4],edx
				//jmp		_toplevel
			_not_toplevel:
				add		eax,profileTimes[ecx].pureFunctionTime[0]
				adc		edx,profileTimes[ecx].pureFunctionTime[4]	;jelenlegi szint tiszta idejéhez: (vége-eleje)

				jecxz	_toplevel
				add		sumOfProfileTimes[ecx-8].pureFunctionTime[0],eax
				adc		sumOfProfileTimes[ecx-8].pureFunctionTime[4],edx	;jelenlegi szint tiszta idejéhez: (vége-eleje)
			_toplevel:

				sub		eax,sumOfProfileTimes[ecx].pureFunctionTime[0]
				sbb		edx,sumOfProfileTimes[ecx].pureFunctionTime[4]	;elõzõ szint tiszta idejéhez: -(vége-eleje)

				mov		ecx,functionData
				jecxz	_end
				add		[ecx]FunctionData.functionTime[0],eax
				adc		[ecx]FunctionData.functionTime[4],edx
			_end:
			}
		}
	}
}


void	debug_begincritsection ()
{
	EnterCriticalSection (&debugCritSection);
}


void	debug_endcritsection ()
{
	LeaveCriticalSection (&debugCritSection);
}


void	debug_perf_measure_begin(__int64 *counter)	{

	_asm	{
		rdtsc
		mov		ecx,counter
		sub		[ecx],eax
		sbb		[ecx+4],edx
	}
}


void	debug_perf_measure_end(__int64 *counter)	{

	_asm	{
		rdtsc
		mov		ecx,counter
		add		[ecx],eax
		adc		[ecx+4],edx
	}
}


void	debug_set_language(int language) {
	
	debugLanguage = language;

}


void	debug_set_module_instance(HINSTANCE hInstance) {

	hModInstance = hInstance;
}

#endif