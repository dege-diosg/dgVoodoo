/*--------------------------------------------------------------------------------- */
/* ConfigExport.cpp - dgVoodooSetup text export of configuration                    */
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


#include "dgVoodooSetup.h"
#include "dgVoodooConfig.h"
#include "Version.h"
#include "Resource.h"
#include "stdio.h"

#define	ITEM_WINDOWS				0
#define ITEM_DOS					1
#define ITEM_ENABLED				2
#define ITEM_DISABLED				3
#define ITEM_CTRALTDISABLED			ITEM_DISABLED
#define ITEM_REFRATEDISABLED		ITEM_DISABLED
#define ITEM_FULLSCR				4
#define ITEM_WINDOWED				5
#define ITEM_16BIT					6
#define ITEM_32BIT					7
#define ITEM_OPTIMAL				8
#define ITEM_DETBYDESKTOP			9
#define ITEM_GRABCLIPBOARD			10
#define ITEM_GRABTOFILE				11
//#define ITEM_REFRATEALIGNEDTOMONITOR 12
//#define ITEM_REFRATEALIGNEDTOAPP    13

unsigned int	invalidConfigStrs[numberOfLanguages][1] =
				{	{IDS_CEINVALIDCFGERROR},
					{IDS_CEINVALIDCFGERROR_HUN} };

unsigned int	formatStrIds[numberOfLanguages][1] =
				{	{IDR_FORMATSTRENG},
					{IDR_FORMATSTRENG/*IDR_FORMATSTRHUN*/} };

int		ExportConfiguration (char *filename, const dgVoodooConfig& config)
{
FILE			*file;
char			texMemSize[32];
char			resolution[32];
char			lfbAccType[128];
char			wBufferingMethod[128];
char			lfbTexTilingMethod[128];
char			ckMethod[128];
char			refRate[32];
char			vesaMemSize[32];

HRSRC			hRsrcFormatStr;
char*			pFormatString;
char			itemDos[64];
char			itemWindows[64];
char			itemEnabled[64];
char			itemDisabled[64];
char			itemFullScr[64];
char			itemWindowed[64];
char			item16Bit[64];
char			item32Bit[64];
char			itemOptimal[64];
char			itemDetByDestop[64];
char			itemShotToClipBoard[64];
char			itemShotToFile[64];
//char			itemRefRateMethodByMonitor[64];
//char			itemRefRateMethodByApp[64];
char*			itemStrings[16];
char			errBuffer[256];

	resourceManager.GetString (config.language, IDS_CEDOS, itemDos);
	resourceManager.GetString (config.language, IDS_CEWINDOWS, itemWindows);
	resourceManager.GetString (config.language, IDS_CEENABLED, itemEnabled);
	resourceManager.GetString (config.language, IDS_CEDISABLED, itemDisabled);
	resourceManager.GetString (config.language, IDS_CEFULLSCR, itemFullScr);
	resourceManager.GetString (config.language, IDS_CEWINDOWED, itemWindowed);
	resourceManager.GetString (config.language, IDS_CE16BIT, item16Bit);
	resourceManager.GetString (config.language, IDS_CE32BIT, item32Bit);
	resourceManager.GetString (config.language, IDS_CEOPTIMAL, itemOptimal);
	resourceManager.GetString (config.language, IDS_CEDETBYDESKTOP, itemDetByDestop);
	resourceManager.GetString (config.language, IDS_CESHOTTOCLIPBOARD, itemShotToClipBoard);
	resourceManager.GetString (config.language, IDS_CESHOTTOFILE, itemShotToFile);
	//GetString (itemRefRateMethodByMonitor, itemRefRateMethods[config.language][0]);
	//GetString (itemRefRateMethodByApp, itemRefRateMethods[config.language][1]);


	itemStrings[ITEM_DOS] = itemDos;
	itemStrings[ITEM_WINDOWS] = itemWindows;
	itemStrings[ITEM_ENABLED] = itemEnabled;
	itemStrings[ITEM_DISABLED] = itemDisabled;
	itemStrings[ITEM_FULLSCR] = itemFullScr;
	itemStrings[ITEM_WINDOWED] = itemWindowed;
	itemStrings[ITEM_16BIT] = item16Bit;
	itemStrings[ITEM_32BIT] = item32Bit;
	itemStrings[ITEM_OPTIMAL] = itemOptimal;
	itemStrings[ITEM_DETBYDESKTOP] = itemDetByDestop;
	itemStrings[ITEM_GRABCLIPBOARD] = itemShotToClipBoard;
	itemStrings[ITEM_GRABTOFILE] = itemShotToFile;
	//itemStrings[ITEM_REFRATEALIGNEDTOMONITOR] = itemRefRateMethodByMonitor;
	//itemStrings[ITEM_REFRATEALIGNEDTOAPP] = itemRefRateMethodByApp;

	sprintf(texMemSize, "%d kB", config.TexMemSize >> 10);

	resourceManager.GetString (config.language, IDS_RESDEFAULT, resolution);
	if ((config.dxres != 0) || (config.dyres != 0))
		sprintf(resolution, "%dx%d", config.dxres, config.dyres);

	resourceManager.GetString (config.language, IDS_REFRESHDEFAULT, refRate);
	if (config.RefreshRate != 0)
		sprintf(refRate, "%d Hz",config.RefreshRate);

	static unsigned int	lfbAccessStrIds[4] = {IDS_ENABLELFBACCESS, IDS_LFBDISABLEREAD, IDS_DISABLELFBWRITE, IDS_DISABLELFBACCESS};
	resourceManager.GetString (config.language, lfbAccessStrIds[GetLfbAccessValue (config.Flags)], lfbAccType);
	static unsigned int	ckMethodStrIds[4] = {IDS_CKAUTOMATIC, IDS_CKALPHABASED, IDS_CKNATIVE, IDS_NATIVEFORTNT};
	resourceManager.GetString (config.language, ckMethodStrIds[GetWBuffMethodValue (config.Flags)], ckMethod);

	sprintf (vesaMemSize, "%d kB", config.VideoMemSize);

	hRsrcFormatStr = FindResource (NULL, (LPCSTR) (formatStrIds[config.language][0]), (LPCSTR) 100);
	pFormatString = (char*) LockResource (LoadResource (NULL, hRsrcFormatStr));
	
	if ( (file = fopen(filename, "wb")) == NULL) return(0);

	//if (!configvalid)	{
	//	//fprintf(file, (char*) (GetString (errBuffer, errorStrs[lang][0]), errBuffer));
	//	//fclose(file);
	//	//return(1);
	//}

	static unsigned int	wBuffMethodsStrIds[4] = {IDS_CEWBUFFDRAWINGTEST, IDS_CEWBUFFRELYONDRIVER, IDS_CEWBUFFFORCEEMULATED, IDS_CEWBUFFFORCEW};
	resourceManager.GetString (config.language, wBuffMethodsStrIds[GetWBuffMethodValue (config.Flags)], wBufferingMethod);
	static unsigned int	lfbTexMethodsStrIds[3] = {IDS_LFBTEXAUTOMATIC, IDS_LFBTEXDISABLED, IDS_LFBTEXFORCED};
	resourceManager.GetString (config.language, lfbTexMethodsStrIds[GetLfbTexureTilingMethod (config.Flags)], lfbTexTilingMethod);
	
	RendererAPI api = (RendererAPI) config.rendererApi;
	fprintf (file, pFormatString,
		PRODUCTNAME,
		itemStrings[(platform == PWINDOWS) ? ITEM_WINDOWS : ITEM_DOS],
		adapterHandler.GetApiName (api),
		(config.dispdev < adapterHandler.GetNumOfAdapters (api)) ? adapterHandler.GetDescriptionOfAdapter (api, config.dispdev) : "?",
		(config.dispdev < adapterHandler.GetNumOfAdapters (api) && config.dispdriver < adapterHandler.GetNumOfDeviceTypes (api, config.dispdev)) ?
			adapterHandler.GetDescriptionOfDeviceType (api, config.dispdev, config.dispdriver) : "?",
		itemStrings[config.Flags & CFG_WINDOWED ? ITEM_WINDOWED : ITEM_FULLSCR],
		itemStrings[config.Flags & CFG_WINDOWED ? ITEM_DETBYDESKTOP : (config.Flags & CFG_DISPMODE32BIT ? ITEM_32BIT : ITEM_16BIT)],
		itemStrings[config.Flags & CFG_GRABENABLE ? (config.Flags & CFG_GRABCLIPBOARD ? ITEM_GRABCLIPBOARD : ITEM_GRABTOFILE) : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_NTVDDMODE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_ACTIVEBACKGROUND ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_FORCECLIPOPF ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_HIDECONSOLE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_SETMOUSEFOCUS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_SETMOUSEFOCUS ? (config.Flags & CFG_CTRLALT ? ITEM_ENABLED : ITEM_DISABLED) : ITEM_CTRALTDISABLED],
		itemStrings[config.Flags2 & CFG_PRESERVEWINDOWSIZE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_PRESERVEASPECTRATIO ? ITEM_ENABLED : ITEM_DISABLED],

		itemStrings[config.Flags & CFG_TEXTURES16BIT ? ITEM_16BIT : (config.Flags & CFG_TEXTURES32BIT ? ITEM_32BIT : ITEM_OPTIMAL)],
		itemStrings[config.Flags & CFG_WINDOWED ? ITEM_REFRATEDISABLED : (config.Flags2 & CFG_CLOSESTREFRESHFREQ ? ITEM_ENABLED : ITEM_DISABLED)],
		//((!config.Flags & CFG_WINDOWED) && (config.Flags2 & CFG_CLOSESTREFRESHFREQ)) ? "-" : itemStrings[(config.Flags & CFG_SETREFRESHRATE ? ITEM_REFRATEALIGNEDTOAPP : ITEM_REFRATEALIGNEDTOMONITOR)],
		itemStrings[config.Flags & CFG_VSYNC ? ITEM_ENABLED : ITEM_DISABLED],
		wBufferingMethod,
		itemStrings[config.Flags2 & CFG_DEPTHEQUALTOCOLOR ? ITEM_ENABLED : ITEM_DISABLED],
		lfbAccType,
		itemStrings[config.Flags & CFG_HWLFBCACHING ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_LFBFASTUNMATCHWRITE ? ITEM_ENABLED : ITEM_DISABLED],
		lfbTexTilingMethod,
		itemStrings[config.Flags & CFG_LFBREALVOODOO ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_CFORMATAFFECTLFB ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_LFBRESCALECHANGESONLY ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_LFBFORCEFASTWRITE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_LFBALWAYSFLUSHWRITES ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_LFBNOMATCHINGFORMATS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_PERFECTTEXMEM ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_DISABLEMIPMAPPING ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_FORCETRILINEARMIPMAPS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_BILINEARFILTER ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_AUTOGENERATEMIPMAPS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_STOREMIPMAP ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_MANAGEDTEXTURES ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_SCALETEXMEM ? ITEM_ENABLED : ITEM_DISABLED],
		texMemSize,
		resolution,
		refRate,
		ckMethod,
		itemStrings[config.Flags & CFG_CKMFORCETNTINAUTOMODE ? ITEM_ENABLED : ITEM_DISABLED],
		config.AlphaCKRefValue,
		config.Gamma,
		itemStrings[config.Flags2 & CFG_ALWAYSINDEXEDPRIMS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_TRIPLEBUFFER ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_TOMBRAIDER ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_CLIPPINGBYD3D ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_ENABLEGAMMARAMP ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_ENABLEHWVBUFFER ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[(GetWBuffMethodValue(config.Flags) == 2) ? ITEM_ENABLED : ITEM_DISABLED],
		config.dosTimerBoostPeriod,
		itemStrings[config.Flags2 & CFG_PREFERTMUWIFPOSSIBLE ? ITEM_ENABLED : ITEM_DISABLED],

		itemStrings[config.Flags & CFG_VESAENABLE ? ITEM_ENABLED : ITEM_DISABLED],
		config.VideoRefreshFreq,
		vesaMemSize,
		itemStrings[config.Flags & CFG_SUPPORTMODE13H ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_DISABLEVESAFLATLFB ? ITEM_ENABLED : ITEM_DISABLED],

		itemStrings[config.dx9ConfigBits & CFG_DX9LFBCOPYMODE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.dx9ConfigBits & CFG_DX9NOVERTICALRETRACE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.dx9ConfigBits & CFG_DX9FFSHADERS ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.dx9ConfigBits & CFG_HWPALETTEGEN ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.dx9ConfigBits & CFG_DX9GRAYSCALERENDERING ? ITEM_ENABLED : ITEM_DISABLED],
		
		itemStrings[config.Flags2 & CFG_YMIRROR ? ITEM_ENABLED : ITEM_DISABLED]/*, 123, 256, 512*/);
	
	fclose(file);
	return(1);
}