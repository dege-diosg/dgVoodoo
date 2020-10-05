	PAGE 58,132
;*******************************************************************************
;
;  Title:      VMD.INC
;
;  Version:    1.0
;
;  Date:       16 May 1992
;
;  Author:     TCS
;
;*******************************************************************************
;
;  Change log:
;
;  DATE        REV DESCRIPTION
;  ----------- --- ------------------------------------------------------------
;  16 May 1992 TCS Original implementation
;  09 Sep 1992 SSS Rewrote it, well most of it.
;
;*******************************************************************************

VMD_Major_Version	EQU 04h
VMD_Minor_Version	EQU 00h

VMOUSE_Device_ID	EQU VMD_Device_ID

VMDINT33_Major_Version	EQU 08h
VMDINT33_Minor_Version	EQU 30h

IFNDEF	Not_VxD

Begin_Service_Table	VMOUSE

VMOUSE_Service	VMD_Get_Version, LOCAL
VMOUSE_Service	VMD_Set_Mouse_Type, LOCAL
VMOUSE_Service	VMD_Get_Mouse_Owner, LOCAL
VMOUSE_Service	VMD_Post_Pointer_Message
VMOUSE_Service	VMD_Set_Cursor_Proc
VMOUSE_Service	VMD_Call_Cursor_Proc
VMOUSE_Service	VMD_Set_Mouse_Data
VMOUSE_Service	VMD_Get_Mouse_Data
VMOUSE_Service	VMD_Manipulate_Pointer_Message
VMOUSE_Service	VMD_Set_Middle_Button
VMOUSE_Service	VMD_Enable_Disable_Mouse_Events
VMOUSE_Service	VMD_Post_Absolute_Pointer_Message

End_Service_Table	VMOUSE

ENDIF

;
; The services provided by VMD for V86 and Prot mode apps.
;

VMDAPI_GET_VERSION			EQU	0
VMDAPI_UPDATE_MOUSE_STATE		EQU	1 ; available in SysVM only
VMDAPI_QUERY_WINDOWED_MOUSE_SUPPORT	EQU	2 ; available in SysVM only
VMDAPI_SET_MOUSE_EVENT_CALLBACK		EQU	3 ; available in SysVM only
VMDAPI_SET_MOUSE_FOCUS			EQU	4 ; available in SysVM only
VMDAPI_GET_MOUSE_INFO			EQU	5 ; available in SysVM only
VMDAPI_SET_MOUSE_TYPE			EQU	100h
VMDAPI_DETECT_MOUSE			EQU	101h
VMDAPI_GET_ADDON			EQU	102h
VMDAPI_MOUSEDRV_CONSISTENT		EQU	103h

;
; Types of mice recognized by VMD.
;
VMD_Type_Undefined	EQU	0
VMD_Type_Bus		EQU	1
VMD_Type_Serial 	EQU	2
VMD_Type_InPort 	EQU	3
VMD_Type_PS2		EQU	4
VMD_Type_HP		EQU	5
VMD_Type_Serial_w_port	EQU	VMD_Type_Serial OR 80h
VMD_Type_MSeries	EQU	6
VMD_Type_CSeries	EQU	7

;
; Data structure describing mouse device Flags, IO Base, IRQ number,
;  Mouse type, Mouse port number, mini driver reference, DevNode handle,
;  IRQ handle, interrupt routine, reset routine and disable routine.
; The mini mouse driver receives this data structure, does its detection
;  and sets appropriate flags and then fills in the HW int, reset and
;  disable routine addresses(zero address means NULL routine). The
;  reference data can be used in any way the mini driver wishes.
;

Mouse_Instance	STRUC

MI_Flags	DW	0		; status flags (defined below)
MI_IO_Base	DW	0		; base to read
MI_IRQNumber	DB	0		; irq#
MI_PortNum	DB	0		; port number (serial mice ?)
MI_MouseType	DW	0		; type of mouse (defined above)
MI_Reference	DD	0		; reference data
MI_hDevNode	DD	0		; dev node being handled
MI_hIRQ		DD	0		; VPICD's irq handle
MI_HWINT	DD	0		; hardware int proc
MI_Reset	DD	0		; Reset routine
MI_Disable	DD	0		; disable routine
MI_Initialize	DD	0		; initialization routine
					; It detects and virtualizes hardware.
MI_LoadHandle	DD	0		; load handle of loaded VxD.

Mouse_Instance	ENDS

; The MI_Flags definitions
MIF_Alloc_Bit           EQU     0                       ; Instance is allocated
MIF_Alloc               EQU     1 SHL MIF_Alloc_Bit
MIF_Active_Bit          EQU     1                       ; Interrupt is active
MIF_Active              EQU     1 SHL MIF_Active_Bit
MIF_Detected_Bit        EQU     2                       ; Device detected
MIF_Detected            EQU     1 SHL MIF_Detected_Bit
MIF_BadDevNode_Bit      EQU     3                       ; Device not detected
MIF_BadDevNode          EQU     1 SHL MIF_BadDevNode_Bit
MIF_NewConfig_Bit       EQU     4                       ; No DevNode for device
MIF_NewConfig           EQU     1 SHL MIF_NewConfig_Bit
MIF_BadIO_Bit           EQU     5                       ; Wrong IO in DevNode
MIF_BadIO               EQU     1 SHL MIF_BadIO_Bit
MIF_BadIRQ_Bit          EQU     6                       ; Wrong IRQ in DevNode
MIF_BadIRQ              EQU     1 SHL MIF_BadIRQ_Bit
MIF_NoIRQ_Bit           EQU     7                       ; Do not virtualize IRQ
MIF_NoIRQ               EQU     1 SHL MIF_NoIRQ_Bit
MIF_BUSMOUSE_Bit        EQU     8                       ; BUSMOUSE
MIF_BUSMOUSE            EQU     1 SHL MIF_BUSMOUSE_Bit
MIF_INPMOUSE_Bit        EQU     9                       ; INPORT
MIF_INPMOUSE            EQU     1 SHL MIF_INPMOUSE_Bit
MIF_AUXMOUSE_Bit        EQU     10                      ; AUX (PS/2) mouse
MIF_AUXMOUSE            EQU     1 SHL MIF_AUXMOUSE_Bit
MIF_OTHERMOUSE_Bit      EQU     11                      ; 3rd party mouse
MIF_OTHERMOUSE          EQU     1 SHL MIF_OTHERMOUSE_Bit

;
; The DevNode for a Mouse device looks like follows:
;
; * Hardware section *
;
; [HLM\Enum\root\*PNPXXXX]
; DeviceDesc=MouseType
; Class=MOUSE
; Driver=Pointer to driver (software section) (Mouse\xxxx).
; BootConfig=..... (Base and IO ranges allocated to the mouse).
; MouseDriver= Optional (handles specific hardware and or software).
;
; * Software section *
;
; [HLM\System\CurrentControlSet\Services\Class\Mouse\xxxx]
; DriverDesc=description of driver
; DevLoader=*VMOUSE
; MouseType=type of mouse (Serial,PS2,Inport,Bus etc.)
;
;
; Till Device_Init, VMOUSE returns all New_DevNode calls with
; CR_DEVLOADER_NOT_READY.
;
; After the Device_Init:
;
; When VMOUSE gets a new devnode call for such a node,
; If MouseDriver entry doesn't exist (of the form mousedriver=xyz.vxd) then
;  look at the MouseType and call the appropriate routine to detect the mouse
;  and configure the mouse support.
; else
;  load the VxD. Then call the SYS_DYNAMIC_DEVICE_INIT proc for the VxD
;  with EBX = VM handle and ECX -> Mouse_Instance structure. The VxD
;  returns with the Mouse_Instance struct initialized. If a mouse is found
;  by the xyz.vxd, then it should call VMD_Set_Mouse_Data to let it know 
;  VMOUSE about the mouse. 
;
; If by the start of the Init_Complete process, VMOUSE doesn't detect a mouse,
;  it searches for all mice it can handle and configures the first mouse.
;  Now it remembers the type of the mouse found AND uses this information
;  at the start of the next boot (after it has gone through the above
;  process of handling devnodes) to reduce the boot time.
;

Point_Struc			STRUC

PS_Y	dw	?
PS_X	dw	?

Point_Struc			ENDS

;
; Structs related to Cursor VxDs
;

;
; DisplayModeInfo struct is used by VMD to get information regarding
; the current display mode from Cursor Drawing VxD.
;

DISPLAYMODEINFO 		STRUC

wSize				dw	?
wFlags				dw	?
DMI_Virtual_Cursor_Maximum	db	(SIZE Point_Struc) DUP (?)
XChar				db	?
YChar				db	?
DMI_Cell_Dimensions		db	(SIZE Point_Struc) DUP (?)
cBitsPerPixel			db	?
wPageSize			dw	?
MaxBlocks			db	?
XCursorMax			db	?
YCursorMax			db	?
bMaxTrail			db	?
cDrawPages			db	?

DISPLAYMODEINFO 		ENDS

;
; Bits of wFlags
;
MI_GRAPHICSCURSOR		EQU	0000000000000001b
MI_COLORCURSOR			EQU	0000000000000010b
MI_NOCURSOR			EQU	0000000000000100b

;
; CursorProcInfo is used by Cursor drawing VxD to register a procedure
; which will be called by VMD to perform certain functions.
;

CursorProcInfo Struc
CursorProcAddress	dd	?
NextCursorProcInfo	dd	?
Video_Mode_Low		dw	?
Video_Mode_High		dw	?
Priority		db	?
Flags			dw	?
CursorProcInfo Ends

;
; Values for flags
;
DC_SupportsOverride	equ	0000000000000001b

;
; TextCursor is used by VMD to inform the cursor drawing VxD about the
; the TEXT cursor set by an app.
;
; TextCursor is represented as follows:
;

TEXTCURSOR	Struc
cbSize			dw	?
dwOffset32Blocks	dd	?
bFlags			db	?
cBlocks			db	?
XORMask			dw	?
ANDMask			dw	?
TEXTCURSOR	Ends

;
; Values for bFlags
;

BF_HARDWARE	EQU	00000001b

;
; GraphicsCursor is used by VMD to inform the cursor drawing VxD about
; the GRAPHICS cursor set by an app.
;
; GraphicsCursor is represented as follows:
;

GRAPHICSCURSOR	Struc
cbSize		dw	?
dwOffset32Masks	dd	?
wWidth		dw	?
wHeight		dw	?
wHotX		dw	?
wHotY		dw	?
wType		dw	?		; monochrome or not
GRAPHICSCURSOR	Ends

;
; Struct for int 10 notifications. This is used by a cursor drawing VxD
; to tell VMD that it should be notified if a certain INT 10 Function
; call is made by an app.
;

Int10Notify Struc
FunctionNumber	db	0
NextNotifyStruc	dd	0
Int10Notify Ends

;
; Ordinals of the functions supported by the Cursor VxD
;

SETTEXTCURSOR		equ	0
SETGRAPHICSCURSOR	equ	1
MOVECURSOR		equ	2
REMOVECURSOR		equ	3
SETDISPLAYMODE		equ	4
SETDISPLAYPAGE		equ	5
SAVERESTORESTATE	equ	6
GETDISPLAYMODEINFO	equ	7
QUERYDISPLAYMODE	equ	8
SETCONDITIONALREGION	equ	9
QUERYMEMORYUNDERCURSOR	equ	10
SETCURSORTRAIL		equ	11
RESETCURSORDRIVER	equ	12

DEFAULT_AND_MASK	EQU	77FFh
DEFAULT_XOR_MASK	EQU	7700h

TEXT_SMALL_BLOCK_SIZE	EQU	1

TEXT_BYTES_PER_BLOCK	EQU	6	;The maximum number of bytes per block
					; in a text cursor.

TEXT_SMALL_MASK_SIZE	EQU	TEXT_SMALL_BLOCK_SIZE * TEXT_BYTES_PER_BLOCK
					;The number of bytes of block data
					; a small text cursor needs.

GR_SMALL_WIDTH		EQU	16	;The width (in pixels) of a small
					; graphics cursor.
GR_SMALL_HEIGHT		EQU	16	;The height (in pixels) of a small
					; graphics cursor.

GR_SMALL_MASK_SIZE	EQU	(GR_SMALL_WIDTH * GR_SMALL_HEIGHT / 8)
					;The size of a mask in a small
					; graphics cursor.

BlockData	MACRO	xOff:REQ, yOff:REQ, xorMask, andMask

	BYTE	xOff, yOff		;Define the offset.

	IFNB	<xorMask>
		WORD	xorMask, andMask
	ENDIF

	ENDM


SMALLTEXTCURSOR		STRUC		;A small text cursor.
tc	TEXTCURSOR	<>
Blocks	BYTE		TEXT_SMALL_MASK_SIZE DUP (?)
SMALLTEXTCURSOR		ENDS

SMALLGRAPHICSCURSOR	STRUC		;A small monochrome graphics cursor.
gc	GRAPHICSCURSOR	<>
Masks	BYTE		GR_SMALL_MASK_SIZE*2 DUP (?)
SMALLGRAPHICSCURSOR	ENDS

SMALLCURSOR	UNION			;A small cursor, either text or
smgc	SMALLGRAPHICSCURSOR	<>
smtc	SMALLTEXTCURSOR		<>
SMALLCURSOR	ENDS

