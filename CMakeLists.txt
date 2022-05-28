cmake_minimum_required (VERSION 3.13)
project(ledstickler)

set(CONAN_DISABLE_CHECK_COMPILER True CACHE BOOL "" FORCE )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable (ledstickler "")
target_sources (ledstickler PRIVATE main.cpp artnet.cpp timeline.cpp)
target_link_libraries(ledstickler PRIVATE http_parser fmt)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options (ledstickler PRIVATE /std:c++17 /Oxs -D_WIN32_WINNT=0x0601)
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options (ledstickler PRIVATE -flto -static-libgcc -Wall -Wextra -Wdouble-promotion -Wconversion -Wuseless-cast -Wlogical-op -Wshadow -Wfloat-conversion -Wnull-dereference -g -O3 -std=c++2a)
	target_link_options (ledstickler PRIVATE -static-libgcc -flto)
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options (ledstickler PRIVATE -flto -Wall -Wextra -Wdouble-promotion -Wconversion -Wshadow -Wfloat-conversion -Wnull-dereference -Wno-missing-braces -g -O3 -std=c++2a)
	target_link_options (ledstickler PRIVATE -flto)
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 
if(MINGW)
    target_link_options (ledstickler PRIVATE -static)
    target_link_libraries(ledstickler PRIVATE mswsock ws2_32)
endif(MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_options (ledstickler PRIVATE -static)
    target_link_libraries(ledstickler PRIVATE pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
