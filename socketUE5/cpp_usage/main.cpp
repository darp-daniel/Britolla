#include "SocketSender.h"
#include "SocketServer.h"
#include "SerialPort.h"

#include <thread>
#include <chrono>
#include <iostream>

int main() {
    try {
        // Replace with your actual port name
        #ifdef _WIN32
        SerialPort serial("COM3", 9600);
        #else
        SerialPort serial("/dev/ttyACM0", 9600);
        #endif

        TCPClient client;

        if (!serial.isConnected()) {
            std::cerr << "Failed to connect to serial port" << std::endl;
            return 1;
        }

        std::cout << "Connected to serial port. Waiting for messages..." << std::endl;
        if (client.Connect("127.0.0.1", 12345)) {
            while (serial.isConnected()) {
                std::string message = serial.getMessage();
                client.Send(message);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }   

        serial.close();
        client.Disconnect();
        std::cout << "Disconnected from serial port and server." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}