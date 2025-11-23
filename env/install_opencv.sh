#!/bin/bash
set -e

OPENCV_VERSION="4.9.0"
OPENCV_URL="https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip"
OPENCV_CONTRIB_URL="https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.zip"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIBS_DIR="${ROOT_DIR}/libs"
OPENCV_SRC_DIR="${LIBS_DIR}/opencv_src"
OPENCV_INSTALL_DIR="${LIBS_DIR}/cv2_bin"

echo "📦 Installing OpenCV ${OPENCV_VERSION} locally..."

# --- Step 1: Install dependencies (nếu có sudo, cài 1 lần thôi) ---
if command -v sudo &>/dev/null; then
  echo "🧰 Installing dependencies..."
  sudo apt-get update -y
  sudo apt-get install -y build-essential cmake git unzip pkg-config \
    libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev \
    libv4l-dev libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb-dev libdc1394-dev libopenblas-dev liblapack-dev libeigen3-dev \
    libhdf5-dev protobuf-compiler libprotobuf-dev libgoogle-glog-dev libgflags-dev \
    libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev
else
  echo "⚠️  Skipping dependency installation (no sudo)"
fi

# --- Step 2: Prepare folder structure ---
echo "🗂 Preparing directories..."
rm -rf "$OPENCV_SRC_DIR" "$OPENCV_INSTALL_DIR"
mkdir -p "$OPENCV_SRC_DIR" "$OPENCV_INSTALL_DIR"
cd "$OPENCV_SRC_DIR"

# --- Step 3: Download source ---
echo "⬇️  Downloading OpenCV ${OPENCV_VERSION}..."
wget -q -O opencv.zip "$OPENCV_URL"
wget -q -O opencv_contrib.zip "$OPENCV_CONTRIB_URL"
unzip -q opencv.zip
unzip -q opencv_contrib.zip
mv "opencv-${OPENCV_VERSION}" opencv
mv "opencv_contrib-${OPENCV_VERSION}" opencv_contrib

mkdir -p opencv/build
cd opencv/build

# --- Step 4: Configure build ---
echo "⚙️  Configuring build..."
cmake -D CMAKE_BUILD_TYPE=Release \
      -D CMAKE_INSTALL_PREFIX="$OPENCV_INSTALL_DIR" \
      -D OPENCV_EXTRA_MODULES_PATH="$OPENCV_SRC_DIR/opencv_contrib/modules" \
      -D BUILD_TESTS=OFF \
      -D BUILD_EXAMPLES=OFF \
      -D WITH_CUDA=OFF \
      -D WITH_OPENCL=OFF \
      -D WITH_QT=OFF \
      -D WITH_TBB=ON \
      -D OPENCV_ENABLE_NONFREE=ON \
      -D OPENCV_GENERATE_PKGCONFIG=ON ..

# --- Step 5: Build & install locally ---
CPU_CORES=$(nproc || echo 2)
echo "🚀 Building with $CPU_CORES cores..."
make -j"$CPU_CORES"
make install  # ✅ không cần sudo

# --- Step 6: Clean up ---
cd "$ROOT_DIR"
rm -rf "$OPENCV_SRC_DIR"

echo "✅ OpenCV ${OPENCV_VERSION} installed successfully in ${OPENCV_INSTALL_DIR}"
echo ""
echo "👉 To use it:"
echo "   export OpenCV_DIR=\"$OPENCV_INSTALL_DIR/lib/cmake/opencv4\""
echo "   export LD_LIBRARY_PATH=\"$OPENCV_INSTALL_DIR/lib:\$LD_LIBRARY_PATH\""
