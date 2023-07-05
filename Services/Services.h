#pragma once
// This directive ensures that the header file is included only once in the compilation process

// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Just learn it!!
// This comment indicates that the code is utilizing features from the latest C++ working draft

// Include necessary headers
#include <iostream>      // Provides basic input/output stream functionality
#include <windows.h>     // Provides Windows-specific functions and types
#include <filesystem>    // Provides filesystem-related functionality
#include <algorithm>	 // For find_if
using namespace std;
using namespace filesystem;

// Function declarations
int wmain(int argc, char* argv[]);
VOID WINAPI ControlHandler(DWORD dwControl);
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

// Global variables
SERVICE_STATUS_HANDLE serviceStatusHandle;   // Handle to the service status
SERVICE_STATUS serviceStatus;                // Structure to hold the service status

// Let's create a manager HMODULE
HMODULE hMODULEManager;

// Thread: First layer service
int wmain(int argc, char* argv[]);

// Typedefs for function pointers
typedef void (*ManagerInit)(const std::wstring&, const std::wstring&, HMODULE&);
typedef void (*ManagerHeartbeat)();
typedef void (*ManagerShutdown)();

// Go and read the code at the specified URL
// https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Services/Services.cpp
