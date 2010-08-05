#include "StdAfx.h"
#include "ComObjectFactory.h"
#include "Log.h"
#include "ScriptObjectFactory.h"
#include "ComObjectWapper.h"
#include <comdef.h>
#include "FlashGetScriptObject.h"
#include "flashget.h"
#include "MiniFlashGetScriptObject.h"

extern CLog gLog;

CComObjectFactory::CComObjectFactory(void)
{
}

CComObjectFactory::~CComObjectFactory(void)
{
}

NPObject* CComObjectFactory::Allocate(NPP npp, NPClass *aClass) {
  CComObjectFactory* pRet = new CComObjectFactory;
  char szLog[256];
  sprintf_s(szLog,"CComObjectFactory this=%ld",pRet);
  gLog.WriteLog("Allocate",szLog);
  if (pRet != NULL) {
    pRet->SetPlugin((CPluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.szFunName,"CreateObject");
    item.FunPtr = ON_INVOKEHELPER(&CComObjectFactory::CreateObject);
    pRet->AddFunction(item);
    strcpy_s(item.szFunName,"CheckObject");
    item.FunPtr = ON_INVOKEHELPER(&CComObjectFactory::CheckObject);
    pRet->AddFunction(item);
    strcpy_s(item.szFunName,"CopyToClipboard");
    item.FunPtr = ON_INVOKEHELPER(&CComObjectFactory::CopyToClipboard);
    pRet->AddFunction(item);
  }
  return pRet;
}

void CComObjectFactory::Deallocate() {
  char szLog[256];
  sprintf_s(szLog,"CComObjectFactory this=%ld",this);
  gLog.WriteLog("Deallocate",szLog);
  delete this;
}

void CComObjectFactory::Invalidate() {

}

bool CComObjectFactory::Construct(const NPVariant *args,uint32_t argCount,
                                  NPVariant *result) {
  return true;
}

bool CComObjectFactory::CheckObject(const NPVariant *args,uint32_t argCount,
                                    NPVariant *result) {
  char szLog[256];
  
  BOOLEAN_TO_NPVARIANT(false,*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* pProgID = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    gLog.WriteLog("ProgID",pProgID);

    if (stricmp(pProgID,"FlashGet")==0){
      FlashgetSupport::IsEnabled(NULL,args,argCount,result);
      return true;
    } else if (stricmp(pProgID,"MiniFlashGet")==0) {
      CMiniFlashGetScriptObject::CheckObject(args,argCount,result);
      return true;
    }

    TCHAR progID[256];
    MultiByteToWideChar(CP_UTF8,0,pProgID,-1,progID,256);
    IDispatch* pInterface;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(progID,&clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid,&pClssID);
    _bstr_t bstr(pClssID);
    gLog.WriteLog("CLSIDFromProgID",bstr);
    if (SUCCEEDED(hr)) {
      BOOLEAN_TO_NPVARIANT(true,*result);
    }
  }

  return true;
}

bool CComObjectFactory::CreateObject(const NPVariant *args,uint32_t argCount,
                                     NPVariant *result) {
  char szLog[256];

  NULL_TO_NPVARIANT(*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* pProgID = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    if (stricmp(pProgID,"FlashGet")==0){
      CScriptObjectBase* pObject = (CScriptObjectBase*)CScriptObjectFactory::CreateObject(this->m_Plugin->m_NPP,CFlashGetScriptObject::Allocate);
      OBJECT_TO_NPVARIANT(pObject,*result);
      return true;
    } else if (stricmp(pProgID,"MiniFlashGet")==0) {
      CScriptObjectBase* pObject = (CScriptObjectBase*)CScriptObjectFactory::CreateObject(this->m_Plugin->m_NPP,CMiniFlashGetScriptObject::Allocate);
      OBJECT_TO_NPVARIANT(pObject,*result);
      return true;
    }

    gLog.WriteLog("ProgID",pProgID);
    TCHAR progID[256];
    MultiByteToWideChar(CP_UTF8,0,pProgID,-1,progID,256);
    IDispatch* pInterface;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(progID,&clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid,&pClssID);
    _bstr_t bstr(pClssID);
    gLog.WriteLog("CLSIDFromProgID",bstr);
    if (SUCCEEDED(hr)) {
      hr = CoCreateInstance(clsid,NULL,CLSCTX_SERVER,IID_IDispatch,
                            (LPVOID*)&pInterface);
      gLog.WriteLog("CreateObject","CoCreateInstance");
      if (SUCCEEDED(hr)) {
        CComObjectWapper* pObject = (CComObjectWapper*)CScriptObjectFactory::CreateObject(this->m_Plugin->m_NPP,CComObjectWapper::Allocate);
        OBJECT_TO_NPVARIANT(pObject,*result);
        pObject->m_pDisp = pInterface;
        sprintf_s(szLog,"pInterface=0x%X,pObject=%ld",pInterface,pObject);
        gLog.WriteLog("CreateObject",szLog);
      } else {
        sprintf_s(szLog,"GetLastError=%ld,hr=0x%X",GetLastError(),hr);
        gLog.WriteLog("Error",szLog);
      }
    }
  }
    
  return true;
}

bool CComObjectFactory::CopyToClipboard(const NPVariant *args,
                                        uint32_t argCount, NPVariant *result) {
  BOOLEAN_TO_NPVARIANT(false,*result);

  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* data= NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    int nLen = NPVARIANT_TO_STRING(args[0]).UTF8Length+1;
    gLog.WriteLog("data",data);
    if (OpenClipboard(m_Plugin->m_Hwnd))
    {
      EmptyClipboard();
      TCHAR* pData = new TCHAR[nLen];
      MultiByteToWideChar(CP_UTF8,0,data,-1,pData,nLen);
      HANDLE h = GlobalAlloc(GMEM_MOVEABLE,nLen*2);
      if (h) {
        LPVOID pMsg = GlobalLock(h);
        wmemcpy((wchar_t*)pMsg,pData,nLen);
        GlobalUnlock(h);
        SetClipboardData(CF_UNICODETEXT,h);
        gLog.WriteLog("Msg","CopyToClipboard");
      }
      delete[] pData;
      BOOLEAN_TO_NPVARIANT(true,*result);
      CloseClipboard();
    }
  }

  return true;
}