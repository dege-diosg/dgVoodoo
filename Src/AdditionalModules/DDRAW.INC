;-----------------------------------------------------------------------------
; DDRAW.INC - DirectDraw7 include for ASM
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

;----------------------------------------------------------------------------
; DIRECTDRAW SETCOOPERATIVELEVEL FLAGS

;Exclusive mode owner will be responsible for the entire primary surface.
;GDI can be ignored. used with DD

DDSCL_FULLSCREEN        EQU     00000001h

;allow CTRL_ALT_DEL to work while in fullscreen exclusive mode
DDSCL_ALLOWREBOOT       EQU     00000002h

;prevents DDRAW from modifying the application window.
;prevents DDRAW from minimize/restore the application window on activation.
DDSCL_NOWINDOWCHANGES   EQU     00000004h

;app wants to work as a regular Windows application
DDSCL_NORMAL            EQU     00000008h

;app wants exclusive access
DDSCL_EXCLUSIVE         EQU     00000010h


;app can deal with non-windows display modes
DDSCL_ALLOWMODEX        EQU     00000040h

;this window will receive the focus messages
DDSCL_SETFOCUSWINDOW    EQU     00000080h

;this window is associated with the DDRAW object and will
;cover the screen in fullscreen mode
DDSCL_SETDEVICEWINDOW   EQU     00000100h

;app wants DDRAW to create a window to be associated with the
;DDRAW object
DDSCL_CREATEDEVICEWINDOW EQU    00000200h

;App explicitly asks DDRAW/D3D to be multithread safe. This makes D3D
;take the global crtisec more frequently.
DDSCL_MULTITHREADED     EQU     00000400h

;App specifies that it would like to keep the FPU set up for optimal Direct3D
;performance (single precision and exceptions disabled) so Direct3D
;does not need to explicitly set the FPU each time. This is assumed by
;default in DirectX 7. See also DDSCL_FPUPRESERVE
DDSCL_FPUSETUP          EQU     00000800h

;App specifies that it needs either double precision FPU or FPU exceptions
;enabled. This makes Direct3D explicitly set the FPU state eah time it is
;called. Setting the flag will reduce Direct3D performance. The flag is
;assumed by default in DirectX 6 and earlier. See also DDSCL_FPUSETUP
DDSCL_FPUPRESERVE       EQU     00001000h
;----------------------------------------------------------------------------
; DIRECTDRAW ENUMDISPLAYMODES FLAGS

;Enumerate Modes with different refresh rates.  EnumDisplayModes guarantees
;that a particular mode will be enumerated only once.  This flag specifies whether
;the refresh rate is taken into account when determining if a mode is unique.

DDEDM_REFRESHRATES      EQU     00000001h

;Enumerate VGA modes. Specify this flag if you wish to enumerate supported VGA
;modes such as mode 0x13 in addition to the usual ModeX modes (which are always
;enumerated if the application has previously called SetCooperativeLevel with the
;DDSCL_ALLOWMODEX flag set).

DDEDM_STANDARDVGAMODES  EQU     00000002h

;----------------------------------------------------------------------------

;ddsCaps field is valid.
DDSD_CAPS               EQU     00000001h       ;default

;dwHeight field is valid.
DDSD_HEIGHT             EQU     00000002h

;dwWidth field is valid.
DDSD_WIDTH              EQU     00000004h

;lPitch is valid.
DDSD_PITCH              EQU     00000008h

;dwBackBufferCount is valid.
DDSD_BACKBUFFERCOUNT    EQU     00000020h

;dwZBufferBitDepth is valid.  (shouldnt be used in DDSURFACEDESC2)
DDSD_ZBUFFERBITDEPTH    EQU     00000040h

;dwAlphaBitDepth is valid.
DDSD_ALPHABITDEPTH      EQU     00000080h

;lpSurface is valid.
DDSD_LPSURFACE          EQU     00000800h

;ddpfPixelFormat is valid.
DDSD_PIXELFORMAT        EQU     00001000h

;ddckCKDestOverlay is valid.
DDSD_CKDESTOVERLAY      EQU     00002000h

;ddckCKDestBlt is valid.
DDSD_CKDESTBLT          EQU     00004000h

;ddckCKSrcOverlay is valid.
DDSD_CKSRCOVERLAY       EQU     00008000h

;ddckCKSrcBlt is valid.
DDSD_CKSRCBLT           EQU     00010000h

;dwMipMapCount is valid.
DDSD_MIPMAPCOUNT        EQU     00020000h

;dwRefreshRate is valid
DDSD_REFRESHRATE        EQU     00040000h

;dwLinearSize is valid
DDSD_LINEARSIZE         EQU     00080000h

;dwTextureStage is valid
DDSD_TEXTURESTAGE       EQU     00100000h

;dwFVF is valid
DDSD_FVF                EQU     00200000h

;dwSrcVBHandle is valid
DDSD_SRCVBHANDLE        EQU     00400000h

;All input fields are valid.
DDSD_ALL                EQU     007ff9eeh

;----------------------------------------------------------------------------
; DIRECTDRAWSURFACE CAPABILITY FLAGS

;This bit is reserved. It should not be specified.
DDSCAPS_RESERVED1       EQU     00000001h

;Indicates that this surface contains alpha-only information.
;(To determine if a surface is RGBA/YUVA, the pixel format must be
;interrogated.)
DDSCAPS_ALPHA           EQU     00000002h

;Indicates that this surface is a backbuffer.  It is generally
;set by CreateSurface when the DDSCAPS_FLIP capability bit is set.
;It indicates that this surface is THE back buffer of a surface
;flipping structure.  DirectDraw supports N surfaces in a
;surface flipping structure.  Only the surface that immediately
;precedeces the DDSCAPS_FRONTBUFFER has this capability bit set.
;The other surfaces are identified as back buffers by the presence
;of the DDSCAPS_FLIP capability, their attachment order, and the
;absence of the DDSCAPS_FRONTBUFFER and DDSCAPS_BACKBUFFER
;capabilities.  The bit is sent to CreateSurface when a standalone
;back buffer is being created.  This surface could be attached to
;a front buffer and/or back buffers to form a flipping surface
;structure after the CreateSurface call.  See AddAttachments for
;a detailed description of the behaviors in this case.
DDSCAPS_BACKBUFFER      EQU     00000004h

;Indicates a complex surface structure is being described.  A
;complex surface structure results in the creation of more than
;one surface.  The additional surfaces are attached to the root
;surface.  The complex structure can only be destroyed by
;destroying the root.
DDSCAPS_COMPLEX         EQU     00000008h

;Indicates that this surface is a part of a surface flipping structure.
;When it is passed to CreateSurface the DDSCAPS_FRONTBUFFER and
;DDSCAP_BACKBUFFER bits are not set.  They are set by CreateSurface
;on the resulting creations.  The dwBackBufferCount field in the
;DDSURFACEDESC structure must be set to at least 1 in order for
;the CreateSurface call to succeed.  The DDSCAPS_COMPLEX capability
;must always be set with creating multiple surfaces through CreateSurface.
DDSCAPS_FLIP            EQU     00000010h

;Indicates that this surface is THE front buffer of a surface flipping
;structure.  It is generally set by CreateSurface when the DDSCAPS_FLIP
;capability bit is set.
;If this capability is sent to CreateSurface then a standalonw front buffer
;is created.  This surface will not have the DDSCAPS_FLIP capability.
;It can be attached to other back buffers to form a flipping structure.
;See AddAttachments for a detailed description of the behaviors in this
;case.
DDSCAPS_FRONTBUFFER     EQU     00000020h

;Indicates that this surface is any offscreen surface that is not an overlay,
;texture, zbuffer, front buffer, back buffer, or alpha surface.  It is used
;to identify plain vanilla surfaces.
DDSCAPS_OFFSCREENPLAIN  EQU     00000040h

;Indicates that this surface is an overlay.  It may or may not be directly visible
;depending on whether or not it is currently being overlayed onto the primary
;surface.  DDSCAPS_VISIBLE can be used to determine whether or not it is being
;overlayed at the moment.
DDSCAPS_OVERLAY         EQU     00000080h

;Indicates that unique DirectDrawPalette objects can be created and
;attached to this surface.
DDSCAPS_PALETTE         EQU     00000100h

;Indicates that this surface is the primary surface.  The primary
;surface represents what the user is seeing at the moment.
DDSCAPS_PRIMARYSURFACE  EQU     00000200h


;This flag used to be DDSCAPS_PRIMARYSURFACELEFT, which is now
;obsolete.
DDSCAPS_RESERVED3       EQU     00000400h

;Indicates that this surface memory was allocated in system memory
DDSCAPS_SYSTEMMEMORY    EQU     00000800h

;Indicates that this surface can be used as a 3D texture.  It does not
;indicate whether or not the surface is being used for that purpose.
DDSCAPS_TEXTURE         EQU     00001000h

;Indicates that a surface may be a destination for 3D rendering.  This
;bit must be set in order to query for a Direct3D Device Interface
;from this surface.
DDSCAPS_3DDEVICE        EQU     00002000h

;Indicates that this surface exists in video memory.
DDSCAPS_VIDEOMEMORY     EQU     00004000h

;Indicates that changes made to this surface are immediately visible.
;It is always set for the primary surface and is set for overlays while
;they are being overlayed and texture maps while they are being textured.
DDSCAPS_VISIBLE         EQU     00008000h

;Indicates that only writes are permitted to the surface.  Read accesses
;from the surface may or may not generate a protection fault, but the
;results of a read from this surface will not be meaningful.  READ ONLY.
DDSCAPS_WRITEONLY       EQU     00010000h

;Indicates that this surface is a z buffer. A z buffer does not contain
;displayable information.  Instead it contains bit depth information that is
;used to determine which pixels are visible and which are obscured.
DDSCAPS_ZBUFFER         EQU     00020000h

;Indicates surface will have a DC associated long term
DDSCAPS_OWNDC           EQU     00040000h

;Indicates surface should be able to receive live video
DDSCAPS_LIVEVIDEO       EQU     00080000h

;Indicates surface should be able to have a stream decompressed
;to it by the hardware.
DDSCAPS_HWCODEC         EQU     00100000h

;Surface is a ModeX surface.
DDSCAPS_MODEX           EQU     00200000h

;Indicates surface is one level of a mip-map. This surface will
;be attached to other DDSCAPS_MIPMAP surfaces to form the mip-map.
;This can be done explicitly, by creating a number of surfaces and
;attaching them with AddAttachedSurface or by implicitly by CreateSurface.
;If this bit is set then DDSCAPS_TEXTURE must also be set.
DDSCAPS_MIPMAP          EQU     00400000h

;This bit is reserved. It should not be specified.
DDSCAPS_RESERVED2       EQU     00800000h


;Indicates that memory for the surface is not allocated until the surface
;is loaded (via the Direct3D texture Load() function).
DDSCAPS_ALLOCONLOAD     EQU     04000000h

;Indicates that the surface will recieve data from a video port.
DDSCAPS_VIDEOPORT       EQU     08000000h

;Indicates that a video memory surface is resident in true, local video
;memory rather than non-local video memory. If this flag is specified then
;so must DDSCAPS_VIDEOMEMORY. This flag is mutually exclusive with
;DDSCAPS_NONLOCALVIDMEM.
DDSCAPS_LOCALVIDMEM     EQU     10000000h

;Indicates that a video memory surface is resident in non-local video
;memory rather than true, local video memory. If this flag is specified
;then so must DDSCAPS_VIDEOMEMORY. This flag is mutually exclusive with
;DDSCAPS_LOCALVIDMEM.
DDSCAPS_NONLOCALVIDMEM  EQU     20000000h

;Indicates that this surface is a standard VGA mode surface, and not a
;ModeX surface. (This flag will never be set in combination with the
;DDSCAPS_MODEX flag).
DDSCAPS_STANDARDVGAMODE EQU     40000000h

;Indicates that this surface will be an optimized surface. This flag is
;currently only valid in conjunction with the DDSCAPS_TEXTURE flag. The surface
;will be created without any underlying video memory until loaded.
DDSCAPS_OPTIMIZED       EQU     80000000h


;Indicates that this surface will receive data from a video port using
;the de-interlacing hardware.  This allows the driver to allocate memory
;for any extra buffers that may be required.  The DDSCAPS_VIDEOPORT and
;DDSCAPS_OVERLAY flags must also be set.
DDSCAPS2_HARDWAREDEINTERLACE    EQU     00000002h

;Indicates to the driver that this surface will be locked very frequently
;(for procedural textures, dynamic lightmaps, etc). Surfaces with this cap
;set must also have DDSCAPS_TEXTURE. This cap cannot be used with
;DDSCAPS2_HINTSTATIC and DDSCAPS2_OPAQUE.
DDSCAPS2_HINTDYNAMIC     EQU    00000004h

;Indicates to the driver that this surface can be re-ordered/retiled on
;load. This operation will not change the size of the texture. It is
;relatively fast and symmetrical, since the application may lock these
;bits (although it will take a performance hit when doing so). Surfaces
;with this cap set must also have DDSCAPS_TEXTURE. This cap cannot be
;used with DDSCAPS2_HINTDYNAMIC and DDSCAPS2_OPAQUE.
DDSCAPS2_HINTSTATIC      EQU    00000008h

;Indicates that the client would like this texture surface to be managed by the
;DirectDraw/Direct3D runtime. Surfaces with this cap set must also have
;DDSCAPS_TEXTURE set.
DDSCAPS2_TEXTUREMANAGE   EQU    00000010h

;These bits are reserved for internal use
DDSCAPS2_RESERVED1       EQU    00000020h
DDSCAPS2_RESERVED2       EQU    00000040h

;Indicates to the driver that this surface will never be locked again.
;The driver is free to optimize this surface via retiling and actual compression.
;All calls to Lock() or Blts from this surface will fail. Surfaces with this
;cap set must also have DDSCAPS_TEXTURE. This cap cannot be used with
;DDSCAPS2_HINTDYNAMIC and DDSCAPS2_HINTSTATIC.

DDSCAPS2_OPAQUE          EQU    00000080h

;Applications should set this bit at CreateSurface time to indicate that they
;intend to use antialiasing. Only valid if DDSCAPS_3DDEVICE is also set.
DDSCAPS2_HINTANTIALIASING EQU   00000100h

;This flag is used at CreateSurface time to indicate that this set of
;surfaces is a cubic environment map
DDSCAPS2_CUBEMAP          EQU   00000200h

;These flags preform two functions:
; - At CreateSurface time, they define which of the six cube faces are
;   required by the application.
; - After creation, each face in the cubemap will have exactly one of these
;   bits set.
DDSCAPS2_CUBEMAP_POSITIVEX EQU  00000400h
DDSCAPS2_CUBEMAP_NEGATIVEX EQU  00000800h
DDSCAPS2_CUBEMAP_POSITIVEY EQU  00001000h
DDSCAPS2_CUBEMAP_NEGATIVEY EQU  00002000h
DDSCAPS2_CUBEMAP_POSITIVEZ EQU  00004000h
DDSCAPS2_CUBEMAP_NEGATIVEZ EQU  00008000h

;This flag is an additional flag which is present on mipmap sublevels from DX7 onwards
;It enables easier use of GetAttachedSurface rather than EnumAttachedSurfaces for surface
;constructs such as Cube Maps, wherein there are more than one mipmap surface attached
;to the root surface.
;This caps bit is ignored by CreateSurface
DDSCAPS2_MIPMAPSUBLEVEL    EQU  00010000h

;This flag indicates that the texture should be managed by D3D only
DDSCAPS2_D3DTEXTUREMANAGE  EQU  00020000h

;This flag indicates that the managed surface can be safely lost
DDSCAPS2_DONOTPERSIST      EQU  00040000h

;indicates that this surface is part of a stereo flipping chain
DDSCAPS2_STEREOSURFACELEFT EQU  00080000h
;----------------------------------------------------------------------------
;DIRECTDRAWPALETTE CAPABILITIES

;Index is 4 bits.  There are sixteen color entries in the palette table.
DDPCAPS_4BIT               EQU  00000001h

;Index is onto a 8 bit color index.  This field is only valid with the
;DDPCAPS_1BIT, DDPCAPS_2BIT or DDPCAPS_4BIT capability and the target
;surface is in 8bpp. Each color entry is one byte long and is an index
;into destination surface's 8bpp palette.
DDPCAPS_8BITENTRIES        EQU  00000002h

;Index is 8 bits.  There are 256 color entries in the palette table.
DDPCAPS_8BIT               EQU  00000004h

;Indicates that this DIRECTDRAWPALETTE should use the palette color array
;passed into the lpDDColorArray parameter to initialize the DIRECTDRAWPALETTE
;object.
;This flag is obsolete. DirectDraw always initializes the color array from
;the lpDDColorArray parameter. The definition remains for source-level
;compatibility.
DDPCAPS_INITIALIZE         EQU  00000000h

;This palette is the one attached to the primary surface.  Changing this
;table has immediate effect on the display unless DDPSETPAL_VSYNC is specified
;and supported.
DDPCAPS_PRIMARYSURFACE     EQU  00000010h

;This palette is the one attached to the primary surface left.  Changing
;this table has immediate effect on the display for the left eye unless
;DDPSETPAL_VSYNC is specified and supported.
DDPCAPS_PRIMARYSURFACELEFT EQU  00000020h

;This palette can have all 256 entries defined
DDPCAPS_ALLOW256           EQU  00000040h

;This palette can have modifications to it synced with the monitors
;refresh rate.
;DDPCAPS_VSYNC             EQU  00000080h

;Index is 1 bit.  There are two color entries in the palette table.
DDPCAPS_1BIT               EQU  00000100h

;Index is 2 bit.  There are four color entries in the palette table.
DDPCAPS_2BIT               EQU  00000200h

;The peFlags member of PALETTEENTRY denotes an 8 bit alpha value
DDPCAPS_ALPHA              EQU  00000400h

;----------------------------------------------------------------------------
;DIRECTDRAW WAITFORVERTICALBLANK FLAGS

;return when the vertical blank interval begins
DDWAITVB_BLOCKBEGIN        EQU  00000001h

;set up an event to trigger when the vertical blank begins
DDWAITVB_BLOCKBEGINEVENT   EQU  00000002h

;return when the vertical blank interval ends and display begins
DDWAITVB_BLOCKEND          EQU  00000004h

;----------------------------------------------------------------------------
;DIRECTDRAWSURFACE LOCK FLAGS

;The default.  Set to indicate that Lock should return a valid memory pointer
;to the top of the specified rectangle.  If no rectangle is specified then a
;pointer to the top of the surface is returned.
DDLOCK_SURFACEMEMORYPTR    EQU  00000000h     ; default

;Set to indicate that Lock should wait until it can obtain a valid memory
;pointer before returning.  If this bit is set, Lock will never return
;DDERR_WASSTILLDRAWING.
DDLOCK_WAIT                EQU  00000001h

;Set if an event handle is being passed to Lock.  Lock will trigger the event
;when it can return the surface memory pointer requested.
DDLOCK_EVENT               EQU  00000002h

;Indicates that the surface being locked will only be read from.
DDLOCK_READONLY            EQU  00000010h

;Indicates that the surface being locked will only be written to
DDLOCK_WRITEONLY           EQU  00000020h

;Indicates that a system wide lock should not be taken when this surface
;is locked. This has several advantages (cursor responsiveness, ability
;to call more Windows functions, easier debugging) when locking video
;memory surfaces. However, an application specifying this flag must
;comply with a number of conditions documented in the help file.
;Furthermore, this flag cannot be specified when locking the primary.
DDLOCK_NOSYSLOCK           EQU  00000800h

;Used only with Direct3D Vertex Buffer Locks. Indicates that no vertices
;that were referred to in Draw*PrimtiveVB calls since the start of the
;frame (or the last lock without this flag) will be modified during the
;lock. This can be useful when one is only appending data to the vertex
;buffer
DDLOCK_NOOVERWRITE         EQU  00001000h

;Indicates that no assumptions will be made about the contents of the 
;surface or vertex buffer during this lock.
;This enables two things:
; -    Direct3D or the driver may provide an alternative memory
;      area as the vertex buffer. This is useful when one plans to clear the
;      contents of the vertex buffer and fill in new data.
; -    Drivers sometimes store surface data in a re-ordered format.
;      When the application locks the surface, the driver is forced to un-re-order
;      the surface data before allowing the application to see the surface contents.
;      This flag is a hint to the driver that it can skip the un-re-ordering process
;      since the application plans to overwrite every single pixel in the surface
;      or locked rectangle (and so erase any un-re-ordered pixels anyway).
;      Applications should always set this flag when they intend to overwrite the entire
;      surface or locked rectangle.

DDLOCK_DISCARDCONTENTS          EQU     00002000h

DDLOCK_OKTOSWAP                 EQU     00002000h

;On IDirectDrawSurface7 and higher interfaces, the default is DDLOCK_WAIT. If you wish
;to override the default and use time when the accelerator is busy (as denoted by
;the DDERR_WASSTILLDRAWING return code) then use DDLOCK_DONOTWAIT.
DDLOCK_DONOTWAIT                EQU     00004000h

;----------------------------------------------------------------------------
;FLIP FLAGS

DDFLIP_WAIT                     EQU     00000001h

;Indicates that the target surface contains the even field of video data.
;This flag is only valid with an overlay surface.
DDFLIP_EVEN                     EQU     00000002h

;Indicates that the target surface contains the odd field of video data.
;This flag is only valid with an overlay surface.
DDFLIP_ODD                      EQU     00000004h

;Causes DirectDraw to perform the physical flip immediately and return
;to the application. Typically, what was the front buffer but is now the back
;buffer will still be visible (depending on timing) until the next vertical
;retrace. Subsequent operations involving the two flipped surfaces will
;not check to see if the physical flip has finished (i.e. will not return
;DDERR_WASSTILLDRAWING for that reason (but may for other reasons)).
;This allows an application to perform Flips at a higher frequency than the
;monitor refresh rate, but may introduce visible artifacts.
;Only effective if DDCAPS2_FLIPNOVSYNC is set. If that bit is not set,
;DDFLIP_NOVSYNC has no effect.
DDFLIP_NOVSYNC                  EQU     00000008h


;Flip Interval Flags. These flags indicate how many vertical retraces to wait between
;each flip. The default is one. DirectDraw will return DDERR_WASSTILLDRAWING for each
;surface involved in the flip until the specified number of vertical retraces has
;ocurred. Only effective if DDCAPS2_FLIPINTERVAL is set. If that bit is not set,
;DDFLIP_INTERVALn has no effect.

;DirectDraw will flip on every other vertical sync
DDFLIP_INTERVAL2                EQU     02000000h


;DirectDraw will flip on every third vertical sync
DDFLIP_INTERVAL3                EQU     03000000h

;DirectDraw will flip on every fourth vertical sync
DDFLIP_INTERVAL4                EQU     04000000h

;DirectDraw will flip and display a main stereo surface
DDFLIP_STEREO                   EQU     00000010h

;On IDirectDrawSurface7 and higher interfaces, the default is DDFLIP_WAIT. If you wish
;to override the default and use time when the accelerator is busy (as denoted by
;the DDERR_WASSTILLDRAWING return code) then use DDFLIP_DONOTWAIT.
DDFLIP_DONOTWAIT                EQU     00000020h

;----------------------------------------------------------------------------
;DIRECTDRAW PIXELFORMAT FLAGS

;The surface has alpha channel information in the pixel format.
DDPF_ALPHAPIXELS                EQU     00000001h

;The pixel format contains alpha only information
DDPF_ALPHA                      EQU     00000002h

;The FourCC code is valid.
DDPF_FOURCC                     EQU     00000004h

;The surface is 4-bit color indexed.
DDPF_PALETTEINDEXED4            EQU     00000008h

;The surface is indexed into a palette which stores indices
;into the destination surface's 8-bit palette.
DDPF_PALETTEINDEXEDTO8          EQU     00000010h

;The surface is 8-bit color indexed.
DDPF_PALETTEINDEXED8            EQU     00000020h

;The RGB data in the pixel format structure is valid.
DDPF_RGB                        EQU     00000040h

;The surface will accept pixel data in the format specified
;and compress it during the write.
DDPF_COMPRESSED                 EQU     00000080h

;The surface will accept RGB data and translate it during
;the write to YUV data.  The format of the data to be written
;will be contained in the pixel format structure.  The DDPF_RGB
;flag will be set.
DDPF_RGBTOYUV                   EQU     00000100h

;pixel format is YUV - YUV data in pixel format struct is valid
DDPF_YUV                        EQU     00000200h

;pixel format is a z buffer only surface
DDPF_ZBUFFER                    EQU     00000400h

;The surface is 1-bit color indexed.
DDPF_PALETTEINDEXED1            EQU     00000800h

;The surface is 2-bit color indexed.
DDPF_PALETTEINDEXED2            EQU     00001000h

;The surface contains Z information in the pixels
DDPF_ZPIXELS                    EQU     00002000h

;The surface contains stencil information along with Z
DDPF_STENCILBUFFER              EQU     00004000h

;Premultiplied alpha format -- the color components have been
;premultiplied by the alpha component.
DDPF_ALPHAPREMULT               EQU     00008000h


;Luminance data in the pixel format is valid.
;Use this flag for luminance-only or luminance+alpha surfaces,
;the bit depth is then ddpf.dwLuminanceBitCount.
DDPF_LUMINANCE                  EQU     00020000h

;Luminance data in the pixel format is valid.
;Use this flag when hanging luminance off bumpmap surfaces,
;the bit mask for the luminance portion of the pixel is then
;ddpf.dwBumpLuminanceBitMask
DDPF_BUMPLUMINANCE              EQU     00040000h

;Bump map dUdV data in the pixel format is valid.
DDPF_BUMPDUDV                   EQU     00080000h

;----------------------------------------------------------------------------
; DIRECTDRAW GETFLIPSTATUS FLAGS

;is it OK to flip now?
DDGFS_CANFLIP                   EQU     00000001h

;is the last flip finished?
DDGFS_ISFLIPDONE                EQU     00000002h

;----------------------------------------------------------------------------
;DIRECTDRAW GETBLTSTATUS FLAGS

;is it OK to blt now?
DDGBS_CANBLT                    EQU     00000001h

;is the blt to the surface finished?
DDGBS_ISBLTDONE                 EQU     00000002h

;------------------------------------------------------------------------------

LPDIRECTDRAW            TYPEDEF DWORD
LPDIRECTDRAWSURFACE     TYPEDEF DWORD

;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;interf‚szek defini l sa
;------------------------------------------------------------------------------

;DirectDraw-f“objektum interf‚sze (leg“sibb DirectDraw verzi¢)

IDirectDraw     STRUCT

QueryInterface          DD      ?
AddRef                  DD      ?
Release                 DD      ?
Compact                 DD      ?
CreateClipper           DD      ?
CreatePalette           DD      ?
CreateSurface           DD      ?
DuplicateSurface        DD      ?
EnumDisplayModes        DD      ?
EnumSurfaces            DD      ?
FlipToGDISurface        DD      ?
GetCaps                 DD      ?
GetDisplayMode          DD      ?
GetFourCCCodes          DD      ?
GetGDISurface           DD      ?
GetMonitorFrequency     DD      ?
GetScanLine             DD      ?
GetVerticalBlankStatus  DD      ?
Initialize              DD      ?
RestoreDisplayMode      DD      ?
SetCooperativeLevel     DD      ?
SetDisplayMode          DD      ?
WaitForVerticalBlank    DD      ?

IDirectDraw             ENDS

;(DirectDraw 7)
IDirectDraw7            STRUCT

QueryInterface7         DD      ?
AddRef7                 DD      ?
Release7                DD      ?
Compact7                DD      ?
CreateClipper7          DD      ?
CreatePalette7          DD      ?
CreateSurface7          DD      ?
DuplicateSurface7       DD      ?
EnumDisplayModes7       DD      ?
EnumSurfaces7           DD      ?
FlipToGDISurface7       DD      ?
GetCaps7                DD      ?
GetDisplayMode7         DD      ?
GetFourCCCodes7         DD      ?
GetGDISurface7          DD      ?
GetMonitorFrequency7    DD      ?
GetScanLine7            DD      ?
GetVerticalBlankStatus7 DD      ?
Initialize7             DD      ?
RestoreDisplayMode7     DD      ?
SetCooperativeLevel7    DD      ?
SetDisplayMode7         DD      ?
WaitForVerticalBlank7   DD      ?
;Added in the v2 interface
GetAvailableVidMem7     DD      ?
;Added in the V4 Interface
GetSurfaceFromDC7       DD      ?
RestoreAllSurfaces7     DD      ?
TestCooperativeLevel7   DD      ?
GetDeviceIdentifier7    DD      ?
StartModeTest7          DD      ?
EvaluateMode7           DD      ?

IDirectDraw7            ENDS

;----------------------------------------------------------------------------
;Pufferobjektum interf‚sze
IDirectDrawSurface      STRUCT

                        DD      3 DUP(?)
;IDirectDrawSurface methods
AddAttachedSurface      DD      ?
AddOverlayDirtyRect     DD      ?
Blt                     DD      ?
BltBatch                DD      ?
BltFast                 DD      ?
DeleteAttachedSurface   DD      ?
EnumAttachedSurfaces    DD      ?
EnumOverlayZOrders      DD      ?
Flip                    DD      ?
GetAttachedSurface      DD      ?
GetBltStatus            DD      ?
GetCapsDS               DD      ?
GetClipper              DD      ?
GetColorKey             DD      ?
GetDC                   DD      ?
GetFlipStatus           DD      ?
GetOverlayPosition      DD      ?
GetPalette              DD      ?
GetPixelFormat          DD      ?
GetSurfaceDesc          DD      ?
InitializeDS            DD      ?
IsLost                  DD      ?
Lock                    DD      ?
ReleaseDC               DD      ?
Restore                 DD      ?
SetClipper              DD      ?
SetColorKey             DD      ?
SetOverlayPosition      DD      ?
SetPalette              DD      ?
Unlock                  DD      ?
UpdateOverlay           DD      ?
UpdateOverlayDisplay    DD      ?
UpdateOverlayZOrder     DD      ?

IDirectDrawSurface      ENDS

IDirectDrawSurface7     STRUC

                        DD      3 DUP(?)
AddAttachedSurface7     DD      ?
AddOverlayDirtyRect7    DD      ?
Blt7                    DD      ?
BltBatch7               DD      ?
BltFast7                DD      ?
DeleteAttachedSurface7  DD      ?
EnumAttachedSurfaces7   DD      ?
EnumOverlayZOrders7     DD      ?
Flip7                   DD      ?
GetAttachedSurface7     DD      ?
GetBltStatus7           DD      ?
GetCapsS7               DD      ?
GetClipper7             DD      ?
GetColorKey7            DD      ?
GetDC7                  DD      ?
GetFlipStatus7          DD      ?
GetOverlayPosition7     DD      ?
GetPalette7             DD      ?
GetPixelFormat7         DD      ?
GetSurfaceDesc7         DD      ?
InitializeS7            DD      ?
IsLost7                 DD      ?
Lock7                   DD      ?
ReleaseDC7              DD      ?
Restore7                DD      ?
SetClipper7             DD      ?
SetColorKey7            DD      ?
SetOverlayPosition7     DD      ?
SetPalette7             DD      ?
Unlock7                 DD      ?
UpdateOverlay7          DD      ?
UpdateOverlayDisplay7   DD      ?
UpdateOverlayZOrder7    DD      ?
;Added in the v2 interface
GetDDInterface7         DD      ?
PageLock7               DD      ?
PageUnlock7             DD      ?
;Added in the v3 interface
SetSurfaceDesc7         DD      ?
;Added in the v4 interface
SetPrivateData7         DD      ?
GetPrivateData7         DD      ?
FreePrivateData7        DD      ?
GetUniquenessValue7     DD      ?
ChangeUniquenessValue7  DD      ?
;Moved Texture7 methods here
SetPriority7            DD      ?
GetPriority7            DD      ?
SetLOD7                 DD      ?
GetLOD7                 DD      ?

IDirectDrawSurface7     ENDS

;------------------------------------------------------------------------------
;Palettaobjektum interf‚sze
IDirectDrawPalette      STRUC

                        DD      3 DUP(?)
GetCapsPAL              DD      ?
GetEntries              DD      ?
InitializePAL           DD      ?
SetEntries              DD      ?
IDirectDrawPalette      ENDS

IDirectDrawClipper      STRUC

                        DD      3 DUP(?)
;QueryInterface          DD      ?
;AddRef                  DD      ?
;Release                 DD      ?
;IDirectDrawClipper methods
GetClipList             DD      ?
GetHWnd                 DD      ?
InitializeClip          DD      ?
IsClipListChanged       DD      ?
SetClipList             DD      ?
SetHWnd                 DD      ?

IDirectDrawClipper      ENDS


;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
DDCOLORKEY              STRUCT

dwColorSpaceLowValue    DD      ?
dwColorSpaceHighValue   DD      ?

DDCOLORKEY              ENDS

;------------------------------------------------------------------------------
DDPIXELFORMAT   STRUCT

dwSize          DD      ?
dwFlagsPF       DD      ?
dwFourCC        DD      ?
        UNION

        dwRGBBitCount           DD      ?
        dwYUVBitCount           DD      ?
        dwZBufferBitDepthPF     DD      ?
        dwAlphaBitDepthPF       DD      ?
        dwLuminanceBitCount     DD      ?
        dwBumpBitCount          DD      ?
        ENDS

        UNION
        dwRBitMask              DD      ?
        dwYBitMask              DD      ?
        dwStencilBitDepth       DD      ?
        dwLuminanceBitMask      DD      ?
        dwBumpDuBitMask         DD      ?
        ENDS

        UNION
        dwGBitMask              DD      ?
        dwUBitMask              DD      ?
        dwZBitMask              DD      ?
        dwBumpDvBitMask         DD      ?
        ENDS

        UNION
        dwBBitMask              DD      ?
        dwVBitMask              DD      ?
        dwStencilBitMask        DD      ?
        dwBumpLuminanceBitMask  DD      ?
        ENDS

        UNION
        dwRGBAlphaBitMask       DD      ?
        dwYUVAlphaBitMask       DD      ?
        dwLuminanceAlphaBitMask DD      ?
        dwRGBZBitMask           DD      ?
        dwYUVZBitMask           DD      ?
        ENDS

DDPIXELFORMAT   ENDS

;------------------------------------------------------------------------------
DDSCAPS         STRUC

dwCapsDC        DD      ?

DDSCAPS         ENDS

DDSCAPS2        STRUC           ;capabilities of surface wanted

dwCapsDC2       DD      ?
dwCapsDC22      DD      ?
dwCapsDC32      DD      ?
dwCapsDC42      DD      ?

DDSCAPS2        ENDS

;------------------------------------------------------------------------------

DDSURFACEDESC   STRUCT

dwSize                  DD      ?
dwFlagsSD               DD      ?
dwHeight                DD      ?
dwWidth                 DD      ?

        UNION
        lPitch                  DD      ?
        dwLinearSize            DD      ?
        ENDS

dwBackBufferCount       DD    ?

        UNION
        dwMipMapCount           DD      ?
        dwZBufferBitDepth       DD      ?
        dwRefreshRate           DD      ?
        ENDS

dwAlphaBitDepth         DD      ?
dwReservedSD            DD      ?
lpSurface               LPVOID  ?
ddckCKDestOverlay       DDCOLORKEY <>
ddckCKDestBlt           DDCOLORKEY <>
ddckCKSrcOverlay        DDCOLORKEY <>
ddckCKSrcBlt            DDCOLORKEY <>
ddpfPixelFormat         DDPIXELFORMAT <>
ddsCaps                 DDSCAPS <>

DDSURFACEDESC   ENDS

DDSURFACEDESC2  STRUC

dwSize2                 DD      ?       ;size of the DDSURFACEDESC structure
dwFlagsSD2              DD      ?       ;determines what fields are valid
dwHeight2               DD      ?       ;Height of surface to be created
dwWidth2                DD      ?       ;width of input surface
        UNION
        lPitch2         DD      ?       ;distance to start of next line (return value only)
        dwLinearSize2   DD      ?       ;Formless late-allocated optimized surface size
        ENDS
dwBackBufferCount2      DD      ?       ;number of back buffers requested
        UNION
        dwMipMapCount2  DD      ?       ;number of mip-map levels requestde
                                        ;dwZBufferBitDepth removed, use ddpfPixelFormat one instead
        dwRefreshRate2  DD      ?       ;refresh rate (used when display mode is described)
        dwSrcVBHandle2  DD      ?       ;The source used in VB::Optimize
        ENDS
dwAlphaBitDepth2        DD      ?       ;depth of alpha buffer requested
dwReserved2             DD      ?       ;reserved
lpSurface2              DD      ?       ;pointer to the associated surface memory
        UNION
        ddckCKDestOverlay2 DDCOLORKEY <> ;color key for destination overlay use
        dwEmptyFaceColor2 DD     ?       ;Physical color for empty cubemap faces
        ENDS
ddckCKDestBlt2          DDCOLORKEY <>   ;color key for destination blt use
ddckCKSrcOverlay2       DDCOLORKEY <>   ;color key for source overlay use
ddckCKSrcBlt2           DDCOLORKEY <>   ;color key for source blt use
        UNION
        ddpfPixelFormat DDPIXELFORMAT<> ;pixel format description of the surface
        dwFVF           DD      ?       ;vertex format description of vertex buffers
        ENDS
ddsCaps2                DDSCAPS2 <>     ;direct draw surface capabilities
dwTextureStage2         DD      ?       ;stage in multitexture cascade

ENDS

;DDBLTFX
;Used to pass override information to the DIRECTDRAWSURFACE callback Blt.
DDBLTFX                 STRUC

dwSize                  DD      ?       ;size of structure
dwDDFX                  DD      ?       ;FX operations
dwROP                   DD      ?       ;Win32 raster operations
dwDDROP                 DD      ?       ;Raster operations new for DirectDraw
dwRotationAngle         DD      ?       ;Rotation angle for blt
dwZBufferOpCode         DD      ?       ;ZBuffer compares
dwZBufferLow            DD      ?       ;Low limit of Z buffer
dwZBufferHigh           DD      ?       ;High limit of Z buffer
dwZBufferBaseDest       DD      ?       ;Destination base value
dwZDestConstBitDepth    DD      ?       ;Bit depth used to specify Z constant for destination

   UNION

        dwZDestConst    DD      ?       ;Constant to use as Z buffer for dest
        lpDDSZBufferDest DD     ?       ;Surface to use as Z buffer for dest
   ENDS

dwZSrcConstBitDepth     DD      ?       ;Bit depth used to specify Z constant for source

   UNION
        dwZSrcConst     DD      ?       ;Constant to use as Z buffer for src
        lpDDSZBufferSrc DD      ?       ;Surface to use as Z buffer for src
   ENDS

dwAlphaEdgeBlendBitDepth DD     ?       ;Bit depth used to specify constant for alpha edge blend
dwAlphaEdgeBlend        DD      ?       ;Alpha for edge blending
;dwReserved              DD      ?       ;
                        DD      ?
dwAlphaDestConstBitDepth DD     ?       ;Bit depth used to specify alpha constant for destination

   UNION 
        dwAlphaDestConst DD     ?       ;Constant to use as Alpha Channel
        lpDDSAlphaDest  DD      ?       ;Surface to use as Alpha Channel
   ENDS

dwAlphaSrcConstBitDepth DD      ?       ;Bit depth used to specify alpha constant for source

   UNION
        dwAlphaSrcConst DD      ?       ;Constant to use as Alpha Channel
        lpDDSAlphaSrc   DD      ?       ;Surface to use as Alpha Channel
   ENDS

   UNION
        dwFillColor     DD      ?       ;color in RGB or Palettized
        dwFillDepth     DD      ?       ;depth value for z-buffer
        dwFillPixel     DD      ?       ;pixel value for RGBA or RGBZ
        lpDDSPattern    DD      ?       ;Surface to use as pattern
   ENDS
   ddckDestColorkey     DDCOLORKEY ?    ;DestColorkey override
   ddckSrcColorkey      DDCOLORKEY ?    ;SrcColorkey override

DDBLTFX                 ENDS
;----------------------------------------------------------------------------
DDCAPS                  STRUCT

dwSize                  DD      ?       ;size of the DDDRIVERCAPS structure
dwCaps                  DD      ?       ;driver specific capabilities
dwCaps2                 DD      ?       ;more driver specific capabilites
dwCKeyCaps              DD      ?       ;color key capabilities of the surface
dwFXCaps                DD      ?       ;driver specific stretching and effects capabilites
dwFXAlphaCaps           DD      ?       ;alpha driver specific capabilities
dwPalCaps               DD      ?       ;palette capabilities
dwSVCaps                DD      ?       ;stereo vision capabilities
dwAlphaBltConstBitDepths DD     ?       ;DDBD_2,4,8
dwAlphaBltPixelBitDepths DD     ?       ;DDBD_1,2,4,8
dwAlphaBltSurfaceBitDepths DD   ?       ;DDBD_1,2,4,8
dwAlphaOverlayConstBitDepths DD ?       ;DDBD_2,4,8
dwAlphaOverlayPixelBitDepths DD ?       ;DDBD_1,2,4,8
dwAlphaOverlaySurfaceBitDepths DD ?     ;DDBD_1,2,4,8
dwZBufferBitDepths      DD      ?       ;DDBD_8,16,24,32
dwVidMemTotal           DD      ?       ;total amount of video memory
dwVidMemFree            DD      ?       ;amount of free video memory
dwMaxVisibleOverlays    DD      ?       ;maximum number of visible overlays
dwCurrVisibleOverlays   DD      ?       ;current number of visible overlays
dwNumFourCCCodes        DD      ?       ;number of four cc codes
dwAlignBoundarySrc      DD      ?       ;source rectangle alignment
dwAlignSizeSrc          DD      ?       ;source rectangle byte size
dwAlignBoundaryDest     DD      ?       ;dest rectangle alignment
dwAlignSizeDest         DD      ?       ;dest rectangle byte size
dwAlignStrideAlign      DD      ?       ;stride alignment
dwRops                  DD      8 DUP(?);ROPS supported
ddsOldCaps              DDSCAPS <>      ;Was DDSCAPS  ddsCaps. ddsCaps is of type DDSCAPS2 for DX6
dwMinOverlayStretch     DD      ?       ;minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwMaxOverlayStretch     DD      ?       ;maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwMinLiveVideoStretch   DD      ?       ;minimum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwMaxLiveVideoStretch   DD      ?       ;maximum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwMinHwCodecStretch     DD      ?       ;minimum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwMaxHwCodecStretch     DD      ?       ;maximum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
dwReserved1DDC          DD      ?       ;reserved
dwReserved2DDC          DD      ?       ;reserved
dwReserved3DDC          DD      ?       ;reserved
dwSVBCaps               DD      ?       ;driver specific capabilities for System->Vmem blts
dwSVBCKeyCaps           DD      ?       ;driver color key capabilities for System->Vmem blts
dwSVBFXCaps             DD      ?       ;driver FX capabilities for System->Vmem blts
dwSVBRops               DD      8 DUP(?);ROPS supported for System->Vmem blts
dwVSBCaps               DD      ?       ;driver specific capabilities for Vmem->System blts
dwVSBCKeyCaps           DD      ?       ;driver color key capabilities for Vmem->System blts
dwVSBFXCaps             DD      ?       ;driver FX capabilities for Vmem->System blts
dwVSBRops               DD      8 DUP(?);ROPS supported for Vmem->System blts
dwSSBCaps               DD      ?       ;driver specific capabilities for System->System blts
dwSSBCKeyCaps           DD      ?       ;driver color key capabilities for System->System blts
dwSSBFXCaps             DD      ?       ;driver FX capabilities for System->System blts
dwSSBRops               DD      8 DUP(?);ROPS supported for System->System blts
dwMaxVideoPorts         DD      ?       ;maximum number of usable video ports
dwCurrVideoPorts        DD      ?       ;current number of video ports used
dwSVBCaps2              DD      ?       ;more driver specific capabilities for System->Vmem blts
dwNLVBCaps              DD      ?       ;driver specific capabilities for non-local->local vidmem blts
dwNLVBCaps2             DD      ?       ;more driver specific capabilities non-local->local vidmem blts
dwNLVBCKeyCaps          DD      ?       ;driver color key capabilities for non-local->local vidmem blts
dwNLVBFXCaps            DD      ?       ;driver FX capabilities for non-local->local blts
dwNLVBRops              DD      8 DUP(?);ROPS supported for non-local->local blts
;Members added for DX6 release
ddsCapsDDC              DDSCAPS2 ?      ;Surface Caps

DDCAPS                  ENDS
;------------------------------------------------------------------------------
