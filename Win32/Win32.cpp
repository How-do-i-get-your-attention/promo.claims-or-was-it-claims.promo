#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

// Entry point for the Win32 Application
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    if (lpCmdLine == NULL)
    {
        // Code to handle service-specific logic
        // This block will execute when the code is running as a service
        OutputDebugString(L"Running as a service");
    }
    else
    {
        // Code to handle non-service logic
        // This block will execute when the code is running as a regular application
        OutputDebugString(L"Running as a regular application");
    }

    return 0;
}
