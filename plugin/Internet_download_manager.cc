#include "stdafx.h"
#include "Internet_download_manager.h"
#include "Log.h"
#include <tchar.h>

extern Log g_Log;

TCHAR InternetDownloadManager::idm_exe_path[MAX_PATH] = L"";

InternetDownloadManager::InternetDownloadManager(void) {
}

InternetDownloadManager::~InternetDownloadManager(void) {
}

NPObject* InternetDownloadManager::Allocate(NPP npp, NPClass *aClass) {
  InternetDownloadManager* pRet = new InternetDownloadManager;
  char logs[256];
  sprintf_s(logs,"InternetDownloadManager this=%ld",pRet);
  g_Log.WriteLog("Allocate",logs);
  if (pRet != NULL) {
    pRet->SetPlugin((PluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.function_name,"Download");
    item.function_pointer = ON_INVOKEHELPER(&InternetDownloadManager::Download);
    pRet->AddFunction(item);
  }
  return pRet;
}

void InternetDownloadManager::Deallocate() {
  char logs[256];
  sprintf_s(logs,"InternetDownloadManager this=%ld",this);
  g_Log.WriteLog("Deallocate",logs);
  delete this;
}

void InternetDownloadManager::Invalidate() {

}

bool InternetDownloadManager::Construct(const NPVariant *args,
                                         uint32_t argCount,
                                         NPVariant *result) {
  return true;
}

bool InternetDownloadManager::CheckObject() {
  DWORD buffer_len = sizeof(idm_exe_path);
  HKEY hkey;
  if (RegOpenKey(HKEY_CURRENT_USER, L"Software\\DownloadManager", &hkey))
    return false;
  if (RegQueryValueEx(hkey, L"ExePath", NULL, NULL, (LPBYTE)idm_exe_path, 
                      &buffer_len)) {
    RegCloseKey(hkey);
    return false;
  }
  RegCloseKey(hkey);
  if (_taccess(idm_exe_path, 0))
    return false;
  return true;
}

bool InternetDownloadManager::Download(const NPVariant *args,
                                       uint32_t argCount,
                                       NPVariant *result) {
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  BOOLEAN_TO_NPVARIANT(FALSE, *result);

  WCHAR* link = new WCHAR[NPVARIANT_TO_STRING(args[0]).UTF8Length + 1];
  if (!link)
    return true;

  wstring params;
  int len = NPVARIANT_TO_STRING(args[0]).UTF8Length + 1;
  if (!MultiByteToWideChar(CP_UTF8, 0, 
                           NPVARIANT_TO_STRING(args[0]).UTF8Characters,
                           -1, link, len)) {
    return true;
  } else {
    params = L"/d ";
    params += link;
    delete[] link;
  }

  HINSTANCE ret = ShellExecuteW(NULL, L"open", idm_exe_path, params.c_str(), 
                               NULL, SW_SHOWNORMAL);
  if (ret != 0)
    return true;

  BOOLEAN_TO_NPVARIANT(TRUE, *result);

  return true;
}