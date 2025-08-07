#pragma once
#include <string>

#ifdef _WIN32
  #include <WinSock2.h>
  #include <WS2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
#endif

class UDPClient
{
public:
    UDPClient();
    ~UDPClient();

    // Setup target address
    bool Connect(const std::string& ip , int port);

    // Send message
    bool Send(const std::string& message);

    bool IsConnected() const { return connected; }

    void Disconnect();

private:
#ifdef _WIN32
    SOCKET m_socket;
#else
    int m_socket;
#endif
    bool connected;
    sockaddr_in serverAddr;

    void Init();
    void Cleanup();
};
