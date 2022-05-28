#!/bin/sh
conan config install https://github.com/tinic/conan-profiles.git
mkdir -p build
cd build
conan install .. --profile linux_gcc11_x64 --build=missing
cmake -G "Ninja" ..
cmake --build .
