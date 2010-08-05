#include "StdAfx.h"
#include "ComPlugin.h"
#include "ComObjectFactory.h"
#include "ScriptObjectFactory.h"
#include "Log.h"

extern CLog gLog;

CComPlugin::CComPlugin(void)
{
}

CComPlugin::~CComPlugin(void)
{
}

NPError CComPlugin::Init(NPP instance, uint16_t mode, int16_t argc,
                         char *argn[], char *argv[], 
                         NPSavedData *saved) {
  m_ScriptObject = NULL;
  instance->pdata = this;
  return CPluginBase::Init(instance,mode,argc,argn,argv,saved);
}

NPError CComPlugin::UnInit(NPSavedData **save) {
  CPluginBase::UnInit(save);
  m_ScriptObject = NULL;
  return NPERR_NO_ERROR;
}

NPError CComPlugin::GetValue(NPPVariable variable, void *value) {
  switch(variable) {
    case NPPVpluginScriptableNPObject:
      if (m_ScriptObject == NULL) {
        m_ScriptObject = CScriptObjectFactory::CreateObject(m_NPP,CComObjectFactory::Allocate);
        gLog.WriteLog("GetValue","GetValue");
      }
      if (m_ScriptObject != NULL) {
        *(NPObject**)value = m_ScriptObject;
      }
      else
        return NPERR_OUT_OF_MEMORY_ERROR;
      break;
    default:
      return NPERR_GENERIC_ERROR;
  }

  return NPERR_NO_ERROR;
}
