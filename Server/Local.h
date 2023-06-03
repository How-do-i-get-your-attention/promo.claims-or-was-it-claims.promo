#pragma once
#include <Windows.h>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;


#include <iomanip>
#include <iostream>
using namespace std;

namespace PCOrCP {
    namespace server {
		class Local
		{
		private:
			vector<uint8_t> argvPath;
			vector<uint8_t> drive;
		public:
			void Developer() {
				std::cout << "Developer" << std::endl;
				this->Services();

			}
			void Services() {
				std::cout << "Services" << std::endl;
			}
			Local(vector<uint8_t> argvPath) {
				this->argvPath = argvPath;
				{
					char windowsPath[MAX_PATH];
					(void)GetWindowsDirectoryA(windowsPath, MAX_PATH);
					drive.insert(drive.end(), windowsPath, windowsPath + 3);
					vector<uint8_t> path = { 80, 67, 79, 114, 67, 80 };
					drive.insert(drive.end(), path.begin(), path.end());
					std::string driveString(drive.begin(), drive.end());
					if (!fs::is_directory(fs::status(driveString))) 
						fs::create_directory(driveString);
					std::cout << "Drive string: " << driveString << std::endl;
				}




			}
			~Local() {
				argvPath.clear();
				argvPath.shrink_to_fit();
				drive.clear();
				drive.shrink_to_fit();
			}
		};
    }
}