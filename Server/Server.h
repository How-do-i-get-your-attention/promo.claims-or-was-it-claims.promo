#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
namespace fs = std::filesystem;
using namespace std;
#pragma once
namespace server {

}
class Program
{
public: 
	Program(const char* programName);
	~Program();
	vector<uint8_t> Drive;
	string DriveAsString;
	char* DriveAsChar;
	SC_HANDLE ServicesManager;
	void Error(const std::exception& exception);
	void Messages(const std::string& messesges);
	
};