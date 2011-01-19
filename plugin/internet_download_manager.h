#pragma once

#include <map>
#include <string>

#include "script_object_base.h"

class InternetDownloadManager : public ScriptObjectBase {
private:
  InternetDownloadManager(void) {}
  virtual ~InternetDownloadManager(void) {}

public:
  static NPObject* Allocate(NPP npp, NPClass *aClass);

  void Deallocate();
  void Invalidate() {}
  bool Construct(const NPVariant *args, uint32_t argCount,
                 NPVariant *result) { return true; }

  static bool CheckObject();

  bool Download(const NPVariant *args, uint32_t argCount,
                NPVariant *result);

  void InitHandler();

private:
  static TCHAR idm_exe_path[MAX_PATH];

};
