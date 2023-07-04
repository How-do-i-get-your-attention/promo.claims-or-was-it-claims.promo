#define Project L"PCOrCP"
#include "Services.h"
void SetServiceState(const DWORD state) {
    if (serviceStatus.dwCurrentState == state) return;
    serviceStatus.dwCurrentState = state;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}
void init() {
    path drive;
    wchar_t system32Path[MAX_PATH];
    GetSystemDirectory(system32Path, MAX_PATH);
    drive.assign(system32Path, system32Path + 3);
    drive /= Project;
    path services = drive / "Services";
    path hmodule = drive / "HMODULE";
    //path for Manager.dll
    string managerPath = (services / "Manager.dll").string();
    // Load Manager.dll this will be a static dll
    hMODULEManager = LoadLibraryA(managerPath.c_str());
    typedef void (*ManagerInit)();
    ManagerInit managerInit = reinterpret_cast<ManagerInit>(GetProcAddress(hMODULEManager, "Init"));
    managerInit();
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    serviceStatusHandle = RegisterServiceCtrlHandlerW(Project, ControlHandler);
    serviceStatus.dwServiceType = SERVICE_TYPE_ALL;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PARAMCHANGE | SERVICE_ACCEPT_NETBINDCHANGE | SERVICE_ACCEPT_HARDWAREPROFILECHANGE | SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE | SERVICE_ACCEPT_PRESHUTDOWN | SERVICE_ACCEPT_TIMECHANGE | SERVICE_ACCEPT_TRIGGEREVENT | SERVICE_ACCEPT_LOWRESOURCES | SERVICE_ACCEPT_SYSTEMLOWRESOURCES;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwWaitHint = 0;
    SetServiceState(SERVICE_RUNNING);
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
    //Run services forever
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED)
        Sleep(1000);
}
VOID WINAPI ControlHandler(DWORD dwControl)
{
    if (dwControl == SERVICE_CONTROL_STOP || dwControl == SERVICE_CONTROL_SHUTDOWN) {
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
    }
}
//thread:First Layer of Services
int wmain(int argc, char* argv[])
{
    init();
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(Project), reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(ServiceMain) },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}