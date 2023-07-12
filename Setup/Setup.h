#pragma once
// Setup:Explore further, fearlessly
// This directive ensures that the header file is included only once in the compilation process

// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Guideline:
// 1. Open your project in Visual Studio.
// 2. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 3. In the project properties window, navigate to the "C/C++" tab.
// 4. Under the "Command Line" section, locate the "Additional Options" field.
// 5. Add /std:c++latest to the "Additional Options" field.
// 6. Click on the "Apply" button to save the changes.
// This comment provides instructions for enabling the latest C++ features in Visual Studio
// Include necessary headers
#include <iostream>      // Provides basic input/output stream functionality
#include <filesystem>    // Provides filesystem-related functionality
#include <windows.h>     // Provides Windows-specific functions and types
#include <tuple>		 // Provides tuple
#include <chrono>        // Provides time-related functionality
// We use the std namespace for C++
using namespace std;
// We use the filesystem namespace for Windows
using namespace filesystem;
// We use the chrono namespace for time-related functionality
using namespace chrono;
// Let's create a manager 
typedef bool (*ManagerSetup)(path, path, path, HMODULE&);
// Function pointer type for ManagerGoodbye
typedef bool (*ManagerGoodbye)();
// Define the tuple type to store time and message
typedef tuple<system_clock::time_point, bool, string> Message;
// Thread: First layer setup
int wmain(int argc, char* argv[]);