#!/bin/bash
set -e  # dừng ngay nếu có lỗi

CMAKE_VER="3.20.0"

echo "Installing CMake $CMAKE_VER ..."

# cleanup old build folders if exist
rm -rf "cmake-$CMAKE_VER" "cmake-$CMAKE_VER.tar.gz"

# install deps
sudo apt update
sudo apt install -y build-essential libssl-dev

# download + build
wget "https://github.com/Kitware/CMake/releases/download/v$CMAKE_VER/cmake-$CMAKE_VER.tar.gz"
tar -xzf "cmake-$CMAKE_VER.tar.gz"
cd "cmake-$CMAKE_VER"

./bootstrap
make -j$(nproc)
sudo make install

# reset shell cache
hash -r

echo "✅ CMake $(cmake --version | head -n1) installed at $(which cmake)"

# cleanup
cd ..
rm -rf "cmake-$CMAKE_VER" "cmake-$CMAKE_VER.tar.gz"
