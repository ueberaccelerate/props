#!/bin/sh

set -e

mkdir build
cd build

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    cmake .. -GXcode
else
    cmake ..
fi

cmake --build .


