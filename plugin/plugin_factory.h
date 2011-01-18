#ifndef PLUGIN_FACTORY_H_
#define PLUGIN_FACTORY_H_

#include <map>
#include "plugin_base.h"
#include <string>

using namespace std;

typedef PluginBase* (*ConstructorPtr)();

class PluginFactory {
public:
  PluginFactory(void);
  ~PluginFactory(void);

  PluginBase* NewPlugin(NPMIMEType pluginType);

private:
  struct Plugin_Type_Item {
    string mime_type;
    ConstructorPtr constructor;
  };

  typedef map<string, Plugin_Type_Item> PluginTypeMap;

  PluginTypeMap plugin_type_map_;
};

#endif