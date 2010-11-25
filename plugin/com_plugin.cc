#include "StdAfx.h"
#include "com_plugin.h"
#include "com_object_factory.h"
#include "script_object_factory.h"
#include "Log.h"

extern Log g_Log;

ComPlugin::ComPlugin(void) {
}

ComPlugin::~ComPlugin(void) {
}

NPError ComPlugin::Init(NPP instance, uint16_t mode, int16_t argc,
                        char *argn[], char *argv[], 
                        NPSavedData *saved) {
  scriptobject_ = NULL;
  instance->pdata = this;
  return PluginBase::Init(instance, mode, argc, argn, argv, saved);
}

NPError ComPlugin::UnInit(NPSavedData **save) {
  PluginBase::UnInit(save);
  scriptobject_ = NULL;
  return NPERR_NO_ERROR;
}

NPError ComPlugin::GetValue(NPPVariable variable, void *value) {
  switch(variable) {
    case NPPVpluginScriptableNPObject:
      if (scriptobject_ == NULL) {
        scriptobject_ = ScriptObjectFactory::CreateObject(get_npp(), 
            ComObjectFactory::Allocate);
        g_Log.WriteLog("GetValue","GetValue");
      }
      if (scriptobject_ != NULL) {
        *(NPObject**)value = scriptobject_;
      }
      else
        return NPERR_OUT_OF_MEMORY_ERROR;
      break;
    default:
      return NPERR_GENERIC_ERROR;
  }

  return NPERR_NO_ERROR;
}
