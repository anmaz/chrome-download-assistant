#include "stdafx.h"
#include "MiniFlashGetScriptObject.h"
#include "Log.h"
#include "utils.h"

extern CLog gLog;

const TCHAR* kMiniFlashGetProgID = L"BHO.IFlashGetNetscape";

CMiniFlashGetScriptObject::CMiniFlashGetScriptObject(void) {
}

CMiniFlashGetScriptObject::~CMiniFlashGetScriptObject(void) {
}

NPObject* CMiniFlashGetScriptObject::Allocate(NPP npp, NPClass *aClass) {
  CMiniFlashGetScriptObject* pRet = new CMiniFlashGetScriptObject;
  char szLog[256];
  sprintf_s(szLog,"CMiniFlashGetScriptObject this=%ld",pRet);
  gLog.WriteLog("Allocate",szLog);
  if (pRet != NULL) {
    pRet->SetPlugin((CPluginBase*)npp->pdata);
    Function_Item item;
    strcpy_s(item.szFunName,"AddLink");
    item.FunPtr = ON_INVOKEHELPER(&CMiniFlashGetScriptObject::AddLink);
    pRet->AddFunction(item);
    strcpy_s(item.szFunName,"DownloadAll");
    item.FunPtr = ON_INVOKEHELPER(&CMiniFlashGetScriptObject::DownloadAll);
    pRet->AddFunction(item);
  }
  return pRet;
}

void CMiniFlashGetScriptObject::Deallocate() {
  char szLog[256];
  sprintf_s(szLog,"CMiniFlashGetScriptObject this=%ld",this);
  gLog.WriteLog("Deallocate",szLog);
  delete this;
}

void CMiniFlashGetScriptObject::Invalidate() {

}

bool CMiniFlashGetScriptObject::Construct(const NPVariant *args,uint32_t argCount,
                                      NPVariant *result) {
  return true;
}

bool CMiniFlashGetScriptObject::CheckObject(const NPVariant* args,uint32_t argCount,
                                            NPVariant* result) {
  BOOLEAN_TO_NPVARIANT(false,*result);

  CLSID clsid;
  HRESULT hr = CLSIDFromProgID(kMiniFlashGetProgID,&clsid);
  TCHAR* pClssID;
  StringFromCLSID(clsid,&pClssID);
  _bstr_t bstr(pClssID);
  gLog.WriteLog("CLSIDFromProgID",bstr);
  if (SUCCEEDED(hr)) {
    BOOLEAN_TO_NPVARIANT(true,*result);
  }

  return true;
}

bool CMiniFlashGetScriptObject::AddLink(const NPVariant* args,uint32_t argCount,
                                        NPVariant* result) {
  if (argCount != 3 || !NPVARIANT_IS_STRING(args[0]) ||
    !NPVARIANT_IS_STRING(args[1]) || !NPVARIANT_IS_STRING(args[2]))
    return false;

  CComPtr<IDispatch> dispatch;

  if (FAILED(dispatch.CoCreateInstance(kMiniFlashGetProgID, NULL)) &&
    FAILED(dispatch.CoCreateInstance(kMiniFlashGetProgID, NULL)))
    return false;

  OLECHAR* name = L"AddUrl";
  DISPID dispid;
  if (FAILED(dispatch->GetIDsOfNames(
    IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
    return false;

  _bstr_t referrer = NPVARIANT_TO_STRING(args[2]).UTF8Characters;
  _bstr_t comments = Utils::Utf8ToUnicode((char*)
    NPVARIANT_TO_STRING(args[1]).UTF8Characters);
  _bstr_t url = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

  VARIANT v[3];
  v[0].vt = v[1].vt = v[2].vt = VT_BSTR;
  v[2].bstrVal = url;
  v[1].bstrVal = comments;
  v[0].bstrVal = referrer;

  if (FAILED(dispatch.InvokeN(dispid, v, 3, NULL)))
    return false;

  return true;
}

bool CMiniFlashGetScriptObject::DownloadAll(const NPVariant* args,uint32_t argCount,
                                            NPVariant* result) {
  if (argCount == 0 || !NPVARIANT_IS_STRING(args[0]) ||
    !NPVARIANT_IS_INT32(args[argCount - 1]))
    return false;

  // build array
  SAFEARRAY *psa = Utils::CreateArray(argCount - 1);
  COleVariant var;
  for (long i = 0 ; i < long(argCount - 1) ; i++) {
    if (NPVARIANT_IS_STRING(args[i])) {
      var.SetString(
        Utils::Utf8ToUnicode(
        (char*)NPVARIANT_TO_STRING(args[i]).UTF8Characters),
        VT_BSTR);
      SafeArrayPutElement(psa, &i, &var);
    } else {
      break;
    }
  }
  VARIANT vList;
  vList.vt = VT_VARIANT|VT_ARRAY|VT_BYREF;
  vList.pparray = &psa;

  CComPtr<IDispatch> dispatch;

  if (!FAILED(dispatch.CoCreateInstance(kMiniFlashGetProgID, NULL))) {
    OLECHAR* name = L"AddAll";
    DISPID dispid;
    if (FAILED(dispatch->GetIDsOfNames(
      IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid)))
      return false;

    _bstr_t referrer = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    VARIANT v[5];
    v[4].vt = VT_VARIANT|VT_BYREF;
    v[0].vt = v[1].vt = v[2].vt = v[3].vt = VT_BSTR;
    v[4].pvarVal = &vList;
    v[0].bstrVal = L"0";
    v[1].bstrVal = L"";
    v[2].bstrVal = L"FlashGetMini";
    v[3].bstrVal = referrer;

    if (FAILED(dispatch.InvokeN(dispid, v, 5, NULL)))
      return false;
  }
  SafeArrayDestroy(psa);

  return true;
}