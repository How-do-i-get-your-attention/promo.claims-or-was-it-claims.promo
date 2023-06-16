#pragma once
#include <iostream>
class PCOrCP
{
public:
	static int Argc;
	static wchar_t** Argv;
	static wchar_t** Envp; 
	static void Init() {
		std::cout << "Init work perfect" << std::endl; 

	};
	PCOrCP(int argc, wchar_t* argv[], wchar_t* envp[]);
	~PCOrCP() {
	
	}
};

