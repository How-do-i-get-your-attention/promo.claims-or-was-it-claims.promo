#include "Manager.h"
// The Write function adds a message along with the current time, error flag, library, and function to the Messages vector.
// Parameters:
// - library: The name of the library the message is associated with
// - function: The name of the function the message is associated with
// - messages: The message content
// - error: The error flag indicating if the message is an error or not
void Write(string library, string function, string messages, bool error) {
    // Check if the 'By' value is equal to 'BySetup'
    if (By == BySetup) {
        // Add the message tuple to the Messages vector
        Messages.push_back(error);
        // Get the current time
        auto currentTime = std::chrono::system_clock::now();
        auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
        // Convert the current time to a time_t
        auto currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
        // Convert the time_t to a local time struct tm
        std::tm localTime{};
        localtime_s(&localTime, &currentTimeT);
        // Format the local time as a string including milliseconds
        char timeBuffer[80];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", &localTime);
        sprintf_s(timeBuffer + 17, sizeof(timeBuffer) - 17, "%03lld", currentTimeMs.count());
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, error ? FOREGROUND_RED : FOREGROUND_GREEN);
        // Print the time, error status, message, library, and function to the console
        cout << "+------------------------+" << endl
            << "| Time: " << timeBuffer << endl
            << "| Error: " << (error ? "Yes" : "No") << endl
            << "| Library: " << library << endl
            << "| Function: " << function << endl
            << "| Message: " << messages << endl
            << "+------------------------+" << endl;
        // Restore the default console text color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    }
    else if(error) {
        // do other error here this is in services i need to think what wee can do here
        // i think about a idaer about some kind server system that live show a error so wee can fix the error
    }
}

// DeleteFileAsync function
// This function asynchronously deletes a module file.
// Parameters:
// - filePath: The path to the module file.
void DeleteFileAsync(path filePath)
{
    thread([filePath = move(filePath)]() {
        sleep_for(seconds(1)); // Delay to ensure the module is unloaded
        error_code ec;
        remove(filePath, ec);
        if (!ec)
        {
            // Write a success message if the file is deleted successfully
            Write("Manager.dll", "DeleteFileAsync", "Deleted module file: " + filePath.string(), false);
        }
        else
        {
            // Write an error message if failed to delete the file
            Write("Manager.dll", "DeleteFileAsync", "Failed to delete module file: " + filePath.string(), true);
        }
        }).detach();
}


// GenerateUUID function
// This function generates a new UUID (Universally Unique Identifier) using the UuidCreate function.
// It returns the generated UUID.
GUID GenerateUUID()
{
    GUID uuid; // Create a variable to store the generated UUID
    (void)UuidCreate(&uuid); // Generate a new UUID using the UuidCreate function
    Write("Manager.dll", "GenerateUUID", "The UUID is generated successfully", false);
    return uuid; // Return the generated UUID
}
// Convert a generated UUID to a string representation
string GenerateUUIDAsString()
{
    GUID uuid = GenerateUUID(); // Generate a UUID using the GenerateUUID function

    RPC_CSTR guidString;
    (void)UuidToStringA(&uuid, &guidString); // Convert the UUID to a string

    string str(reinterpret_cast<const char*>(guidString)); // Create a string from the GUID string

    RpcStringFreeA(&guidString); // Free the memory allocated by UuidToStringA

    Write("Manager.dll", "GenerateUUIDAsString", "UUID:"+str, false); // Write a message indicating the conversion

    return str; // Return the generated UUID as a string
}
// Add function
// This function copies the content of the input file to the output file.
// Parameters:
// - input: The path to the input file.
// Returns:
// - The handle to the loaded module (HMODULE) if successful, nullptr otherwise.
HMODULE Add(path input)
{
    std::string uuidStr = GenerateUUIDAsString();
    path pathHMODULE = _driveHMODULE / (uuidStr + ".dll");;
    // Copies the content of the input file to the output file, overwriting the existing file if it already exists.
    if (!copy_file(input, pathHMODULE, copy_options::overwrite_existing)) {
        Write("Manager.dll", "Add", "Input: " + input.string() + ", Output: " + pathHMODULE.string(), true);
        return nullptr;
    }

    // Load the module using the path of the copied file
    HMODULE hModule = LoadLibraryW(pathHMODULE.wstring().c_str());
    if (hModule != nullptr) {
        Write("Manager.dll", "Add", "Input: " + input.string() + ", Output: " + pathHMODULE.string(), false);
        // Load the module using the path of the copied file
        HMODULE hModule = LoadLibraryW(pathHMODULE.wstring().c_str());
        return hModule;
    }
    else {
        Write("Manager.dll", "Add", "Input: " + input.string() + ", Output: " + pathHMODULE.string(), true);
        return nullptr;
    }
}
// Get function
// This function retrieves the HMODULE associated with the specified input file path.
// If the input file is found in the libraries vector, its associated HMODULE is returned.
// If the input file is not found, the Add function is called to add the file and retrieve its associated HMODULE.
// Parameters:
// - input: The path to the input file.
// Returns:
// - The HMODULE associated with the input file.
HMODULE Get(path input)
{
    // Search for the input path in the libraries vector
    auto libraryIt = std::find_if(libraries.begin(), libraries.end(), [&input](const Library& library) {
        return library.input == input;
        });

    // If the input path is found, return the associated HMODULE
    if (libraryIt != libraries.end())
    {
        Write("Manager.dll", "Get", "Input: " + input.string() + ", Found in libraries", false);
        return libraryIt->module;
    }

    // If the input path is not found, call the Add function to add the file and get the associated HMODULE
    HMODULE hModule = Add(input);
    Write("Manager.dll", "Get", "Input: " + input.string() + ", Added to libraries", false);
    return hModule;
}
// Stop function
// This function stops the specified input library by unloading it and deleting its module file.
// If the input library is found in the libraries vector, it is unloaded, its module file is deleted, and the library is removed from the vector.
// Parameters:
// - input: The path to the input library.
void Stop(path input)
{
    // Find the library in the vector based on the input path
    auto libraryIt = std::find_if(libraries.begin(), libraries.end(), [&input](const Library& library) {
        return library.input == input;
        });

    // If the library is found
    if (libraryIt != libraries.end())
    {
        HMODULE module = libraryIt->module;
        // Unload the library
        FreeLibrary(module);
        libraries.erase(libraryIt);
        // Get the path of the module
        TCHAR modulePath[MAX_PATH];
        if (GetModuleFileName(module, modulePath, MAX_PATH) != 0)
            // Delete the module file
            DeleteFileAsync(modulePath);
        else
            Write("Manager.dll", "Stop", "Failed to get module file path", true);
    }
    else
        Write("Manager.dll", "Stop", "Library not found", true);
}
// Override function
// This function replaces an existing module in the libraries vector with a new module or adds a new module if the input is not found.
// Parameters:
// - input: The path to the module
// - module: The new HMODULE to override or add
void Override(path input, HMODULE module) {
    // Find the library in the libraries vector based on the input path
    auto libraryIt = find_if(libraries.begin(), libraries.end(), [&input](const Library& library) {
        return library.input == input;
        });

    if (libraryIt != libraries.end()) {
        FreeLibrary(libraryIt->module); // Unload the existing module

        // Get the path of the module
        TCHAR modulePath[MAX_PATH];
        if (GetModuleFileName(libraryIt->module, modulePath, MAX_PATH) != 0)
            // Delete the module file
            DeleteFileAsync(modulePath);
        else {
            Write("Manager.dll", "Override", "Failed to get module file path", true);
        }

        libraryIt->module = module; // Update the module field with the new HMODULE

        // Write a success message
        Write("Manager.dll", "Override", "Module overridden for input: " + input.string(), false);
    }
    else {
        // Add the new library to the libraries vector
        libraries.push_back({ input, module });

        // Write a success message
        Write("Manager.dll", "Override", "New module added for input: " + input.string(), false);
    }
}
// Setup function
// This function is called by Setup.exe to perform the initial setup process.
// It sets the Manager.dll as BySetup and carries out the necessary steps for installation.
// Parameters:
// - path: The path to the executable service
// - path: The path to the drive services
// - path: The path to the drive HMODULE
// - HMODULE: The HMODULE handle
bool Setup(path executableService, path driveServices, path driveHMODULE, HMODULE& manager)
{
    By = BySetup; // Set BY as BYSetup
    // Assign the manager parameter to the _manager reference
    _manager = manager;
    // Assign the paths to their respective variables
    _executableService = executableService;
    _driveServices = driveServices;
    _driveHMODULE = driveHMODULE;
    std::string uuidStr = GenerateUUIDAsString();
    path tt = driveHMODULE / (uuidStr + ".dll");;
    path ff = executableService / "Start.dll";
    Write("Manager.dll", "Setup", "Running test", true);
    Add(ff);
    Stop(ff);

    for (bool error : Messages) // Loop through each message in the Messages container
    {
        if (error)
            return false; // Return false if any message has an error flag set to false
    }
    return true; // Return true if all messages have error flags set to true
}
// Services function
// This function is called by "Services".exe to perform specific service-related operations.
// It sets the Manager.dll as ByServices and carries out the necessary steps accordingly.
// Parameters:
// - path: The path to the executable service
// - path: The path to the drive services
// - HMODULE: The HMODULE handle
void Services(path driveServices, path driveHMODULE, HMODULE& manager)
{
    By = ByServices; // Set BY as ByServices
    // Assign the manager parameter to the _manager reference
    _manager = manager;
    // Assign the paths to their respective variables
    _driveServices = driveServices;
    _driveHMODULE = driveHMODULE;

    // Perform specific service operations here
    // The service name is changed to the user-defined name
    // Cannot run install, but can run update from Setup
    // Add additional code for service-related operations as needed
}


void Goodbye()
{
    Write("Manager.dll", "Goodbye", "Call Goodbye()", false);
    FreeLibrary(_manager);
}
 