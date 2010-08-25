#pragma once
#include "script_object_base.h"

class MiniflashgetScriptObject :
  public ScriptObjectBase
{
public:
  MiniflashgetScriptObject(void);
  virtual ~MiniflashgetScriptObject(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  virtual void Deallocate();
  virtual void Invalidate();
  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result);

  bool static CheckObject(const NPVariant* args, uint32_t argCount, 
                          NPVariant* result);
  bool AddLink(const NPVariant* args, uint32_t argCount, 
               NPVariant* result);
  bool DownloadAll(const NPVariant* args, uint32_t argCount, 
                   NPVariant* result);

};
