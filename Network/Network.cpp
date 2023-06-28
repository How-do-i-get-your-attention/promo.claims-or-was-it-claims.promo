#include "Network.h"
std::vector<Network> Networks;
void Network::ChangeTimeout(int timeoutSeconds)
{
    TimeoutSeconds = timeoutSeconds;
    RunTimeOut = TimeoutSeconds != 0;
}
bool Network::Have(const std::string& name) const
{
    for (const auto& function : Functions) {
        if (function.Name == name) {
            return true; 
        }
    }
    return false;
}
void Network::Add(const std::string& name, void (*run)())
{
    if (Have(name)) {
        return; 
    }

    Function newFunction;
    newFunction.Name = name;
    newFunction.Run = run;
    Functions.push_back(newFunction);
}
void Network::Remove(const std::string& name)
{
    auto it = std::find_if(Functions.begin(), Functions.end(), [&](const Function& function) {
        return function.Name == name;
        });
    if (it != Functions.end()) {
        Functions.erase(it);
    }
}
Network::Network(int addressFamily, int type, int protocol, int port, int timeoutSeconds)
{
    Socket = socket(AddressFamily = addressFamily, Type = type, Protocol = protocol);
    if (AddressFamily == AF_INET6) {
        IPv6sockaddr_in.sin6_family = AddressFamily;
        IPv6sockaddr_in.sin6_addr = in6addr_any;
        IPv6sockaddr_in.sin6_port = htons(Port = port);
    }
    else {
        IPv4sockaddr_in.sin_family = AddressFamily;
        IPv4sockaddr_in.sin_addr.s_addr = INADDR_ANY;
        IPv4sockaddr_in.sin_port = htons(Port = port);
    }
    ChangeTimeout(timeoutSeconds);
}
