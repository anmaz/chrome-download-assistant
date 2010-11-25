#pragma once
#include "script_object_base.h"

<<<<<<< HEAD
class ComObjectWapper : public ScriptObjectBase
{
=======
class ComObjectWapper : public ScriptObjectBase {
>>>>>>> refactor
public:
  ComObjectWapper(void);
  virtual ~ComObjectWapper(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  virtual void Deallocate();
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);
  virtual bool Enumerate(NPIdentifier **value, uint32_t *count);
  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result);

  friend class ComObjectFactory;

private:
  IDispatch* disp_pointer_;
};
