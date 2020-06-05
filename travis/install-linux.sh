#!/bin/sh

set -e
# Uninstall the default version provided by Ubuntu's package manager
sudo apt remove --purge --auto-remove cmake


version=3.16
build=5
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/

./bootstrap
make -j$(nproc)
sudo make install
