#include "StdAfx.h"
#include "com_object_factory.h"
#include "log.h"
#include "script_object_factory.h"
#include "com_object_wapper.h"
#include <comdef.h>
<<<<<<< HEAD
#include "flashget_script_object.h"
#include "flashget.h"
#include "miniflashget_script_object.h"
=======
#include "Internet_download_manager.h"
>>>>>>> refactor

extern Log g_Log;

ComObjectFactory::ComObjectFactory(void) {
}

ComObjectFactory::~ComObjectFactory(void) {
}

NPObject* ComObjectFactory::Allocate(NPP npp, NPClass *aClass) {
  ComObjectFactory* factory = new ComObjectFactory;
  char logs[256];
<<<<<<< HEAD
  sprintf_s(logs, "CComObjectFactory this=%ld", factory);
=======
  sprintf(logs, "ComObjectFactory this=%ld", factory);
>>>>>>> refactor
  g_Log.WriteLog("Allocate", logs);
  if (factory != NULL) {
    factory->SetPlugin((PluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.function_name, "CreateObject");
    item.function_pointer = ON_INVOKEHELPER(&ComObjectFactory::CreateObject);
    factory->AddFunction(item);
    strcpy_s(item.function_name, "CheckObject");
    item.function_pointer = ON_INVOKEHELPER(&ComObjectFactory::CheckObject);
    factory->AddFunction(item);
    strcpy_s(item.function_name, "CopyToClipboard");
    item.function_pointer = ON_INVOKEHELPER(&ComObjectFactory::CopyToClipboard);
    factory->AddFunction(item);
  }
  return factory;
}

void ComObjectFactory::Deallocate() {
  char logs[256];
<<<<<<< HEAD
  sprintf_s(logs, "CComObjectFactory this=%ld", this);
=======
  sprintf(logs, "ComObjectFactory this=%ld", this);
>>>>>>> refactor
  g_Log.WriteLog("Deallocate", logs);
  delete this;
}

void ComObjectFactory::Invalidate() {

}

bool ComObjectFactory::Construct(const NPVariant *args, uint32_t argCount,
                                 NPVariant *result) {
  return true;
}

bool ComObjectFactory::CheckObject(const NPVariant *args, uint32_t argCount,
                                   NPVariant *result) { 
  BOOLEAN_TO_NPVARIANT(false,*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* prog_id = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    g_Log.WriteLog("ProgID", prog_id);

<<<<<<< HEAD
    if (stricmp(prog_id, "FlashGet") == 0){
      FlashgetSupport::IsEnabled(NULL, args, argCount, result);
      return true;
    } else if (stricmp(prog_id,"MiniFlashGet") == 0) {
      MiniflashgetScriptObject::CheckObject(args, argCount, result);
=======
    if (stricmp(prog_id, "DownlWithIDM.LinkProcessor") == 0) {
      BOOLEAN_TO_NPVARIANT(InternetDownloadManager::CheckObject(), *result);
>>>>>>> refactor
      return true;
    }

    TCHAR wchar_prog_id[256];
    MultiByteToWideChar(CP_UTF8, 0, prog_id, -1, wchar_prog_id, 256);
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

  return true;
}

bool ComObjectFactory::CreateObject(const NPVariant *args, uint32_t argCount,
                                    NPVariant *result) {
  char logs[256];

  NULL_TO_NPVARIANT(*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* pProgID = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
<<<<<<< HEAD
    if (stricmp(pProgID, "FlashGet") == 0){
      ScriptObjectBase* pObject = (ScriptObjectBase*)ScriptObjectFactory::
          CreateObject(plugin_->get_npp(), FlashGetScriptObject::Allocate);
      OBJECT_TO_NPVARIANT(pObject, *result);
      return true;
    } else if (stricmp(pProgID,"MiniFlashGet") == 0) {
      ScriptObjectBase* pObject = (ScriptObjectBase*)ScriptObjectFactory::
          CreateObject(plugin_->get_npp(), MiniflashgetScriptObject::Allocate);
=======

    g_Log.WriteLog("ProgID", pProgID);
    if (stricmp(pProgID, "DownlWithIDM.LinkProcessor") == 0) {
      ScriptObjectBase* pObject = (ScriptObjectBase*)ScriptObjectFactory::
          CreateObject(plugin_->get_npp(), InternetDownloadManager::Allocate);
>>>>>>> refactor
      OBJECT_TO_NPVARIANT(pObject, *result);
      return true;
    }

<<<<<<< HEAD
    g_Log.WriteLog("ProgID", pProgID);
=======
>>>>>>> refactor
    TCHAR progID[256];
    MultiByteToWideChar(CP_UTF8, 0, pProgID, -1, progID, 256);
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
            CreateObject(plugin_->get_npp(), ComObjectWapper::Allocate);
        OBJECT_TO_NPVARIANT(pObject, *result);
        pObject->disp_pointer_ = pInterface;
        sprintf_s(logs, "pInterface=0x%X,pObject=%ld", pInterface, pObject);
        g_Log.WriteLog("CreateObject", logs);
      } else {
        sprintf_s(logs, "GetLastError=%ld,hr=0x%X", GetLastError(), hr);
        g_Log.WriteLog("Error", logs);
      }
    }
  }
    
  return true;
}

bool ComObjectFactory::CopyToClipboard(const NPVariant *args,
                                       uint32_t argCount, NPVariant *result) {
  BOOLEAN_TO_NPVARIANT(false, *result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* data= NPVARIANT_TO_STRING(args[0]).UTF8Characters;
<<<<<<< HEAD
    int nLen = NPVARIANT_TO_STRING(args[0]).UTF8Length+1;
=======
    int nLen = NPVARIANT_TO_STRING(args[0]).UTF8Length + 1;
>>>>>>> refactor
    g_Log.WriteLog("data", data);
    if (OpenClipboard(plugin_->get_hwnd()))
    {
      EmptyClipboard();
      TCHAR* pData = new TCHAR[nLen];
      MultiByteToWideChar(CP_UTF8, 0, data, -1, pData, nLen);
      HANDLE h = GlobalAlloc(GMEM_MOVEABLE, nLen*2);
      if (h) {
        LPVOID pMsg = GlobalLock(h);
        wmemcpy((wchar_t*)pMsg, pData, nLen);
        GlobalUnlock(h);
        SetClipboardData(CF_UNICODETEXT, h);
        g_Log.WriteLog("Msg", "CopyToClipboard");
      }
      delete[] pData;
      BOOLEAN_TO_NPVARIANT(true, *result);
      CloseClipboard();
    }
  }

  return true;
}