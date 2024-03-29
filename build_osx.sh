#!/bin/zsh
conan config install https://github.com/tinic/conan-profiles.git
mkdir -p build
cd build
conan install .. --profile osx_gcc12_x64 --build=missing
cmake -G "Ninja" .. -DCMAKE_CXX_COMPILER=g++-12 -DCMAKE_C_COMPILER=gcc-12
cmake --build .
