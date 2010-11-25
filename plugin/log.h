#pragma once

#include <stdio.h>
#include <windows.h>

<<<<<<< HEAD
class Log
{
=======
class Log {
>>>>>>> refactor
public:
  Log(void);
  ~Log(void);

  bool OpenLog(LPCSTR header);
  bool WriteLog(LPCSTR title, LPCSTR contents);
  bool CloseLog();

private:
  FILE* file_;
  char buffer_[2048];
  SYSTEMTIME time_;
};