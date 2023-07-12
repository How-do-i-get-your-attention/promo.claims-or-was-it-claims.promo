#define Project L"PCOrCP"
#pragma warning(disable: 6387)
#include "Services.h"
// ServiceMain function that handles service startup and control
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Set the service status to running
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    // Update the service status using the service control manager
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
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
    string managerPathString = (drive / "Manager.dll").string();
    LPCSTR managerPathLPCSTR = managerPathString.c_str();
    // Load the DLL using LoadLibraryA
    manager = LoadLibraryA(managerPathString.c_str());
    // Check if the DLL was loaded successfully
    if (manager != nullptr) {
        ManagerServices managerServices = reinterpret_cast<ManagerServices>(GetProcAddress(manager, "Services"));
        Goodbye = reinterpret_cast<ManagerGoodbye>(GetProcAddress(manager, "Goodbye"));
        if (managerServices != nullptr)
            // Call the Services function provided by the DLL
            // Parameters: executableService, executableHMODULE, driveServices, driveHMODULE
            managerServices(driveServices, driveHMODULE, manager);
        else
            // Print an error message indicating the failure to retrieve the "Setup" function address
            cout << "Failed to retrieve (Services) function address." << endl;
    }
    else
        // Failed to load the DLL, display an error message
        // Print an error message indicating the failure to load the DLL
        cout << "Failed to load DLL: " << managerPathString << endl;
}
// ControlHandler function that handles service control requests
VOID WINAPI ControlHandler(DWORD dwControl)
{
    if (dwControl == SERVICE_CONTROL_STOP || dwControl == SERVICE_CONTROL_SHUTDOWN) {
        // Perform Goodbye operations
        if (Goodbye != nullptr)
            Goodbye();
        // Set the service status to stopped
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        // Update the service status using the service control manager
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
    }
}
//thread:First Layer of Services
int wmain(int argc, char* argv[])
{
    // Register the service control handler
    serviceStatusHandle = RegisterServiceCtrlHandlerW(Project, ControlHandler);
    // Set the service type and accepted controls
    serviceStatus.dwServiceType = SERVICE_TYPE_ALL;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PARAMCHANGE | SERVICE_ACCEPT_NETBINDCHANGE | SERVICE_ACCEPT_HARDWAREPROFILECHANGE | SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE | SERVICE_ACCEPT_PRESHUTDOWN | SERVICE_ACCEPT_TIMECHANGE | SERVICE_ACCEPT_TRIGGEREVENT | SERVICE_ACCEPT_LOWRESOURCES | SERVICE_ACCEPT_SYSTEMLOWRESOURCES;
    // Set the exit code and wait hint
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwWaitHint = 0;
    // Define the service table
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(Project), reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(ServiceMain) },
        { NULL, NULL }
    };
    // Start the service control dispatcher
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}