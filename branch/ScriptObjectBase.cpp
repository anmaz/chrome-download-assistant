#include "stdafx.h"
#include "ScriptObjectBase.h"

CScriptObjectBase::CScriptObjectBase(void) {
}

CScriptObjectBase::~CScriptObjectBase(void) {
}

void CScriptObjectBase::SetPlugin(CPluginBase* p) {
  m_Plugin = p;
}

bool CScriptObjectBase::HasMethod(NPIdentifier name) {
  bool bRet = false;
  vector<Function_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_FunList.begin();iter != m_FunList.end();iter++) {
    if (strcmp((const char*)iter->szFunName,szName) == 0) {
      bRet = true;
      break;
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool CScriptObjectBase::Invoke(NPIdentifier name,const NPVariant *args, 
                               uint32_t argCount,NPVariant *result) {
  bool bRet = false;
  vector<Function_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_FunList.begin();iter != m_FunList.end();iter++) {
   if (strcmp((const char*)iter->szFunName,szName) == 0) {
     bRet = (this->*(iter->FunPtr))(args,argCount,result);
     break;
   }
  }
  if (szName != NULL)
   NPN_MemFree(szName);
  return bRet;
}

bool CScriptObjectBase::InvokeDefault(const NPVariant *args,uint32_t argCount,
                                      NPVariant *result) {
  return false;
}

bool CScriptObjectBase::HasProperty(NPIdentifier name) {
  bool bRet = false;
  vector<Property_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_PropList.begin();iter != m_PropList.end();iter++) {
    if (strcmp((const char*)iter->szPropName,szName) == 0) {
      bRet = true;
      break;
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool CScriptObjectBase::GetProperty(NPIdentifier name,NPVariant *result) {
  bool bRet = false;
  vector<Property_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_PropList.begin();iter != m_PropList.end();iter++) {
    if (strcmp((const char*)iter->szPropName,szName) == 0) {
      bRet = true;
      *result = iter->value;
      break;
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}

bool CScriptObjectBase::SetProperty(NPIdentifier name,
                                    const NPVariant *value) {
  bool bRet = false;
  vector<Property_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_PropList.begin();iter != m_PropList.end();iter++) {
    if (strcmp((const char*)iter->szPropName,szName) == 0) {
      bRet = true;
      iter->value = *value;
      break;
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}
bool CScriptObjectBase::RemoveProperty(NPIdentifier name) {
  bool bRet = false;
  vector<Property_Item>::iterator iter;
  char* szName = NPN_UTF8FromIdentifier(name);
  for (iter = m_PropList.begin();iter != m_PropList.end();iter++) {
    if (strcmp((const char*)iter->szPropName,szName) == 0) {
      bRet = true;
      m_PropList.erase(iter);
      break;
    }
  }
  if (szName != NULL)
    NPN_MemFree(szName);
  return bRet;
}
bool CScriptObjectBase::Enumerate(NPIdentifier **value,uint32_t *count) {
//  *count = m_PropList.size() + m_FunList.size();
  return true;
}

void CScriptObjectBase::AddProperty(Property_Item& item) {
  m_PropList.push_back(item);
}

void CScriptObjectBase::AddFunction(Function_Item& item) {
  m_FunList.push_back(item);
}