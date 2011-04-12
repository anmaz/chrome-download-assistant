#include "stdafx.h"
#include "downloader_script_object.h"
#include "log.h"
#include <stdlib.h>
#ifdef OS_LINUX
#include <unistd.h>
#include <wait.h>
#endif

extern Log g_Log;

NPObject* DownloaderScriptObject::Allocate(NPP npp, NPClass *aClass) {
  DownloaderScriptObject* pRet = new DownloaderScriptObject;
  char logs[256];
  sprintf(logs, "DownloaderScriptObject this=%ld", pRet);
  g_Log.WriteLog("Allocate",logs);
  if (pRet != NULL) {
    pRet->set_plugin((PluginBase*)npp->pdata);
  }
  return pRet;
}

void DownloaderScriptObject::Deallocate() {
  char logs[256];
  sprintf(logs, "DownloaderScriptObject this=%ld", this);
  g_Log.WriteLog("Deallocate",logs);
  delete this;
}

void DownloaderScriptObject::InitHandler() {
  Function_Item item;
  item.function_name = "Download";
  item.function_pointer = ON_INVOKEHELPER(&DownloaderScriptObject::
      Download);
  AddFunction(item);
}

bool DownloaderScriptObject::Download(const NPVariant *args,
                                      uint32_t argCount,
                                      NPVariant *result) {
#ifdef OS_LINUX
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  const char* parameter = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

  if (fork() == 0) {
    execlp(execute_file_.c_str(), execute_file_.c_str(), parameter, 0);
    exit(0);
  }
#endif
  return true;
}
