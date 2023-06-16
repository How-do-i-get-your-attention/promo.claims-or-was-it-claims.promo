#include "PCOrCP.h"
#pragma warning(disable: 26444)
wchar_t** PCOrCP::Argv = nullptr;
wchar_t** PCOrCP::Envp = nullptr;
PCOrCP::PCOrCP( wchar_t* argv[], wchar_t* envp[]) {
	PCOrCP::Argv = argv;
	PCOrCP::Envp = envp;
	PCOrCP::Init();
}
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]){PCOrCP(argv, envp);return 0;}
