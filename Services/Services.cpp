#include "Services.h"
vector<tuple<string, string, HMODULE>> Modules;
void InitModule(const string& path) {
    auto moduleTuple = find_if(Modules.begin(), Modules.end(), [&path](const auto& tuple) {return get<0>(tuple) == path;});
    if (moduleTuple != Modules.end())
        return;
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    string exePath(buffer);
    string directory = exePath.substr(0, exePath.find_last_of("\\"));
    string fullPath = directory + "\\" + path;
    string tempFolderPath = directory + "\\_temp_";
    CreateDirectoryA(tempFolderPath.c_str(), nullptr);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 15);
    const std::string hexDigits = "0123456789abcdef";
    std::string uniqueID;
    for (int i = 0; i < 32; ++i)
        uniqueID += hexDigits[distribution(generator)];
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    char dateTimeBuffer[16];
    sprintf_s(dateTimeBuffer, sizeof(dateTimeBuffer), "%04d%02d%02d%02d%02d%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay,systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
    string tempNewPath = tempFolderPath + "\\" + string(dateTimeBuffer) + uniqueID + ".dll";
    CopyFileA(fullPath.c_str(), tempNewPath.c_str(), FALSE);
    HMODULE module = LoadLibraryA(tempNewPath.c_str());
    Modules.push_back(std::make_tuple(path, tempNewPath, module));
    typedef void (*InitFunc)(std::vector<std::tuple<std::string, std::string, HMODULE>>&, void(*)(const std::string&), HMODULE(*)(const std::string&), void(*)(const std::string&));
    InitFunc initFunction = reinterpret_cast<InitFunc>(GetProcAddress(module, "Init"));
    std::thread dobbeltThread([initFunction, path]() {initFunction(Modules, InitModule, GetModule, RemoveModule); RemoveModule(path); });
    dobbeltThread.detach();
}
void RemoveModule(const std::string& path) {
    auto moduleTuple = std::find_if(Modules.begin(), Modules.end(), [&path](const auto& tuple) {
        return std::get<0>(tuple) == path;
        });

    if (moduleTuple != Modules.end() && std::get<0>(*moduleTuple) != "*") {
        std::get<0>(*moduleTuple) = "*";
        HMODULE module = std::get<2>(*moduleTuple);
        voidFunc shutdownFunction = reinterpret_cast<voidFunc>(GetProcAddress(module, "Shutdown"));
        if (shutdownFunction) {
            shutdownFunction();
        }
        FreeLibrary(module);
        const std::string& tempPath = std::get<1>(*moduleTuple);
        remove(tempPath.c_str());
        Modules.erase(moduleTuple);
    }
}
HMODULE GetModule(const std::string& path)
{
    HMODULE module = nullptr;
    auto moduleTuple = std::find_if(Modules.begin(), Modules.end(), [&path](const auto& tuple) {return std::get<0>(tuple) == path; });
    if (moduleTuple != Modules.end())
        module = std::get<2>(*moduleTuple);
    else
    {
        InitModule(path);
        module = GetModule(path);
    }
    return module;
}
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
    case SERVICE_CONTROL_STOP:
        for (const auto& module : Modules)
        {
            HMODULE dllModule = std::get<2>(module);
            voidFunc sFunction = reinterpret_cast<voidFunc>(GetProcAddress(dllModule, "Stop"));
            if (sFunction)
            {
                sFunction();
            }
        }
        SetServiceState(SERVICE_STOPPED);
        break;
    case SERVICE_CONTROL_PAUSE:
        for (const auto& module : Modules)
        {
            HMODULE dllModule = std::get<2>(module);
            voidFunc sFunction = reinterpret_cast<voidFunc>(GetProcAddress(dllModule, "Pause"));
            if (sFunction)
            {
                sFunction();
            }
        }
        SetServiceState(SERVICE_PAUSED);
        break;
    case SERVICE_CONTROL_CONTINUE:
        for (const auto& module : Modules)
        {
            HMODULE dllModule = std::get<2>(module);
            voidFunc sFunction = reinterpret_cast<voidFunc>(GetProcAddress(dllModule, "Play"));
            sFunction();
        }
        SetServiceState(SERVICE_RUNNING);
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        for (const auto& module : Modules)
        {
            const std::string& moduleName = std::get<0>(module);
            if (moduleName != "*") {
                RemoveModule(moduleName);
            }
        }
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
    InitModule("PCOrCP.dll");
    while (serviceStatus.dwCurrentState != SERVICE_STOPPED)
        Sleep(2000);
    for (const auto& module : Modules)
    {
        const std::string& moduleName = std::get<0>(module);
        if (moduleName != "*") {
            RemoveModule(moduleName);
        }
    }
}
int wmain(int argc, char* argv[])
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    string exePath(buffer);
    string directory = exePath.substr(0, exePath.find_last_of("\\"));
    string tempFolderPath = directory + "\\_temp_";
    std::string searchPath = tempFolderPath + "\\*.*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filePath = tempFolderPath + "\\" + findData.cFileName;
                DeleteFileA(filePath.c_str());
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
    SERVICE_TABLE_ENTRYW serviceTable[] =
    {
        { const_cast<LPWSTR>(L"PCOrCP"), reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(ServiceMain) },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcherW(serviceTable);
    return 0;
}