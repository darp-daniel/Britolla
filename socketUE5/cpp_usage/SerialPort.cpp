#include "SerialPort.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

SerialPort::SerialPort(const std::string& portName, unsigned int baudRate) 
    : portName(portName), baudRate(baudRate), connected(false) {
    #ifdef _WIN32
    handle = INVALID_HANDLE_VALUE;
    #else
    fileDescriptor = -1;
    #endif
    
    if (!configurePort()) {
        throw std::runtime_error("Failed to configure serial port");
    }
}

SerialPort::~SerialPort() {
    close();
}

bool SerialPort::isConnected() const {
    return connected;
}

std::string SerialPort::getMessage() {
    if (!connected) return "";

    #ifdef _WIN32
    char buffer[256];
    DWORD bytesRead;
    
    if (ReadFile(handle, buffer, sizeof(buffer), &bytesRead, NULL)) {
        if (bytesRead > 0) {
            return std::string(buffer, bytesRead);
        }
    }
    #else
    char buffer[256];
    int n = read(fileDescriptor, buffer, sizeof(buffer));
    if (n > 0) {
        buffer[n] = '\0';
        return std::string(buffer);
    }
    #endif
    
    return "";
}

void SerialPort::close() {
    if (!connected) return;

    #ifdef _WIN32
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
    #else
    if (fileDescriptor != -1) {
        ::close(fileDescriptor);
        fileDescriptor = -1;
    }
    #endif
    
    connected = false;
}

bool SerialPort::configurePort() {
    #ifdef _WIN32
    handle = CreateFile(portName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(handle, &dcbSerialParams)) {
        CloseHandle(handle);
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(handle, &dcbSerialParams)) {
        CloseHandle(handle);
        return false;
    }

    // Set timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(handle, &timeouts)) {
        CloseHandle(handle);
        return false;
    }
    #else
    fileDescriptor = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fileDescriptor == -1) {
        return false;
    }

    // Make sure the file descriptor is in blocking mode
    fcntl(fileDescriptor, F_SETFL, 0);

    struct termios tty;
    if (tcgetattr(fileDescriptor, &tty)) {
        ::close(fileDescriptor);
        return false;
    }

    cfsetispeed(&tty, baudRate);
    cfsetospeed(&tty, baudRate);

    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 data bits
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem control lines

    // Non-canonical mode
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    tty.c_oflag &= ~OPOST; // Raw output

    // Set timeouts
    tty.c_cc[VMIN] = 0;  // Non-blocking read
    tty.c_cc[VTIME] = 5; // 0.5 second read timeout

    if (tcsetattr(fileDescriptor, TCSANOW, &tty)) {
        ::close(fileDescriptor);
        return false;
    }
    #endif

    connected = true;
    return true;
}