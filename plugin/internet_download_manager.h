#pragma once

#include <map>
#include <string>

#include "downloader_script_object.h"

class InternetDownloadManager : public DownloaderScriptObject {
private:
  InternetDownloadManager(void) {}
  virtual ~InternetDownloadManager(void) {}

public:
  static NPObject* Allocate(NPP npp, NPClass *aClass);
  void Deallocate();

  static bool CheckObject();

  bool Download(const NPVariant *args, uint32_t argCount,
                NPVariant *result);

  void InitHandler();

private:
  static TCHAR idm_exe_path[MAX_PATH];

};
