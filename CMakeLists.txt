cmake_minimum_required(VERSION 3.7)
project(myshell)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp util.h struct.h define.h)
add_executable(myshell ${SOURCE_FILES})