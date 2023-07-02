#include "Services.h"
/*Windows Services*/
SERVICE_STATUS_HANDLE serviceStatusHandle;
SERVICE_STATUS serviceStatus;
void SetServiceState(const DWORD state) {
    if (serviceStatus.dwCurrentState == state) return;
    serviceStatus.dwCurrentState = state;
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
    // Connect to ControlHandler
    serviceStatusHandle = RegisterServiceCtrlHandlerW(AppName, ControlHandler);
    //Service Types (Bit Mask)
    serviceStatus.dwServiceType = SERVICE_TYPE_ALL;
    // Controls Accepted  (Bit Mask)
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PARAMCHANGE | SERVICE_ACCEPT_NETBINDCHANGE | SERVICE_ACCEPT_HARDWAREPROFILECHANGE| SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE| SERVICE_ACCEPT_PRESHUTDOWN| SERVICE_ACCEPT_TIMECHANGE| SERVICE_ACCEPT_TRIGGEREVENT| SERVICE_ACCEPT_LOWRESOURCES | SERVICE_ACCEPT_SYSTEMLOWRESOURCES;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwWaitHint = 0;
    SetServiceState(SERVICE_RUNNING);
    //Keep run First Layer
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED)
        Sleep(1000);
}
//thread:First Layer of Services
int wmain(int argc, char* argv[])
{
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(AppName), reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(ServiceMain) },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}