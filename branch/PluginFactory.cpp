#include "stdafx.h"
#include "PluginFactory.h"
#include "ComPlugin.h"

CPluginFactory::CPluginFactory(void) {
  memset(m_PluginTypeList,sizeof(m_PluginTypeList),0);
  strcpy_s(m_PluginTypeList[0].szMIMEType,"application/x-npdownload");
  m_PluginTypeList[0].constructor = &CComPlugin::CreateObject;
}

CPluginFactory::~CPluginFactory(void) {

}

CPluginBase* CPluginFactory::NewPlugin(NPMIMEType pluginType) {
  CPluginBase* pPlugin = NULL;
  for(int i=0;i<MAX_PLUGIN_TYPE_COUNT;i++) {
    if (m_PluginTypeList[i].szMIMEType == NULL)
      break;
    else if (_stricmp(pluginType,m_PluginTypeList[i].szMIMEType) == 0) {
      pPlugin = (*m_PluginTypeList[i].constructor)();
      break;
    }
  }
  return pPlugin;
}
