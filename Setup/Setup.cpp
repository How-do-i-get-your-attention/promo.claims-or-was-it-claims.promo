
// Project PCOrCP
#define Project L"PCOrCP"
// Output Directory: ..\PCOrCP\Setup.exe
// This directive ensures that the header file is included only once in the compilation process
#pragma once
#include "Setup.h"
// Preview - Features from the Latest C++ Working Draft (/std:c++latest)
// Guideline:
// 1. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 2. In the project properties window, navigate to the "C/C++" tab.
// 3. Under the "Command Line" section, locate the "Additional Options" field.
// 4. Add /std:c++latest to the "Additional Options" field.
// 5. Click on the "Apply" button to save the changes.
// This comment provides instructions for enabling the latest C++ features in Visual Studio


// Thread: First layer setup
// Entry point for a Windows console application supporting Unicode
// This setup needs to be in an executable.
// Guideline:
// 1. Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
// 2. In the project properties window, navigate to the "Configuration Properties" section.
// 3. Select the "General" category.
// 4. Locate the "Configuration Type" option and set it to "Application (.exe)".
// 5. Click on the "Apply" button to save the changes.
// Function to print a message box to the console
void Messages(string context) {
    int width = context.length() + 4; // Calculate the width of the box
    string horizontalLine(width, '-'); // Create the horizontal line
    // Print the box with the input text centered
    cout << endl << '+' << horizontalLine << '+' << endl << "| " << context <<  endl << '+' << horizontalLine << '+' << endl;
}

int wmain(int argc, char* argv[])
{
    // We need to get the path of the location of the Setup.exe as it runs as:
    // 1. Administrator
    // Guideline:
    // 1.1 Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
    // 1.2 In the project properties window, navigate to the "Linker" section and click on "Manifest File" under it.
    // 1.3 Set the "UAC Execution Level" option to "requireAdministrator" from the drop-down menu.
    // 1.4 Click on the "Apply" button to save the changes.
    // 2. Multi-threaded (/MT)
    // Guideline:
    // 2.1 Right-click on your project in the Solution Explorer and select "Properties" from the context menu.
    // 2.2 In the project properties window, navigate to the "C/C++" section.
    // 2.3 Select the "Code Generation" category.
    // 2.4 Set the "Runtime Library" option to "Multi-threaded (/MT)".
    // 2.5 Click on the "Apply" button to save the changes.
    // Create a wchar_t array with max path length
    wchar_t executablePath[MAX_PATH];
    // Get the executable folder
    GetModuleFileNameW(nullptr, executablePath, MAX_PATH);
    // Convert wchar_t array to path and remove Setup.exe
    path executable = path(executablePath).parent_path();
    path executableService = executable / "Services";

    wchar_t system32Path[MAX_PATH];
    path drive;
    // Get the System32 folder
    GetSystemDirectory(system32Path, MAX_PATH);
    // Extract the first two characters (drive letter) and set drive
    drive.assign(system32Path, system32Path + 3);
    // Set Working area folder for Project and reuse drive
    drive = drive / Project;
    path driveServices = drive / "Services";
    path driveHMODULE = drive / "HMODULE";

    // Prepare servicesControl
    SC_HANDLE servicesControl = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    // Request the service PCOrCP from servicesControl
    SC_HANDLE openService = OpenServiceW(servicesControl, Project, SERVICE_ALL_ACCESS);
    // Check if the service exists or not
    if (openService != nullptr) {
        // Declare a variable to store the service status
        SERVICE_STATUS serviceStatus;
        // Stop the service
        ControlService(openService, SERVICE_CONTROL_STOP, &serviceStatus);
        // Delete the service
        DeleteService(openService);
        // Close the service handle
        CloseServiceHandle(openService);
    }
    // If the drive exists, remove it
    if (exists(drive))
        remove_all(drive);
    // We always provide a fresh installation for developers.
    CloseServiceHandle(servicesControl);
    // Let's validate the source.
    // Convert the path to a string
    string managerPathString = (executableService / "Manager.dll").string();

    LPCSTR managerPathLPCSTR = managerPathString.c_str();
    // Load the DLL using LoadLibraryA
    HMODULE manager = LoadLibraryA(managerPathString.c_str());
    // Check if the DLL was loaded successfully
    if (manager != nullptr) {
        ManagerSetup managerSetup = reinterpret_cast<ManagerSetup>(GetProcAddress(manager, "Setup"));
        if (managerSetup != nullptr)
            if (managerSetup(executableService, driveServices, driveHMODULE, manager)) {
                Messages("Install services");
                // Handle successful Setup
            }
            else
                // Print error message for services not being installed
                Messages("Services not installed");

        // Print error message for services not being installed
        else
            // Print an error message indicating the failure to retrieve the "Setup" function address
            Messages("Failed to retrieve (Setup) function address.");
    }
    else 
        // Failed to load the DLL, display an error message
        // Print an error message indicating the failure to load the DLL
        Messages("Failed to load DLL: " + managerPathString);
    // Successful
    return 0;
    // This is published in folder PCOrCP as Setup.exe
}


