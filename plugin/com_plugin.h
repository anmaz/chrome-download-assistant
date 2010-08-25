#pragma once
#include "plugin_base.h"
#include "script_object_base.h"

class ComPlugin :
  public PluginBase
{
public:
  ComPlugin(void);
  virtual ~ComPlugin(void);

  NPError Init(NPP instance,uint16_t mode, int16_t argc, char* argn[],
               char* argv[], NPSavedData* saved);
  NPError UnInit(NPSavedData** save);
  NPError GetValue(NPPVariable variable, void *value);

  static PluginBase* CreateObject() { return new ComPlugin; }

private:
  ScriptObjectBase* scriptobject_;

};
