#pragma once
#include "scriptobjectbase.h"

class CComObjectWapper :
  public CScriptObjectBase
{
public:
  CComObjectWapper(void);
  virtual ~CComObjectWapper(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  virtual void Deallocate();
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name,const NPVariant *args,
                      uint32_t argCount,NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args,uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name,NPVariant *result);
  virtual bool SetProperty(NPIdentifier name,const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);
  virtual bool Enumerate(NPIdentifier **value,uint32_t *count);
  virtual bool Construct(const NPVariant *args,uint32_t argCount,
                         NPVariant *result);

  friend class CComObjectFactory;

private:
  IDispatch* m_pDisp;
};
