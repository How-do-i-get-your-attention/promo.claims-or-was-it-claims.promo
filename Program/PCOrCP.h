#include <iostream>
#include <map>
#include <string>
#include <windows.h>
#include <shlwapi.h>
#include <guiddef.h>

extern wchar_t** Argv;
extern wchar_t** Envp;

typedef void (*InitFunc)(wchar_t* [], wchar_t* []);
typedef void (*VoidFunc)();

void Library(bool IsSetup) {
    std::wstring Path = std::wstring(Argv[0]).substr(0, std::wstring(Argv[0]).find_last_of(L"\\/"));
    std::wstring libPath = Path + L"\\Library.dll";

    GUID guid;
    CoCreateGuid(&guid);
    wchar_t guidString[40];
    StringFromGUID2(guid, guidString, sizeof(guidString) / sizeof(guidString[0]));

    std::wstring tempFolder = Path + L"\\_temp_";
    CreateDirectoryW(tempFolder.c_str(), nullptr);
    std::wstring newFileName = tempFolder + L"\\" + std::wstring(guidString) + L".dll";

    std::wstring searchPattern = tempFolder + L"\\*";
    WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW(searchPattern.c_str(), &findData);
    if (findHandle != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring filePath = tempFolder + L"\\" + findData.cFileName;
                DeleteFileW(filePath.c_str());
            }
        } while (FindNextFileW(findHandle, &findData));
        FindClose(findHandle);
    }
    if (CopyFileW(libPath.c_str(), newFileName.c_str(), FALSE)) {
        HMODULE lib = LoadLibraryW(newFileName.c_str());
        if (lib != nullptr) {
            InitFunc initFunc = reinterpret_cast<InitFunc>(GetProcAddress(lib, "Init"));
            initFunc(Envp, Argv);

            VoidFunc voidFunc;
            if (IsSetup) {
                voidFunc = reinterpret_cast<VoidFunc>(GetProcAddress(lib, "Setup"));
            }
            else {
                voidFunc = reinterpret_cast<VoidFunc>(GetProcAddress(lib, "Background"));
            }
            voidFunc();

            voidFunc = reinterpret_cast<VoidFunc>(GetProcAddress(lib, "Dispose"));
            voidFunc();

            FreeLibrary(lib);
        }
        DeleteFileW(newFileName.c_str());
    }
    RemoveDirectoryW(tempFolder.c_str());
    Library(IsSetup);
}