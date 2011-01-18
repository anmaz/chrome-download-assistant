#ifndef SCRIPT_OBJECT_BASE_H_
#define SCRIPT_OBJECT_BASE_H_

#include <map>
#include <string>

#include "npapi.h"
#include "npruntime.h"
#include "plugin_base.h"

using namespace std;

class ScriptObjectBase : public NPObject {
public:
  ScriptObjectBase(void) {}
  virtual ~ScriptObjectBase(void) {}

  typedef bool (ScriptObjectBase::*InvokePtr)(const NPVariant *args,
                                              uint32_t argCount,
                                              NPVariant *result);

  struct Function_Item {
    string function_name;
    InvokePtr function_pointer;
  };

  struct Property_Item {
    string property_name;
    NPVariant value;
  };

  virtual void Deallocate() = 0;
  virtual void Invalidate() = 0;
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
                         NPVariant *result) = 0;

  void AddProperty(Property_Item& item);
  void AddFunction(Function_Item& item);

  void SetPlugin(PluginBase* p);

protected:
  PluginBase* plugin_;

private:
  typedef map<string, Function_Item> FunctionMap;
  typedef map<string, Property_Item> PropertyMap;
  FunctionMap function_map_;
  PropertyMap property_map_;
};

#define ON_INVOKEHELPER(_funPtr) \
  static_cast<bool (ScriptObjectBase::*)(const NPVariant *,uint32_t , \
                                         NPVariant *)>(_funPtr)

#endif