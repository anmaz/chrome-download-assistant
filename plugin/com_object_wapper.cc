#include "StdAfx.h"
#include "com_object_wapper.h"
#include "Log.h"
#include <comdef.h>

extern Log g_Log;

ComObjectWapper::ComObjectWapper(void) {
  disp_pointer_ = NULL;
}

ComObjectWapper::~ComObjectWapper(void) {
  if (disp_pointer_ != NULL)
    disp_pointer_->Release();
}

NPObject* ComObjectWapper::Allocate(NPP npp, NPClass *aClass) {
  ComObjectWapper* pRet = new ComObjectWapper;
  char logs[256];
  sprintf_s(logs, "CComObjectWapper this=%ld", pRet);
  g_Log.WriteLog("Allocate", logs);
  if (pRet != NULL) {
    pRet->SetPlugin((PluginBase*)npp->pdata);
  }
  return pRet;
}

void ComObjectWapper::Deallocate() {
  char logs[256];
  sprintf_s(logs, "CComObjectWapper this=%ld", this);
  g_Log.WriteLog("Deallocate", logs);
  delete this;
}

bool ComObjectWapper::HasMethod(NPIdentifier name) {
  bool bRet = false;
  char* szName = NPN_UTF8FromIdentifier(name);
  TCHAR szWName[256];
  TCHAR* p = szWName;
  MultiByteToWideChar(CP_UTF8, 0, szName, -1, szWName, 256);
  DISPID dispid;
  HRESULT hr = disp_pointer_->GetIDsOfNames(IID_NULL, &p, 1, 
                                            LOCALE_USER_DEFAULT, &dispid);
  if (SUCCEEDED(hr))
    bRet = true;
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool ComObjectWapper::Invoke(NPIdentifier name, const NPVariant *args, 
                             uint32_t argCount, NPVariant *result) {
  bool bRet = false;
  char szLog[256];
  char* szName = NPN_UTF8FromIdentifier(name);
  g_Log.WriteLog("Invoke", szName);
  TCHAR szWName[256];
  TCHAR* p = szWName;
  MultiByteToWideChar(CP_UTF8, 0, szName, -1, szWName, 256);
  DISPID dispid;
  HRESULT hr = disp_pointer_->GetIDsOfNames(IID_NULL, &p, 1, 
                                            LOCALE_USER_DEFAULT, &dispid);
  if (SUCCEEDED(hr)) {
    sprintf_s(szLog, "dispid=%ld", dispid);
    g_Log.WriteLog("Invoke", szLog);
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
            g_Log.WriteLog("Param", NPVARIANT_TO_STRING(args[i]).UTF8Characters);
            bstrList[i] = NPVARIANT_TO_STRING(args[i]).UTF8Characters;
            varlist[argCount-i-1].vt = VT_BSTR;
            int nLen = bstrList[i].length()+1;
            if (!MultiByteToWideChar(CP_UTF8, 0,
                NPVARIANT_TO_STRING(args[i]).UTF8Characters, -1,
                bstrList[i],nLen)) {
                sprintf(szLog, "GetLastError=%ld", GetLastError());
                g_Log.WriteLog("Error", szLog);
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
    g_Log.WriteLog("Invoke", "Before Invoke");
    unsigned int nErrIndex = 0;

    hr = disp_pointer_->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, 
                               DISPATCH_METHOD, &params, &varRet, NULL, 
                               &nErrIndex);
    sprintf_s(szLog, 
        "Invoke End,hr=0x%X,GetLastError=%ld,nErrIndex=%ld,varRet.Type=%ld",
        hr, GetLastError(), nErrIndex, varRet.vt);
    g_Log.WriteLog("Invoke", szLog);
    delete[] varlist;
    delete[] bstrList;
    if (SUCCEEDED(hr)) {
      bRet = true;
      switch(varRet.vt) {
        case VT_BOOL:
          BOOLEAN_TO_NPVARIANT(varRet.boolVal, *result);
          break;
        case VT_INT:
        case VT_I4:
          INT32_TO_NPVARIANT(varRet.intVal, *result);
          break;
        case VT_R8:
          DOUBLE_TO_NPVARIANT(varRet.dblVal, *result);
          break;
        case VT_BSTR:
          _bstr_t bstr = varRet.bstrVal;
          int nLen = bstr.length();
          char* p = (char*)NPN_MemAlloc(nLen*4);
          WideCharToMultiByte(CP_UTF8, 0, bstr, -1, p, nLen*4, 0, 0);
          STRINGZ_TO_NPVARIANT(p, *result);
          break;
      }
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool ComObjectWapper::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                    NPVariant *result) {
  return false;
}

bool ComObjectWapper::HasProperty(NPIdentifier name) {
  bool bRet = false;
  return bRet;
}

bool ComObjectWapper::GetProperty(NPIdentifier name, NPVariant *result) {
  bool bRet = false;
  return bRet;
}

bool ComObjectWapper::SetProperty(NPIdentifier name,
                                  const NPVariant *value) {
  bool bRet = false;
  return bRet;
}

bool ComObjectWapper::RemoveProperty(NPIdentifier name) {
  bool bRet = false;
  return bRet;
}

bool ComObjectWapper::Enumerate(NPIdentifier **value, uint32_t *count) {
  //  *count = m_PropList.size() + m_FunList.size();
  return false;
}

void ComObjectWapper::Invalidate() {

}

bool ComObjectWapper::Construct(const NPVariant *args, uint32_t argCount,
                                NPVariant *result) {
  return true;
}
