/*--------------------------------------------------------------------------------- */
/* dgVoodooSetup.h - dgVoodooSetup main setup header                                */
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
/* dgVoodoo: dgVoodooSetup.h																*/
/*			 Main Setup	header																*/
/*------------------------------------------------------------------------------------------*/
#define	_CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <commctrl.h>
#include "dgVoodooConfig.h"
#include "ConfigTree.h"
#include "AdapterHandler.hpp"

//	Config platform defs
#define	PWINDOWS				0
#define PDOS					1
#define PUNKNOWN				0xFF


#ifdef __cplusplus
extern "C" {
#endif
	
extern	int				configvalid;
extern	unsigned int	lang;
extern	int				cmdLineMode;
extern	unsigned int	platform;

extern	HINSTANCE		hInstance;

extern	HWND			hDlgMain;
extern	HWND			hActPage;


#ifdef __cplusplus
}
#endif

void	SetConfig (const dgVoodooConfig& config);
void	Exit (bool writeCurrentConfig);

extern	dgVoodooConfig	config;
extern	dgVoodooConfig	defaultConfigs[2];


class	AdapterHandler:		public IAdapterHandler::IResolutionEnumCallback,
							public IAdapterHandler::IRefreshRateEnumCallback
{
public:
	class DisplayMode {
	public:
		unsigned short x;
		unsigned short y;
	};

private:
	RendererAPI			currentAPI;
	IAdapterHandler*	iHandler;
	DisplayMode			displayModes[48];
	unsigned int		displayModesNum;
	unsigned int		refreshRates[16];
	unsigned int		refreshRatesNum;

private:
			bool			SwitchToAPI (RendererAPI api);
			void			ShutDownCurrentAPI ();
	virtual void			AddResolution (unsigned int xRes, unsigned int yRes);
	virtual void			AddRefreshRate (unsigned int freq);

public:
			AdapterHandler ();

			const char*		GetApiName (RendererAPI api);

			bool			IsAPIAvailable (RendererAPI api);
			unsigned int	GetNumOfAdapters (RendererAPI api);
			const char*		GetDescriptionOfAdapter (RendererAPI api, unsigned int index);
			unsigned int	GetNumOfDeviceTypes (RendererAPI api, unsigned int adapter);
			const char*		GetDescriptionOfDeviceType (RendererAPI api, unsigned int adapter, unsigned int index);
			
			void			RefreshResolutionListByAdapter (RendererAPI api, unsigned int adapter);
			unsigned int	GetNumOfResolutions ();
			DisplayMode		GetResolution (unsigned int index);

			void			RefreshRefreshRateListByDisplayMode (RendererAPI api, unsigned int adapter,
																 const DisplayMode& mode, unsigned int bitDepth);
			unsigned int	GetNumOfRefreshRates ();
			unsigned int	GetRefreshRate (unsigned int index);

			bool			GetCurrentDisplayMode (DisplayMode& resolution, unsigned int& bitDepth, unsigned int& refRate);

			void			ShutDown ();
};


extern	AdapterHandler	adapterHandler;


class	WrapperInstances
{
public:
	struct InstanceInfo	{
		char *name;
		char *remark;
	};

	struct WrapperInstance	{
		char	filePath[1024];
	};

	enum	ConfigStatus	{
		ConfigOK	=	0,
		ConfigError,
		ConfigInvalidVersion,
		FileNotFound
	};

private:
	static	InstanceInfo	instanceInfo[];
	unsigned int			numOfInstances;
	WrapperInstance			wrapperInstances[32];

	static ConfigStatus		GetConfigAndStatus (const char* path, bool dllFile, dgVoodooConfig (*configs)[2]);
	static bool				WriteConfig (const char* path, bool dllFile, const dgVoodooConfig (&configs)[2]);

public:
	static ConfigStatus		GetConfigAndStatus (const char* path, dgVoodooConfig (*configs)[2] = NULL);
	static ConfigStatus		GetExternalConfigAndStatus (const char* path, dgVoodooConfig (*configs)[2] = NULL);
	static bool				WriteExternalConfig (const char* path, const dgVoodooConfig (&configs)[2]);
	
	ConfigStatus			ReadConfig (unsigned int instance, dgVoodooConfig (&configs)[2]);
	bool					WriteConfig (unsigned int instance, const dgVoodooConfig (&configs)[2]);
	
	bool					SearchWrapperInstances (const char *bufferm, unsigned int glideVersion = 0);
	bool					IsInList (const char* path) const;
	void					AddInstance (const WrapperInstance& instance);

	void					Clear ();

	unsigned int			GetNumOfInstances () const;
	WrapperInstance			GetInstance (unsigned int index);

	WrapperInstances ();
	~WrapperInstances ();
};

	

namespace	UI	{

typedef	void (_stdcall *SetThemeAppPropertiesProc)(DWORD);

class	MainDialog	{
public:
	enum	ConfigState	{
		Invalid		=	0,
		Valid
	};

private:
	bool						pageVisible[8];
	HMENU						hFloatingMenus[16];
	unsigned int				langIndex;
	HINSTANCE					uxThemeHandle;
	SetThemeAppPropertiesProc	pSetThemeAppProperties;

	unsigned int	GetTabIndex (unsigned int pageIndex);
	unsigned int	GetPageIndex (unsigned int tabIndex);

	void			FillGetOpenFileStruc (OPENFILENAME&	fn, const char* filter1, const char* filter2);

	void			DisplayChanged ();
	void			SetEnableTheming ();

	bool			IsMenuItemChecked (unsigned int id);
	bool			ToggleMenuItemCheck (unsigned int id);
	bool			SetMenuItemStatus (unsigned int id, bool enabled);
	void			HandleFloatingMenus (WPARAM wParam, LPARAM lParam);
	static	int CALLBACK	DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	MainDialog ();
	~MainDialog ();
	
	void			SetLanguage (unsigned int langIndex);
	void			SetConfigState (ConfigState state);

	void			ShowPage (unsigned int pageIndex);
	void			HidePage (unsigned int pageIndex);
	void			SelectPage (unsigned int tabIndex);
	unsigned int	GetSelectedPage () const;

	void			LanguageChanged (unsigned int newLangIndex);

	void			Invoke ();
};


class	Page
{
	protected:
		MainDialog&		mainDialog;
		unsigned int	langIndex;
		HWND			pageWnd[MAXLANGNUM];
		HWND			toolTipWnd;
		TOOLINFO		toolInfo;

	protected:
		static INT_PTR	DefaultProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		INT_PTR			HandleMsg (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void			AddControlToToolTip (HWND pageHwnd, int ctlId);

		HWND			GetWindowFromResourceManager (unsigned int langIndex, HWND mainDlgWnd, HWND toolTipWnd, ResourceManager::UIPages page, DLGPROC lpDialogFunc);
		unsigned int	GetLangIndexFromWindow (HWND hWnd);
		virtual void	SetPageToConfig (HWND pageWnd) = 0;

		virtual bool	Init (HWND pageWnd) = 0;
		virtual bool	ButtonClicked (HWND pageWnd, WORD id);
		virtual bool	ComboBoxSelChanged (HWND pageWnd, WORD id);
		virtual bool	SliderChanged (HWND pageWnd, WORD id);

	public:
		Page (MainDialog& mainDialog, unsigned int langIndex = 0);
		~Page ();

				void	SetLanguage (unsigned int langIndex);
		virtual HWND	GetPageWindow (HWND mainDlgWnd, HWND toolTipWnd) = 0;

		HWND			GetWindowFromItem (unsigned int item);

		void			ThemeChanged ();
		virtual void	DisplayChanged ();
};


}	// namespace UI


namespace	CV	{

class	ConfigView
{
public:
	enum	ConfigState	{
		Invalid		=	0,
		Valid
	};

protected:

	unsigned int		langIndex;
	ConfigState			state;
	UI::MainDialog&		mainDialog;

public:
	ConfigView (UI::MainDialog& dialog, unsigned int langIndex);
	~ConfigView ();

	virtual void			Init () = 0;

	virtual void			SetLanguage (unsigned int langIndex) = 0;
	virtual void			SetConfigState (ConfigState state) = 0;

	virtual unsigned int	GetPageNum () = 0;
	virtual void			GetPageName (unsigned int pageIndex, char* name) = 0;
	virtual HWND			GetPageWindow (unsigned int pageIndex, HWND mainDlgWnd, HWND toolTipWnd) = 0;
	virtual void			ThemeChanged () = 0;

	virtual unsigned int	GetCustomMenuItemsNum () = 0;
	virtual unsigned int	GetCustomMenuItem (unsigned int index, char* text) = 0;
	virtual bool			IsCustomMenuItemCheckable (unsigned int index) = 0;
	virtual bool			HandleMenuItem (unsigned int id, bool checked) = 0;
	
	virtual void			ConfigChanged () = 0;
};


class	ConfigViewManager
{
public:
	enum Views	{
		TopBottomPages	=	0,

		QuickView,
		ClassicView,
		TweakerView,

		NumOfViews
	};

private:
	ConfigView*		views[NumOfViews];

public:
	//ConfigViewManager ();
	
	ConfigView*		GetView (UI::MainDialog* dialog, Views view, unsigned int langIndex);
};

extern	ConfigViewManager	configViewManager;

}	// namespace CV

namespace	UI	{

extern	UI::Page*		CreateTopPage (MainDialog& dialog);
extern	UI::Page*		CreateBottomPage (MainDialog& dialog);

extern	bool			FlushCurrentConfig (UI::Page* topPage);

}

namespace	CV	{

extern	CV::ConfigView*	CreateClassicConfigView (UI::MainDialog& dialog, unsigned int langIndex);

}

