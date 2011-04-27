#include "download_helper_script_object.h"

#ifdef OS_LINUX
#include <unistd.h>
#include <wait.h>

#include <gtk/gtk.h>

#elif defined OS_WIN
#include <comdef.h>

#include "com_object_wapper.h"
#include "internet_download_manager.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "downloader_script_object.h"
#include "log.h"
#include "script_object_factory.h"
#include "utils.h"

extern Log g_logger;

std::string DownloadHelperScriptObject::download_path_ = "";

NPObject* DownloadHelperScriptObject::Allocate(NPP npp, NPClass *aClass) {
  DownloadHelperScriptObject* script_object = new DownloadHelperScriptObject;
  char logs[256];
  sprintf(logs, "DownloadHelperScriptObject this=%ld", script_object);
  g_logger.WriteLog("Allocate",logs);
  if (script_object != NULL) {
    script_object->set_plugin((PluginBase*)npp->pdata);
  }
  return script_object;
}

void DownloadHelperScriptObject::Deallocate() {
  char logs[256];
  sprintf(logs, "DownloadHelperScriptObject this=%ld", this);
  g_logger.WriteLog("Deallocate",logs);
  delete this;
}

void DownloadHelperScriptObject::InitHandler() {
  FunctionItem item;
  item.function_name = "CreateObject";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CreateObject);
  AddFunction(item);
  item.function_name = "CheckObject";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CheckObject);
  AddFunction(item);
  item.function_name = "CopyToClipboard";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      CopyToClipboard);
  AddFunction(item);
  item.function_name = "OpenDownloadPath";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      OpenDownloadPath);
  AddFunction(item);
  item.function_name = "OpenDownloadFilePath";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      OpenDownloadFilePath);
  AddFunction(item);
  item.function_name = "SetDownloadPath";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      SetDownloadPath);
  AddFunction(item);
  item.function_name = "UpdateDownloadPath";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      UpdateDownloadPath);
  AddFunction(item);
  item.function_name = "GetDefaultDownloadPath";
  item.function_pointer = ON_INVOKEHELPER(&DownloadHelperScriptObject::
      GetDefaultDownloadPath);
  AddFunction(item);
}

bool DownloadHelperScriptObject::CreateObject(const NPVariant* args,
                                              uint32_t argCount,
                                              NPVariant* result) {
  char logs[256];
  NULL_TO_NPVARIANT(*result);
#ifdef OS_WIN
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* pProgID = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    g_logger.WriteLog("ProgID", pProgID);
    if (stricmp(pProgID, "DownlWithIDM.LinkProcessor") == 0) {
      DownloaderScriptObject* pObject =
          (DownloaderScriptObject*)ScriptObjectFactory::
          CreateObject(get_plugin()->get_npp(),
                       InternetDownloadManager::Allocate);
      OBJECT_TO_NPVARIANT(pObject, *result);
      return true;
    }

    utils::Utf8ToUnicode progID(pProgID);
    IDispatch* pInterface;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(progID, &clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid, &pClssID);
    _bstr_t bstr(pClssID);
    g_logger.WriteLog("CLSIDFromProgID", bstr);
    if (SUCCEEDED(hr)) {
      hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IDispatch,
          (LPVOID*)&pInterface);
      g_logger.WriteLog("CreateObject", "CoCreateInstance");
      if (SUCCEEDED(hr)) {
        ComObjectWapper* pObject = (ComObjectWapper*)ScriptObjectFactory::
            CreateObject(get_plugin()->get_npp(), ComObjectWapper::Allocate);
        OBJECT_TO_NPVARIANT(pObject, *result);
        pObject->disp_pointer_ = pInterface;
        sprintf(logs, "pInterface=0x%X,pObject=%ld", pInterface, pObject);
        g_logger.WriteLog("CreateObject", logs);
      } else {
        sprintf(logs, "GetLastError=%ld,hr=0x%X", GetLastError(), hr);
        g_logger.WriteLog("Error", logs);
      }
    }
  }
#elif defined OS_LINUX
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* execute_file = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

    g_logger.WriteLog("ProgID", execute_file);

    DownloaderScriptObject* pObject = (DownloaderScriptObject*)
        ScriptObjectFactory::CreateObject(get_plugin()->get_npp(),
        DownloaderScriptObject::Allocate);
    OBJECT_TO_NPVARIANT(pObject, *result);
    if (pObject)
      pObject->set_execute_file(execute_file);
  }
#endif
  
  return true;
}


bool DownloadHelperScriptObject::CheckObject(const NPVariant* args,
                                             uint32_t argCount,
                                             NPVariant* result) {
  BOOLEAN_TO_NPVARIANT(false, *result);
#ifdef OS_WIN
  if (argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
    const char* prog_id = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
    g_logger.WriteLog("ProgID", prog_id);

    if (stricmp(prog_id, "DownlWithIDM.LinkProcessor") == 0) {
      BOOLEAN_TO_NPVARIANT(InternetDownloadManager::CheckObject(), *result);
      return true;
    }

    utils::Utf8ToUnicode wchar_prog_id(prog_id);
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(wchar_prog_id, &clsid);
    TCHAR* pClssID;
    StringFromCLSID(clsid, &pClssID);
    _bstr_t bstr(pClssID);
    g_logger.WriteLog("CLSIDFromProgID", bstr);
    if (SUCCEEDED(hr)) {
      BOOLEAN_TO_NPVARIANT(true, *result);
    }
  }
#elif defined OS_LINUX
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  const char* downloader_name = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  std::string command = "which ";
  command += downloader_name;
  FILE* p = popen(command.c_str(), "r");
  if (p != NULL) {
    char echo_contents[256] = "";
    int count = fread(echo_contents, 1, 256, p);
    if (count > 0) {
      echo_contents[count] = 0;
    }
    if (strstr(echo_contents, downloader_name) != NULL)
      BOOLEAN_TO_NPVARIANT(true, *result);
    pclose(p);
  }
#endif
  return true;
}

bool DownloadHelperScriptObject::CopyToClipboard(const NPVariant* args,
                                                 uint32_t argCount,
                                                 NPVariant* result) {
  return true;
}

bool DownloadHelperScriptObject::OpenDownloadPath(const NPVariant* args,
                                                  uint32_t argCount,
                                                  NPVariant* result) {
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

#ifdef OS_LINUX
  const char* path = NPVARIANT_TO_STRING(args[0]).UTF8Characters;

  if (fork() == 0) {
    execlp("xdg-open", "xdg-open", path, NULL);
    execlp("gnome-open", "gnome-open", path, NULL);
    exit(1);
  }
#endif
  
  return true;
}

bool DownloadHelperScriptObject::OpenDownloadFilePath(const NPVariant* args,
                                                      uint32_t argCount,
                                                      NPVariant* result) {
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

#ifdef OS_LINUX
  const char* file_name = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  char* path = g_path_get_dirname(file_name);

  if (fork() == 0) {
    execlp("xdg-open", "xdg-open", path, NULL);
    execlp("gnome-open", "gnome-open", path, NULL);
    exit(1);
  }
  g_free(path);
#endif

  return true;
}

bool DownloadHelperScriptObject::SetDownloadPath(const NPVariant* args,
                                                 uint32_t argCount,
                                                 NPVariant* result) {
#ifdef OS_LINUX
  if (argCount < 1)
    return false;
  for (int index = 0; index < argCount; index++)
    if (!NPVARIANT_IS_STRING(args[index]))
      return false;

  const char* path = NULL;
  const char* title = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  if (argCount == 2)
    path = NPVARIANT_TO_STRING(args[1]).UTF8Characters;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      title, NULL,
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
  
  char* ret_value = NULL;
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char* folder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if (folder) {
      int len = strlen(folder);
      ret_value = (char*)NPN_MemAlloc(len + 1);
      memcpy(ret_value, folder, len);
      ret_value[len] = 0;
    }
    g_free(folder);
  } else {
    if (path) {
      int len = strlen(path);
      ret_value = (char*)NPN_MemAlloc(len + 1);
      memcpy(ret_value, path, len);
      ret_value[len] = 0;
    }
  }
  gtk_widget_destroy(dialog);
  if (ret_value) {
    download_path_ = ret_value;
    STRINGZ_TO_NPVARIANT(ret_value, *result);
  }

#endif
  
  return true;
}

bool DownloadHelperScriptObject::UpdateDownloadPath(const NPVariant* args,
                                                    uint32_t argCount,
                                                    NPVariant* result) {
  if (argCount != 1 || !NPVARIANT_IS_STRING(args[0]))
    return false;

  const char* path = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
  download_path_ = path;
  
  return true;
}

bool DownloadHelperScriptObject::GetDefaultDownloadPath(const NPVariant* args,
                                                        uint32_t argCount,
                                                        NPVariant* result) {
#ifdef OS_LINUX
  const char* path = g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD);
  if (path == NULL)
    path = g_get_home_dir();
  int length = strlen(path);
  char* copy = (char *)NPN_MemAlloc(length + 1);
  memcpy(copy, path, length);
  copy[length] = 0;
  STRINGN_TO_NPVARIANT(copy, length, *result);
#endif
  
  return true;
}