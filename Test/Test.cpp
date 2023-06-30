#include <string>
#include <thread>
#include <Windows.h>
#include <tuple>
#include <vector>
#include <tuple>
#include <Windows.h>

using namespace std;
vector<tuple<string, string, HMODULE>>(*GetModules);
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
extern "C" __declspec(dllexport) void Init(vector<tuple<string, string, HMODULE>>(*getModules), void(*initModule)(const string&), HMODULE(*getModule)(const string&), void(*removeModule)(const string&))
{
	GetModules = getModules;
	InitModule = initModule;
	GetModule = getModule;
	RemoveModule = removeModule;
	thread theadPlay(Play);
	theadPlay.detach();
	//Run forever with Interaction
	while (Interaction != 0)
		Sleep(5000);
	//Otherwise, RemoveModule will be called automatically
}
extern "C" __declspec(dllexport) void Play() {
	Interaction = 1;
}
extern "C" __declspec(dllexport) void Pause() {
	Interaction = 2;
}
extern "C" __declspec(dllexport) void Stop() {
	Interaction = 0;
}

#define SOFTWARE_VERSION 0
extern "C"  __declspec(dllexport) int Version() { return SOFTWARE_VERSION; }
extern "C"  __declspec(dllexport) void PrepareForUpdate() {
	Interaction = 3;
}