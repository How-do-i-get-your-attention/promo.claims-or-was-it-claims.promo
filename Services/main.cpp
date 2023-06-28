#include "../Network/Network.h"
//Windows Services
SERVICE_STATUS_HANDLE serviceStatusHandle;
SERVICE_STATUS serviceStatus;
void SetServiceState(DWORD Value) {
    if (serviceStatus.dwCurrentState == Value) return;
    serviceStatus.dwCurrentState = Value;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}
VOID WINAPI ControlHandler(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        // Handle stop request
        // Stop any running processes or threads
        // Cleanup and terminate the service
        SetServiceState(SERVICE_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE:
        // Handle pause request
        // Pause ongoing operations
        SetServiceState(SERVICE_PAUSED);
        break;
    case SERVICE_CONTROL_CONTINUE:
        // Handle continue request
        // Resume paused operations
        SetServiceState(SERVICE_RUNNING);
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        // Handle system shutdown notification
        // Perform necessary cleanup before the system shuts down
        CloseAllSocket();
        WSACleanup();
        SetServiceState(SERVICE_STOPPED);
        break;
    }
}
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    serviceStatusHandle = RegisterServiceCtrlHandlerW(L"PCOrCP", ControlHandler);
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwWaitHint = 0;
    SetServiceState(SERVICE_RUNNING);
    WSAStartup(MAKEWORD(2, 2), &windowsSocketsData);
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED) {}
}
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(L"PCOrCP"), ServiceMain },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}