;----------------------------------------------------------------------------
;Macro include

;IFNDEF  DGMACROS

DGMACROS        EQU     1

LARGE			EQU 	<DWORD PTR>
NULL			EQU		0

M_TRUE          EQU     1
M_FALSE         EQU     0

VISUALC         EQU     M_TRUE ;M_FALSE          ;Visual C function naming convention

;-------------- Defs for module type

ASSEMBLY_INCLUDE        EQU     1h      ;Adott forr s assembly include-k‚nt
STANDALONE_OMF          EQU     2h      ;Adott forr s beszerkeszthet“ objectk‚nt
NEEDEDONLY_OMF          EQU     3h      ;Adott forr s objectk‚nt

;-------------- Macro for Windows API function calling and naming convention

IF      VISUALC EQ M_TRUE

        W32     MACRO   APIfunc,ParamNum

                IFB     <ParamNum>

                        EXTRN   APIfunc:PROC
                        call    APIfunc

                ELSE
	
                        PARAMBYTES      =     ParamNum*4
			APINAME		CATSTR	<_>,<APIfunc>,<@>,%PARAMBYTES
                        EXTRN   APINAME :PROC
                        call    APINAME
                ENDIF

        ENDM

ELSE

        W32     MACRO   APIfunc,ParamNum

                EXTRN   APIfunc:PROC
                call    APIfunc
        ENDM

ENDIF

EXT     MACRO   APIfunc

        EXTRN   APIfunc:PROC

ENDM

;ENDIF
