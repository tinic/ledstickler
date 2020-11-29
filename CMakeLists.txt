cmake_minimum_required (VERSION 3.13)
project(ledstickler)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable (ledstickler "")
target_sources (ledstickler PRIVATE main.cpp artnet.cpp timeline.cpp)
target_compile_options (ledstickler PRIVATE -flto -static-libgcc -Wall -Wextra -Wdouble-promotion -Wconversion -Wuseless-cast -Wlogical-op -Wshadow -Wfloat-conversion -Wnull-dereference -g -O3 -std=c++2a)
target_link_libraries(ledstickler PRIVATE http_parser fmt)
target_link_options (ledstickler PRIVATE -static-libgcc -flto)

if(MINGW)
    target_link_options (ledstickler PRIVATE -static)
    target_link_libraries(ledstickler PRIVATE mswsock ws2_32)
endif(MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_options (ledstickler PRIVATE -static)
    target_link_libraries(ledstickler PRIVATE pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
