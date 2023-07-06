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

    // Perform heartbeat
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED) {
        
        Sleep(1);
    }

}

// ControlHandler function that handles service control requests
VOID WINAPI ControlHandler(DWORD dwControl)
{
    if (dwControl == SERVICE_CONTROL_STOP || dwControl == SERVICE_CONTROL_SHUTDOWN) {
        // Perform shutdown operations

        // Get the function pointer for the "Shutdown" function from the loaded DLL
        ManagerShutdown managerShutdown = reinterpret_cast<ManagerShutdown>(GetProcAddress(hMODULEManager, "Shutdown"));

        // Call the "Shutdown" function
        managerShutdown();

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
// Go and read the code at the specified URL
// https://github.com/How-do-i-get-your-attention/promo.claims-or-was-it-claims.promo/blob/master/Manager/Manager.h