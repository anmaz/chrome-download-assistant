#pragma once

#include <stdio.h>
#include <windows.h>

class CLog
{
public:
  CLog(void);
  ~CLog(void);

  bool OpenLog(LPCSTR header);
  bool WriteLog(LPCSTR title,LPCSTR contents);
  bool CloseLog();

private:
  FILE* m_File;
  char m_Buffer[2048];
  SYSTEMTIME m_Time;
};