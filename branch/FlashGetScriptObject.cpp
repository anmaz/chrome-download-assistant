#include "StdAfx.h"
#include "FlashGetScriptObject.h"
#include "Log.h"
#include "flashget.h"

extern CLog gLog;

CFlashGetScriptObject::CFlashGetScriptObject(void)
{
}

CFlashGetScriptObject::~CFlashGetScriptObject(void)
{
}

NPObject* CFlashGetScriptObject::Allocate(NPP npp, NPClass *aClass) {
  CFlashGetScriptObject* pRet = new CFlashGetScriptObject;
  char szLog[256];
  sprintf_s(szLog,"CFlashGetScriptObject this=%ld",pRet);
  gLog.WriteLog("Allocate",szLog);
  if (pRet != NULL) {
    pRet->SetPlugin((CPluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.szFunName,"IsEnabled");
    item.FunPtr = ON_INVOKEHELPER(&CFlashGetScriptObject::IsEnabled);
    pRet->AddFunction(item);
    strcpy_s(item.szFunName,"AddLink");
    item.FunPtr = ON_INVOKEHELPER(&CFlashGetScriptObject::AddLink);
    pRet->AddFunction(item);
    strcpy_s(item.szFunName,"DownloadAll");
    item.FunPtr = ON_INVOKEHELPER(&CFlashGetScriptObject::DownloadAll);
    pRet->AddFunction(item);
  }
  return pRet;
}

void CFlashGetScriptObject::Deallocate() {
  char szLog[256];
  sprintf_s(szLog,"CFlashGetScriptObject this=%ld",this);
  gLog.WriteLog("Deallocate",szLog);
  delete this;
}

void CFlashGetScriptObject::Invalidate() {

}

bool CFlashGetScriptObject::Construct(const NPVariant *args,uint32_t argCount,
                                      NPVariant *result) {
  return true;
}

bool CFlashGetScriptObject::IsEnabled(const NPVariant* args,uint32_t argCount,
                                      NPVariant* result) {
  FlashgetSupport::IsEnabled(NULL,args,argCount,result);
  return true;
}

bool CFlashGetScriptObject::AddLink(const NPVariant* args,uint32_t argCount,
                                    NPVariant* result) {
  FlashgetSupport::AddLink(NULL,args,argCount,result);
  return true;
}

bool CFlashGetScriptObject::DownloadAll(const NPVariant* args,uint32_t argCount,
                                        NPVariant* result) {
  FlashgetSupport::DownloadAll(NULL,args,argCount,result);
  return true;
}
