;******************************************************************************
;
;   (C) Copyright MICROSOFT Corp., 1988-1990
;
;   Title:	vkd.inc -
;
;   Version:	1.00
;
;   Date:	10-Aug-1988
;
;   Author:	RAP
;
;------------------------------------------------------------------------------
;
;   Change log:
;
;      DATE	REV		    DESCRIPTION
;   ----------- --- -----------------------------------------------------------
;   10-Aug-1988 RAP started re-write of VKD
;   03-Nov-1988 RAP separated internal stuff out into VKDSYS.INC
;   08-Aug-1992 SSS seperated VAD from VKD
;   16-Jun-1993 rjc New VKD_Put_Byte service for accessibility
;
;==============================================================================


Begin_Service_Table VKD

VKD_Service	VKD_Get_Version, LOCAL
VKD_Service	VKD_Define_Hot_Key, LOCAL
VKD_Service	VKD_Remove_Hot_Key, LOCAL
VKD_Service	VKD_Local_Enable_Hot_Key, LOCAL
VKD_Service	VKD_Local_Disable_Hot_Key, LOCAL
VKD_Service	VKD_Reflect_Hot_Key, LOCAL
VKD_Service	VKD_Cancel_Hot_Key_State, LOCAL
VKD_Service	VKD_Force_Keys, LOCAL
VKD_Service	VKD_Get_Kbd_Owner, LOCAL
VKD_Service	VKD_Define_Paste_Mode, LOCAL
VKD_Service	VKD_Start_Paste, LOCAL
VKD_Service	VKD_Cancel_Paste, LOCAL
VKD_Service	VKD_Get_Msg_Key, LOCAL
VKD_Service	VKD_Peek_Msg_Key, LOCAL
VKD_Service	VKD_Flush_Msg_Key_Queue, LOCAL

IFNDEF	WIN31COMPAT
;
; The following services are new for Windows 4.0.
;
VKD_Service	VKD_Enable_Keyboard,LOCAL
VKD_Service	VKD_Disable_Keyboard,LOCAL
VKD_Service	VKD_Get_Shift_State,LOCAL
VKD_Service	VKD_Filter_Keyboard_Input
VKD_Service	VKD_Put_Byte
VKD_Service	VKD_Set_Shift_State,LOCAL

;
; New for Windows 98 (VKD version 0300h)
;
VKD_Service VKD_Send_Data
VKD_Service VKD_Set_LEDs
VKD_Service VKD_Set_Key_Rate
;VKD_Service VKD_Get_Key_Rate


ENDIF


End_Service_Table VKD


VKD_Version     equ 0300h

;******************************************************************************
;
; shift state bits for both global & local shift states
;
ifdef	NEC_98
                      ;;5432109876543210
SS_Shift        equ     0000000000000001b
SS_Caps         equ     0000000000000010b
SS_Kana         equ     0000000000000100b
SS_Alt          equ     0000000000001000b
SS_Ctrl         equ     0000000000010000b
SS_Win		equ	1000000000000000b   ; Nexus Windows-Logo key (L or R)
SS_Unused       equ     0111111111100000b

SS_LShift       equ     SS_Shift
SS_RShift       equ     SS_Shift
SS_LAlt         equ     SS_Alt
SS_RAlt         equ     SS_Alt
SS_LCtrl        equ     SS_Ctrl
SS_RCtrl        equ     ----------SS_Ctrl

SS_Toggle_mask      equ     0
SS_Toggle_Dn_mask   equ     0
SS_Shift_mask       equ     SS_Shift + SS_Ctrl + SS_Alt
SS_Lock_mask        equ     SS_Caps + SS_Kana

SS_Either_Shift     equ     SS_Shift
SS_Either_Ctrl      equ     SS_Ctrl
SS_Either_Alt       equ     SS_Alt

else	;NEC_98
		      ;;5432109876543210
SS_Shift	equ	0000000000000001b   ; left or right
SS_LShift	equ	0000000000000010b
SS_RShift	equ	0000001000000000b
SS_Ctrl 	equ	0000000010000000b   ; left or right
SS_LCtrl	equ	0000000000000100b
SS_RCtrl	equ	0000010000000000b
SS_Alt		equ	0000000100000000b   ; left or right
SS_LAlt 	equ	0000000000001000b
SS_RAlt 	equ	0000100000000000b
SS_CapLock	equ	0000000001000000b
SS_NumLock	equ	0000000000100000b
SS_ScrlLock	equ	0000000000010000b
SS_CapLock_Dn	equ	0100000000000000b   ; SS_CapLock shl 8
SS_NumLock_Dn	equ	0010000000000000b   ; SS_NumLock shl 8
SS_ScrlLock_Dn	equ	0001000000000000b   ; SS_ScrlLock shl 8
SS_Win		equ	1000000000000000b   ; Nexus Windows-Logo key (L or R)
SS_Unused	equ	0000000000000000b

SS_Toggle_mask	    equ     SS_CapLock + SS_NumLock + SS_ScrlLock
SS_Toggle_Dn_mask   equ     SS_CapLock_Dn + SS_NumLock_Dn + SS_ScrlLock_Dn
SS_LShift_mask	    equ     SS_LShift + SS_LCtrl + SS_LAlt
SS_Shift_mask	    equ     SS_LShift_mask + SS_RShift + SS_RCtrl + SS_RAlt

SS_Either_Shift     equ     SS_LShift + SS_RShift
SS_Either_Ctrl	    equ     SS_LCtrl + SS_RCtrl
SS_Either_Alt	    equ     SS_LAlt + SS_RAlt
endif	;NEC_98


ifdef	NEC_98
; LED state bits
;       xxxxxxxxxx
;       bit7  |||+ non
;      -bit4  ||+- non
;      =CMD   |+-- kana
;             +--- caps
;
LED_ScLok       equ     1                   ; Scroll Lock
LED_NmLok       equ     2                   ; Num Lock
LED_KanaLok     equ     4                   ; Kana Lock
LED_CpLok       equ     8                   ; Caps Lock
Toggle_To_LED   equ     1                   ; "state shr Toggle_To_LED" converts
                                            ; state bits into kybd LED bits
else	;NEC_98

; LED state bits
LED_ScLok	equ	1		    ; Scroll Lock
LED_NmLok	equ	2		    ; Num Lock
LED_CpLok	equ	4		    ; Caps Lock
Toggle_To_LED	equ	4		    ; "state shr Toggle_To_LED" converts
					    ; state bits into kybd LED bits
endif	;NEC_98

; VKD_Define_Hot_Key equates & macro

ExtendedKey_B	equ 1
ExtendedKey	equ 100h
AllowExtended_B equ 0FFh
AllowExtended	equ 0FF00h


;VKD_Define_Hot_Key flags:

CallOnPress	equ 1		; Call call-back when key press is detected
CallOnRelease	equ 2		; Call call-back when key release is detected
				; (keyboard may still be in hot-key hold state)
CallOnRepeat	equ 4		; Call call-back when repeated press is detected
CallOnComplete	equ 8		; Call call-back when the hot key state is
				; ended (all shift modifier keys are released)
				; or when a different hot key is entered
				; (i.e. pressing ALT 1 2, if both ALT-1
				;  and ALT-2 are defined hot keys, then ALT-1's
				;  call back will be called before ALT-2's to
				;  indicate that the ALT-1 is complete even
				;  though the ALT key is still down)
CallOnUpDwn	equ CallOnPress + CallOnRelease
				; Call call-back on both press and release
CallOnAll	equ 0		; Call call-back on press, release, auto-repeat
				; and complete
PriorityNotify	equ 10h 	; notification calls to the call-back can only
				; happen when interrupts are enabled and
				; the critical section is un-owned
Monitor_Key	equ 40h		; New for Windows 4.0
				; Automatically reflect the hotkey.
Local_Key	equ 80h 	; key can be locally enabled/disabled

Hot_Key_Pressed     equ 0
Hot_Key_Released    equ 1
Hot_Key_Repeated    equ 2
Hot_Key_Completed   equ 3

Hot_Key_Monitor	    equ 40h	; New for Windows 4.0
Hot_Key_MonitorBit  equ 6	; New for Windows 4.0

Hot_Key_SysVM_Notify	 equ 80h
Hot_Key_SysVM_Notify_bit equ 7

BeginDoc
;******************************************************************************
;
;   ShiftState
;
;   DESCRIPTION:    Macro to aid in setting EBX with the shift state mask and
;		    shift state compare value for calling VKD_Define_Hot_Key.
;
;   USAGE:	    ShiftState mask, compare
;
;		    The mask specifies the shift state bits that should be
;		    excluded before the compare is done.
;
;==============================================================================
EndDoc
ShiftState MACRO mask, compare
	mov	ebx, ((NOT mask) SHL 16) + compare
	   ENDM

; common shift states

ifdef	NEC_98
HKSS_Shift  equ (NOT SS_Toggle_mask) SHL 16 + SS_Shift
HKSS_Ctrl   equ (NOT SS_Toggle_mask) SHL 16 + SS_Ctrl
HKSS_Alt    equ (NOT SS_Toggle_mask) SHL 16 + SS_Alt
else	;NEC_98
HKSS_Shift  equ (NOT (SS_Either_Shift + SS_Toggle_mask)) SHL 16 + SS_Shift
HKSS_Ctrl   equ (NOT (SS_Either_Ctrl + SS_Toggle_mask)) SHL 16 + SS_Ctrl
HKSS_Alt    equ (NOT (SS_Either_Alt + SS_Toggle_mask)) SHL 16 + SS_Alt
endif	;NEC_98

; include structure of entries in paste buffer

include VKDpaste.INC


; paste completion flags passed to the paste call-back

Paste_Complete	equ 0	; paste successful
Paste_Aborted	equ 1	; paste aborted by user
Paste_VM_Term	equ 2	; paste aborted because VM terminated

;
; Modifier byte for Message Keys, retrieved with services VKD_Get_Msg_Key and
; VKD_Peek_Msg_Key
;
ifdef	NEC_98
MK_Shift        equ 00000001b
MK_Kana         equ 00000010b
MK_Caps         equ 00000100b
MK_Alt          equ 00001000b
MK_Ctrl         equ 00010000b
MK_Extended     equ 10000000b
else	;NEC_98
MK_Shift	equ 00000010b
MK_Ctrl 	equ 00000100b
MK_Alt		equ 00001000b
MK_Extended	equ 10000000b
endif	;NEC_98
