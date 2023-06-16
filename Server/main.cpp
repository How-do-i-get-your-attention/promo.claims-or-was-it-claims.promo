#include "PCOrCP.h"
#pragma warning(disable: 26444)
int PCOrCP::Argc = 0;
wchar_t** PCOrCP::Argv = nullptr;
wchar_t** PCOrCP::Envp = nullptr;
PCOrCP::PCOrCP(int argc, wchar_t* argv[], wchar_t* envp[]) {
	PCOrCP::Argc = argc;
	PCOrCP::Argv = argv;
	PCOrCP::Envp = envp;
	PCOrCP::Init();
}
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]){PCOrCP(argc, argv, envp);return 0;}
