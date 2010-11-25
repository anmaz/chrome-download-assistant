#pragma once
#include "plugin_base.h"
#include "script_object_base.h"

<<<<<<< HEAD
class ComPlugin :
  public PluginBase
{
=======
class ComPlugin : public PluginBase {
>>>>>>> refactor
public:
  ComPlugin(void);
  virtual ~ComPlugin(void);

<<<<<<< HEAD
  NPError Init(NPP instance,uint16_t mode, int16_t argc, char* argn[],
=======
  NPError Init(NPP instance, uint16_t mode, int16_t argc, char* argn[],
>>>>>>> refactor
               char* argv[], NPSavedData* saved);
  NPError UnInit(NPSavedData** save);
  NPError GetValue(NPPVariable variable, void *value);

  static PluginBase* CreateObject() { return new ComPlugin; }

private:
  ScriptObjectBase* scriptobject_;

};
