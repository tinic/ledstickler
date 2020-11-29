cmake_minimum_required (VERSION 3.13)
project(ledstickler)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable (ledstickler "")
target_sources (ledstickler PRIVATE main.cpp artnet.cpp timeline.cpp)
target_compile_options (ledstickler PRIVATE -flto -static-libgcc -Wall -Wextra -Wdouble-promotion -Wconversion -Wuseless-cast -Wlogical-op -Wshadow -Wfloat-conversion -Wnull-dereference -g -O3 -std=c++2a)

if(MINGW)
    target_compile_definitions(ledstickler PRIVATE WIN32)
    target_link_options (ledstickler PRIVATE -static -static-libgcc -flto)
    target_link_libraries(ledstickler ws2_32 http_parser mswsock fmt)
endif(MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_options (ledstickler PRIVATE -static-libgcc -flto)
    target_link_libraries(ledstickler http_parser fmt)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_options (ledstickler PRIVATE -static -static-libgcc -flto)
    target_link_libraries(ledstickler http_parser fmt pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
