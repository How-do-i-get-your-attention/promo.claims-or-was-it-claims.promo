#pragma once
#include <tuple>
#include <vector>
#include <tuple>
#include <Windows.h>
#include <string>
#include <guiddef.h>
#include <random>
#include <thread>
#pragma warning( disable : 4309 )
using namespace std;
/*
	Vector for all modules thats loadet
*/
extern vector<tuple<string, string, HMODULE>> Modules;
/*
	voidFunc that need to be link to the dll control by ControlHandler
	Play : extern "C" __declspec(dllexport) void Play()
	Pause : extern "C" __declspec(dllexport) void Pause()
	Stop : extern "C" __declspec(dllexport) void Stop()

*/
typedef void (*voidFunc)();
/*
	InitModule install a path in the work folder where Services.exe is
	it start extern "C" __declspec(dllexport) void Init(std::vector<std::tuple<std::string, std::string, HMODULE>>&modules, void(*initModule)(const std::string&), HMODULE(*getModule)(const std::string&), void(*removeModule)(const std::string&)) whit out wait.

*/
void InitModule(const std::string& path);
/*
	Remove a path
*/
void RemoveModule(const std::string& path);
/*
	Get module to get modul betwenn modules
*/
HMODULE GetModule(const std::string& path);
extern SERVICE_STATUS_HANDLE serviceStatusHandle;
extern SERVICE_STATUS serviceStatus;
void SetServiceState(const DWORD Value);
VOID WINAPI ControlHandler(DWORD dwControl);
/*
	   InitModule("PCOrCP.dll");
	   check if service not stop 
	   if stop call all Stop in every module.
*/
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
/*
	First time start up run  a clarn for _temp_ folder. and strart the services
*/
int wmain(int argc, char* argv[]);