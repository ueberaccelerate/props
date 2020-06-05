#!/bin/sh

set -e
# Uninstall the default version provided by Ubuntu's package manager
sudo apt remove --purge --auto-remove cmake

# Go to the official CMake webpage, then download and install
version=3.16
build=5
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build-Linux-x86_64.sh 

sudo mkdir /opt/cmake
sudo sh cmake-$version.$build-Linux-x86_64.sh --prefix=/opt/cmake

sudo ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
