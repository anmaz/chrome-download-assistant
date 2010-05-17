/* ***** BEGIN LICENSE BLOCK *****
* Copyright 2010 Chen Bo (chen_bo-bj@vanceinfo.com)
* Version: 0.5
* This code was based on the npdownloadhelper.cpp sample code in Gecko-sdk.
*
* The contents of this file are subject to the Mozilla Public License Version
* 1.1 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
* ***** END LICENSE BLOCK ***** */

#include "../utils.h.h"
static TCHAR* utf8ToUnicode(char *utf8)
{
	int   length = MultiByteToWideChar(CP_UTF8,0,utf8,-1,NULL,0); 
	TCHAR *unicode = new TCHAR[length]; 
	MultiByteToWideChar(CP_UTF8,0,utf8,-1,unicode,length); 
	return unicode;
}