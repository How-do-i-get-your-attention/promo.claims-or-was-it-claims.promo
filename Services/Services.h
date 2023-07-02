//C:\Windows\System32\PCOrCP\Services.exe
#pragma once
#include <tuple>
#include <vector>
#include <tuple>
#include <thread>
#include <algorithm>
#include <tchar.h>
#include <Windows.h>
#define AppName L"PCOrCP"
using namespace std;
/*Windows Services*/
extern SERVICE_STATUS_HANDLE serviceStatusHandle;
extern SERVICE_STATUS serviceStatus;
void SetServiceState(const DWORD Value);
VOID WINAPI ControlHandler(DWORD dwControl);
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
int wmain(int argc, char* argv[]);