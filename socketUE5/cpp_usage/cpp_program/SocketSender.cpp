#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
#endif

#include <iostream>
#include <string>
#include "SocketSender.h"

TCPClient::TCPClient() : connected(false) {
    Init();
}

TCPClient::~TCPClient() {
    Disconnect();
    Cleanup();
}

void TCPClient::Init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
    }
#endif
}

void TCPClient::Cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool TCPClient::Connect(const std::string& ip, int port) {
    if (connected)
        Disconnect();

#ifdef _WIN32
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        return false;
    }
#else
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        perror("Socket creation failed");
        return false;
    }
#endif

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        return false;
    }

#ifdef _WIN32
    if (connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(m_socket);
        return false;
    }
#else
    if (connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(m_socket);
        return false;
    }
#endif

    connected = true;
    std::cout << "Connected to " << ip << ":" << port << "\n";
    return true;
}

void TCPClient::Disconnect() {
    if (!connected) return;

#ifdef _WIN32
    closesocket(m_socket);
#else
    close(m_socket);
#endif

    connected = false;
}

bool TCPClient::Send(const std::string& message) {
    if (!connected) {
        std::cerr << "Not connected\n";
        return false;
    }

#ifdef _WIN32
    int sent = send(m_socket, message.c_str(), static_cast<int>(message.size()), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
        Disconnect();
        return false;
    }
#else
    ssize_t sent = send(m_socket, message.c_str(), message.size(), 0);
    if (sent < 0) {
        perror("Send failed");
        Disconnect();
        return false;
    }
#endif

    std::cout << "Sent " << sent << " bytes\n";
    return true;
}
