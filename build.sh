#!/usr/bin/env sh

set -x

if [ -d build ] || [ -f build ]; then
    rm -rf build
fi

mkdir build
cd build

conan install .. --build missing
cmake ..
cmake --build .