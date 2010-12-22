#ifdef _WINDOWS
#include "stdafx.h"
#elif defined linux
#include <string.h>
#endif

#include "download_helper_plugin.h"
#include "plugin_factory.h"

PluginFactory::PluginFactory(void) {
  memset(plugin_type_list_, 0, sizeof(plugin_type_list_));
  strcpy(plugin_type_list_[0].mime_type, "application/x-npdownload");
  plugin_type_list_[0].constructor = &DownloadHelperPlugin::CreateObject;
}

PluginFactory::~PluginFactory(void) {
}

PluginBase* PluginFactory::NewPlugin(NPMIMEType pluginType) {
  PluginBase* pPlugin = NULL;
  for(int i = 0; i < MAX_PLUGIN_TYPE_COUNT; i++) {
    if (plugin_type_list_[i].mime_type == NULL)
      break;
    else if (strcmp(pluginType, plugin_type_list_[i].mime_type) == 0) {
      pPlugin = (*plugin_type_list_[i].constructor)();
      break;
    }
  }
  return pPlugin;
}
