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
#include <iostream>      // Duplicate inclusion, not necessary
#include <windows.h>     // Provides Windows-specific functions and types
#include <filesystem>    // Provides filesystem-related functionality

using namespace std;
using namespace filesystem;

extern "C" {
    // Exported function declarations

    // Init function
    // Parameters:
    //   - services: The path to the Services folder
    //   - hmodule: The path to the HMODULE folder
    //   - manager: Reference to the HMODULEManager
    __declspec(dllexport) void Init(const std::wstring& services, const std::wstring& hmodule, HMODULE& manager);

    // Heartbeat function
    __declspec(dllexport) void Heartbeat();

    // Shutdown function
    __declspec(dllexport) void Shutdown();
}
