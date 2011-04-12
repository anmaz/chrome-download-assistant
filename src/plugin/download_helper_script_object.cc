#include "stdafx.h"
#ifdef OS_LINUX
#include <unistd.h>
#include <wait.h>
#elif defined OS_WIN
#include <comdef.h>
#include "com_object_wapper.h"
#include "internet_download_manager.h"
#endif

#include "download_helper_script_object.h"
#include "downloader_script_object.h"
#include "log.h"
#include "script_object_factory.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

extern Log g_Log;

NPObject* DownloadHelperScriptObject::Allocate(NPP npp, NPClass *aClass) {
  DownloadHelperScriptObject* pRet = new DownloadHelperScriptObject;
  char logs[256];
  sprintf(logs, "DownloadHelperScriptObject this=%ld", pRet);
  g_Log.WriteLog("Allocate",logs);
  if (pRet != NULL) {
    pRet->set_plugin((PluginBase*)npp->pdata);
  }
  return pRet;
}

void DownloadHelperScriptObject::Deallocate() {
  char logs[256];
  sprintf(logs, "DownloadHelperScriptObject this=%ld", this);
  g_Log.WriteLog("Deallocate",logs);
  delete this;
}

void DownloadHelperScriptObject::InitHandler() {
  Function_Item item;
  item.function_name = "CreateObject";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CreateObject);
  AddFunction(item);
  item.function_name = "CheckObject";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CheckObject);
  AddFunction(item);
  item.function_name = "CopyToClipboard";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CopyToClipboard);
  AddFunction(item);
}

bool DownloadHelperScriptObject::CreateObject(const NPVariant* args,
                                              uint32_t argCount,
                                              NPVariant* result) {
  char logs[256];
  NULL_TO_NPVARIANT(*result);
#ifdef OS_WIN
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* pProgID = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    g_Log.WriteLog("ProgID", pProgID);
    if (stricmp(pProgID, "DownlWithIDM.LinkProcessor") == 0) {
      DownloaderScriptObject* pObject = (DownloaderScriptObject*)ScriptObjectFactory::
          CreateObject(get_plugin()->get_npp(), InternetDownloadManager::Allocate);
      OBJECT_TO_NPVARIANT(pObject, *result);
      return true;
    }

    utils::Utf8ToUnicode progID(pProgID);
    IDispatch* pInterface;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(progID, &clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid, &pClssID);
    _bstr_t bstr(pClssID);
    g_Log.WriteLog("CLSIDFromProgID", bstr);
    if (SUCCEEDED(hr)) {
      hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IDispatch,
          (LPVOID*)&pInterface);
      g_Log.WriteLog("CreateObject", "CoCreateInstance");
      if (SUCCEEDED(hr)) {
        ComObjectWapper* pObject = (ComObjectWapper*)ScriptObjectFactory::
            CreateObject(get_plugin()->get_npp(), ComObjectWapper::Allocate);
        OBJECT_TO_NPVARIANT(pObject, *result);
        pObject->disp_pointer_ = pInterface;
        sprintf(logs, "pInterface=0x%X,pObject=%ld", pInterface, pObject);
        g_Log.WriteLog("CreateObject", logs);
      } else {
        sprintf(logs, "GetLastError=%ld,hr=0x%X", GetLastError(), hr);
        g_Log.WriteLog("Error", logs);
      }
    }
  }
#elif defined OS_LINUX
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* execute_file = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    g_Log.WriteLog("ProgID", execute_file);

    DownloaderScriptObject* pObject = (DownloaderScriptObject*)
        ScriptObjectFactory::CreateObject(get_plugin()->get_npp(),
        DownloaderScriptObject::Allocate);
    OBJECT_TO_NPVARIANT(pObject, *result);
    if (pObject)
      pObject->set_execute_file(execute_file);
  }
#endif
  
  return true;
}


bool DownloadHelperScriptObject::CheckObject(const NPVariant* args,
                                             uint32_t argCount,
                                             NPVariant* result) {
  BOOLEAN_TO_NPVARIANT(false, *result);
#ifdef OS_WIN
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* prog_id = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    g_Log.WriteLog("ProgID", prog_id);

    utils::Utf8ToUnicode wchar_prog_id(prog_id);
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(wchar_prog_id, &clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid, &pClssID);
    _bstr_t bstr(pClssID);
    g_Log.WriteLog("CLSIDFromProgID", bstr);
    if (SUCCEEDED(hr)) {
      BOOLEAN_TO_NPVARIANT(true, *result);
    }
  }
#elif defined OS_LINUX
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  const char* downloader_name = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  std::string command = "which ";
  command += downloader_name;
  FILE* p = popen(command.c_str(), "r");
  if (p != NULL) {
    char echo_contents[256] = "";
    int count = fread(echo_contents, 1, 256, p);
    if (count > 0) {
      echo_contents[count] = 0;
    }
    if (strstr(echo_contents, downloader_name) != NULL)
      BOOLEAN_TO_NPVARIANT(true, *result);
    pclose(p);
  }
#endif
  return true;
}

bool DownloadHelperScriptObject::CopyToClipboard(const NPVariant* args,
                                                 uint32_t argCount,
                                                 NPVariant* result) {
  return true;
}
