#include "../Program/PCOrCP.h"
const PCOrCP::RunAs PCOrCP::Program::RunAsValue = PCOrCP::Background;
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    PCOrCP::Program(envp, argv);
    return 0;
}