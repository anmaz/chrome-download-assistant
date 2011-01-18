#ifndef SCRIPT_OBJECT_FACTORY_H_
#define SCRIPT_OBJECT_FACTORY_H_

#include "script_object_base.h"

class ScriptObjectFactory {
protected:
  ScriptObjectFactory(void) {}
  ~ScriptObjectFactory(void) {}

public:
  static ScriptObjectBase* CreateObject(NPP npp,
                                        NPAllocateFunctionPtr allocate);

private:
  static NPClass npclass_;
};

#endif