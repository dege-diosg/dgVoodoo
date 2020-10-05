/*--------------------------------------------------------------------------------- */
/* dgVoodooSetupClassicView.cpp - dgVoodooSetup UI pages for classic config view    */
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
/* dgVoodoo: dgVoodooSetupClassicView.cpp													*/
/*			 Main Setup classic config view													*/
/*------------------------------------------------------------------------------------------*/

#include "dgVoodooSetup.h"
#include "dgVoodooSetupDX7.h"
#include "dgVoodooSetupDX9.h"

#include "Resource.h"

#include "Commctrl.h"

#include <stdio.h>


#define MAXDEPENDINGITEMS		16

#define	DTYPE_FLAGMASKTRUE		0
#define	DTYPE_FLAG2MASKTRUE		1 
#define	DTYPE_FLAGMASKFALSE		2
#define	DTYPE_FLAG2MASKFALSE	3
#define	DTYPE_FUNCTION			4
#define DTYPE_FUNCTIONFALSE		5
#define DTYPE_PLATFORM			6
#define DTYPE_DX9FLAGMASKTRUE	7
#define DTYPE_DX9FLAGMASKFALSE	8


namespace	CV	{

// --------------------------------- Classic Config View -------------------------------------

class	ClassicConfigView:	public ConfigView
{
friend class ClassicGlobalPage;
friend class ClassicGlidePage;
friend class ClassicVESAPage;
friend class ClassicRendererPage;

private:
	enum	ConfigPage	{
		Global,
		Glide,
		VESA,
		Renderer,

		None
	};

private:
	class	ClassicGlobalPage: public UI::Page
	{
		private:
			ClassicConfigView*	parent;
		private:
			void	EnumerateAdapters (HWND pageWnd);
			void	EnumerateRenderingDevices (HWND pageWnd, unsigned short adapter);
			void	SetRendererApiComboBox (HWND pageWnd);
			void	SetAdapterComboBox (HWND pageWnd);
			void	SetDeviceComboBox (HWND pageWnd);
			void	SetScreenModeBox (HWND pageWnd);
			void	SetScreenBitDepthBox (HWND pageWnd);
			void	SetHideConsoleBox (HWND pageWnd);
			void	SetSetMouseFocusBox (HWND pageWnd);
			void	SetCTRLALTBox (HWND pageWnd);
			void	SetVDDModeBox (HWND pageWnd);
			void	SetActiveBackgroundBox (HWND pageWnd);
			void	SetPreserveWindowSizeBox (HWND pageWnd);
			void	SetPreserveAspectRatioBox (HWND pageWnd);
			void	SetCLIPOPFBox (HWND pageWnd);
			void	SetCaptureOnBox (HWND pageWnd);
			void	SetGrabModeBox (HWND pageWnd);

		protected:
			static INT_PTR	CALLBACK GlobalPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

			virtual bool	Init (HWND pageWnd);
			virtual bool	ButtonClicked (HWND pageWnd, WORD id);
			virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);

			virtual void	SetPageToConfig (HWND pageWnd);

		public:
			ClassicGlobalPage (ClassicConfigView* parent, unsigned int langIndex);

			virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
			void			ConfigChanged ();
	};


	class	ClassicGlidePage: public UI::Page
	{
		private:
			ClassicConfigView*	parent;
		private:
			static	unsigned int	texMemSizes[];
			static	char*			texMemSizeStr[];
			static	WORD			lfbAccessStrIDs[];
			static	WORD			ckMethodStrsIDs[];

			void	EnumerateResolutions (HWND pageWnd, unsigned short adapter);
			void	EnumerateRefreshRates (HWND pageWnd, unsigned short adapter);

			void	SetTexBitDepthBox (HWND pageWnd);
			void	SetClosestRefRateBox (HWND pageWnd);
			void	SetVSyncBox (HWND pageWnd);

			void	SetLfbAccessComboBox (HWND pageWnd);
			void	SetHwBuffCachingBox (HWND pageWnd);
			void	SetFastWriteBox (HWND pageWnd);
			void	SetTextureTilesBox (HWND pageWnd);
			void	SetRealHwBox (HWND pageWnd);
			
			void	SetCKComboBox (HWND pageWnd);
			void	SetTexMemSizeComboBox (HWND pageWnd);
			
			void	SetGammaControl (HWND pageWnd);
			void	SetResolutionComboBox (HWND pageWnd);
			void	SetRefreshFreqComboBox (HWND pageWnd);

			void	SetMipMappingDisableBox (HWND pageWnd);
			void	SetTrilinearMipMapsBox (HWND pageWnd);
			void	SetAutogenerateMipMapsBox (HWND pageWnd);
			void	SetBilinearFilteringBox (HWND pageWnd);
			void	SetTexMemScaleBox (HWND pageWnd);
			
			void	SetTripleBufferBox (HWND pageWnd);
			void	SetTombRaiderBox (HWND pageWnd);
			void	SetHwVBuffBox (HWND pageWnd);
			void	SetGammaRampBox (HWND pageWnd);
			void	SetTimerBoostBox (HWND pageWnd);
			void	SetForceWBufferingBox (HWND pageWnd);

		protected:
			static INT_PTR	CALLBACK GlidePageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

			virtual bool	Init (HWND pageWnd);
			virtual bool	ButtonClicked (HWND pageWnd, WORD id);
			virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);
			virtual bool	SliderChanged (HWND pageWnd, WORD id);

			virtual void	SetPageToConfig (HWND pageWnd);

		public:
			ClassicGlidePage (ClassicConfigView* parent, unsigned int langIndex);

			virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
			void			ConfigChanged ();

			void			ApiChanged ();
			void			AdapterChanged ();
	};


	class	ClassicVESAPage: public UI::Page
	{
		private:
			ClassicConfigView*		parent;
			static	char*			vidMemSizeStr[];
			static	unsigned int	vidMemSizes[];

			void	SetVESAEnableBox (HWND pageWnd);
			void	SetRefreshFreq (HWND pageWnd);
			void	SetEmulateVidMemSizeComboBox (HWND pageWnd);
			void	SetSupportMode13hBox (HWND pageWnd);
			void	SetSupportDisableFlatLFBBox (HWND pageWnd);
		
		protected:
			static INT_PTR	CALLBACK VESAPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

			virtual bool	Init (HWND pageWnd);
			virtual bool	ButtonClicked (HWND pageWnd, WORD id);
			virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);
			virtual bool	SliderChanged (HWND pageWnd, WORD id);

			virtual void	SetPageToConfig (HWND pageWnd);

		public:
			ClassicVESAPage (ClassicConfigView* parent, unsigned int langIndex);

			virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
			void			ConfigChanged ();
	};


	class	ClassicRendererPage: public UI::Page
	{
		private:
			ClassicConfigView*	parent;

			void			SetDX9LfbCopyModeBox (HWND pageWnd);
			void			SetDX9VerticalRetraceBox (HWND pageWnd);
			void			SetDX9FFShadersBox (HWND pageWnd);
			void			SetDX9HwPalGenBox (HWND pageWnd);
			void			SetDX9BlackWhiteRenderingBox (HWND pageWnd);

		protected:
			static INT_PTR	CALLBACK RendererPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

			virtual bool	Init (HWND pageWnd);
			virtual bool	ButtonClicked (HWND pageWnd, WORD id);
			virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);

			virtual void	SetPageToConfig (HWND pageWnd);

		public:
			ClassicRendererPage (ClassicConfigView* parent, unsigned int langIndex);

			virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd);
			void			ConfigChanged ();
	};

	ClassicGlobalPage*		classicGlobalPage;
	ClassicGlidePage*		classicGlidePage;
	ClassicVESAPage*		classicVESAPage;
	ClassicRendererPage*	classicRendererPage;

	class DependencyTable	{
	public:
		class Dependency	{
		public:
			unsigned int	type;
			unsigned int	data;
			int				numofdependingitems;
			ConfigPage		depending;
			unsigned int	dependingitems[MAXDEPENDINGITEMS];

			Dependency operator= (const Dependency& other)
			{
				type = other.type;
				data = other.data;
				numofdependingitems = other.numofdependingitems;
				depending = other.depending;
				for (int i = 0; i < numofdependingitems; i++) {
					dependingitems[i] = other.dependingitems[i];
				}
				return *this;
			}
		};

		class MultipleDepItem	{
		public:
			ConfigPage		depending;
			unsigned int	dependingitem;
			bool			conjunction;

		};

	private:
		unsigned int		depNum;
		Dependency			dependencies[32];
		unsigned int		multiDepNum;
		MultipleDepItem		multiDependencies[32];

	public:
		DependencyTable (): depNum (0),
							multiDepNum (0)
		{
		}

		bool			ItemStatus (ClassicConfigView* configView, ConfigPage page, unsigned int item)
		{
			if (configView->state != ConfigView::Valid)
				return false;
			
			for (unsigned int i = 0; i < multiDepNum; i++) {
				multiDependencies[i].conjunction = true;
			}
			bool b = false;
			for (unsigned int j = 0; j < depNum; j++) {
				switch (dependencies[j].type)	{
					case DTYPE_FLAGMASKTRUE:
						b = !(config.Flags & dependencies[j].data);
						break;
					case DTYPE_FLAGMASKFALSE:
						b = config.Flags & dependencies[j].data;
						break;
					case DTYPE_FLAG2MASKTRUE:
						b = !(config.Flags2 & dependencies[j].data);
						break;
					case DTYPE_FLAG2MASKFALSE:
						b = config.Flags2 & dependencies[j].data;
						break;
					case DTYPE_FUNCTION:
						b = (*( (bool (*)()) dependencies[j].data)) ();
						break;
					case DTYPE_FUNCTIONFALSE:
						b = !(*( (bool (*)()) dependencies[j].data)) ();
						break;
					case DTYPE_PLATFORM:
						b = (platform != dependencies[j].data);
						break;
					case DTYPE_DX9FLAGMASKTRUE:
						b = !(config.dx9ConfigBits & dependencies[j].data);
						break;
					case DTYPE_DX9FLAGMASKFALSE:
						b = (config.dx9ConfigBits & dependencies[j].data);
						break;
				}

				for (unsigned int k = 0; k < dependencies[j].numofdependingitems; k++)	{
					unsigned int i = 0;
					for (; i < multiDepNum; i++) {
						if ((dependencies[j].depending == multiDependencies[i].depending) &&
							(dependencies[j].dependingitems[k] == multiDependencies[i].dependingitem) )	{
							multiDependencies[i].conjunction &= b;
							break;
						}
					}
					if (i == multiDepNum)	{
						if (page == None) {
							configView->EnableItemOnPage (dependencies[j].depending, dependencies[j].dependingitems[k], b);
						} else {
							if (page == dependencies[j].depending && item == dependencies[j].dependingitems[k]) {
								return b;
							}
						}
					}
				}
			}
			for (unsigned int i = 0; i < multiDepNum; i++) {
				if (page == None) {
					configView->EnableItemOnPage (multiDependencies[i].depending, multiDependencies[i].dependingitem, multiDependencies[i].conjunction);
				} else {
					if (page == multiDependencies[i].depending && item == multiDependencies[i].dependingitem) {
						return multiDependencies[i].conjunction;
					}
				}
			}
			
			return true;
		}

		void			AddDependency (const Dependency& dependency)
		{
			dependencies[depNum++] = dependency;
		}

		void			AddItemWithMultiDependency (ConfigPage onPage, unsigned int item)
		{
			multiDependencies[multiDepNum].depending = onPage;
			multiDependencies[multiDepNum++].dependingitem = item;
		}

	} dependencyTable;

	static bool	IsHwBuffCachingEnabled ()	{

		return ( (config.Flags2 & (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE)) != (CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE) );
		//return ( (!(config.Flags2 & CFG_DISABLELFBREAD)) || ( (!(config.Flags2 & CFG_DISABLELFBWRITE)) && (!(config.Flags2 & CFG_LFBFORCETEXTURING)) ) );
	}

	static bool IsItGlide211Config ()
	{
		return (config.GlideVersion == 0x211);
	}


	static bool IsRescaleChangesOnlyEnabled ()
	{
		return ( (config.Flags2 & CFG_DISABLELFBWRITE) || (!(config.Flags & CFG_LFBFASTUNMATCHWRITE)) || ( (config.dxres==0) && (config.dyres==0) ) );
	}

	static bool	IsCurrentApiAvailable ()
	{
		return adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi);
	}

	
private:
	void	ApiChanged ();
	void	AdapterChanged ();

	void			EnableItemOnPage (ConfigPage onPage, unsigned int item, bool enable)
	{
		UI::Page* page = NULL;
		switch (onPage)	{
			case Global:	page = classicGlobalPage; break;
			case Glide:		page = classicGlidePage; break;
			case VESA:		page = classicVESAPage; break;
			case Renderer:	page = classicRendererPage; break;
		}
		if (page != NULL) {
			EnableWindow (page->GetWindowFromItem (item), enable ? TRUE : FALSE);
		}
	}

	struct StateProcInfo {
		ClassicConfigView*	view;
		ConfigPage			page;
		ConfigState			state;
	};


	static BOOL CALLBACK	PageStateProc (HWND hwnd, LPARAM lParam)
	{
		StateProcInfo* info = (StateProcInfo*) lParam;

		switch (info->state) {
			case Invalid:
				EnableWindow (hwnd, FALSE);
				break;
			
			case Valid:
				EnableWindow (hwnd, info->view->dependencyTable.ItemStatus (info->view, info->page, GetDlgCtrlID (hwnd)));
				break;

		}
		return TRUE;
	}


	void			SetPageState (ConfigPage onPage, HWND hwnd, ConfigState state)
	{
		/*UI::Page* page = NULL;
		switch (onPage)	{
			case Global:	page = classicGlobalPage; break;
			case Glide:		page = classicGlidePage; break;
			case VESA:		page = classicVESAPage; break;
			case Renderer:	page = classicRendererPage; break;
		}
		if (page != NULL) {
			HWND hwnd =
		}*/
		if (hwnd != NULL) {
			StateProcInfo info = {this, onPage, state};
			EnumChildWindows (hwnd, ClassicConfigView::PageStateProc, (LPARAM) &info);
		}
	}

public:
	ClassicConfigView (UI::MainDialog& handler, unsigned int langIndex);
	~ClassicConfigView ();

	virtual void			Init ();

	virtual void			SetLanguage (unsigned int langIndex);
	virtual void			SetConfigState (ConfigState stateP);

	UI::MainDialog&			GetMainDialog () const;

	virtual unsigned int	GetPageNum ();
	virtual void			GetPageName (unsigned int pageIndex, char* name);
	virtual HWND			GetPageWindow (unsigned int pageIndex, HWND mainDlgWnd, HWND toolTipWnd);

	virtual unsigned int	GetCustomMenuItemsNum ();
	virtual unsigned int	GetCustomMenuItem (unsigned int index, char* text);
	virtual bool			IsCustomMenuItemCheckable (unsigned int id);
	virtual bool			HandleMenuItem (unsigned int id, bool checked);

	virtual void			ConfigChanged ();
	virtual void			ThemeChanged ();

	bool			GetItemStatus (ConfigPage page, unsigned int item)
	{
		return dependencyTable.ItemStatus (NULL, page, item);
	}

	void			SetAllItemsStatus ()
	{
		dependencyTable.ItemStatus (this, None, 0);
	}
};


ClassicConfigView::ClassicConfigView (UI::MainDialog& mainDialog, unsigned int langIndex):
	ConfigView			(mainDialog, langIndex),
	classicGlobalPage	(NULL),
	classicGlidePage	(NULL),
	classicVESAPage		(NULL),
	classicRendererPage (NULL)
{
	static	DependencyTable::Dependency	d0	= {DTYPE_FLAGMASKTRUE, CFG_WINDOWED, 3, Global, IDC_SBDBOX,IDC_16BIT,IDC_32BIT};
	static	DependencyTable::Dependency	d1	= {DTYPE_FLAGMASKFALSE, CFG_GRABENABLE, 2, Global, IDC_SAVETOFILE, IDC_SAVETOCLIPBOARD};
	static	DependencyTable::Dependency	d2	= {DTYPE_FLAGMASKFALSE, CFG_VESAENABLE, 11, VESA, IDC_GBVESAREFFREQ,
																							   IDC_SLIDER2,
																							   IDC_SLIDERSTATIC2,
																							   IDC_45HZ,
																							   IDC_70HZ,
																							   IDC_GBVESAVIDMEM,
																							   IDC_VESALABEL2,
																							   IDC_COMBOVIDMEMSIZE,
																							   IDC_GBVESAMISC,
																							   IDC_SUPPORTMODE13H,
																							   IDC_DISABLEFLATLFB};
	static	DependencyTable::Dependency	d3	= {DTYPE_FUNCTION, (unsigned int) IsHwBuffCachingEnabled, 1, Glide, IDC_HWBUFFCACHING};
	static	DependencyTable::Dependency	d4	= {DTYPE_FLAG2MASKTRUE, CFG_DISABLELFBWRITE, 1, Glide, IDC_FASTWRITE};
	static	DependencyTable::Dependency	d5	= {DTYPE_PLATFORM, PWINDOWS, 2, Glide, IDC_TOMBRAIDER, IDC_TIMERLOOP};
	static	DependencyTable::Dependency	d6	= {DTYPE_PLATFORM, PWINDOWS, 10, Global, IDC_MISC,IDC_HIDECONSOLE,IDC_SETMOUSEFOCUS,
																					   IDC_WINXPDOS,IDC_VDDMODE,IDC_ACTIVEBACKG,IDC_CTRLALT,IDC_CLIPOPF, IDC_PRESERVEWINDOWSIZE,
																					   IDC_PRESERVEASPECTRATIO};
	static	DependencyTable::Dependency	d7	= {DTYPE_FLAGMASKFALSE, CFG_SETMOUSEFOCUS, 1, Global, IDC_CTRLALT};
	static	DependencyTable::Dependency	d8	= {DTYPE_FLAGMASKTRUE, CFG_DISABLEMIPMAPPING, 1, Glide, IDC_AUTOGENMIPMAPS};
	static	DependencyTable::Dependency	d9	= {DTYPE_FLAGMASKTRUE, CFG_WINDOWED, 3, Glide, IDC_CLOSESTFREQ, IDC_REFRATESTATIC, IDC_REFRESHRATEVALUE};
	static	DependencyTable::Dependency	d10	= {DTYPE_FLAG2MASKTRUE, CFG_CLOSESTREFRESHFREQ, 2, Glide, IDC_REFRATESTATIC, IDC_REFRESHRATEVALUE/*, IDC_RRMONITOR, IDC_RRAPP*/};
	static	DependencyTable::Dependency	d11	= {DTYPE_FLAGMASKTRUE, CFG_DISABLEMIPMAPPING, 1, Glide, IDC_TRILINEARMIPMAPS};
	static	DependencyTable::Dependency	d12	= {DTYPE_FUNCTION, (unsigned int) IsRescaleChangesOnlyEnabled, 1, Glide, IDC_RESCALECHANGES};
	static	DependencyTable::Dependency	d13	= {DTYPE_FUNCTIONFALSE, (unsigned int) IsItGlide211Config, 1, Glide, IDC_REALHW};
	//static	Dependency	d14	= {DTYPE_FLAG2MASKTRUE, CFG_LFBDISABLETEXTURING, 1, HWND_GLIDE, IDC_FASTWRITE};
	static	DependencyTable::Dependency	d15	= {DTYPE_FLAG2MASKTRUE, CFG_DISABLELFBWRITE, 1, Glide, IDC_LFBTEXTURETILES};
	static	DependencyTable::Dependency	d16	= {DTYPE_FLAGMASKFALSE, CFG_WINDOWED, 2, Global, IDC_PRESERVEWINDOWSIZE, IDC_PRESERVEASPECTRATIO};
	//static	DependencyTable::Dependency	d17	= {DTYPE_FUNCTIONFALSE, (unsigned int) IsItGlide211Config, 1, HWND_HEADER, IDC_PDOS};
	static	DependencyTable::Dependency	d18	= {DTYPE_DX9FLAGMASKTRUE, CFG_DX9FFSHADERS, 1, Renderer, IDC_DX9GRAYSCALERENDERING};
	static	DependencyTable::Dependency	d19	= {DTYPE_DX9FLAGMASKTRUE, CFG_DX9FFSHADERS, 1, Renderer, IDC_DX9_HWPALETTEGEN};
	static	DependencyTable::Dependency	d20	= {DTYPE_FUNCTION, (unsigned int) IsCurrentApiAvailable, 2, Global, IDC_CBDISPDEVS, IDC_CBDISPDRIVERS};
	static	DependencyTable::Dependency	d21	= {DTYPE_FUNCTION, (unsigned int) IsCurrentApiAvailable, 2, Glide, IDC_COMBORESOLUTION, IDC_REFRESHRATEVALUE};

	dependencyTable.AddDependency (d0);
	dependencyTable.AddDependency (d1);
	dependencyTable.AddDependency (d2);
	dependencyTable.AddDependency (d3);
	dependencyTable.AddDependency (d4);
	dependencyTable.AddDependency (d5);
	dependencyTable.AddDependency (d6);
	dependencyTable.AddDependency (d7);
	dependencyTable.AddDependency (d8);
	dependencyTable.AddDependency (d9);
	dependencyTable.AddDependency (d10);
	dependencyTable.AddDependency (d11);
	dependencyTable.AddDependency (d12);
	dependencyTable.AddDependency (d13);
	//dependencyTable.AddDependency (d14);
	dependencyTable.AddDependency (d15);
	dependencyTable.AddDependency (d16);
	//dependencyTable.AddDependency (d17);
	dependencyTable.AddDependency (d18);
	dependencyTable.AddDependency (d19);
	dependencyTable.AddDependency (d20);
	dependencyTable.AddDependency (d21);

	dependencyTable.AddItemWithMultiDependency (Global, IDC_CTRLALT);
	dependencyTable.AddItemWithMultiDependency (Glide, IDC_REFRESHRATEVALUE);
	dependencyTable.AddItemWithMultiDependency (Glide, IDC_REFRATESTATIC);
	dependencyTable.AddItemWithMultiDependency (Glide, IDC_FASTWRITE);
	dependencyTable.AddItemWithMultiDependency (Global, IDC_PRESERVEWINDOWSIZE);
	dependencyTable.AddItemWithMultiDependency (Global, IDC_PRESERVEASPECTRATIO);
}


ClassicConfigView::~ClassicConfigView ()
{
	delete classicGlobalPage;
	delete classicGlidePage;
	delete classicVESAPage;
	delete classicRendererPage;
}


void			ClassicConfigView::Init ()
{
	mainDialog.ShowPage (0);
	mainDialog.ShowPage (1);
	if (platform == PDOS) {
		mainDialog.ShowPage (2);
	}
	mainDialog.SelectPage (0);
}


unsigned int	ClassicConfigView::GetPageNum ()
{
	return 4;
}


void			ClassicConfigView::GetPageName (unsigned int pageIndex, char* name)
{
	ConfigPage page = (ConfigPage) pageIndex;
	switch (page) {
		case Global:
			resourceManager.GetString (langIndex, IDS_CLASSICTABPANESTR1, name);
			break;
		case Glide:
			resourceManager.GetString (langIndex, IDS_CLASSICTABPANESTR2, name);
			break;
		case VESA:
			resourceManager.GetString (langIndex, IDS_CLASSICTABPANESTR3, name);
			break;
		case Renderer:
			resourceManager.GetString (langIndex, IDS_CLASSICTABPANESTR4, name);
			break;
		default:
			name[0] = 0x0;
			break;
	}
}


HWND			ClassicConfigView::GetPageWindow (unsigned int pageIndex, HWND mainDlgWnd, HWND toolTipWnd)
{
	switch (pageIndex)	{
		case 0:
			if (classicGlobalPage == NULL) {
				classicGlobalPage = new ClassicGlobalPage (this, langIndex);
			}
			return (classicGlobalPage != NULL) ? classicGlobalPage->GetPageWindow (mainDlgWnd, toolTipWnd) : NULL;

		case 1:
			if (classicGlidePage == NULL) {
				classicGlidePage = new ClassicGlidePage (this, langIndex);
			}
			return (classicGlidePage != NULL) ? classicGlidePage->GetPageWindow (mainDlgWnd, toolTipWnd) : NULL;

		case 2:
			if (classicVESAPage == NULL) {
				classicVESAPage = new ClassicVESAPage (this, langIndex);
			}
			return (classicVESAPage != NULL) ? classicVESAPage->GetPageWindow (mainDlgWnd, toolTipWnd) : NULL;

		case 3:
			if (classicRendererPage == NULL) {
				classicRendererPage = new ClassicRendererPage (this, langIndex);
			}
			return (classicRendererPage != NULL) ? classicRendererPage->GetPageWindow (mainDlgWnd, toolTipWnd) : NULL;

		default:
			return NULL;
	}
}


bool			ClassicConfigView::IsCustomMenuItemCheckable (unsigned int index)
{
	return true;
}


unsigned int	ClassicConfigView::GetCustomMenuItemsNum ()
{
	return 1;
}


unsigned int	ClassicConfigView::GetCustomMenuItem (unsigned int /*index*/, char* text)
{
	resourceManager.GetString (langIndex, IDS_CLASSICVIEWMENUSTR1, text);
	return IDS_CLASSICVIEWMENUSTR1;
}


bool			ClassicConfigView::HandleMenuItem (unsigned int id, bool checked)
{
	switch (id) {
		case IDS_CLASSICVIEWMENUSTR1:
			if (checked) {
				mainDialog.ShowPage (3);
			} else {
				mainDialog.HidePage (3);
			}
			break;

		default:
			return false;
	}
	return true;
}


void			ClassicConfigView::SetLanguage (unsigned int langIndex)
{
	ConfigView::SetLanguage (langIndex);
	if (classicGlobalPage != NULL) {
		classicGlobalPage->SetLanguage (langIndex);
	}
	if (classicGlidePage != NULL) {
		classicGlidePage->SetLanguage (langIndex);
	}
	if (classicVESAPage != NULL) {
		classicVESAPage->SetLanguage (langIndex);
	}
	if (classicRendererPage != NULL) {
		classicRendererPage->SetLanguage (langIndex);
	}
}


void			ClassicConfigView::SetConfigState (ConfigState stateP)
{
	state = stateP;
	if (classicGlobalPage != NULL) {
		classicGlobalPage->ConfigChanged ();
	}
	if (classicGlidePage != NULL) {
		classicGlidePage->ConfigChanged ();
	}
	if (classicVESAPage != NULL) {
		classicVESAPage->ConfigChanged ();
	}
	if (classicRendererPage != NULL) {
		classicRendererPage->ConfigChanged ();
	}
}


UI::MainDialog&		ClassicConfigView::GetMainDialog () const
{
	return mainDialog;
}


ConfigView*		CreateClassicConfigView (UI::MainDialog& handler, unsigned int langIndex)
{
	return new ClassicConfigView (handler, langIndex);
}


// ------------------------------------ Global Page -----------------------------------------
void	ClassicConfigView::ClassicGlobalPage::EnumerateAdapters (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDEVS), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
	for (unsigned int adapterNum = 0; adapterNum < adapterHandler.GetNumOfAdapters ((RendererAPI) config.rendererApi); adapterNum++) {
		SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDEVS), CB_ADDSTRING,
			(WPARAM) 0, (LPARAM) adapterHandler.GetDescriptionOfAdapter ((RendererAPI) config.rendererApi, adapterNum));
	}

	EnumerateRenderingDevices (pageWnd, config.dispdev);
}


void	ClassicConfigView::ClassicGlobalPage::EnumerateRenderingDevices (HWND pageWnd, unsigned short adapter)
{
	SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDRIVERS), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);

	if (adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi))  {
		char	strBuffer[MAXSTRINGLENGTH];
		resourceManager.GetString (langIndex, IDS_DDRIVERDEFAULT, strBuffer);
		// Automatically selected
		SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDRIVERS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
		for (unsigned int deviceNum = 0; deviceNum < adapterHandler.GetNumOfDeviceTypes ((RendererAPI) config.rendererApi, adapter); deviceNum++) {
			SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDRIVERS), CB_ADDSTRING, (WPARAM) 0, (LPARAM) adapterHandler.GetDescriptionOfDeviceType ((RendererAPI) config.rendererApi, adapter, deviceNum));
		}
	}
}


void	ClassicConfigView::ClassicGlobalPage::SetRendererApiComboBox (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_SETCURSEL, config.rendererApi, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetAdapterComboBox (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDEVS), CB_SETCURSEL, config.dispdev, (LPARAM) 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetDeviceComboBox (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDRIVERS), CB_SETCURSEL, config.dispdriver, (LPARAM) 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetScreenModeBox (HWND pageWnd)
{
	unsigned int	f,w;

	if (config.Flags & CFG_WINDOWED)	{
		f = BST_UNCHECKED;
		w = BST_CHECKED;
	} else {
		w = BST_UNCHECKED;
		f = BST_CHECKED;
	}
	SendMessage (GetDlgItem (pageWnd, IDC_FULLSCR), BM_SETCHECK, f, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_WINDOWED), BM_SETCHECK, w, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlobalPage::SetScreenBitDepthBox (HWND pageWnd)
{
	unsigned int	_16,_32;

	if (config.Flags & CFG_DISPMODE32BIT)	{
		_16 = BST_UNCHECKED;
		_32 = BST_CHECKED;
	} else {
		_32 = BST_UNCHECKED;
		_16 = BST_CHECKED;
	}
	SendMessage (GetDlgItem (pageWnd, IDC_16BIT), BM_SETCHECK, _16, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_32BIT), BM_SETCHECK, _32, 0);
}

void	ClassicConfigView::ClassicGlobalPage::SetHideConsoleBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_HIDECONSOLE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_HIDECONSOLE), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetSetMouseFocusBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_SETMOUSEFOCUS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_SETMOUSEFOCUS), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlobalPage::SetCTRLALTBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_CTRLALT) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_CTRLALT), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetVDDModeBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_NTVDDMODE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_VDDMODE), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetActiveBackgroundBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_ACTIVEBACKGROUND) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_ACTIVEBACKG), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetPreserveWindowSizeBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_PRESERVEWINDOWSIZE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_PRESERVEWINDOWSIZE), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetPreserveAspectRatioBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_PRESERVEASPECTRATIO) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_PRESERVEASPECTRATIO), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetCLIPOPFBox (HWND pageWnd)
{
	unsigned int	checkState = (config.Flags2 & CFG_FORCECLIPOPF) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_CLIPOPF), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlobalPage::SetCaptureOnBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_GRABENABLE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_SCAPTUREON), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlobalPage::SetGrabModeBox (HWND pageWnd)
{
	unsigned int	sfile, sclipb;
	
	if (config.Flags & CFG_GRABCLIPBOARD)	{
		sfile = BST_UNCHECKED;
		sclipb = BST_CHECKED;
	} else {
		sfile = BST_CHECKED;
		sclipb = BST_UNCHECKED;
	}
	SendMessage (GetDlgItem (pageWnd, IDC_SAVETOFILE), BM_SETCHECK, sfile, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_SAVETOCLIPBOARD), BM_SETCHECK, sclipb, 0);	
}


INT_PTR CALLBACK ClassicConfigView::ClassicGlobalPage::GlobalPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_CTLCOLORSTATIC:
			if (GetDlgCtrlID ((HWND) lParam) == IDC_RENDERERAPISTATUS) {
				SetTextColor ((HDC) wParam, RGB (255, 128, 0));
				return (INT_PTR) GetStockObject (HOLLOW_BRUSH);
			}
			break;	
	}
	return UI::Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}


bool			ClassicConfigView::ClassicGlobalPage::Init (HWND pageWnd)
{
	// Setup 'Renderer API' combo
	SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
	SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_ADDSTRING, (WPARAM) 0, (LPARAM) adapterHandler.GetApiName (DirectX7));
	SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_ADDSTRING, (WPARAM) 0, (LPARAM) adapterHandler.GetApiName (DirectX9));
	SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);


	AddControlToToolTip (pageWnd, IDC_CBDISPDEVS);
	AddControlToToolTip (pageWnd, IDC_CBDISPDRIVERS);
	AddControlToToolTip (pageWnd, IDC_FULLSCR);
	AddControlToToolTip (pageWnd, IDC_WINDOWED);
	AddControlToToolTip (pageWnd, IDC_16BIT);
	AddControlToToolTip (pageWnd, IDC_32BIT);
	AddControlToToolTip (pageWnd, IDC_SAVETOFILE);
	AddControlToToolTip (pageWnd, IDC_SAVETOCLIPBOARD);
	AddControlToToolTip (pageWnd, IDC_VDDMODE);
	AddControlToToolTip (pageWnd, IDC_ACTIVEBACKG);
	AddControlToToolTip (pageWnd, IDC_HIDECONSOLE);
	AddControlToToolTip (pageWnd, IDC_SETMOUSEFOCUS);
	AddControlToToolTip (pageWnd, IDC_CTRLALT);
	AddControlToToolTip (pageWnd, IDC_PRESERVEWINDOWSIZE);
	AddControlToToolTip (pageWnd, IDC_PRESERVEASPECTRATIO);

	SetPageToConfig (pageWnd);
	return true;
}


bool			ClassicConfigView::ClassicGlobalPage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_FULLSCR:				config.Flags &= ~CFG_WINDOWED; SetScreenModeBox (pageWnd); break;
		case IDC_WINDOWED:				config.Flags |= CFG_WINDOWED; SetScreenModeBox (pageWnd); break;
		case IDC_16BIT:					config.Flags &= ~CFG_DISPMODE32BIT; SetScreenBitDepthBox (pageWnd); break;
		case IDC_32BIT:					config.Flags |= CFG_DISPMODE32BIT; SetScreenBitDepthBox (pageWnd); break;
		case IDC_HIDECONSOLE:			config.Flags2 ^= CFG_HIDECONSOLE; SetHideConsoleBox (pageWnd); break;
		case IDC_SETMOUSEFOCUS:			config.Flags ^= CFG_SETMOUSEFOCUS; SetSetMouseFocusBox (pageWnd); break;
		case IDC_CTRLALT:				config.Flags ^= CFG_CTRLALT; SetCTRLALTBox (pageWnd); break;
		case IDC_VDDMODE:				config.Flags ^= CFG_NTVDDMODE; SetVDDModeBox (pageWnd); break;
		case IDC_ACTIVEBACKG:			config.Flags ^= CFG_ACTIVEBACKGROUND; SetActiveBackgroundBox (pageWnd); break;
		case IDC_PRESERVEWINDOWSIZE:	config.Flags2 ^= CFG_PRESERVEWINDOWSIZE; SetPreserveWindowSizeBox (pageWnd); break;
		case IDC_PRESERVEASPECTRATIO:	config.Flags2 ^= CFG_PRESERVEASPECTRATIO; SetPreserveAspectRatioBox (pageWnd); break;
		case IDC_CLIPOPF:				config.Flags2 ^= CFG_FORCECLIPOPF; SetCLIPOPFBox (pageWnd); break;
		case IDC_SCAPTUREON:			config.Flags ^= CFG_GRABENABLE; SetCaptureOnBox (pageWnd); break;
		case IDC_SAVETOFILE:			config.Flags &= ~CFG_GRABCLIPBOARD; SetGrabModeBox (pageWnd); break;
		case IDC_SAVETOCLIPBOARD:		config.Flags |= CFG_GRABCLIPBOARD; SetGrabModeBox (pageWnd); break;

		default:
			return false;
	}
	return true;
}


bool			ClassicConfigView::ClassicGlobalPage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_RENDERER:
			{
				config.rendererApi = (unsigned char) SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_GETCURSEL, 0, 0);
				bool isApiAvaliable = adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi);
				ShowWindow (GetDlgItem (pageWnd, IDC_RENDERERAPISTATUS), isApiAvaliable ? SW_HIDE : SW_SHOWNA);
				parent->SetAllItemsStatus ();
				EnumerateAdapters (pageWnd);
				SetAdapterComboBox (pageWnd);
				SetDeviceComboBox (pageWnd);
				parent->ApiChanged ();
			}
			break;

		case IDC_CBDISPDEVS:
			{
				config.dispdev = (unsigned short) SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDEVS), CB_GETCURSEL, 0, 0);
				SetDeviceComboBox (pageWnd);
				parent->AdapterChanged ();
			}
			break;
	
		default:
			return false;
	}
	return true;
}


ClassicConfigView::ClassicGlobalPage::ClassicGlobalPage (ClassicConfigView* parent, unsigned int langIndex):
	Page		(parent->GetMainDialog (), langIndex),
	parent		(parent)
{
}


HWND			ClassicConfigView::ClassicGlobalPage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::GlobalPage, GlobalPageProc);
}


void			ClassicConfigView::ClassicGlobalPage::SetPageToConfig (HWND pageWnd)
{
	SetRendererApiComboBox (pageWnd);
	bool isApiAvaliable = adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi);
	ShowWindow (GetDlgItem (pageWnd, IDC_RENDERERAPISTATUS), isApiAvaliable ? SW_HIDE : SW_SHOWNA);
	if (SendMessage (GetDlgItem (pageWnd, IDC_CBDISPDEVS), CB_GETCOUNT, (WPARAM) 0, (LPARAM) 0) == 0 ||
		SendMessage (GetDlgItem (pageWnd, IDC_RENDERER), CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) != config.rendererApi) {
		EnumerateAdapters (pageWnd);
		adapterHandler.RefreshResolutionListByAdapter ((RendererAPI) config.rendererApi, config.dispdev);
	}
	SetAdapterComboBox (pageWnd);
	SetDeviceComboBox (pageWnd);

	SetScreenModeBox (pageWnd);
	SetScreenBitDepthBox (pageWnd);
	SetHideConsoleBox (pageWnd);
	SetSetMouseFocusBox (pageWnd);
	SetCTRLALTBox (pageWnd);
	SetVDDModeBox (pageWnd);
	SetActiveBackgroundBox (pageWnd);
	SetPreserveWindowSizeBox (pageWnd);
	SetPreserveAspectRatioBox (pageWnd);
	SetCLIPOPFBox (pageWnd);
	SetCaptureOnBox (pageWnd);
	SetGrabModeBox (pageWnd);

	//parent->SetAllItemsStatus ();
	parent->SetPageState (Global, pageWnd, parent->state);
}


void			ClassicConfigView::ClassicGlobalPage::ConfigChanged ()
{
	SetPageToConfig (pageWnd[langIndex]);
}

// ------------------------------------ Glide Page -----------------------------------------

unsigned int	ClassicConfigView::ClassicGlidePage::texMemSizes[]	= {1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 10240, 12288, 14336, 16384, 32768, 65536 };

char*	ClassicConfigView::ClassicGlidePage::texMemSizeStr[] = { "1024 kB", "2048 kB", "3072 kB", "4096 kB", "5120 kB", "6144 kB",
																 "7168 kB", "8192 kB", "10240 kB", "12288 kB", "14336 kB", "16384 kB",
																 "32768 kB", "65536 kB" };

WORD	ClassicConfigView::ClassicGlidePage::lfbAccessStrIDs[] = {IDS_ENABLELFBACCESS, IDS_LFBDISABLEREAD, IDS_DISABLELFBWRITE, IDS_DISABLELFBACCESS};
WORD	ClassicConfigView::ClassicGlidePage::ckMethodStrsIDs[] = {IDS_CKAUTOMATIC, IDS_CKALPHABASED, IDS_CKNATIVE, IDS_NATIVEFORTNT};


void	ClassicConfigView::ClassicGlidePage::EnumerateResolutions (HWND pageWnd, unsigned short adapter)
{
	char	strBuffer[MAXSTRINGLENGTH];
	resourceManager.GetString (langIndex, IDS_RESDEFAULT, strBuffer);
	SendMessage (GetDlgItem (pageWnd, IDC_COMBORESOLUTION), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
	SendMessage (GetDlgItem (pageWnd, IDC_COMBORESOLUTION), CB_ADDSTRING, (WPARAM) 0, (LPARAM) (strBuffer));

	if (adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi))  {
		char	strBuffer[MAXSTRINGLENGTH];
		for (unsigned int i = 0; i < adapterHandler.GetNumOfResolutions (); i++) {
			AdapterHandler::DisplayMode mode = adapterHandler.GetResolution (i);
			strBuffer[0] = 0;
			sprintf(strBuffer, "%dx%d", mode.x, mode.y);
			SendMessage (GetDlgItem (pageWnd, IDC_COMBORESOLUTION), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
		}
	}
}


void	ClassicConfigView::ClassicGlidePage::EnumerateRefreshRates (HWND pageWnd, unsigned short adapter)
{
	char	strBuffer[MAXSTRINGLENGTH];
	resourceManager.GetString (langIndex, IDS_REFRESHDEFAULT, strBuffer);
	SendMessage (GetDlgItem (pageWnd, IDC_REFRESHRATEVALUE), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
	SendMessage (GetDlgItem (pageWnd, IDC_REFRESHRATEVALUE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) (strBuffer));
	
	if (adapterHandler.IsAPIAvailable ((RendererAPI) config.rendererApi))  {
		for (unsigned int i = 0; i < adapterHandler.GetNumOfRefreshRates (); i++) {
			unsigned int freq = adapterHandler.GetRefreshRate (i);
			strBuffer[0] = 0;
			sprintf(strBuffer, "%dHz", freq);
			SendMessage (GetDlgItem (pageWnd, IDC_REFRESHRATEVALUE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) strBuffer);
		}
	}
}


void	ClassicConfigView::ClassicGlidePage::SetTexBitDepthBox (HWND pageWnd)
{
	unsigned int	tex16, tex32, texopt;

	tex16 = BST_UNCHECKED;
	tex32 = BST_UNCHECKED;
	texopt = BST_CHECKED;
	if (config.Flags & CFG_TEXTURES32BIT)	{
		tex16 = BST_UNCHECKED;
		tex32 = BST_CHECKED;
		texopt = BST_UNCHECKED;
	}
	if (config.Flags & CFG_TEXTURES16BIT)	{
		tex16 = BST_CHECKED;
		tex32 = BST_UNCHECKED;
		texopt = BST_UNCHECKED;
	}
	SendMessage (GetDlgItem (pageWnd, IDC_TEX16), BM_SETCHECK, tex16, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_TEX32), BM_SETCHECK, tex32, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_TEXOPT), BM_SETCHECK, texopt, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetClosestRefRateBox (HWND pageWnd)
{
	unsigned int m = (config.Flags2 & CFG_CLOSESTREFRESHFREQ) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_CLOSESTFREQ), BM_SETCHECK, m, 0);	
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlidePage::SetVSyncBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_VSYNC) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_VSYNC), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetLfbAccessComboBox (HWND pageWnd)
{
	unsigned int j = 0;
	if (config.Flags2 & CFG_DISABLELFBREAD) j = 1;
	if (config.Flags2 & CFG_DISABLELFBWRITE) j |= 2;
	SendMessage (GetDlgItem (pageWnd, IDC_COMBOLFBACCESS), CB_SETCURSEL, j, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetHwBuffCachingBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_HWLFBCACHING) ? BST_CHECKED :  BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_HWBUFFCACHING), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetFastWriteBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_LFBFASTUNMATCHWRITE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_FASTWRITE), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlidePage::SetTextureTilesBox (HWND pageWnd)
{
	unsigned int checkState = (GetLfbTexureTilingMethod (config.Flags2) == CFG_LFBDISABLEDTEXTURING) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_LFBTEXTURETILES), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlidePage::SetRealHwBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_LFBREALVOODOO) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_REALHW), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetCKComboBox (HWND pageWnd)
{
	SendMessage (GetDlgItem (pageWnd, IDC_COMBOCOLORKEY), CB_SETCURSEL, GetCKMethodValue(config.Flags), 0);
}


void	ClassicConfigView::ClassicGlidePage::SetTexMemSizeComboBox (HWND pageWnd)
{
	unsigned int i = config.TexMemSize >> 10;
	unsigned int j = 0;
	for (; (i != texMemSizes[j]) && (j < 14); j++);
	if (j >= 14) j = 0;
	SendMessage (GetDlgItem (pageWnd, IDC_COMBOTEXMEMSIZE), CB_SETCURSEL, j, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetGammaControl (HWND pageWnd)
{
	char	slider[8];
	
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDER1), TBM_SETPOS, (LPARAM) TRUE, (WPARAM) config.Gamma);
	sprintf (slider, "%d%%", config.Gamma);
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDERSTATIC), WM_SETTEXT, (LPARAM) 0, (WPARAM) slider);
}


void	ClassicConfigView::ClassicGlidePage::SetResolutionComboBox (HWND pageWnd)
{
	unsigned int j = 0;
	for (unsigned int i = 0; i < adapterHandler.GetNumOfResolutions (); i++)	{
		AdapterHandler::DisplayMode mode = adapterHandler.GetResolution (i);
		if (config.dxres == mode.x && config.dyres == mode.y) {
			j = i + 1;
		}
	}
    SendMessage (GetDlgItem (pageWnd, IDC_COMBORESOLUTION), CB_SETCURSEL, j, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetRefreshFreqComboBox (HWND pageWnd)
{
	unsigned int j = 0;
	for (unsigned int i = 0; i < adapterHandler.GetNumOfRefreshRates (); i++)	{
		unsigned int freq = adapterHandler.GetRefreshRate (i);
		if (config.RefreshRate == freq) {
			j = i + 1;
		}
	}
    SendMessage (GetDlgItem (pageWnd, IDC_REFRESHRATEVALUE), CB_SETCURSEL, j, 0);
}

/*void	ClassicConfigView::ClassicGlidePage::SetRefreshFreq (HWND pageWnd)
{
	char	slider[8];
	
	SendMessage (GetDlgItem (hDlgVESA, IDC_SLIDER2), TBM_SETPOS, (LPARAM) TRUE, (WPARAM) config.VideoRefreshFreq);
	sprintf (slider, "%dHz", config.VideoRefreshFreq);
	SendMessage (GetDlgItem (hDlgVESA, IDC_SLIDERSTATIC2), WM_SETTEXT, (LPARAM) 0, (WPARAM) slider);
}*/


void	ClassicConfigView::ClassicGlidePage::SetMipMappingDisableBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_DISABLEMIPMAPPING) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_MIPMAPPINGDISABLE), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicGlidePage::SetTrilinearMipMapsBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_FORCETRILINEARMIPMAPS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_TRILINEARMIPMAPS), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetAutogenerateMipMapsBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_AUTOGENERATEMIPMAPS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_AUTOGENMIPMAPS), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetBilinearFilteringBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_BILINEARFILTER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_BILINEARFILTERING), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetTexMemScaleBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_SCALETEXMEM) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_TEXMEMSCALE), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetTripleBufferBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_TRIPLEBUFFER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_TRIPLEBUFFER), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetTombRaiderBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_TOMBRAIDER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd,IDC_TOMBRAIDER), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetHwVBuffBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_ENABLEHWVBUFFER) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_HWVBUFFER), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetGammaRampBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_ENABLEGAMMARAMP) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_GAMMARAMP), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetTimerBoostBox (HWND pageWnd)
{
	unsigned int checkState = config.dosTimerBoostPeriod ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_TIMERLOOP), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicGlidePage::SetForceWBufferingBox (HWND pageWnd)
{
	unsigned int checkState = ((config.Flags & (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) == (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_FORCEWBUFFERING), BM_SETCHECK, checkState, 0);
}


INT_PTR CALLBACK ClassicConfigView::ClassicGlidePage::GlidePageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return UI::Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}


ClassicConfigView::ClassicGlidePage::ClassicGlidePage (ClassicConfigView* parent, unsigned int langIndex):
	Page		(parent->GetMainDialog (), langIndex),
	parent		(parent)
{
}


HWND			ClassicConfigView::ClassicGlidePage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::GlidePage, GlidePageProc);
}


void			ClassicConfigView::ClassicGlidePage::SetPageToConfig (HWND pageWnd)
{
	EnumerateResolutions (pageWnd, config.dispdev);
	AdapterHandler::DisplayMode mode = {config.dxres, config.dyres};
	adapterHandler.RefreshRefreshRateListByDisplayMode ((RendererAPI) config.rendererApi, config.dispdev,
														mode, (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16);
	EnumerateRefreshRates (pageWnd, config.dispdev);

	SetTexBitDepthBox (pageWnd);
	SetClosestRefRateBox (pageWnd);
	SetVSyncBox (pageWnd);

	SetLfbAccessComboBox (pageWnd);
	SetHwBuffCachingBox (pageWnd);
	SetFastWriteBox (pageWnd);
	SetTextureTilesBox (pageWnd);
	SetRealHwBox (pageWnd);
	
	SetCKComboBox (pageWnd);
	SetTexMemSizeComboBox (pageWnd);
	
	SetGammaControl (pageWnd);
	SetResolutionComboBox (pageWnd);
	SetRefreshFreqComboBox (pageWnd);

	SetMipMappingDisableBox (pageWnd);
	SetTrilinearMipMapsBox (pageWnd);
	SetAutogenerateMipMapsBox (pageWnd);
	SetBilinearFilteringBox (pageWnd);
	SetTexMemScaleBox (pageWnd);
	
	SetTripleBufferBox (pageWnd);
	SetTombRaiderBox (pageWnd);
	SetHwVBuffBox (pageWnd);
	SetGammaRampBox (pageWnd);
	SetTimerBoostBox (pageWnd);
	SetForceWBufferingBox (pageWnd);

	parent->SetPageState (Glide, pageWnd, parent->state);
	//parent->SetAllItemsStatus ();
}


void			ClassicConfigView::ClassicGlidePage::ConfigChanged ()
{
	parent->SetPageState (Glide, pageWnd[langIndex], parent->state);
	SetPageToConfig (pageWnd[langIndex]);
}


bool			ClassicConfigView::ClassicGlidePage::Init (HWND pageWnd)
{
	// Gamma slider
	SendMessage(GetDlgItem(pageWnd, IDC_SLIDER1), TBM_SETRANGE, FALSE, (LPARAM) MAKELONG(0, 400));
	SendMessage(GetDlgItem(pageWnd, IDC_SLIDER1), TBM_SETTICFREQ, 50, 0);
	SendMessage(GetDlgItem(pageWnd, IDC_SLIDER1), TBM_SETPOS, TRUE, 100);

	// Texture memory size combo
	for(unsigned int i = 0; i < 14; i++)	{
		SendMessage(GetDlgItem(pageWnd, IDC_COMBOTEXMEMSIZE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) texMemSizeStr[i]);
	}

	char	strBuffer[MAXSTRINGLENGTH];

	// Lfb access combo
	for (unsigned int i = 0; i < 4; i++)	{
		resourceManager.GetString (langIndex, lfbAccessStrIDs[i], strBuffer);
		SendMessage(GetDlgItem(pageWnd, IDC_COMBOLFBACCESS), CB_ADDSTRING,
					(WPARAM) 0, (LPARAM) strBuffer);
	}

	// Colorkey combo
	for (unsigned int i = 0; i < 4; i++)	{
		resourceManager.GetString (langIndex, ckMethodStrsIDs[i], strBuffer);
		SendMessage(GetDlgItem(pageWnd, IDC_COMBOCOLORKEY), CB_ADDSTRING,
					(WPARAM) 0, (LPARAM) strBuffer);
	}

	AddControlToToolTip (pageWnd, IDC_TEX16);
	AddControlToToolTip (pageWnd, IDC_TEX32);
	AddControlToToolTip (pageWnd, IDC_TEXOPT);
	AddControlToToolTip (pageWnd, IDC_CLOSESTFREQ);
	AddControlToToolTip (pageWnd, IDC_VSYNC);
	AddControlToToolTip (pageWnd, IDC_MIPMAPPINGDISABLE);
	AddControlToToolTip (pageWnd, IDC_TRILINEARMIPMAPS);
	AddControlToToolTip (pageWnd, IDC_AUTOGENMIPMAPS);
	AddControlToToolTip (pageWnd, IDC_BILINEARFILTERING);
	AddControlToToolTip (pageWnd, IDC_TEXMEMSCALE);
	AddControlToToolTip (pageWnd, IDC_COMBOLFBACCESS);
	AddControlToToolTip (pageWnd, IDC_HWBUFFCACHING);
	AddControlToToolTip (pageWnd, IDC_FASTWRITE);
	AddControlToToolTip (pageWnd, IDC_LFBTEXTURETILES);
	AddControlToToolTip (pageWnd, IDC_REALHW);
	AddControlToToolTip (pageWnd, IDC_COMBOCOLORKEY);
	AddControlToToolTip (pageWnd, IDC_COMBOTEXMEMSIZE);
	AddControlToToolTip (pageWnd, IDC_COMBORESOLUTION);
	AddControlToToolTip (pageWnd, IDC_SLIDER1);
	AddControlToToolTip (pageWnd, IDC_REFRESHRATEVALUE);
	AddControlToToolTip (pageWnd, IDC_CHECK1);
	AddControlToToolTip (pageWnd, IDC_TOMBRAIDER);
	AddControlToToolTip (pageWnd, IDC_HWVBUFFER);
	AddControlToToolTip (pageWnd, IDC_GAMMARAMP);
	AddControlToToolTip (pageWnd, IDC_TIMERLOOP);
	AddControlToToolTip (pageWnd, IDC_FORCEWBUFFERING);

	SetPageToConfig (pageWnd);
	return true;
}


bool			ClassicConfigView::ClassicGlidePage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_TEX16:				config.Flags &= ~CFG_TEXTURES32BIT; config.Flags |= CFG_TEXTURES16BIT; SetTexBitDepthBox (pageWnd); break;
		case IDC_TEX32:				config.Flags &= ~CFG_TEXTURES16BIT; config.Flags |= CFG_TEXTURES32BIT; SetTexBitDepthBox (pageWnd); break;
		case IDC_TEXOPT:			config.Flags &= ~(CFG_TEXTURES32BIT | CFG_TEXTURES16BIT); SetTexBitDepthBox (pageWnd); break;
		case IDC_CLOSESTFREQ:		config.Flags2 ^= CFG_CLOSESTREFRESHFREQ; SetClosestRefRateBox (pageWnd); break;
		case IDC_VSYNC:				config.Flags ^= CFG_VSYNC; SetVSyncBox (pageWnd); break;
		case IDC_HWBUFFCACHING:		config.Flags ^= CFG_HWLFBCACHING; SetHwBuffCachingBox (pageWnd); break;
		case IDC_FASTWRITE:			config.Flags ^= CFG_LFBFASTUNMATCHWRITE; SetFastWriteBox (pageWnd); break;
		case IDC_LFBTEXTURETILES:
			if (GetLfbTexureTilingMethod (config.Flags2) == CFG_LFBAUTOTEXTURING) {
				SetLfbTexureTilingMethod (config.Flags2, CFG_LFBDISABLEDTEXTURING);
			} else {
				SetLfbTexureTilingMethod (config.Flags2, CFG_LFBAUTOTEXTURING);
			}
			SetTextureTilesBox (pageWnd);
			break;
		case IDC_REALHW:			config.Flags ^= CFG_LFBREALVOODOO; SetRealHwBox (pageWnd); break;
		case IDC_MIPMAPPINGDISABLE:	config.Flags ^= CFG_DISABLEMIPMAPPING; SetMipMappingDisableBox (pageWnd); break;
		case IDC_TRILINEARMIPMAPS:	config.Flags2 ^= CFG_FORCETRILINEARMIPMAPS; SetTrilinearMipMapsBox (pageWnd); break;
		case IDC_AUTOGENMIPMAPS:	config.Flags2 ^= CFG_AUTOGENERATEMIPMAPS; SetAutogenerateMipMapsBox (pageWnd); break;
		case IDC_BILINEARFILTERING:	config.Flags ^= CFG_BILINEARFILTER; SetBilinearFilteringBox (pageWnd); break;
		case IDC_TEXMEMSCALE:		config.Flags2 ^= CFG_SCALETEXMEM; SetTexMemScaleBox (pageWnd); break;
		case IDC_TRIPLEBUFFER:		config.Flags ^= CFG_TRIPLEBUFFER; SetTripleBufferBox (pageWnd); break;
		case IDC_TOMBRAIDER:		config.Flags ^= CFG_TOMBRAIDER; SetTombRaiderBox (pageWnd); break;
		case IDC_HWVBUFFER:			config.Flags ^= CFG_ENABLEHWVBUFFER; SetHwVBuffBox (pageWnd); break;
		case IDC_GAMMARAMP:			config.Flags ^= CFG_ENABLEGAMMARAMP; SetGammaRampBox (pageWnd); break;
		case IDC_TIMERLOOP:
			config.dosTimerBoostPeriod = config.dosTimerBoostPeriod ? 0 : DEFAULT_DOS_TIMER_BOOST;
			SetTimerBoostBox (pageWnd);
			break;

		case IDC_FORCEWBUFFERING:
			if ((config.Flags & (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) == (CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER)) {
				config.Flags &= ~CFG_WBUFFERMETHODISFORCED;
				config.Flags |= CFG_RELYONDRIVERFLAGS;
			} else {
				config.Flags |= CFG_WBUFFERMETHODISFORCED | CFG_WBUFFER;
			}
			SetForceWBufferingBox (pageWnd);
			break;

		default:
			return false;
	}
	return true;
}


bool			ClassicConfigView::ClassicGlidePage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_COMBOLFBACCESS:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_COMBOLFBACCESS), CB_GETCURSEL, 0, 0);
				config.Flags2 &= ~(CFG_DISABLELFBREAD | CFG_DISABLELFBWRITE);
				if (i & 1) config.Flags2 |= CFG_DISABLELFBREAD;
				if (i & 2) config.Flags2 |= CFG_DISABLELFBWRITE;
				parent->SetAllItemsStatus ();
			}
			break;
		
		case IDC_COMBOCOLORKEY:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_COMBOCOLORKEY), CB_GETCURSEL, 0, 0);
				SetCKMethodValue(config.Flags, i);
			}
			break;

		case IDC_COMBOTEXMEMSIZE:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_COMBOTEXMEMSIZE), CB_GETCURSEL, 0, 0);
				if (i == CB_ERR) i = 0;
				config.TexMemSize = texMemSizes[i]*1024;
			}
			break;

		case IDC_COMBORESOLUTION:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_COMBORESOLUTION), CB_GETCURSEL, 0, 0);
				AdapterHandler::DisplayMode mode = {0, 0};
				if (i != 0) {
					mode = adapterHandler.GetResolution (i-1);
					config.dxres = mode.x;
					config.dyres = mode.y;
				} else {
					config.dxres = config.dyres = 0;
				}
				adapterHandler.RefreshRefreshRateListByDisplayMode ((RendererAPI) config.rendererApi, config.dispdev,
																	mode, (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16);
				EnumerateRefreshRates (pageWnd, config.dispdev);
				SetRefreshFreqComboBox (pageWnd);
			}
			break;

		case IDC_REFRESHRATEVALUE:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_REFRESHRATEVALUE), CB_GETCURSEL, 0, 0);
				unsigned int freq = 0;
				if (i != 0) {
					freq = adapterHandler.GetRefreshRate (i-1);
				}
				config.RefreshRate = freq;
			}
			break;

		default:
			return false;
	}
	return true;
}


void			ClassicConfigView::ClassicGlidePage::ApiChanged ()
{
	adapterHandler.RefreshResolutionListByAdapter ((RendererAPI) config.rendererApi, config.dispdev);
	AdapterHandler::DisplayMode mode = {config.dxres, config.dyres};
	adapterHandler.RefreshRefreshRateListByDisplayMode ((RendererAPI) config.rendererApi, config.dispdev,
														mode, (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16);
	EnumerateResolutions (pageWnd[langIndex], config.dispdev);
	SetResolutionComboBox (pageWnd[langIndex]);
	EnumerateRefreshRates (pageWnd[langIndex], config.dispdev);
	SetRefreshFreqComboBox (pageWnd[langIndex]);
}


void			ClassicConfigView::ClassicGlidePage::AdapterChanged ()
{
	adapterHandler.RefreshResolutionListByAdapter ((RendererAPI) config.rendererApi, config.dispdev);
	AdapterHandler::DisplayMode mode = {config.dxres, config.dyres};
	adapterHandler.RefreshRefreshRateListByDisplayMode ((RendererAPI) config.rendererApi, config.dispdev,
														mode, (config.Flags & CFG_DISPMODE32BIT) ? 32 : 16);
	EnumerateRefreshRates (pageWnd[langIndex], config.dispdev);
	SetResolutionComboBox (pageWnd[langIndex]);
	SetRefreshFreqComboBox (pageWnd[langIndex]);
}


bool			ClassicConfigView::ClassicGlidePage::SliderChanged (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_SLIDER1:
			config.Gamma = SendMessage (GetDlgItem (pageWnd, id), TBM_GETPOS, 0, 0);
			SetGammaControl (pageWnd);
			break;
		
		default:
			return false;
	}

	return true;
}
// ------------------------------------- VESA Page -----------------------------------------
char*			ClassicConfigView::ClassicVESAPage::vidMemSizeStr[] = { "256 kB", "512 kB", "1024 kB", "2048 kB", "3072 kB", "4096 kB", "5120 kB" };
unsigned int	ClassicConfigView::ClassicVESAPage::vidMemSizes[]	= {256, 512, 1024, 2048, 3072, 4096, 5120};

void	ClassicConfigView::ClassicVESAPage::SetVESAEnableBox (HWND pageWnd)
{
	unsigned int	j,v;

	if (config.Flags & CFG_VESAENABLE)	{
		v = BST_CHECKED;
		j = 1;
	} else {
		v = BST_UNCHECKED;
		j = 0;
	}
	SendMessage (GetDlgItem (pageWnd, IDC_VESAENABLE), BM_SETCHECK, v, 0);
	parent->SetAllItemsStatus ();
}


void	ClassicConfigView::ClassicVESAPage::SetRefreshFreq (HWND pageWnd)
{
	char	slider[8];
	
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDER2), TBM_SETPOS, (LPARAM) TRUE, (WPARAM) config.VideoRefreshFreq);
	sprintf (slider, "%dHz", config.VideoRefreshFreq);
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDERSTATIC2), WM_SETTEXT, (LPARAM) 0, (WPARAM) slider);
}


void	ClassicConfigView::ClassicVESAPage::SetEmulateVidMemSizeComboBox (HWND pageWnd)
{
	unsigned int i = config.VideoMemSize;
	unsigned int j = 0;
	for(; (i != vidMemSizes[j]) && (j < 7); j++);
	if (j >= 7) j = 0;
	SendMessage (GetDlgItem (pageWnd, IDC_COMBOVIDMEMSIZE), CB_SETCURSEL, j, 0);
}


void	ClassicConfigView::ClassicVESAPage::SetSupportMode13hBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags & CFG_SUPPORTMODE13H) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_SUPPORTMODE13H), BM_SETCHECK, checkState, 0);
}


void	ClassicConfigView::ClassicVESAPage::SetSupportDisableFlatLFBBox (HWND pageWnd)
{
	unsigned int checkState = (config.Flags2 & CFG_DISABLEVESAFLATLFB) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DISABLEFLATLFB), BM_SETCHECK, checkState, 0);
}


INT_PTR CALLBACK ClassicConfigView::ClassicVESAPage::VESAPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return UI::Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}


ClassicConfigView::ClassicVESAPage::ClassicVESAPage (ClassicConfigView* parent, unsigned int langIndex):
	Page		(parent->GetMainDialog (), langIndex),
	parent		(parent)
{
}


HWND			ClassicConfigView::ClassicVESAPage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::VESAPage, VESAPageProc);
}


void			ClassicConfigView::ClassicVESAPage::SetPageToConfig (HWND pageWnd)
{
	SetVESAEnableBox (pageWnd);
	SetRefreshFreq (pageWnd);
	SetEmulateVidMemSizeComboBox (pageWnd);
	SetSupportMode13hBox (pageWnd);
	SetSupportDisableFlatLFBBox (pageWnd);

	//parent->SetAllItemsStatus ();
	parent->SetPageState (VESA, pageWnd, parent->state);
}


void			ClassicConfigView::ClassicVESAPage::ConfigChanged ()
{
	parent->SetPageState (VESA, pageWnd[langIndex], parent->state);
	SetPageToConfig (pageWnd[langIndex]);
}


bool			ClassicConfigView::ClassicVESAPage::Init (HWND pageWnd)
{
	// Refresh freq slider
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDER2), TBM_SETRANGE, FALSE, (LPARAM) MAKELONG(45, 70));
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDER2), TBM_SETTICFREQ, 5, 0);
	SendMessage (GetDlgItem (pageWnd, IDC_SLIDER2), TBM_SETPOS, TRUE, 60);

	// Video memory size combo
	for (unsigned int i = 0; i < 7; i++)	{
		SendMessage (GetDlgItem (pageWnd, IDC_COMBOVIDMEMSIZE), CB_ADDSTRING, (WPARAM) 0, (LPARAM) vidMemSizeStr[i]);
	}

	AddControlToToolTip (pageWnd, IDC_VESAENABLE);
	AddControlToToolTip (pageWnd, IDC_SLIDER2);
	AddControlToToolTip (pageWnd, IDC_DISABLEFLATLFB);
	AddControlToToolTip (pageWnd, IDC_SUPPORTMODE13H);

	SetPageToConfig (pageWnd);
	return true;
}


bool			ClassicConfigView::ClassicVESAPage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_VESAENABLE:		config.Flags ^= CFG_VESAENABLE; SetVESAEnableBox (pageWnd); break;
		case IDC_SUPPORTMODE13H:	config.Flags ^= CFG_SUPPORTMODE13H; SetSupportMode13hBox (pageWnd); break;
		case IDC_DISABLEFLATLFB:	config.Flags2 ^= CFG_DISABLEVESAFLATLFB; SetSupportDisableFlatLFBBox (pageWnd); break;
		default:
			return false;
	}
	return true;
}


bool			ClassicConfigView::ClassicVESAPage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	switch(id)	{					
		case IDC_COMBOVIDMEMSIZE:
			{
				int i = SendMessage (GetDlgItem (pageWnd, IDC_COMBOVIDMEMSIZE), CB_GETCURSEL, 0, 0);
				if (i == CB_ERR) i = 0;
				config.VideoMemSize = vidMemSizes[i];
			}
			break;
		
		default:
			return false;
	}
	return true;
}


bool			ClassicConfigView::ClassicVESAPage::SliderChanged (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_SLIDER2:
			config.VideoRefreshFreq = SendMessage (GetDlgItem (pageWnd, id), TBM_GETPOS, 0, 0);
			SetRefreshFreq (pageWnd);
			break;
		
		default:
			return false;
	}

	return true;
}
// ----------------------------------- Renderer Page ---------------------------------------
INT_PTR CALLBACK ClassicConfigView::ClassicRendererPage::RendererPageProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return UI::Page::DefaultProc (hwndDlg, uMsg, wParam, lParam);
}

ClassicConfigView::ClassicRendererPage::ClassicRendererPage (ClassicConfigView* parent, unsigned int langIndex):
	Page	(parent->GetMainDialog (), langIndex),
	parent	(parent)
{
}


HWND			ClassicConfigView::ClassicRendererPage::GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd)
{
	return GetWindowFromResourceManager (langIndex, mainDlgWnd, toolTipWnd, ResourceManager::RendererSpecific, RendererPageProc);
}


void			ClassicConfigView::ClassicRendererPage::SetDX9LfbCopyModeBox (HWND pageWnd)
{
	unsigned int checkState = (config.dx9ConfigBits & CFG_DX9LFBCOPYMODE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DX9FORCELFCOPY), BM_SETCHECK, checkState, 0);
}


void			ClassicConfigView::ClassicRendererPage::SetDX9VerticalRetraceBox (HWND pageWnd)
{
	unsigned int checkState = (config.dx9ConfigBits & CFG_DX9NOVERTICALRETRACE) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DX9NORETRACE), BM_SETCHECK, checkState, 0);
}


void			ClassicConfigView::ClassicRendererPage::SetDX9FFShadersBox (HWND pageWnd)
{
	unsigned int checkState = (config.dx9ConfigBits & CFG_DX9FFSHADERS) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DX9FORCEFFSHADERS), BM_SETCHECK, checkState, 0);
	parent->SetAllItemsStatus ();
}


void			ClassicConfigView::ClassicRendererPage::SetDX9HwPalGenBox (HWND pageWnd)
{
	unsigned int checkState = (config.dx9ConfigBits & CFG_HWPALETTEGEN) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DX9_HWPALETTEGEN), BM_SETCHECK, checkState, 0);
}


void			ClassicConfigView::ClassicRendererPage::SetDX9BlackWhiteRenderingBox (HWND pageWnd)
{
	unsigned int checkState = (config.dx9ConfigBits & CFG_DX9GRAYSCALERENDERING) ? BST_CHECKED : BST_UNCHECKED;
	SendMessage (GetDlgItem (pageWnd, IDC_DX9GRAYSCALERENDERING), BM_SETCHECK, checkState, 0);
}


void			ClassicConfigView::ClassicRendererPage::SetPageToConfig (HWND pageWnd)
{
	SetDX9LfbCopyModeBox (pageWnd);
	SetDX9VerticalRetraceBox (pageWnd);
	SetDX9FFShadersBox (pageWnd);
	SetDX9HwPalGenBox (pageWnd);
	SetDX9BlackWhiteRenderingBox (pageWnd);
	parent->SetPageState (Renderer, pageWnd, parent->state);
}


void			ClassicConfigView::ClassicRendererPage::ConfigChanged ()
{
	parent->SetPageState (Renderer, pageWnd[langIndex], parent->state);
	SetPageToConfig (pageWnd[langIndex]);
}


bool			ClassicConfigView::ClassicRendererPage::Init (HWND pageWnd)
{
	AddControlToToolTip (pageWnd, IDC_DX9FORCELFCOPY);
	AddControlToToolTip (pageWnd, IDC_DX9NORETRACE);
	AddControlToToolTip (pageWnd, IDC_DX9FORCEFFSHADERS);
	AddControlToToolTip (pageWnd, IDC_DX9_HWPALETTEGEN);
	AddControlToToolTip (pageWnd, IDC_DX9GRAYSCALERENDERING);

	SetPageToConfig (pageWnd);
	return true;
}


bool			ClassicConfigView::ClassicRendererPage::ButtonClicked (HWND pageWnd, WORD id)
{
	switch (id) {
		case IDC_DX9FORCELFCOPY:
			config.dx9ConfigBits ^= CFG_DX9LFBCOPYMODE;
			SetDX9LfbCopyModeBox (pageWnd);
			break;
		
		case IDC_DX9NORETRACE:
			config.dx9ConfigBits ^= CFG_DX9NOVERTICALRETRACE;
			SetDX9VerticalRetraceBox (pageWnd);
			break;

		case IDC_DX9FORCEFFSHADERS:
			config.dx9ConfigBits ^= CFG_DX9FFSHADERS;
			SetDX9FFShadersBox (pageWnd);
			break;

		case IDC_DX9_HWPALETTEGEN:
			config.dx9ConfigBits ^= CFG_HWPALETTEGEN;
			SetDX9HwPalGenBox (pageWnd);
			break;

		case IDC_DX9GRAYSCALERENDERING:
			config.dx9ConfigBits ^= CFG_DX9GRAYSCALERENDERING;
			SetDX9BlackWhiteRenderingBox (pageWnd);
			break;

		default:
			return false;
	}
	return true;
}


bool			ClassicConfigView::ClassicRendererPage::ComboBoxSelChanged (HWND pageWnd, WORD id)
{
	return false;
}


void			ClassicConfigView::ConfigChanged ()
{
	if (platform == PDOS) {
		mainDialog.ShowPage (2);
	} else {
		mainDialog.HidePage (2);
	}
	if (classicGlobalPage != NULL) {
		classicGlobalPage->ConfigChanged ();
	}
	if (classicGlidePage != NULL) {
		classicGlidePage->ConfigChanged ();
	}
	if (classicVESAPage != NULL) {
		classicVESAPage->ConfigChanged ();
	}
	if (classicRendererPage != NULL) {
		classicRendererPage->ConfigChanged ();
	}
}


void			ClassicConfigView::ThemeChanged ()
{
	if (classicGlobalPage != NULL) {
		classicGlobalPage->ThemeChanged ();
	}
	if (classicGlidePage != NULL) {
		classicGlidePage->ThemeChanged ();
	}
	if (classicVESAPage != NULL) {
		classicVESAPage->ThemeChanged ();
	}
	if (classicRendererPage != NULL) {
		classicRendererPage->ThemeChanged ();
	}
}


void			ClassicConfigView::ApiChanged ()
{
	if (classicGlidePage != NULL) {
		classicGlidePage->ApiChanged ();
	}
}


void			ClassicConfigView::AdapterChanged ()
{
	if (classicGlidePage != NULL) {
		classicGlidePage->AdapterChanged ();
	}
}

}	// namespace CV