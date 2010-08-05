#include "stdafx.h"
#include "Log.h"

CLog::CLog(void)
{
  m_File = NULL;
}

CLog::~CLog(void)
{
  if (m_File != NULL)
    CloseLog();
}

bool CLog::OpenLog(LPCSTR header)
{
  if (m_File != NULL)
    return false;

  char szFileName[MAX_PATH];
  GetLocalTime(&m_Time);
  sprintf_s(szFileName,"D:\\Log\\%s_%d%02d%02d_%d.log",
          header,m_Time.wYear,m_Time.wMonth,m_Time.wDay,
          GetProcessId(GetCurrentProcess()));
  m_File = fopen(szFileName,"a");
  if (m_File == NULL)
    return false;
  else
    return true;
}

bool CLog::WriteLog(LPCSTR title, LPCSTR contents)
{
  if (m_File == NULL)
  {
    return false;
  }

  GetLocalTime(&m_Time);
  sprintf_s(m_Buffer,"[%02d:%02d:%02d %03d] [%s] %s\n",
          m_Time.wHour,m_Time.wMinute,m_Time.wSecond,m_Time.wMilliseconds,
    title,contents);
  int nLen = strlen(m_Buffer);
  if (fwrite(m_Buffer,nLen,1,m_File) == 1)
  {
    fflush(m_File);
    return true;
  }
  else
  {
    return false;
  }
}

bool CLog::CloseLog()
{
  if (m_File != NULL) {
    fclose(m_File);
    m_File = NULL;
  }
  return true;
}