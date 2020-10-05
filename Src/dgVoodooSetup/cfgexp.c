/*--------------------------------------------------------------------------------- */
/* CfgExp.c - dgVoodooSetup text export of configuration                            */
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
#define ITEM_REFRATEALIGNEDTOMONITOR 12
#define ITEM_REFRATEALIGNEDTOAPP    13

unsigned int	invalidConfigStrs[numberOfLanguages][1] =
				{	{IDS_CEINVALIDCFGERROR},
					{IDS_CEINVALIDCFGERROR_HUN} };

unsigned int	itemDosTable[numberOfLanguages][1] =
				{	{IDS_CEDOS},
					{IDS_CEDOS_HUN} };

unsigned int	itemWindowsTable[numberOfLanguages][1] =
				{	{IDS_CEWINDOWS},
					{IDS_CEWINDOWS_HUN} };

unsigned int	itemEnabledTable[numberOfLanguages][1] =
				{	{IDS_CEENABLED},
					{IDS_CEENABLED_HUN} };

unsigned int	itemDisabledTable[numberOfLanguages][1] =
				{	{IDS_CEDISABLED},
					{IDS_CEDISABLED_HUN} };

unsigned int	itemFullScrTable[numberOfLanguages][1] =
				{	{IDS_CEFULLSCR},
					{IDS_CEFULLSCR_HUN} };

unsigned int	itemWindowedTable[numberOfLanguages][1] =
				{	{IDS_CEWINDOWED},
					{IDS_CEWINDOWED_HUN} };

unsigned int	item16BitTable[numberOfLanguages][1] =
				{	{IDS_CE16BIT},
					{IDS_CE16BIT_HUN} };

unsigned int	item32BitTable[numberOfLanguages][1] =
				{	{IDS_CE32BIT},
					{IDS_CE32BIT_HUN} };

unsigned int	itemOptimalTable[numberOfLanguages][1] =
				{	{IDS_CEOPTIMAL},
					{IDS_CEOPTIMAL_HUN} };

unsigned int	itemDetByDesktopTable[numberOfLanguages][1] =
				{	{IDS_CEDETBYDESKTOP},
					{IDS_CEDETBYDESKTOP_HUN} };

unsigned int	itemShotToClipBoardTable[numberOfLanguages][1] =
				{	{IDS_CESHOTTOCLIPBOARD},
					{IDS_CESHOTTOCLIPBOARD_HUN} };

unsigned int	itemShotToFileTable[numberOfLanguages][1] =
				{	{IDS_CESHOTTOFILE},
					{IDS_CESHOTTOFILE_HUN} };

unsigned int	itemWBuffMethods[numberOfLanguages][4] =
				{	{IDS_CEWBUFFDRAWINGTEST, IDS_CEWBUFFRELYONDRIVER, IDS_CEWBUFFFORCEEMULATED, IDS_CEWBUFFFORCEW},
					{IDS_CEWBUFFDRAWINGTEST_HUN, IDS_CEWBUFFRELYONDRIVER_HUN, IDS_CEWBUFFFORCEEMULATED_HUN, IDS_CEWBUFFFORCEW_HUN} };

unsigned int	itemRefRateMethods[numberOfLanguages][2] =
				{	{IDS_CEREFRATEMONITOR, IDS_REFRATEAPP},
					{IDS_CEREFRATEMONITOR_HUN, IDS_REFRATEAPP_HUN} };

unsigned int	formatStrIds[numberOfLanguages][1] =
				{	{IDR_FORMATSTRENG},
					{IDR_FORMATSTRHUN} };

unsigned int	enumCount;
char			enumStr[128];


BOOL WINAPI DDExportDisplayDeviceEnumCallback(GUID FAR *lpGUID, LPSTR  lpDriverDescription, LPSTR  lpDriverName, LPVOID lpContext	)
{
	if (enumCount == config.dispdev)	{
		strcpy (enumStr, lpDriverDescription);
	}
	enumCount++;
	return (1);
}


HRESULT CALLBACK ExportDisplayDriverEnumCallback(LPSTR DevDesc, LPSTR DevName, LPD3DDEVICEDESC7 lpD3DDeviceDesc, LPVOID lpContext)	{

	if (++enumCount == config.dispdriver)	{
		strcpy ((unsigned char *) lpContext, DevName);
		return(D3DENUMRET_CANCEL);
	}
	return(D3DENUMRET_OK);
}

int ExportConfiguration(char *filename)	{
FILE			*file;
char			dispDevice[128];
char			dispDriver[128];
char			texMemSize[32];
char			resolution[32];
char			lfbAccType[128];
char			wBufferingMethod[128];
char			ckMethod[128];
char			refRate[32];
char			vesaMemSize[32];

HRSRC			hRsrcFormatStr;
unsigned char*	pFormatString;
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
char			itemRefRateMethodByMonitor[64];
char			itemRefRateMethodByApp[64];
unsigned char*  itemStrings[16];
unsigned char	errBuffer[256];
MOVIEDATA		movie;
LPDIRECT3D7		lpD3D;	
LPDIRECTDRAW7	lpDD;
	

	GetString (itemDos, itemDosTable[config.language][0]);
	GetString (itemWindows, itemWindowsTable[config.language][0]);
	GetString (itemEnabled, itemEnabledTable[config.language][0]);
	GetString (itemDisabled, itemDisabledTable[config.language][0]);
	GetString (itemFullScr, itemFullScrTable[config.language][0]);
	GetString (itemWindowed, itemWindowedTable[config.language][0]);
	GetString (item16Bit, item16BitTable[config.language][0]);
	GetString (item32Bit, item32BitTable[config.language][0]);
	GetString (itemOptimal, itemOptimalTable[config.language][0]);
	GetString (itemDetByDestop, itemDetByDesktopTable[config.language][0]);
	GetString (itemShotToClipBoard, itemShotToClipBoardTable[config.language][0]);
	GetString (itemShotToFile, itemShotToFileTable[config.language][0]);
	GetString (itemRefRateMethodByMonitor, itemRefRateMethods[config.language][0]);
	GetString (itemRefRateMethodByApp, itemRefRateMethods[config.language][1]);


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
	itemStrings[ITEM_REFRATEALIGNEDTOMONITOR] = itemRefRateMethodByMonitor;
	itemStrings[ITEM_REFRATEALIGNEDTOAPP] = itemRefRateMethodByApp;

	if (!cmdLineMode) {
		SendMessage(GetDlgItem(hDlgGlobal, IDC_CBDISPDEVS), CB_GETLBTEXT, (LPARAM) config.dispdev, (WPARAM) dispDevice);
		SendMessage(GetDlgItem(hDlgGlobal,IDC_CBDISPDRIVERS), CB_GETLBTEXT, (LPARAM) config.dispdev, (WPARAM) dispDriver);
	} else {
		enumCount = 0;
		if (EnumDisplayDevs(DDExportDisplayDeviceEnumCallback))	{
			strcpy (dispDevice, enumStr);
		} else {
			dispDevice[0] = 0x0;
		}

		GetString (dispDriver, driverStrs[lang][0]);

		if (config.dispdriver != 0)	{
			movie.cmiFlags = MV_3D;
			InitDraw(&movie, NULL);
			lpDD = movie.cmiDirectDraw;
			lpD3D = movie.cmiDirect3D;
			if ( (lpDD != NULL) && (lpD3D != NULL) ) {
				enumCount = 0;
				lpD3D->lpVtbl->EnumDevices(lpD3D, ExportDisplayDriverEnumCallback, dispDriver);
			}
		} else {
			GetString (dispDriver, driverStrs[lang][0]);
		}
	}

	sprintf(texMemSize, "%d kB", config.TexMemSize >> 10);

	GetString (resolution, resolutionStrs[lang][0]);
	if ((config.dxres != 0) || (config.dyres != 0))
		sprintf(resolution, "%dx%d", config.dxres, config.dyres);

	GetString (refRate, refreshFreqStrs[lang][0]);
	if (config.RefreshRate != 0)
		sprintf(refRate, "%d Hz",config.RefreshRate);

	GetString (lfbAccType, lfbAccessStrs[lang][GetLfbAccessValue (config.Flags)]);
	GetString (ckMethod, ckMethodStrs[lang][GetWBuffMethodValue (config.Flags)]);

	sprintf(vesaMemSize, "%d kB", config.VideoMemSize);

	hRsrcFormatStr = FindResource (NULL, (LPCSTR) (formatStrIds[config.language][0]), (LPCSTR) 100);
	pFormatString = LockResource (LoadResource (NULL, hRsrcFormatStr));
	
	if ( (file = fopen(filename, "wb")) == NULL) return(0);

	if (!configvalid)	{
		fprintf(file, (unsigned char*) (GetString (errBuffer, errorStrs[lang][0]), errBuffer));
		fclose(file);
		return(1);
	}

	GetString (wBufferingMethod, itemWBuffMethods[lang][GetWBuffMethodValue(config.Flags)]);
	fprintf (file, pFormatString,
		PRODUCTNAME,
		itemStrings[(platform == PWINDOWS) ? ITEM_WINDOWS : ITEM_DOS],
		dispDevice,
		dispDriver,
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
		((!config.Flags & CFG_WINDOWED) && (config.Flags2 & CFG_CLOSESTREFRESHFREQ)) ? "-" : itemStrings[(config.Flags & CFG_SETREFRESHRATE ? ITEM_REFRATEALIGNEDTOAPP : ITEM_REFRATEALIGNEDTOMONITOR)],
		itemStrings[config.Flags & CFG_VSYNC ? ITEM_ENABLED : ITEM_DISABLED],
		wBufferingMethod,
		itemStrings[config.Flags2 & CFG_DEPTHEQUALTOCOLOR ? ITEM_ENABLED : ITEM_DISABLED],
		lfbAccType,
		itemStrings[config.Flags & CFG_HWLFBCACHING ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags & CFG_LFBFASTUNMATCHWRITE ? ITEM_ENABLED : ITEM_DISABLED],
		itemStrings[config.Flags2 & CFG_LFBDISABLETEXTURING ? ITEM_ENABLED : ITEM_DISABLED],
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
		
		itemStrings[config.Flags2 & CFG_YMIRROR ? ITEM_ENABLED : ITEM_DISABLED]/*, 123, 256, 512*/);
	
	fclose(file);
	return(1);
}