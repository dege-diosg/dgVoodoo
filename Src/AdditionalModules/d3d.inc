;-----------------------------------------------------------------------------
; D3D.INC - Direct3D7 include for ASM
;
; Copyright (C) 2001 Dege
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


;------------------------------------------------------------------------------
;Flags for Enumerate functions

;Stop the enumeration
D3DENUMRET_CANCEL       EQU     0

;Continue the enumeration
D3DENUMRET_OK           EQU     1

;------------------------------------------------------------------------------
;Direct3D7 Renderstate enum
D3DRENDERSTATE_ANTIALIAS        EQU     2       ;Antialiasing mode
D3DRENDERSTATE_TEXTUREPERSPECTIVE EQU   4       ;Perspective correction
D3DRENDERSTATE_ZENABLE          EQU     7       ;Enable z test
D3DRENDERSTATE_FILLMODE         EQU     8       ;Fill mode
D3DRENDERSTATE_SHADEMODE        EQU     9       ;Shade mode
D3DRENDERSTATE_LINEPATTERN      EQU     10      ;Line pattern
D3DRENDERSTATE_ZWRITEENABLE     EQU     14      ;Enable z writes
D3DRENDERSTATE_ALPHATESTENABLE  EQU     15      ;Enable alpha tests
D3DRENDERSTATE_LASTPIXEL        EQU     16      ;Draw last pixel in a line
D3DRENDERSTATE_SRCBLEND         EQU     19      ;Blend factor for source
D3DRENDERSTATE_DESTBLEND        EQU     20      ;Blend factor for destination
D3DRENDERSTATE_CULLMODE         EQU     22      ;Back-face culling mode
D3DRENDERSTATE_ZFUNC            EQU     23      ;Z-comparison function
D3DRENDERSTATE_ALPHAREF         EQU     24      ;Reference alpha value
D3DRENDERSTATE_ALPHAFUNC        EQU     25      ;Alpha-comparison function
D3DRENDERSTATE_DITHERENABLE     EQU     26      ;Enable dithering
D3DRENDERSTATE_ALPHABLENDENABLE EQU     27      ;Enable alpha blending
D3DRENDERSTATE_FOGENABLE        EQU     28      ;Enable fog
D3DRENDERSTATE_SPECULARENABLE   EQU     29      ;Enable specular highlights
D3DRENDERSTATE_ZVISIBLE         EQU     30      ;Enable z-checking
D3DRENDERSTATE_STIPPLEDALPHA    EQU     33      ;Enable stippled alpha
D3DRENDERSTATE_FOGCOLOR         EQU     34      ;Fog color
D3DRENDERSTATE_FOGTABLEMODE     EQU     35      ;Fog mode
D3DRENDERSTATE_FOGTABLESTART    EQU     36      ;Fog table start (same as D3DRENDERSTATE_FOGSTART)
D3DRENDERSTATE_FOGTABLEEND      EQU     37      ;Fog table end (same as D3DRENDERSTATE_FOGEND)
D3DRENDERSTATE_FOGTABLEDENSITY  EQU     38      ;Fog table density (same as D3DRENDERSTATE_FOGDENSITY)
D3DRENDERSTATE_FOGSTART         EQU     36      ;Fog start (for both vertex and pixel fog)
D3DRENDERSTATE_FOGEND           EQU     37      ;Fog end (for both vertex and pixel fog)
D3DRENDERSTATE_FOGDENSITY       EQU     38      ;Fog density (for both vertex and pixel fog)
D3DRENDERSTATE_EDGEANTIALIAS    EQU     40      ;Antialias edges
D3DRENDERSTATE_COLORKEYENABLE   EQU     41      ;Enable color-key transparency
D3DRENDERSTATE_ZBIAS            EQU     47      ;Z-bias
D3DRENDERSTATE_RANGEFOGENABLE   EQU     48      ;Enables range-based fog
D3DRENDERSTATE_STENCILENABLE    EQU     52      ;Enable or disable stenciling
D3DRENDERSTATE_STENCILFAIL      EQU     53      ;Stencil operation
D3DRENDERSTATE_STENCILZFAIL     EQU     54      ;Stencil operation
D3DRENDERSTATE_STENCILPASS      EQU     55      ;Stencil operation
D3DRENDERSTATE_STENCILFUNC      EQU     56      ;Stencil comparison function
D3DRENDERSTATE_STENCILREF       EQU     57      ;Reference value for stencil test
D3DRENDERSTATE_STENCILMASK      EQU     58      ;Mask value used in stencil test
D3DRENDERSTATE_STENCILWRITEMASK EQU     59      ;Stencil buffer write mask
D3DRENDERSTATE_TEXTUREFACTOR    EQU     60      ;Texture factor
D3DRENDERSTATE_WRAP0            EQU     128     ;Wrap flags for 1st texture coord set
                                                ;Wrap render states 1 through 6 omitted here.
D3DRENDERSTATE_WRAP7            EQU     135     ;Wrap flags for last texture coord set
D3DRENDERSTATE_CLIPPING         EQU     136     ;Enable or disable primitive clipping
D3DRENDERSTATE_LIGHTING         EQU     137     ;Enable or disable lighting
D3DRENDERSTATE_EXTENTS          EQU     138     ;Enable or disable updating screen extents
D3DRENDERSTATE_AMBIENT          EQU     139     ;Ambient color for scene
D3DRENDERSTATE_FOGVERTEXMODE    EQU     140     ;Fog mode for vertex fog
D3DRENDERSTATE_COLORVERTEX      EQU     141     ;Enable or disable per-vertex color
D3DRENDERSTATE_LOCALVIEWER      EQU     142     ;Enable or disable perspective specular highlights
D3DRENDERSTATE_NORMALIZENORMALS EQU     143     ;Enable automatic normalization of vertex normals
D3DRENDERSTATE_COLORKEYBLENDENABLE EQU  144     ;Enable or disable alpha-blended color keying
D3DRENDERSTATE_DIFFUSEMATERIALSOURCE EQU 145    ;Location for per-vertex diffuse color
D3DRENDERSTATE_SPECULARMATERIALSOURCE EQU 146   ;Location for per-vertex specular color
D3DRENDERSTATE_AMBIENTMATERIALSOURCE EQU 147    ;Location for per-vertex ambient color
D3DRENDERSTATE_EMISSIVEMATERIALSOURCE EQU 148   ;Location for per-vertex emissive color
D3DRENDERSTATE_VERTEXBLEND      EQU     151     ;Multi-matrix vertex blending mode
D3DRENDERSTATE_CLIPPLANEENABLE  EQU     152     ;Enable one or more user-defined clipping planes
D3DRENDERSTATE_FORCE_DWORD      EQU     7fffffffh

;------------------------------------------------------------------------------
;interf‚szek defini l sa
;------------------------------------------------------------------------------

IDirect3D7      STRUCT

                DD      ?
                DD      ?
                DD      ?
EnumDevices7    DD      ?
CreateDevice7   DD      ?
CreateVertexBuffer7 DD  ?
EnumZBufferFormats7 DD  ?
EvictManagedTextures7 DD ?

IDirect3D7      ENDS

IDirect3DDevice7        STRUC

                DD      ?
                DD      ?
                DD      ?
GetCaps3D7      DD      ?
EnumTextureFormats7 DD  ?
BeginScene7     DD      ?
EndScene7       DD      ?
GetDirect3D7    DD      ?
SetRenderTarget7 DD     ?
GetRenderTarget7 DD     ?
Clear7          DD      ?
SetTransform7   DD      ?
GetTransform7   DD      ?
SetViewport7    DD      ?
MultiplyTransform DD    ?
GetViewport7    DD      ?
SetMaterial7    DD      ?
GetMateria7     DD      ?
SetLight7       DD      ?
GetLight7       DD      ?
SetRenderState7 DD      ?
GetRenderState7 DD      ?
BeginStateBlock7 DD     ?
EndStateBlock7  DD      ?
PreLoad7        DD      ?
DrawPrimitive7  DD      ?
DrawIndexedPrimitive7 DD ?
SetClipStatus7  DD      ?
GetClipStatus7  DD      ?
DrawPrimitiveStrided7 DD ?
DrawIndexedPrimitiveStrided7 DD ?
DrawPrimitiveVB7 DD     ?
DrawIndexedPrimitiveVB7 DD ?
ComputeSphereVisibility DD ?
GetTexture7     DD      ?
SetTexture7     DD      ?
GetTextureStageState7 DD ?
SetTextureStageState7 DD ?
ValidateDevice7 DD      ?
ApplyStateBlock7 DD     ?
CaptureStateBlock7 DD   ?
DeleteStateBlock7 DD    ?
CreateStateBlock7 DD    ?
Load7           DD      ?
LightEnable7    DD      ?
GetLightEnable7 DD      ?
SetClipPlane7   DD      ?
GetClipPlane7   DD      ?
GetInfo7        DD      ?

IDirect3DDevice7        ENDS

