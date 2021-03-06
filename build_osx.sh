#!/bin/zsh
conan config install https://github.com/tinic/conan-profiles.git
mkdir -p build
cd build
conan install .. --profile osx_gcc10_x64 --build=missing
cmake -G "Ninja" .. -DCMAKE_CXX_COMPILER=g++-10 -DCMAKE_C_COMPILER=gcc-10
cmake --build .
