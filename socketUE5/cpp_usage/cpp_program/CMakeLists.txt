cmake_minimum_required(VERSION 3.10)
project(SerialToUE5)

set(CMAKE_CXX_STANDARD 17)

add_executable(SerialToUE5 
    main.cpp
    SerialPort.cpp
    SocketSender.cpp)

target_link_libraries(SerialToUE5 pthread)
