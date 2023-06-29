#include "Test.h"
#include <iostream>

#include <Windows.h>
#include <tuple>
#include <tuple>
#include <vector>
#include <tuple>
#include <Windows.h>
#include <string>
#include <guiddef.h>
#include <random>
#include <thread>


void(*InitModule)(const std::string&);
HMODULE(*GetModule)(const std::string&);
void(*RemoveModule)(const std::string&);
extern "C" __declspec(dllexport) void Init(std::vector<std::tuple<std::string, std::string, HMODULE>>&modules, void(*initModule)(const std::string&), HMODULE(*getModule)(const std::string&), void(*removeModule)(const std::string&))
{
    std::cout << "hallo world" << std::endl;
    //RemoveModule("Test.dll");
}
extern "C" __declspec(dllexport) void Play() {}
extern "C" __declspec(dllexport) void Pause() {}
extern "C" __declspec(dllexport) void Stop() {}

