#pragma once
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Did you learn it!!
// Output Directory: ..\PCOrCP\Services\Manager.dll
// Guideline:
// Open your project in Visual Studio.
// 1. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 2. In the project properties window, navigate to the "Configuration Type" setting under the "General" category.
// 3. Change the "Configuration Type" to "Dynamic Library (.dll)".
// 4. Click the "Apply" button to save the changes.
// Multi-threaded DLL (/MD)
// 1. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 2. In the project properties window, navigate to the "Configuration Properties" > "C/C++" > "Code Generation" category.
// 3. Look for the "Runtime Library" property. It is typically set to "Multi-threaded (/MT)" by default.
// 4. Select "Multi-threaded DLL (/MD)" from the drop-down menu.
// 5. Click the "Apply" button to save the changes.

// Manager.h

#include <iostream>      // Provides basic input/output stream functionality
#include <windows.h>     // Provides Windows-specific functions and types
#include <filesystem>    // Provides filesystem-related functionality
#include <fstream>       // For file operations
#include <Shlwapi.h>     // For PathCombineW
#include <sstream>       // For string manipulation
#pragma comment(lib, "Rpcrt4.lib")  // Link against Rpcrt4.lib

using namespace std;
using namespace filesystem;

extern "C" {
    // Exported function declarations

    // Init function
    // Parameters:
    //   - services: The path to the Services folder
    //   - hmodule: The path to the HMODULE folder
    //   - manager: Reference to the HMODULEManager
    __declspec(dllexport) void Init(const wstring& services, const wstring& hmodule, HMODULE& manager);
    // Heartbeat function
    __declspec(dllexport) void Heartbeat();
    // Shutdown function
    __declspec(dllexport) void Shutdown();
    // Function to check if the vector contains a File with a matching Path and return the associated Module
    __declspec(dllexport) HMODULE Get(LPCWSTR Path);
    // Function to update a file in vector
    __declspec(dllexport) void Update(LPCWSTR Path);
    // Function to remove a file from vector
    __declspec(dllexport) void Remove(LPCWSTR Path);

}

// Declaration of string references and HMODULE reference
wstring& _hmodule = *(new wstring());   // Reference to the hmodule string
wstring& _services = *(new wstring());  // Reference to the services string
HMODULE& _manager = *(new HMODULE());   // Reference to the manager HMODULE


// Typedef for the initialization function pointer
// Parameters:
//   - const wstring&: A constant reference to a wstring object representing the services path.
//   - HMODULE&: A reference to an HMODULE object representing the manager.
typedef void (*ModuleInit)(const wstring&, HMODULE&);

// Typedef for the module heartbeat function pointer
typedef void (*ModuleHeartbeat)();

// Typedef for the module shutdown function pointer
typedef void (*ModuleShutdown)();

// Struct to represent a loaded DLL file
struct File
{
    LPCWSTR OldPath;             // Path to the original file
    LPCWSTR NewPath;             // Path to the new file
    HMODULE Module;              // HMODULE associated with the file
    ModuleHeartbeat Heartbeat;   // Heartbeat function
    ModuleShutdown Shutdown;     // Shutdown function
};

// Vector to store loaded DLL files
vector<File> Files;

// Go and read the code at the specified URL
// https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Manager/Manager.cpp
