#include "../Program/PCOrCP.h"
#pragma warning(disable : 26444)
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    PCOrCP::Program(envp, argv, PCOrCP::Background);
    return 0;
}