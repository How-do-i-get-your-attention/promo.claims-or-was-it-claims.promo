#include "PCOrCP.h"

using namespace std;

#include <fstream>
#include <iostream>
#include <windows.h>

std::string getCurrentDirectory() {
	char buffer[MAX_PATH];
	DWORD len = GetCurrentDirectoryA(MAX_PATH, buffer);
	if (len != 0) {
		return std::string(buffer, len);
	}
	return "";
}

void writeLog(const std::string& message) {
	std::string currentDir = getCurrentDirectory();

	if (!currentDir.empty()) {
		std::string logFilePath = currentDir + "\\log.txt";
		std::ofstream logFile(logFilePath, std::ios_base::app);

		if (logFile.is_open()) {
			logFile << message << std::endl;
			logFile.close();
		}
	}
}
vector<tuple<string, string, HMODULE>>(*GetModules)();
void(*InitModule)(const string&);
HMODULE(*GetModule)(const string&);
void(*RemoveModule)(const string&);
/*
	Stop=0
	Play=1
	Pause=2
	PrepareForUpdate=3
*/
int Interaction = 1;
#define SOFTWARE_VERSION 3
extern "C" __declspec(dllexport) void Init(vector<tuple<string, string, HMODULE>>(*getModules)(), void(*initModule)(const string&), HMODULE(*getModule)(const string&), void(*removeModule)(const string&))
{
	writeLog("Init");
	GetModules = getModules;
	InitModule = initModule;
	GetModule = getModule;
	RemoveModule = removeModule;
	while (Interaction == 1) {
		vector<tuple<string, string, HMODULE>> Modules = GetModules();
		Modules.erase(std::remove_if(Modules.begin(), Modules.end(), [](const tuple<string, string, HMODULE>& moduleTuple) {return std::get<0>(moduleTuple).find("*") != std::string::npos; }), Modules.end());
		for (const auto& moduleTuple : Modules) {
			string path = std::get<0>(moduleTuple);
			HMODULE module = std::get<2>(moduleTuple);
			HMODULE defaultDLL = LoadLibraryA(path.c_str());
			if (defaultDLL != nullptr) {
				typedef int(*intFunc)();
				intFunc defaultDLLversionFunc = (intFunc)GetProcAddress(defaultDLL, "Version");
				intFunc moduleDLLversionFunc = (intFunc)GetProcAddress(module, "Version");
				if (defaultDLLversionFunc != nullptr && moduleDLLversionFunc != nullptr && defaultDLLversionFunc() > moduleDLLversionFunc()) {
					intFunc moduleDLLPrepareForUpdateFunc = (intFunc)GetProcAddress(module, "PrepareForUpdate");
					if (moduleDLLPrepareForUpdateFunc != nullptr) {
						moduleDLLPrepareForUpdateFunc();
					}
					RemoveModule(path);
				}
				FreeLibrary(defaultDLL);
			}
		}
		Sleep(20000);
	}
	if(Interaction == 2||Interaction==3)
		InitModule("PCOrCP.dll");
}

extern "C" __declspec(dllexport) void Play() {
	writeLog("Play");
	InitModule("PCOrCP.dll");
}
extern "C" __declspec(dllexport) void Pause() {
	writeLog("Pause");
	Interaction = 2;
}
extern "C" __declspec(dllexport) void Stop() {
	Interaction = 0;
}
extern "C"  __declspec(dllexport) int Version() { return SOFTWARE_VERSION; }
extern "C"  __declspec(dllexport) void PrepareForUpdate() {
	writeLog("PrepareForUpdate");
	Interaction = 3;
}