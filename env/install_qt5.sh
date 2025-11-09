#!/bin/bash
set -e

echo "📦 Installing Qt5 and build dependencies..."

# Update package list
sudo apt update

# Install modern Qt5 packages (qt5-default is deprecated)
sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
    qtdeclarative5-dev qtquickcontrols2-5-dev \
    qml-module-qtquick-controls2 qml-module-qtquick-layouts \
    qml-module-qtgraphicaleffects qml-module-qtqml \
    qml-module-qtqml-models2 qml-module-qtquick-window2 \
    qtmultimedia5-dev qml-module-qtmultimedia  # 👈 Thêm dòng này

# Optional: install Qt Creator IDE (if you want GUI IDE inside container)
# sudo apt install -y qtcreator

# Install dependencies for building OpenCV or other C++ libs
sudo apt install -y build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb12 libtbbmalloc2 libtbb-dev libdc1394-dev

echo "✅ Qt5 (with Multimedia) and OpenCV build dependencies installed successfully."
