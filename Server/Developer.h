#include "Local.h"
#include <cstdint>
#pragma once
namespace PCOrCP {
    namespace server {
		class Developer
		{
		public:
			Developer(int argc, char* argv[])
			{

				vector<uint8_t> argvPath;
				for (int i = 0; argv[0][i] != '\0'; ++i) 
					argvPath.push_back(static_cast<uint8_t>(argv[0][i]));
				PCOrCP::server::Local local(argvPath);
				vector<uint8_t> buildExecutableName = {83, 101, 114, 118,101, 114, 46, 101,120, 101};
				argvPath.erase(argvPath.begin(), argvPath.end() - buildExecutableName.size());
				if (argvPath == buildExecutableName) {
					argvPath.clear();
					argvPath.shrink_to_fit();
					buildExecutableName.clear();
					buildExecutableName.shrink_to_fit();
					local.Developer();
					return;
				}
				local.Services();
				argvPath.clear();
				argvPath.shrink_to_fit();
				buildExecutableName.clear();
				buildExecutableName.shrink_to_fit();
			}
		};
    }
}