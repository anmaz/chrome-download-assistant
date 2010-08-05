#pragma once
#include "scriptobjectbase.h"

class CMiniFlashGetScriptObject :
  public CScriptObjectBase
{
public:
  CMiniFlashGetScriptObject(void);
  virtual ~CMiniFlashGetScriptObject(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  virtual void Deallocate();
  virtual void Invalidate();
  virtual bool Construct(const NPVariant *args,uint32_t argCount,
    NPVariant *result);

  bool static CheckObject(const NPVariant* args,uint32_t argCount, NPVariant* result);
  bool AddLink(const NPVariant* args,uint32_t argCount, NPVariant* result);
  bool DownloadAll(const NPVariant* args,uint32_t argCount, NPVariant* result);

};
