// include Server.h
#include "Server.h"


//Start program as developer
Program::Program(const char* DriveAsChar)
{
    {
        // Get the path of Windows
        char windowsPath[MAX_PATH];
        // Get Windows Directory
        (void)GetWindowsDirectoryA(windowsPath, MAX_PATH);
        //Inset in Program::Drive
        Drive.insert(Drive.end(), windowsPath, windowsPath + 3);
    }
    // Memory used by windowsPath is released here
    // windowsPath is not accessible outside the scoped block
    {
        // Make PCOrCP\Server.exe in ASCII
        vector<uint8_t> Path = { 80, 67, 79, 114, 67, 80, 92, 83, 101, 114, 118, 101, 114, 46, 101, 120, 101 };
        // Insert Path into Drive
        Drive.insert(Drive.end(), Path.begin(), Path.end());
    }
    // Outside the scope, the Path vector no longer exists, and its memory is deallocated
    // The elements in Path are not accessible and do not occupy memory
    // Convert Drive to String
    DriveAsString = string(Drive.begin(), Drive.end()).c_str();
    // Convert Drive to Char
    Program::DriveAsChar = const_cast<char*>(DriveAsString.c_str());
    // If the parameter DriveAsChar is the same as Program::DriveAsChar, exit because it's already running as a service and not a console task.
    if (Program::DriveAsChar == DriveAsChar) {
        //Call the Exit of the program
        Program::~Program();
        //Block to continue the code.
        return;
    }
    //Check if Drive exists
    if (fs::exists(DriveAsString)) {
        
        //comming soon
        

        //Block to continue the code.
        return;
    }
    cout << DriveAsChar << endl;
    PSID adminGroup;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        BOOL isMember;
        if (CheckTokenMembership(NULL, adminGroup, &isMember)) {
            if (isMember) {
                // Free the adminGroup SID after the check
                FreeSid(adminGroup);
                {
                    // Get services Path
                    std::filesystem::path Path = std::filesystem::path(DriveAsString).parent_path();
                    // Check if Path exists if not create the Path
                    if (!std::filesystem::exists(Path))
                        std::filesystem::create_directories(Path);
                }
                // Outside the scope, the Path no longer exists, and its memory is deallocated
                // The elements in Path are not accessible and do not occupy memory
                // Copy file from DriveAsChar to DriveAsString
                fs::copy_file(DriveAsChar, DriveAsString, fs::copy_options::overwrite_existing);
                // OpenSCManager and open it.
                ServicesManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
                {
                    // Ignore the return value of UuidCreate
                    UUID uuid;
                    (void)UuidCreate(&uuid);
                    // Ignore the return value of UuidToStringA
                    RPC_CSTR uuidString;
                    (void)UuidToStringA(&uuid, &uuidString);
                    //Name the services
                    std::wstring Name(reinterpret_cast<wchar_t*>(uuidString));
                    // Free the allocated string
                    RpcStringFreeA(&uuidString);
                    // Erase - from Name
                    Name.erase(std::remove(Name.begin(), Name.end(), '-'), Name.end());
                    std::wstring DriveWideString(DriveAsString.begin(), DriveAsString.end());
                    // Get current UTC time
                    auto now = std::chrono::system_clock::now();
                    auto time = std::chrono::system_clock::to_time_t(now);

                    std::tm tm;
#ifdef _MSC_VER
                    gmtime_s(&tm, &time);
#else
                    gmtime_r(&time, &tm);
#endif

                    std::wstringstream wss;
                    wss << std::put_time(&tm, L"%Y%m%d%H%M%S");

                    // Construct command-line arguments
                    std::wstring  CommandLineArgs = L"-Name " + Name + L" -UTCDatetime " + wss.str();
                    //Create the services and CloseServiceHandle
                    CloseServiceHandle(CreateService(ServicesManager, Name.c_str(), Name.c_str(), SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, (DriveWideString + L" " + CommandLineArgs).c_str(), nullptr, nullptr, nullptr, nullptr, nullptr));
                }
                //Call the Exit of the program
                Program::~Program();
                //Block to continue the code.
                return;
            }
            else {
                cout << "User is not an administrator." << endl;
                // Free the adminGroup SID after the check
                FreeSid(adminGroup);
                //Block to continue the code.
                return;
            }
        }
        // Free the adminGroup SID after the check
        FreeSid(adminGroup);
    }
    cout << "Failed to check membership." << endl;
    //Block to continue the code.
    return;
}
//Exit of the program
Program::~Program()
{
    //Clear Drive
    Drive.clear();
    //Clear DriveAsString
    DriveAsString.clear();
    //Shrink Drive
    Drive.shrink_to_fit();
    //Shrink DriveAsString
    DriveAsString.shrink_to_fit();
    //Close ServicesManager if not NULL
    if(ServicesManager!=NULL)
        CloseServiceHandle(ServicesManager);
    // When exit(0) is called, the operating system takes over and performs necessary cleanup tasks. 
    // This includes deallocating the memory used by the program, closing open files, and releasing other system resources.
    exit(0);
}
// This error function is for testing purposes.
// Use it with try-catch.
// After testing, do not use try-catch.
void Program::Error(const std::exception& exception)
{
    // Create the output file named "error.txt"
    ofstream outputFile("error.txt");
    // Open the file.
    if (outputFile.is_open()) {
        // Write the exception message to the file
        outputFile << exception.what() << std::endl;
        // Close the file after writing
        outputFile.close();
    }
}
// This Messages function is for testing purposes.
void Program::Messages(const std::string& messesges)
{
    // Create the output file named "error.txt"
    ofstream outputFile("messesges.txt");
    // Open the file.
    if (outputFile.is_open()) {
        // Write the messages to the file
        outputFile << messesges << std::endl;
        // Close the file after writing
        outputFile.close();
    }
}
void PrintArguments(int argc, char* argv[])
{
    // Create the output file named "arguments.txt"
    std::ofstream outputFile("arguments.txt");

    // Open the file
    if (outputFile.is_open()) {
        // Write each argument to the file
        for (int i = 0; i < argc; ++i) {
            outputFile << argv[i] << std::endl;
        }

        // Close the file after writing
        outputFile.close();
    }
}
//C++ start of Server.cpp
int main(int argc, char* argv[])
{
    // Start program as developer
    if (argc == 1) {

        Program program(argv[0]);
    }
    PrintArguments(argc, argv);
    //Return a value to the console or service.
    return 0;
}
