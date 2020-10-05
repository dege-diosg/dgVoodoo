/*--------------------------------------------------------------------------------- */
/* ConfigTree.h - dgVoodooSetup half-ready unused config something with             */
/*                  UI tabpage resource handler                                     */
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


#ifndef	CONFIGTREE_H
#define	CONFIGTREE_H

#include <windows.h>

#define	MAXSTRINGLENGTH			1024

class	ResourceManager
{
private:
	struct	ResourceHeader	{
		DWORD	dataSize;
		DWORD	headerSize;
		WORD	resType[2];			// Our raw resource data contains only ordinal resource type
		WORD	resName[2];			// Our raw resource data contains only ordinal resource name
	};

public:
	enum	ResType	{
		Menu		=	4,
		Dialog		=	5,
		String		=	6
	};

	enum	UIPages	{
		AboutDialog		=	0,

		HeaderPage,
		BottomPage,

		// Classic style pages
		GlobalPage,
		GlidePage,
		VESAPage,
		RendererSpecific,

		NumOfUIPages
	};

public:
	class	LangUIData	{
		public:
			struct PageUIData {
				DWORD		resId;
				void*		resData;
				HWND		pageWindow;
			};

		private:
			PageUIData		uiPages[NumOfUIPages];		// UI page dialog template resources
			void*			stringTable;				// String table resources
			char			langName[256];

		public:
			LangUIData ();
			~LangUIData ();

			void		SetLanguageName (char* langName);
			void		SetPageResourceData (UIPages page, void* resData);
			void		SetPageWindow (UIPages page, HWND window);
			void		SetStringTableResourceData (void* stringTable);

			PageUIData	GetPageData (UIPages page);
			void*		GetStringTableData ();
	};

private:
	LangUIData*		langUIData;

	struct	ExternResFile	{
		HRSRC			resLocResHandle;
		HGLOBAL			resLocHandle;
		void*			resLoc;
		char			langName[128];
	} externRes[32];
	
	unsigned int	numOfExternRes;


private:
	void			CreateDefaultUI ();
	void*			FindResourceData (ResType resType, WORD resName, void* rawResourceData);

public:

	ResourceManager ();
	~ResourceManager ();

	bool			Init ();
	void			Exit ();

	void*			FindResourceData (unsigned int langIndex, ResType resType, WORD resName);
	
	unsigned int	GetNumOfExternalResources () const;
	void			GetLangNameOfExternalResource (unsigned int index, char* outPut) const;
	bool			GetString (unsigned int langIndex, WORD resId, char* outPut);
	HMENU			GetMenu (unsigned int langIndex, WORD resId);

	HWND			GetUIPageWindow (unsigned int langIndex, UIPages page, HWND parentWnd, DLGPROC lpDialogFunc, void* userData = NULL);

	unsigned int	GetLangIndexFromWindow (HWND hWnd);

};


extern	ResourceManager	resourceManager;


class TreeElem
{
public:
	const char*		name;
	const char*		value;
	unsigned int	subElemNum;
};


class XMLMemoryReader
{
private:
	static bool			IsEndOfXML (const char* xmlPtr);
	static void			SkipWhiteSpaces (const char** xmlPtr);
	static bool			IsOnNode (const char* xmlPtr);
	static bool			IsOnNodeClose (const char* xmlPtr);
	static unsigned int	GetNodeName (const char* xmlPtr, char* copyTo, bool closingTag = false);
	static unsigned int	GetValue (const char* xmlPtr, char* copyTo);
	static bool			SkipNodeNameTag (const char** xmlPtr);
	static bool			SkipValue (const char** xmlPtr);

	static bool			ReadSubTree (const char** xmlPtr, TreeElem* parentNode, char* strings);

	static void			CreateTreeElem (const char* elemName, const char* elemValue, TreeElem* to);

public:
	static bool			Read (const char* xmlInMem, TreeElem* nodes, char* strings);
};

#endif