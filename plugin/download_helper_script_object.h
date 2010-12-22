#ifndef DOWNLOAD_HELPER_SCRIPT_OBJECT_H_
#define	DOWNLOAD_HELPER_SCRIPT_OBJECT_H_

#include <string>
#include "script_object_base.h"

using namespace std;

class DownloadHelperScriptObject :public ScriptObjectBase {
public:
  DownloadHelperScriptObject();
  virtual ~DownloadHelperScriptObject();

  static NPObject* Allocate(NPP npp, NPClass *aClass);

  void Deallocate();
  void Invalidate();
  bool Construct(const NPVariant *args,uint32_t argCount,
                 NPVariant *result);

  bool CreateObject(const NPVariant *args, uint32_t argCount,
                    NPVariant *result);

  bool CheckObject(const NPVariant *args, uint32_t argCount,
                   NPVariant *result);

  bool CopyToClipboard(const NPVariant *args, uint32_t argCount,
                       NPVariant *result);

  bool Download(const NPVariant *args, uint32_t argCount,
                NPVariant *result);

  void set_execute_file(const char* name) { execute_file_ = name; }

private:
  string execute_file_;

};

#endif	/* DOWNLOADHELPERSCRIPTOBJECT_H */

