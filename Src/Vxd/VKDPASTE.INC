;******************************************************************************
;
;   (C) Copyright MICROSOFT Corp., 1989-1990
;
;   Title:	vkdpaste.inc -
;
;   Version:	1.00
;
;   Date:	20-Mar-1989
;
;   Author:	RAP
;
;------------------------------------------------------------------------------
;
;   Change log:
;
;      DATE	REV		    DESCRIPTION
;   ----------- --- -----------------------------------------------------------
;   20-Mar-1989 RAP
;   23-Mar-1989 RAP added PM API equates
;
;==============================================================================

;
; structure of entries in paste buffer
;
Paste_Rec STRUC
Paste_Char	    db	?   ; OEM char value
Paste_ScanCode	    db	?   ; keyboard scan code
Paste_ShiftState    dw	?   ; shift state
Paste_Rec ENDS

Paste_Key   equ word ptr Paste_Char


;BeginDoc
;------------------------------------------------------------------------------
;
; API services:
;
VKD_API_Version     equ 0
	;   ENTER:  EAX = 0
	;   EXIT:   AH = major version #
	;	    AL = minor version #
	;	    Carry clear

;
; Whenever the PM API changes in a backwards-compatible way,
; increment the low byte.
;
; Whenever the PM API changes in a way that is not backwards-compatible,
; increment the high byte and reset the low byte to zero.

VKD_PM_API_Version_Major equ 02h
VKD_PM_API_Version_Minor equ 01h
VKD_PM_API_Version equ VKD_PM_API_Version_Major * 256 + VKD_PM_API_Version_Minor

VKD_Force_Key	    equ 1
	;   ENTER:  EAX = 1
	;	    EBX = VM handle  (0 for current)
	;	    CH = scan code
	;	    CL = repeat count
	;	    EDX = shift state  (-1 means no change)
	;   EXIT:   Carry set, if failed

;
; shift state equates.  All other bits are reserved and must be zero.
;
FKSS_Insert_Down    equ 1000000000000000b
FKSS_CapLck_Down    equ 0100000000000000b
FKSS_NumLck_Down    equ 0010000000000000b
FKSS_ScrLck_Down    equ 0001000000000000b
FKSS_Insert_On	    equ 0000000010000000b
FKSS_CapLck_On	    equ 0000000001000000b
FKSS_NumLck_On	    equ 0000000000100000b
FKSS_ScrLck_On	    equ 0000000000010000b
FKSS_Alt_Down	    equ 0000000000001000b
FKSS_Ctrl_Down	    equ 0000000000000100b
FKSS_Shift_Down     equ 0000000000000010b
FKSS_Extended_Key   equ	0000000000000001b ; New for API version 2.1

ifdef	NEC_98
;
; This function is for only NEC Win95 BUT
; You MUST NOT assign this number for the new function because we
; have to keep the compatibility between US and NEC version.
;  NEC PC-9800s cannot be performed the valiable key repeat because
;  the H/W doesn't have this feature. So we realized it to emulate
;  by VKD.
;  Keyboard driver receives the message when the user changes repeat
;  parameter with CPL, keyboard driver notifies it to VKD via this
;  function.
;
VKD_Set_Speed	    equ 2
	;   ENTER:  EAX = 2
	;	    BX = Repeat mode
	;	        = 1 : No repeat
	;	        = 2 : Repeat by KEYBOARD.DRV(compatible with Win3.1)
	;	        = 3 : Repeat by VKD(New feature from Win95)
	;	    CH = repeat delay
	;	    CL = repeat rate
	;   EXIT:   Carry set, if failed

endif	; NEC_98

;EndDoc
