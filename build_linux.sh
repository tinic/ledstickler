#!/bin/sh
mkdir -p build
cd build
conan install .. --profile ../conan_linux.profile --build=missing
cmake -G "Ninja" ..
ninja
