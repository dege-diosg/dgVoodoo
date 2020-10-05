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
#include	"RendererTypes.hpp"


/*------------------------------------------------------------------------------------------*/
/*............................ Definíciók az egyes állapotokhoz ............................*/

/* különbözõ állapotflagek */
#define STATE_AITRGBLIGHTING			0x01			/* Alpha ITRGB lighting engedélyezve */
#define STATE_COLORCOMBINEUSETEXTURE	0x10			/* A color combining használja a textúra valamely komponensét */
#define STATE_ALPHACOMBINEUSETEXTURE	0x20			/* Az alpha combining használja a textúra valamely komponensét */
#define STATE_COLORDIFFUSEUSED			0x40			/* A color combining használja a diffuse komponenst */
#define STATE_ALPHADIFFUSEUSED			0x80			/* Az alpha combining használja a diffuse komponenst */
#define STATE_APMULTITEXTURING			0x100			/* Az alpha/paletta multitextúrázva */
#define STATE_TCLOCALALPHA				0x200			/* A texture combining local alpha-t eredményez */
#define STATE_TCCINVERT					0x400			/* A texture combining inverz color-t képez */
#define STATE_TCAINVERT					0x800			/* A texture combining inverz alpha-t képez */
#define STATE_USETMUW					0x1000			/* Z-buffer és táblás köd esetén a tmu w-t használjuk */
#define STATE_DELTA0					0x2000			/* Delta0-mód: konstans iterált rgb */

#define STATE_TEXTURE0					0x20000
#define STATE_TEXTURE1					0x40000
#define STATE_TEXTURE2					0x80000
#define STATE_TEXTURE3					0x100000
#define STATE_TEXTUREUSAGEMASK			(STATE_TEXTURE0 | STATE_TEXTURE1 | STATE_TEXTURE2 | STATE_TEXTURE3)

#define STATE_NORENDERTARGET			0x200000
#define STATE_TEXTUREALPHAUSED			0x400000
#define STATE_TEXTURERGBUSED			0x800000

#define	STATE_CKMUNDEFINED				0x0000000
#define	STATE_CKMDISABLED				0x1000000
#define	STATE_CKMNATIVE					0x2000000
#define	STATE_CKMNATIVETNT				0x3000000
#define	STATE_CKMALPHABASED				0x4000000
#define	STATE_CKMMASK					0x7000000

/* A Glide aktuális futásképes állapotát tükrözõ flagek */
#define RF_INITOK						0x1			/* Az init sikerült */
#define RF_SCENECANBEDRAWN				0x2			/* A scene-t meg lehet rajzolni (cooperative level ok, BeginScene ok) */
#define RF_CANFULLYRUN					0x3			/* Ha az elõzõ két flag egyszerre be van állítva, a scene rajzolása mehet */
#define RF_WINDOWCREATED				0x4			/* Windows platform: a wrapper létrehozott egy ablakot az alk. számára */

/* call flags: a primitívek megrajzolása elõtt meghívandó függvényekhez */
#define UPDATEFLAG_COLORCOMBINE			0x1
#define UPDATEFLAG_ALPHACOMBINE			0x2
#define UPDATEFLAG_SETPALETTE			0x4
#define UPDATEFLAG_SETNCCTABLE			0x8
#define UPDATEFLAG_AITRGBLIGHTING		0x10
#define UPDATEFLAG_SETTEXTURE			0x40
#define UPDATEFLAG_RESETTEXTURE			0x80
#define UPDATEFLAG_TEXCOMBINE			0x100
#define UPDATEFLAG_ALPHATESTFUNC		0x200
#define UPDATEFLAG_ALPHATESTREFVAL		0x400
#define UPDATEFLAG_COLORKEYSTATE		0x800

/*------------------------------------------------------------------------------------------*/
/*....................................... Struktúrák .......................................*/


/* Saját GrState struktúra az eredeti helyett: nem lehet több 320 bájtnál! */
typedef struct {

	struct TexCacheEntry*	acttex;
	unsigned int			actmmid;
	TextureType				lpDDTCCTex, lpDDTCATex;
	TextureType				lpDDTex[4];
	FxU32					hints;
	GrOriginLocation_t		locOrig;					/* Glide render origó */
	unsigned int			flags;
	float					divx, divy;
	DWORD					constColorValue, delta0Rgb;
	DWORD					colorMask, alphaWriteMask;
	GrMipMapMode_t			mipMapMode;
	GrAlpha_t				alpharef;
	GrCmpFnc_t				alphaTestFunc;
	GrDepthBufferMode_t		depthBuffMode;
	GrCmpFnc_t				depthFunction;
	FxBool					depthMaskEnable;
	int						depthbiaslevel;
	GrTextureFilterMode_t	magFilter, minFilter;
	GrTextureClampMode_t	clampModeU, clampModeV;
	float					lodBias;
	GrCullMode_t			gcullmode;
	DWORD					vertexmode;
	FxU32					minx, maxx, miny, maxy;
	DWORD					perspective, colorkey;
	DWORD					fogmode, fogcolor, fogmodifier;
	FxU32					startAddress[4];
	float					gammaCorrectionValue;
	FxBool					lodBlend;
	FxBool					multiBaseTexturing;
	unsigned char			alphaBlendEnable, colorKeyEnable;
	unsigned char			rgbSrcBlend, rgbDstBlend, alphaSrcBlend, alphaDstBlend;
	unsigned char			smallLod, largeLod, aspectRatio, format;
	unsigned char			cfuncp;						/* grColorCombine paraméterei */
	unsigned char			cfactorp;
	unsigned char			clocalp;
	unsigned char			cotherp;
	unsigned char			cinvertp;
	unsigned char			afuncp;						/* grAlphaCombine paraméterei */
	unsigned char			afactorp;
	unsigned char			alocalp;					/* Az alocal fontos a grColorCombine-hoz is */
	unsigned char			aotherp;
	unsigned char			ainvertp;	
	unsigned char			rgb_function;				/* a grTexCombine paraméterei */
    unsigned char			rgb_factor;
    unsigned char			alpha_function;
    unsigned char			alpha_factor;
    unsigned char			rgb_invert;
    unsigned char			alpha_invert;
	unsigned char			currCkMethod;
	unsigned char			aItRGBLightingMode;
	
} _GrState, _GlideActState;


/* Struktúra a statisztikákhoz: egyelõre csak a feldolgozott háromszögek számát tárolja */
typedef struct {

	unsigned int			trisProcessed;

} _stat;

/*------------------------------------------------------------------------------------------*/
/*.................................. Globális változók .....................................*/

#ifdef __cplusplus
extern "C" {
#endif

extern	GrColorFormat_t				cFormat;
extern	unsigned int				buffswapnum;
extern	unsigned char				*WToInd;

extern	unsigned int				updateFlags;
extern	unsigned int				runflags;
extern	int							renderbuffer;

extern	_GlideActState				astate;
extern	_stat						stats;

extern	unsigned int				appXRes, appYRes;
extern	int							drawresample;

extern	float						xScale, yScale;
extern	float						xConst, yConst;

extern	DWORD						cmpFuncLookUp[];

extern	float						IndToW[GR_FOG_TABLE_SIZE+1];
extern	GrFog_t						fogtable[GR_FOG_TABLE_SIZE+1];

extern  unsigned int				needRecomposePixelPipeLine;

extern	int							convbuffxres;

#ifdef __cplusplus
}
#endif


/*------------------------------------------------------------------------------------------*/
/*............................. Belsõ függvények predeklarációja ...........................*/

int				GetCurrentDisplayModeBitDepth (unsigned short adapter, unsigned short device);
int				IsRendererApiAvailable ();
int				RefreshDisplayByFrontBuffer ();

void			GlideAlphaCombineUpdateVertexMode();
void			GlideUpdateConstantColorKeyingState();
void			GlideColorCombineUpdateVertexMode();
void			GlideUpdateUsingTmuWMode ();
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
void			GlideTexCombine();
void			GlideAlphaItRGBLighting ();
void			GlideUpdateAlphaTestState ();
void			GlideRecomposePixelPipeLine ();


/*------------------------------------------------------------------------------------------*/
/*............................. Glide-függvények predeklarációja ...........................*/


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
void	EXPORT grAlphaControlsITRGBLighting( FxBool enable );
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