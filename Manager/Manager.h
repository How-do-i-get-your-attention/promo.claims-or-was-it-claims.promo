#define BySetup DWORD(1)
#define ByServices DWORD(2)
#pragma once
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
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
#include <tuple>		 // Provides tuple
#include <chrono>        // Provides time-related functionality
#include <iomanip>       // Provides manipulators for stream formatting
#include <sstream>       // Provides string stream functionality
#include <rpc.h>         // Provides the necessary definitions for working with Remote Procedure Call (RPC) functions
#include <fstream>       // Provides file stream functionality for reading and writing files
#include <vector>        // Provides dynamic array functionality


#pragma comment(lib, "Rpcrt4.lib") // Link the Rpcrt4.lib library
// We use the std namespace for C++
using namespace std;
// We use the filesystem namespace for Windows
using namespace filesystem;
// We use the chrono namespace for time-related functionality
using namespace chrono;
// We use the this_thread namespace for thread-related functionality
using namespace this_thread;
// Define a struct named Library
struct Library
{
    path input; // The path to the drive services
    HMODULE module;   // The reference to the HMODULE
};
// This code includes the necessary libraries, defines the Library struct, and creates a vector named 'libraries' to store Library objects.
vector<Library> libraries;
// Declare the 'extern "C"' block which tells the C++ compiler to prevent name mangling.
// This makes it easier to link to these functions from C programs or other languages.
extern "C"
{
    // The '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Setup', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are three paths and an HMODULE.
    // If the function returns 'false', it indicates an error condition.
    __declspec(dllexport) bool Setup(path, path, path, HMODULE&);
    // Similarly, the '__declspec(dllexport)' keyword tells the compiler that the following function,
    // 'Services', should be made available to other DLLs or executables that reference this DLL.
    // The parameters it accepts are two paths and an HMODULE.
    __declspec(dllexport) void Services(path, path, HMODULE&);
    // `Goodbye`: An exported function that is intended to be called upon completion or termination of the program,
    // or when the DLL is about to be unloaded. It doesn't accept any parameters.
    // Its specific functionality depends on its implementation in the DLL's source code.
    __declspec(dllexport) void Goodbye();
    // The 'Write' function is made available to other DLLs or executables that reference this DLL.
    // Parameters:
    // - library: The name of the library the message is associated with
    // - function: The name of the function the message is associated with
    // - messages: The message content
    // - error: The error flag indicating if the message is an error or not
    void Write(string library, string function, string messages, bool error);
    // 'GenerateUUID', should be made available to other DLLs or executables that reference this DLL.
    // It generates a new UUID (Universally Unique Identifier).
    __declspec(dllexport) GUID GenerateUUID();
    // 'Get', should be made available to other DLLs or executables that reference this DLL.
    // The parameter 'input' is the path to the input file.
    // It returns the handle to the loaded module (HMODULE) if successful, otherwise nullptr.
    __declspec(dllexport) HMODULE Get(path input);
    // is available for other DLLs or executables that reference this DLL.
    // Parameters:
    // - path: The path to the input file.
    __declspec(dllexport) void Stop(path input);
    // 'Override' function declaration
    // This function replaces an existing module in the libraries vector with a new module or adds a new module if the input is not found.
    // Parameters:
    // - input: The path to the module
    // - module: The new HMODULE to override or add
    __declspec(dllexport) void Override(path input, HMODULE module);
}
// is it Setup or Services ?
DWORD By;
// Create a vector named Messages to store Message tuples
vector<bool> Messages;
// Parameters from Setup or Services
path _executableService, // The path to the executable service
     _driveServices, // The path to the drive services
     _driveHMODULE; // The path to the drive HMODULE
HMODULE managerObject = nullptr; // Create an HMODULE object and initialize it with nullptr
HMODULE& _manager = managerObject; // Create a reference to the HMODULE object
// Function declaration for generating a UUID
GUID GenerateUUID();

