#include "stdafx.h"
#include "script_object_base.h"

void ScriptObjectBase::SetPlugin(PluginBase* p) {
  plugin_ = p;
}

bool ScriptObjectBase::HasMethod(NPIdentifier name) {
  bool ret = false;
  char* method_name = NPN_UTF8FromIdentifier(name);
  if (method_name != NULL) {
    FunctionMap::iterator iter = function_map_.find(method_name);
    if (iter != function_map_.end())
      ret = true;
    NPN_MemFree(method_name);
  }
  return ret;
}

bool ScriptObjectBase::Invoke(NPIdentifier name,const NPVariant *args,
                              uint32_t argCount,NPVariant *result) {
  bool ret = false;
  char* method_name = NPN_UTF8FromIdentifier(name);
  if (method_name != NULL) {
    FunctionMap::iterator iter = function_map_.find(method_name);
    if (iter != function_map_.end())
      ret = (this->*(iter->second.function_pointer))(args, argCount, result);
    NPN_MemFree(method_name);
  }
  return ret;
}

bool ScriptObjectBase::InvokeDefault(const NPVariant *args,uint32_t argCount,
                                     NPVariant *result) {
  return false;
}

bool ScriptObjectBase::HasProperty(NPIdentifier name) {
  bool ret = false;
  char* property_name = NPN_UTF8FromIdentifier(name);
  if (property_name != NULL) {
    PropertyMap::iterator iter = property_map_.find(property_name);
    if (iter != property_map_.end())
      ret = true;
    NPN_MemFree(property_name);
  }
  return ret;
}

bool ScriptObjectBase::GetProperty(NPIdentifier name, NPVariant *result) {
  bool ret = false;
  char* property_name = NPN_UTF8FromIdentifier(name);
  if (property_name != NULL) {
    PropertyMap::iterator iter = property_map_.find(property_name);
    if (iter != property_map_.end()) {
      *result = iter->second.value;
      ret = true;
    }
    NPN_MemFree(property_name);
  }
  return ret;
}

bool ScriptObjectBase::SetProperty(NPIdentifier name,
                                   const NPVariant *value) {
  bool ret = false;
  char* property_name = NPN_UTF8FromIdentifier(name);
  if (property_name != NULL) {
    PropertyMap::iterator iter = property_map_.find(property_name);
    if (iter != property_map_.end()) {
      iter->second.value = *value;
      ret = true;
    }
    NPN_MemFree(property_name);
  }
  return ret;
}
bool ScriptObjectBase::RemoveProperty(NPIdentifier name) {
  bool ret = false;
  char* property_name = NPN_UTF8FromIdentifier(name);
  if (property_name != NULL) {
    PropertyMap::iterator iter = property_map_.find(property_name);
    if (iter != property_map_.end()) {
      property_map_.erase(iter);
      ret = true;
    }
    NPN_MemFree(property_name);
  }
  return ret;
}
bool ScriptObjectBase::Enumerate(NPIdentifier **value, uint32_t *count) {
//  *count = m_PropList.size() + m_FunList.size();
  return true;
}

void ScriptObjectBase::AddProperty(Property_Item& item) {
  property_map_.insert(PropertyMap::value_type(item.property_name, item));
}

void ScriptObjectBase::AddFunction(Function_Item& item) {
  function_map_.insert(FunctionMap::value_type(item.function_name, item));
}
