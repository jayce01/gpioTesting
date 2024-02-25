cmake_minimum_required(VERSION 3.0)
project(gpioTesting)

set(CMAKE_CXX_STANDARD 17)

add_executable(gpioTesting #main.cpp
        buttonTest.h
        buttonTest.cpp
        pigpio.h
        RaspLCDDriver/test.cpp
)