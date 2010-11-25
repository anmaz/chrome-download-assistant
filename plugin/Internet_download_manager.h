#pragma once
#include "script_object_base.h"
#include <string> 
#include <map>

using namespace std;

class InternetDownloadManager : public ScriptObjectBase {
public:
  InternetDownloadManager(void);
  virtual ~InternetDownloadManager(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  void Deallocate();
  void Invalidate();
  bool Construct(const NPVariant *args,uint32_t argCount,
                 NPVariant *result);

  static bool CheckObject();

  bool Download(const NPVariant *args, uint32_t argCount,
                NPVariant *result);

private:
  static TCHAR idm_exe_path[MAX_PATH];

};
