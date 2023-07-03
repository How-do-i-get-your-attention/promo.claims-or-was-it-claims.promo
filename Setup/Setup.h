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
// Storage in memory for the path to this executable folder.
path executable,
// Storage in memory for the location of the Services.exe file 
executableServicesFile,
// Storage in memory for the path to the windows drive
drive,
// path for project folder
pathProject;
// Storage in memory for the Services Control.
SC_HANDLE servicesControl,
// Storage in memory OpenService for servicesControl
openService;
// Storage in memory service status
SERVICE_STATUS serviceStatus;
// Function for copying Project to drive
void CopyProject();
// Read https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Setup/Setup.cpp