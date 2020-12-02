"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
mkdir -p build
cd build
conan install .. --build=missing
cmake ..
cmake --build . --config Release

