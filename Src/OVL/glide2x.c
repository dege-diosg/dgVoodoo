/*--------------------------------------------------------------------------------- */
/* Glide2x.c - DOS Glide2x OVL implementation                                       */
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
/* dgVoodoo Glide Wrapper, DOS driver GLIDE2X.OVL											*/
/*------------------------------------------------------------------------------------------*/

#define	FX_GLIDE_NO_FUNC_PROTO

#include <glide.h>
//#include <dgVoodooConfig.h>
#include <Dos.h>
#include <RMDriver.h>

/*------------------------------------------------------------------------------------------*/
/*..................................... Definíciók .........................................*/

#define EXPORT					__export __stdcall

#if GLIDE1

#define VERSIONSTR				"Glide 2.11"

#else

#define VERSIONSTR				"Glide 2.43"

#endif

#define PLATFORM_DOSWIN9X		1
#define PLATFORM_DOSWINNT		2

#define MAX_UTIL_TEXTURES       1024

#define NULL					((void *) 0)

#include "notimp.c"

/*------------------------------------------------------------------------------------------*/
/*................................... Külsõ függvények .....................................*/

extern void _stdcall	CopyMem (void *dst, void *src, unsigned int len);
extern void _stdcall	CopyData (const void *ptr, unsigned int len);
extern void				GlideDLLInit ();
extern void				GlideDLLCleanUp ();
extern int  _stdcall	IsRealModeDriverPresent (unsigned int vector);
extern void * _stdcall	DPMIGetMem (unsigned int len);
extern void _stdcall	DPMIFreeMem (void *);
extern int _stdcall		GetCanonicFileName (const char *);
extern void * _stdcall	InstallFakeVesaDrv ();
extern void _stdcall	UnInstallFakeVesaDrv (void *);
extern void * _stdcall	InstallMouseDrv (int,int);
extern void _stdcall	UnInstallMouseDrv (void *);


/*------------------------------------------------------------------------------------------*/
/*.................................... Külsõ változók ......................................*/

extern CommArea			*ca, *ca_server;
extern unsigned int		databuffend;
extern unsigned char	canrun;
extern char				filename[];
extern dgVoodooConfig	config;
extern unsigned int		platform;
extern void				*sap;
extern int				exebuff_free;


/*------------------------------------------------------------------------------------------*/
/*....................................... Változók .........................................*/

unsigned short	_xResolution[] = {320, 320, 400, 512, 640, 640, 640, 640, 800, 960, 856, 512, 1024, 1280, 1600, 400};
unsigned short	_yResolution[] = {200, 240, 256, 384, 200, 350, 400, 480, 600, 720, 480, 256, 768, 1024, 1200, 300};
unsigned int	lodToSize[]	   = { 256, 128, 64, 32, 16, 8, 4, 2, 1 };
unsigned char	aspectRatioScaler[]	= { 3, 2, 1, 0, 1, 2, 3 };
unsigned char	formatBYTEPP[]		= { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
FxU32			screenWidth, screenHeight;
unsigned char	vgaMode;
unsigned char	*lfb;
GrMipMapId_t	actutex;
int				gshutdown = 0;
float			IndToW[GR_FOG_TABLE_SIZE];
GrMipMapInfo	*mipmapInfos;
void			*hMouse, *hVesa;


/*------------------------------------------------------------------------------------------*/
/*........................................ Makrók ..........................................*/

#define PEXECBUFF	register unsigned long *execbuff;

#define GetExecBuffPointer(n)    \
                                 \
        if ((ca->ExeCodeIndex + n) >= EXEBUFFSIZE) FlushExecBuffer(); \
        execbuff = ca->ExeCodes + ca->ExeCodeIndex;

#define VERTEX_EXTRA sizeof(GrTmuVertex)

#define CheckForRoom(x) {       \
        if ( (((unsigned int) ca->FDPtr) + (x)) > databuffend) FlushExecBuffer();      \
}

#define GetFDPtr()			( ((unsigned long) ca->FDPtr) - ( (unsigned long) ca) + ( (unsigned long) ca_server) )

#define GlideGetLOD(lod)				lodToSize[lod]
#define	GlideGetTexBYTEPP(format)		formatBYTEPP[format]

/*------------------------------------------------------------------------------------------*/
/*......................... Segédfüggvények a textúraméret-számításhoz .....................*/

unsigned int GlideGetXY(GrLOD_t Lod, GrAspectRatio_t ar)
{
		unsigned int xy;

        xy = GlideGetLOD(Lod);
		return (ar > GR_ASPECT_1x1) ? ((xy >> aspectRatioScaler[ar]) << 16) + xy : (xy << 16) + (xy >> (aspectRatioScaler[ar]));
}


unsigned int GetTextureSize(GrLOD_t smallLod, GrLOD_t largeLod,
                            GrAspectRatio_t aspect, GrTextureFormat_t format)
{
		unsigned int llod, slod, size, needed;

        llod = GlideGetLOD(largeLod);
        slod = GlideGetLOD(smallLod);
        needed = GlideGetXY(largeLod,aspect);
        size = ( (needed >> 16) * (needed & 0xFFFF) ) * GlideGetTexBYTEPP(format);
        for(needed = 0; llod >= slod; llod >>= 1) {
                needed += size;
                if ( (size >>= 2) == 0 ) size = 1;
        }
        return(needed);
}


/*------------------------------------------------------------------------------------------*/
/*.................................... Glide-függvények ....................................*/

FxBool EXPORT grSstQueryBoards( GrHwConfiguration *hwConfig )   {

        //if (!canrun) return(FXFALSE);
        hwConfig->num_sst = 1;
        return(FXTRUE);
}


FxBool EXPORT grSstQueryHardware( GrHwConfiguration *hwConfig ) {

        //if (!canrun) return(FXFALSE);
        hwConfig->SSTs[0].type = GR_SSTTYPE_VOODOO;
        hwConfig->SSTs[0].sstBoard.VoodooConfig.fbRam = config.TexMemSize / (1024*1024);
        hwConfig->SSTs[0].sstBoard.VoodooConfig.fbiRev = 1;
        hwConfig->SSTs[0].sstBoard.VoodooConfig.nTexelfx = 1;
        hwConfig->SSTs[0].sstBoard.VoodooConfig.sliDetect = FXFALSE;
        hwConfig->SSTs[0].sstBoard.VoodooConfig.tmuConfig[0].tmuRev = 1;
        hwConfig->SSTs[0].sstBoard.VoodooConfig.tmuConfig[0].tmuRam = config.TexMemSize / (1024*1024);
        return(FXTRUE);
}


void EXPORT grGlideGetVersion( char version[80] )       {
		
		int	i = 0;
        while(version[i] = VERSIONSTR[i]) i++;
}


void EXPORT grGlideGetState( GrState *state ) {
		
		PEXECBUFF;
		register int	isVddMode;
		void			*FDPtr;

        GetExecBuffPointer(2);
        execbuff[0] = GRGLIDEGETSTATE;
        execbuff[1] = (unsigned long) state;
        isVddMode = (config.Flags & CFG_NTVDDMODE) && (state >= sap);
        if (!isVddMode)       {
			execbuff[1] = (unsigned long) GetFDPtr();
            FDPtr = ca->FDPtr;
        }
        ca->ExeCodeIndex += 2;
        FlushExecBuffer();
        if (!isVddMode) CopyMem(state, FDPtr, sizeof(GrState));
}


void EXPORT grGlideSetState( const GrState *state ) {
		
		PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRGLIDESETSTATE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(state, sizeof(GrState));
        ca->ExeCodeIndex += 2;
}


FxU32 EXPORT grSstStatus( void ) {
		
		PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRSSTSTATUS;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
        return( (FxU32) execbuff[0] );
}


FxU32 EXPORT grSstVideoLine( void ) {
		
		PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRSSTVIDEOLINE;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
        return( (FxU32) execbuff[0] );
}


FxBool EXPORT grSstVRetraceOn( void ) {
		PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRSSTVRETRACEON;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT grSstIsBusy( void ) {

		return(FXFALSE);
}


void EXPORT grResetTristats()  {
PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRRESETTRISTATS;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
}


void EXPORT grTristats(FxU32 *trisProcessed, FxU32 *trisDrawn)  {
PEXECBUFF;
FxU32 *retp;

        GetExecBuffPointer(3);
        execbuff[0] = GRTRISTATS;
        execbuff[1] = (unsigned long) ((FxU32 *)GetFDPtr());
        execbuff[2] = (unsigned long) (((FxU32 *)GetFDPtr()) + 1);
        retp = (FxU32 *) ca->FDPtr;
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        *trisProcessed = retp[0];
        *trisDrawn = retp[1];
}


void EXPORT grGlideInit( void ) {
PEXECBUFF;

		GlideDLLInit ();
        if (!canrun) return;
        GetExecBuffPointer(1);
        execbuff[0] = GRGLIDEINIT;
        ca->ExeCodeIndex++;
}


void EXPORT grGlideShutdown(void ) {
PEXECBUFF;

        if (!canrun) return;
        gshutdown = 0;
        GetExecBuffPointer(1);
        if (!(config.Flags & CFG_VESAENABLE) ||
			((config.Flags & CFG_VESAENABLE) && !IsRealModeDriverPresent (0x10)) ) {
                
			UnInstallFakeVesaDrv(hVesa);
            hVesa = NULL;
        }
        if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) )        {
                execbuff[0] = GLIDEUNINSTALLMOUSE;
                ca->ExeCodeIndex++;
                FlushExecBuffer();
        }
        execbuff[0] = GRGLIDESHUTDOWN;
        ca->ExeCodeIndex += 1;
        FlushExecBuffer();
        _asm {
                call GlideDLLCleanUp
                push eax
                mov  ax,3h
                ;xor  ah,ah
                ;mov  al,vgaMode
                int  10h
                pop eax
        }
        if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) )        {
                UnInstallMouseDrv(hMouse);
                hMouse = NULL;
        }
        canrun = 0;
}


#ifndef GLIDE1

void EXPORT grSstWinClose( void );

FxBool EXPORT grSstWinOpen(FxU32 hwnd,
                           GrScreenResolution_t res,
                           GrScreenRefresh_t ref,
                           GrColorFormat_t cformat,
                           GrOriginLocation_t org_loc,
                           int num_buffers,
                           int num_aux_buffers)    {

PEXECBUFF;
FxBool	retVal;
void	*FDPtr;

    if (!canrun) return(FXFALSE);
    if (gshutdown) return(FXFALSE);//grSstWinClose();
    _asm    {
            push eax
			mov  ah,0fh
			int  10h
			mov  vgaMode,al
           ; cmp  al,3h
           ; je   grWOtextmode
            mov  ax,3h
            int  10h
    grWOtextmode:
            pop eax
    }
	if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) )        {
		hMouse = InstallMouseDrv(_xResolution[res], _yResolution[res]);
	}
    gshutdown = 0;
    GetExecBuffPointer(10);
    execbuff[0] = GRSSTWINOPEN;
    execbuff[1] = (unsigned long) hwnd;
    execbuff[2] = (unsigned long) res;
    execbuff[3] = (unsigned long) ref;
    execbuff[4] = (unsigned long) cformat;
    execbuff[5] = (unsigned long) org_loc;
    execbuff[6] = (unsigned long) num_buffers;
    execbuff[7] = (unsigned long) num_aux_buffers;
    execbuff[8] = GLIDEGETINDTOWTABLE;
    execbuff[9] = (unsigned long) GetFDPtr();
    FDPtr = ca->FDPtr;
    ca->ExeCodeIndex += 10;
    FlushExecBuffer();
    CopyMem(IndToW, FDPtr, GR_FOG_TABLE_SIZE * sizeof(float));
	
	if (retVal = (FxBool) execbuff[0]) {
		gshutdown = 1;
		execbuff = ca->ExeCodes;
		execbuff[0] = GRSSTSCREENWIDTH;
		execbuff[1] = GRSSTSCREENHEIGHT;
		execbuff[2] = GLIDEGETCONVBUFFXRES;
		ca->ExeCodeIndex += 3;
		FlushExecBuffer();
		screenWidth = (FxU32) execbuff[2];
		screenHeight = (FxU32) execbuff[1];

		if (config.Flags & CFG_NTVDDMODE)       {
				lfb = (unsigned char *) DPMIGetMem(screenWidth * screenHeight * LFBMAXPIXELSIZE * 3);
				execbuff[4] = 0;
		} else {
				lfb = ca->LFB;
				execbuff[4] = 1;
		}
		execbuff[0] = GLIDESETUPLFBDOSBUFFERS;
		execbuff[1] = (unsigned long) lfb;
		execbuff[2] = (unsigned long) lfb + screenWidth * screenHeight * LFBMAXPIXELSIZE;
		execbuff[3] = (unsigned long) lfb + 2 * screenWidth * screenHeight * LFBMAXPIXELSIZE;
		ca->ExeCodeIndex += 5;
		FlushExecBuffer();
		
		if (execbuff[0])     {
			if (!(config.Flags & CFG_VESAENABLE) ||
				((config.Flags & CFG_VESAENABLE) && !IsRealModeDriverPresent (0x10)) ) {
				
				hVesa = InstallFakeVesaDrv();
			}
			if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) && (hMouse != NULL))        {
				execbuff[0] = GLIDEINSTALLMOUSE;
				execbuff[1] = (unsigned long) ( ((char*)hMouse) + (((MouseDriverHeader *) hMouse)->driverInfoStruct - 0x100) );
				ca->ExeCodeIndex += 2;
				FlushExecBuffer();
			}
		} else {
			grSstWinClose();
			retVal = FXFALSE;
		}
		mipmapInfos = NULL;
	}
	if (retVal == FXFALSE) {
		if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) && (hMouse != NULL))      {
			UnInstallMouseDrv(hMouse);
			hMouse = NULL;
		}
	}
    return(retVal);
}


void EXPORT grSstWinClose( void )       {
PEXECBUFF;

    if (!canrun) return;
    if (!gshutdown) return;
    gshutdown = 0;
    if (mipmapInfos) DPMIFreeMem(mipmapInfos);
    GetExecBuffPointer(1);
    if ((config.Flags & CFG_NTVDDMODE) && (lfb != 0)) {
		DPMIFreeMem(lfb);
	}
    
    if (!(config.Flags & CFG_VESAENABLE) ||
		((config.Flags & CFG_VESAENABLE) && !IsRealModeDriverPresent (0x10)) ) {

        UnInstallFakeVesaDrv (hVesa);
        hVesa = NULL;
    }
    if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) )        {
        execbuff[0] = GLIDEUNINSTALLMOUSE;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
    }
    execbuff[0] = GRSSTWINCLOSE;
    ca->ExeCodeIndex += 1;
    FlushExecBuffer();
    _asm {
        push eax
        xor  ah,ah
        mov  al,vgaMode
        ;cmp  al,3h
        ;je   grWCtextmode
        int  10h
grWCtextmode:
        pop eax
    }
    if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) && (hMouse != NULL) )        {
        UnInstallMouseDrv(hMouse);
        hMouse = NULL;
    }
}


#else


FxBool EXPORT grSstOpen(GrScreenResolution_t res, GrScreenRefresh_t ref,
                        GrColorFormat_t cformat, GrOriginLocation_t org_loc,
                        GrSmoothingMode_t smoothing_mode, int num_buffers )     {

PEXECBUFF;
FxBool	retval;
void	*FDPtr;

        if (!canrun) return(FXFALSE);
        if (gshutdown) return(FXFALSE);
        if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) )        {
			execbuff[0] = GLIDEISMOUSEINSTALLED;
			ca->ExeCodeIndex++;
            FlushExecBuffer();
			if (!execbuff[0])
				hMouse = InstallMouseDrv(_xResolution[res], _yResolution[res]);
			else
				hMouse = NULL;
        }
        _asm    {
                push eax
				mov  ah,0fh
				int  10h
				mov  vgaMode,al
               ; cmp  al,3h
               ; je   grWOtextmode
                mov  ax,3h
                int  10h
        grWOtextmode:
                pop eax
        }
        gshutdown = 0;
        GetExecBuffPointer(9);
        execbuff[0] = GRSSTOPEN;
        execbuff[1] = (unsigned long) res;
        execbuff[2] = (unsigned long) ref;
        execbuff[3] = (unsigned long) cformat;
        execbuff[4] = (unsigned long) org_loc;
        execbuff[5] = (unsigned long) smoothing_mode;
        execbuff[6] = (unsigned long) num_buffers;
        execbuff[7] = GLIDEGETINDTOWTABLE;
        execbuff[8] = (unsigned long) GetFDPtr();
        FDPtr = ca->FDPtr;
        ca->ExeCodeIndex += 9;
        FlushExecBuffer();
        CopyMem(IndToW, FDPtr, GR_FOG_TABLE_SIZE * sizeof(float));
        if (retval = (FxBool) execbuff[0]) gshutdown = 1;
        execbuff = ca->ExeCodes;
        execbuff[0] = GRSSTSCREENWIDTH;
        execbuff[1] = GRSSTSCREENHEIGHT;
        execbuff[2] = GLIDEGETCONVBUFFXRES;
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        screenWidth = (FxU32) execbuff[2];
        screenHeight = (FxU32) execbuff[1];
        if (config.Flags & CFG_NTVDDMODE)       {
                lfb = (unsigned char *) DPMIGetMem(screenWidth * screenHeight * LFBMAXPIXELSIZE * 3);
                execbuff[4] = 0;
        } else {
                lfb = ca->LFB;
                execbuff[4] = 1;
        }
        execbuff[0] = GLIDESETUPLFBDOSBUFFERS;
        execbuff[1] = (unsigned long) lfb;
        execbuff[2] = (unsigned long) lfb + screenWidth * screenHeight * LFBMAXPIXELSIZE;
        execbuff[3] = (unsigned long) lfb + 2 * screenWidth * screenHeight * LFBMAXPIXELSIZE;
        ca->ExeCodeIndex += 5;
        FlushExecBuffer();
        if (!execbuff[0])       {
                grGlideShutdown();
                retval = FXFALSE;
        }
        if (retval)     {
                if (!(config.Flags & CFG_VESAENABLE) ||
					((config.Flags & CFG_VESAENABLE) && !IsRealModeDriverPresent (0x10)) ) {

					hVesa = InstallFakeVesaDrv();
				}
                if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) && (hMouse != NULL))        {
                        execbuff[0] = GLIDEINSTALLMOUSE;
                        execbuff[1] = (unsigned long) ( ((char*)hMouse)+(((_dllheader*)hMouse)->driverinfo - 0x100) );
                        ca->ExeCodeIndex += 2;
                        FlushExecBuffer();
                 }
        } else {
                if ( (platform == PLATFORM_DOSWINNT) && (config.Flags & CFG_SETMOUSEFOCUS) && (hMouse != NULL))      {
                        UnInstallMouseDrv(hMouse);
                        hMouse = NULL;
                }
        }
        mipmapInfos = NULL;
        return(retval);
}

#endif


void EXPORT grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth ) {
PEXECBUFF;

        GetExecBuffPointer(4);
        execbuff[0] = GRBUFFERCLEAR;
        execbuff[1] = (unsigned long) color;
        execbuff[2] = (unsigned long) alpha;
        execbuff[3] = (unsigned long) depth;
        ca->ExeCodeIndex += 4;
}


void EXPORT grBufferSwap( int swap_interval ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRBUFFERSWAP;
        execbuff[1] = (unsigned long) swap_interval;
        ca->ExeCodeIndex += 2;
        FlushExecBuffer();
}


int EXPORT grBufferNumPending( void ) {
PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRBUFFERNUMPENDING;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
        return( (int) execbuff[0] );
}


void EXPORT grRenderBuffer( GrBuffer_t buffer ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRRENDERBUFFER;
        execbuff[1] = (unsigned long) buffer;
        ca->ExeCodeIndex += 2;
}


void EXPORT grSstOrigin( GrOriginLocation_t origin ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRSSTORIGIN;
        execbuff[1] = (unsigned long) origin;
        ca->ExeCodeIndex += 2;
}


void EXPORT grCullMode( GrCullMode_t mode ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRCULLMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grDisableAllEffects( void ) {
PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRDISABLEALLEFFECTS;
        ca->ExeCodeIndex++;
}


void EXPORT grDitherMode( GrDitherMode_t mode ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRDITHERMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grGammaCorrectionValue( float value ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRGAMMACORRECTIONVALUE;
        //(float) execbuff[1] = value;
		execbuff[1] = * (unsigned long *) &value;
        ca->ExeCodeIndex += 2;
}


void EXPORT grHints( GrHint_t type, FxU32 hintMask ) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GRHINTS;
        execbuff[1] = type;
        execbuff[2] = hintMask;
        ca->ExeCodeIndex += 3;        
}


void EXPORT grColorCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert ) {
PEXECBUFF;

        GetExecBuffPointer(6);
        execbuff[0] = GRCOLORCOMBINE;
        execbuff[1] = (unsigned long) func;
        execbuff[2] = (unsigned long) factor;
        execbuff[3] = (unsigned long) local;
        execbuff[4] = (unsigned long) other;
        execbuff[5] = (unsigned long) invert;
        ca->ExeCodeIndex += 6;
}


void EXPORT grAlphaCombine(	GrCombineFunction_t func, 
							GrCombineFactor_t factor,
							GrCombineLocal_t local, 
							GrCombineOther_t other, 
							FxBool invert ) {
PEXECBUFF;

        GetExecBuffPointer(6);
        execbuff[0] = GRALPHACOMBINE;
        execbuff[1] = (unsigned long) func;
        execbuff[2] = (unsigned long) factor;
        execbuff[3] = (unsigned long) local;
        execbuff[4] = (unsigned long) other;
        execbuff[5] = (unsigned long) invert;
        ca->ExeCodeIndex += 6;
}


void EXPORT grConstantColorValue( GrColor_t color ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRCONSTANTCOLORVALUE;
        execbuff[1] = (unsigned long) color;
        ca->ExeCodeIndex += 2;
}


void EXPORT grConstantColorValue4( float a, float r, float g, float b ) {
PEXECBUFF;

        GetExecBuffPointer(5);
        execbuff[0] = GRCONSTANTCOLORVALUE4;
        execbuff[1] = * (unsigned long *) &a;
        execbuff[2] = * (unsigned long *) &r;
        execbuff[3] = * (unsigned long *) &g;
        execbuff[4] = * (unsigned long *) &b;
        ca->ExeCodeIndex += 5;
}


void EXPORT grAlphaTestFunction( GrCmpFnc_t function )  {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRALPHATESTFUNCTION;
        execbuff[1] = (unsigned int) function;
        ca->ExeCodeIndex += 2;        
}


void EXPORT grAlphaTestReferenceValue( GrAlpha_t value )        {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRALPHATESTREFERENCEVALUE;
        execbuff[1] = (unsigned int) value;
        ca->ExeCodeIndex += 2;
}


void EXPORT grColorMask( FxBool rgb, FxBool alpha ) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GRCOLORMASK;
        execbuff[1] = (unsigned long) rgb;
        execbuff[2] = (unsigned long) alpha;
        ca->ExeCodeIndex += 3;
}


void EXPORT grAlphaBlendFunction(	GrAlphaBlendFnc_t rgb_sf,
									GrAlphaBlendFnc_t rgb_df,
									GrAlphaBlendFnc_t alpha_sf,
									GrAlphaBlendFnc_t alpha_df )    {
PEXECBUFF;

        GetExecBuffPointer(5);
        execbuff[0] = GRALPHABLENDFUNCTION;
        execbuff[1] = (unsigned long) rgb_sf;
        execbuff[2] = (unsigned long) rgb_df;
        execbuff[3] = (unsigned long) alpha_sf;
        execbuff[4] = (unsigned long) alpha_df;
        ca->ExeCodeIndex += 5;
}


void EXPORT grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )   {
PEXECBUFF;

        CheckForRoom(3 * (sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(4);
        execbuff[0] = GRDRAWTRIANGLE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(a, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(b, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[3] = (unsigned long) GetFDPtr();
        CopyData(c, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 4;
}


void EXPORT grAADrawPoint(GrVertex *p) {
PEXECBUFF;

        CheckForRoom((sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(2);
        execbuff[0] = GRAADRAWPOINT;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(p, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 2;
}


void EXPORT grDrawPoint( const GrVertex *a ) {
PEXECBUFF;

        CheckForRoom((sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(2);
        execbuff[0] = GRDRAWPOINT;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(a, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 2;
}


void EXPORT grAADrawLine(GrVertex *va, GrVertex *vb) {
PEXECBUFF;

        CheckForRoom(2 * (sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(3);
        execbuff[0] = GRAADRAWLINE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(va, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vb, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 3;
}


void EXPORT grDrawLine( const GrVertex *a, const GrVertex *b ) {
PEXECBUFF;

        CheckForRoom(2*(sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(3);
        execbuff[0] = GRDRAWLINE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(a, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(b, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 3;        
}


void EXPORT grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[]) {
PEXECBUFF;
unsigned int i;
int *p;

        CheckForRoom(nVerts * sizeof(GrVertex) + (nVerts-1) * 3 * sizeof(int));
        GetExecBuffPointer(4);
        execbuff[0] = GRAADRAWPOLYGON;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        p = (int *) ca->FDPtr;
        for(i = 0; i < nVerts - 2; i++) {
                p[0] = 0;
                p[1] = i + 1;
                p[2] = i + 2;
                p += 3;
        }
        ca->FDPtr = (void *) p;
        execbuff[3] = (unsigned long) GetFDPtr();
        for(i = 0; i < nVerts; i++) CopyData(&vlist[ilist[i]], sizeof(GrVertex));
        ca->ExeCodeIndex += 4;
}


void EXPORT grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[]) {
PEXECBUFF;

        CheckForRoom(nVerts * sizeof(GrVertex));
        GetExecBuffPointer(3);
        execbuff[0] = GRAADRAWPOLYGONVERTEXLIST;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vlist, nVerts * sizeof(GrVertex));
        ca->ExeCodeIndex += 3;
}


void EXPORT grAADrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c,
                              FxBool antialiasAB, FxBool antialiasBC, FxBool antialiasCA) {
PEXECBUFF;

        CheckForRoom(3 * (sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(7);
        execbuff[0] = GRAADRAWTRIANGLE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(a, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(b, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[3] = (unsigned long) GetFDPtr();
        CopyData(c, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[4] = (unsigned long) antialiasAB;
        execbuff[5] = (unsigned long) antialiasBC;
        execbuff[6] = (unsigned long) antialiasCA;
        ca->ExeCodeIndex += 7;
}


void EXPORT grDrawPlanarPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
PEXECBUFF;
unsigned	int i;
int			*p;

        CheckForRoom(nVerts * sizeof(GrVertex) + (nVerts-1) * 3 * sizeof(int));
        GetExecBuffPointer(4);
        execbuff[0] = GRDRAWPLANARPOLYGON;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        p = (int *) ca->FDPtr;
        for(i = 0; i < nVerts - 2; i++) {
                p[0] = 0;
                p[1] = i + 1;
                p[2] = i + 2;
                p += 3;
        }
        ca->FDPtr = (void *) p;
        execbuff[3] = (unsigned long) GetFDPtr();
        for(i = 0; i < nVerts; i++) CopyData(&vlist[ilist[i]], sizeof(GrVertex));
        ca->ExeCodeIndex += 4;
}


void EXPORT grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] ) {
PEXECBUFF;

        CheckForRoom(nVerts * sizeof(GrVertex));
        GetExecBuffPointer(3);
        execbuff[0] = GRDRAWPLANARPOLYGONVERTEXLIST;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vlist, nVerts * sizeof(GrVertex));
        ca->ExeCodeIndex += 3;
}


void EXPORT grDrawPolygon( int nVerts, int ilist[], const GrVertex vlist[] ) {
PEXECBUFF;
unsigned	int i;
int			*p;

        CheckForRoom(nVerts * sizeof(GrVertex) + (nVerts - 1) * 3 * sizeof(int));
        GetExecBuffPointer(4);
        execbuff[0] = GRDRAWPOLYGON;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        p = (int *) ca->FDPtr;
        for(i = 0; i < nVerts - 2; i++) {
                p[0] = 0;
                p[1] = i + 1;
                p[2] = i + 2;
                p += 3;
        }
        ca->FDPtr=(void *) p;
        execbuff[3]=(unsigned long) GetFDPtr();
        for(i=0;i<nVerts;i++) CopyData(&vlist[ilist[i]],sizeof(GrVertex));
        ca->ExeCodeIndex+=4;
}


void EXPORT guDrawTriangleWithClip(const GrVertex *va, const GrVertex *vb, const GrVertex *vc ) {
PEXECBUFF;

        CheckForRoom(3 * (sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(4);
        execbuff[0] = GUDRAWTRIANGLEWITHCLIP;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(va, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vb, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[3] = (unsigned long) GetFDPtr();
        CopyData(vc, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 4;
}


void EXPORT guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc ) {
PEXECBUFF;

        CheckForRoom(3 * (sizeof(GrVertex) - VERTEX_EXTRA));
        GetExecBuffPointer(4);
        execbuff[0] = GUAADRAWTRIANGLEWITHCLIP;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(va, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vb, sizeof(GrVertex) - VERTEX_EXTRA);
        execbuff[3] = (unsigned long) GetFDPtr();
        CopyData(vc, sizeof(GrVertex) - VERTEX_EXTRA);
        ca->ExeCodeIndex += 4;
}


void EXPORT grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] ) {
PEXECBUFF;

        CheckForRoom(nVerts * sizeof(GrVertex));
        GetExecBuffPointer(3);
        execbuff[0] = GRDRAWPOLYGONVERTEXLIST;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vlist, nVerts * sizeof(GrVertex));
        ca->ExeCodeIndex += 3;
}


void EXPORT guDrawPolygonVertexListWithClip(int nVerts, const GrVertex vlist[])    {
PEXECBUFF;

        CheckForRoom(nVerts * sizeof(GrVertex));
        GetExecBuffPointer(3);
        execbuff[0] = GUDRAWPOLYGONVERTEXLISTWITHCLIP;
        execbuff[1] = (unsigned long) nVerts;
        execbuff[2] = (unsigned long) GetFDPtr();
        CopyData(vlist, nVerts * sizeof(GrVertex));
        ca->ExeCodeIndex += 3;
}


void EXPORT grTexDownloadMipMap( GrChipID_t tmu, FxU32 startAddress,
                                 FxU32 evenOdd, GrTexInfo *info )       {
PEXECBUFF;
register int	vdd;
GrTexInfo		*tempinfo;
void			*FDPtr;
unsigned int	copylen;

        GetExecBuffPointer(5);
        vdd = (config.Flags & CFG_NTVDDMODE) && (info >= sap) && (info->data >= sap);
        if (vdd)       {
                execbuff[4] = (unsigned long) info;
        } else {                
                CheckForRoom((copylen = GetTextureSize(info->smallLod, info->largeLod, info->aspectRatio, info->format)) + sizeof(GrTexInfo));
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                tempinfo = (GrTexInfo *) ca->FDPtr;
                FDPtr = (void *) GetFDPtr();
                CopyData(info, sizeof(GrTexInfo));
                tempinfo->data = (void *) GetFDPtr();
                CopyData(info->data, copylen);
                execbuff[4] = (unsigned long) FDPtr;
        }
        execbuff[0] = GRTEXDOWNLOADMIPMAP;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) startAddress;
        execbuff[3] = (unsigned long) evenOdd;
        ca->ExeCodeIndex += 5;
        FlushExecBuffer();
}


void EXPORT grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress,
				   GrLOD_t thisLod, GrLOD_t largeLod,
       				GrAspectRatio_t aspectRatio,
				GrTextureFormat_t format,
                                FxU32 evenOdd, void *data )     {
PEXECBUFF;
register int vdd;
unsigned int copylen;

        GetExecBuffPointer(9);
        vdd=(config.Flags & CFG_NTVDDMODE) && (data>=sap);
        if (vdd)       {
                execbuff[8]=(unsigned long) data;
        } else {
                CheckForRoom(copylen=GetTextureSize(thisLod,thisLod,aspectRatio, format));
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                execbuff[8]=(unsigned long) GetFDPtr();
                CopyData(data,copylen);
        }
        execbuff[0]=GRTEXDOWNLOADMIPMAPLEVEL;
        execbuff[1]=(unsigned long) tmu;
        execbuff[2]=(unsigned long) startAddress;
        execbuff[3]=(unsigned long) thisLod;
        execbuff[4]=(unsigned long) largeLod;
        execbuff[5]=(unsigned long) aspectRatio;
        execbuff[6]=(unsigned long) format;
        execbuff[7]=(unsigned long) evenOdd;
        ca->ExeCodeIndex+=9;
        FlushExecBuffer();        
}


void EXPORT grTexDownloadMipMapLevelPartial( GrChipID_t tmu, FxU32 startAddress,
                                             GrLOD_t thisLod, GrLOD_t largeLod,
                                             GrAspectRatio_t aspectRatio,
                                             GrTextureFormat_t format,
                                             FxU32 evenOdd, void *data,
                                             int start, int end )    {
PEXECBUFF;
register int vdd;
unsigned int copylen;

        GetExecBuffPointer(11);
        vdd = (config.Flags & CFG_NTVDDMODE) && (data >= sap);
        if (vdd)       {
                execbuff[8] = (unsigned long) data;
        } else {
                CheckForRoom(copylen = (GlideGetXY(thisLod, aspectRatio) >> 16) * GlideGetTexBYTEPP(format) * (end - start + 1) );
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                execbuff[8] = (unsigned long) GetFDPtr();
                CopyData(data, copylen);
        }
        execbuff[0] = GRTEXDOWNLOADMIPMAPLEVELPARTIAL;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) startAddress;
        execbuff[3] = (unsigned long) thisLod;
        execbuff[4] = (unsigned long) largeLod;
        execbuff[5] = (unsigned long) aspectRatio;
        execbuff[6] = (unsigned long) format;
        execbuff[7] = (unsigned long) evenOdd;
        execbuff[9] = (unsigned long) start;
        execbuff[10] =(unsigned long) end;
        ca->ExeCodeIndex += 11;
        FlushExecBuffer();
}


void EXPORT grTexSource( GrChipID_t tmu, FxU32 startAddress, 
                         FxU32 evenOdd, GrTexInfo *info ) {
PEXECBUFF;

        GetExecBuffPointer(5);
        execbuff[0] = GRTEXSOURCE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) startAddress;
        execbuff[3] = (unsigned long) evenOdd;
        execbuff[4] = (unsigned long) GetFDPtr();
        CopyData(info, sizeof(GrTexInfo) );
        ca->ExeCodeIndex += 5;
}


void EXPORT grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend ) {
PEXECBUFF;

        GetExecBuffPointer(4);
        execbuff[0] = GRTEXMIPMAPMODE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) mode;
        execbuff[3] = (unsigned long) lodBlend;
        ca->ExeCodeIndex += 4;
}


void EXPORT grTexCombine(
             GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor, 
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert,
             FxBool alpha_invert) {
PEXECBUFF;

        GetExecBuffPointer(8);
        execbuff[0] = GRTEXCOMBINE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) rgb_function;
        execbuff[3] = (unsigned long) rgb_factor;
        execbuff[4] = (unsigned long) alpha_function;
        execbuff[5] = (unsigned long) alpha_factor;
        execbuff[6] = (unsigned long) rgb_invert;
        execbuff[7] = (unsigned long) rgb_invert;
        ca->ExeCodeIndex += 8;
}


void EXPORT grTexFilterMode( GrChipID_t tmu,
                             GrTextureFilterMode_t minFilterMode,
                             GrTextureFilterMode_t magFilterMode ) {
PEXECBUFF;

        GetExecBuffPointer(4);
        execbuff[0] = GRTEXFILTERMODE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) minFilterMode;
        execbuff[3] = (unsigned long) magFilterMode;
        ca->ExeCodeIndex += 4;
}


void EXPORT grTexClampMode( GrChipID_t tmu,
                            GrTextureClampMode_t sClampMode,
                            GrTextureClampMode_t tClampMode ) {
PEXECBUFF;

        GetExecBuffPointer(4);
        execbuff[0] = GRTEXCLAMPMODE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) sClampMode;
        execbuff[3] = (unsigned long) tClampMode;
        ca->ExeCodeIndex += 4;
}


void EXPORT grTexLodBiasValue( GrChipID_t tmu, float bias ) {
PEXECBUFF;

        GetExecBuffPointer(4);
        execbuff[0] = GRTEXLODBIASVALUE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = *((unsigned long *) &bias);
        ca->ExeCodeIndex += 4;
}


FxU32 EXPORT grTexMinAddress( GrChipID_t tmu ) {

        return(0);
}


FxU32 EXPORT grTexMaxAddress( GrChipID_t tmu ) {

        return(config.TexMemSize);
}


FxU32 EXPORT grTexCalcMemRequired( GrLOD_t smallLod, GrLOD_t largeLod,
                                   GrAspectRatio_t aspect, GrTextureFormat_t format ) {
unsigned int asprat,x,y,l;

        l = GlideGetLOD(largeLod);
        x = GlideGetXY(largeLod,aspect);
        y = x & 0xFFFF;
		x >>= 16;
        asprat = GlideGetLOD(smallLod);
        x = y = x * y * GlideGetTexBYTEPP(format);
        while(l != asprat)        {
                x >>= 2;
				y += x;
				l >>= 1;
        }
        return((y + 7) & (~7) );
}


FxU32 EXPORT grTexTextureMemRequired( FxU32 evenOdd, GrTexInfo *info ) {

        return( grTexCalcMemRequired( info->smallLod, info->largeLod, info->aspectRatio,
                                      info->format ) );
}


/* texture utility functions */

GrMipMapId_t EXPORT guTexAllocateMemory(GrChipID_t tmu,
										FxU8 evenOddMask,
										int width, int height,
										GrTextureFormat_t format,
										GrMipMapMode_t mmMode,
										GrLOD_t smallLod, GrLOD_t largeLod,
										GrAspectRatio_t aspectRatio,
										GrTextureClampMode_t sClampMode,
										GrTextureClampMode_t tClampMode,
										GrTextureFilterMode_t minFilterMode,
										GrTextureFilterMode_t magFilterMode,
										float lodBias,
                                        FxBool lodBlend )       {
PEXECBUFF;

        if (!canrun) return(GR_NULL_MIPMAP_HANDLE);
        if (mipmapInfos == NULL)
                if ( (mipmapInfos = (GrMipMapInfo *) DPMIGetMem(MAX_UTIL_TEXTURES * sizeof(GrMipMapInfo))) == NULL) return(GR_NULL_MIPMAP_HANDLE);
        GetExecBuffPointer(16);
        execbuff[0] = GUTEXALLOCATEMEMORY;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) evenOddMask;
        execbuff[3] = (unsigned long) width;
        execbuff[4] = (unsigned long) height;
        execbuff[5] = (unsigned long) format;
        execbuff[6] = (unsigned long) mmMode;
        execbuff[7] = (unsigned long) smallLod;
        execbuff[8] = (unsigned long) largeLod;
        execbuff[9] = (unsigned long) aspectRatio;
        execbuff[10] = (unsigned long) sClampMode;
        execbuff[11] = (unsigned long) tClampMode;
        execbuff[12] = (unsigned long) minFilterMode;
        execbuff[13] = (unsigned long) magFilterMode;
		execbuff[14] = * (unsigned long *) &lodBias;
        execbuff[15] = (unsigned long) lodBlend;
        ca->ExeCodeIndex += 16;
        FlushExecBuffer();
        return( (GrMipMapId_t) execbuff[0] );
}


FxBool EXPORT guTexChangeAttributes( GrMipMapId_t mmid,
					int width, int height,
					GrTextureFormat_t format,
					GrMipMapMode_t mmMode,
					GrLOD_t smallLod, GrLOD_t largeLod,
					GrAspectRatio_t aspectRatio,
					GrTextureClampMode_t sClampMode,
					GrTextureClampMode_t tClampMode,
					GrTextureFilterMode_t minFilterMode,
                    GrTextureFilterMode_t magFilterMode)    {
PEXECBUFF;

        if (!canrun) return(FXFALSE);
        GetExecBuffPointer(13);
        execbuff[0] = GUTEXCHANGEATTRIBUTES;
        execbuff[1] = (unsigned long) mmid;
        execbuff[2] = (unsigned long) width;
        execbuff[3] = (unsigned long) height;
        execbuff[4] = (unsigned long) format;
        execbuff[5] = (unsigned long) mmMode;
        execbuff[6] = (unsigned long) smallLod;
        execbuff[7] = (unsigned long) largeLod;
        execbuff[8] = (unsigned long) aspectRatio;
        execbuff[9] = (unsigned long) sClampMode;
        execbuff[10] = (unsigned long) tClampMode;
        execbuff[11] = (unsigned long) minFilterMode;
        execbuff[12] = (unsigned long) magFilterMode;
        ca->ExeCodeIndex += 13;
        FlushExecBuffer();
        return( (FxBool) execbuff[0] );
}


void EXPORT guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )  {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GUTEXCOMBINEFUNCTION;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) func;
        ca->ExeCodeIndex += 3;
}


void EXPORT grTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )  {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GUTEXCOMBINEFUNCTION;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) func;
        ca->ExeCodeIndex += 3;
}


void EXPORT grTexDownloadTable( GrChipID_t tmu, GrTexTable_t type, void *data ) {
PEXECBUFF;
        
        CheckForRoom(1024);
        GetExecBuffPointer(4);
        execbuff[0] = GRTEXDOWNLOADTABLE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) type;
        execbuff[3] = (unsigned long) GetFDPtr();
        ca->ExeCodeIndex += 4;
        CopyData(data, 1024);
}


void EXPORT grTexDownloadTablePartial( GrChipID_t tmu, GrTexTable_t type, void *data,
                                                                int start, int end ) {
PEXECBUFF;

        CheckForRoom(1024);
        GetExecBuffPointer(6);
        execbuff[0] = GRTEXDOWNLOADTABLE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) type;
        execbuff[3] = (unsigned long) GetFDPtr();
        execbuff[4] = (unsigned long) start;
        execbuff[5] = (unsigned long) end;
        ca->ExeCodeIndex += 6;
        CopyData(data, (end - start + 1) * 4);
}


void EXPORT grTexNCCTable( GrChipID_t tmu, GrNCCTable_t table ) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GRTEXNCCTABLE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) table;
        ca->ExeCodeIndex += 3;
}


void EXPORT grTexMultibase( GrChipID_t tmu, FxBool enable ) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GRTEXMULTIBASE;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) enable;
        ca->ExeCodeIndex += 3;
        
}


void EXPORT grTexMultibaseAddress( GrChipID_t tmu, GrTexBaseRange_t range,
                                   FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info ) {
PEXECBUFF;

        GetExecBuffPointer(6);
        execbuff[0] = GRTEXMULTIBASEADDRESS;
        execbuff[1] = (unsigned long) tmu;
        execbuff[2] = (unsigned long) range;
        execbuff[3] = (unsigned long) startAddress;
        execbuff[4] = (unsigned long) evenOdd;
        execbuff[5] = (unsigned long) GetFDPtr();
        CopyData(info, sizeof(GrTexInfo) );
        ca->ExeCodeIndex += 6;
}


void EXPORT guTexDownloadMipMap( GrMipMapId_t mmid, const void *src,
                                  const GuNccTable *nccTable )  {
PEXECBUFF;
register int vdd;
unsigned int copylen;

        GetExecBuffPointer(4);
        vdd = (config.Flags & CFG_NTVDDMODE) && (src >= sap);
        if (!vdd)       {
                execbuff[0] = GLIDEGETUTEXTURESIZE;
                execbuff[1] = (unsigned long) mmid;
                execbuff[3] = (unsigned long) 0;
                ca->ExeCodeIndex += 4;
                FlushExecBuffer();
                copylen = execbuff[0];
        }
        execbuff = ca->ExeCodes+ca->ExeCodeIndex;
        execbuff[0] = GUTEXDOWNLOADMIPMAP;
        execbuff[1] = (unsigned long) mmid;
        execbuff[3] = (unsigned long) nccTable;
        execbuff[2] = (unsigned long) src;
        if (!vdd)       {
                execbuff[2] = (unsigned long) GetFDPtr();
                CopyData(src, copylen);
        }
        ca->ExeCodeIndex += 4;
        FlushExecBuffer();
}


void EXPORT guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src ) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
const void		*psrc;
void			**paramsrc;

        GetExecBuffPointer(4);
        vdd = (config.Flags & CFG_NTVDDMODE) && ( (*src) >= sap );
        execbuff[0] = GLIDEGETUTEXTURESIZE;
        execbuff[1] = (unsigned long) mmid;
        execbuff[2] = (unsigned long) lod;
        execbuff[3] = (unsigned long) 1;
        ca->ExeCodeIndex += 4;
        FlushExecBuffer();
        copylen = execbuff[0];

        execbuff = ca->ExeCodes+ca->ExeCodeIndex;
        execbuff[0] = GUTEXDOWNLOADMIPMAPLEVEL;
        execbuff[1] = (unsigned long) mmid;
        execbuff[2] = (unsigned long) lod;
        psrc = *src;
        if (!vdd)       {
			psrc = (void *) GetFDPtr();
			CopyData(*src, copylen);
        }
        execbuff[3] = (unsigned long) GetFDPtr();
        paramsrc = (void **) GetFDPtr();
        *paramsrc = psrc;
        ca->ExeCodeIndex += 4;
        FlushExecBuffer();
        ((char *) (*src)) += copylen;
}


void EXPORT guTexSource( GrMipMapId_t mmid ) {
PEXECBUFF;

        actutex = mmid;
        GetExecBuffPointer(2);
        execbuff[0] = GUTEXSOURCE;
        execbuff[1] = (unsigned long) mmid;
        ca->ExeCodeIndex += 2;
}


GrMipMapId_t EXPORT guTexGetCurrentMipMap ( GrChipID_t tmu ) {

        return( actutex );
}


void EXPORT guTexMemReset( void ) {
PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GUTEXMEMRESET;
        ca->ExeCodeIndex++;
}


FxU32 EXPORT guTexMemQueryAvail( GrChipID_t tmu ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GUTEXMEMQUERYAVAIL;
        execbuff[1] = (unsigned long) tmu;
        ca->ExeCodeIndex += 2;
        FlushExecBuffer();
        return( (FxU32) execbuff[0] );
}


GrMipMapInfo *EXPORT guTexGetMipMapInfo( GrMipMapId_t mmid ) {
PEXECBUFF;
void	*FDPtr;

        CheckForRoom(sizeof(GrMipMapInfo));
        FDPtr = ca->FDPtr;
        GetExecBuffPointer(3);
        execbuff[0] = GUTEXGETMIPMAPINFO;
        execbuff[1] = (unsigned long) GetFDPtr();
        execbuff[2] = (unsigned long) mmid;
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        if (execbuff[0])        {
                CopyMem(mipmapInfos + mmid, FDPtr, sizeof(GrMipMapInfo));
                return(mipmapInfos + mmid);
        } else return(NULL);
}


void EXPORT guColorCombineFunction( GrColorCombineFnc_t fnc ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GUCOLORCOMBINEFUNCTION;
        execbuff[1] = (unsigned long) fnc;
        ca->ExeCodeIndex += 2;
}


FxU16 EXPORT guEndianSwapBytes(FxU16 value)    {

		return( ( (value & 0xFF00) >> 8) | ( (value && 0xFF) << 8) );
}


FxU32 EXPORT guEndianSwapWords(FxU32 value)    {

        return( ( (value & 0xFFFF0000) >> 16) | ( (value && 0xFFFF) << 16) );
}


void EXPORT guAlphaSource( GrAlphaSource_t mode )       {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GUALPHASOURCE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


FxU32 EXPORT grSstScreenHeight( void ) {

        return(screenHeight);
}


FxU32 EXPORT grSstScreenWidth( void ) {

        return(screenWidth);
}


void EXPORT grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy ) {
PEXECBUFF;

        GetExecBuffPointer(5);
        execbuff[0] = GRCLIPWINDOW;
        execbuff[1] = (unsigned long) minx;
        execbuff[2] = (unsigned long) miny;
        execbuff[3] = (unsigned long) maxx;
        execbuff[4] = (unsigned long) maxy;
        ca->ExeCodeIndex += 5;
}


void EXPORT grDepthBufferMode( GrDepthBufferMode_t mode ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRDEPTHBUFFERMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grDepthBufferFunction( GrCmpFnc_t func ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRDEPTHBUFFERFUNCTION;
        execbuff[1] = (unsigned long) func;
        ca->ExeCodeIndex += 2;
}


void EXPORT grDepthMask( FxBool enable ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRDEPTHMASK;
        execbuff[1] = (unsigned long) enable;
        ca->ExeCodeIndex += 2;
}


void EXPORT grDepthBiasLevel( FxI16 level )     {
PEXECBUFF;

        GetExecBuffPointer(1);
        execbuff[0] = GRDEPTHBIASLEVEL;
		execbuff[1] = * (unsigned long *) &level;
        ca->ExeCodeIndex += 2;
}


#ifndef GLIDE1

FxBool EXPORT grLfbLock(GrLock_t type, 
						GrBuffer_t buffer, 
						GrLfbWriteMode_t writeMode, 
						GrOriginLocation_t origin, 
						FxBool pixelPipeline, 
						GrLfbInfo_t *info) {
PEXECBUFF;
void *FDPtr;

        CheckForRoom(sizeof(GrLfbInfo_t));
        GetExecBuffPointer(7);
        execbuff[0] = GRLFBLOCK;
        execbuff[1] = (unsigned long) type;
        execbuff[2] = (unsigned long) buffer;
        execbuff[3] = (unsigned long) writeMode;
        execbuff[4] = (unsigned long) origin;
        execbuff[5] = (unsigned long) pixelPipeline;
        FDPtr = ca->FDPtr;
        execbuff[6] = (unsigned long) GetFDPtr();
        ca->ExeCodeIndex += 7;
        FlushExecBuffer();
        CopyMem(info, FDPtr, sizeof(GrLfbInfo_t));
//        _asm fgh: jmp fgh
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT grLfbUnlock( GrLock_t type, GrBuffer_t buffer ) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0]=GRLFBUNLOCK;
        execbuff[1]=(unsigned long) type;
        execbuff[2]=(unsigned long) buffer;
        ca->ExeCodeIndex+=3;
        FlushExecBuffer();
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT grLfbReadRegion(	GrBuffer_t src_buffer, 
								FxU32 src_x, FxU32 src_y, 
								FxU32 src_width, FxU32 src_height, 
								FxU32 dst_stride, void *dst_data ) {
PEXECBUFF;
register int vdd;
unsigned int copylen;
void *FDPtr;

        GetExecBuffPointer(8);
        execbuff[7] = (unsigned long) dst_data;
        vdd = (config.Flags & CFG_NTVDDMODE) && (dst_data>=sap);
        if (!vdd)       {
                CheckForRoom(copylen = src_height * dst_stride);
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                FDPtr = ca->FDPtr;
                execbuff[7] = (unsigned long) GetFDPtr();
        }
        execbuff[0] = GRLFBREADREGION;
        execbuff[1] = (unsigned long) src_buffer;
        execbuff[2] = (unsigned long) src_x;
        execbuff[3] = (unsigned long) src_y;
        execbuff[4] = (unsigned long) src_width;
        execbuff[5] = (unsigned long) src_height;
        execbuff[6] = (unsigned long) dst_stride;
        ca->ExeCodeIndex += 8;
        FlushExecBuffer();
        if (!vdd) CopyMem(dst_data, FDPtr, copylen);
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT grLfbWriteRegion( GrBuffer_t dst_buffer, 
								FxU32 dst_x, FxU32 dst_y, 
								GrLfbSrcFmt_t src_format, 
								FxU32 src_width, FxU32 src_height, 
								FxU32 src_stride, void *src_data ) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(9);
        execbuff[8] = (unsigned long) src_data;
        vdd = (config.Flags & CFG_NTVDDMODE) && (src_data >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = src_height * src_stride);
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                CopyData(src_data, copylen);
                execbuff[8] = (unsigned long) GetFDPtr();
        }
        execbuff[0] = GRLFBWRITEREGION;
        execbuff[1] = (unsigned long) dst_buffer;
        execbuff[2] = (unsigned long) dst_x;
        execbuff[3] = (unsigned long) dst_y;
        execbuff[4] = (unsigned long) src_format;
        execbuff[5] = (unsigned long) src_width;
        execbuff[6] = (unsigned long) src_height;
        execbuff[7] = (unsigned long) src_stride;
        ca->ExeCodeIndex += 9;
        FlushExecBuffer();
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT grSstControl( FxU32 mode) {
PEXECBUFF;

        if (!canrun) return(FXFALSE);
        GetExecBuffPointer(2);
        execbuff[0] = GRSSTCONTROLMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
		FlushExecBuffer();
        return( (FxBool) execbuff[0] );
}


#else


void EXPORT grLfbBypassMode( GrLfbBypassMode_t mode )   {
PEXECBUFF;

        if (!canrun) return;
        GetExecBuffPointer(2);
        execbuff[0] = GRLFBBYPASSMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grLfbOrigin(GrOriginLocation_t origin)      {
PEXECBUFF;

        if (!canrun) return;
        GetExecBuffPointer(2);
        execbuff[0] = GRLFBORIGIN;
        execbuff[1] = (unsigned long) origin;
        ca->ExeCodeIndex += 2;
}


void EXPORT grLfbWriteMode( GrLfbWriteMode_t mode )     {
PEXECBUFF;

        if (!canrun) return;
        GetExecBuffPointer(2);
        execbuff[0] = GRLFBWRITEMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grLfbBegin()        {
PEXECBUFF;

        if (!canrun) return;
        GetExecBuffPointer(1);
        execbuff[0] = GRLFBBEGIN;
        ca->ExeCodeIndex++;
}


void EXPORT grLfbEnd()  {
PEXECBUFF;

        if (!canrun) return;
        GetExecBuffPointer(1);
        execbuff[0] = GRLFBEND;
        ca->ExeCodeIndex++;
        FlushExecBuffer();
}


unsigned int EXPORT grLfbGetReadPtr(GrBuffer_t buffer ) {
PEXECBUFF;

        if (!canrun) return(NULL);
        GetExecBuffPointer(2);
        execbuff[0] = GRLFBGETREADPTR;
        execbuff[1] = (unsigned long) buffer;
        ca->ExeCodeIndex += 2;
        FlushExecBuffer();
        return( (unsigned int) execbuff[0]);
}


unsigned int EXPORT grLfbGetWritePtr(GrBuffer_t buffer )        {
PEXECBUFF;

        if (!canrun) return(NULL);
        GetExecBuffPointer(2);
        execbuff[0] = GRLFBGETWRITEPTR;
        execbuff[1] = (unsigned long) buffer;
        ca->ExeCodeIndex += 2;
        FlushExecBuffer();
        return((unsigned int) execbuff[0]);
}


void EXPORT guFbReadRegion(	FxU32 src_x, FxU32 src_y, 
							FxU32 w, FxU32 h,
							void *dst, const int strideInBytes) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(7);
        execbuff[5] = (unsigned long) dst;
        vdd=(config.Flags & CFG_NTVDDMODE) && (dst >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = h * strideInBytes);
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                FDPtr = ca->FDPtr;
                execbuff[5] = (unsigned long) GetFDPtr();
        }
        execbuff[0] = GUFBREADREGION;
        execbuff[1] = (unsigned long) src_x;
        execbuff[2] = (unsigned long) src_y;
        execbuff[3] = (unsigned long) w;
        execbuff[4] = (unsigned long) h;
        execbuff[6] = (unsigned long) strideInBytes;
        ca->ExeCodeIndex += 7;
        FlushExecBuffer();
        if (!vdd) CopyMem(dst, FDPtr, copylen);
        return;
}


FxBool EXPORT guFbWriteRegion(	FxU32 dst_x, FxU32 dst_y, 
								FxU32 w, FxU32 h,
								void *src, const int strideInBytes) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(7);
        execbuff[5] = (unsigned long) src;
        vdd=(config.Flags & CFG_NTVDDMODE) && (src >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = h * strideInBytes);
                execbuff = ca->ExeCodes+ca->ExeCodeIndex;
                CopyData(src, copylen);
                execbuff[5] = (unsigned long) GetFDPtr();
        }
        execbuff[0] = GUFBWRITEREGION;
        execbuff[1] = (unsigned long) dst_x;
        execbuff[2] = (unsigned long) dst_y;
        execbuff[3] = (unsigned long) w;
        execbuff[4] = (unsigned long) h;
        execbuff[6] = (unsigned long) strideInBytes;
        ca->ExeCodeIndex += 7;
        FlushExecBuffer();
        return;
}


#endif


void EXPORT grLfbWriteColorSwizzle(FxBool swizzleBytes, FxBool swapWords) {
PEXECBUFF;

        GetExecBuffPointer(3);
        execbuff[0] = GRLFBWRITECOLORSWIZZLE;
        execbuff[1] = (unsigned long) swizzleBytes;
        execbuff[2] = (unsigned long) swapWords;
        ca->ExeCodeIndex += 3;
}


void EXPORT grLfbWriteColorFormat(GrColorFormat_t colorFormat)  {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRLFBWRITECOLORFORMAT;
        execbuff[1] = (unsigned long) colorFormat;
        ca->ExeCodeIndex += 2;
}


void EXPORT grLfbConstantAlpha( GrAlpha_t alpha )       {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRLFBCONSTANTALPHA;
        execbuff[1] = (unsigned long) alpha;
        ca->ExeCodeIndex += 2;
}


void EXPORT grChromakeyMode( GrChromakeyMode_t mode ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRCHROMAKEYMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grChromakeyValue( GrColor_t value ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRCHROMAKEYVALUE;
        execbuff[1] = (unsigned long) value;
        ca->ExeCodeIndex += 2;
}


FxBool EXPORT gu3dfGetInfo( const char *_3dffilename, Gu3dfInfo *info ) {
PEXECBUFF;
void	*FDPtr;

        if (!canrun) return(FXFALSE);
        if (!GetCanonicFileName(_3dffilename)) return(FXFALSE);
        CheckForRoom(256 + sizeof(Gu3dfInfo));
        GetExecBuffPointer(3);
        execbuff[0] = GU3DFGETINFO;
        execbuff[1] = (unsigned long) GetFDPtr(); //filename;
        CopyData(filename, 256);
        execbuff[2] = (unsigned long) GetFDPtr(); //info;
        FDPtr = ca->FDPtr;
        CopyData(info, sizeof(Gu3dfInfo));
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        CopyMem(info, FDPtr, sizeof(Gu3dfInfo));
        return( (FxBool) execbuff[0] );
}


FxBool EXPORT gu3dfLoad( const char *_3dffilename, Gu3dfInfo *info )        {
PEXECBUFF;
register int	vdd;
Gu3dfInfo		*_3dfinfo;
void *FDPtr,	*dataptr;
unsigned int	copylen;

        if (!canrun) return(FXFALSE);
        if (!GetCanonicFileName(_3dffilename)) return(FXFALSE);
        copylen = GetTextureSize(info->header.small_lod,info->header.large_lod,
								info->header.aspect_ratio, info->header.format);
        vdd = (config.Flags & CFG_NTVDDMODE) && (info->data >= sap);
        if (vdd) copylen = 0;
        CheckForRoom(256 + sizeof(Gu3dfInfo) + copylen);
        GetExecBuffPointer(3);
        execbuff[0] = GU3DFLOAD;
        execbuff[1] = (unsigned long) GetFDPtr(); //filename;
        CopyData(filename, 256);
        execbuff[2] = (unsigned long) GetFDPtr(); //info;
        _3dfinfo = (Gu3dfInfo *) ca->FDPtr;
        CopyData(info, sizeof(Gu3dfInfo));
        if (!vdd)       {
                dataptr = info->data;
                _3dfinfo->data = (void *) GetFDPtr();
                FDPtr = ca->FDPtr;
        }
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        CopyMem(info, _3dfinfo, sizeof(Gu3dfInfo));
        if (!vdd)       {
                CopyMem(dataptr, FDPtr, copylen);
                info->data = dataptr;
        }
        return( (FxBool) execbuff[0] );
}


void EXPORT grFogColorValue( GrColor_t value ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRFOGCOLORVALUE;
        execbuff[1] = (unsigned long) value;
        ca->ExeCodeIndex += 2;
}


void EXPORT grFogMode( GrFogMode_t mode ) {
PEXECBUFF;

        GetExecBuffPointer(2);
        execbuff[0] = GRFOGMODE;
        execbuff[1] = (unsigned long) mode;
        ca->ExeCodeIndex += 2;
}


void EXPORT grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] ) {
PEXECBUFF;

        CheckForRoom(GR_FOG_TABLE_SIZE*sizeof(GrFog_t));
        GetExecBuffPointer(2);
        execbuff[0] = GRFOGTABLE;
        execbuff[1] = (unsigned long) GetFDPtr();
        CopyData(table, GR_FOG_TABLE_SIZE * sizeof(GrFog_t));
        ca->ExeCodeIndex += 2;
}


void EXPORT guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(3);
        execbuff[1] = (unsigned long) fogTable;
        vdd = (config.Flags & CFG_NTVDDMODE) && (fogTable >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = GR_FOG_TABLE_SIZE * sizeof(GrFog_t));
                execbuff = ca->ExeCodes + ca->ExeCodeIndex;
                execbuff[1] = (unsigned long) GetFDPtr();
                FDPtr = ca->FDPtr;
        }
        execbuff[0] = GUFOGGENERATEEXP;
		execbuff[2] = * (unsigned long *) &density;
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        if (!vdd) CopyMem(fogTable, FDPtr, copylen);
}


void EXPORT guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density ) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(3);
        execbuff[1] = (unsigned long) fogTable;
        vdd = (config.Flags & CFG_NTVDDMODE) && (fogTable >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = GR_FOG_TABLE_SIZE * sizeof(GrFog_t));
                execbuff = ca->ExeCodes + ca->ExeCodeIndex;
                execbuff[1] = (unsigned long) GetFDPtr();
                FDPtr = ca->FDPtr;
        }
        execbuff[0] = GUFOGGENERATEEXP2;
		execbuff[2] = * (unsigned long *) &density;
        ca->ExeCodeIndex += 3;
        FlushExecBuffer();
        if (!vdd) CopyMem(fogTable, FDPtr, copylen);
}


void EXPORT guFogGenerateLinear( GrFog_t fogTable[GR_FOG_TABLE_SIZE],
                                 float nearW, float farW ) {
PEXECBUFF;
register int	vdd;
unsigned int	copylen;
void			*FDPtr;

        GetExecBuffPointer(4);
        execbuff[1] = (unsigned long) fogTable;
        vdd = (config.Flags & CFG_NTVDDMODE) && (fogTable >= sap);
        if (!vdd)       {
                CheckForRoom(copylen = GR_FOG_TABLE_SIZE * sizeof(GrFog_t));
                execbuff = ca->ExeCodes + ca->ExeCodeIndex;
                execbuff[1] = (unsigned long) GetFDPtr();
                FDPtr = ca->FDPtr;
        }
        execbuff[0] = GUFOGGENERATELINEAR;
		execbuff[2] = * (unsigned long *) &nearW;
        execbuff[3] = * (unsigned long *) &farW;
        ca->ExeCodeIndex += 4;
        FlushExecBuffer();
        if (!vdd) CopyMem(fogTable, FDPtr, copylen);
}


float EXPORT guFogTableIndexToW( int i ) {

		return(IndToW[i]);
}


FxBool EXPORT grSstDetectResources()    {

		return(FXTRUE);
}
