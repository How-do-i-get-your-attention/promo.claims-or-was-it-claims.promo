// Setup:Explore further c++, fearlessly
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
    // Set the path for Services.exe
    path executableServicesFile = executable / "Services.exe";

    wchar_t system32Path[MAX_PATH];
    path drive;

    // Get the System32 folder
    GetSystemDirectory(system32Path, MAX_PATH);
    // Extract the first two characters (drive letter) and set drive
    drive.assign(system32Path, system32Path + 3);

    // Set Working area folder for Project and reuse drive
    drive = drive / Project;

    // Set HMODULE path for Project
    path pathProject = drive / "HMODULE";

    // Prepare servicesControl
    SC_HANDLE servicesControl = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

    // Request the service PCOrCP from servicesControl
    SC_HANDLE openService = OpenServiceW(servicesControl, Project, SERVICE_ALL_ACCESS);

    // Convert drive to wstring
    // Check if the service exists or not
    if (openService == nullptr) {
        // Check if drive exists
        if (exists(drive))
            // Delete the folder and all items
            remove_all(drive);

        // Create projectPath
        create_directory(drive);

        // Create HMODULE folder
        // This folder is created temporarily to run the add-on DLL with the Services.exe
        create_directory(pathProject);

        // Reuse of projectServicesPath
        pathProject = drive / "Services";

        // Create Services folder
        // This folder is created to have all the libraries that need to be loaded to the Services.exe. 
        // When a DLL is needed, it will be copied to the HMODULE folder temporarily.
        create_directory(pathProject);

        // Reuse of drive
        drive /= path(wstring(Project) + L".exe");

        // Copy executableServicesFile to projectPath
        copy(executableServicesFile, drive);

        // This is the file path where all DLLs need to be placed in order to work with Services.exe.
        executable /= "Services";

        // Convert drive to wstring
        wstring wStringDrive = drive.wstring();

        // Modified the Drive to "\\"
        wstring modifiedDrive;
        for (wchar_t ch : wStringDrive) {
            if (ch == L'\\') {
                modifiedDrive += L"\\\\";
            }
            else {
                modifiedDrive += ch;
            }
        }

        // Convert modifiedDrive to LPCWSTR
        LPCWSTR lpcwstrDrive = modifiedDrive.c_str();

        // Loop through all files in executable
        for (const auto& entry : directory_iterator(executable)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                // Get the filename of the DLL
                const auto& dllFilename = entry.path().filename();

                // Create the destination path in the pathProject directory
                const path destinationPath = pathProject / dllFilename;

                // Copy the DLL file to the destination path
                copy_file(entry.path(), destinationPath);
            }
        }

        // Create the service
        openService = CreateService(servicesControl, Project, Project, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, lpcwstrDrive, nullptr, nullptr, nullptr, nullptr, nullptr);

        // Start the service
        StartService(openService, 0, nullptr);

        // Close openService handle
        CloseServiceHandle(openService);
    }
    // Close servicesControl
    CloseServiceHandle(servicesControl);
    // Successful
    return 0;
    // This is published in folder PCOrCP as Setup.exe
}


// Go and Read the code at the specified URL
// https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Services/Services.h
