/*--------------------------------------------------------------------------------- */
/* ConfigTree.cpp - dgVoodooSetup half-ready, ditched, unused config something with */
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


#include <string.h>
#include "Dgw.h"
#include "dgVoodooConfig.h"
#include "ConfigTree.h"

#include "Resource.h"
#include "ResourceFix.h"

static	WORD	pageResIDs[ResourceManager::NumOfUIPages] = {IDD_DIALOGABOUT, IDD_DIALOGTOP, IDD_DIALOGBOTTOM, 
														   IDD_DIALOGGLOBAL, IDD_DIALOGGLIDE, IDD_DIALOGVESA,
														   IDD_DIALOGRENDERER};

static	char*	engLangName = "English";
static	char*	hunLangName = "Magyar";

ResourceManager	resourceManager;

ResourceManager::LangUIData::LangUIData ():
	stringTable (NULL)
{
	SetLanguageName (engLangName);
	for (unsigned int i = 0; i < NumOfUIPages; i++) {
		uiPages[i].resId = pageResIDs[i];
		uiPages[i].resData = NULL;
		uiPages[i].pageWindow = NULL;
	}
}


ResourceManager::LangUIData::~LangUIData ()
{
}


ResourceManager::LangUIData::PageUIData	ResourceManager::LangUIData::GetPageData (UIPages page)
{
	return uiPages[page];
}


void*	ResourceManager::LangUIData::GetStringTableData ()
{
	return stringTable;
}


void	ResourceManager::LangUIData::SetLanguageName (char* langName)
{
	strcpy (this->langName, langName);
}


void	ResourceManager::LangUIData::SetPageResourceData (UIPages page, void* resData)
{
	uiPages[page].resData = resData;
}


void	ResourceManager::LangUIData::SetPageWindow (UIPages page, HWND window)
{
	uiPages[page].pageWindow = window;
}


void	ResourceManager::LangUIData::SetStringTableResourceData (void* stringTableParam)
{
	stringTable = stringTableParam;
}


ResourceManager::ResourceManager ():
	numOfExternRes (0)
{
	langUIData = new LangUIData[MAXLANGNUM];
}


ResourceManager::~ResourceManager ()
{
	delete[] langUIData;
}


void*	ResourceManager::FindResourceData (ResType resType, unsigned short resName, void* rawResourceData)
{
	ResourceHeader* resPtr = (ResourceHeader*) rawResourceData;

	DWORD ordinalInBundle = 0;
	if (resType == String)	{
		ordinalInBundle = resName & 0xF;
		resName = (resName >> 4) + 1;
	}

	while (!(resPtr->resType[0] == 0xFFFF && resPtr->resType[1] == resType &&
		   resPtr->resName[0] == 0xFFFF && resPtr->resName[1] == resName))	{
		if (resPtr->headerSize != 0) {
			resPtr = (ResourceHeader*) (((char*) resPtr) + (((resPtr->headerSize + resPtr->dataSize) + 0x3) & 0xFFFFFFFC));
		} else {
			return NULL;
		}
	}

	char* rawData = ((char*) resPtr) + resPtr->headerSize;
	if (resType == String)	{
		for (unsigned int i = 0; i < ordinalInBundle; i++) {
			rawData += 2*(*((WORD*) rawData)) + 2;
		}
	}

	return rawData;
}


void*	ResourceManager::FindResourceData (unsigned int langIndex, ResType resType, WORD resName)
{
	if (langIndex != 0) {
		return FindResourceData (resType, resName, externRes[langIndex-1].resLoc);
	}
	return NULL;
}

void	ResourceManager::CreateDefaultUI ()
{
	for (unsigned int j = 0; j < numOfExternRes; j++) {
		LangUIData uiData;

		uiData.SetLanguageName (hunLangName);
		for (unsigned int i = 0; i < NumOfUIPages; i++) {
			uiData.SetPageResourceData ((UIPages) i, FindResourceData (Dialog, pageResIDs[i], externRes[j].resLoc));
			uiData.SetStringTableResourceData (externRes[j].resLoc);
		}

		langUIData[j+1] = uiData;
	}
}


bool	ResourceManager::Init ()
{
	unsigned int externResId = IDR_LOC_HUN;

	for (;; externResId++) {
		HRSRC resLocResHandle = FindResource (NULL, (LPCSTR) externResId, (LPCSTR) 201);
		if (resLocResHandle != NULL) {
			HGLOBAL resLocHandle = LoadResource (NULL, resLocResHandle);
			if (resLocHandle != NULL) {
				void* resLoc = LockResource (resLocHandle);
				if (resLoc != NULL) {
					externRes[numOfExternRes].resLocResHandle = resLocResHandle;
					externRes[numOfExternRes].resLocHandle = resLocHandle;
					externRes[numOfExternRes].resLoc = resLoc;

					char* str = (char*) FindResourceData (String, 1, resLoc);
					if (str != NULL) {
						int size = WideCharToMultiByte (CP_ACP, 0, ((WCHAR*) str)+1, *((SHORT*) str), NULL, 0, NULL, NULL);
						WideCharToMultiByte (CP_ACP, 0, ((WCHAR*) str)+1, *((SHORT*) str), externRes[numOfExternRes].langName, size, NULL, NULL);
					} else {
						strcpy (externRes[numOfExternRes].langName, "(Unknown)");
					}
					

					numOfExternRes++;
					continue;
				}

			}
			DeleteObject ((HGDIOBJ) resLocResHandle);
		} else {
			break;
		}
	}

	CreateDefaultUI ();
	return true;
}


void	ResourceManager::Exit ()
{
	for (unsigned int i = 0; i < numOfExternRes; i++) {
		UnlockResource (externRes[i].resLocHandle);
		DeleteObject ((HGDIOBJ) externRes[i].resLocResHandle);
	}
}


unsigned int	ResourceManager::GetNumOfExternalResources () const
{
	return numOfExternRes;
}


void			ResourceManager::GetLangNameOfExternalResource (unsigned int index, char* outPut) const
{
	outPut[0] = 0x0;
	if (index < numOfExternRes) {
		strcpy (outPut, externRes[index].langName);
	}
}


HWND	ResourceManager::GetUIPageWindow (unsigned int langIndex, UIPages page, HWND parentWnd, DLGPROC lpDialogFunc,
										  void* userData /*= NULL*/)
{
	LangUIData::PageUIData pageData = langUIData[langIndex].GetPageData (page);
	if (pageData.pageWindow == NULL)	{
		if (pageData.resData != NULL) {
			// Custom localized resource
			pageData.pageWindow = CreateDialogIndirectParam (GetModuleHandle (NULL), (LPDLGTEMPLATE) pageData.resData,
															 parentWnd, lpDialogFunc, (LPARAM) userData);
		} else {
			// Builtin English resource
			pageData.pageWindow = CreateDialogParam (GetModuleHandle (NULL), MAKEINTRESOURCE (pageData.resId),
													 parentWnd, lpDialogFunc, (LPARAM) userData);
		}
		ShowWindow (pageData.pageWindow, SW_HIDE);
		langUIData[langIndex].SetPageWindow (page, pageData.pageWindow);
	}
	return pageData.pageWindow;
}


bool	ResourceManager::GetString (unsigned int langIndex, WORD resId, char* outPut)
{
	if (langUIData[langIndex].GetStringTableData () != NULL) {
		char* str = (char*) FindResourceData (String, resId, langUIData[langIndex].GetStringTableData ());
		if (str != NULL) {
			int size = WideCharToMultiByte (CP_ACP, 0, ((WCHAR*) str)+1, *((SHORT*) str), NULL, 0, NULL, NULL);
			WideCharToMultiByte (CP_ACP, 0, ((WCHAR*) str)+1, *((SHORT*) str), outPut, size, NULL, NULL);
			outPut[size] = 0x0;
			return true;
		}
	} else {
		return (LoadString (GetModuleHandle (NULL), resId, outPut, MAXSTRINGLENGTH) != 0);
	}

	return false;
}


HMENU			ResourceManager::GetMenu (unsigned int langIndex, WORD resId)
{
	if (langUIData[langIndex].GetStringTableData () != NULL) {
		MENUTEMPLATE* menuTemplate = (MENUTEMPLATE*) FindResourceData (Menu, resId, externRes[langIndex-1].resLoc);
		if (menuTemplate != NULL) {
			return LoadMenuIndirect (menuTemplate);
		}
	} else {
		return LoadMenu (GetModuleHandle (NULL), MAKEINTRESOURCE(resId));
	}
	return NULL;
}


unsigned int	ResourceManager::GetLangIndexFromWindow (HWND hWnd)
{
	for (int j = 0; j < MAXLANGNUM; j++) {
		for (int i = 0; i < NumOfUIPages; i++)	{
			if (langUIData[j].GetPageData ((UIPages) i).pageWindow == hWnd) {
				return j;
			}
		}
	}

	return 0;
}


bool			XMLMemoryReader::IsEndOfXML (const char* xmlPtr)
{
	return xmlPtr[0] == 0x0;
}

void			XMLMemoryReader::SkipWhiteSpaces (const char** xmlPtr)
{
	while ((*xmlPtr)[0] == ' ' || (*xmlPtr)[0] == '\0xD' || (*xmlPtr)[0] == '\0xA') {
		(*xmlPtr)++;
	}
}


bool			XMLMemoryReader::IsOnNode (const char* xmlPtr)
{
	return xmlPtr[0] == '<';
}


bool			XMLMemoryReader::IsOnNodeClose (const char* xmlPtr)
{
	return xmlPtr[0] == '<' && xmlPtr[1] == '/';
}


unsigned int	XMLMemoryReader::GetNodeName (const char* xmlPtr, char* copyTo, bool closingTag)
{
	unsigned int length = 0;
	unsigned int offset = closingTag ? 2 : 1;
	
	if (IsOnNode (xmlPtr)) {
		const char* tempPtr = xmlPtr + (closingTag ? 2 : 1);
		SkipWhiteSpaces (&tempPtr);
		while (tempPtr[0] != ' ' && tempPtr[0] != '>' && tempPtr[0] != '\0xD' && tempPtr[0] != '\0xA') {
			if (tempPtr[0] != '<' && xmlPtr[0] != 0x0) {
				*(copyTo++) = *(tempPtr++);
				length++;
			} else {
				return 0;			// syntax error
			}
		}
		*copyTo = 0x0;
	}

	return length;
}


unsigned int	XMLMemoryReader::GetValue (const char* xmlPtr, char* copyTo)
{
	unsigned int length = 0;

	if (!IsOnNode (xmlPtr)) {
		while (xmlPtr[0] != ' ' && xmlPtr[0] != '<' && xmlPtr[0] != '\0xD' && xmlPtr[0] != '\0xA') {
			if (xmlPtr[0] != '>' && xmlPtr[0] != 0x0) {
				*(copyTo++) = *(xmlPtr++);
				length++;
			} else {
				return 0;			// syntax error
			}
		}
		*copyTo = 0x0;
	}

	return  length;
}


bool	XMLMemoryReader::SkipNodeNameTag (const char** xmlPtr)
{
	while ((++(*xmlPtr))[0] != '>') {
		if ((*xmlPtr)[0] == '<' || (*xmlPtr)[0] == 0x0) {
			return false;			// syntax error
		}
	}
	(*xmlPtr)++;
	return true;
}


bool	XMLMemoryReader::SkipValue (const char** xmlPtr)
{
	SkipWhiteSpaces (xmlPtr);
	while ((++(*xmlPtr))[0] != '<') {
		if ((*xmlPtr)[0] == 0x0) {
			return false;			// syntax error
		}
	}
	return true;
}


bool	XMLMemoryReader::ReadSubTree (const char** xmlPtr, TreeElem* parentNode, char* strings)
{
	TreeElem* currentNode = parentNode+1;

	bool succeeded = true;

	for (; succeeded && !IsEndOfXML (*xmlPtr);) {
		SkipWhiteSpaces (xmlPtr);
		if (IsOnNodeClose (*xmlPtr)) {
			break;
		} else if (IsOnNode (*xmlPtr)) {
			unsigned int length = GetNodeName (*xmlPtr, strings);
			(*xmlPtr)++;
			if (SkipNodeNameTag (xmlPtr)) {
				SkipWhiteSpaces (xmlPtr);
				if (IsOnNode (*xmlPtr)) {
					if (!IsOnNodeClose (*xmlPtr)) {
						CreateTreeElem (strings, NULL, currentNode);
						parentNode->subElemNum++;
						if (ReadSubTree (xmlPtr, currentNode, strings+length+1)) {
							SkipWhiteSpaces (xmlPtr);
							if (IsOnNodeClose (*xmlPtr)) {
								char name[4096];
								GetNodeName (*xmlPtr, name, true);
								if (strcmp (name, strings) == 0) {
									SkipNodeNameTag (xmlPtr);
									continue;
								}
							}
						}
					}
				} else {
					unsigned int valueLength = GetValue (*xmlPtr, strings+length+1);
					if (valueLength > 0) {
						CreateTreeElem (strings, strings+length+1, currentNode);
						parentNode->subElemNum++;
						SkipValue (xmlPtr);
						SkipWhiteSpaces (xmlPtr);
						if (IsOnNodeClose (*xmlPtr)) {
							char name[4096];
							GetNodeName (*xmlPtr, name, true);
							if (strcmp (name, strings) == 0) {
								SkipNodeNameTag (xmlPtr);
								continue;
							}
						}
					}
				}
			}
		}
		succeeded = false;
	}
	return succeeded;
}


void	XMLMemoryReader::CreateTreeElem (const char* elemName, const char* elemValue, TreeElem* to)
{
	to->name = elemName;
	to->value = elemValue;
	to->subElemNum = 0;
}


bool	XMLMemoryReader::Read (const char* xmlInMem, TreeElem* nodes, char* strings)
{
	SkipWhiteSpaces (&xmlInMem);

	CreateTreeElem ("dgVoodooRootNode", NULL, nodes);

	return ReadSubTree (&xmlInMem, nodes, strings);
}

extern "C" {
void ReadXML ();
}

void ReadXML ()
{
	char xml[] = {"<dgVoodoo>  <Settings><  Something>4 </Something></Settings></dgVoodoo>"};

	TreeElem* nodes = (TreeElem*) _GetMem (1024 * sizeof (TreeElem));
	char* strings = (char*) _GetMem (8192);

	XMLMemoryReader::Read (xml, nodes, strings);
}