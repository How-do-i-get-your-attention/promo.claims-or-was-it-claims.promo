#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <map>
#include <string>
#include <windows.h>
#include <WinSock2.h>
#include <guiddef.h>
#include <rpc.h>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <atlbase.h>
#pragma comment(lib, "ws2_32.lib")
struct Documentation {
    std::string name;
    std::string description;
    void (*run)() = nullptr;
};
std::map<int, Documentation> documentations;
void View() {
    system("cls");
    for (const auto& doc : documentations) {
        std::cout << "# " << doc.second.name << " / ID: " << doc.first << std::endl  << doc.second.description << std::endl << std::endl;
    }

    int selectedID;
    std::cout << "Enter ID to start: ";
    std::cin >> selectedID;

    auto iter = documentations.find(selectedID);
    if (iter != documentations.end() && iter->second.run != nullptr) {
        iter->second.run();
    }
    else {
        std::cout << "Invalid ID or no corresponding function found." << std::endl;
    }
    std::cout << "Press any key to continue...";
    std::cin.ignore();
    std::cin.get();
    View();
}
void Add(std::string name, std::string description, void (*run)()) {
    documentations[static_cast<int>(documentations.size())] = { name, description, run };
}
wchar_t** Envp;
std::wstring GetEnvironmentVariable(const wchar_t* variable) {
    for (int i = 0; Envp[i] != nullptr; i++) {
        std::wstring envVariable(Envp[i]);
        size_t delimiterPos = envVariable.find(L"=");
        std::wstring envName = envVariable.substr(0, delimiterPos);
        if (envName == variable) {
            std::wstring envValue = envVariable.substr(delimiterPos + 1);
            return envValue;
        }
    }
    return L"";
}
wchar_t** Argv;
void Environment() {
    for (int i = 0; Envp[i] != nullptr; i++) {
        std::wcout << Envp[i] << std::endl;
    }
}
void Arguments() {
    for (int i = 0; Argv[i] != nullptr; i++) {
        std::wcout << Argv[i] << std::endl;
    }
}
bool IsVisualStudio() {
    for (int i = 0; Envp[i] != nullptr; i++) {
        std::wstring envVariable(Envp[i]);
        size_t delimiterPos = envVariable.find(L"=");
        std::wstring envName = envVariable.substr(0, delimiterPos);
        if (envName == L"VSAPPIDNAME") 
            return true;
    }
    return false;
}
WSADATA WSAData;
std::map<int, SOCKET> Gateway;
std::pair<int, SOCKET> CreateGateway(int addressFamily, int socketType, int protocol) {
    static int keyCounter = 0;
    int key = keyCounter++;
    SOCKET newSocket = socket(addressFamily, socketType, protocol);
    Gateway[key] = newSocket;
    return std::make_pair(key, newSocket);
}
void RemoveGateway(int key) {
    auto it = Gateway.find(key);
    if (it != Gateway.end()) {
        closesocket(it->second);
        Gateway.erase(it);
    }
}
std::string ComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    GetComputerNameA(computerName, &size);
    std::transform(computerName, computerName + size, computerName, [](unsigned char c) { return std::tolower(c); });
    if (IsVisualStudio()) {
        return "vs-" + std::string(computerName);
    }
    else {
        return std::string(computerName);
    }
}
std::wstring PCOrCPFolderPath() {
    return GetEnvironmentVariable(L"HOMEDRIVE") + L"\\PCOrCP-" + std::wstring(ComputerName().begin(), ComputerName().end());
}
bool PCOrCPFolderCreate() {
    std::wstring folderPath = PCOrCPFolderPath();
    if (!std::filesystem::exists(folderPath))
    {
        std::filesystem::create_directory(folderPath);
        return true;
    }
    return false;
}
std::string ExecutablePath() {
    std::wstring argW = Argv[0];
    std::string arg(argW.begin(), argW.end());
    size_t pos = arg.find_last_of("\\/");
    if (pos != std::string::npos) {
        arg.erase(pos + 1);
    }
    return arg;
}



void CloneLibrary() {}
void CloneBackground() {}


void Exit() {
    std::exit(0);
}
void Install() {
    PCOrCPFolderCreate();
  
    Exit();
}
extern "C" __declspec(dllexport) void Setup() {

    //Documentations
    Add("Documentation", "Struct for Documentation", []() {
        std::cout << "std::string name: Name of the function." << std::endl
            << "std::string description: Description of the function." << std::endl
            << "void (*run)(): A startup function or the function itself." << std::endl;
        });
    Add("Documentations", "std::map<int, Documentation> documentations", []() {
        std::cout << "std::map<int, Documentation> documentations for Setup, View, and Add functions." << std::endl;
        });
    Add("View", "View() - View all Documentations", View);
    Add("Add", "Add(std::string name, std::string description, void (*run)())", []() {
        std::cout << "A function that easily adds to Documentations." << std::endl;
        });
    Add("Init", "Init(wchar_t* envp[], wchar_t* argv[])", []() {
        std::cout << "This function initializes the environment and sets up the necessary components for socket programming." << std::endl
            << "Parameters:" << std::endl
            << "- envp: A pointer to an array of wide character strings representing the environment variables." << std::endl
            << "- argv: A pointer to an array of wide character strings representing the command-line arguments." << std::endl
            << "Initialization Steps:" << std::endl
            << "1. Store the environment variables and command-line arguments for later use." << std::endl
            << "2. Call WSAStartup() with the appropriate version (e.g., MAKEWORD(2, 2)) to initialize the Winsock library." << std::endl
            << "   - This function returns information about the Winsock implementation in the WSAData structure." << std::endl
            << "   - Make sure to define the WSAData structure (e.g., WSADATA WSAData) before calling WSAStartup." << std::endl;
        });
    Add("Setup", "Setup()", []() {
        std::cout << "This function is only called in Setup.exe." << std::endl;
        });
    Add("Background", "Background()", []() {
        std::cout << "This function is only called in Background.dll." << std::endl;
        });
    Add("Dispose", "Dispose()\nAutomatic cleanup and removal of all gateway entries.", []() {
        std::cout << "This function performs automatic cleanup and removal of all gateway entries." << std::endl
                  << "It closes all associated sockets in the Gateway map and clears the map." << std::endl
                  << "Finally, it calls WSACleanup() to clean up the Winsock library." << std::endl;
        });
    Add("Environment", "Environment()\nThis function printing all Envp", Environment);
    Add("GetEnvironmentVariable", "std::wstring GetEnvironmentVariable(const wchar_t* variable)\nThis function retrieves the value of the specified environment variable.", []() {
        std::wstring variable;
        std::wcout << "Enter the environment variable name: ";
        std::wcin >> variable;
        std::wstring value = GetEnvironmentVariable(variable.c_str());
        std::wcout << "Value of " << variable << ": " << value << std::endl;
        });
    Add("Arguments ", "Arguments()\nThis function printing all Argv", Arguments);
    Add("IsVisualStudio", "IsVisualStudio()\nCheck if it's Visual Studio and return a bool.", []() {
        if (IsVisualStudio())
            std::cout << "Is Visual Studio." << std::endl;
        else 
            std::cout << "Is not Visual Studio." << std::endl;
        });
    Add("Gateway", "std::map<int, SOCKET> Gateway", []() {
        if (Gateway.empty()) {
            std::cout << "No socket entries in the Gateway." << std::endl;
        }
        else {
            std::cout << "IDs of socket entries in the Gateway:" << std::endl;
            for (const auto& entry : Gateway) {
                std::cout << entry.first << std::endl;
            }
        }
        });
    Add("CreateGateway", "std::pair<int, SOCKET> CreateGateway(int addressFamily, int socketType, int protocol)", []() {
        std::cout << "This function creates a gateway with a unique integer key and a socket descriptor." << std::endl
            << "Parameters:" << std::endl
            << "- addressFamily: The address family or protocol family of the socket. Use AF_INET for IPv4 communication or AF_INET6 for IPv6 communication." << std::endl
            << "- socketType: The type of the socket to be created. Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP communication." << std::endl
            << "- protocol: The specific protocol to be used with the socket. Use IPPROTO_TCP for TCP or IPPROTO_UDP for UDP communication." << std::endl
            << "Returns:" << std::endl
            << "A pair containing the generated integer key and the newly created socket. If the socket creation fails, the key will be -1, and the socket descriptor will be an INVALID_SOCKET value." << std::endl;

        int addressFamily = AF_INET; // Set the desired address family (e.g., AF_INET for IPv4)
        int socketType = SOCK_STREAM; // Set the desired socket type (e.g., SOCK_STREAM for TCP)
        int protocol = IPPROTO_TCP; // Set the desired protocol (e.g., IPPROTO_TCP for TCP)

        std::pair<int, SOCKET> gateway = CreateGateway(addressFamily, socketType, protocol);
        int key = gateway.first;
        SOCKET socket = gateway.second;

        if (key == -1 || socket == INVALID_SOCKET) {
            std::cout << "Failed to create the socket. Please check the parameters and try again." << std::endl;
        }
        else {
            std::cout << "Socket created with ID: " << key << std::endl;
        }
        });
    Add("RemoveGateway", "void RemoveGateway(int key)", []() {
        std::cout << "This function removes a gateway entry from the Gateway map based on the provided key." << std::endl
            << "Parameters:" << std::endl
            << "- key: The integer key associated with the gateway entry to be removed." << std::endl
            << "Functionality:" << std::endl
            << "- Closes the socket associated with the found entry." << std::endl
            << "- Removes the entry from the Gateway map." << std::endl;

        if (Gateway.empty()) {
            std::cout << "No gateway entries available for removal." << std::endl;
            return;
        }
        int key;
        std::cout << "Enter the ID of the gateway entry to remove: ";
        std::cin >> key;

        auto it = Gateway.find(key);
        if (it != Gateway.end()) {
            closesocket(it->second);
            Gateway.erase(it);
            std::cout << "Gateway entry with ID " << key << " removed." << std::endl;
        }
        else {
            std::cout << "Invalid gateway entry ID. No entry removed." << std::endl;
        }

        });
    Add("ComputerName", "std::string ComputerName()\nThis function gets the computer name.\nIt uses IsVisualStudio() to determine if it's Visual Studio and adds 'vs-' prefix if true.", []() {std::cout << ComputerName() << std::endl;});
    Add("PCOrCPFolderPath", "std::wstring PCOrCPFolderPath()\nThis function retrieves the folder path for the PCOrCP folder.\nIt uses GetEnvironmentVariable(L\"HOMEDRIVE\") and the computer name to construct the path.", []() {std::wstring folderPath = PCOrCPFolderPath();std::wcout << "PCOrCP folder path: " << folderPath << std::endl << std::endl; });
    Add("PCOrCPFolderCreate", "bool PCOrCPFolderCreate()\nThis function checks if the PCOrCP folder exists and creates it if it doesn't exist.\nReturns true if the folder is created or already exists, false if the creation fails.\n\nIt uses PCOrCPFolderPath() to obtain the folder path.", []() {if (PCOrCPFolderCreate()) std::wcout << "PCOrCP folder creation succeeded." << std::endl;else std::wcout << "PCOrCP folder already exists." << std::endl;});
    Add("ExecutablePath", "std::string ExecutablePath()\nThis function retrieves the path by removing the executable name from the Argv[0] string.\nReturns the modified path as a std::string.", []() {std::cout << "Path: " << ExecutablePath() << std::endl;});
   

    Add("Exit", "Exit()\nAn Exit for Setup.exe", Exit);
    Add("Install", "Install()\nAn installer for Background.dll as a service in Windows.\nIt includes an PCOrCPFolderCreate function\nnIt includes an BackgroundClone function\nIt includes an Exit function.", Install);
    View();
}

extern "C" __declspec(dllexport) void Background() {
    std::cout << "Background" << std::endl;
}



extern "C" __declspec(dllexport) void Init(wchar_t* envp[], wchar_t* argv[]) {
    Envp = envp;
    Argv = argv;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
}
extern "C" __declspec(dllexport) void Dispose() {
    for (const auto& entry : Gateway) {
        closesocket(entry.second);
    }
    Gateway.clear();
    WSACleanup();
 
}