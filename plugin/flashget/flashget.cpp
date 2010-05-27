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

#include "flashget.h"

#include <afxdisp.h>
#include <atlbase.h>
#include <atlcom.h>
#include <comutil.h>
#include "../utils.h"

using namespace std;

FlashgetSupport::FlashgetSupport() {
}

FlashgetSupport::~FlashgetSupport() {
}

std::wstring FlashgetSupport::GetProgID(int version) {
  if (version == 1) {
    return L"JetCar.Netscape";
  } else {
    return L"FG2CatchUrl.Netscape";
  }
}

// Check if Flashget is enabled.
void FlashgetSupport::IsEnabled(NPObject* obj, const NPVariant* args,
                             uint32_t argCount, NPVariant* result) {
  result->type = NPVariantType_Bool;
  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL)) &&
      FAILED(dispatch.CoCreateInstance(GetProgID(1).c_str(), NULL)))
    result->value.intValue = FALSE;
  result->value.intValue = TRUE;
}

// Add a link to download in Flashget.
void FlashgetSupport::AddLink(NPObject* obj, const NPVariant* args,
                           uint32_t argCount, NPVariant* result) {
  if (argCount != 3 || !NPVARIANT_IS_STRING(args[0]) ||
      !NPVARIANT_IS_STRING(args[1]) || !NPVARIANT_IS_STRING(args[2]))
    return;

  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL)) &&
      FAILED(dispatch.CoCreateInstance(GetProgID(1).c_str(), NULL)))
    return;

  OLECHAR* name = L"AddUrl";
  DISPID dispid;
  if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return;

  _bstr_t referrer = NPVARIANT_TO_STRING(args[2]).UTF8Characters;
  _bstr_t comments = Utils::Utf8ToUnicode((char*)
      NPVARIANT_TO_STRING(args[1]).UTF8Characters);
  _bstr_t url = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

  VARIANT v[3];
  v[0].vt = v[1].vt = v[2].vt = VT_BSTR;
  v[2].bstrVal = url;
  v[1].bstrVal = comments;
  v[0].bstrVal = referrer;

  if (FAILED(dispatch.InvokeN(dispid, v, 3, NULL)))
    return;
}

// Download all links with Flashget.
void FlashgetSupport::DownloadAll(NPObject* obj, const NPVariant* args,
                               uint32_t argCount, NPVariant* result) {
  if (argCount == 0 || !NPVARIANT_IS_STRING(args[0]) ||
      !NPVARIANT_IS_INT32(args[argCount - 1]))
    return;

  // build array
  SAFEARRAY *psa = Utils::CreateArray(argCount - 1);
  COleVariant var;
  for (long i = 0 ; i < long(argCount - 1) ; i++) {
    if (NPVARIANT_IS_STRING(args[i])) {
      var.SetString(
          Utils::Utf8ToUnicode(
              (char*)NPVARIANT_TO_STRING(args[i]).UTF8Characters),
          VT_BSTR);
      SafeArrayPutElement(psa, &i, &var);
    } else {
      break;
    }
  }
  VARIANT vList;
  vList.vt = VT_VARIANT|VT_ARRAY|VT_BYREF;
  vList.pparray = &psa;

  CComPtr<IDispatch> dispatch;

  if (!FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL))) {
    // TODO(jingzhao): figure out the currect name
    OLECHAR* name = L"AddAll";
    DISPID dispid;
    if (FAILED(dispatch->GetIDsOfNames(
        IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
      return;

    _bstr_t referrer = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    VARIANT v[5];
    v[4].vt = VT_VARIANT|VT_BYREF;
    v[0].vt = v[1].vt = v[2].vt = v[3].vt = VT_BSTR;
    v[4].pvarVal = &vList;
    v[0].bstrVal = L"0";
    v[1].bstrVal = L"";
    v[2].bstrVal = L"FlashGet3";
    v[3].bstrVal = referrer;

    if (FAILED(dispatch.InvokeN(dispid, v, 5, NULL)))
      return;
  } else if (!FAILED(dispatch.CoCreateInstance(GetProgID(1).c_str(), NULL))) {
    OLECHAR* name = L"AddUrlList";
    DISPID dispid;
    if (FAILED(dispatch->GetIDsOfNames(
        IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
      return;

    VARIANT v[1];
    v[0].vt = VT_VARIANT|VT_BYREF;
    v[0].pvarVal = &vList;

    if (FAILED(dispatch.InvokeN(dispid, v, 1, NULL)))
      return;
  }
  SafeArrayDestroy(psa);
}