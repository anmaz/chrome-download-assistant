#include "StdAfx.h"
#include "flashget_script_object.h"
#include "Log.h"
#include "flashget.h"

extern Log g_Log;

FlashGetScriptObject::FlashGetScriptObject(void)
{
}

FlashGetScriptObject::~FlashGetScriptObject(void)
{
}

NPObject* FlashGetScriptObject::Allocate(NPP npp, NPClass *aClass) {
  FlashGetScriptObject* pRet = new FlashGetScriptObject;
  char logs[256];
  sprintf_s(logs, "CFlashGetScriptObject this=%ld", pRet);
  g_Log.WriteLog("Allocate", logs);
  if (pRet != NULL) {
    pRet->SetPlugin((PluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.function_name, "IsEnabled");
    item.function_pointer = ON_INVOKEHELPER(&FlashGetScriptObject::IsEnabled);
    pRet->AddFunction(item);
    strcpy_s(item.function_name, "AddLink");
    item.function_pointer = ON_INVOKEHELPER(&FlashGetScriptObject::AddLink);
    pRet->AddFunction(item);
    strcpy_s(item.function_name, "DownloadAll");
    item.function_pointer = ON_INVOKEHELPER(&FlashGetScriptObject::DownloadAll);
    pRet->AddFunction(item);
  }
  return pRet;
}

void FlashGetScriptObject::Deallocate() {
  char logs[256];
  sprintf_s(logs, "CFlashGetScriptObject this=%ld", this);
  g_Log.WriteLog("Deallocate", logs);
  delete this;
}

void FlashGetScriptObject::Invalidate() {

}

bool FlashGetScriptObject::Construct(const NPVariant *args, uint32_t argCount,
                                     NPVariant *result) {
  return true;
}

bool FlashGetScriptObject::IsEnabled(const NPVariant* args, uint32_t argCount,
                                     NPVariant* result) {
  FlashgetSupport::IsEnabled(NULL, args, argCount, result);
  return true;
}

bool FlashGetScriptObject::AddLink(const NPVariant* args, uint32_t argCount,
                                   NPVariant* result) {
  FlashgetSupport::AddLink(NULL, args, argCount, result);
  return true;
}

bool FlashGetScriptObject::DownloadAll(const NPVariant* args, uint32_t argCount,
                                       NPVariant* result) {
  FlashgetSupport::DownloadAll(NULL, args, argCount, result);
  return true;
}
