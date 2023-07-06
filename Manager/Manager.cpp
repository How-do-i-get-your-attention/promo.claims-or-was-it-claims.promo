#include "Manager.h"

// Function to check if the vector contains a File with a matching Path and return the associated Module
HMODULE Get(LPCWSTR Path) {
    cout << "Get";
    // Find the first occurrence of a File with a matching Path using std::find_if
    auto it = find_if(Files.begin(), Files.end(),
        [Path](const File& file) {
            return wcscmp(file.OldPath, Path) == 0;
        });

    if (it != Files.end()) {
        // If a matching Path is found, return the associated Module
        return it->Module;
    }

    // If no matching Path is found, add the file to the vector and return the associated Module
    // Open the source file in the _services folder
    wstring sourcePath = _services + L"\\" + Path;
    ifstream sourceFile(sourcePath, ios::binary);
    if (!sourceFile) {
        // Handle error when unable to open the source file
        return nullptr;
    }
    UUID newUUID;
    UuidCreate(&newUUID);  // Generate a new UUID

    wchar_t uuidString[40] = {};  // Initialize the buffer with null characters

    UuidToStringW(&newUUID, (RPC_WSTR*)uuidString);  // Convert the UUID to a string representation

    wstring newFileName = wstring(uuidString) + L".dll";  // Combine the UUID string with ".dll" to form the new filename
    wstring destinationPath = _hmodule + L"\\" + newFileName;  // Generate the full destination path
    // Copy the contents of the source file to the destination file
    ofstream destinationFile(destinationPath, ios::binary);
    destinationFile << sourceFile.rdbuf();
    // Close the files
    sourceFile.close();
    destinationFile.close();
    // Load the copied module
    HMODULE module = LoadLibraryW(destinationPath.c_str());
    if (module == nullptr) {
        // Handle error when unable to load the module
        return nullptr;
    }
    // Create a new File instance
  // Create a new File instance
    File newFile{
        Path,                                           // OldPath: Path to the original file
        destinationPath.c_str(),                        // NewPath: Path to the new file
        module,                                         // Module: HMODULE associated with the file
        reinterpret_cast<ModuleHeartbeat>(              // Heartbeat: Function pointer to the 'Heartbeat' function
            GetProcAddress(module, "Heartbeat")
        ),
        reinterpret_cast<ModuleShutdown>(               // Shutdown: Function pointer to the 'Shutdown' function
            GetProcAddress(module, "Shutdown")
        )
    };
    // Get the "Init" function address from the module
    ModuleInit moduleInit = reinterpret_cast<ModuleInit>(GetProcAddress(module, "Init"));
    // Call the moduleInit function if it exists
    if (moduleInit != nullptr) {
        moduleInit(_services, _manager);  // Initialize the module
    }
    // Add the new file to the Files vector
    Files.push_back(newFile);
    return newFile.Module;
}

// Function to update a file
void Update(LPCWSTR Path) {
    // Find the File with a matching Path using std::find_if
    auto it = find_if(Files.begin(), Files.end(),
        [Path](const File& file) {
            return wcscmp(file.OldPath, Path) == 0;
        });

    if (it == Files.end()) {
        // If the file is not found, call the Get() function to add and load the file
        (void)Get(Path);
    }
    else
    {
        auto oldCreationTime = last_write_time(it->OldPath);
        auto newCreationTime = last_write_time(it->NewPath);

        if (oldCreationTime > newCreationTime) {
            // If the old file is newer than the copied file, update the file

            // Construct the source path for the file
            wstring sourcePath = _services + L"\\" + Path;
            ifstream sourceFile(sourcePath, ios::binary);
            if (!sourceFile) {
                // Handle error when unable to open the source file
                return;
            }

            UUID newUUID;
            UuidCreate(&newUUID);  // Generate a new UUID

            wchar_t uuidString[40] = {};  // Initialize the buffer with null characters

            UuidToStringW(&newUUID, (RPC_WSTR*)uuidString);  // Convert the UUID to a string representation


            // Construct the new destination file name using the UUID
            wstring newFileName = wstring(uuidString) + L".dll";
            wstring destinationPath = _hmodule + L"\\" + newFileName;

            // Copy the contents of the source file to the destination file
            ofstream destinationFile(destinationPath, ios::binary);
            destinationFile << sourceFile.rdbuf();

            // Close the files
            sourceFile.close();
            destinationFile.close();

            // Load the updated module
            HMODULE module = LoadLibraryW(destinationPath.c_str());

            // Free the previous module
            FreeLibrary(it->Module);

            // Update the File entry with the new module and path
            it->Module = module;
            remove(it->NewPath);
            it->NewPath = destinationPath.c_str();

            // Get the "Init" function address from the module
            ModuleInit moduleInit = reinterpret_cast<ModuleInit>(GetProcAddress(module, "Init"));

            // Set the heartbeat function address from the module if available
            it->Heartbeat = reinterpret_cast<ModuleHeartbeat>(GetProcAddress(module, "Heartbeat"));

            // Set the shutdown function address from the module if available
            it->Shutdown = reinterpret_cast<ModuleShutdown>(GetProcAddress(module, "Shutdown"));

            // Call the moduleInit function if it exists
            if (moduleInit != nullptr) {
                moduleInit(_services, _manager);  // Initialize the module
            }
        }
    }
    // Perform update logic if needed
}


// Shutdown function
// This function performs the shutdown logic for the Manager.
// It iterates through the loaded files and calls their shutdown functions if available.
// After calling the shutdown function, it frees the associated library using FreeLibrary.
void Shutdown() {
    // Perform shutdown logic for Manager

    // Iterate through the loaded files and call their shutdown functions if available
    for (const auto& file : Files) {
        if (file.Shutdown != nullptr) {
            file.Shutdown();  // Call the shutdown function
        }
    }

    // Free the associated libraries and remove copied files
    for (const auto& file : Files) {
        FreeLibrary(file.Module);  // Free the associated library
        remove(file.NewPath);  // Remove the copied file
    }

    // Clear the list of files
    Files.clear();
}


// Init function
void Init(const wstring& servicesPath, const wstring& hmodulePath, HMODULE& manager) {
    // Initialization logic for the Manager
    // Perform necessary initialization steps using the updated hmodule, services, and manager references

    // Set the hmodule, services, and manager references with the provided paths
    _hmodule = hmodulePath;
    _services = servicesPath;
    _manager = manager;

    for (const auto& entry : directory_iterator(_services)) {
        remove(entry);
    }
}

// Heartbeat function
// This function performs heartbeat logic for the Manager.
// It iterates through the loaded files and calls their heartbeat functions if available.
// Each heartbeat function is called in a separate detached thread to run asynchronously.
// The heartbeat logic is performed once, and the function returns immediately.
void Heartbeat() {
    for (const auto& file : Files) {
        if (file.Heartbeat != nullptr) {
            thread threadHeartbeat(file.Heartbeat);
            threadHeartbeat.detach();
        }
    }
}

// Shutdown function
// This function performs the shutdown logic for the Manager.
// It iterates through the loaded files and calls their shutdown functions if available.
// After calling the shutdown function, it frees the associated library using FreeLibrary.
void Shutdown() {
    // Perform shutdown logic for Manager
    // Iterate through the loaded files and call their shutdown functions if available
    for (const auto& file : Files) {
        if (file.Shutdown != nullptr) {
            file.Shutdown();  // Call the shutdown function
        }
    }
    for (const auto& file : Files) {
        FreeLibrary(file.Module);  // Free the associated library
        remove(file.NewPath);
    }
    // Clear the list
    Files.clear();
}
