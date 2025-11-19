#!/bin/bash
set -e
QT_BRANCH="5.15"   # dùng nhánh 5.15 thay vì tag riêng
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIBS_DIR="${ROOT_DIR}/libs"
QT_SRC_DIR="${LIBS_DIR}/qt_src"
QT_INSTALL_DIR="${LIBS_DIR}/qt5_bin"

echo "📦 Building Qt $QT_BRANCH (community branch, GCC 13 ready)..."

# --- Step 1: install dependencies ---
sudo apt-get update -y
sudo apt-get install -y build-essential perl python3 git \
  libgl1-mesa-dev libxkbcommon-x11-dev libpulse-dev libasound2-dev \
  libfontconfig1-dev libdbus-1-dev libxcb1-dev libx11-dev libxext-dev \
  libxrender-dev libxi-dev libxrandr-dev libxcursor-dev libxfixes-dev

# --- Step 2: prepare directories ---
rm -rf "$QT_SRC_DIR" "$QT_INSTALL_DIR"
mkdir -p "$QT_SRC_DIR"
cd "$QT_SRC_DIR"

# --- Step 3: fetch Qt source ---
git clone https://code.qt.io/qt/qt5.git .
git checkout $QT_BRANCH

# Lấy các submodule chính (đủ để build core, gui, multimedia)
perl init-repository --module-subset=qtbase,qtdeclarative,qtmultimedia

# --- Step 4: optional safety patch for GCC 13 ---
PATCH_FILES=(
  "$QT_SRC_DIR/qtbase/src/corelib/global/qfloat16.h"
  "$QT_SRC_DIR/qtbase/src/corelib/text/qbytearraymatcher.h"
)
for file in "${PATCH_FILES[@]}"; do
  if [ -f "$file" ] && ! grep -q "<limits>" "$file"; then
    echo "⚙️  Patching $(basename "$file") ..."
    sed -i '1i #include <limits>' "$file"
  fi
done

# --- Step 5: configure ---
mkdir build && cd build
export CXXFLAGS="-std=c++14"
export CFLAGS="-std=c++14"
export CPPFLAGS="-std=c++14"

../configure -prefix "$QT_INSTALL_DIR" \
  -opensource -confirm-license \
  -nomake tests -nomake examples \
  -no-icu -no-dbus -skip qtwebengine

# --- Step 6: build & install ---
CPU_CORES=$(nproc || echo 2)
echo "🚀 Building Qt with $CPU_CORES cores..."
make -j"$CPU_CORES" || echo "⚠️  make returned non-zero, continuing..."
make install

# --- Step 7: cleanup ---
cd "$ROOT_DIR"
rm -rf "$QT_SRC_DIR"

echo "✅ Qt branch $QT_BRANCH built successfully!"
echo ""
echo "To use this build:"
echo "   export PATH=\"$QT_INSTALL_DIR/bin:\$PATH\""
echo "   export QTDIR=\"$QT_INSTALL_DIR\""
echo "   export CMAKE_PREFIX_PATH=\"$QT_INSTALL_DIR/lib/cmake:\$CMAKE_PREFIX_PATH\""
