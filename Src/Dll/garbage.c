/*--------------------------------------------------------------------------------- */
/* Garbage.c - General work-testing codes; this file is needless, I think (I HOPE)  */
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


DWORD WINAPI jmpthread(LPVOID lpParameter)	{
	SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_LOWEST);
	_asm	{
		ide:
		inc  byte ptr ds:[0xb8004]
		jmp ide
	}
}

void (_stdcall *VDDSimulateInterrupt)(int,int,int);

void __declspec(dllexport) initproc()	{
HINSTANCE hInst;
	
	hInst = GetModuleHandle("ntvdm.exe");
	if (hInst==NULL)	{
		DEBUGLOG(0,"\nCannot get module handle of ntvdm.exe");
		DEBUGLOG(1,"\nNem tudom lekérdezni az ntvdm.exe module handle-jét");
	}
	_getCH = (unsigned char (_stdcall *)() ) GetProcAddress(hInst,"getCH");
	_getBX = (unsigned char (_stdcall *)() ) GetProcAddress(hInst,"getBX");
	_setEAX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst,"setEAX");
	_setECX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst,"setECX");
	_setEDX = (void (_stdcall *)(unsigned int) ) GetProcAddress(hInst,"setEDX");
	_VDDSimulateInterrupt = (void (_stdcall *)(int,int,int) ) GetProcAddress(hInst,"VDDSimulateInterrupt");
	//CreateThread(NULL,4096,jmpthread,consHwnd,0,&NTMsgLoopThreadID);
}

void __declspec(dllexport) egyproc()	{
	//SwitchToThread();
	//(*VDDSimulateInterrupt(ICA_MASTER,7,1);
	//(*VDDSimulateInterrupt(ICA_MASTER,7,1);
	//(*VDDSimulateInterrupt(ICA_MASTER,7,1);
	//(*setEDX)(0x12345678);
	LOG(1,"\nBIOS: %0x",(*getBX)() & 0xFFFF);
	return;
}

void _stdcall inb_handler(WORD iport, BYTE *data)	{

	LOG(1,"\n_inb(%0x) = %0x at EIP: %0x",iport,*data,(*getEIP)());
}

void _stdcall inw_handler(WORD iport, WORD *data)	{

	LOG(1,"\n_inw(%0x) = %0x at EIP: %0x",iport,*data,(*getEIP)());
}

void _stdcall insb_handler(WORD iport, BYTE *data, WORD count)	{
int i;

	LOG(1,"\n_insb(%0x) (count=%0x) = ",iport,count);
	for(i=0;i<count;i++) LOG(1,"%0x ",data[i]);
	LOG(1," at EIP: %0x",(*getEIP)());
}

/*void _stdcall outb_handler(WORD iport, BYTE data)	{

	_asm ide: jmp ide
	LOG(1,"\n_outb(%0x,%0x) at EIP: %0x",iport,data,(*getEIP)());
}*/

void _stdcall outw_handler(WORD iport, WORD data)	{

	LOG(1,"\n_outw(%0x,%0x) at EIP: %0x",iport,data,(*getEIP)());
}

void _stdcall outsb_handler(WORD iport, BYTE *data, WORD count)	{
int i;

	LOG(1,"\n_outsb(%0x) (count=%0x) = ",iport,count);
	for(i=0;i<count;i++) LOG(1,"%0x ",data[i]);
	LOG(1," at EIP: %0x",(*getEIP)());
}

void _stdcall outsw_handler(WORD iport, WORD *data, WORD count)	{
int i;

	LOG(1,"\n_outsw(%0x) (count=%0x) = ",iport,count);
	for(i=0;i<count;i++) LOG(1,"%0x ",data[i]);
	LOG(1," at EIP: %0x",(*getEIP)());
}
