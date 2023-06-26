#include "../Program/PCOrCP.h"
#pragma warning(disable : 26444)
wchar_t** Argv = nullptr;
wchar_t** Envp = nullptr;
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    Envp = envp;
    Argv = argv;
    Library(true);
    return 0;
}