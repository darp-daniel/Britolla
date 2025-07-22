#pragma once
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

class TCPClient
{
public:
    TCPClient();
    ~TCPClient();

    // Connect to server
    bool Connect(const std::string& ip = "127.0.0.1", int port = 12345);

    // Disconnect from server
    void Disconnect();

    // Send message
    bool Send(const std::string& message);

    // Check connection status
    bool IsConnected() const { return m_connected; }

private:
    SOCKET m_socket;
    bool m_connected;
    std::string m_serverIP;
    int m_serverPort;

    void InitializeWinsock();
    void CleanupWinsock();
};