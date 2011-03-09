#ifndef DOWNLOADER_SCRIPT_OBJECT_H
#define DOWNLOADER_SCRIPT_OBJECT_H

#include "script_object_base.h"
#include <string>

class DownloaderScriptObject : public ScriptObjectBase {
public:
  DownloaderScriptObject(void) {}
  virtual ~DownloaderScriptObject(void) {}
  
  static NPObject* Allocate(NPP npp, NPClass *aClass);

  void Deallocate();
  virtual void Invalidate() {}
  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result) { return true; }

  virtual bool Download(const NPVariant *args, uint32_t argCount,
                        NPVariant *result);

  void set_execute_file(const char* name) { execute_file_ = name; }

  void InitHandler();

private:
  std::string execute_file_;
  
};

#endif
