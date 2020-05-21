cmake_minimum_required (VERSION 3.13)
project(ledstickler)

add_executable (ledstickler "")
target_sources (ledstickler PRIVATE main.cpp datagram.cpp)

target_compile_options (ledstickler PRIVATE -Wall -O3 -std=c++2a)
