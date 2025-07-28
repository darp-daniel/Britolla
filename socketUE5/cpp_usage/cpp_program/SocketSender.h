#pragma once
#include <string>
#ifdef _WIN32
  #include <WinSock2.h>
  #include <WS2tcpip.h>
  #pragma comment(lib, "ws2_32.lib") // Só Windows
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
#endif

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
    #ifdef _WIN32
        SOCKET m_socket;
    #else
        int m_socket;
    #endif
    bool m_connected;
    std::string m_serverIP;
    int m_serverPort;

    void InitializeWinsock();
    void CleanupWinsock();
};