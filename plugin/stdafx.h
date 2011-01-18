#ifndef STDAFX_H_
#define STDAFX_H_

#ifdef OS_WIN
#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>

typedef HWND NativeWindow;
#elif defined OS_LINUX
#include <X11/X.h>
typedef Window NativeWindow;
#endif

#endif