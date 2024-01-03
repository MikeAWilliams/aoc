#!/bin/bash

# find vcpkg in $PATH
VCPKG_ROOT="$( dirname "$( which vcpkg )" )"
echo $VCPKG_ROOT

#export CC=/usr/bin/clang
#export CXX=/usr/bin/clang++
rm -rf build
mkdir build
cd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build .
./day7