#pragma once
#include "scriptobjectbase.h"

class CComObjectFactory :
  public CScriptObjectBase
{
public:
  CComObjectFactory(void);
  ~CComObjectFactory(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  void Deallocate();
  void Invalidate();
  bool Construct(const NPVariant *args,uint32_t argCount,
                 NPVariant *result);

  bool CreateObject(const NPVariant *args,uint32_t argCount,
                    NPVariant *result);

  bool CheckObject(const NPVariant *args,uint32_t argCount,
                   NPVariant *result);

  bool CopyToClipboard(const NPVariant *args,uint32_t argCount,
                       NPVariant *result);

};
