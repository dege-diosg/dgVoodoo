/*--------------------------------------------------------------------------------- */
/* dgVoodooGlide.h - Glide internal definitions, structures                         */
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

#ifndef		DGVOODOOGLIDE_H
#define		DGVOODOOGLIDE_H

#include	"dgVoodooBase.h"
#include	<windows.h>
#include	<glide.h>
//#include	"resource.h"
#include	"DDraw.h"
#include	"D3d.h"
#include	"movie.h"


/*------------------------------------------------------------------------------------------*/
/*............................ Defin�ci�k az egyes �llapotokhoz ............................*/

#define MAX_TEXSTAGES				4				/* Direct3D-ben a haszn�lt text�ra stage-ek max sz�ma */

/* k�l�nb�z� �llapotflagek */
#define STATE_NATIVECKMETHOD		0x1				/* Nat�v colorkeying haszn�lata (egy�bk�nt alfa alap�) */
#define STATE_COLORKEYTNT			0x2				/* TNT-colorkeying �ppen haszn�latban (nat�v ck eset�n) */
#define STATE_ALPHACKUSED			0x4				/* Alfa alap� colorkeying �ppen haszn�latban (alpha based eset�n) */
#define STATE_CCACSTATECHANGED		0x8				/* A color vagy alpha combine f�ggv�ny megv�ltozott */
#define STATE_COLORTEXTUREUSED		0x10			/* A color combining haszn�lja a text�r�t is */
#define STATE_ALPHATEXTUREUSED		0x20			/* Az alpha combining haszn�lja a text�r�t is */
#define STATE_COLORDIFFUSEUSED		0x40			/* A color combining haszn�lja a diffuse komponenst */
#define STATE_ALPHADIFFUSEUSED		0x80			/* Az alpha combining haszn�lja a diffuse komponenst */
#define STATE_APMULTITEXTURING		0x100			/* Az alpha/paletta multitext�r�zva */
#define STATE_TCLOCALALPHA			0x200			/* A texture combining local alpha-t eredm�nyez */
#define STATE_TCCINVERT				0x400			/* A texture combining inverz color-t k�pez */
#define STATE_TCAINVERT				0x800			/* A texture combining inverz alpha-t k�pez */
#define STATE_USETMUW				0x1000			/* Z-buffer �s t�bl�s k�d eset�n a tmu w-t haszn�ljuk */
#define STATE_DELTA0				0x2000			/* Delta0-m�d: konstans iter�lt rgb */
#define STATE_COMPAREDEPTH			0x4000
#define STATE_ALPHABLENDCHANGED		0x8000			/* Alpha blending f�ggv�ny megv�ltozott */
#define STATE_ALPHATESTCHANGED		0x10000			/* Alpha testing f�ggv�ny megv�ltozott */

#define STATE_TEXTURE0				0x20000
#define STATE_TEXTURE1				0x40000
#define STATE_TEXTURE2				0x80000
#define STATE_TEXTURE3				0x100000
#define STATE_TEXTUREUSAGEMASK		(STATE_TEXTURE0 | STATE_TEXTURE1 | STATE_TEXTURE2 | STATE_TEXTURE3)

/* A Glide aktu�lis fut�sk�pes �llapot�t t�kr�z� flagek */
#define RF_INITOK					0x1				/* Az init siker�lt */
#define RF_SCENECANBEDRAWN			0x2				/* A scene-t meg lehet rajzolni (cooperative level ok, BeginScene ok) */
#define RF_CANFULLYRUN				0x3				/* Ha az el�z� k�t flag egyszerre be van �ll�tva, a scene rajzol�sa mehet */
#define RF_WINDOWCREATED			0x4				/* Windows platform: a wrapper l�trehozott egy ablakot az alk. sz�m�ra */

/* call flags: a primit�vek megrajzol�sa el�tt megh�vand� f�ggv�nyekhez */
#define CALLFLAG_COLORCOMBINE		0x1
#define CALLFLAG_ALPHACOMBINE		0x2
#define CALLFLAG_SETPALETTE			0x4
#define CALLFLAG_SETNCCTABLE		0x8
#define CALLFLAG_SETCOLORKEY		0x10
#define CALLFLAG_SETCOLORKEYSTATE	0x20
#define CALLFLAG_SETTEXTURE			0x40
#define CALLFLAG_RESETTEXTURE		0x80


#define	STATEDESC_DELIMITER			0xEEEEEEEE

/*------------------------------------------------------------------------------------------*/
/*....................................... Strukt�r�k .......................................*/


/* Saj�t GrState strukt�ra az eredeti helyett: nem lehet t�bb 320 b�jtn�l! */
typedef struct {

	void					*acttex;
	unsigned int			actmmid;
	LPDIRECTDRAWSURFACE7	lpDDTCCTex, lpDDTCATex;
	LPDIRECTDRAWSURFACE7	lpDDTex[4];
	FxU32					hints;
	GrOriginLocation_t		locOrig;					/* Glide render orig� */
	unsigned int			flags, flagsold;
	float					divx, divy;
	DWORD					colorOp[MAX_TEXSTAGES];
	DWORD					alphaOp[MAX_TEXSTAGES];
	DWORD					colorArg1[MAX_TEXSTAGES];
	DWORD					alphaArg1[MAX_TEXSTAGES];
	DWORD					colorArg2[MAX_TEXSTAGES];
	DWORD					alphaArg2[MAX_TEXSTAGES];
	DWORD					constColorValue, delta0Rgb;
	DWORD					colorMask;
	DWORD					alocal, aother;
	DWORD					mipMapMode;
	unsigned char			srcBlend, dstBlend, alphaBlendEnable, colorKeyEnable;
	DWORD					alpharef, alphatestfunc;
	DWORD					depthbuffmode, zfunc, zenable;
	int						depthbiaslevel;
	DWORD					magfilter,minfilter;
	DWORD					clampmodeu, clampmodev;
	float					lodBias;
	DWORD					cullmode, gcullmode, vertexmode;
	FxU32					minx, maxx, miny, maxy;
	DWORD					perspective, colorkey;
	DWORD					fogmode, fogcolor, fogmodifier;
	FxU32					startAddress[4];
	unsigned char			smallLod, largeLod, aspectRatio, format;
	unsigned char			cfuncp;						/* grColorCombine param�terei */
	unsigned char			cfactorp;
	unsigned char			clocalp;
	unsigned char			cotherp;
	unsigned char			cinvertp;
	unsigned char			afuncp;						/* grAlphaCombine param�terei */
	unsigned char			afactorp;
	unsigned char			alocalp;					/* Az alocal fontos a grColorCombine-hoz is */
	unsigned char			aotherp;
	unsigned char			ainvertp;	
	unsigned char			astagesnum, cstagesnum;
	unsigned char			rgb_function;				/* a grTexCombine param�terei */
    unsigned char			rgb_factor;
    unsigned char			alpha_function;
    unsigned char			alpha_factor;
    unsigned char			rgb_invert;
    unsigned char			alpha_invert;
	
} _GrState, _GlideActState;


/* Strukt�ra a statisztik�khoz: egyel�re csak a feldolgozott h�romsz�gek sz�m�t t�rolja */
typedef struct {

	unsigned int			trisProcessed;

} _stat;

/*------------------------------------------------------------------------------------------*/
/*.................................. Glob�lis v�ltoz�k .....................................*/


extern	LPDIRECTDRAW7				lpDD;
extern	LPDIRECT3D7					lpD3D;
extern	LPDIRECT3DDEVICE7			lpD3Ddevice;
extern	LPDIRECTDRAWGAMMACONTROL	lpDDGamma;
extern	LPDIRECTDRAWSURFACE7		lpDDFrontBuff;
extern	LPDIRECTDRAWSURFACE7		lpDDBackBuff;

extern	GrColorFormat_t				cFormat;
extern	unsigned int				buffswapnum;
extern	unsigned char				*WToInd;

extern	unsigned int				callflags;
extern	unsigned int				runflags;
extern	int							renderbuffer;

extern	_GlideActState				astate;
extern	_stat						stats;

extern	unsigned int				appXRes, appYRes;
extern	int							drawresample;

extern	float						depthBias_Z;
extern	int							depthBias_W;
extern	float						depthBias_cz, depthBias_cw;

extern	float						xScale, yScale;
extern	float						xConst, yConst;

extern	DWORD						cmpFuncLookUp[];

extern	float						IndToW[GR_FOG_TABLE_SIZE+1];
extern	GrFog_t						fogtable[GR_FOG_TABLE_SIZE+1];


/*------------------------------------------------------------------------------------------*/
/*............................. Bels� f�ggv�nyek predeklar�ci�ja ...........................*/

void			GlideAlphaCombineUpdateVertexMode();
void			GlideUpdateConstantColorKeyingState();
void			GlideColorCombineUpdateVertexMode();
unsigned long	GlideGetColor(GrColor_t color);
void			GlideGetD3DState(DWORD *);
void			GlideSetD3DState(DWORD *);
void			GlideSave3DStates (DWORD *states, DWORD *saveArea);
void			GlideSave3DStageStates (DWORD *stateDesc, DWORD *saveArea);
void			GlideSave3DTextures (DWORD *stageDesc, DWORD *texHandles);
void			GlideRestore3DStates (DWORD *states, DWORD *saveArea);
void			GlideRestore3DStageStates (DWORD *stateDesc, DWORD *saveArea);
void			GlideRestore3DTextures (DWORD *stageDesc, DWORD *texHandles);
int				GlideSelectAndCreate3DDevice();
void			GlideGetIndToWTable(float *table);
void			GlideAlphaCombine();
void			GlideColorCombine();


/*------------------------------------------------------------------------------------------*/
/*............................. Glide-f�ggv�nyek predeklar�ci�ja ...........................*/


void	EXPORT	grGlideGetVersion(char version[80]);
FxBool	EXPORT	grSstQueryBoards(GrHwConfiguration *hwConfig);
FxBool	EXPORT	grSstQueryHardware(GrHwConfiguration *hwConfig);
void	EXPORT	grGlideInit();
void	EXPORT	grSstSelect(int whichSST);
void	EXPORT	grBufferClear(GrColor_t color, GrAlpha_t alpha, FxU16 depth);
void	EXPORT	grBufferSwap(int swapinterval);
int		EXPORT	grBufferNumPending( void );
void	EXPORT	grConstantColorValue( GrColor_t color );
void	EXPORT	grConstantColorValue4(float a, float r, float g, float b);
void	EXPORT	grColorMask( FxBool rgb, FxBool alpha );
void	EXPORT	grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy );
void	EXPORT	grGlideGetState( _GrState *state );
void	EXPORT	grGlideSetState( _GrState *state );
void	EXPORT	grAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df,
									 GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df );
void	EXPORT	grColorCombine(	GrCombineFunction_t func, 
								GrCombineFactor_t factor,
								GrCombineLocal_t local, 
								GrCombineOther_t other, 
								FxBool invert );
void	EXPORT	grAlphaCombine(	GrCombineFunction_t func, 
								GrCombineFactor_t factor,
								GrCombineLocal_t local, 
								GrCombineOther_t other, 
								FxBool invert);
void	EXPORT	guColorCombineFunction( GrColorCombineFnc_t func );
void	EXPORT	guAlphaSource( GrAlphaSource_t mode );
FxU32	EXPORT	guEndianSwapWords(FxU32 value);
FxU16	EXPORT	guEndianSwapBytes(FxU16 value);
void	EXPORT	grGammaCorrectionValue( float value );
void	EXPORT	grAlphaTestFunction( GrCmpFnc_t function );
void	EXPORT	grAlphaTestReferenceValue( GrAlpha_t value );
void	EXPORT	grDitherMode( GrDitherMode_t mode );
void	EXPORT	grDisableAllEffects( void );
void	EXPORT	grCullMode( GrCullMode_t mode );
void	EXPORT	grHints( GrHint_t type, FxU32 hintMask );
FxBool	EXPORT	grSstIsBusy( void );
FxBool	EXPORT	grSstVRetraceOn( void );
FxU32	EXPORT	grSstStatus( void );
void	EXPORT	grResetTriStats( void );
void	EXPORT	grTriStats(FxU32 *trisProcessed, FxU32 *trisDrawn);
FxU32	EXPORT	grSstVideoLine( void );
FxBool	EXPORT	grSstControlMode( FxU32 mode);
FxBool	EXPORT	grSstControl( FxU32 mode);

#ifdef GLIDE1

//void EXPORT grSstPassthruMode( GrSstPassthruMode_t mode);
void	EXPORT	grSstPassthruMode( int mode);

#endif

float	EXPORT	guFogTableIndexToW( int i );
void	EXPORT	guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density );
void	EXPORT	guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], float density );
void	EXPORT	guFogGenerateLinear(GrFog_t fogTable[GR_FOG_TABLE_SIZE],
									float nearW, float farW );
void	EXPORT	grFogColorValue( GrColor_t value );
void	EXPORT	grFogMode( GrFogMode_t mode );
void	EXPORT	grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] );
void	EXPORT	grSstOrigin( GrOriginLocation_t origin );
FxU32	EXPORT	grSstScreenWidth( void );
FxU32	EXPORT	grSstScreenHeight( void );
void	EXPORT	grRenderBuffer(GrBuffer_t buffer);
void			grGlideShutdownDos9x();
void			grGlideShutdownDosNT();
FxBool	EXPORT1	grSstWinOpen(FxU32 _hwnd, GrScreenResolution_t res,
							 GrScreenRefresh_t refresh, GrColorFormat_t l_cFormat,
							 GrOriginLocation_t locateOrigin, int numBuffers,
							 int numAuxBuffers);
void	EXPORT	grSstWinClose( void );
void	EXPORT	grGlideShutdown();

#ifdef	GLIDE1

FxBool	EXPORT	grSstOpen(GrScreenResolution_t res, GrScreenRefresh_t ref, GrColorFormat_t cformat,
						  GrOriginLocation_t org_loc, GrSmoothingMode_t smoothing_mode, int num_buffers );

#endif



#endif