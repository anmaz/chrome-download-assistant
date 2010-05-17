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

#include "thunder.h"
#include "../utils.h"

#import "ThunderAgent.dll"
using namespace THUNDERAGENTLib;

// Check if Thunder is enabled.
bool InvokeThunderIsEnabled(NPObject* obj, const NPVariant* args,
                            uint32_t argCount, NPVariant* result) {
  result->type = NPVariantType_Bool;
  HRESULT hr = ::CoInitialize(0);
  IAgent *pAgent = NULL; 
  hr = Utils::GetCom(__uuidof(Agent), __uuidof(IAgent), (void**)&pAgent); 
  if (FAILED(hr)) {
      result->value.intValue = FALSE;
  } else {
    result->value.intValue = TRUE;
    pAgent->Release();
  }
  return true;
}

// Add a link to download in Thunder.
bool InvokeThunderAddLink(NPObject* obj, const NPVariant* args,
                          uint32_t argCount, NPVariant* result) {
  bool flag = true;
  if (argCount == 0) {
    flag = false;
  } else if (argCount == 3 && NPVARIANT_IS_STRING(args[0]) &&
             NPVARIANT_IS_STRING(args[1]) && NPVARIANT_IS_STRING(args[2])){
    HRESULT hr = ::CoInitialize(0);
    IAgent *pAgent;
    hr = Utils::GetCom(__uuidof(Agent),  __uuidof(IAgent), (void**)&pAgent); 
    if (FAILED(hr)) {
      flag = false;
    } else {
      hr = pAgent->AddTask(
          NPVARIANT_TO_STRING(args[0]).UTF8Characters, "", "",
          Utils::Utf8ToUnicode(
              (char*)NPVARIANT_TO_STRING(args[1]).UTF8Characters),
          NPVARIANT_TO_STRING(args[2]).UTF8Characters, 1, 0, -1);
      pAgent->CommitTasks();
      pAgent->Release();
    }
  }
  return true;
}

bool InvokeThunderCommit(NPObject* obj, const NPVariant* args,
                         uint32_t argCount, NPVariant* result){
  // HRESULT hr = pAgent->CommitTasks();
  return true;
}

// Download all links with Thunder.
bool InvokeThunderDownloadAll(NPObject* obj, const NPVariant* args,
                              uint32_t argCount, NPVariant* result){
  bool flag = true;
  if (argCount == 0) {
    flag = false;
  } else if (NPVARIANT_IS_STRING(args[0]) &&
             NPVARIANT_IS_INT32(args[argCount - 1])) {
    int count = argCount - 1;
    HRESULT hr = ::CoInitialize(0);
    IAgent *pAgent;
    hr = Utils::GetCom(__uuidof(Agent), __uuidof(IAgent), (void**)&pAgent); 
    if (FAILED(hr)) {
      flag = false;
    } else {
      for (int i = 2 ; i < count ; i += 2) {
        if (NPVARIANT_IS_STRING(args[i]) && NPVARIANT_IS_STRING(args[i - 1])) {
          hr = pAgent->AddTask(
              NPVARIANT_TO_STRING(args[i - 1]).UTF8Characters, "", "",
              Utils::Utf8ToUnicode(
                  (char*)NPVARIANT_TO_STRING(args[i]).UTF8Characters),
              NPVARIANT_TO_STRING(args[0]).UTF8Characters, 1, 0, -1);
        } else {
          break;
        }
      }
      pAgent->CommitTasks();
      pAgent->Release();
    }
  } else {
    flag = false;
  }
  return flag;
}