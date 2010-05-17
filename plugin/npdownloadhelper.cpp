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

#include "npdownloadhelper.h"

static NPObject* so = NULL;
NPNetscapeFuncs* npnfuncs = NULL;

const char* kThunderIsEnabledMethod = "thunderIsEnabled";
const char* kThunderAddLinkMethod = "thunderAddLink";
const char* kThunderCommitMethod = "thunderCommit";
const char* kThunderDownloadAllMethod = "thunderDownloadAll";
const char* kFlashgetIsEnabledMethod = "flashgetIsEnabled";
const char* kFlashgetAddLinkMethod = "flashgetAddLink";
const char* kFlashgetCommitMethod = "flashgetCommit";
const char* kFlashgetDownloadAllMethod = "flashgetDownloadAll";

void DebugLog(const char* msg) {
#ifdef DEBUG
#ifndef _WINDOWS
  fputs(msg, stderr);
#else
  FILE* out = fopen("\\npdownloadhelper.log", "a");
  fputs(msg, out);
  fclose(out);
#endif
#endif
}

NPObject* CPlugin::Allocate(NPP instance, NPClass* npclass) {
  return (NPObject*)(new CPlugin);
}

void CPlugin::Deallocate(NPObject* obj) {
  delete (CPlugin*)obj;
}

bool CPlugin::HasMethod(NPObject* obj, NPIdentifier methodName) {
  DebugLog("npDownloadHelper: HasMethod\n");
  return true;
}

bool CPlugin::InvokeDefault(NPObject* obj, const NPVariant* args,
                            uint32_t argCount,
    NPVariant* result) {
  DebugLog("npDownloadHelper: InvokeDefault\n");  
  return true;
}

bool CPlugin::Invoke(NPObject* obj, NPIdentifier methodName,
                     const NPVariant* args, uint32_t argCount,
                     NPVariant* result) {
  DebugLog("npDownloadHelper: Invoke\n");
  char* name = npnfuncs->utf8fromidentifier(methodName);
  bool ret_val = false;
  if (!name) {
    return false;
  }
  if (!strncmp((const char*)name, kThunderIsEnabledMethod,
               strlen(kThunderIsEnabledMethod))) {
    ret_val = InvokeThunderIsEnabled(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kThunderAddLinkMethod,
                      strlen(kThunderAddLinkMethod))) {
    ret_val = InvokeThunderAddLink(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kThunderCommitMethod,
                      strlen(kThunderCommitMethod))) {
    ret_val = InvokeThunderCommit(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kThunderDownloadAllMethod,
                      strlen(kThunderDownloadAllMethod))) {
    ret_val = InvokeThunderDownloadAll(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kFlashgetIsEnabledMethod,
                      strlen(kFlashgetIsEnabledMethod))) {
    ret_val = InvokeFlashgetIsEnabled(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kFlashgetAddLinkMethod,
                      strlen(kFlashgetAddLinkMethod))) {
    ret_val = InvokeFlashgetAddLink(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kFlashgetCommitMethod,
                      strlen(kFlashgetCommitMethod))) {
    ret_val = InvokeFlashgetCommit(obj, args, argCount, result);
  } else if (!strncmp((const char*)name, kFlashgetDownloadAllMethod,
                      strlen(kFlashgetDownloadAllMethod))) {
    ret_val = InvokeFlashgetDownloadAll(obj, args, argCount, result);
  } else {
    // Exception handling. 
    npnfuncs->setexception(obj, "exception during invocation");
    ret_val = false;
  }
  if (name) {
    npnfuncs->memfree(name);
  }
  return ret_val;
}

bool CPlugin::HasProperty(NPObject* obj, NPIdentifier propertyName) {
  DebugLog("npDownloadHelper: HasProperty\n");
  char* name = npnfuncs->utf8fromidentifier(propertyName);
  bool ret_val = false;
  // if (name && !strncmp((const char*)name, kGetConnectionNameProperty,
  //                      strlen(kGetConnectionNameProperty))) {
  //   ret_val = true;
  // }
  if (name) {
    npnfuncs->memfree(name);
  }
  return ret_val;
}

bool CPlugin::GetProperty(NPObject* obj, NPIdentifier propertyName,
                          NPVariant* result) {
  DebugLog("npDownloadHelper: GetProperty\n");
  char* name = npnfuncs->utf8fromidentifier(propertyName);
  bool ret_val = false;
  // if (name && !strncmp((const char*)name, kGetConnectionNameProperty,
  //                      strlen(kGetConnectionNameProperty))) {    
  //   ret_val = GetConnectionName(obj, result);
  // }
  if (name) {
    npnfuncs->memfree(name);
  }
  return ret_val;
}

static NPClass plugin_ref_obj = {
  NP_CLASS_STRUCT_VERSION,
  CPlugin::Allocate,
  CPlugin::Deallocate,
  NULL,
  CPlugin::HasMethod,
  CPlugin::Invoke,
  CPlugin::InvokeDefault,
  CPlugin::HasProperty,
  CPlugin::GetProperty,
  NULL,
  NULL,
};

static NPError GetValue(NPP instance, NPPVariable variable, void* value) {
  switch(variable) {
  default:
    DebugLog("npDownloadHelper: GetValue - default\n");
    return NPERR_GENERIC_ERROR;
  case NPPVpluginNameString:
    DebugLog("npDownloadHelper: GetValue - name string\n");
    *((char **)value) = "DownloadHelperPlugin";
    break;
  case NPPVpluginDescriptionString:
    DebugLog("npDownloadHelper: GetValue - description string\n");
    *((char **)value) = "DownloadHelperPlugin plugin.";
    break;
  case NPPVpluginScriptableNPObject:
    DebugLog("npDownloadHelper: GetValue - scriptable object\n");
    if(!so) {
      so = npnfuncs->createobject(instance, &plugin_ref_obj);
    }
    // Retain the object since we keep it in plugin code
    // so that it won't be freed by browser.
    npnfuncs->retainobject(so);
    *(NPObject **)value = so;
    break;
#if defined(XULRUNNER_SDK)
  case NPPVpluginNeedsXEmbed:
    DebugLog("npDownloadHelper: GetValue - xembed\n");
    *((PRBool *)value) = PR_FALSE;
    break;
#endif
  }
  return NPERR_NO_ERROR;
}

static NPError NewNPInstance(NPMIMEType pluginType, NPP instance,
                             uint16_t mode, int16_t argc, char* argn[],
                             char* argv[], NPSavedData* saved) {
  DebugLog("npDownloadHelper: new\n");
  return NPERR_NO_ERROR;
}

static NPError DestroyNPInstance(NPP instance, NPSavedData** save) {
  if(so) {
    npnfuncs->releaseobject(so);
  }
  so = NULL;
  DebugLog("npDownloadHelper: DestroyNPInstance\n");
  return NPERR_NO_ERROR;
}

#ifdef __cplusplus
extern "C" {
#endif
  NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* nppfuncs) {
    DebugLog("npDownloadHelper: NP_GetEntryPoints\n");
    nppfuncs->version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
    nppfuncs->newp = NewNPInstance;
    nppfuncs->destroy = DestroyNPInstance;
    nppfuncs->getvalue = GetValue;
    return NPERR_NO_ERROR;
  }

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL NP_Initialize(NPNetscapeFuncs* npnf
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
               , NPPluginFuncs *nppfuncs) {
#else
               ) {
#endif
                 DebugLog("npDownloadHelper: NP_Initialize\n");
                 if(npnf == NULL) {
                   return NPERR_INVALID_FUNCTABLE_ERROR;
                 }
                 if(HIBYTE(npnf->version) > NP_VERSION_MAJOR) {
                   return NPERR_INCOMPATIBLE_VERSION_ERROR;
                 }
                 npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
                 NP_GetEntryPoints(nppfuncs);
#endif

                 return NPERR_NO_ERROR;
}
NPError  OSCALL NP_Shutdown() {
  DebugLog("npDownloadHelper: NP_Shutdown\n");
  return NPERR_NO_ERROR;
}

char* NP_GetMIMEDescription(void) {
  DebugLog("npDownloadHelper: NP_GetMIMEDescription\n");
  return "application/x-downloadhelper";
}

// Needs to be present for WebKit based browsers.
NPError OSCALL NP_GetValue(void* npp, NPPVariable variable, void* value) {
  return GetValue((NPP)npp, variable, value);
}
#ifdef __cplusplus
}
#endif