#include "SocketSender.h"
#include "SerialPort.h"

#include <thread>
#include <chrono>
#include <iostream>

int main() {
    try {
        #ifdef _WIN32
            SerialPort serial("COM3", 115200);
        #else
            SerialPort serial("/dev/ttyUSB0", 115200);
        #endif
        TCPClient client;

        if (!serial.isConnected()) {
            std::cerr << "Failed to connect to serial port" << std::endl;
            return 1;
        }

        if (!client.Connect("127.0.0.1", 12345)) {
            std::cerr << "Não conectou com a UE. \n";
            return 1;
        }

        std::cout << "Conectado ao UE";

        std::string lstMessage;
        while (serial.isConnected() && client.IsConnected()){
            std::string msg = serial.getMessage();
            if(!msg.empty()){
                if(msg != lstMessage){
                    lstMessage = msg;
                    std::cout << "Enviando mensagem: " << msg << "\n";
                    client.Send(msg);
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(100));
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
