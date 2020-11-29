mkdir -p build
cd build
conan install .. --profile ..\conan_msys2_mingw.profile --build=missing
cmake -G "Ninja" ..
ninja
