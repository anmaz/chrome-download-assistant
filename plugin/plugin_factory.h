#ifndef PLUGIN_FACTORY_H_
#define PLUGIN_FACTORY_H_

#include <map>
#include "plugin_base.h"
#include <string>

typedef PluginBase* (*ConstructorPtr)();

class PluginFactory {
private:
  PluginFactory(void);
  ~PluginFactory(void);

public:
  static void Init();
  static PluginBase* NewPlugin(NPMIMEType pluginType);

public:
  struct Plugin_Type_Item {
    std::string mime_type;
    ConstructorPtr constructor;
  };

  typedef std::map<std::string, Plugin_Type_Item> PluginTypeMap;

private:
  static PluginTypeMap plugin_type_map_;
};

#endif