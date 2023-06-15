#include <Windows.h>
#include <iostream>
using namespace std;
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {

    // Print the number of command-line arguments
   wcout << L"Number of command-line arguments: " << argc << endl;
   wcout << L"argv: "  << endl;
    // Loop through each command-line argument and print
    for (int i = 0; i < argc; i++)
        wcout << argv[i] << endl;
    wcout << L"envp: " << endl;
    // Loop through each environment variable and print
    for (wchar_t** env = envp; *env != 0; env++)
        wcout << *env << endl;
    return 0;
}