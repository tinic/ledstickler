cc_compiler=gcc-10
cxx_compiler=g++-10

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
os_build=Macos
os=Macos
arch=x86_64
arch_build=x86_64
compiler=gcc
compiler.libcxx=libstdc++11
compiler.cppstd=20
compiler.version=10
build_type=Release

[options]
[env]
CC=gcc-10
CXX=g++-10

