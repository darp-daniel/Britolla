#include "SocketSender.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

TCPClient::TCPClient() : m_socket(INVALID_SOCKET), m_connected(false), m_serverPort(0)
{
    InitializeWinsock();
}

TCPClient::~TCPClient()
{
    Disconnect();
    CleanupWinsock();
}

void TCPClient::InitializeWinsock()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
    }
}

void TCPClient::CleanupWinsock()
{
    WSACleanup();
}

bool TCPClient::Connect(const std::string& ip, int port)
{
    if (m_connected) Disconnect();

    m_serverIP = ip;
    m_serverPort = port;

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return false;
    }

    m_connected = true;
    std::cout << "Connected to " << ip << ":" << port << "\n";
    return true;
}

void TCPClient::Disconnect()
{
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    m_connected = false;
}

bool TCPClient::Send(const std::string& message)
{
    if (!m_connected) {
        std::cerr << "Not connected to server\n";
        return false;
    }

    int bytesSent = send(m_socket, message.c_str(), static_cast<int>(message.size()), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
        Disconnect();
        return false;
    }

    std::cout << "Sent " << bytesSent << " bytes\n";
    return true;
}