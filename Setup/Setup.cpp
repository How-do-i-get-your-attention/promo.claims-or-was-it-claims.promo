// Project PCOrCP
#define Project L"PCOrCP"
// This is the initial setup for a developer's project.
// We need a starting point, which in this case is the main function.
// Let's make it the entry point of the program and add a title thread: "First layer setup".
// But before we can begin, we need a header file. Without the header file, the script will have problems finding necessary declarations and information.
// The header file, "Setup.h", can also provide information about different components.
#include "Setup.h"
// Thread: First layer setup
// Entry point for a Windows console application supporting Unicode
// This setup needs to be in executable.
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
    // 'executable' is already declared in the header.
    // Create a wchar_t array with max path length
    wchar_t executablePath[MAX_PATH];
    // Get the executable folder
    GetModuleFileNameW(nullptr, executablePath, MAX_PATH);
    // Convert wchar_t array to path and remove Setup.exe
    executable = path(executablePath).parent_path();
    // set the path for Sevices.dll
    executableServicesFile = executable / "Services.dll";
    wchar_t system32Path[MAX_PATH];
    // Get the System32 folder
    GetSystemDirectory(system32Path, MAX_PATH);
    // Extract the first two characters (drive letter) and set drive
    drive.assign(system32Path, system32Path + 3); 
    // set Working area folder for Project and reuse drive
    drive = drive / Project;
    // set HMODULE path for Project
    pathProject = drive / "HMODULE";
    // 'servicesControl' is already declared in the header.
    // Prepare servicesControl
    servicesControl = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    // Request the service PCOrCP from servicesControl
    openService = OpenServiceW(servicesControl, Project, SERVICE_ALL_ACCESS);
    // Check if the service exists or not
    if (openService == nullptr) {
        // Start CopyProject
        CopyProject();
        //other step will happen here
    }
    else {
        // Get the service status using openService
        (void)QueryServiceStatus(openService, &serviceStatus);
        // Check if the service is running
        if (serviceStatus.dwCurrentState == SERVICE_RUNNING) 
            // Stop the service
            ControlService(openService, SERVICE_CONTROL_STOP, &serviceStatus);
        // Start CopyProject
        CopyProject();
        //other step will happen here
        // Close openService handle
        CloseServiceHandle(openService);
    }
    // Close servicesControl
    CloseServiceHandle(servicesControl);
    // Successful
    return 0;
}
// Function for copying Project Services
void CopyProject()
{
    // Check if exists
    if (exists(drive))
        // Delete the folder and all items
        remove_all(drive);
    // Create projectPath
    create_directory(drive);
    // Create HMODULE folder
    // This folder is created temporarily to run the add-on DLL with the Services.dll
    create_directory(pathProject);
    // Reuse of projectServicesPath
    pathProject = drive / "Services";
    // Create Services folder
    // This folder is created to have all the libraries that need to be loaded to the Services.dll. 
    // When a DLL is needed, it will be copied to the HMODULE folder temporarily.
    create_directory(pathProject);
    // Reuse of drive
    drive /= path(wstring(Project) + L".dll");
    // copy executableServicesFile to projectPath
    copy(executableServicesFile, drive);
    // This is the file path where all DLLs need to be placed in order to work with Services.dll.
    executable /= "Services";
    // Loop all files in executable.
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
}
