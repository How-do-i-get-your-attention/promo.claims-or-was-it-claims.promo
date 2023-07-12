#pragma once
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Guideline:
// 1. Open your project in Visual Studio.
// 2. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 3. In the project properties window, navigate to the "C/C++" tab.
// 4. Under the "Command Line" section, locate the "Additional Options" field.
// 5. Add /std:c++latest to the "Additional Options" field.
// 6. Click on the "Apply" button to save the changes.
// We use Console for testing
#include <iostream>
// We need to include the header for filesystem API access in Windows
#include <filesystem>
// We need to include the Windows API
#include <windows.h>
// We use the std namespace for C++
using namespace std;
// We use the filesystem namespace for Windows
using namespace filesystem;
// Thread: First layer setup
int wmain(int argc, char* argv[]);
// Let's create a manager 
// Storage for the manager module handle
HMODULE manager;
// Function pointer type for ManagerServices
typedef bool (*ManagerServices)(path, path, HMODULE&);
// Function pointer type for ManagerGoodbye
typedef bool (*ManagerGoodbye)();
// Function pointer for the Goodbye function
ManagerGoodbye Goodbye;
// Storage in memory for the Services Control.
SC_HANDLE servicesControl,
// Storage in memory OpenService for servicesControl
openService;
// Storage in memory service status
SERVICE_STATUS serviceStatus;
// Storage in memory for the service status handle
SERVICE_STATUS_HANDLE serviceStatusHandle;