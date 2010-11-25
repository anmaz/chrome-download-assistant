#pragma once

#include <windows.h>

#include "npfunctions.h"

class Utils {
public:
  static TCHAR* Utf8ToUnicode(char *utf8);
  static SAFEARRAY* CreateArray(uint32_t length);
  static VARIANT ToVariant(SAFEARRAY* psa);
  static HRESULT GetCom(REFCLSID rclsid, REFIID riid, __deref_out LPVOID* ppv);
};
