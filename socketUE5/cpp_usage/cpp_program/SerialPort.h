#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>

class SerialPort {
public:
    SerialPort(const std::string& portName, unsigned int baudRate);
    ~SerialPort();

    bool isConnected() const;
    std::string getMessage();
    void close();

private:
    #ifdef _WIN32
    void* handle;
    #else
    int fileDescriptor;
    #endif
    
    std::string portName;
    unsigned int baudRate;
    bool connected;

    bool configurePort();
    std::string readBuffer;
};

#endif // SERIALPORT_H
