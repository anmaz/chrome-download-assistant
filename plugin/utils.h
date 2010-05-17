/* ***** BEGIN LICENSE BLOCK *****
* Version: MPL 1.1/GPL 2.0/LGPL 2.1
* This code was based on the npsimple.c sample code in Gecko-sdk.
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
*
* Contributor(s):
*   Bo Chen   <chen_bo-bj@vanceinfo.com>
*   Jing Zhao <jingzhao@google.com>
*
* Alternatively, the contents of this file may be used under the terms of
* either the GNU General Public License Version 2 or later (the "GPL"), or 
* the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
* in which case the provisions of the GPL or the LGPL are applicable instead
* of those above. If you wish to allow use of your version of this file only
* under the terms of either the GPL or the LGPL, and not to allow others to
* use your version of this file under the terms of the NPL, indicate your
* decision by deleting the provisions above and replace them with the notice
* and other provisions required by the GPL or the LGPL. If you do not delete
* the provisions above, a recipient may use your version of this file under
* the terms of any one of the NPL, the GPL or the LGPL.
* ***** END LICENSE BLOCK ***** */

#include <windows.h>

class Utils {
public:
  static TCHAR* Utf8ToUnicode(char *utf8) {
    int length = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0); 
    TCHAR *unicode = new TCHAR[length];
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, length); 
    return unicode;
  }
  static SAFEARRAY* CreateArray(uint32_t length) {
    SAFEARRAYBOUND rgsabound[1];    
    rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = length;
    SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);
    return psa;
  }
  static VARIANT ToVariant(SAFEARRAY* psa) {
    VARIANT vList;
    vList.vt = VT_VARIANT | VT_ARRAY | VT_BYREF;
    vList.pparray = &psa;
    return vList;
  }
  static HRESULT GetCom(REFCLSID rclsid, REFIID riid, __deref_out LPVOID* ppv) {
    return CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, riid, ppv); 
  }
};