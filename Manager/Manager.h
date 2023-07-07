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
// We use the std namespace for C++
using namespace std;

// We use the filesystem namespace for Windows
using namespace filesystem;
// Declare the 'extern "C"' block which tells the C++ compiler to prevent name mangling.
// This makes it easier to link to these functions from C programs or other languages.
extern "C"
{
    // The '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Setup', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are three paths and an HMODULE.
    __declspec(dllexport) void Setup(path, path, path, HMODULE);

    // Similarly, the '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Services', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are two paths and an HMODULE.
    __declspec(dllexport) void Services(path, path, HMODULE);
}
