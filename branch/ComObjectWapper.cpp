#include "StdAfx.h"
#include "ComObjectWapper.h"
#include "Log.h"
#include <comdef.h>

extern CLog gLog;

CComObjectWapper::CComObjectWapper(void) {
  m_pDisp = NULL;
}

CComObjectWapper::~CComObjectWapper(void) {
  if (m_pDisp != NULL)
    m_pDisp->Release();
}

NPObject* CComObjectWapper::Allocate(NPP npp, NPClass *aClass) {
  CComObjectWapper* pRet = new CComObjectWapper;
  char szLog[256];
  sprintf_s(szLog,"CComObjectWapper this=%ld",pRet);
  gLog.WriteLog("Allocate",szLog);
  if (pRet != NULL) {
    pRet->SetPlugin((CPluginBase*)npp->pdata);
  }
  return pRet;
}

void CComObjectWapper::Deallocate() {
  char szLog[256];
  sprintf_s(szLog,"CComObjectWapper this=%ld",this);
  gLog.WriteLog("Deallocate",szLog);
  delete this;
}

bool CComObjectWapper::HasMethod(NPIdentifier name) {
  bool bRet = false;
  char* szName = NPN_UTF8FromIdentifier(name);
  TCHAR szWName[256];
  TCHAR* p = szWName;
  MultiByteToWideChar(CP_UTF8,0,szName,-1,szWName,256);
  DISPID dispid;
  HRESULT hr = m_pDisp->GetIDsOfNames(IID_NULL,&p,1,LOCALE_USER_DEFAULT,&dispid);
  if (SUCCEEDED(hr))
    bRet = true;
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool CComObjectWapper::Invoke(NPIdentifier name,const NPVariant *args, 
                               uint32_t argCount,NPVariant *result) {
  bool bRet = false;
  char szLog[256];
  char* szName = NPN_UTF8FromIdentifier(name);
  gLog.WriteLog("Invoke",szName);
  TCHAR szWName[256];
  TCHAR* p = szWName;
  MultiByteToWideChar(CP_UTF8,0,szName,-1,szWName,256);
  DISPID dispid;
  HRESULT hr = m_pDisp->GetIDsOfNames(IID_NULL,&p,1,LOCALE_USER_DEFAULT,
                                      &dispid);
  if (SUCCEEDED(hr)) {
    sprintf_s(szLog,"dispid=%ld",dispid);
    gLog.WriteLog("Invoke",szLog);
    VARIANT varRet;
    DISPPARAMS params;
    params.cNamedArgs = 0;
    params.rgdispidNamedArgs = 0;
    params.cArgs = argCount;
    VARIANT* varlist = new VARIANT[argCount];
    _bstr_t* bstrList = new _bstr_t[argCount];
    for (int i=0;i<argCount;i++) {
      switch (args[i].type) {
        case NPVariantType_Bool:
          varlist[argCount-i-1].vt = VT_BOOL;
          varlist[argCount-i-1].boolVal = NPVARIANT_TO_BOOLEAN(args[i]);
          break;
        case NPVariantType_Int32:
          varlist[argCount-i-1].vt = VT_INT;
          varlist[argCount-i-1].intVal = NPVARIANT_TO_INT32(args[i]);
          break;
        case NPVariantType_Double:
          varlist[argCount-i-1].vt = VT_R8;
          varlist[argCount-i-1].dblVal = NPVARIANT_TO_DOUBLE(args[i]);
          break;
        case NPVariantType_String:
          {
            gLog.WriteLog("Param",NPVARIANT_TO_STRING(args[i]).UTF8Characters);
            bstrList[i] = NPVARIANT_TO_STRING(args[i]).UTF8Characters;
            varlist[argCount-i-1].vt = VT_BSTR;
            int nLen=bstrList[i].length()+1;
            if (!MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[i]).UTF8Characters,-1,
              bstrList[i],nLen)) {
                sprintf(szLog,"GetLastError=%ld",GetLastError());
                gLog.WriteLog("Error",szLog);
            }
            varlist[argCount-i-1].bstrVal = bstrList[i];
          }
          break;
        case NPVariantType_Object:
          {
            NPObject* pObject = NPVARIANT_TO_OBJECT(args[i]);
          }
          break;
      }
    }
    params.rgvarg = varlist;
    gLog.WriteLog("Invoke","Before Invoke");
    unsigned int nErrIndex=0;

    hr = m_pDisp->Invoke(dispid,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,
                         &params,&varRet,NULL,&nErrIndex);
    sprintf_s(szLog,"Invoke End,hr=0x%X,GetLastError=%ld,nErrIndex=%ld,varRet.Type=%ld",
              hr,GetLastError(),nErrIndex,varRet.vt);
    gLog.WriteLog("Invoke",szLog);
    delete[] varlist;
    delete[] bstrList;
    if (SUCCEEDED(hr)) {
      bRet = true;
      switch(varRet.vt) {
        case VT_BOOL:
          BOOLEAN_TO_NPVARIANT(varRet.boolVal,*result);
          break;
        case VT_INT:
        case VT_I4:
          INT32_TO_NPVARIANT(varRet.intVal,*result);
          break;
        case VT_R8:
          DOUBLE_TO_NPVARIANT(varRet.dblVal,*result);
          break;
        case VT_BSTR:
          _bstr_t bstr = varRet.bstrVal;
          int nLen = bstr.length();
          char* p = (char*)NPN_MemAlloc(nLen*4);
          WideCharToMultiByte(CP_UTF8,0,bstr,-1,p,nLen*4,0,0);
          STRINGZ_TO_NPVARIANT(p,*result);
          break;
      }
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool CComObjectWapper::InvokeDefault(const NPVariant *args,uint32_t argCount,
                                     NPVariant *result) {
  return false;
}

bool CComObjectWapper::HasProperty(NPIdentifier name) {
  bool bRet = false;
  return bRet;
}

bool CComObjectWapper::GetProperty(NPIdentifier name,NPVariant *result) {
  bool bRet = false;
  return bRet;
}

bool CComObjectWapper::SetProperty(NPIdentifier name,
                                   const NPVariant *value) {
  bool bRet = false;
  return bRet;
}

bool CComObjectWapper::RemoveProperty(NPIdentifier name) {
  bool bRet = false;
  return bRet;
}

bool CComObjectWapper::Enumerate(NPIdentifier **value,uint32_t *count) {
  //  *count = m_PropList.size() + m_FunList.size();
  return false;
}

void CComObjectWapper::Invalidate() {

}

bool CComObjectWapper::Construct(const NPVariant *args,uint32_t argCount,
                                 NPVariant *result) {
  return true;
}
