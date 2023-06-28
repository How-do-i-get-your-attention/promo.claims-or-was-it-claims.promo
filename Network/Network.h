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
    class Client
    {
    public:
        SOCKET Socket;
        std::vector<char> Buffer;
        std::string IP;
        int Port;
        Client(SOCKET socket, const std::vector<char>& buffer, const std::string& ip, int port)
            : Socket(socket), Buffer(buffer), IP(ip), Port(port)
        {
        }
    };
    class Function
    {
    public:
        std::string Name;
        bool (*Run)(Client&);
    };
private:
    SOCKET Socket;
    sockaddr_in IPv4sockaddr_in;
    sockaddr_in6 IPv6sockaddr_in;
    std::vector<Function> Functions;
    bool IsRunning;
    void Receiver();
    std::time_t TimeoutTime;
    bool GetTimeout();
public:
    int AddressFamily;
    int Type;
    int Protocol;
    int Port;
    int TimeoutSeconds;
    int BufferSize;
    void ChangeTimeout(int timeoutSeconds);
    void UpdateTimeout();
    void ChangeBufferSize(int bufferSize);
    bool Have(const std::string& name) const;
    void Add(const std::string& name, bool (*run)(Client&));
    void Remove(const std::string& name);
    void Close();
    Network(int addressFamily, int type, int protocol, int port, int bufferSize, int timeoutSeconds);
};

extern std::vector<Network> Networks;
Network CreateSocket(int addressFamily, int type, int protocol, int port, int bufferSize, int timeoutSeconds = 0);
void CloseAllSocket();