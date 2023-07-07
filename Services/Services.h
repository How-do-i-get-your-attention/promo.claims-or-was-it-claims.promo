#define BySetup DWORD(1)
#define ByServices DWORD(2)
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
// Include necessary headers
#include <iostream>      // Provides basic input/output stream functionality
#include <filesystem>    // Provides filesystem-related functionality
#include <windows.h>     // Provides Windows-specific functions and types
#include <tuple>         // Provides tuple
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
// Dynamic Link Library (DLL) - 
// A DLL is a file that contains reusable code and resources that can be shared across multiple programs.
// It allows different programs to dynamically load and access functions or resources from the DLL at runtime,
// providing modular and efficient code reuse. DLLs are commonly used in Windows systems to provide
// functionality to various applications without the need for duplication of code.
// In this context, the Manager.dll serves as a DLL that exports functions and resources to be used by other programs.
