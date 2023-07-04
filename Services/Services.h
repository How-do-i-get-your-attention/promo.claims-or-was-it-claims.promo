#pragma once
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Just learn it!!
// This is the same in all project. Preview - Features from the Latest C++ 
// Output Directory :..\PCOrCP\Services.exe
// Link Rpcrt4.lib to use UUID
#pragma comment(lib, "Rpcrt4.lib")
#include <iostream>
#include <windows.h>
#include <filesystem>
using namespace std;
using namespace filesystem;
int wmain(int argc, char* argv[]);
SERVICE_STATUS_HANDLE serviceStatusHandle;
SERVICE_STATUS serviceStatus;
VOID WINAPI ControlHandler(DWORD dwControl);
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

// Let's create a manager HMODULE
HMODULE hMODULEManager;
// Thread: First layer service
int wmain(int argc, char* argv[]);
//Go and read https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Services/Services.cpp