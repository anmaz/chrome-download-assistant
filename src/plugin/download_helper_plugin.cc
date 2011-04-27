#include "download_helper_plugin.h"

#include "download_helper_script_object.h"
#include "log.h"
#include "script_object_factory.h"

extern Log g_logger;

NPError DownloadHelperPlugin::Init(NPP instance, uint16_t mode, int16_t argc,
                                   char* argn[],char* argv[], 
                                   NPSavedData* saved) {
  g_logger.WriteLog("msg", "DownloadHelperPlugin Init");
  scriptobject_ = NULL;
  instance->pdata = this;
  return PluginBase::Init(instance, mode, argc, argn, argv, saved);
}

NPError DownloadHelperPlugin::UnInit(NPSavedData** save) {
  g_logger.WriteLog("msg", "DownloadHelperPlugin UnInit");
  PluginBase::UnInit(save);
  scriptobject_ = NULL;
  return NPERR_NO_ERROR;
}

NPError DownloadHelperPlugin::GetValue(NPPVariable variable, void *value) {
  switch(variable) {
    case NPPVpluginScriptableNPObject:
      if (scriptobject_ == NULL) {
        scriptobject_ = ScriptObjectFactory::CreateObject(
            get_npp(), DownloadHelperScriptObject::Allocate);
        g_logger.WriteLog("GetValue","GetValue");
      }
      if (scriptobject_ != NULL)
        *(NPObject**)value = scriptobject_;
      else
        return NPERR_OUT_OF_MEMORY_ERROR;
      break;
    case NPPVpluginNeedsXEmbed:
      *(bool*)value = 1;
      break;
    default:
      return NPERR_GENERIC_ERROR;
  }

  return NPERR_NO_ERROR;
}
