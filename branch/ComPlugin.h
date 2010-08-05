#pragma once
#include "pluginbase.h"
#include "ScriptObjectBase.h"

class CComPlugin :
  public CPluginBase
{
public:
  CComPlugin(void);
  virtual ~CComPlugin(void);

  NPError Init(NPP instance,uint16_t mode, int16_t argc, char* argn[],
               char* argv[], NPSavedData* saved);
  NPError UnInit(NPSavedData** save);
  NPError GetValue(NPPVariable variable, void *value);

  static CPluginBase* CreateObject() {return new CComPlugin;}

private:
  CScriptObjectBase* m_ScriptObject;

};
