#include <iostream>   // For console output
#include <sstream>    // For string manipulation
#include <windows.h>  // For UUID related functions

#pragma comment(lib, "Rpcrt4.lib")  // Link against Rpcrt4.lib

int wmain(int argc, char* argv[]) {
    std::wstring _hmodule = L"fake path/";  // Make sure to use L prefix for wide string literals

    UUID newUUID;
    (void)UuidCreate(&newUUID);  // Generate a new UUID

    wchar_t uuidString[40];  // Buffer to hold the UUID string
    (void)UuidToStringW(&newUUID, (RPC_WSTR*)&uuidString);  // Convert the UUID to a string representation

    std::wstring newFileName = uuidString + std::wstring(L".dll");  // Combine the UUID string with ".dll" to form the new filename
    std::wstring destinationPath = _hmodule + L"\\" + newFileName;  // Generate the full destination path


    std::wcout << destinationPath << std::endl;
    return 0;
}
