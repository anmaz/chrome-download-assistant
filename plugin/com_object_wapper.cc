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
<<<<<<< HEAD
  sprintf_s(logs, "CComObjectWapper this=%ld", pRet);
=======
  sprintf(logs, "ComObjectWapper this=%ld", pRet);
>>>>>>> refactor
  g_Log.WriteLog("Allocate", logs);
  if (pRet != NULL) {
    pRet->SetPlugin((PluginBase*)npp->pdata);
  }
  return pRet;
}

void ComObjectWapper::Deallocate() {
  char logs[256];
<<<<<<< HEAD
  sprintf_s(logs, "CComObjectWapper this=%ld", this);
=======
  sprintf(logs, "ComObjectWapper this=%ld", this);
>>>>>>> refactor
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
<<<<<<< HEAD
    for (int i=0;i<argCount;i++) {
=======
    for (int i = 0; i < argCount; i++) {
>>>>>>> refactor
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
<<<<<<< HEAD
            int nLen = bstrList[i].length()+1;
=======
            int nLen = bstrList[i].length() + 1;
>>>>>>> refactor
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
<<<<<<< HEAD
          }
          break;
=======
            NPIdentifier id = NPN_GetStringIdentifier("length");
            if (id) {
              NPVariant ret;
              if (NPN_GetProperty(plugin_->get_npp(), pObject, id, &ret) &&
                  NPVARIANT_IS_INT32(ret) && NPVARIANT_TO_INT32(ret) > 0) {
                int array_len = NPVARIANT_TO_INT32(ret);
                sprintf(szLog, "array_len=%d", array_len);
                g_Log.WriteLog("Array", szLog);
                SAFEARRAYBOUND rgsabound[1];
                rgsabound[0].lLbound = 0;
                rgsabound[0].cElements = array_len;
                SAFEARRAY* psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);
                if (psa) {
                  for (int index = 0; index < array_len; index++) {
                    id = NPN_GetIntIdentifier(index);
                    NPN_GetProperty(plugin_->get_npp(), pObject, id, &ret);
                    if (!NPVARIANT_IS_STRING(ret))
                      continue;
                    const char* array_item = NPVARIANT_TO_STRING(ret).UTF8Characters;
                    g_Log.WriteLog("array", array_item);
                    _bstr_t bstr = array_item;
                    MultiByteToWideChar(CP_UTF8, 0, array_item, -1, 
                        bstr, bstr.length()+1);
                    VARIANT var_out;
                    var_out.vt = VT_BSTR;
                    var_out.bstrVal = bstr;
                    SafeArrayPutElement(psa, (long*)&index, &var_out);
                  }
                  varlist[argCount-i-1].vt = VT_ARRAY | VT_VARIANT | VT_BYREF;
                  varlist[argCount-i-1].pparray = new SAFEARRAY*;
                  *varlist[argCount-i-1].pparray = psa;
                }
              }
            }
          }
          break;
        default:
          varlist[argCount-i-1].vt = VT_EMPTY;
          break;
>>>>>>> refactor
      }
    }
    params.rgvarg = varlist;
    g_Log.WriteLog("Invoke", "Before Invoke");
    unsigned int nErrIndex = 0;
<<<<<<< HEAD

=======
  
>>>>>>> refactor
    hr = disp_pointer_->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, 
                               DISPATCH_METHOD, &params, &varRet, NULL, 
                               &nErrIndex);
    sprintf_s(szLog, 
        "Invoke End,hr=0x%X,GetLastError=%ld,nErrIndex=%ld,varRet.Type=%ld",
        hr, GetLastError(), nErrIndex, varRet.vt);
    g_Log.WriteLog("Invoke", szLog);
<<<<<<< HEAD
=======
    for (int i = 0; i < argCount; i++) {
      if (varlist[i].vt == (VT_ARRAY | VT_VARIANT | VT_BYREF)) {
        SafeArrayDestroy(*varlist[i].pparray);
        delete varlist[i].pparray;
      }
    }
>>>>>>> refactor
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
