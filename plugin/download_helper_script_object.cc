#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#include "download_helper_script_object.h"
#include "script_object_factory.h"
#include "log.h"

extern Log g_Log;

DownloadHelperScriptObject::DownloadHelperScriptObject() {
}

DownloadHelperScriptObject::~DownloadHelperScriptObject() {
}

NPObject* DownloadHelperScriptObject::Allocate(NPP npp, NPClass *aClass) {
  DownloadHelperScriptObject* pRet = new DownloadHelperScriptObject;
//  char logs[256];
//  sprintf(logs, "DownloadHelperScriptObject this=%ld", pRet);
//  g_Log.WriteLog("Allocate",logs);
  if (pRet != NULL) {
    pRet->SetPlugin((PluginBase*)npp->pdata);
    Function_Item item;
    strcpy(item.function_name, "CreateObject");
    item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
        CreateObject);
    pRet->AddFunction(item);
    strcpy(item.function_name, "CheckObject");
    item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
        CheckObject);
    pRet->AddFunction(item);
    strcpy(item.function_name, "CopyToClipboard");
    item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
        CopyToClipboard);
    pRet->AddFunction(item);
    strcpy(item.function_name, "Download");
    item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
        Download);
    pRet->AddFunction(item);
  }
  return pRet;
}

void DownloadHelperScriptObject::Deallocate() {
//  char logs[256];
//  sprintf(logs, "DownloadHelperScriptObject this=%ld", this);
//  g_Log.WriteLog("Deallocate",logs);
  delete this;
}

void DownloadHelperScriptObject::Invalidate() {

}

bool DownloadHelperScriptObject::Construct(const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result) {
  return true;
}

bool DownloadHelperScriptObject::CreateObject(const NPVariant* args,
                                              uint32_t argCount,
                                              NPVariant* result) {
  char logs[256];

  NULL_TO_NPVARIANT(*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* execute_file = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    g_Log.WriteLog("ProgID", execute_file);

    DownloadHelperScriptObject* pObject = (DownloadHelperScriptObject*)
        ScriptObjectFactory::CreateObject(plugin_->get_npp(),
        DownloadHelperScriptObject::Allocate);
    OBJECT_TO_NPVARIANT(pObject, *result);
    if (pObject)
      pObject->set_execute_file(execute_file);
  }
  
  return true;
}


bool DownloadHelperScriptObject::CheckObject(const NPVariant* args,
                                             uint32_t argCount,
                                             NPVariant* result) {
  BOOLEAN_TO_NPVARIANT(false, *result);
  
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  const char* downloader_name = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  string command = "which ";
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
  return true;
}

bool DownloadHelperScriptObject::CopyToClipboard(const NPVariant* args,
                                                 uint32_t argCount,
                                                 NPVariant* result) {
  return true;
}

bool DownloadHelperScriptObject::Download(const NPVariant *args,
                                          uint32_t argCount,
                                          NPVariant *result) {
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;
  
  const char* parameter = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  
  if (fork() == 0) {
    execlp(execute_file_.c_str(), execute_file_.c_str(), parameter, 0);
    exit(0);
  }
  
  return true;
}
