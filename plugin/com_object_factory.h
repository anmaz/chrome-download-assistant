#pragma once
#include "script_object_base.h"

<<<<<<< HEAD
class ComObjectFactory : public ScriptObjectBase
{
=======
class ComObjectFactory : public ScriptObjectBase {
>>>>>>> refactor
public:
  ComObjectFactory(void);
  ~ComObjectFactory(void);

  static NPObject* Allocate(NPP npp, NPClass *aClass); 

  void Deallocate();
  void Invalidate();
<<<<<<< HEAD
  bool Construct(const NPVariant *args,uint32_t argCount,
                 NPVariant *result);

  bool CreateObject(const NPVariant *args,uint32_t argCount,
                    NPVariant *result);

  bool CheckObject(const NPVariant *args,uint32_t argCount,
                   NPVariant *result);

  bool CopyToClipboard(const NPVariant *args,uint32_t argCount,
=======
  bool Construct(const NPVariant *args, uint32_t argCount,
                 NPVariant *result);

  bool CreateObject(const NPVariant *args, uint32_t argCount,
                    NPVariant *result);

  bool CheckObject(const NPVariant *args, uint32_t argCount,
                   NPVariant *result);

  bool CopyToClipboard(const NPVariant *args, uint32_t argCount,
>>>>>>> refactor
                       NPVariant *result);

};
