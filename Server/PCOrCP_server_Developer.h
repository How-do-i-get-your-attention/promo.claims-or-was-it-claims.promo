#include "PCOrCP_server_Local.h"
#pragma once
namespace PCOrCP {
    namespace server {
        class Developer
        {
        private:
            vector<uint8_t> argvPath;
            vector<uint8_t> buildExecutableName = { 83, 101, 114, 118, 101, 114, 46, 101, 120, 101 };
            Local* local = nullptr;

            void Clear() {
                argvPath.clear();
                argvPath.shrink_to_fit();
                buildExecutableName.clear();
                buildExecutableName.shrink_to_fit();
            }
            void DeveloperExecutable() {
                Clear();
                local->Developer();
                delete local;
            }
        public:
            Developer(int argc, char* argv[]) {
                argvPath.assign(argv[0], argv[0] + strlen(argv[0]));
                local = new Local(argvPath);
                if (local->Type == Type::Developer) {
                    DeveloperExecutable();
                    return;
                }
                argvPath.erase(argvPath.begin(), argvPath.end() - buildExecutableName.size());
                if (argvPath == buildExecutableName) {
                    DeveloperExecutable();
                    return;
                }
                Clear();
                local->Services();
                delete local;
            }
        };
    }
}
