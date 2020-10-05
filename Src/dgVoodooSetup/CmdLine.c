/*--------------------------------------------------------------------------------- */
/* CmdLine.c - dgVoodooSetup manipulating configuration from command prompt         */
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
/* dgVoodooSetup: CmdLine.c																	*/
/*				  Parancssoros modul														*/
/*------------------------------------------------------------------------------------------*/

#include "dgVoodooConfig.h"

#include <windows.h>
#include "Resource.h"
#include "Version.h"
#include "CmdLine.h"
#include "file.h"

/*------------------------------------------------------------------------------------------*/
/*...................................... Definíciók ........................................*/

enum	{

	Or		=	1,
	AndNot	=	2,
	Xor		=	3

} BoolType;

enum	{

	None	=	0,
	Text	=	1,
	Numeric	=	2,
	Bool	=	3,		/* Bool in Flags */
	Bool2	=	4		/* Bool in Flags2 */

} ParameterType;

typedef	struct	{
	
	unsigned char		*name;
	unsigned int		parameter;
	enum ParameterType	parameterType;
	unsigned char		defined;

} OptionDescriptor;


enum	{

	saveOpt		=	0,
	helpOpt1,
	helpOpt2,
	helpOpt3,
	langOpt,
	importOpt,
	exportOpt,
	exportTextOpt,

	cfgPlatform,

	cfgWindowed,
	cfgBitDepth,
	cfgScrShot,
	cfgScrShotToClipBoard,
	cfgVDDMode,
	cfgRunInBkgnd,
	cfgCLIPOPF,
	cfgHideConsole,
	cfgMouse,
	cfgMouseCtrlAlt,
	cfgPreWinSize,
	cfgPreAspectRatio,
	
	cfgTexBitDepth,
	cfgClosestMonFreq,
	cfgSyncToAppFreq,
	cfgWaitOneRetrace,

	cfgPerfectTexEmu,
	cfgStoreTexCopies,
	cfgDisableMipMap,
	cfgForceTriMipMap,
	cfgAutoGenMipMap,
	cfgForceBilinear,

	cfgLfbAccess,
	cfgHwCacheBuffs,
	cfgFastWriteUnmatch,
	cfgFastWriteMatch,
	cfgColorFmtOnDepthFmt,
	cfgRescaleChanges,
	cfgFlushAtUnlock,
	cfgNoMatchingFmt,
	cfgNoTexTiles,
	cfgLfbRealHw,
	
	cfgCKMethod,
	cfgCKTNTInFallBack,
	cfgAlphaRef,
	
	cfgTexMemSize,
	
	cfgXRes,
	cfgYRes,
	
	cfgGamma,
	cfgMonFreq,

	cfgForceTriBuff,
	cfgFixTR1,
	cfgClipByD3D,
	cfgEnableGlideRamp,
	cfgHwVertexBuff,
	cfgWBuffDetMethod,
	cfgDepthColorEq,
	cfgForceIndexed,
	cfgPreferTmuW,
	cfgTimerBoost,
	
	cfgUseVesa,
	cfgVesaRefRate,
	cfgVesaMemSize,
	cfgSupp13,
	cfgDisableFlat,

	cfgMirrorVertically,

	NumberOfOptions

} Options;


enum	{

	Empty		=	0,
	File		=	1,
	Option		=	2,
	Erroneous	=	3

} ElementType;

/*------------------------------------------------------------------------------------------*/
/*...................................... Globálisok ........................................*/

static	LPSTR				cmdLinePtr;
static	unsigned char		lastParameterType;
static	OptionDescriptor	options[NumberOfOptions] = {
							{ "NOSAVE",				0,						None,		0 },
							{ "H",					0,						None,		0 },
							{ "?",					0,						None,		0 },
							{ "HELP",				0,						None,		0 },
							{ "LANG",				(unsigned int) NULL,	Text,		0 },
							{ "IMPORT",				(unsigned int) NULL,	Text,		0 },
							{ "EXPORT",				(unsigned int) NULL,	Text,		0 },
							{ "EXPORTTOTEXT",		(unsigned int) NULL,	Text,		0 },

							{ "PLATFORM",			(unsigned int) "WIN",	Text,		0 },

							{ "WINDOWED",			Or,						Bool,		0 },
							{ "BITDEPTH",			16,						Numeric,	0 },
							{ "SCRSHOT",			Or,						Bool,		0 },
							{ "SCRSHOTTOCB",		Or,						Bool,		0 },
							{ "VDDMODE",			Or,						Bool,		0 },
							{ "RUNINBKGND",			Or,						Bool,		0 },
							{ "CLIPOPF",			Or,						Bool2,		0 },
							{ "HIDECONSOLE",		Or,						Bool2,		0 },
							{ "MOUSE",				Or,						Bool,		0 },
							{ "MOUSECTRLALT",		Or,						Bool,		0 },
							{ "PREWINSIZE",			Or,						Bool2,		0 },
							{ "PREWINASPRAT",		Or,						Bool2,		0 },
							
							{ "TEXTUREBITDEPTH",	0,						Numeric,	0 },

							{ "CLOSESTMONFREQ",		Or,						Bool2,		0 },
							{ "SYNCTOAPPFREQ",		Or,						Bool,		0 },
							{ "WAITONERETRACE",		Or,						Bool,		0 },
							
							{ "PERFECTTEXEMU",		Or,						Bool,		0 },
							{ "STORETEXCOPIES",		Or,						Bool,		0 },
							{ "DISABLEMIPMAP",		Or,						Bool,		0 },
							{ "FORCETRIMIPMAP",		Or,						Bool2,		0 },
							{ "AUTOGENMIPMAP",		Or,						Bool2,		0 },
							{ "FORCEBILINEAR",		Or,						Bool,		0 },
							
							{ "LFBACCESS",			(unsigned int) "ENABLE",Text,		0 },
							{ "HWCACHEBUFFS",		Or,						Bool,		0 },
							{ "FASTWRITEUNMATCH",	Or,						Bool,		0 },
							{ "FASTWRITEMATCH",		Or,						Bool2,		0 },
							{ "COLORFMTONLFBFMT",	Or,						Bool2,		0 },
							{ "RESCALECHANGESONLY",	Or,						Bool2,		0 },
							{ "FLUSHATUNLOCK",		Or,						Bool2,		0 },
							{ "NOMATCHINGFMT",		Or,						Bool2,		0 },
							{ "NOTEXTURETILES",		Or,						Bool2,		0 },
							{ "LFBREALHW",			Or,						Bool,		0 },
							
							{ "CKMETHOD",			(unsigned int) "AUTO",	Text,		0 },
							{ "CKTNTINFALLBACK",	Or,						Bool,		0 },
							{ "ALPHAREF",			0,						Numeric,	0 },
							
							{ "TEXMEMSIZE",			8192,					Numeric,	0 },
							
							{ "XRES",				0,						Numeric,	0 },
							{ "YRES",				0,						Numeric,	0 },
							
							{ "GAMMA",				100,					Numeric,	0 },
							
							{ "MONFREQ",			0,						Numeric,	0 },
							
							{ "FORCETRIBUFF",		Or,						Bool,		0 },
							{ "FIXTR1",				Or,						Bool,		0 },
							{ "CLIPBYD3D",			Or,						Bool,		0 },
							{ "GLIDEGAMMA",			Or,						Bool,		0 },
							{ "HWVERTEXBUFF",		Or,						Bool,		0 },
							{ "WBUFFDETMETHOD",		(unsigned int)"DRIVERFLAG",	Text,		0 },
							{ "DEPTHCOLOREQ",		Or,						Bool2,		0 },
							{ "FORCEINDEXED",		Or,						Bool2,		0 },
							{ "PREFERTMUW",			Or,						Bool2,		0 },
							{ "TIMERBOOST",			80,						Numeric,	0 },

							{ "VESA",				Or,						Bool,		0 },
							{ "VESAREFRATE",		50,						Numeric,	0 },
							{ "VESAMEMSIZE",		1024,					Numeric,	0 },
							{ "SUPP13",				Or,						Bool,		0 },
							{ "DISABLEFLAT",		Or,						Bool2,		0 },

							{ "MIRRORVERT",			Or,						Bool2,		0 }

};

unsigned int	boolOptions[NumberOfOptions] = {
	0,							/* saveOpt */
	0,							/* helpOpt1 */
	0,							/* helpOpt2 */
	0,							/* helpOpt3 */
	0,							/* langOpt */
	0,							/* importOpt */
	0,							/* exportOpt */
	0,							/* exportTextOpt */

	0,							/* cfgPlatform */

	CFG_WINDOWED,				/* cfgWindowed */
	0,							/* cfgBitDepth */
	CFG_GRABENABLE,				/* cfgScrShot */
	CFG_GRABCLIPBOARD,			/* cfgScrShotToClipBoard */
	CFG_NTVDDMODE,				/* cfgVDDMode */
	CFG_ACTIVEBACKGROUND,		/* cfgRunInBkgnd */
	CFG_FORCECLIPOPF,			/* cfgCLIPOPF */
	CFG_HIDECONSOLE,			/* cfgHideConsole */
	CFG_SETMOUSEFOCUS,			/* cfgMouse */
	CFG_CTRLALT,				/* cfgMouseCtrlAlt */
	CFG_PRESERVEWINDOWSIZE,		/* cfgPreWinSize */
	CFG_PRESERVEASPECTRATIO,	/* cfgPreAspectRatio */
	
	0,							/* cfgTexBitDepth */
	CFG_CLOSESTREFRESHFREQ,		/* cfgClosestMonFreq */
	CFG_SETREFRESHRATE,			/* cfgSyncToAppFreq */
	CFG_VSYNC,					/* cfgWaitOneRetrace */

	CFG_PERFECTTEXMEM,			/* cfgPerfectTexEmu */
	CFG_STOREMIPMAP,			/* cfgStoreTexCopies */
	CFG_DISABLEMIPMAPPING,		/* cfgDisableMipMap */
	CFG_FORCETRILINEARMIPMAPS,	/* cfgForceTriMipMap */
	CFG_AUTOGENERATEMIPMAPS,	/* cfgAutoGenMipMap */
	CFG_BILINEARFILTER,			/* cfgForceBilinear */

	0,							/* cfgLfbAccess */
	CFG_HWLFBCACHING,			/* cfgHwCacheBuffs */
	CFG_LFBFASTUNMATCHWRITE,	/* cfgFastWriteUnmatch */
	CFG_LFBFORCEFASTWRITE,		/* cfgFastWriteMatch */
	CFG_CFORMATAFFECTLFB,		/* cfgColorFmtOnDepthFmt */
	CFG_LFBRESCALECHANGESONLY,	/* cfgRescaleChanges */
	CFG_LFBALWAYSFLUSHWRITES,	/* cfgFlushAtUnlock */
	CFG_LFBNOMATCHINGFORMATS,	/* cfgNoMatchingFmt */
	CFG_LFBDISABLETEXTURING,	/* cfgNoTexTiles */
	CFG_LFBREALVOODOO,			/* cfgLfbRealHw */
	
	0,							/* cfgCKMethod */
	CFG_CKMFORCETNTINAUTOMODE,	/* cfgCKTNTInFallBack */
	0,							/* cfgAlphaRef */
	
	0,							/* cfgTexMemSize */
	
	0,							/* cfgXRes */
	0,							/* cfgYRes */
	
	0,							/* cfgGamma */
	0,							/* cfgMonFreq */

	CFG_TRIPLEBUFFER,			/* cfgForceTriBuff */
	CFG_TOMBRAIDER,				/* cfgFixTR1 */
	CFG_CLIPPINGBYD3D,			/* cfgClipByD3D */
	CFG_ENABLEGAMMARAMP,		/* cfgEnableGlideRamp */
	CFG_ENABLEHWVBUFFER,		/* cfgHwVertexBuff */
	0,							/* cfgWBuffDetMethod */
	CFG_DEPTHEQUALTOCOLOR,		/* cfgDepthColorEq */
	CFG_ALWAYSINDEXEDPRIMS,		/* cfgForceIndexed */
	CFG_PREFERTMUWIFPOSSIBLE,	/* cfgPreferTmuW */
	0,							/* cfgTimerBoost */
	
	CFG_VESAENABLE,				/* cfgUseVesa */
	0,							/* cfgVesaRefRate */
	0,							/* cfgVesaMemSize */
	CFG_SUPPORTMODE13H,			/* cfgSupp13 */
	CFG_DISABLEVESAFLATLFB,		/* cfgDisableFlat */

	CFG_YMIRROR					/* cfgMirrorVertically */
};


typedef struct	{

	unsigned char *name;
	unsigned char *remark;

} _InstanceInfo;

extern	dgVoodooConfig	config;
extern	dgVoodooConfig	configs[2];
extern	unsigned int	platform;
extern  unsigned int	lang;
extern	int				cmdLineMode;
extern  int				configvalid;
extern  _InstanceInfo	instanceInfo[];

extern	int ReaddgVoodooConfig(unsigned char *filename, unsigned int platform);
extern	int WritedgVoodooConfig(unsigned char *filename, unsigned int platform);
extern	int	LoadConfiguration(char *filename);
extern	int SaveConfiguration(char *filename);
extern	int ExportConfiguration(char *filename);

static	LOGFONT	helpFont =	{-11, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 49, "Courier New"};

/*------------------------------------------------------------------------------------------*/
/*...................................... Függvények ........................................*/

static int CALLBACK	cmdHelpDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)	{
HRSRC				hRsrcFormatStr;
unsigned char*		pFormatString;
static HFONT		hFont;
HDC					editHdc;

	switch(uMsg)	{
		case WM_INITDIALOG:
			hRsrcFormatStr = FindResource (NULL, (LPCSTR) IDR_CMDLINEHELP, (LPCSTR) 200);
			pFormatString = LockResource (LoadResource (NULL, hRsrcFormatStr));
			
			hFont = CreateFontIndirect (&helpFont);

			SendMessage(GetDlgItem(hwndDlg, IDC_HELPEDIT), WM_SETTEXT, 0, (LPARAM) pFormatString);
			SendMessage(GetDlgItem(hwndDlg, IDC_HELPEDIT), WM_SETFONT, (WPARAM) hFont, MAKELPARAM(TRUE, 0));
			return(TRUE);

		case WM_DESTROY:
			DeleteObject ((HGDIOBJ) hFont);
			return(0);

		case WM_COMMAND:
			if (HIWORD(wParam) != BN_CLICKED) return(0);
			if (LOWORD(wParam) == IDC_HELPOKBUTTON) {
				EndDialog(hwndDlg, 1);
				return (1);
			}

		case  WM_CTLCOLORSTATIC:
			editHdc = (HDC) wParam;
			return (int) GetStockObject (WHITE_BRUSH);
		
		default:
			return(0);
	}
}


int				EmptyCommandLine (LPSTR cmdLine)
{
	while ((*cmdLine) != 0x0)	{
		if ((*cmdLine) != ' ')
			return(0);
	}
	return(1);
}


static	int		DoesFileExists (unsigned char *fileName)
{
HANDLE			hFile;
	
	if ( (hFile = _OpenFile(fileName)) == INVALID_HANDLE_VALUE) return(0);
	
	_CloseFile (hFile);
	return (1);
}


static	int		FindInstance (unsigned char *file, unsigned int fileParameter)
{
unsigned int	i;
unsigned int	dirLen;

	if (fileParameter == 0) {
		fileParameter = 2;
		i = 0;
	} else {
		i = fileParameter - 1;
	}

	dirLen = strlen (file);
	
	while (fileParameter > i)	{
		file[dirLen] = 0x0;
		strcat (file, instanceInfo[fileParameter - 1].name);
		if (DoesFileExists (file))
			return (1);
		fileParameter--;
	}
	return (0);
}


static	int		GetNextCmdLineParameter (unsigned char *option, unsigned char *optionParameter)
{
unsigned char	*target;
unsigned char	hasParameter;

	lastParameterType = Empty;
	option[0] = 0x0;
	optionParameter[0] = 0x0;

	while (cmdLinePtr[0] == ' ') cmdLinePtr++;
	if (cmdLinePtr[0] == 0x0) return (0);

	if ( (cmdLinePtr[0] == '-') || (cmdLinePtr[0] == '/') ) {
		lastParameterType = Option;
		cmdLinePtr++;
	} else {
		lastParameterType = File;
	}

	if ( (cmdLinePtr[0] == ' ') || (cmdLinePtr[0] == '-') || (cmdLinePtr[0] == '/') || (cmdLinePtr[0] == ':'))	{
		lastParameterType = Erroneous;
		return (1);
	}

	target = option;
	hasParameter = 0;

	while (1)	{
		*(target++) = *(cmdLinePtr++);
		if (cmdLinePtr[0] == ':')	{
			hasParameter = 1;
			cmdLinePtr++;
			target[0] = 0x0;
			break;
		}
		if ( (cmdLinePtr[0] == ' ') || (cmdLinePtr[0] == '-') || (cmdLinePtr[0] == '/') || (cmdLinePtr[0] == 0x0) )	{
			target[0] = 0x0;
			break;
		}
	}

	/*if ((lastParameterType != Option) && (hasParameter))	{
		lastParameterType = Erroneous;
		return (0);
	}*/

	if (hasParameter)	{
		target = optionParameter;
		
		while (cmdLinePtr[0] == ' ') cmdLinePtr++;
		while (1)	{
			*(target++) = *(cmdLinePtr++);
			if (cmdLinePtr[0] == ':')	{
				lastParameterType = Erroneous;
				return (1);
			}
			if ( (cmdLinePtr[0] == ' ') || (cmdLinePtr[0] == '-') || (cmdLinePtr[0] == '/') || (cmdLinePtr[0] == 0x0))	{
				target[0] = 0x0;
				break;
			}
		}
	}
	return (1);
}


static int	GetNumericValue (unsigned char *str, unsigned int *num)
{
unsigned int	digit;
	
	*num = 0;

	if ((str[0] == 'x') || ((str[0] == '0') && (str[1] == 'x')))	{
		

		str += str[1] == 'x' ? 2 : 1;
		if (str[0] == 0x0) return (0);
		
		while(str[0] != 0x0)	{
			if ((str[0] >= '0') && (str[0] <= '9'))	{
				digit = (unsigned int) (str[0] - '0');
			} else if ((str[0] >= 'a') && (str[0] <= 'f'))	{
				digit = (unsigned int) (str[0] - 'a' + 10);
			} else if ((str[0] >= 'A') && (str[0] <= 'F'))	{
				digit = (unsigned int) (str[0] - 'A' + 10);
			} else
				return (0);
			
			*num = (*num << 4) + digit;
			str++;
		}

	} else {

		while(str[0] != 0x0)	{
			if ((str[0] >= '0') && (str[0] <= '9'))	{
				*num = (*num * 10) + (unsigned int) (str[0] - '0');
			} else
				return (0);
			str++;
		}
	}
	return (1);
}


static int		ModifyConfig ()
{
unsigned int		i;
OptionDescriptor	*optDesc;
	
	optDesc = options;

	for (i = 0; i < NumberOfOptions; i++)	{
		if (optDesc->defined)	{
			if (optDesc->parameterType == Bool)	{
				switch (optDesc->parameter)	{
					case Or:
						config.Flags |= boolOptions[i];
						break;
					case AndNot:
						config.Flags &= ~boolOptions[i];
						break;
					case Xor:
						config.Flags ^= boolOptions[i];
						break;
				}
			} else if (optDesc->parameterType == Bool2)	{
				switch (optDesc->parameter)	{
					case Or:
						config.Flags2 |= boolOptions[i];
						break;
					case AndNot:
						config.Flags2 &= ~boolOptions[i];
						break;
					case Xor:
						config.Flags2 ^= boolOptions[i];
						break;
				}
			} else {
				switch (i)	{
					case langOpt:
						lang = config.language = English;
						strupr ((char *) optDesc->parameter);
						if (strcmp ((char *) optDesc->parameter, "ENG") == 0)	{
							lang = config.language = English;
						} else if (strcmp ((char *) optDesc->parameter, "HUN") == 0)	{
							lang = config.language = Hungarian;
						} else {
							//Error
							return(0);
						}
						break;
					case cfgBitDepth:
						if (optDesc->parameter == 16) config.Flags &= ~CFG_DISPMODE32BIT;
						else if (optDesc->parameter == 32) config.Flags |= CFG_DISPMODE32BIT;
						else {
							//Error
							return(0);
						}
						break;

					case cfgTexBitDepth:
						config.Flags &= ~(CFG_TEXTURES16BIT | CFG_TEXTURES32BIT);
						switch (optDesc->parameter)	{
							case 0:
								break;
							case 16:
								config.Flags |= CFG_TEXTURES16BIT;
								break;
							case 32:
								config.Flags |= CFG_TEXTURES32BIT;
								break;
							default:
								//Error
								return(0);
						}
						break;

					case cfgLfbAccess:
						strupr ((char *) optDesc->parameter);
						if (strcmp ((char *) optDesc->parameter, "DISABLE") == 0)	{
							config.Flags |= CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE;
						} else if (strcmp ((char *) optDesc->parameter, "DISABLEREAD") == 0)	{
							config.Flags |= CFG_DISABLELFBREAD;
							config.Flags &= ~CFG_DISABLELFBWRITE;
						} else if (strcmp ((char *) optDesc->parameter, "DISABLEWRITE") == 0)	{
							config.Flags |= CFG_DISABLELFBWRITE;
							config.Flags &= ~CFG_DISABLELFBREAD;
						} else if (strcmp ((char *) optDesc->parameter, "ENABLE") == 0)	{
							config.Flags &= ~(CFG_DISABLELFBWRITE | CFG_DISABLELFBREAD);
						} else {
							//Error
							return(0);
						}
						break;

					case cfgCKMethod:
						strupr ((char *) optDesc->parameter);
						if (strcmp ((char *) optDesc->parameter, "AUTO") == 0)	{
							SetCKMethodValue (config.Flags, CFG_CKMAUTOMATIC);
						} else if (strcmp ((char *) optDesc->parameter, "ALPHA") == 0)	{
							SetCKMethodValue (config.Flags, CFG_CKMALPHABASED);
						} else if (strcmp ((char *) optDesc->parameter, "NATIVE") == 0)	{
							SetCKMethodValue (config.Flags, CFG_CKMNATIVE);
						} else if (strcmp ((char *) optDesc->parameter, "NATIVETNT") == 0)	{
							SetCKMethodValue (config.Flags, CFG_CKMNATIVETNT);
						} else {
							//Error
							return(0);
						}
						break;


					case cfgAlphaRef:
						config.AlphaCKRefValue = optDesc->parameter;
						break;

					case cfgTexMemSize:
						config.TexMemSize = optDesc->parameter * 1024;
						break;

					case cfgXRes:
						config.dxres = optDesc->parameter;
						break;

					case cfgYRes:
						config.dyres = optDesc->parameter;
						break;
	
					case cfgGamma:
						config.Gamma = optDesc->parameter;
						break;

					case cfgMonFreq:
						config.RefreshRate = optDesc->parameter;
						break;

					case cfgWBuffDetMethod:
						strupr ((char *) optDesc->parameter);
						if (strcmp ((char *) optDesc->parameter, "FORCEZ") == 0)	{
							SetWBuffMethodValue(config.Flags, CFG_WDETFORCEZ);
						} else if (strcmp ((char *) optDesc->parameter, "FORCEW") == 0)	{
							SetWBuffMethodValue(config.Flags, CFG_WDETFORCEW);
						} else if (strcmp ((char *) optDesc->parameter, "DRIVERFLAG") == 0)	{
							SetWBuffMethodValue(config.Flags, CFG_WDETONDRIVERFLAG);
						} else if (strcmp ((char *) optDesc->parameter, "DRAWINGTEST") == 0)	{
							SetWBuffMethodValue(config.Flags, CFG_WDETDRAWINGTEST);
						} else {
							//Error
							return(0);
						}
						break;
					
					case cfgTimerBoost:
						config.dosTimerBoostPeriod = optDesc->parameter;
						break;
	
					case cfgVesaRefRate:
						config.VideoRefreshFreq = optDesc->parameter;
						break;
					
					case cfgVesaMemSize:
						config.VideoMemSize = optDesc->parameter * 1024;
						break;

					default:
						break;
				}
			}
		}
		optDesc++;
	}
	return(1);
}


void	ProcessCommandLine (LPSTR cmdLine)
{
unsigned char		option[1024];
unsigned char		optionParameter[1024];
unsigned char		file[1024];
unsigned char		textParameters[2048];
unsigned char		*textParamPtr;
OptionDescriptor	*optDesc;
unsigned int		i;
unsigned int		num;
unsigned int		fileParameter;

	cmdLineMode = 1;
	platform = 0;
	configvalid = 1;

	cmdLinePtr = cmdLine;
	textParamPtr = textParameters;
	file[0] = 0x0;

	while (GetNextCmdLineParameter (option, optionParameter))	{
		
		switch (lastParameterType)	{
			case File:
				if (option[0] == '$')	{
					i = 0;
					do {
						file[i] = option[i];
						i++;
					} while ((option[i - 1] != 0x0) && ((i == 1) || (option[i - 1] != '$')));

					file[i] = 0x0;
					
					strupr (file);
					
					fileParameter = 0;
					if (strlen (optionParameter) != 0) {
						if (!GetNumericValue (optionParameter, &fileParameter))
							return;
						
						if (fileParameter > 2)
							return;
					}

					/* $Win$ helyettesítése */
					if (strcmp (file, "$WIN$") == 0)	{
						
						if ((option[i] != 0x0) && (strlen (optionParameter) != 0))
							return;
						
						if ( file[GetWindowsDirectory (file, sizeof(file)) - 1] != '\\' ) {
							strcat (file, "\\");
						}
						
						if (option[i] != 0x0)	{
							strcat (file, option + i);
						} else {
							FindInstance (file, fileParameter);
						}

					} else if (strcmp (file, "$FIRSTFOUND$") == 0)	{
					/* $FirstFound$ helyettesítése */
						if (option[i] != 0x0)
							return;
						
						if ( file[GetCurrentDirectory (sizeof(file), file) - 1] != '\\' ) {
							strcat (file, "\\");
						}
						if (!FindInstance (file, fileParameter)) {

							if ( file[GetWindowsDirectory (file, sizeof(file)) - 1] != '\\' ) {
								strcat (file, "\\");
							}
							FindInstance (file, fileParameter);
						}
					}
				} else {
					if (strlen (optionParameter) != 0)
						return;

					strcpy (file, option);
				}
				break;
			
			case Option:
				optDesc = options;
				strupr (option);
				for (i = 0; i < NumberOfOptions; i++)	{
					if (strcmp (option, optDesc->name) == 0)	{
						if (strlen (optionParameter) != 0)	{
							switch (optDesc->parameterType)	{
								
								case Text:
									optDesc->parameter = (unsigned int) textParamPtr;
									strcpy (textParamPtr, optionParameter);
									textParamPtr += strlen (textParamPtr) + 1;
									break;

								case Numeric:
									if (GetNumericValue (optionParameter, &num))	{
										optDesc->parameter = num;
									} else {
										return;
									}
									break;

								case Bool:
								case Bool2:
									strupr (optionParameter);
									if (strcmp (optionParameter, "ON") == 0) optDesc->parameter = Or;
									else if (strcmp (optionParameter, "OFF") == 0) optDesc->parameter = AndNot;
									else if (strcmp (optionParameter, "INV") == 0) optDesc->parameter = Xor;
									else return;
									break;

								case None:
								default:
									return;
							}
						}
						optDesc->defined = 1;
						break;
					}
					optDesc++;
				}
				break;

			case Erroneous:
			default:
				return;
			
		}

	}

	if ((options[helpOpt1].defined) || (options[helpOpt2].defined) || (options[helpOpt3].defined))	{
	
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CMDLINEHELP), NULL, cmdHelpDialogProc, 0);
	}

	platform = 0;
	if (options[cfgPlatform].defined)	{	
		strupr ((char *) options[cfgPlatform].parameter);
		if (strcmp ((char *) options[cfgPlatform].parameter, "WIN") == 0)	{
			platform = 0;
		} else if (strcmp ((char *) options[cfgPlatform].parameter, "DOS") == 0)	{
			platform = 1;
		}
	}

	config = configs[platform];

	if (options[importOpt].defined)	{
		LoadConfiguration((unsigned char *) options[importOpt].parameter);
	} else {
		if (ReaddgVoodooConfig (file, platform)) file[0] = 0x0;
	}
	
	if (!ModifyConfig ()) return;

	if (!options[saveOpt].defined && (strlen(file) != 0))	{
		WritedgVoodooConfig (file, platform);
	}

	if (options[exportOpt].defined)	{
		SaveConfiguration((unsigned char *) options[exportOpt].parameter);
	}

	if (options[exportTextOpt].defined)	{
		ExportConfiguration((unsigned char *) options[exportTextOpt].parameter);
	}
}