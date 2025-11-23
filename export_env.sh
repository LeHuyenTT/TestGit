#!/bin/bash
# Tự động cấu hình biến môi trường local libs

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIBS_DIR="$ROOT_DIR/libs"

# --- Qt5 local install ---
export PATH="$LIBS_DIR/qt5_bin/usr/lib/qt5/bin:$PATH"
export QTDIR="$LIBS_DIR/qt5_bin/usr"
export CMAKE_PREFIX_PATH="$LIBS_DIR/qt5_bin/usr/lib/x86_64-linux-gnu/cmake:$CMAKE_PREFIX_PATH"
export Qt5_DIR="$LIBS_DIR/qt5_bin/usr/lib/x86_64-linux-gnu/cmake/Qt5"

# --- CMake local ---
export PATH="$LIBS_DIR/cmake_bin/bin:$PATH"

# --- OpenCV local ---
export OpenCV_DIR="$LIBS_DIR/cv2_bin/lib/cmake/opencv4"
export LD_LIBRARY_PATH="$LIBS_DIR/cv2_bin/lib:$LD_LIBRARY_PATH"

# --- ncnn local ---
export ncnn_DIR="$LIBS_DIR/ncnn_bin/install/lib/cmake/ncnn"

# --- SocketIO lib ---
export PATH_LIB_SOCKET="$LIBS_DIR/SocketIO_bin/libsioclient.a"

echo "✅ Local environment exported successfully."
echo "   Qt5_DIR=$Qt5_DIR"
echo "   OpenCV_DIR=$OpenCV_DIR"
