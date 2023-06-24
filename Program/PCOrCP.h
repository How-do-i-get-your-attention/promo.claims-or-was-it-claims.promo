#pragma once
#pragma warning(disable : 26495 6387)
#include <map>
#include <functional>
#include <iostream>
#include <Windows.h>
namespace PCOrCP {
    enum RunAs {
        Setup,
        Background,
        Library
    };
    struct Version {
        int MAJOR;
        int MINOR;
        int PATCH;
        std::string  Description;
    };
    class Program {
    public:
        std::map<std::string, std::pair<Version, std::function<void()>>> LibraryMapVoid;
        PCOrCP::Program& GetModuleProgram() {
            typedef void (*InitFunc)(wchar_t* [], wchar_t* []);
            typedef Program& (*GetModuleProgramFunc)();
            GetModuleProgramFunc getModuleProgramFunc = reinterpret_cast<GetModuleProgramFunc>(GetProcAddress(Module, "GetModuleProgram"));
            return getModuleProgramFunc();
        }
        void UpdateProgram() {
            HMODULE Module = LoadLibrary("Library.dll");
            typedef void (*InitFunc)(wchar_t* [], wchar_t* []);
            InitFunc initFunc = reinterpret_cast<InitFunc>(GetProcAddress(Module, "Init"));
            initFunc(Program::Envp, Program::Argv);
            LibraryMap();
            FreeLibrary(Module);
        }
        void LibraryMap() {
            LibraryMapVoid = GetModuleProgram().LibraryMapVoid;
        }
        Version LibraryMapVoidVersion(std::string Name) {
            auto it = LibraryMapVoid.find(Name);
            return it->second.first;
        }
        std::function<void()> LibraryMapVoidFunction(std::string Name) {
            auto it = LibraryMapVoid.find(Name);
            return it->second.second;
        }
        void RemoveLibraryMap(std::string Name) {
            auto it = LibraryMapVoid.find(Name);
            if (it != LibraryMapVoid.end())
                LibraryMapVoid.erase(it);
        }
        void AddLibraryMap(std::string Name, Version Version, std::function<void()> Function) {
            auto it = LibraryMapVoid.find(Name);
            if (it != LibraryMapVoid.end()) {
                it->second.first = Version;
                it->second.second = Function;
            }
            else
                LibraryMapVoid[Name] = std::make_pair(Version, Function);
        }
        wchar_t** Argv;
        wchar_t** Envp;
        static const RunAs RunAsValue;
        Program() {}
        Program(wchar_t* envp[], wchar_t* argv[]) {
            Program::Envp = envp;
            Program::Argv = argv;
            if (RunAsValue != Library) {
                UpdateProgram();
                if (RunAsValue == Background) {
                    LibraryMapVoid["Background:Program"] = std::make_pair(Version{ 1, 0, 0,"Background is services that will run in windows control manager as Background.dll" }, [this]() {
                        std::cout << "call Background" << std::endl;
                     });
                    LibraryMapVoidFunction("Background:Program")();
                }
                else {
                    LibraryMapVoid["Setup:Program"] = std::make_pair(Version{ 1, 0, 0,"Setup is settings for for start then will run on Setup.exe" },[this]() {
                            std::cout << "call Setup" << std::endl;
                        });
                    LibraryMapVoidFunction("Setup:Program")();
                }
                exit(0);
            }
            else
            {
                AddLibraryMap("Setup:Program", { 0 , 0 , 0 }, [this]() {
                    std::cout << "call Setup" << std::endl;
                    });
                AddLibraryMap("Update", Version{ 1, 0, 0,"Update not finnish" }, [this]() { UpdateProgram(); });
            }
        }
    };
}
