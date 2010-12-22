#ifndef PLUGIN_BASE_H_
#define PLUGIN_BASE_H_

#include "npapi.h"

#ifdef linux
#include <X11/X.h>
#endif

class PluginBase {
public:
  PluginBase(void);
  virtual ~PluginBase(void);

  virtual NPError Init(NPP instance, uint16_t mode, int16_t argc, char* argn[],
                       char* argv[], NPSavedData* saved);
  virtual NPError UnInit(NPSavedData** save);
  virtual NPError SetWindow(NPWindow* window);
  virtual NPError NewStream(NPMIMEType type, NPStream* stream, NPBool seekable,
                            uint16_t* stype);
  virtual NPError DestroyStream(NPStream* stream, NPReason reason);
  virtual int32_t WriteReady(NPStream* stream);
  virtual int32_t Write(NPStream* stream, int32_t offset, int32_t len,
                        void* buffer);
  virtual void StreamAsFile(NPStream* stream, const char* fname);
  virtual void Print(NPPrint* platformPrint);
  virtual int16_t HandleEvent(void* event);
  virtual void URLNotify(const char* url, NPReason reason, void* notifyData);
  virtual NPError GetValue(NPPVariable variable, void *value);
  virtual NPError SetValue(NPNVariable variable, void *value);

  NPP get_npp() const { return npp_; }
  void set_npp(NPP npp) { npp_ = npp; }

#ifdef _WINDOWS
  HWND get_hwnd() const { return hwnd_; }
  void set_hwnd(HWND hwnd) { hwnd_ = hwnd; }
#elif defined linux
  Window get_window_id() const { return wid_; }
  void set_window_id(Window wid) { wid_ = wid; }
#endif
  
private:
  NPP npp_;
#ifdef _WINDOWS
  HWND hwnd_;
#elif defined linux
  Window wid_;
#endif
};

#endif