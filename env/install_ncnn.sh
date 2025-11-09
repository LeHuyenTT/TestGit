#!/bin/bash
set -e

echo "📦 Installing dependencies for ncnn..."
sudo apt update
sudo apt install -y build-essential git cmake libprotobuf-dev protobuf-compiler

# --- Định nghĩa biến tuyệt đối ---
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIBS_DIR="${ROOT_DIR}/../libs"

NCNN_SRC_DIR="${LIBS_DIR}/ncnn"
NCNN_BUILD_DIR="${NCNN_SRC_DIR}/build"
NCNN_INSTALL_DIR="${LIBS_DIR}/ncnn_bin"
NCNN_TAG="20240410"

# --- Chuẩn bị thư mục ---
echo "🗂 Preparing directories..."
rm -rf "$NCNN_SRC_DIR" "$NCNN_INSTALL_DIR"
mkdir -p "$NCNN_SRC_DIR" "$NCNN_INSTALL_DIR"

# --- Clone repo ---
echo "⬇️  Cloning ncnn (${NCNN_TAG})..."
git clone --depth 1 --branch "${NCNN_TAG}" https://github.com/Tencent/ncnn.git "$NCNN_SRC_DIR"
cd "$NCNN_SRC_DIR"
git submodule update --init

# --- Build ---
echo "⚙️  Building ncnn..."
mkdir -p build && cd build
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DNCNN_VULKAN=OFF \
  -DNCNN_BUILD_EXAMPLES=OFF \
  -DNCNN_BUILD_TOOLS=ON \
  -DNCNN_INSTALL_SDK=ON
make -j"$(nproc)"

# --- Cài đặt vào thư mục ncnn_bin ---
echo "📥 Installing to $NCNN_INSTALL_DIR ..."
cmake --install . --prefix "$NCNN_INSTALL_DIR"

# --- Dọn dẹp ---
cd "$ROOT_DIR"
rm -rf "$NCNN_SRC_DIR"

echo "✅ ncnn ${NCNN_TAG} built and installed successfully!"
echo "📁 Installed in: $NCNN_INSTALL_DIR"
