#include "../Program/PCOrCP.h"
const PCOrCP::RunAs PCOrCP::Program::RunAsValue = PCOrCP::Library;
PCOrCP::Program ModuleProgram;  
extern "C" __declspec(dllexport) PCOrCP::Program& GetModuleProgram() {
    return ModuleProgram;
}
extern "C" __declspec(dllexport) void Init(wchar_t* envp[], wchar_t* argv[]) {
    ModuleProgram = PCOrCP::Program(envp, argv);
}