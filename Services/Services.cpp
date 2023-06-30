#include "Services.h"

SERVICE_STATUS_HANDLE serviceStatusHandle;
SERVICE_STATUS serviceStatus;
void SetServiceState(const DWORD Value) {
    if (serviceStatus.dwCurrentState == Value) return;
    serviceStatus.dwCurrentState = Value;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}
VOID WINAPI ControlHandler(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_PAUSE:
        SetServiceState(SERVICE_PAUSED);
        break;
    case SERVICE_CONTROL_CONTINUE:
        SetServiceState(SERVICE_RUNNING);
        break;
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
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
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED)
        Sleep(2000);

}
int wmain(int argc, char* argv[])
{
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(L"PCOrCP"), reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(ServiceMain) },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}