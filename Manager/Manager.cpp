#include "Manager.h"

// Function to check if the vector contains a File with a matching Path and return the associated Module
HMODULE Get(LPCWSTR Path) {
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

    wchar_t uuidString[40];  // Buffer to hold the UUID string
    UuidToStringW(&newUUID, (RPC_WSTR*)&uuidString);  // Convert the UUID to a string representation

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
    File newFile;
    newFile.OldPath = Path;
    newFile.NewPath = destinationPath.c_str();
    newFile.Module = module;
    // Get the "Init" function address from the module
    ModuleInit moduleInit = reinterpret_cast<ModuleInit>(GetProcAddress(module, "Init"));
    // Set the heartbeat function address from the module if available
    newFile.Heartbeat = reinterpret_cast<ModuleHeartbeat>(GetProcAddress(module, "Heartbeat"));
    // Set the shutdown function address from the module if available
    newFile.Shutdown = reinterpret_cast<ModuleShutdown>(GetProcAddress(module, "Shutdown"));
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
    HMODULE module = Get(Path);

    // Perform update logic if needed
}

// Function to remove a file
void Remove(LPCWSTR Path) {
    // Find the File with a matching Path using std::find_if
    auto it = find_if(Files.begin(), Files.end(),
        [Path](const File& file) {
            return wcscmp(file.OldPath, Path) == 0;
        });

    if (it != Files.end()) {
        // If a matching Path is found, free the associated library
        HMODULE module = it->Module;
        if (module != nullptr) {
            FreeLibrary(module);
        }
        // Remove the File from the vector
        Files.erase(it);
    }
}

// Init function
void Init(const wstring& servicesPath, const wstring& hmodulePath, HMODULE& manager) {
    // Initialization logic for the Manager
    // Perform necessary initialization steps using the updated hmodule, services, and manager references

    // Set the hmodule, services, and manager references with the provided paths
    _hmodule = hmodulePath;
    _services = servicesPath;
    _manager = manager;
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
        FreeLibrary(file.Module);  // Free the associated library
    }
}
