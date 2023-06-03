//#include <iostream>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//
//#pragma comment(lib, "ws2_32.lib")
//
//int main() {
//    const char* domain = "how-do-i-get-your-attention.com";
//    WSADATA wsaData;
//
//    // Initialize Winsock
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        std::cerr << "WSAStartup failed" << std::endl;
//        return 1;
//    }
//
//    struct addrinfo* result = nullptr;
//    struct addrinfo hints {};
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    // Call getaddrinfo to retrieve DNS information
//    int status = getaddrinfo(domain, nullptr, &hints, &result);
//    if (status != 0) {
//        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // Traverse the linked list of addrinfo structures
//    for (struct addrinfo* addr = result; addr != nullptr; addr = addr->ai_next) {
//        if (addr->ai_family == AF_INET) {
//            // IPv4 address
//            struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(addr->ai_addr);
//            char ip[INET_ADDRSTRLEN];
//            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
//            std::cout << "IPv4 Address: " << ip << std::endl;
//        }
//        else if (addr->ai_family == AF_INET6) {
//            // IPv6 address
//            struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(addr->ai_addr);
//            char ip[INET6_ADDRSTRLEN];
//            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
//            std::cout << "IPv6 Address: " << ip << std::endl;
//        }
//    }
//
//    // Free the memory allocated by getaddrinfo
//    freeaddrinfo(result);
//
//    // Cleanup Winsock
//    WSACleanup();
//
//    return 0;
//}
