#include "../Program/PCOrCP.h"
#pragma warning(disable : 4190)
extern "C" __declspec(dllexport) PCOrCP::Program Init(wchar_t* envp[], wchar_t* argv[], PCOrCP::RunAs RunAsValue) {
    return PCOrCP::Program(envp, argv, RunAsValue);
}