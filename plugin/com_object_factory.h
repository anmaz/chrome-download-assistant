#pragma once

#include "script_object_base.h"

class ComObjectFactory : public ScriptObjectBase {
private:
  ComObjectFactory(void) {}
  ~ComObjectFactory(void) {}

public:
  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  void Deallocate();
  void Invalidate() {}
  bool Construct(const NPVariant *args, uint32_t argCount,
                 NPVariant *result) { return true; }

  bool CreateObject(const NPVariant *args, uint32_t argCount,
                    NPVariant *result);

  bool CheckObject(const NPVariant *args, uint32_t argCount,
                   NPVariant *result);

  bool CopyToClipboard(const NPVariant *args, uint32_t argCount,
                       NPVariant *result);

  void InitHandler();
};
