;-----------------------------------------------------------------------------
; VESADEFS.INC - C-header for general VESA things
;
; Copyright (C) 2004 Dege
;
; This library is free software; you can redistribute it and/or
; modify it under the terms of the GNU Lesser General Public
; License as published by the Free Software Foundation; either
; version 2.1 of the License, or (at your option) any later version.
;
; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; Lesser General Public License for more details.
;
; You should have received a copy of the GNU Lesser General Public
; License along with this library; if not, write to the Free Software
; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
;-----------------------------------------------------------------------------


;------------------------------------------------------------------------------------------
; dgVoodoo: VesaDefs.h
;			�ltal�nos dolgok a VESA interf�szhez
;------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------
;................................. Sz�ks�ges strukt�r�k  ..................................


; VBE Inform�ci�
VBEInfoBlock		STRUC

vbeSignature		DD	?
vbeVersion			DW	?
oemStringPtr		DD	?
capabilities		DD	?
videoModePtr		DD	?
totalMemory			DW	?

oemSoftwareRev		DW	?
oemVendorNamePtr	DD	?
oemProductNamePtr	DD	?
oemProductRevPtr	DD	?
reserved			DB	222 DUP(?)

oemData				DB	256 DUP(?)

VBEInfoBlock		ENDS


; VESA-m�dot le�r� strukt�ra
ModeInfoBlock		STRUC

modeAttributes		DW	?
winAAttributes		DB	?
winBAttributes		DB	?
winGranularity		DW	?
winSize				DW	?
winASegment			DW	?
winBSegment			DW	?
winFuncPtr			DD	?
bytesPerScanLineVESA DW	?

xResolution			DW	?
yResolution			DW	?
xCharSize			DB	?
yCharSize			DB	?
numberOfPlanes		DB	?
bitsPerPixel		DB	?
numberOfBanks		DB	?
memoryModel			DB	?
bankSize			DB	?
numberOfImagePages	DB	?
mIReserved			DB	?

redMaskSize			DB	?
redFieldPosition	DB	?
greenMaskSize		DB	?
greenFieldPosition	DB	?
blueMaskSize		DB	?
blueFieldPosition	DB	?
rsvdMaskSize		DB	?
rsvdFieldPosition	DB	?
directColorModeInfo	DB	?

physBasePtr			DD	?
offScreenMemOffset	DD	?
offScreenMemSize	DW	?
mIReserved2			DB	206 DUP(?)

ModeInfoBlock		ENDS


; A bels�leg haszn�lt, �llapot elment�s�re szolg�l� strukt�ra
StateBuffer			STRUC

paletteVesa			DB	256 DUP(?)
bytesPerScanlineState	DD	?
displayOffsetState	DD	?
vgaRAMDACSizeState	DB	?

StateBuffer			ENDS