#include "stdafx.h"
#include "Log.h"

<<<<<<< HEAD
Log::Log(void)
{
  file_ = NULL;
}

Log::~Log(void)
{
=======
Log::Log(void) {
  file_ = NULL;
}

Log::~Log(void) {
>>>>>>> refactor
  if (file_ != NULL)
    CloseLog();
}

<<<<<<< HEAD
bool Log::OpenLog(LPCSTR header)
{
=======
bool Log::OpenLog(LPCSTR header) {
>>>>>>> refactor
  if (file_ != NULL)
    return false;

  char filename[MAX_PATH];
  GetLocalTime(&time_);
<<<<<<< HEAD
  sprintf_s(filename, "D:\\Log\\%s_%d%02d%02d_%d.log",
            header, time_.wYear, time_.wMonth, time_.wDay,
            GetProcessId(GetCurrentProcess()));
=======
  sprintf_s(filename, "C:\\Log\\%s_%d%02d%02d_%d.log",
            header, time_.wYear, time_.wMonth, time_.wDay,
            GetCurrentProcessId());
>>>>>>> refactor
  file_ = fopen(filename, "a");
  if (file_ == NULL)
    return false;
  else
    return true;
}

<<<<<<< HEAD
bool Log::WriteLog(LPCSTR title, LPCSTR contents)
{
  if (file_ == NULL)
  {
=======
bool Log::WriteLog(LPCSTR title, LPCSTR contents) {
  if (file_ == NULL) {
>>>>>>> refactor
    return false;
  }

  GetLocalTime(&time_);
  sprintf_s(buffer_, "[%02d:%02d:%02d %03d] [%s] %s\n",
            time_.wHour, time_.wMinute, time_.wSecond, time_.wMilliseconds,
            title, contents);
  int nLen = strlen(buffer_);
<<<<<<< HEAD
  if (fwrite(buffer_, nLen, 1, file_) == 1)
  {
    fflush(file_);
    return true;
  }
  else
  {
=======
  if (fwrite(buffer_, nLen, 1, file_) == 1) {
    fflush(file_);
    return true;
  } else {
>>>>>>> refactor
    return false;
  }
}

<<<<<<< HEAD
bool Log::CloseLog()
{
=======
bool Log::CloseLog() {
>>>>>>> refactor
  if (file_ != NULL) {
    fclose(file_);
    file_ = NULL;
  }
  return true;
}