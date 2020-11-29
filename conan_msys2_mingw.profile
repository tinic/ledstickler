toolchain=C:/msys64/mingw64/bin
target_host=x86_64-w64-mingw32
cc_compiler=gcc
cxx_compiler=g++

[env]
CONAN_CMAKE_FIND_ROOT_PATH=$toolchain
CHOST=$target_host
AR=$target_host-ar
AS=$target_host-as
RANLIB=$target_host-ranlib
CC=$target_host-$cc_compiler
CXX=$target_host-$cxx_compiler
STRIP=$target_host-strip
RC=$target_host-windres

[settings]
os_build=Windows
os=Windows
arch=x86_64
arch_build=x86_64
compiler=gcc
compiler.libcxx=libstdc++11
compiler.cppstd=20
compiler.version=10
build_type=Release

[options]
[env]
CC=C:/msys64/mingw64/bin/gcc
CXX=C:/msys64/mingw64/bin/g++

