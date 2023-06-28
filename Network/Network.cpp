#include "Network.h"
#include <chrono>
std::vector<Network> Networks;
void Network::ChangeTimeout(int timeoutSeconds)
{
    TimeoutSeconds = timeoutSeconds;
}
bool Network::Have(const std::string& name) const
{
    for (const auto& function : Functions)
    {
        if (function.Name == name)
            return true;
    }
    return false;
}
void Network::Add(const std::string& name, bool (*run)(Client&))
{
    if (Have(name))
        return;

    Function newFunction;
    newFunction.Name = name;
    newFunction.Run = run;
    Functions.push_back(newFunction);

    if (!IsRunning)
    {
        IsRunning = true;
        std::thread threadReceiver(&Network::Receiver, this);
        threadReceiver.detach();
    }
}
void Network::Remove(const std::string& name)
{
    auto it = std::find_if(Functions.begin(), Functions.end(), [&](const Function& function) { return function.Name == name; });
    if (it != Functions.end())
        Functions.erase(it);
}
void Network::ChangeBufferSize(int bufferSize)
{
    BufferSize = bufferSize;
    UpdateTimeout();
}
void Network::UpdateTimeout()
{
    if (TimeoutSeconds == 0)
        return;
    auto currentTime = std::chrono::system_clock::now();
    auto timeoutDuration = std::chrono::seconds(TimeoutSeconds);
    std::chrono::time_point<std::chrono::system_clock> timeoutTime = currentTime + timeoutDuration;
    TimeoutTime = std::chrono::system_clock::to_time_t(timeoutTime);
}
bool Network::GetTimeout()
{
    if (TimeoutSeconds == 0)
        return true;
    auto currentTime = std::chrono::system_clock::now();
    auto timeoutTime = std::chrono::system_clock::from_time_t(TimeoutTime);
    return currentTime <= timeoutTime;
}
Network::Network(int addressFamily, int type, int protocol, int port, int bufferSize, int timeoutSeconds)
{
    Socket = socket(AddressFamily = addressFamily, Type = type, Protocol = protocol);
    if (AddressFamily == AF_INET6)
    {
        IPv6sockaddr_in.sin6_family = AddressFamily;
        IPv6sockaddr_in.sin6_addr = in6addr_any;
        IPv6sockaddr_in.sin6_port = htons(Port = port);
        bind(Socket, reinterpret_cast<SOCKADDR*>(&IPv6sockaddr_in), sizeof(IPv6sockaddr_in));
    }
    else
    {
        IPv4sockaddr_in.sin_family = AddressFamily;
        IPv4sockaddr_in.sin_addr.s_addr = INADDR_ANY;
        IPv4sockaddr_in.sin_port = htons(Port = port);
        bind(Socket, reinterpret_cast<SOCKADDR*>(&IPv4sockaddr_in), sizeof(IPv4sockaddr_in));
    }

    ChangeTimeout(timeoutSeconds);
    ChangeBufferSize(bufferSize);
}
void Network::Receiver()
{
    if (Type == SOCK_DGRAM)
    {
        while (!Functions.empty()&&GetTimeout())
        {
           
            std::vector<char> buffer(BufferSize);
            if (AddressFamily == AF_INET6) {
                sockaddr_in6 clientAddr;
                int clientAddrSize = sizeof(clientAddr);
                int receivedBytes = recvfrom(Socket, buffer.data(), BufferSize, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), &clientAddrSize);
                if (receivedBytes > 0)
                {
                    UpdateTimeout();
                    char clientIP[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, &(clientAddr.sin6_addr), clientIP, INET6_ADDRSTRLEN);
                    int clientPort = ntohs(clientAddr.sin6_port);
                    Client client(Socket, buffer, clientIP, clientPort);
                    for (const auto& function : Functions)
                        if (function.Run(client))
                            break;
                }
            }
            else
            {
                sockaddr_in clientAddr;
                int clientAddrSize = sizeof(clientAddr);
                int receivedBytes = recvfrom(Socket, buffer.data(), BufferSize, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), &clientAddrSize);
                if (receivedBytes > 0)
                {
                    UpdateTimeout();
                    Client client(Socket, buffer, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    for (const auto& function : Functions)
                        if (function.Run(client))
                            break;
                }
            }
        }
    }
    else
    {
        while (!Functions.empty() && GetTimeout())
        {
            std::vector<char> buffer(BufferSize);
            std::vector<char> buffer(BufferSize);
            if (AddressFamily == AF_INET6) {
                sockaddr_in6 clientAddr;
                int clientAddrSize = sizeof(clientAddr);
                int receivedBytes = recv(Socket, buffer.data(), BufferSize, 0);
                if (receivedBytes > 0)
                {
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    getpeername(Socket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrSize);
                    char clientIP[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, &(clientAddr.sin6_addr), clientIP, INET6_ADDRSTRLEN);
                    Client client(Socket, buffer, clientIP, ntohs(clientAddr.sin6_port));
                    for (const auto& function : Functions)
                        if (function.Run(client))
                            break;
                }
            }
            else
            {
                int receivedBytes = recv(Socket, buffer.data(), BufferSize, 0);
                if (receivedBytes > 0)
                {
                    sockaddr_in clientAddr;
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    getpeername(Socket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrSize);
                    Client client(Socket, buffer, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    for (const auto& function : Functions)
                        if (function.Run(client))
                            break;
                }
            }
        }
    }
    IsRunning = false;
    if (TimeoutSeconds != 0) {
        Close();
    }
}
void Network::Close()
{
    Functions.clear();
    if (Socket != INVALID_SOCKET)
    {
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
    IsRunning = false;
    auto it = std::find(Networks.begin(), Networks.end(), *this);
    if (it != Networks.end())
    {
        Networks.erase(it);
    }
}
Network CreateSocket(int addressFamily, int type, int protocol, int port, int bufferSize, int timeoutSeconds)
{
    for (auto& network : Networks)
    {
        if (network.AddressFamily == addressFamily && network.Type == type && network.Protocol == protocol && network.Port == port)
        {
            if (network.TimeoutSeconds != timeoutSeconds)
            {
                network.ChangeTimeout(timeoutSeconds);
            }
            return network;
        }
    }

    Network newNetwork(addressFamily, type, protocol, port, bufferSize, timeoutSeconds);
    Networks.push_back(newNetwork);
    return newNetwork;
}
void CloseAllSocket() {
    for (auto& network : Networks)
    {
        network.Close();
    }
}