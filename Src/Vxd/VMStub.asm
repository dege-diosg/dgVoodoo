.386

INCLUDE	..\..\Inc\RMDriver.inc

;INT 10h stub, amelyet a kernelmodul minden dos-os virtu lis g‚p
;l‚trehoz sakor beilleszt abba. Ez az‚rt fontos, hogy a 10h-s
;megszak¡t st mindig el tudjuk cs¡pni, ne csak akkor, amikor
;t‚nylegesen egy INT utas¡t ssal h¡v¢dik meg.

code segment use16
        assume cs:code, ds:code

org 100h

start:

vesaDriverHeader	LABEL _VesaDriverHeader
	DD	'EGED'
	DW	OFFSET Temp
	DW	0
	DW	0
	DW	0

	DW	0
	DD	0
	DW	0
	DW	0
	DW	OFFSET vesaDriverProttable
	DW	vesaDriverProtTableEnd - vesaDriverProttable
	DB	0

vDrvHeaderPointer	LABEL WORD
		DW	OFFSET	vesaDriverHeader

Temp:   or      cs:vesaDriverHeader.semafor,0h
        jne     regi
        mov     cs:vesaDriverHeader.semafor,1h
        int     10h
        mov     cs:vesaDriverHeader.semafor,0h
vege:   iret
regi:   mov     cs:vesaDriverHeader.semafor,0h
        jmp     DWORD PTR cs:[vesaDriverHeader.origInt10Handler]

;V‚dett m¢d£ interf‚sz t bl zata: nincsenek I/O c¡mek ‚s mem¢riatartom nyok
;A h rom szolg ltat s k¢dja egy INT 10h-t hajt v‚gre
vesaDriverProttable       LABEL
        DW      8
        DW      19
        DW      28
        DW      0
        DB      60h,33h,0DBh,66h,0B8h,05h,4Fh,0CDh,10h,61h,0C3h
        DB      60h,66h,0B8h,07h,4Fh,0CDh,10h,61h,0C3h
	
	DB      60h,0Ah,0F6h,74h,0Dh,90h,90h,90h,90h,66h,0B8h,01h,4Fh
	DB	0EBh,12h,90h,90h,90h,8Ah,0FAh,8Ch,0C6h,8Bh,0D7h,0C1h,0EAh,10h,66h,0B8h,0Bh,4Fh,0CDh,10h,61h,0C3h


vesaDriverProtTableEnd    LABEL

code ends
end start
