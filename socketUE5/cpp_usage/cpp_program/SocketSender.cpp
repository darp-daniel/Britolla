#include "SocketSender.h"
#include <iostream>

UDPClient::UDPClient() : connected(false) {
    Init();
}

UDPClient::~UDPClient() {
    Disconnect();
    Cleanup();
}

void UDPClient::Init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
    }
#endif
}

void UDPClient::Cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool UDPClient::Connect(const std::string& ip, int port) {
    if (connected)
        Disconnect();

#ifdef _WIN32
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        return false;
    }
#else
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket < 0) {
        perror("Socket creation failed");
        return false;
    }
#endif

    serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        return false;
    }

    connected = true;
    std::cout << "UDP target set to " << ip << ":" << port << "\n";
    return true;
}

void UDPClient::Disconnect() {
    if (!connected) return;

#ifdef _WIN32
    closesocket(m_socket);
#else
    close(m_socket);
#endif

    connected = false;
}

bool UDPClient::Send(const std::string& message) {
    if (!connected) {
        std::cerr << "UDP target not set\n";
        return false;
    }

#ifdef _WIN32
    int sent = sendto(m_socket, message.c_str(), static_cast<int>(message.size()), 0,
                      (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
        return false;
    }
#else
    ssize_t sent = sendto(m_socket, message.c_str(), message.size(), 0,
                          (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sent < 0) {
        perror("Send failed");
        return false;
    }
#endif

    std::cout << "Sent " << sent << " bytes via UDP\n";
    return true;
}
