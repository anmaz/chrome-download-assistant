#pragma once

#include <map>
#include <string>

#include "script_object_base.h"
#import "idmantypeinfo.tlb"

using namespace std;
using namespace IDManLib;

class InternetDownloadManager : public ScriptObjectBase {
public:
  InternetDownloadManager(void);
  virtual ~InternetDownloadManager(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass);

  void Deallocate();
  void Invalidate();
  bool Construct(const NPVariant *args,uint32_t argCount,
                 NPVariant *result);

  bool Download(const NPVariant *args, uint32_t argCount,
                NPVariant *result);
  bool DownloadAll(const NPVariant *args, uint32_t argCount,
                   NPVariant *result);

private:
  ICIDMLinkTransmitter2* disp_pointer_;

};
