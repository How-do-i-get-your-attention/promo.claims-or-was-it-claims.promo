#pragma once
#include <tuple>
#include <vector>
#include <tuple>
#include <Windows.h>
#include <string>
#include <guiddef.h>
#include <random>
#include <thread>
#include <fstream>
#pragma warning( disable : 4309 )
using namespace std;


extern SERVICE_STATUS_HANDLE serviceStatusHandle;
extern SERVICE_STATUS serviceStatus;
void SetServiceState(const DWORD Value);
VOID WINAPI ControlHandler(DWORD dwControl);

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

int wmain(int argc, char* argv[]);