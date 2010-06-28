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

#include <atlbase.h>
#include <atlcom.h>
#include <comutil.h>
#include "utils.h"
#include "thunder.h"

using namespace std;

ThunderSupport::ThunderSupport() {
}

ThunderSupport::~ThunderSupport() {
}

std::wstring ThunderSupport::GetProgID() {
  return L"ThunderAgent.Agent";
}

// Check if Thunder is enabled.
void ThunderSupport::IsEnabled(NPObject* obj, const NPVariant* args,
                            uint32_t argCount, NPVariant* result) {
  result->type = NPVariantType_Bool;
  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL)))
    result->value.boolValue = FALSE;
  else
    result->value.boolValue = TRUE;
}

// Add a link to download in Thunder.
void ThunderSupport::AddLink(NPObject* obj, const NPVariant* args,
                          uint32_t argCount, NPVariant* result) {
  if (argCount != 3 || !NPVARIANT_IS_STRING(args[0]) ||
      !NPVARIANT_IS_STRING(args[1]) || !NPVARIANT_IS_STRING(args[2]))
    return;

  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL)))
    return;

  OLECHAR* name = L"AddTask";
  DISPID dispid;
  if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return;

  _bstr_t referrer = NPVARIANT_TO_STRING(args[2]).UTF8Characters;
  _bstr_t comments = Utils::Utf8ToUnicode((char*)
      NPVARIANT_TO_STRING(args[1]).UTF8Characters);
  _bstr_t filename = L"";
  _bstr_t path = L"";
  _bstr_t url = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

  VARIANT v[8];
  v[3].vt = v[4].vt = v[5].vt = v[6].vt = v[7].vt = VT_BSTR;
  v[0].vt = v[1].vt = v[2].vt = VT_INT;
  v[3].bstrVal = referrer;
  v[4].bstrVal = comments;
  v[5].bstrVal = filename;
  v[6].bstrVal = path;
  v[7].bstrVal = url;
  v[2].intVal = -1;
  v[1].intVal = 0;
  v[0].intVal = -1;

  if (FAILED(dispatch.InvokeN(dispid, v, 8, NULL)))
    return;

  name = L"CommitTasks";
  if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return;
  if (FAILED(dispatch.Invoke0(dispid, NULL)))
    return;
}

// Download all links with Thunder.
void ThunderSupport::DownloadAll(NPObject* obj, const NPVariant* args,
                              uint32_t argCount, NPVariant* result){
  if (argCount == 0 || !NPVARIANT_IS_STRING(args[0]) ||
      !NPVARIANT_IS_INT32(args[argCount - 1]))
    return;

  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(GetProgID().c_str(), NULL)))
    return;

  OLECHAR* name = L"AddTask";
  DISPID dispid;
  if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return;

  _bstr_t referrer = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  _bstr_t filename = L"";
  _bstr_t path = L"";
  for (uint32_t i = 1; i < argCount - 1; i += 2) {
    _bstr_t comments = Utils::Utf8ToUnicode((char*)
        NPVARIANT_TO_STRING(args[i + 1]).UTF8Characters);
    _bstr_t url = NPVARIANT_TO_STRING(args[i]).UTF8Characters;

    VARIANT v[8];
    v[3].vt = v[4].vt = v[5].vt = v[6].vt = v[7].vt = VT_BSTR;
    v[0].vt = v[1].vt = v[2].vt = VT_INT;
    v[3].bstrVal = referrer;
    v[4].bstrVal = comments;
    v[5].bstrVal = filename;
    v[6].bstrVal = path;
    v[7].bstrVal = url;
    v[2].intVal = -1;
    v[1].intVal = 0;
    v[0].intVal = -1;

    if (FAILED(dispatch.InvokeN(dispid, v, 8, NULL)))
      return;
  }

  name = L"CommitTasks";
  if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return;
  if (FAILED(dispatch.Invoke0(dispid, NULL)))
    return;
}
