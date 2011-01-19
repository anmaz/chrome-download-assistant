#pragma once

#include "script_object_base.h"

class ComObjectWapper : public ScriptObjectBase {
private:
  ComObjectWapper(void);
  virtual ~ComObjectWapper(void);

public:
  static NPObject* Allocate(NPP npp, NPClass *aClass);

  virtual void Deallocate();
  virtual void Invalidate() {}
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result) { return false; }
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);
  virtual bool Enumerate(NPIdentifier **value, uint32_t *count) { return false; }
  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result) { return true; }

  friend class DownloadHelperScriptObject;

private:
  bool FindFunctionByInvokeKind(const char* name, int invokekind);

private:
  IDispatch* disp_pointer_;
};
