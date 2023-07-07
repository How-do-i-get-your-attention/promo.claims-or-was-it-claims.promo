#define BySetup DWORD(1)
#define ByServices DWORD(2)
#pragma once
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Output Directory: ..\PCOrCP\Services\Manager.dll
// Multi-threaded (/MT)
// Include necessary headers
#include <iostream>      // Provides basic input/output stream functionality
#include <filesystem>    // Provides filesystem-related functionality
#include <windows.h>     // Provides Windows-specific functions and types
#include <tuple>		 // Provides tuple
#include <chrono>        // Provides time-related functionality
#include <iomanip>       // Provides manipulators for stream formatting
#include <sstream>       // Provides string stream functionality

// We use the std namespace for C++
using namespace std;

// We use the filesystem namespace for Windows
using namespace filesystem;

// We use the chrono namespace for time-related functionality
using namespace chrono;

// Declare the 'extern "C"' block which tells the C++ compiler to prevent name mangling.
// This makes it easier to link to these functions from C programs or other languages.
extern "C"
{
    // The '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Setup', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are three paths and an HMODULE.
    // If the function returns 'false', it indicates an error condition.
    __declspec(dllexport) bool Setup(path, path, path, HMODULE);

    // Similarly, the '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Services', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are two paths and an HMODULE.
    __declspec(dllexport) void Services(path, path, HMODULE);

    // `Goodbye`: An exported function that is intended to be called upon completion or termination of the program,
    // or when the DLL is about to be unloaded. It doesn't accept any parameters.
    // Its specific functionality depends on its implementation in the DLL's source code.
    __declspec(dllexport) void Goodbye();
    // The '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Write', should be made available to other DLLs or executables that reference this DLL.
    __declspec(dllexport) void Write(string, bool);
}
DWORD By;
// Create a vector named Messages to store Message tuples
vector<bool> Messages;