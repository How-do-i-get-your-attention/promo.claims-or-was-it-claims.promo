#include <Windows.h>

// Service status structure
SERVICE_STATUS        g_ServiceStatus;
SERVICE_STATUS_HANDLE g_ServiceStatusHandle;

// Service control handler function
void WINAPI ServiceCtrlHandler(DWORD dwCtrlCode)
{
    switch (dwCtrlCode)
    {
    case SERVICE_CONTROL_STOP:
        // Handle stop request
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
        // Perform necessary cleanup and stop the service
        // ...
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
        break;

        // Handle other service control requests as needed

    default:
        break;
    }
}

// Service main function
void WINAPI ServiceMain(DWORD argc, LPWSTR* argv)
{
    // Register the service control handler
    g_ServiceStatusHandle = RegisterServiceCtrlHandler(L"Promo.Claims", ServiceCtrlHandler);
    if (!g_ServiceStatusHandle)
    {
        // Error handling
        return;
    }

    // Set initial service status
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;  // Add other accepted controls as needed
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;
    g_ServiceStatus.dwWaitHint = 0;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);

    // Perform initialization tasks and start the service
    // ...

    // Update service status to indicate it is running
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);

    // Service main loop
    while (g_ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        // Perform the main functionality of the service
        // ...
        // You can also implement a sleep or wait mechanism to control the service behavior
    }
}

// Entry point for the console application
int wmain(int argc, LPWSTR* argv)
{
    // Define and initialize service table entry
    SERVICE_TABLE_ENTRY serviceTable[] =
    {
        { const_cast<LPWSTR>(L"Promo.Claims"), ServiceMain },
        { NULL, NULL }
    };

    // Start the service control dispatcher
    if (!StartServiceCtrlDispatcher(serviceTable))
    {
        // Error handling
        return GetLastError();
    }

    return 0;
}


//sc.exe create PromoClaims binPath= "C:\Path\To\Your\Service.exe"