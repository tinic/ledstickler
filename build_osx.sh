#!/bin/zsh
mkdir -p build
cd build
conan install .. --profile ../conan_osx.profile --build=missing
cmake .. -DCMAKE_CXX_COMPILER=g++-10 -DCMAKE_C_COMPILER=gcc-10
make -j

