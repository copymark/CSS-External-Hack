#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <time.h>

#include "resource.h"
#include "Error.h"

extern HINSTANCE g_hInstance;
extern HWND g_hWindow;

void add_log(const char * fmt, ...);
