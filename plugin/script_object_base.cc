#include "stdafx.h"
#include "script_object_base.h"

bool ScriptObjectBase::HasMethod(NPIdentifier name) {
  bool ret = false;
  char* method_name = NPN_UTF8FromIdentifier(name);
  if (method_name != NULL) {
    FunctionMap::iterator iter = function_map_.find(method_name);
    ret = iter != function_map_.end();
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

bool ScriptObjectBase::HasProperty(NPIdentifier name) {
  bool ret = false;
  char* property_name = NPN_UTF8FromIdentifier(name);
  if (property_name != NULL) {
    PropertyMap::iterator iter = property_map_.find(property_name);
   ret = iter != property_map_.end();
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

void ScriptObjectBase::AddProperty(Property_Item& item) {
  PropertyMap::iterator iter = property_map_.find(item.property_name);
  if (iter != property_map_.end())
    return;

  property_map_.insert(PropertyMap::value_type(item.property_name, item));
}

void ScriptObjectBase::AddFunction(Function_Item& item) {
  FunctionMap::iterator iter = function_map_.find(item.function_name);
  if (iter != function_map_.end())
    return;

  function_map_.insert(FunctionMap::value_type(item.function_name, item));
}
