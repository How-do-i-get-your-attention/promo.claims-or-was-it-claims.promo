#pragma once
#pragma warning(disable : 26495 6387)
#include <map>
#include <functional>
#include <iostream>
#include <Windows.h>
#include <iostream>
#include <stdexcept> 
#include <string>

namespace PCOrCP {
    enum RunAs {
        Setup,
        Background,
        LibraryBySetup,
        Library
    };

    struct Version {
        int MAJOR;
        int ID;
    };

    class Program {

    public:
        std::string RegisterDefaultPointerText = "# Registered Default Function(RDF)";
        HMODULE LibraryPath;
        std::string PointerText = "# Pointer(P)";
        std::map<std::string, std::pair<Version, std::function<void()>>> PointerMap;
        wchar_t** Argv;
        wchar_t** Envp;
        RunAs RunAsValue;
        Program() {}

        Program(wchar_t* envp[], wchar_t* argv[], RunAs RunAsValue) {
            this->Envp = envp;
            this->Argv = argv;
            this->RunAsValue = RunAsValue;

            switch (this->RunAsValue) {
            case Setup:
            case Background:
                SetLibrary();
                GetPointer("Erase", { 1,this->RunAsValue == Background ? 1 : 0 })();
                GetPointer("Setup", { 1,this->RunAsValue == Background ? 1 : 0 })();
                break;
            default:
                RegisterDefaultPointer("RegisterDefaultPointer", { 1, 0 }, "The RegisterDefaultPointer function is updated to match my provided signature, taking std::string Name, Version Version, and std::string Description as parameters.\nInside the function, I can perform any desired operations, such as adding the provided information to the documentation or performing further processing.");
                RegisterDefaultPointer("SetLibrary", { 1, 0}, "Function to set the pointer map.");
                RegisterDefaultPointer("GetPointer", { 1, 0 }, "Function to retrieve a function from the pointer map.");
                RegisterDefaultPointer("AddPointer", { 1, 0 }, "Function to add or update an entry in the pointer map.");
                RegisterDefaultPointer("ErasePointer", { 1, 0 }, "Function to remove in the pointer map.");



                AddPointer("Setup", { 1, 0 }, "Function to start Setup", []() {
                    std::cout << "Function to start Setup" << std::endl;
                    });
                AddPointer("Setup", { 1, 1 }, "Function to start Setup(Background)", []() {
                    std::cout << "Function to start Background" << std::endl;
                    });
                AddPointer("Erase", { 1, 0 }, "Function to Erase Setup 1.1", [this]() {
                    this->ErasePointer("Setup", { 1, 1 });
                });
                AddPointer("Erase", { 1, 1 }, "Function to Erase Setup 1.0", [this]() {
                    this->ErasePointer("Setup", { 1, 0 });
                });

               
                break;
            }
        }

        void RegisterDefaultPointer(std::string Name, Version Version, std::string Description) {
            if (this->RunAsValue != LibraryBySetup)
                return;
            RegisterDefaultPointerText += "\n\n## " + Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID) +  "\n" + Description;
        }
        void SetLibrary() {
            if (LibraryPath != nullptr) {
                FreeLibrary(LibraryPath);
                LibraryPath = nullptr;
            }
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(nullptr, path, MAX_PATH);
            std::wstring executablePath(path);
            std::wstring executableDir = executablePath.substr(0, executablePath.find_last_of(L"\\") + 1);
            std::wstring dllPath = executableDir + L"Library.dll";
            LibraryPath = LoadLibraryW(dllPath.c_str());

            typedef PCOrCP::Program(*InitFunc)(wchar_t* [], wchar_t* [], PCOrCP::RunAs);
            InitFunc initFunc = reinterpret_cast<InitFunc>(GetProcAddress(LibraryPath, "Init"));
            PCOrCP::Program program = initFunc(this->Envp, this->Argv, this->RunAsValue == Background ? Library : LibraryBySetup);

            this->PointerMap = program.PointerMap;

            if (this->RunAsValue == Setup) {
                std::cout << program.RegisterDefaultPointerText << std::endl << std::endl << program.PointerText << std::endl << std::endl << "@Documention its end next is console messegsas and test or result!" << std::endl << std::endl << std::endl << std::endl;
            }
        }

        std::function<void()> GetPointer(std::string Name, Version Version) {
            auto it = PointerMap.find(Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID));
            return it->second.second;
        }
        void ErasePointer(std::string Name, Version Version) {
            auto it = PointerMap.find(Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID));
            if (it != PointerMap.end()) {
                PointerMap.erase(Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID));
            }
        }

        void AddPointer(std::string Name, Version Version, std::string Description,  std::function<void()> Function) {
            auto it = PointerMap.find(Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID));
            if (it != PointerMap.end()) {
                if (this->RunAsValue == LibraryBySetup)
                    PointerText += "\n\n# Working on:";
            }
            if (this->RunAsValue == LibraryBySetup)
                PointerText += "\n\n## " + Name + " " + std::to_string(Version.MAJOR) + "." + std::to_string(Version.ID)  + "\n" + Description;
            PointerMap[Name + " " + std::to_string(Version.MAJOR) +"." + std::to_string(Version.ID)] = std::make_pair(Version, Function);
        }
    };
}
