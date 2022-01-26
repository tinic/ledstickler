#!/bin/zsh
conan config install https://github.com/tinic/conan-profiles.git
mkdir -p build
cd build
conan install .. --profile osx_gcc11_x64 --build=missing
cmake -G "Ninja" .. -DCMAKE_CXX_COMPILER=g++-11 -DCMAKE_C_COMPILER=gcc-11
cmake --build .
