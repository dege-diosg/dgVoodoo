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

#include <windows.h>
#include <stdio.h>
#include "debug.h"
#include "dgw.h"

#ifdef	DEBUG

/*------------------------------------------------------------------------------------------*/
/*........................... Definíciók debug-támogatás esetén ............................*/

#define	NUM_OF_EXCEPTIONS		20

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
char				*history_strings;
int					history_ptr[MAX_THREADNUM];
int					history_num[MAX_THREADNUM];
int					threadNum = 0;
int					exc_logged = 0;
unsigned int		tickCount[MAX_THREADNUM*MAX_DEBUGNESTLEVEL] = {0, 0, 0, 0, 0};
unsigned int		maxticks[MAX_LISTELEMENTS] = {0, 0, 0, 0, 0};
ProfileTimes		profileTimes[MAX_DEBUGNESTLEVEL];
char				ftimes[32*MAX_DEBUGNESTLEVEL];
FunctionData		functionDatas[512];
int					numOfRegisteredFuncs;
unsigned int		globalTime[2];
int					debugLanguage;
HINSTANCE			hModInstance;
unsigned int		totalTime[2] = {0,0};


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


int debug_thread_register()	{
int i;
	
	if (!debugInited)	{
		if ( (history_strings = _GetMem(MAX_THREADNUM*MAX_DEBUGNESTLEVEL*32)) == NULL) return(-1);
		for(i = 0; i < MAX_THREADNUM; i++) history_ptr[i] = history_num[i] = 0;
		ZeroMemory(profileTimes, MAX_DEBUGNESTLEVEL * sizeof(__int64));
		ZeroMemory(functionDatas, sizeof(functionDatas));
		numOfRegisteredFuncs = 0;
		debugInited = 1;
	}
	history_ptr[threadNum] = threadNum*MAX_DEBUGNESTLEVEL*32;
	return(threadNum++);
}


FunctionData*	get_funcdataptr(char *str, int num)	{
FunctionData*	funcDPtr;

	funcDPtr = functionDatas+num;
	funcDPtr->functionName = str;
	return funcDPtr;
/*	funcDPtr = * (FunctionData**) str;
	if (funcDPtr==NULL)	{
		functionDatas[numOfRegisteredFuncs].functionName = str+4;
		functionDatas[numOfRegisteredFuncs].functionTime[0] =
		functionDatas[numOfRegisteredFuncs].functionTime[4] = 0;
		funcDPtr = * (FunctionData**) str = functionDatas+numOfRegisteredFuncs;
		numOfRegisteredFuncs++;
	}
	return(funcDPtr);*/
}


float	getdiv64(unsigned int *int64)	{
double	f, f2;

	f = ((double) int64[1]) * ((double) (2<<16)) * ((double) (2<<16));
	f += (double) int64[0];
	
//	f2 = ((double) globalTime[1]) * ((double) (2<<16)) * ((double) (2<<16));
//	f2 += (double) globalTime[0];
	f2 = ((double) totalTime[1]) * ((double) (2<<16)) * ((double) (2<<16));
	f2 += (double) totalTime[0];
	return (float) (100.0*f/f2);
}


float	debug_get_perf_quotient(__int64 counter1, __int64 counter2)	{
	
	return( ((float) counter1) / ((float) counter2) );
}


void debug_global_time_begin()	{
	_asm	{
		rdtsc
		mov		globalTime[0],eax
		mov		globalTime[4],edx
	}
}


void debug_global_time_end()	{
int		i;
float	f;

	_asm	{
		rdtsc
		sub		eax,globalTime[0]
		sbb		edx,globalTime[4]
		mov		globalTime[0],eax
		mov		globalTime[4],edx
	}
	for(i=0; i<512; i++)	{
		if (functionDatas[i].functionName != NULL)	{
			if ( (f = getdiv64(functionDatas[i].functionTime)) > 0.001f )
				DEBUGLOG(2,"\n%s: %f", functionDatas[i].functionName, f);
		}
	}
}



void _exception(EXCEPTION_POINTERS *excp, FunctionData *functionData, int threadid)	{
char	history[10*1024];
int		i,ptr;
char	*exceptionString;
char	*infunction;

	if (exc_logged) return;
	infunction = functionData->functionName;
	for(i = 0; i < NUM_OF_EXCEPTIONS; i++) if (excp->ExceptionRecord->ExceptionCode == exceptionCodes[i]) break;
	exceptionString = (i != NUM_OF_EXCEPTIONS) ? exceptionStrings[i] : "UNKNOWN";
	
	DEBUGLOG(0,"\n\nException occured in function: %s\nException: %s",infunction, exceptionString);
	DEBUGLOG(1,"\n\nKivétel keletkezett a következõ függvény végrehajtásakor: %s\nKivétel: %s",infunction, exceptionString);
	DEBUGLOG(2,"\n\nEAX: %0p  EBX: %0p  ECX: %0p  EDX: %0p", excp->ContextRecord->Eax, excp->ContextRecord->Ebx,
			 excp->ContextRecord->Ecx, excp->ContextRecord->Edx);
	DEBUGLOG(2,"\nESI: %0p  EDI: %0p  EBP: %0p  ESP: %0p", excp->ContextRecord->Esi, excp->ContextRecord->Edi,
			 excp->ContextRecord->Ebp, excp->ContextRecord->Esp);
	DEBUGLOG(2,"\nEIP: %0p  EFlags: %0p, DLL Base=%0p", excp->ContextRecord->Eip, excp->ContextRecord->EFlags,
			 hModInstance);
	if (threadid!=-1)	{
		history[0]=0;
		ptr=threadid*MAX_DEBUGNESTLEVEL*32;
		for(i=0;i<history_num[threadid];i++)	{
			DebugStrCat(history,history_strings+ptr);
			while(history_strings[ptr]!=0) ptr++;
			if (i!=(history_num[threadid]-1)) DebugStrCat(history,"->");
			ptr++;
		}
		DEBUGLOG(0,"\n\nCalling history: %s",history);
		DEBUGLOG(1,"\n\nHívási sorrend: %s",history);
	} else {
		DEBUGLOG(0,"\n   Error: Calling history is not available due to unsuccessful debug support initialization");
		DEBUGLOG(1,"\n   Hiba: A hívási verem nem elérhetõ a debug-támogatás sikertelen inicializációja miatt");
	}
	exc_logged++;
}


void handle_history_strings(FunctionData *functionData, int threadID)	{
int		i = 0;
char	*inFunction;

	if (threadID!=-1)	{
		inFunction = functionData->functionName;
		do history_strings[history_ptr[threadID]++] = inFunction[i]; while(inFunction[i++] != 0);
		tickCount[threadID*MAX_DEBUGNESTLEVEL+history_num[threadID]]=GetTickCount();
		
		_asm	{
			xor		eax,eax
			rdtsc
			mov		ecx,threadID
			mov		ecx,history_num[4*ecx]
			imul	ecx,1 * 8
			jecxz	_top_level
			add		profileTimes[ecx-8].pureFunctionTime[0],eax
			adc		profileTimes[ecx-8].pureFunctionTime[4],edx	;elõzõ szint tiszta idejéhez: -(vége-eleje)
			jmp		_not_toplevel
		_top_level:
			sub		totalTime[0],eax
			sbb		totalTime[4],edx
		_not_toplevel:
			mov		dword ptr profileTimes[ecx].pureFunctionTime[0],0
			mov		dword ptr profileTimes[ecx].pureFunctionTime[4],0
			sub		profileTimes[ecx].pureFunctionTime[0],eax
			sbb		profileTimes[ecx].pureFunctionTime[4],edx	;jelenlegi szint tiszta idejéhez: (vége-eleje)			
		}
		history_num[threadID]++;
	}
	//_exception(NULL,infunction);
}

void handle_history_strings_out(FunctionData *functionData, int threadID)	{
unsigned int	tc,i,j;
char			*inFunction;
	
	if (threadID!=-1)	{
		inFunction = functionData->functionName;
		history_num[threadID]--;
		tc=GetTickCount()-tickCount[threadID*MAX_DEBUGNESTLEVEL+history_num[threadID]];
		for(i=0;i<MAX_LISTELEMENTS;i++) if (tc>maxticks[i]) {
			for(j=MAX_LISTELEMENTS-1;j>i;j--)	{
				maxticks[j]=maxticks[j-1];
				ftimes[j*32]=0;
				DebugStrCat(ftimes+j*32,ftimes+(j-1)*32);
			}
			maxticks[i]=tc;
			ftimes[i*32]=0;
			DebugStrCat(ftimes+i*32,inFunction);
			break;
		}
		history_ptr[threadID]--;
		_asm	{
			rdtsc
			mov		ecx,threadID
			mov		ecx,history_num[4*ecx]
			imul	ecx,1 * 8
			jecxz	_top_level
			sub		profileTimes[ecx-8].pureFunctionTime[0],eax
			sbb		profileTimes[ecx-8].pureFunctionTime[4],edx	;elõzõ szint tiszta idejéhez: -(vége-eleje)
			jmp		_not_toplevel
		_top_level:
			add		totalTime[0],eax
			adc		totalTime[4],edx
		_not_toplevel:
			add		profileTimes[ecx].pureFunctionTime[0],eax
			adc		profileTimes[ecx].pureFunctionTime[4],edx	;jelenlegi szint tiszta idejéhez: (vége-eleje)
			mov		eax,profileTimes[ecx].pureFunctionTime[0]
			mov		edx,profileTimes[ecx].pureFunctionTime[4]
			mov		ecx,functionData
			add		[ecx].functionTime[0],eax
			adc		[ecx].functionTime[4],edx
		}
		while( (history_ptr[threadID]!=threadID*MAX_DEBUGNESTLEVEL*32) && (history_strings[--history_ptr[threadID]]!=0) );
		if (history_ptr[threadID]!=(threadID*MAX_DEBUGNESTLEVEL*32) ) history_ptr[threadID]++;
	}
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


void	debug_log_maxtimes()	{
int i;

	DEBUGLOG(2,"\n----------------------------------------------------------------------------------------------");
	DEBUGLOG(0,"\nTime statistics:");
	DEBUGLOG(0,"\nFirst %d functions spending most time",MAX_LISTELEMENTS);
	DEBUGLOG(1,"\nIdõstatisztika:");
	DEBUGLOG(1,"\nAz elsõ %d függvény, amelyek végrehajtása a legtöbb idõt vette igénybe:",MAX_LISTELEMENTS);
	for(i=0;i<MAX_LISTELEMENTS;i++) if (ftimes[i*32]) DEBUGLOG(2,"\n  %s, %dms",ftimes+i*32,maxticks[i]);
	DEBUGLOG(2,"\n----------------------------------------------------------------------------------------------");
}


void	debug_set_language(int language) {
	
	debugLanguage = language;

}


void	debug_set_module_instance(HINSTANCE hInstance) {

	hModInstance = hInstance;
}

#endif