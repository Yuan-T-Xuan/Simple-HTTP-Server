cmake_minimum_required(VERSION 3.8)
project(testflight)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c makesocket.c)
add_executable(testflight ${SOURCE_FILES})