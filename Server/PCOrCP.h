#pragma once
#include <Windows.h>

#include <iostream>
class PCOrCP
{
private:
	static void Services() {
		SERVICE_TABLE_ENTRY serviceTable[] =
		{
			{ const_cast<LPWSTR>(SERVICE_NAME), ServiceMain },
			{ NULL, NULL }
		};
	}
public:
	static wchar_t** Argv;
	static wchar_t** Envp; 
	static void Init() {
		Services();
	};
	PCOrCP(wchar_t* argv[], wchar_t* envp[]);
	~PCOrCP() {
	
	}
};

