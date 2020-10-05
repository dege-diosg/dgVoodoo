/*--------------------------------------------------------------------------------- */
/* DX9General.cpp - dgVoodoo 3D general rendering interface, DX9 implementation     */
/*                                                                                  */
/* Copyright (C) 2007 Dege                                                          */
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
/* dgVoodoo: DX9General.cpp																	*/
/*			 DX9 renderer general class implementation										*/
/*------------------------------------------------------------------------------------------*/


// ------------------------------------------------------------------------------------------
// ----- Includes ---------------------------------------------------------------------------

extern "C" {

#include	"Debug.h"
#include	"dgVoodooGlide.h"
#include	"Log.h"
#include	"dgVoodooBase.h"
#include	"Dgw.h"
#include	"Draw.h"
#include	"Main.h"

}

#include	"DX9General.hpp"
#include	"DX9Drawing.hpp"
#include	"DX9LfbDepth.hpp"
#include	"DX9Texturing.hpp"
#include	"DX9BaseFunctions.h"


// ------------------------------------------------------------------------------------------
// ----- Defs -------------------------------------------------------------------------------

#define		STATEDESC_DELIMITER			0xEEEEEEEE


#define CreateShaderUnIdLow(texRgbFunc, texAlphaFunc, ccFunc, acFunc, colorKeyingMethod, alphaItRGBLigthing) \
						  ((texRgbFunc & 0x1F) << 27) | ((texAlphaFunc & 0x1F) << 22) | ((ccFunc & 0x1F) << 17) | \
						  ((acFunc & 0x1F) << 12) | (alphaItRGBLigthing ? 4 : 0) | (colorKeyingMethod & 0x3)

#define CreateShaderUnIdHigh(texRgbFact, texAlphaFact, texRgbInv, texAlphaInv, \
						      ccFact, ccLocal, ccOther, ccInvert, acFact, acLocal, acOther, acInvert) \
							((texRgbFact & 0xF) << 28) | ((texAlphaFact & 0xF) << 24) | ((ccFact & 0xF) << 20) | \
							((acFact & 0xF) << 16) | ((ccOther & 0x3) << 13) | ((acOther & 0x3) << 10) | \
							((ccLocal & 0xF) << 7) | ((acLocal & 0xF) << 4) | \
							(texRgbInv ? 8 : 0) | (texAlphaInv ? 4 : 0) | (ccInvert ? 2 : 0) | (acInvert ? 1 : 0)


/*-----------------------------------------------------------------------------------------	*/
/*....................................... Táblák ...........................................*/

/* A Glide alpha blending módok (faktorok) és a D3D módjai (faktorai)között leképezô táblázat */
/* Megtehetjük, mert a Glide-konstansok 0-tól valameddig vannak definiálva ilyen sorrendben. */

static DWORD alphaModeLookupSrc[] = {D3DBLEND_ZERO,				/* GR_BLEND_ZERO */
									 D3DBLEND_SRCALPHA,			/* GR_BLEND_SRC_ALPHA */
									 D3DBLEND_DESTCOLOR,		/* GR_BLEND_DST_COLOR */
									 D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
									 D3DBLEND_ONE,				/* GR_BLEND_ONE */
									 D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
									 D3DBLEND_INVDESTCOLOR,		/* GR_BLEND_ONE_MINUS_DST_COLOR */													   
									 D3DBLEND_INVDESTALPHA,		/* GR_BLEND_ONE_MINUS_DST_ALPHA */
									 0,0,0,0,0,0,0,				/* ezek fenntartottak */						  
									 D3DBLEND_SRCALPHASAT};		/* GR_BLEND_ALPHA_SATURE */

/* Magyarázat a GR_BLEND_PREFOG_COLOR-hoz: ez a blendelési mód a forrásszínt (SRCCOLOR) */
/* használja blending factorként, ua. mint a GR_BLEND_SRC_COLOR, annyi különbséggel, hogy */
/* ezen blending factorra nincs hatással a köd (az ugyanis még az alpha blending elõtt */
/* történik). */
static DWORD alphaModeLookupDst[] = {D3DBLEND_ZERO,				/* GR_BLEND_ZERO */
									 D3DBLEND_SRCALPHA,			/* GR_BLEND_SRC_ALPHA */
									 D3DBLEND_SRCCOLOR,			/* GR_BLEND_SRC_COLOR */
									 D3DBLEND_DESTALPHA,		/* GR_BLEND_DST_ALPHA */
									 D3DBLEND_ONE,				/* GR_BLEND_ONE */
									 D3DBLEND_INVSRCALPHA,		/* GR_BLEND_ONE_MINUS_SRC_ALPHA */
									 D3DBLEND_INVSRCCOLOR,		/* GR_BLEND_ONE_MINUS_SRC_COLOR */													
									 D3DBLEND_INVDESTALPHA,		/* GR_BLEND_ONE_MINUS_DST_ALPHA */
									 0,0,0,0,0,0,0,				/* ezek fenntartottak */
									 D3DBLEND_SRCCOLOR};		/* GR_BLEND_PREFOG_COLOR */

/* combine local paramétert konvertáló táblázat */
static DWORD combinelocallookup[] =		{D3DTA_DIFFUSE, D3DTA_TFACTOR, D3DTA_DIFFUSE};

/* combine other paramétert konvertáló táblázat */
static DWORD combineotherlookup[] =		{D3DTA_DIFFUSE, D3DTA_TEXTURE, D3DTA_TFACTOR};

static DWORD combineargd3dlookup[] =	{D3DTOP_BLENDDIFFUSEALPHA, 0, D3DTOP_BLENDTEXTUREALPHA, D3DTOP_BLENDFACTORALPHA};




static DWORD		scrShotSaveStates[] =	{D3DRS_ALPHATESTENABLE, D3DRS_ALPHABLENDENABLE,
											 D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_ZENABLE,
											 D3DRS_CULLMODE, D3DRS_ZWRITEENABLE, 
											 D3DRS_FOGENABLE,
											 STATEDESC_DELIMITER};

static DWORD		scrShotSamplerStates[] ={D3DSAMP_ADDRESSU, D3DSAMP_ADDRESSV, D3DSAMP_MAGFILTER, D3DSAMP_MINFILTER,
											 STATEDESC_DELIMITER};

static DWORD		scrShotSaveTSStates[]  ={0, D3DTSS_COLOROP,  0, D3DTSS_COLORARG1,  0, D3DTSS_COLORARG2,
											 0, D3DTSS_ALPHAOP, 0, D3DTSS_ALPHAARG1, 0, D3DTSS_ALPHAARG2,
											 /*0, D3DTSS_ADDRESSU, 0, D3DTSS_ADDRESSV, 0, D3DTSS_MAGFILTER,
											 0, D3DTSS_MINFILTER,*/ 1, D3DTSS_COLOROP,
											 STATEDESC_DELIMITER};



static DWORD		lfbSaveStatesNoPipeline[] =	{D3DRS_ALPHATESTENABLE, D3DRS_ALPHABLENDENABLE,
												 D3DRS_ZENABLE, D3DRS_CULLMODE,
												 D3DRS_FOGENABLE,
												 D3DRS_TEXTUREFACTOR,
												 STATEDESC_DELIMITER };

static DWORD		lfbSaveSamplerStates[] =	{D3DSAMP_MAGFILTER, D3DSAMP_MINFILTER, STATEDESC_DELIMITER};


static DWORD		lfbSaveStatesPipeline[]   =	{D3DRS_CULLMODE,
												 D3DRS_TEXTUREFACTOR, D3DRS_ZWRITEENABLE,
												 STATEDESC_DELIMITER };

static DWORD		lfbSaveTSStatesNoPipeline[] =
												{0, D3DTSS_COLOROP, 0, D3DTSS_COLORARG1, 0, D3DTSS_ALPHAOP,
												 0, D3DTSS_ALPHAARG1, /*0, D3DTSS_MAGFILTER, 0, D3DTSS_MINFILTER,*/
												 1, D3DTSS_COLOROP,
												 STATEDESC_DELIMITER};


static DWORD		gammaSaveStates[]	=	{D3DRS_ALPHATESTENABLE, D3DRS_ALPHABLENDENABLE,
											 D3DRS_SRCBLEND, D3DRS_DESTBLEND,
											 D3DRS_ZENABLE, D3DRS_CULLMODE,
											 STATEDESC_DELIMITER};

static DWORD		gammaSaveTSStates[]	=	{0, D3DTSS_COLOROP,  0, D3DTSS_COLORARG1, 1, D3DTSS_COLOROP,
											 STATEDESC_DELIMITER};




/*------------------------------------------------------------------------------------------*/
/*...................... DX7 implementáció az általános dolgokhoz ..........................*/


static const DWORD	dxCullModeLookUp[] = {D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW};


/* Egy pixel shaderben megadott argumentumot átalakít D3DTA formátumra.  */
/* Figyelembe veszi, ha a texture combine egység a kimenetet invertálja. */
DWORD _fastcall	DX9General::GlideGetStageArg(DWORD type, DWORD d3dFactor, DWORD d3dLocal, DWORD d3dOther, int texinvert)
{
register DWORD	modifier = 0;
	
	if (type & PS_INVERT)
		modifier ^= D3DTA_COMPLEMENT;
	
	if (type & PS_ALPHA)
		modifier |= D3DTA_ALPHAREPLICATE;
	
	switch(type & (~(PS_ALPHA | PS_INVERT)) )	{
		case PS_LOCAL:
			return(d3dLocal ^ modifier);
		
		case PS_OTHER:
			if ( texinvert && ((d3dOther & D3DTA_SELECTMASK) == D3DTA_TEXTURE) )
				modifier ^= D3DTA_COMPLEMENT;
			return(d3dOther ^ modifier);
		
		case PS_FACTOR:
			return(d3dFactor ^ modifier);
		
		default:
			return(type ^ modifier);
	}
}


void		DX9General::ConfigureAlphaPixelPipeLineFF (GrCombineFunction_t func, GrCombineFactor_t factor,
													   GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
APixelShader2Stages		*setting;
unsigned int			i, tcInvert, afuncpindex;

	DWORD d3dlocal = combinelocallookup[local];
	DWORD d3dother = combineotherlookup[other];
	DWORD d3dfactor = 0;
	
	switch(factor)	{
		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_LOCAL:
			d3dfactor = d3dlocal;
			break;
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = d3dother;
			break;
		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = d3dother | D3DTA_COMPLEMENT;
			break;
		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;
	}
	setting = alphaPixelShaders;
	afuncpindex = grfunctranslating[func];
	
	if (factor == GR_COMBINE_FACTOR_ZERO) setting = alphaPixelShadersf0;
	if (factor == GR_COMBINE_FACTOR_ONE) setting = alphaPixelShadersf1;
	
	i = 0;
	if (astate.flags & STATE_APMULTITEXTURING)	{
		if ( (d3dother == D3DTA_TEXTURE) &&
			( (setting[func].alphaarg1_0 == PS_OTHER) || (setting[func].alphaarg2_0 == PS_OTHER) ) )	{
			alphaOp[0] = D3DTOP_SELECTARG1;
			alphaArg1[0] = D3DTA_DIFFUSE;
			alphaArg2[0] = D3DTA_DIFFUSE;
			i++;
		}
	}

	tcInvert = astate.flags & STATE_TCAINVERT;
	this->astagesnum = i + 1;
	alphaOp[i] = setting[afuncpindex].alphaop_0;
	alphaArg1[i] = GlideGetStageArg(setting[afuncpindex].alphaarg1_0, d3dfactor, d3dlocal, d3dother, tcInvert);
	alphaArg2[i] = GlideGetStageArg(setting[afuncpindex].alphaarg2_0, d3dfactor, d3dlocal, d3dother, tcInvert);
	if ( (alphaOp[i+1] = setting[afuncpindex].alphaop_1) != D3DTOP_DISABLE )	{
		this->astagesnum = i+2;
		alphaArg1[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg1_1, d3dfactor, d3dlocal, d3dother, tcInvert);
		alphaArg2[i+1] = GlideGetStageArg(setting[afuncpindex].alphaarg2_1, d3dfactor, d3dlocal, d3dother, tcInvert);
	}

	// A speciális opkódok feldolgozása (implicit argumentumokkal)
	// Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ
	switch(alphaOp[i])	{
		case PS_BLENDFACTORALPHA:
			alphaOp[i] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = alphaArg1[i];
				alphaArg1[i] = alphaArg2[i];
				alphaArg2[i] = i;
			}
			break;
	}		
	if (invert)	{
		alphaOp[this->astagesnum] = D3DTOP_SELECTARG2;
		alphaArg2[this->astagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		this->astagesnum++;
	}
	/*if (astate.flags & STATE_COLORKEYTNT)	{
		alphaOp[0] = D3DTOP_SELECTARG1;
		alphaArg1[0] = D3DTA_TEXTURE;
		this->astagesnum = 1;
	}*/

	alphaLocal = d3dlocal;
	alphaOther = d3dother;
}


void		DX9General::ConfigureColorPixelPipeLineFF (GrCombineFunction_t func, GrCombineFactor_t factor,
													   GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
CPixelShader3Stages *setting;
unsigned int		i,cfuncpindex;


	DWORD d3dlocal = combinelocallookup[local];
	DWORD d3dother = combineotherlookup[other];
	DWORD d3dfactor = 0;
	if ( (d3dother == D3DTA_TEXTURE) && (astate.flags & STATE_TCLOCALALPHA) ) d3dother |= D3DTA_ALPHAREPLICATE;

	setting = pixelShadersfa;
	switch(factor)	{		
		case GR_COMBINE_FACTOR_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			setting = pixelShadersfloc;
			d3dfactor = d3dlocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_RGB:
			setting = pixelShadersfloc;
			d3dfactor = D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
			break;
		
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			d3dfactor = alphaOther;
			break;

		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
			d3dfactor = alphaLocal;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE;
			break;
			
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
			d3dfactor = alphaLocal | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			d3dfactor = alphaOther | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			d3dfactor = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			break;

		case GR_COMBINE_FACTOR_ZERO:
			setting = pixelShadersf0;
			break;

		case GR_COMBINE_FACTOR_ONE:
			setting = pixelShadersf1;
			break;

		default:							// ha nem valamelyik konstans
			setting = pixelShadersf0;
	}
	
	cfuncpindex = grfunctranslating[func];

	// Argumentumok és opkód kiolvasása a shaderbõl
	this->cstagesnum = 1;
	i = astate.flags & STATE_TCCINVERT;
	colorOp[0] = setting[cfuncpindex].colorop_0;
	colorArg1[0] = GlideGetStageArg(setting[cfuncpindex].colorarg1_0, d3dfactor, d3dlocal, d3dother, i);
	colorArg2[0] = GlideGetStageArg(setting[cfuncpindex].colorarg2_0, d3dfactor, d3dlocal, d3dother, i);
	if ( (colorOp[1] = setting[cfuncpindex].colorop_1) != D3DTOP_DISABLE )	{
		this->cstagesnum = 2;
		colorArg1[1] = GlideGetStageArg(setting[cfuncpindex].colorarg1_1, d3dfactor, d3dlocal, d3dother, i);
		colorArg2[1] = GlideGetStageArg(setting[cfuncpindex].colorarg2_1, d3dfactor, d3dlocal, d3dother, i);
		if ( (colorOp[2] = setting[cfuncpindex].colorop_2) != D3DTOP_DISABLE )	{
			this->cstagesnum = 3;
			colorArg1[2] = GlideGetStageArg(setting[cfuncpindex].colorarg1_2, d3dfactor, d3dlocal, d3dother, i);
			colorArg2[2] = GlideGetStageArg(setting[cfuncpindex].colorarg2_2, d3dfactor, d3dlocal, d3dother, i);
		}
	}
		
	// A speciális opkódok feldolgozása (implicit argumentumokkal)
	// Jelenleg 1 spec. kód van, az is az 1. stage-en fordulhat elõ
	switch(colorOp[1])	{		
		case PS_BLENDFACTORALPHA:
			colorOp[1] = combineargd3dlookup[d3dfactor & D3DTA_SELECTMASK];
			if (d3dfactor & D3DTA_COMPLEMENT)	{
				i = colorArg1[1];
				colorArg1[1] = colorArg2[1];
				colorArg2[1] = i;
			}
			break;
	}	

	//colorOp[0]=D3DTOP_SELECTARG1;
	//colorArg1[0]=D3DTA_TEXTURE;// | D3DTA_COMPLEMENT;
	//colorOp[1]=D3DTOP_DISABLE;
	
	if (invert)	{
		colorOp[this->cstagesnum] = D3DTOP_SELECTARG2;
		colorArg1[this->cstagesnum] = D3DTA_CURRENT;
		colorArg2[this->cstagesnum] = D3DTA_CURRENT | D3DTA_COMPLEMENT;
		this->cstagesnum++;
	}
}


void		DX9General::ComposeFFShader ()
{
	/* Alpha stage-ek beállítása */
	unsigned int i;
	for(i = 0; i < this->astagesnum; i++)	{
		if (this->cachedAlphaOp[i] != alphaOp[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = alphaOp[i]);
		if (this->cachedAlphaArg1[i] != alphaArg1[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = alphaArg1[i]);
		if (this->cachedAlphaArg2[i] != alphaArg2[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAARG2, this->cachedAlphaArg2[i] = alphaArg2[i]);
	}

	for (;i < MAX_TEXSTAGES; i++)	{
		if (this->cachedAlphaOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = D3DTOP_SELECTARG1);
		if (this->cachedAlphaArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = D3DTA_CURRENT);
	}

	/* Color stage-ek beállítása */
	for(i = 0; i < this->cstagesnum; i++)	{
		if (this->cachedColorOp[i] != colorOp[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = colorOp[i]);
		if (this->cachedColorArg1[i] != colorArg1[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLORARG1, this->cachedColorArg1[i] = colorArg1[i]);
		if (this->cachedColorArg2[i] != colorArg2[i])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLORARG2, this->cachedColorArg2[i] = colorArg2[i]);
	}

	for(i = this->cstagesnum; i < this->astagesnum; i++)	{
		if (this->cachedColorOp[i] != D3DTOP_SELECTARG1)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_SELECTARG1);
		if (this->cachedColorArg1[i] != D3DTA_CURRENT)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLORARG1, this->cachedColorArg1[i] = D3DTA_CURRENT);
	}

	if (alphaCkAlphaOp[0] != D3DTOP_DISABLE) {
		DWORD cArg2 = D3DTA_CURRENT;
		DWORD aArg2 = D3DTA_CURRENT;

		int onlyInvertColorOutput = 0;
		int onlyInvertAlphaOutput = 0;
		//Ha az utolsó color- és alpha-stage-k csak a kimenetet invertálják, vagy csak "kitöltõ" stage-ek,
		//akkor ezt a stage-t fel lehet használni a mûvelethez, egyébként új stage
		if ( (this->astagesnum < i) || (onlyInvertColorOutput = ((this->astagesnum == i) && (astate.ainvertp))) )	{
			if ( (this->cstagesnum < i) || (onlyInvertAlphaOutput = ((this->cstagesnum == i) && (astate.cinvertp))) )	{
				if (onlyInvertColorOutput) cArg2 = D3DTA_CURRENT | D3DTA_COMPLEMENT;
				if (onlyInvertAlphaOutput) aArg2 = D3DTA_CURRENT | D3DTA_COMPLEMENT;
				i--;
			}
		}

		if (this->cachedAlphaOp[i] != alphaCkAlphaOp[0])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAOP, this->cachedAlphaOp[i] = alphaCkAlphaOp[0]);
		if (this->cachedAlphaArg1[i] != alphaCkAlphaArg1[0])
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAARG1, this->cachedAlphaArg1[i] = alphaCkAlphaArg1[0]);
		if (this->cachedAlphaArg2[i] != aArg2)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_ALPHAARG2, this->cachedAlphaArg2[i] = aArg2);

		if (this->cachedColorOp[i] != D3DTOP_SELECTARG2)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_SELECTARG2);
		if (this->cachedColorArg2[i] != cArg2)
			lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLORARG2, this->cachedColorArg2[i] = cArg2);

		i++;
	}
	maxUsedSamplerNum = i;

	if (i != MAX_TEXSTAGES)
		if (this->cachedColorOp[i] != D3DTOP_DISABLE) lpD3Ddevice9->SetTextureStageState(i, D3DTSS_COLOROP, this->cachedColorOp[i] = D3DTOP_DISABLE);
}


void		DX9General::ConfigureTexelPixelPipeLine14 (GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor, 
													   GrCombineFunction_t alpha_function, GrCombineFactor_t alpha_factor,
													   FxBool /*rgb_invert*/, FxBool /*alpha_invert*/)
{
	DWORD alphaLocal = (astate.flags & STATE_APMULTITEXTURING) ? PSSR_R(1) : PSSR_R(0);
	if (alpha_factor == GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA)
		alpha_factor = GR_COMBINE_FACTOR_ONE;

	DWORD alphaFactor = tFactorRegLookUp[alpha_factor];
	if ((astate.flags & STATE_APMULTITEXTURING) && ((alphaFactor & ~PSREGMODIFIERMASK) == PSSR_R(0)))
		alphaFactor = PSSR_R(1) | (alphaFactor & PSREGMODIFIERMASK);

	switch (tFuncShaderTableLookUp[alpha_factor]) {
		case 0:
			pTexAlphaShader = tShaderTokens14_f0;
			break;
		case 1:
			pTexAlphaShader = tShaderTokens14_f1;
			break;
		case 2:
			pTexAlphaShader = tShaderTokens14_f;
			break;
	}
	pTexAlphaShader += MAX_14SHADER_LENGTH * funcToShaderIndex[alpha_function];

	texAlphaTemplateRegs[ALocal] = texAlphaTemplateRegs[CLocal] = alphaLocal;
	texAlphaTemplateRegs[NegCLocal] = alphaLocal | PSRM_Negate;
	texAlphaTemplateRegs[Factor] = alphaFactor;
	texAlphaTemplateRegs[InvFactor] = alphaFactor ^ PSRM_Complement;
	//TexAlphaCombine kimenete a COther template-ben

	if (rgb_factor == GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA)
		rgb_factor = GR_COMBINE_FACTOR_ONE;

	DWORD colorFactor;
	switch (rgb_factor) {
		case GR_COMBINE_FACTOR_LOCAL:
			colorFactor = PSSR_R(0);
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			colorFactor = PSSR_R(0) | PSRM_Complement;
			break;

		default:
			colorFactor = tFactorRegLookUp[rgb_factor] | PSRM_AlphaSwizzle;
			if ((astate.flags & STATE_APMULTITEXTURING) && ((colorFactor & ~PSREGMODIFIERMASK) == PSSR_R(0)))
				colorFactor = PSSR_R(1) | (colorFactor & PSREGMODIFIERMASK);

	}
	switch (tFuncShaderTableLookUp[rgb_factor]) {
		case 0:
			pTexColorShader = tShaderTokens14_f0;
			break;
		case 1:
			pTexColorShader = tShaderTokens14_f1;
			break;
		case 2:
			pTexColorShader = tShaderTokens14_f;
			break;
	}
	pTexColorShader += MAX_14SHADER_LENGTH * funcToShaderIndex[rgb_function];

	texColorTemplateRegs[ALocal] = alphaLocal | PSRM_AlphaSwizzle;
	texColorTemplateRegs[CLocal] = PSSR_R(0);
	texColorTemplateRegs[NegCLocal] = PSSR_R(0) | PSRM_Negate;
	texColorTemplateRegs[Factor] = colorFactor;
	texColorTemplateRegs[InvFactor] = colorFactor ^ PSRM_Complement;
	//TexColorCombine kimenete a COther template-ben
}


void		DX9General::ConfigureAlphaPixelPipeLine14 (GrCombineFunction_t func, GrCombineFactor_t factor,
													   GrCombineLocal_t local, GrCombineOther_t other, FxBool /*invert*/)
{
	alphaLocal = localRegLookUp[local];
	// A aOther és aFactor regiszter template, ha a textúrát jelölik
	alphaOther = otherRegLookUp[other];
	
	DWORD alphaFactor = 0;
	
	pAlphaShader = cShaderTokens14_f;
	switch(factor)	{		
		case GR_COMBINE_FACTOR_LOCAL:
			alphaFactor = alphaLocal;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			alphaFactor = alphaLocal | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_RGB:
			alphaFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE]; // ezt a doksi nem említi
			break;
		
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			alphaFactor = alphaOther;
			break;

		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
			alphaFactor = alphaLocal;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			alphaFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE];
			break;
			
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
			alphaFactor = alphaLocal | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			alphaFactor = alphaOther | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			alphaFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE] | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_ZERO:
			pAlphaShader = cShaderTokens14_f0;
			break;

		case GR_COMBINE_FACTOR_ONE:
			pAlphaShader = cShaderTokens14_f1;
			break;

		default:							// ha nem valamelyik konstans
			pAlphaShader = cShaderTokens14_f0;
			break;
	}
	

	pAlphaShader += MAX_14SHADER_LENGTH * funcToShaderIndex[func];// * sizeof (ShaderTokens);

	alphaTemplateRegs[ALocal] = alphaTemplateRegs[CLocal] = alphaLocal;
	alphaTemplateRegs[NegCLocal] = (alphaLocal == PSSR_C(0)) ? PSSR_C(1) : (alphaLocal | PSRM_Negate);
	alphaTemplateRegs[COther] = alphaOther;
 	alphaTemplateRegs[Factor] = alphaFactor;
	alphaTemplateRegs[InvFactor] = (alphaFactor == PSSR_C(0)) ? PSSR_C(2) : (alphaFactor ^ PSRM_Complement);
	// AlphaCombine kimenete az R0 regiszterben
}


void		DX9General::ConfigureColorPixelPipeLine14 (GrCombineFunction_t func, GrCombineFactor_t factor,
													   GrCombineLocal_t local, GrCombineOther_t other, FxBool /*invert*/)
{
	DWORD colorLocal = alphaItRgbLightingEnabled ? PSSR_R(2) : localRegLookUp[local];
	// A cOther és cFactor regiszter template, ha a textúrát jelölik
	DWORD colorOther = otherRegLookUp[other];
	
	DWORD colorFactor = 0;

	pColorShader = cShaderTokens14_f;
	switch(factor)	{		
		case GR_COMBINE_FACTOR_LOCAL:
			colorFactor = colorLocal;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
			colorFactor = colorLocal | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_RGB:
			colorFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE]; // ezt a doksi nem említi
			break;
		
		case GR_COMBINE_FACTOR_OTHER_ALPHA:
			colorFactor = alphaOther | PSRM_AlphaSwizzle;
			if (colorFactor & PSR_TEMPLATE)
				colorFactor = (colorFactor & ~PSR_TEMPLATE) | PSR_TEXALPHATEMPLATE;
			break;

		case GR_COMBINE_FACTOR_LOCAL_ALPHA:
			colorFactor = alphaLocal | PSRM_AlphaSwizzle;
			break;

		case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
			colorFactor = PSR_TEXALPHATEMPLATE | PSRM_AlphaSwizzle;
			/*colorFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE] | PSRM_AlphaSwizzle;
			if (colorFactor & PSR_TEMPLATE)
				colorFactor = (colorFactor & ~PSR_TEMPLATE) | PSR_TEXALPHATEMPLATE;*/
			break;
			
		case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
			colorFactor = alphaLocal | PSRM_Complement;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
			colorFactor = alphaOther | PSRM_AlphaSwizzle | PSRM_Complement;
			if (colorFactor & PSR_TEMPLATE)
				colorFactor = (colorFactor & ~PSR_TEMPLATE) | PSR_TEXALPHATEMPLATE;
			break;

		case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
			colorFactor = PSR_TEXALPHATEMPLATE | PSRM_AlphaSwizzle | PSRM_Complement;
			/*colorFactor = otherRegLookUp[GR_COMBINE_OTHER_TEXTURE] | PSRM_AlphaSwizzle | PSRM_Complement;
			if (colorFactor & PSR_TEMPLATE)
				colorFactor = (colorFactor & ~PSR_TEMPLATE) | PSR_TEXALPHATEMPLATE;*/
			break;

		case GR_COMBINE_FACTOR_ZERO:
			pColorShader = cShaderTokens14_f0;
			break;

		case GR_COMBINE_FACTOR_ONE:
			pColorShader = cShaderTokens14_f1;
			break;

		default:							// ha nem valamelyik konstans
			pColorShader = cShaderTokens14_f0;
			break;
	}
	

	pColorShader += MAX_14SHADER_LENGTH * funcToShaderIndex[func];

	colorTemplateRegs[CLocal] = colorLocal;
	colorTemplateRegs[ALocal] = colorLocal | PSRM_AlphaSwizzle;
	colorTemplateRegs[NegCLocal] = (colorLocal == PSSR_C(0)) ? PSSR_C(1) : (colorLocal | PSRM_Negate);
	colorTemplateRegs[COther] = colorOther;
	colorTemplateRegs[Factor] = colorFactor;
	colorTemplateRegs[InvFactor] = (colorFactor == PSSR_C(0)) ? PSSR_C(2) : (colorFactor ^ PSRM_Complement);
	// ColorCombine kimenete az R0 regiszterben

}


void		DX9General::ComposePixelShader ()
{
	unsigned int unIdLow = CreateShaderUnIdLow (astate.rgb_function, astate.alpha_function, astate.cfuncp, astate.afuncp, \
												colorKeyingMethod, alphaItRgbLightingEnabled);
	unsigned int unIdHigh = CreateShaderUnIdHigh (astate.rgb_factor, astate.alpha_factor, astate.rgb_invert, astate.alpha_invert, \
												  astate.cfactorp, astate.clocalp, astate.cotherp, astate.cinvertp, \
												  astate.afactorp, astate.alocalp, astate.aotherp, astate.ainvertp);

	int i, j;
	unsigned int useCounter = 0xFFFFFFFF;
	PixelShaderCache* pixelShaderCache = cachedPixelShaders;
	for (i = j = 0; i < MAXNUMOFCACHEDPSHADERS; i++, pixelShaderCache++) {
		if (pixelShaderCache->lpD3DPShader9 != NULL) {
			if ((unIdLow == pixelShaderCache->psUnIdLow) && (unIdHigh == pixelShaderCache->psUnIdHigh))
				break;
			if (pixelShaderCache->useCounter < useCounter) {
				useCounter = pixelShaderCache->useCounter;
				j = i;
			}
		} else
			break;
	}

	if (i == MAXNUMOFCACHEDPSHADERS)
		pixelShaderCache = cachedPixelShaders + j;
	
	if ((unIdLow != pixelShaderCache->psUnIdLow) || (unIdHigh != pixelShaderCache->psUnIdHigh)) {
		
		i = 1;

		int maskTextureSampler = 0;
		
		pixelShaderByteCode[0] = PSVERSION_14;

		/* texel pipeline összerakása */
		if ((astate.flags & STATE_TEXTURERGBUSED) || (astate.flags & STATE_TEXTUREALPHAUSED)) {

			int colorEnd = !(astate.flags & STATE_TEXTURERGBUSED);
			int alphaEnd = !(astate.flags & STATE_TEXTUREALPHAUSED);

			int instrIndex = 0;
			//A texColorCombine és texAlphaCombine kimenetei a COther template-ben
			//Ha nincs End utasítás, akkor az aritmetikai mûveletek kimenete az R0, ez itt alapértelmezettnek beállítjuk
			texAlphaTemplateRegs[COther] = texColorTemplateRegs[COther] = PSSR_R(0);
			ShaderTokens* pColorShader = this->pTexColorShader;
			ShaderTokens* pAlphaShader = this->pTexAlphaShader;
			char texLdUsed[3] = {0, 0, 0};
			int colorArithmeticInstr;


			// Elõreolvasás, meghatározzuk, hogy milyen texld utasításokra lesz szükség
			if (!colorEnd) {
				if (pColorShader->instruction == D3DSIO_TEX) {
					DWORD dstReg = pColorShader->dstReg;
					if (dstReg == PSDR_ACLOCAL) {
						texLdUsed[0] = 1;
						dstReg = PSDR_ALOCAL;
					}
					texLdUsed[GetRegNumber(texColorTemplateRegs[dstReg & ~PSR_TEMPLATE])] = 1;
					//pColorShader++;
				}
			}
			if (!alphaEnd) {
				if (pAlphaShader->instruction == D3DSIO_TEX) {
					DWORD dstReg = pAlphaShader->dstReg;
					if (dstReg == PSDR_ACLOCAL)
						dstReg = PSDR_ALOCAL;
					texLdUsed[GetRegNumber(texAlphaTemplateRegs[dstReg & ~PSR_TEMPLATE])] = 1;
					//pAlphaShader++;
				}
			}

			if (colorKeyingMethod != Disabled)
				texLdUsed[maskTextureSampler = (texLdUsed[1] ? 2 : 1)] = 1;

			for (j = 0; j < 3; j++) {
				if (texLdUsed[j]) {
					pixelShaderByteCode[i++] = D3DSIO_TEX;
					pixelShaderByteCode[i++] = PSDR_R(j) | PDWM_RGB | PDWM_Alpha;
					pixelShaderByteCode[i++] = PSSR_T(0);
				}
			}


			while ((!(colorEnd && alphaEnd)) && (instrIndex < MAX_14SHADER_LENGTH)) {
				colorArithmeticInstr = 0;
				
				// Aritmetikai és end tokenek kezelése
				if (!colorEnd) {
					if (pColorShader->instruction == D3DSIO_END) {
						texColorTemplateRegs[COther] = (pColorShader->dstReg & PSR_TEMPLATE) ? texColorTemplateRegs[(pColorShader->dstReg & ~PSR_TEMPLATE)] : pColorShader->dstReg;
						colorEnd = 1;
					} else {
						if (pColorShader->instruction != D3DSIO_TEX) {
							pixelShaderByteCode[i++] = pColorShader->instruction;
							pixelShaderByteCode[i++] = pColorShader->dstReg | PDWM_RGB;
							for (j = 0; j < 3; j++) {
								if (pColorShader->srcRegs[j])
									pixelShaderByteCode[i++] = (pColorShader->srcRegs[j] & PSR_TEMPLATE) ? texColorTemplateRegs[(pColorShader->srcRegs[j] & ~PSR_TEMPLATE)] : pColorShader->srcRegs[j];
							}
							colorArithmeticInstr = 1;
						}
						pColorShader++;
					}
				}
				if (!alphaEnd) {
					if (pAlphaShader->instruction == D3DSIO_END) {
						texAlphaTemplateRegs[COther] = (pAlphaShader->dstReg & PSR_TEMPLATE) ? texAlphaTemplateRegs[(pAlphaShader->dstReg & ~PSR_TEMPLATE)] : pAlphaShader->dstReg;
						alphaEnd = 1;
					} else {
						if (pAlphaShader->instruction != D3DSIO_TEX) {
							pixelShaderByteCode[i++] = pAlphaShader->instruction | (colorArithmeticInstr ? PSIM_CoIssue : 0);
							pixelShaderByteCode[i++] = pAlphaShader->dstReg | PDWM_Alpha;
							for (j = 0; j < 3; j++) {
								if (pAlphaShader->srcRegs[j])
									pixelShaderByteCode[i++] = (pAlphaShader->srcRegs[j] & PSR_TEMPLATE) ? texAlphaTemplateRegs[(pAlphaShader->srcRegs[j] & ~PSR_TEMPLATE)] : pAlphaShader->srcRegs[j];
							}
						}
						pAlphaShader++;
					}
				}
				instrIndex++;
			}
			if (astate.rgb_invert)
				texColorTemplateRegs[COther] = (texColorTemplateRegs[COther] == PSSR_C(3)) ? PSSR_C(4) : texColorTemplateRegs[COther] ^ PSRM_Complement;
			if (astate.alpha_invert)
				texAlphaTemplateRegs[COther] = (texAlphaTemplateRegs[COther] == PSSR_C(3)) ? PSSR_C(4) : texAlphaTemplateRegs[COther] ^ PSRM_Complement;
		}

		/* Chroma comparison */
		if (colorKeyingMethod != Disabled) {
			int alphaSavedUsed = (astate.flags & STATE_TEXTUREALPHAUSED) && (GetRegType(texAlphaTemplateRegs[COther]) == D3DSPR_TEMP);
			
			ShaderTokens* pChromaShader = NULL;
			if (colorKeyingMethod == Native)
				pChromaShader = alphaSavedUsed ? chromaShaderTokens14 : chromaShaderTokens14_noAlphaSave;
			else
				pChromaShader = alphaSavedUsed ? chromaShaderTokensAlphaBased14 : chromaShaderTokensAlphaBasedNoAlphaSave14;
			
			/* A chroma shader Clocal eleme a maszk-textúrát adja meg */
			chromaCompTemplateRegs[CLocal] = PSSR_R(maskTextureSampler) | ((colorKeyingMethod == AlphaBased) ? PSRM_AlphaSwizzle : 0);
			chromaCompTemplateRegs[COther] = texColorTemplateRegs[COther];
			chromaCompTemplateRegs[ALocal] = texColorTemplateRegs[COther] | PSRM_AlphaSwizzle;

			while (pChromaShader->instruction != D3DSIO_END) {
				pixelShaderByteCode[i++] = pChromaShader->instruction;
				if (pChromaShader->dstReg)
					pixelShaderByteCode[i++] = pChromaShader->dstReg;
				for (j = 0; j < 3; j++) {
					if (pChromaShader->srcRegs[j])
						pixelShaderByteCode[i++] = (pChromaShader->srcRegs[j] & PSR_TEMPLATE) ? chromaCompTemplateRegs[(pChromaShader->srcRegs[j] & ~PSR_TEMPLATE)] : pChromaShader->srcRegs[j];
				}
				pChromaShader++;
			}
			//Ha a textúra alpha megõrzésére volt szükség, akkor az az r5-ben keletkezik, r5.r alakban elérhetõ
			if (alphaSavedUsed)
				texAlphaTemplateRegs[COther] = PSSR_R(5) & (~PSRM_AlphaSwizzle);
		}

		/* Alpha ItRGBLighting shader */
		if (alphaItRgbLightingEnabled) {
			ShaderTokens* pAlphaItRGBShader = alphaItRGBShaderTokens;
			while (pAlphaItRGBShader->instruction != D3DSIO_END) {
				pixelShaderByteCode[i++] = pAlphaItRGBShader->instruction;
				pixelShaderByteCode[i++] = pAlphaItRGBShader->dstReg;
				for (j = 0; j < 3; j++) {
					if (pAlphaItRGBShader->srcRegs[j])
						pixelShaderByteCode[i++] = (pAlphaItRGBShader->srcRegs[j] & PSR_TEMPLATE) ? (texAlphaTemplateRegs[(pAlphaItRGBShader->srcRegs[j] & ~PSR_TEMPLATE)]) | PSRM_AlphaSwizzle : pAlphaItRGBShader->srcRegs[j];
				}
				pAlphaItRGBShader++;
			}
		}

		/* Color- és alpha pipeline összerakása */
		int colorEnd = 0;
		int alphaEnd = 0;

		int instrIndex = 0;
		ShaderTokens* pColorShader = this->pColorShader;
		ShaderTokens* pAlphaShader = this->pAlphaShader;
		while ((!(colorEnd && alphaEnd)) && (instrIndex < MAX_14SHADER_LENGTH)) {
			colorEnd = (pColorShader->instruction == D3DSIO_END);
			alphaEnd = (pAlphaShader->instruction == D3DSIO_END);

			/*int skipColorInst = 0;
			savedInstPos = i;*/
			if (!colorEnd) {
				pixelShaderByteCode[i++] = pColorShader->instruction;
				pixelShaderByteCode[i++] = pColorShader->dstReg | PDWM_RGB;
				for (j = 0; j < 3; j++) {
					DWORD reg;
					if (reg = pColorShader->srcRegs[j]) {
						if (reg & (PSR_TEMPLATE | PSR_TEXALPHATEMPLATE)) {
							reg = colorTemplateRegs[reg & ~PSR_TEMPLATE];
							if (reg & PSR_TEMPLATE)
								reg = texColorTemplateRegs[reg & ~PSR_TEMPLATE];
							else if (reg & PSR_TEXALPHATEMPLATE)
								reg = texAlphaTemplateRegs[reg & ~PSR_TEXALPHATEMPLATE];
						}
						pixelShaderByteCode[i++] = reg;
					}
				}
				/*if (pColorShader->instruction == D3DSIO_MOV) {
					if (GetRegSwizzle(pixelShaderByteCode[savedInstPos+2]) == 0x00E40000) {
						if (GetRegTypeAndNumber (pixelShaderByteCode[savedInstPos+2]) != GetRegTypeAndNumber (pColorShader->dstReg))
							skipColorInst = i = savedInstPos;
					}
				}*/
				pColorShader++;
			}

			//savedInstPos = i;
			if (!alphaEnd) {
				pixelShaderByteCode[i++] = pAlphaShader->instruction | ((!colorEnd /*&& !skipColorInst*/) ? PSIM_CoIssue : 0);
				pixelShaderByteCode[i++] = pAlphaShader->dstReg | PDWM_Alpha;
				for (j = 0; j < 3; j++) {
					DWORD reg;
					if (reg = pAlphaShader->srcRegs[j]) {
						if (reg & PSR_TEMPLATE) {
							reg = alphaTemplateRegs[reg & ~PSR_TEMPLATE];
							if (reg & PSR_TEMPLATE)
								reg = texAlphaTemplateRegs[reg & ~PSR_TEMPLATE];
						}
						pixelShaderByteCode[i++] = reg;
					}
				}
				/*if (pAlphaShader->instruction == D3DSIO_MOV) {
					if (GetRegSwizzle(pixelShaderByteCode[savedInstPos+2]) == 0x00E40000) {
						if (GetRegTypeAndNumber (pixelShaderByteCode[savedInstPos+2]) == GetRegTypeAndNumber (pAlphaShader->dstReg))
							i = savedInstPos;
					}
				}*/
				pAlphaShader++;
			}
			instrIndex++;
		}

		if (astate.cinvertp || astate.ainvertp) {
			pixelShaderByteCode[i++] = D3DSIO_MOV;
			pixelShaderByteCode[i++] = PSDR_R(0) | (astate.cinvertp ? PDWM_RGB : 0) | (astate.ainvertp ? PDWM_Alpha : 0);
			pixelShaderByteCode[i++] = PSSR_R(0) | PSRM_Complement;
		}

		pixelShaderByteCode[i++] = D3DSIO_END;

		/* Az esetleges elõzõ shader eldobása */
		if (pixelShaderCache->lpD3DPShader9 != NULL) {
			pixelShaderCache->lpD3DPShader9->Release ();
			pixelShaderCache->lpD3DPShader9 = NULL;
		}

		/* Shader létrehozása */
		DEBUGLOG (0, "\n---- Generated shader:");
		DEBUGLOG (1, "\n---- Generált shader:");
		DisAsmShader (pixelShaderByteCode);

		HRESULT hr = lpD3Ddevice9->CreatePixelShader (pixelShaderByteCode, &pixelShaderCache->lpD3DPShader9);
		if (FAILED (hr)) {
			DEBUGLOG(0,"\n   Warning:  DX9General::ComposePixelShader: creating pixel shader has failed, error code: %0x", hr);
			DEBUGLOG(1,"\n   Figyelmeztetés: DX9General::ComposePixelShader: a pixel shader létrehozása nem sikerült, hibakód: %0x", hr);
			DEBUGLOG (0, "\n---- Generated shader:");
			DEBUGLOG (1, "\n---- Generált shader:");
			DisAsmShader (pixelShaderByteCode);
		}

		/* Shadert berakjuk a cache-be */
		pixelShaderCache->psUnIdLow = unIdLow;
		pixelShaderCache->psUnIdHigh = unIdHigh;
		pixelShaderCache->useCounter = 0;
	}
	pixelShaderCache->useCounter++;

	/*DWORD buffer[256];
	UINT size = 1024;
	pixelShaderCache->lpD3DPShader9->GetFunction (&buffer, &size);
	LOG (1, "\nUsed shader:");
	DisAsmShader (buffer);*/


	HRESULT hr = lpD3Ddevice9->SetPixelShader (currentPixelShader = pixelShaderCache->lpD3DPShader9);

	if (FAILED (hr)) {
		DEBUGLOG(0,"\n   Warning:  DX9General::ComposePixelShader: setting current pixel shader has failed, error code: %0x", hr);
		DEBUGLOG(1,"\n   Figyelmeztetés: DX9General::ComposePixelShader: a pixel shader beállítása nem sikerült, hibakód: %0x", hr);
	}
}


int			DX9General::Init3D ()
{
	lpD3Ddevice9->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	lpD3Ddevice9->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);

	unsigned int i;
	for (i = 0; i < MAX_TEXSTAGES; i++) {
		cachedColorOp[i] = cachedAlphaOp[i] = cachedColorArg1[i] = cachedAlphaArg1[i] = cachedColorArg2[i] = cachedAlphaArg2[i] = 0xEEEEEEEE;
	}
	astagesnum = cstagesnum = maxUsedSamplerNum = 0;
	alphaItRgbLightingEnabled = 0;
	currentPixelShader = NULL;

	usePixelShaders = (config.dx9ConfigBits & CFG_DX9FFSHADERS) ? 0 : (d3dCaps9.PixelShaderVersion >= 0xFFFF0104);
	ZeroMemory (cachedPixelShaders, MAXNUMOFCACHEDPSHADERS * sizeof (PixelShaderCache));
	ZeroMemory (lpD3DStaticShaders, NumberOfStaticShaders * sizeof (LPDIRECT3DPIXELSHADER9));

	if (usePixelShaders) {
		SetupStaticConstantPixelShaderRegs ();

		DWORD* staticShaderTokens[NumberOfStaticShaders] = {lfbTextureTileShaderTokens,
															lfbTextureTileWithColorKeyShaderTokens,
															lfbTextureTileConstAlphaShaderTokens,
															lfbTextureTileConstAlphaWithColorKeyShaderTokens,
															scrShotShaderTokens,
															gammaCorrShaderTokens,
															paletteShaderTokens,
															grayScaleShaderTokens};

		for (i = 0; i < NumberOfStaticShaders; i++) {
			lpD3DStaticShaders[i] = NULL;
			/*DEBUGLOG (0, "\n--- Used static shader:");
			DEBUGLOG (1, "\n--- Használt statikus shader:");
			DisAsmShader (staticShaderTokens[i]);*/

			HRESULT hr = lpD3Ddevice9->CreatePixelShader (staticShaderTokens[i], lpD3DStaticShaders + i);
			DEBUGCHECK (0, FAILED (hr), "\n   Error: DX9General::Init3D: Cannot create one of the static pixel shaders!!" );
			DEBUGCHECK (1, FAILED (hr), "\n   Hiba: DX9General::Init3D: Nem tudtam létrehozni az egyik statikus pixel shadert!!" );
			if (FAILED (hr)) {
				DEBUGLOG (0, "\n--- Used static shader:");
				DEBUGLOG (1, "\n--- Használt statikus shader:");
				DisAsmShader (staticShaderTokens[i]);
			}
		}
	}
	return (1);
}


void		DX9General::Deinit3D ()
{
	lpD3Ddevice9->SetPixelShader (NULL);
	unsigned int i;
	for (i = 0;i < MAXNUMOFCACHEDPSHADERS; i++) {
		if (cachedPixelShaders[i].lpD3DPShader9 != NULL)
			cachedPixelShaders[i].lpD3DPShader9->Release ();
	}
	for (i = 0; i < NumberOfStaticShaders; i++) {
		if (lpD3DStaticShaders[i] != NULL) {
			lpD3DStaticShaders[i]->Release ();
			lpD3DStaticShaders[i] = NULL;
		}
	}
}


void		DX9General::SetupStaticConstantPixelShaderRegs ()
{
	float c[3*4];
	c[0] = c[1] = c[2] = c[3] = 0.0f;
	c[4] = c[5] = c[6] = c[7] = 1.0f;
	c[8] = c[9] = c[10] = c[11] = -1.0f;
	lpD3Ddevice9->SetPixelShaderConstantF (3, c, 3);
	
	unsigned char alphaRef = config.AlphaCKRefValue;
	if (alphaRef == 0)
		alphaRef = 1;

	c[3] = ((float) alphaRef) / 255.0f;
	lpD3Ddevice9->SetPixelShaderConstantF (7, c, 1);
}


int			DX9General::GetSwapChainAndBackBuffers ()
{
	HRESULT hr = lpD3Ddevice9->GetSwapChain (0, &lpD3DSwapChain9);
	if (!FAILED (hr)) {

		ZeroMemory (renderBuffers, 3*sizeof (LPDIRECT3DSURFACE9));
		if (lfbCopyMode) {

			for (unsigned int i = 0; i <= bufferNum; i++) {
				hr = lpD3Ddevice9->CreateRenderTarget (d3dPresentParameters.BackBufferWidth,
													   d3dPresentParameters.BackBufferHeight,
													   d3dPresentParameters.BackBufferFormat,
													   D3DMULTISAMPLE_NONE,
													   0,
													   TRUE,
													   renderBuffers+i, NULL);
				if (FAILED (hr)) {
					for (unsigned int j = 0; j < i; j++)
						renderBuffers[j]->Release ();

					lpD3DSwapChain9->Release ();
					return (0);
				}
			}
			//lpD3DSwapChain9->GetBackBuffer (0, D3DBACKBUFFER_TYPE_MONO, );

		} else {
			//renderBuffers[0] = NULL;	//hiányzó front buffer
			for (unsigned int i = 1; i <= d3dPresentParameters.BackBufferCount; i++)
				lpD3DSwapChain9->GetBackBuffer (i-1, D3DBACKBUFFER_TYPE_MONO, renderBuffers+i);
		}

		//for (; i < 3; i++)
		//	renderBuffers[i] = NULL;

		return (1);
	}
	return (0);
}


void		DX9General::ReleaseSwapChainAndBackBuffers ()
{
	for (int i = 0; i < 3; i++) {
		if (renderBuffers[i] != NULL) {
			renderBuffers[i]->Release ();
			renderBuffers[i] = NULL;
		}
	}

	lpD3DSwapChain9->Release ();
}


void		DX9General::PrepareForInit (HWND rendererWnd)
{
	SetForegroundWindow (rendererWnd);
	//ShowWindow (consoleWnd, SW_MINIMIZE);
}


int			DX9General::InitRendererApi ()
{
	dx9General = this;
	return InitDX9 ();
}


void		DX9General::UninitRendererApi ()
{
	UninitDX9 ();
}


int			DX9General::GetApiCaps ()
{
	return (lfbCopyMode ? CAP_FRONTBUFFAVAILABLE : 0);
}


int			DX9General::SetWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										unsigned int bufferNum, HWND hWnd, unsigned short adapter, unsigned short device,
										unsigned int flags)
{
	inScene = 0;
	if (EnumSupportedD3D9DeviceTypesOnAdapter (adapter)) {
	
		lpD3D9 = GetDirect3D9 ();

		D3DDISPLAYMODE d3dDisplayMode;

		renderTargetFormat = (bitPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_A8R8G8B8;

		deviceDisplayFormat = (bitPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;

		if (flags & SWM_WINDOWED) {
			D3DDISPLAYMODE adapterMode;

			lpD3D9->GetAdapterDisplayMode (adapter, &adapterMode);
			deviceDisplayFormat = renderTargetFormat = adapterMode.Format;
		} else {
			hwndStyle = GetWindowLong (hWnd, GWL_STYLE);
			hwndStyleEx = GetWindowLong (hWnd, GWL_EXSTYLE);
			SetWindowLong (hWnd, GWL_STYLE, hwndStyle & (~(WS_BORDER | WS_CAPTION | WS_DLGFRAME | WS_THICKFRAME)));
			SetWindowLong (hWnd, GWL_EXSTYLE, hwndStyleEx & (~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE)));
			SetWindowPos (hWnd, HWND_TOPMOST, 0, 0, xRes, yRes, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSENDCHANGING);
			//SetWindowPos (hWnd, HWND_NOTOPMOST, 0, 0, xRes, yRes, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSENDCHANGING);
		}

		if ((monRefreshRate != 0) && (!(flags & SWM_WINDOWED)) && (!(flags & SWM_USECLOSESTFREQ))) {
			unsigned int modeCount = lpD3D9->GetAdapterModeCount (adapter, deviceDisplayFormat);

			unsigned int i;
			for (i = 0; i < modeCount; i++) {
				HRESULT hr = lpD3D9->EnumAdapterModes (adapter, deviceDisplayFormat, i, &d3dDisplayMode);
				if (!FAILED (hr)) {
					if ((d3dDisplayMode.Width == xRes) && (d3dDisplayMode.Height == yRes) && (d3dDisplayMode.Format	== deviceDisplayFormat) &&
						(d3dDisplayMode.RefreshRate == monRefreshRate))
						break;
				}
			}
			if (i == modeCount)
				return (0);
		}

		lfbCopyMode = (config.dx9ConfigBits & CFG_DX9LFBCOPYMODE);
		this->bufferNum = bufferNum;

		D3DDEVTYPE devType = (device != 0) ? GetDeviceType (device-1) : D3DDEVTYPE_HAL;

		d3dPresentParameters.BackBufferWidth = xRes;
		d3dPresentParameters.BackBufferHeight = yRes;
		d3dPresentParameters.BackBufferFormat = renderTargetFormat;
		d3dPresentParameters.BackBufferCount = lfbCopyMode ? 1 : bufferNum-1;
		d3dPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dPresentParameters.MultiSampleQuality = 0;
		d3dPresentParameters.SwapEffect = (flags & SWM_WINDOWED) ? (lfbCopyMode ? D3DSWAPEFFECT_COPY : D3DSWAPEFFECT_FLIP) : D3DSWAPEFFECT_FLIP;
		d3dPresentParameters.hDeviceWindow = hWnd;
		d3dPresentParameters.Windowed = (flags & SWM_WINDOWED) ? TRUE : FALSE;
		d3dPresentParameters.EnableAutoDepthStencil = FALSE;
		d3dPresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		d3dPresentParameters.FullScreen_RefreshRateInHz = (flags & SWM_WINDOWED) ? 0 : monRefreshRate;
		d3dPresentParameters.PresentationInterval = (config.dx9ConfigBits & CFG_DX9NOVERTICALRETRACE) ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;

		DWORD behaviorFlags = ((flags & SWM_MULTITHREADED) ? D3DCREATE_MULTITHREADED : 0) | D3DCREATE_FPU_PRESERVE | D3DCREATE_NOWINDOWCHANGES | D3DCREATE_MIXED_VERTEXPROCESSING;


		HRESULT hr = lpD3D9->CreateDevice (adapter, devType, hWnd, behaviorFlags, &d3dPresentParameters, &lpD3Ddevice9);
		if (!FAILED (hr)) {
			if (GetSwapChainAndBackBuffers ()) {
				this->adapter = adapter;
				this->deviceType = devType;

				lpD3D9->GetDeviceCaps (adapter, devType, &d3dCaps9);
				if (Init3D ()) {
					actRenderTarget = 1;		//BackBuffer
					return (1);
				}
				Deinit3D ();
			}
			lpD3Ddevice9->Release ();
		} else {
			DEBUGLOG(0,"\n   Error:  DX9General::SetWorkingMode: CreateDevice has failed, error code: %0x", hr);
			DEBUGLOG(1,"\n   Hiba: DX9General::SetWorkingMode: CreateDevice nem sikerült, hibakód: %0x", hr);
		}
	}
	return (0);
}


int			DX9General::ModifyWorkingMode (unsigned int xRes, unsigned int yRes, unsigned int bitPP, unsigned int monRefreshRate,
										   unsigned int /*bufferNum*/, unsigned int /*flags*/)
{
	//flags = (createFlags & SWM_WINDOWED) | (flags & ~SWM_WINDOWED);

	inScene = 0;
	D3DFORMAT renderTargetFormat = (bitPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_A8R8G8B8;

	if ((xRes != d3dPresentParameters.BackBufferWidth) || (yRes != d3dPresentParameters.BackBufferHeight) ||
		/*(bufferNum != this->bufferNum) ||*/
		(!d3dPresentParameters.Windowed && ((renderTargetFormat != d3dPresentParameters.BackBufferFormat) || (monRefreshRate != d3dPresentParameters.FullScreen_RefreshRateInHz)))) {

		Deinit3D ();
		ReleaseSwapChainAndBackBuffers ();

		d3dPresentParameters.BackBufferWidth = xRes;
		d3dPresentParameters.BackBufferHeight = yRes;
		if (!d3dPresentParameters.Windowed) {
			this->renderTargetFormat = renderTargetFormat;
			d3dPresentParameters.BackBufferFormat = renderTargetFormat;
			d3dPresentParameters.FullScreen_RefreshRateInHz = monRefreshRate;

			SetWindowPos (d3dPresentParameters.hDeviceWindow, HWND_TOPMOST, 0, 0, xRes, yRes, /*SWP_FRAMECHANGED |*/ SWP_NOMOVE /*| SWP_NOSIZE*/ | SWP_NOSENDCHANGING);
		}
		//d3dPresentParameters.BackBufferCount = bufferNum-1;

		HRESULT hr = lpD3Ddevice9->Reset (&d3dPresentParameters);

		if (!FAILED (hr)) {
			if (GetSwapChainAndBackBuffers ()) {
				if (Init3D ()) {
					actRenderTarget = 1;		//BackBuffer
					return (1);
				}
				Deinit3D ();
			}
		}
		lpD3Ddevice9->Release ();
		return (0);
	}
	return (1);
}


void		DX9General::RestoreWorkingMode ()
{
	Deinit3D ();
	ReleaseSwapChainAndBackBuffers ();
	if (!d3dPresentParameters.Windowed) {
		SetWindowLong (d3dPresentParameters.hDeviceWindow, GWL_STYLE, (hwndStyle));
		SetWindowLong (d3dPresentParameters.hDeviceWindow, GWL_EXSTYLE, (hwndStyleEx));
		SetWindowPos (d3dPresentParameters.hDeviceWindow, (hwndStyleEx & WS_EX_TOPMOST) ? HWND_TOPMOST : HWND_NOTOPMOST,
					  0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOSENDCHANGING);
	}
	lpD3Ddevice9->Release ();
}


int			DX9General::GetCurrentDisplayMode (unsigned short adapter, unsigned short /*device*/, DisplayMode* displayMode)
{
	int	success = 0;
	if (InitDX9 ()) {
		if (adapter < GetNumOfDX9Adapters ()) {
			LPDIRECT3D9 lpD3D9 = GetDirect3D9 ();
			
			D3DDISPLAYMODE d3dDisplayMode;
			lpD3D9->GetAdapterDisplayMode (adapter, &d3dDisplayMode);

			displayMode->xRes = d3dDisplayMode.Width;
			displayMode->yRes = d3dDisplayMode.Height;
			displayMode->refreshRate = d3dDisplayMode.RefreshRate;
			ConvertToPixFmt (d3dDisplayMode.Format, &displayMode->pixFmt);
			//displayMode->bitDepth = displayMode->pixFmt.BitPerPixel;

			success = 1;
		}
		UninitDX9 ();
	}
	
	return (success);
}


int			DX9General::EnumDisplayModes (unsigned short adapter, unsigned short /*device*/, unsigned int /*byRefreshRates*/,
										  DISPLAYMODEENUMCALLBACK callbackFunc)
{
	LPDIRECT3D9		lpD3D9 = NULL;
	D3DFORMAT		displayFormatsD3D9[2] = {D3DFMT_X8R8G8B8, /*D3DFMT_X1R5G5B5,*/ D3DFMT_R5G6B5};


	if (InitDX9 ()) {
		lpD3D9 = GetDirect3D9 ();
		for (int i = 0; i < 2; i++) {
			unsigned int modeCount = lpD3D9->GetAdapterModeCount (adapter, displayFormatsD3D9[i]);

			D3DDISPLAYMODE pMode;

			for (unsigned int j = 0; j < modeCount; j++) {
				HRESULT hr = lpD3D9->EnumAdapterModes (adapter, displayFormatsD3D9[i], j, &pMode);
				if (!FAILED (hr)) {
					DisplayMode displayMode;
					displayMode.xRes = pMode.Width;
					displayMode.yRes = pMode.Height;
					displayMode.refreshRate = pMode.RefreshRate;
					ConvertToPixFmt (pMode.Format, &displayMode.pixFmt);
					(*callbackFunc) (&displayMode);

					/*if (pMode.Format == D3DFMT_X8R8G8B8) {
						ZeroMemory (&displayMode.pixFmt, sizeof (_pixfmt));
						displayMode.pixFmt.BitPerPixel = 8;
						(*callbackFunc) (&displayMode);
					}*/
				}
			}
		}
		UninitDX9 ();
		return (1);
	}
	return (0);
}


/* Ez a függvény csak a naplózó verzióban szerepel: kiírja a meghajtó képességeit. */
void		DX9General::WriteOutRendererCaps ()
{
/*D3DDEVICEDESC7	devdesc;
DDCAPS			ddcaps, helcaps;

	ZeroMemory(&ddcaps, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);
	CopyMemory(&helcaps, &ddcaps, sizeof(ddcaps));
	lpDD->GetCaps(&ddcaps, &helcaps);
	DEBUGLOG(2,"\n\n------------------------- dgVoodoo: DirectDraw caps ----------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n\
----------------------- dgVoodoo: DirectDraw HEL caps --------------------------\n \
				dwCaps: %x\n\
				dwCaps2: %x\n\
				dwCKeyCaps: %x\n\
				dwFXCaps: %x\n",
				ddcaps.dwCaps,
				ddcaps.dwCaps2,
				ddcaps.dwCKeyCaps,
				ddcaps.dwFXCaps,
				helcaps.dwCaps,
				helcaps.dwCaps2,
				helcaps.dwCKeyCaps,
				helcaps.dwFXCaps);

	lpD3Ddevice->GetCaps(&devdesc);
	DEBUGLOG(2,"\n------------------------- dgVoodoo: driver description -------------------------\n \
				dwDevCaps: %x\n\
			dpcLineCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
			dpcTriCaps\n\
				dwMiscCaps: %0x\n\
				dwRasterCaps: %0x\n\
				dwZCmpCaps: %0x\n\
				dwSrcBlendCaps: %0x\n\
				dwDestBlendCaps: %0x\n\
				dwAlphaCmpCaps: %0x\n\
				dwShadeCaps: %0x\n\
				dwTextureCaps: %0x\n\
				dwTextureFilterCaps: %0x\n\
				dwTextureBlendCaps: %0x\n\
				dwTextureAddressCaps: %0x\n\
				dwStippleWidth: %0x\n\
				dwStippleHeight: %0x\n\
				dwDeviceRenderBitDepth: %0x\n\
				dwDeviceZBufferBitDepth: %0x\n\
				dwMinTextureWidth: %0x\n\
				dwMinTextureHeight: %0x\n\
				dwMaxTextureWidth: %0x\n\
				dwMaxTextureHeight: %0x\n\
				dwMaxTextureRepeat: %0x\n\
				dwMaxTextureAspectRatio: %0x\n\
				dwMaxAnisotropy: %0x\n\
				dvGuardBandLeft: %f\n\
				dvGuardBandTop: %f\n\
				dvGuardBandRight: %f\n\
				dvGuardBandBottom: %f\n\
				dvExtentsAdjust: %f\n\
				dwStencilCaps: %0x\n\
				dwFVFCaps: %0x\n\
				dwTextureOpCaps: %0x\n\
				wMaxTextureBlendStages: %0x\n\
				wMaxSimultaneousTextures: %0x\n\
				dwMaxActiveLights: %0x\n\
				dvMaxVertexW: %f\n\
				wMaxUserClipPlanes: %0x\n\
				wMaxVertexBlendMatrices: %0x\n\
				dwVertexProcessingCaps: %0x\n", \
				devdesc.dwDevCaps, \
				devdesc.dpcLineCaps.dwMiscCaps, \
				devdesc.dpcLineCaps.dwRasterCaps, \
				devdesc.dpcLineCaps.dwZCmpCaps, \
				devdesc.dpcLineCaps.dwSrcBlendCaps, \
				devdesc.dpcLineCaps.dwDestBlendCaps, \
				devdesc.dpcLineCaps.dwAlphaCmpCaps, \
				devdesc.dpcLineCaps.dwShadeCaps, \
				devdesc.dpcLineCaps.dwTextureCaps, \
				devdesc.dpcLineCaps.dwTextureFilterCaps, \
				devdesc.dpcLineCaps.dwTextureBlendCaps, \
				devdesc.dpcLineCaps.dwTextureAddressCaps, \
				devdesc.dpcLineCaps.dwStippleWidth, \
				devdesc.dpcLineCaps.dwStippleHeight, \
				devdesc.dpcTriCaps.dwMiscCaps, \
				devdesc.dpcTriCaps.dwRasterCaps, \
				devdesc.dpcTriCaps.dwZCmpCaps, \
				devdesc.dpcTriCaps.dwSrcBlendCaps, \
				devdesc.dpcTriCaps.dwDestBlendCaps, \
				devdesc.dpcTriCaps.dwAlphaCmpCaps, \
				devdesc.dpcTriCaps.dwShadeCaps, \
				devdesc.dpcTriCaps.dwTextureCaps, \
				devdesc.dpcTriCaps.dwTextureFilterCaps, \
				devdesc.dpcTriCaps.dwTextureBlendCaps, \
				devdesc.dpcTriCaps.dwTextureAddressCaps, \
				devdesc.dpcTriCaps.dwStippleWidth, \
				devdesc.dpcTriCaps.dwStippleHeight, \
				devdesc.dwDeviceRenderBitDepth, \
				devdesc.dwDeviceZBufferBitDepth, \
				devdesc.dwMinTextureWidth, \
				devdesc.dwMinTextureHeight, \
				devdesc.dwMaxTextureWidth, \
				devdesc.dwMaxTextureHeight, \
				devdesc.dwMaxTextureRepeat, \
				devdesc.dwMaxTextureAspectRatio, \
				devdesc.dwMaxAnisotropy, \
				devdesc.dvGuardBandLeft, \
				devdesc.dvGuardBandTop, \
				devdesc.dvGuardBandRight, \
				devdesc.dvGuardBandBottom, \
				devdesc.dvExtentsAdjust, \
				devdesc.dwStencilCaps, \
				devdesc.dwFVFCaps, \
				devdesc.dwTextureOpCaps, \
				devdesc.wMaxTextureBlendStages, \
				devdesc.wMaxSimultaneousTextures, \
				devdesc.dwMaxActiveLights, \
				devdesc.dvMaxVertexW, \
				devdesc.wMaxUserClipPlanes, \
				devdesc.wMaxVertexBlendMatrices, \
				devdesc.dwVertexProcessingCaps);*/
}


int			DX9General::CreateFullScreenGammaControl (GammaRamp* originalGammaRamp)
{
	D3DCAPS9 d3dCaps9;

	lpD3D9->GetDeviceCaps (this->adapter, this->deviceType, &d3dCaps9);
	if (d3dCaps9.Caps2 & D3DCAPS2_FULLSCREENGAMMA) {
		D3DGAMMARAMP d3dGammaRamp;
		lpD3Ddevice9->GetGammaRamp (0, &d3dGammaRamp);

		int usesByteOnly = 1;
		for (int i = 0; i < 256; i++) {
			if ((d3dGammaRamp.red[i] & 0xFF00) || (d3dGammaRamp.green[i] & 0xFF00) || (d3dGammaRamp.blue[i] & 0xFF00)) {
				usesByteOnly = 0;
				break;
			}
		}
		if (usesByteOnly) {
			for (int i = 0; i < 256; i++) {
				d3dGammaRamp.red[i] <<= 8;
				d3dGammaRamp.green[i] <<= 8;
				d3dGammaRamp.blue[i] <<= 8;
			}
		}

		CopyMemory (originalGammaRamp, &d3dGammaRamp, sizeof (GammaRamp));
		return (1);
	}
	return (0);
}


void		DX9General::DestroyFullScreenGammaControl ()
{
}


void		DX9General::SetGammaRamp (GammaRamp* gammaRamp)
{
	D3DGAMMARAMP d3dGammaRamp;
	CopyMemory (&d3dGammaRamp, gammaRamp, sizeof (GammaRamp));

	lpD3Ddevice9->SetGammaRamp (0, D3DSGR_NO_CALIBRATION, &d3dGammaRamp);
}


RendererGeneral::DeviceStatus DX9General::GetDeviceStatus ()
{
	HRESULT hr = lpD3Ddevice9->TestCooperativeLevel ();

	DEBUGCHECK (0, hr == D3DERR_DRIVERINTERNALERROR, "\n   Panic: DX9General::GetDeviceStatus: internal driver error reported! What to do???");
	DEBUGCHECK (1, hr == D3DERR_DRIVERINTERNALERROR, "\n   Pánik: DX9General::GetDeviceStatus: belsõ driver-hiba! Mit kellene tennem???");

	if (hr == D3D_OK)
		return StatusOk;
	else if (hr == D3DERR_DEVICENOTRESET)
		return StatusLostCanBeReset;
	else
		return StatusLost;
}


void		DX9General::PrepareForDeviceReset ()
{
	if (dx9Drawing != NULL)
		dx9Drawing->ReleaseDefaultPoolResources ();
	
	if (dx9LfbDepth != NULL)
		dx9LfbDepth->ReleaseDefaultPoolResources ();

	ReleaseSwapChainAndBackBuffers ();
}


int			DX9General::DeviceReset ()
{
	HRESULT hr = lpD3Ddevice9->Reset (&d3dPresentParameters);
	return (!FAILED (hr));
}


int			DX9General::ReallocResources ()
{
	if (GetSwapChainAndBackBuffers ()) {
		if (dx9Drawing != NULL) {
			if (!dx9Drawing->ReallocDefaultPoolResources ()) {
				return (0);
			}
		}

		if (dx9LfbDepth != NULL) {
			if (!dx9LfbDepth->ReallocDefaultPoolResources ()) {
				return (0);
			}
		}

		if (dx9Texturing != NULL) {
			if (!dx9Texturing->ReallocDefaultPoolResources ()) {
				return (0);
			}
		}

		if (usePixelShaders)
			SetupStaticConstantPixelShaderRegs ();

		return (1);
	}
	return (0);
}


int			DX9General::BeginScene ()
{
	DEBUGLOG (2, "\nBeginScene");
	inScene = !FAILED (lpD3Ddevice9->BeginScene ());
	return (inScene);
}


void		DX9General::EndScene ()
{
	DEBUGLOG (2, "\nEndScene");
	lpD3Ddevice9->EndScene ();
	inScene = 0;
}


void		DX9General::ShowContentChangesOnFrontBuffer ()
{
	RefreshDisplayByBuffer (FrontBuffer);
}


int		DX9General::RefreshDisplayByBuffer (int buffer)
{
	if (lfbCopyMode) {

		int lInScene = inScene;
		if (inScene) {
			EndScene ();
		}

		LPDIRECT3DSURFACE9 realBackBuffer = NULL;
		
		lpD3DSwapChain9->GetBackBuffer (0, D3DBACKBUFFER_TYPE_MONO, &realBackBuffer);
		lpD3Ddevice9->StretchRect (renderBuffers[buffer], NULL, realBackBuffer, NULL, D3DTEXF_POINT);
		lpD3DSwapChain9->Present (NULL, NULL, NULL, NULL, 0);
		realBackBuffer->Release ();

		if (lInScene) {
			BeginScene ();
		}

		return (1);
	}
	return (0);
}


void		DX9General::SwitchToNextBuffer (unsigned int swapInterval, unsigned int renderBuffer)
{
	if (swapInterval > 1) {
		swapInterval--;

		for(unsigned int i = 0; i < swapInterval; i++)	{
			D3DRASTER_STATUS rasterStatus;

			while(1)	{
				rasterStatus.InVBlank = FALSE;
				/*HRESULT hr =*/ lpD3Ddevice9->GetRasterStatus (0, &rasterStatus);
				//if ((hr != D3D_OK) && ((hr != D3DERR_INVALIDCALL) || (d3dCaps9.Caps & D3DCAPS_READ_SCANLINE))
				//	break;
				if (!rasterStatus.InVBlank)
					break;
			}
			while(1)	{
				rasterStatus.InVBlank = TRUE;
				/*HRESULT hr =*/ lpD3Ddevice9->GetRasterStatus (0, &rasterStatus);
				//if ((hr != D3D_OK) && ((hr != D3DERR_INVALIDCALL) || (d3dCaps9.Caps & D3DCAPS_READ_SCANLINE))
				//	break;
				if (rasterStatus.InVBlank)
					break;
			}
		}
	}
	
	dx9LfbDepth->ConvertToGrayScale (renderBuffers[BackBuffer]);
	
	if (lfbCopyMode) {
		RefreshDisplayByBuffer (BackBuffer);
		
		LPDIRECT3DSURFACE9 firstSurface = renderBuffers[0];

		unsigned int i;
		for (i = 1; i < bufferNum; i++)
			renderBuffers[i-1] = renderBuffers[i];

		renderBuffers[i-1] = firstSurface;
	} else {
		lpD3DSwapChain9->Present (NULL, NULL, NULL, NULL, 0);
	}

	if (renderBuffers[renderBuffer] != NULL)
		lpD3Ddevice9->SetRenderTarget(0, renderBuffers[actRenderTarget = renderBuffer]);
}


int			DX9General::WaitUntilBufferSwitchDone ()
{
	/*if (!(createFlags & SWM_WINDOWED)) {
		HRESULT hr;
		int buffer = renderTarget == 0 ? bufferNum - 1 : renderTarget;

		while ((hr = renderBuffers[buffer]->GetFlipStatus (DDGFS_ISFLIPDONE)) == DDERR_WASSTILLDRAWING)
			Sleep (1);

		if (FAILED (hr))
			return (0);
	}
	return (1);*/
	return (0);
}


int			DX9General::GetNumberOfPendingBufferFlippings ()
{
	/*HRESULT hr;
	int		flipNum = 0;
	
	for (unsigned int i = 1; i < bufferNum; i++) {
		if (createFlags & SWM_WINDOWED)
			hr = renderBuffers[i]->GetBltStatus (DDGBS_ISBLTDONE);
		else
			hr = renderBuffers[i]->GetFlipStatus (DDGFS_ISFLIPDONE);

		if (hr == DDERR_WASSTILLDRAWING)
			flipNum++;
	}
	return flipNum;*/
	return (0);
}


int			DX9General::GetVerticalBlankStatus ()
{
	D3DRASTER_STATUS rasterStatus;
	
	HRESULT hr = lpD3Ddevice9->GetRasterStatus (0, &rasterStatus);

	getRasterStatusCount++;

	if (FAILED (hr))
		return (getRasterStatusCount & 1);
	else
		return rasterStatus.InVBlank;
}


unsigned int DX9General::GetCurrentScanLine ()
{
	D3DRASTER_STATUS rasterStatus;
	
	HRESULT hr = lpD3Ddevice9->GetRasterStatus (0, &rasterStatus);

	getRasterStatusCount++;

	if (FAILED (hr))
		return (getRasterStatusCount % appYRes);
	else
		return rasterStatus.ScanLine;
}


int			DX9General::SetRenderTarget (int buffer)
{
	if (renderBuffers[buffer] != NULL) {
		lpD3Ddevice9->SetRenderTarget(0, renderBuffers[actRenderTarget = buffer]);
		return (1);
	}
	return (0);
}


void		DX9General::ClearBuffer (unsigned int flags, RECT* rect, unsigned int color, float depth)
{
	DWORD clearFlags = ((flags & CLEARFLAG_COLORBUFFER) ? D3DCLEAR_TARGET : 0) |
					   ((flags & CLEARFLAG_DEPTHBUFFER) ? D3DCLEAR_ZBUFFER : 0);

	//if (stencilBitDepth) clearFlags |= D3DCLEAR_STENCIL;

	if (rect != NULL) {
		D3DRECT d3dRect;

		d3dRect.x1 = rect->left;
		d3dRect.x2 = rect->right;
		d3dRect.y1 = rect->top;
		d3dRect.y2 = rect->bottom;
		lpD3Ddevice9->Clear(1, &d3dRect, clearFlags, color, depth, 0);
	} else {
		lpD3Ddevice9->Clear(0, NULL, clearFlags, color, depth, 0);
	}
}


void		DX9General::SetClipWindow (RECT* clipRect)
{
D3DVIEWPORT9		viewport;

	viewport.X = (DWORD) clipRect->left;
	viewport.Width = (DWORD) (clipRect->right - clipRect->left);
	viewport.Y = (DWORD) clipRect->top;
	viewport.Height = (DWORD) (clipRect->bottom - clipRect->top);
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	lpD3Ddevice9->SetViewport(&viewport);
}


void		DX9General::SetCullMode (GrCullMode_t mode)
{
	lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, dxCullModeLookUp[mode]);
}


void		DX9General::SetColorAndAlphaWriteMask (FxBool rgb, FxBool alpha)
{
	if (d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) {
		lpD3Ddevice9->SetRenderState (D3DRS_COLORWRITEENABLE,
			(rgb ? (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE) : 0) |
			(alpha ? D3DCOLORWRITEENABLE_ALPHA : 0));
	} else {
		//Fallback path
		if (rgb) {
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlendEnabled);
			lpD3Ddevice9->SetRenderState(D3DRS_SRCBLEND, srcBlendFunc);
			lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, dstBlendFunc);
		} else {
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			lpD3Ddevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		if (d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_SEPARATEALPHABLEND) {
			if (alpha) {
				lpD3Ddevice9->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
				lpD3Ddevice9->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
				lpD3Ddevice9->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
			} else {
				lpD3Ddevice9->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
				lpD3Ddevice9->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
				lpD3Ddevice9->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
			}
		}
	}
}


void		DX9General::EnableAlphaBlending (int enable)
{
	alphaBlendEnabled = enable ? TRUE : FALSE;

	if ((d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) || astate.colorMask)
		lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, enable ? TRUE : FALSE);
		
	if ((d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) || astate.alphaWriteMask) {
		if (d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_SEPARATEALPHABLEND)
			lpD3Ddevice9->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, enable ? TRUE : FALSE);
	}
}


void		DX9General::SetRgbAlphaBlendingFunction (GrAlphaBlendFnc_t srcFunc, GrAlphaBlendFnc_t dstFunc)
{
	srcBlendFunc = alphaModeLookupSrc[srcFunc];
	dstBlendFunc = alphaModeLookupDst[dstFunc];
	if ((d3dCaps9.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) || astate.colorMask) {
		lpD3Ddevice9->SetRenderState(D3DRS_SRCBLEND, srcBlendFunc);
		lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, dstBlendFunc);
	}
}


void		DX9General::SetAlphaAlphaBlendingFunction (GrAlphaBlendFnc_t /*srcFunc*/, GrAlphaBlendFnc_t /*dstFunc*/)
{
}


void		DX9General::SetDestAlphaBlendFactorDirectly (DestAlphaBlendFactor destAlphaBlendFactor)
{
	lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, destAlphaBlendFactor == DestAlphaFactorZero ? D3DBLEND_ZERO : D3DBLEND_ONE);
}


void		DX9General::EnableAlphaTesting (int enable)
{
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE , enable ? TRUE : FALSE);
}


void		DX9General::SetAlphaTestFunction (GrCmpFnc_t function)
{
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHAFUNC, cmpFuncLookUp[function]);
}


void		DX9General::SetAlphaTestRefValue (GrAlpha_t value)
{
	lpD3Ddevice9->SetRenderState(D3DRS_ALPHAREF, value);
}


void		DX9General::SetFogColor (unsigned int fogColor)
{
	lpD3Ddevice9->SetRenderState(D3DRS_FOGCOLOR, (D3DCOLOR) fogColor);
}


void		DX9General::EnableFog (int enable)
{
	lpD3Ddevice9->SetRenderState(D3DRS_FOGENABLE, enable ? TRUE : FALSE);
}


void		DX9General::SetDitheringMode (GrDitherMode_t mode)
{
	lpD3Ddevice9->SetRenderState(D3DRS_DITHERENABLE, ((mode == GR_DITHER_2x2) || (mode == GR_DITHER_4x4)) ? TRUE : FALSE);
}


void		DX9General::SetConstantColor (unsigned int constantColor)
{
	if (usePixelShaders) {
		constColor = constantColor;
		float	c[3*4];
		c[0] = ((float)((constantColor >> 16) & 0xFF)) / 255.0f;
		c[1] = ((float)((constantColor >> 8) & 0xFF)) / 255.0f;
		c[2] = ((float)(constantColor & 0xFF)) / 255.0f;
		c[3] = ((float)((constantColor >> 24) & 0xFF)) / 255.0f;
		c[4] = -c[0];
		c[5] = -c[1];
		c[6] = -c[2];
		c[7] = -c[3];
		c[8] = c[4] + 1.0f;
		c[9] = c[5] + 1.0f;
		c[10] = c[6] + 1.0f;
		c[11] = c[7] + 1.0f;
		lpD3Ddevice9->SetPixelShaderConstantF (0, c, 3);
	} else {
		lpD3Ddevice9->SetRenderState(D3DRS_TEXTUREFACTOR, (D3DCOLOR) constantColor);
	}
}


void		DX9General::ConfigureTexelPixelPipeLine (GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor, 
													 GrCombineFunction_t alpha_function, GrCombineFactor_t alpha_factor,
													 FxBool rgb_invert, FxBool alpha_invert)
{
	if (usePixelShaders)
		ConfigureTexelPixelPipeLine14 (rgb_function, rgb_factor, alpha_function, alpha_factor, rgb_invert, alpha_invert);
}


void		DX9General::ConfigureColorPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
	if (usePixelShaders)
		ConfigureColorPixelPipeLine14 (func, factor, local, other, invert);
	else
		ConfigureColorPixelPipeLineFF (func, factor, local, other, invert);
}


void		DX9General::ConfigureAlphaPixelPipeLine (GrCombineFunction_t func, GrCombineFactor_t factor,
													 GrCombineLocal_t local, GrCombineOther_t other, FxBool invert)
{
	if (usePixelShaders)
		ConfigureAlphaPixelPipeLine14 (func, factor, local, other, invert);
	else
		ConfigureAlphaPixelPipeLineFF (func, factor, local, other, invert);
}


void		DX9General::ComposeConcretePixelPipeLine ()
{
	if (usePixelShaders)
		ComposePixelShader ();
	else
		ComposeFFShader ();
}


void		DX9General::GetCkMethodPreferenceOrder (ColorKeyMethod*	ckMethodPreference)
{
	if (usePixelShaders) {
		ckMethodPreference[0] = RendererGeneral::Native;
		ckMethodPreference[1] = RendererGeneral::AlphaBased;
		ckMethodPreference[2] = RendererGeneral::Disabled;
	} else {
		ckMethodPreference[0] = RendererGeneral::AlphaBased;
		ckMethodPreference[1] = RendererGeneral::Disabled;
	}
}


int			DX9General::IsAlphaTestUsedForAlphaBasedCk ()
{
	return (!usePixelShaders);
}


int			DX9General::EnableAlphaColorkeying (AlphaCkModeInPipeline alphaCkModeInPipeline, int alphaOutputUsed)
{
	if (usePixelShaders) {

		colorKeyingMethod = (alphaCkModeInPipeline != AlphaDisabled) ? AlphaBased : Disabled;

	} else {

		DWORD op = D3DTOP_DISABLE;
		if (alphaCkModeInPipeline != AlphaDisabled) {

			//Az AlphaTest milyen függvényt használ? < AlphaRef vagy > AlphaRef ?
			DWORD arg = D3DTA_TEXTURE;
			op = D3DTOP_MODULATE;
			if (alphaCkModeInPipeline == AlphaForFncLess) {
				op = D3DTOP_ADD;
				arg = D3DTA_TEXTURE | D3DTA_COMPLEMENT;
			}
			
			//Ha az alpha-kimenet nincs felhasználva, akkor használhatjuk közvetlenül az alpha-textúrát
			if (!alphaOutputUsed)
				op = D3DTOP_SELECTARG1;

			alphaCkAlphaArg1[0] = arg;
		}
		alphaCkAlphaOp[0] = op;
	}
	
	return (1);
}


int 		DX9General::EnableNativeColorKeying (int enable, int /*forTnt*/)
{
	colorKeyingMethod = enable ? Native : Disabled;
	return (usePixelShaders);
}


int			DX9General::EnableAlphaItRGBLighting (int enable)
{
	alphaItRgbLightingEnabled = enable;
	return (usePixelShaders);
}


int			DX9General::GetTextureUsageFlags ()
{
	int	textureUsage = STATE_TEXTURE0;
	int flags = 0;
	
	if (usePixelShaders) {
		
		//unsigned int tUsage = STATE_TEXTURE0;
		if ((GetRegType(texColorTemplateRegs[COther]) == D3DSPR_TEMP) ||
			(GetRegType(texAlphaTemplateRegs[COther]) == D3DSPR_TEMP)) {
			flags |= textureUsage;
			textureUsage <<= 1;
		}
		if (colorKeyingMethod != Disabled) {
			flags |= textureUsage;
		}
		

	} else {

		for(int i = 0; i < maxUsedSamplerNum; i++)	{
			if ( ((this->cachedAlphaArg1[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) || ((this->cachedAlphaArg2[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) ||
				 ((this->cachedColorArg1[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) || ((this->cachedColorArg2[i] & D3DTA_SELECTMASK) == D3DTA_TEXTURE) )
				 flags |= textureUsage;
			textureUsage <<= 1;
		}

	}

	return (flags);
}


int			DX9General::GetNumberOfTextureStages ()
{
	return MIN (d3dCaps9.MaxTextureBlendStages, d3dCaps9.MaxSimultaneousTextures);
}


void		DX9General::SaveRenderState (RenderStateSaveType renderStateSaveType)
{
	DWORD*	savedRenderStates;
	DWORD*	savedTextureStageStates;
	DWORD*  savedSamplerStates;

	int i;
	int pos = 0;
	
	switch (renderStateSaveType) {
		
		case ScreenShotLabel:
			savedRenderStates = scrShotSaveStates;
			savedTextureStageStates = scrShotSaveTSStates;
			savedSamplerStates = scrShotSamplerStates;
			break;

		case LfbNoPipeline:
		case GrayScaleRendering:
			savedRenderStates = lfbSaveStatesNoPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			savedSamplerStates = lfbSaveSamplerStates;
			break;

		case LfbPipeline:
			savedRenderStates = lfbSaveStatesPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			savedSamplerStates = lfbSaveSamplerStates;
			break;

		case GammaCorrection:
			savedRenderStates = gammaSaveStates;
			savedTextureStageStates = gammaSaveTSStates;
			savedSamplerStates = NULL;
			break;

		default:
			return;
	}


	for(i = 0; savedRenderStates[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice9->GetRenderState ((D3DRENDERSTATETYPE) savedRenderStates[i], savedState+(pos++));

	if (savedSamplerStates != NULL) {
		for(i = 0; savedSamplerStates[i] != STATEDESC_DELIMITER; i++)
			lpD3Ddevice9->GetSamplerState (0, (D3DSAMPLERSTATETYPE) savedSamplerStates[i], savedState+(pos++));
	}

	if (usePixelShaders) {
		//lpD3Ddevice9->GetPixelShader (&savedPixelShader);
		lpD3Ddevice9->GetPixelShaderConstantF (0, savedPixelShaderConstant, 1);
	} else {
		for(i = 0; savedTextureStageStates[i] != STATEDESC_DELIMITER; i+=2)
			lpD3Ddevice9->GetTextureStageState (savedTextureStageStates[i], (D3DTEXTURESTAGESTATETYPE) savedTextureStageStates[i+1],
												savedState+(pos++));
	} 

	/*lpD3Ddevice9->GetTexture(0, (LPDIRECT3DBASETEXTURE9 *) savedState + (pos++));
	lpD3Ddevice9->GetTexture(1, (LPDIRECT3DBASETEXTURE9 *) savedState + pos);*/
	
}


void		DX9General::RestoreRenderState (RenderStateSaveType renderStateSaveType)
{
	DWORD*	savedRenderStates = NULL;
	DWORD*	savedTextureStageStates = NULL;
	DWORD*  savedSamplerStates = NULL;
	
	int i;
	int pos = 0;

	switch (renderStateSaveType) {
		
		case ScreenShotLabel:
			savedRenderStates = scrShotSaveStates;
			savedTextureStageStates = scrShotSaveTSStates;
			savedSamplerStates = scrShotSamplerStates;
			break;

		case LfbNoPipeline:
		case GrayScaleRendering:
			savedRenderStates = lfbSaveStatesNoPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			savedSamplerStates = lfbSaveSamplerStates;
			break;

		case LfbPipeline:
			savedRenderStates = lfbSaveStatesPipeline;
			savedTextureStageStates = lfbSaveTSStatesNoPipeline;
			savedSamplerStates = lfbSaveSamplerStates;
			break;

		case GammaCorrection:
			savedRenderStates = gammaSaveStates;
			savedTextureStageStates = gammaSaveTSStates;
			savedSamplerStates = NULL;
			break;
	}

	for(i = 0; savedRenderStates[i] != STATEDESC_DELIMITER; i++)
		lpD3Ddevice9->SetRenderState ((D3DRENDERSTATETYPE) savedRenderStates[i], savedState[pos++]);

	if (savedSamplerStates != NULL) {
		for(i = 0; savedSamplerStates[i] != STATEDESC_DELIMITER; i++)
			lpD3Ddevice9->SetSamplerState (0, (D3DSAMPLERSTATETYPE) savedSamplerStates[i], savedState[pos++]);
	}

	if (usePixelShaders) {
		lpD3Ddevice9->SetPixelShader (currentPixelShader);
		lpD3Ddevice9->SetPixelShaderConstantF (0, savedPixelShaderConstant, 1);
	} else {
		for(i = 0; savedTextureStageStates[i] != STATEDESC_DELIMITER; i+=2)
			lpD3Ddevice9->SetTextureStageState (savedTextureStageStates[i], (D3DTEXTURESTAGESTATETYPE) savedTextureStageStates[i+1],
												savedState[pos++]);
	}


	lpD3Ddevice9->SetTexture(0, (LPDIRECT3DBASETEXTURE9) astate.lpDDTex[0]);
	lpD3Ddevice9->SetTexture(1, (LPDIRECT3DBASETEXTURE9) astate.lpDDTex[1]);
}


void		DX9General::SetRenderState (RenderStateSaveType renderStateSaveType, int lfbEnableNativeCk, int lfbUseConstAlpha,
										unsigned int lfbConstAlpha)
{
	int usedShader;
	
	switch (renderStateSaveType) {
		case ScreenShotLabel:
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	
			lpD3Ddevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			lpD3Ddevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			lpD3Ddevice9->SetRenderState(D3DRS_FOGENABLE, FALSE);

			if (!usePixelShaders) {
				lpD3Ddevice9->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	

				lpD3Ddevice9->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			} else {
				usedShader = ScrShot;
			}

			break;

		case LfbNoPipeline:
		case GrayScaleRendering:
			lpD3Ddevice9->SetRenderState(D3DRS_FOGENABLE, FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			break;

		case LfbPipeline:
			lpD3Ddevice9->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			break;

		case GammaCorrection:
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			lpD3Ddevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			lpD3Ddevice9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			if (!usePixelShaders) {
				lpD3Ddevice9->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			} else
				usedShader = GammaCorr;
			break;
	}

	if ((renderStateSaveType == LfbNoPipeline) || (renderStateSaveType == LfbPipeline)) {

		lpD3Ddevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		if (usePixelShaders) {

			if (!lfbUseConstAlpha) {
				usedShader = lfbEnableNativeCk ? LfbTileWithCk : LfbTileWithCk;
			} else {
				float c[4] = {0.0f, 0.0f, 0.0f, ((float) lfbConstAlpha) / 255.0f};
				lpD3Ddevice9->SetPixelShaderConstantF (0, c, 1);
				usedShader = lfbEnableNativeCk ? LfbTileConstAlphaWithCk : LfbTileConstAlpha;
			}

		} else {
			lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			lpD3Ddevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			lpD3Ddevice9->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			lpD3Ddevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			if (!lfbUseConstAlpha) {
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			} else {
				lpD3Ddevice9->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
				lpD3Ddevice9->SetRenderState(D3DRS_TEXTUREFACTOR, lfbConstAlpha << 24);
			}
			
			DEBUGLOG (0, "\n   Warning: DX9General::SetRenderState: cannot use native colorkeying by fixed function shaders for texture tiling!!");
			DEBUGLOG (1, "\n   Figyelmeztetés: DX9General::SetRenderState: nem tudok natív colorkeyinget használni a textúracsempékhez FF shaderekkel!!");
		}
	}

	if (renderStateSaveType == GrayScaleRendering) {
		float c[4] = {0.3125f, 0.5f, 0.1875f, 0.0f};
		lpD3Ddevice9->SetPixelShaderConstantF (0, c, 1);
		usedShader = GrayScaleRenderer;
	}

	if (usePixelShaders)
		lpD3Ddevice9->SetPixelShader (lpD3DStaticShaders[usedShader]);
}


int			DX9General::UsesPixelShaders ()
{
	return usePixelShaders;
}


int			DX9General::Is3DNeeded ()
{
	return dx9LfbDepth->Is3DNeeded ();
}