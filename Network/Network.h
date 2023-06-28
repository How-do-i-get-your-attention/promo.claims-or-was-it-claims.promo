#pragma once
#pragma warning(disable : 4309 6031 26495)
#include <thread>
#include <Ws2tcpip.h>
#include <string>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
WSADATA windowsSocketsData;
class Network
{
    class Function
    {
    public:
        std::string Name;
        void (*Run)();
    };
private:
    SOCKET Socket;
    sockaddr_in IPv4sockaddr_in;
    sockaddr_in6 IPv6sockaddr_in;
    bool RunTimeOut;
    std::vector<Function> Functions;
public:
    int AddressFamily;
    int Type;
    int Protocol;
    int Port;
    int TimeoutSeconds;
    void ChangeTimeout(int timeoutSeconds);
    bool Have(const std::string& name) const;
    void Add(const std::string& name, void (*run)());
    void Remove(const std::string& name);
    Network(int addressFamily, int type, int protocol, int port, int timeoutSeconds);
};
extern std::vector<Network> Networks;
Network CreateSocket(int addressFamily, int type, int protocol, int port, int timeoutSeconds = 0);