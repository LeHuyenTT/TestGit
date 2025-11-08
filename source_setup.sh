#!/bin/bash

# ================================================
# Build script for AppTeacher (face recognition version)
# ================================================

# Get absolute path
SCRIPT_PATH=$(readlink -f "$0")
PROJECT_ROOT=$(dirname "$SCRIPT_PATH")
BUILD_DIR="$PROJECT_ROOT/build"

echo "----------------------------------------"
echo "📁 Project root: $PROJECT_ROOT"
echo "📦 Build directory: $BUILD_DIR"
echo "----------------------------------------"

# Create or clean build folder
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "✅ Created build folder."
else
    echo "♻️  Cleaning existing build folder..."
    rm -rf "$BUILD_DIR"/*
fi

cd "$BUILD_DIR" || exit 1

# Check build variant
if [ -z "$1" ] || { [ "$1" != "Debug" ] && [ "$1" != "Release" ]; }; then
    echo "❌ Invalid build variant!"
    echo "Usage: ./build.sh [Debug|Release]"
    exit 1
fi

VARIANT="$1"
echo "🚀 Building with variant: $VARIANT"
echo "----------------------------------------"

# Configure CMake
cmake -DCMAKE_BUILD_TYPE="$VARIANT" ..

# Build and check result
if cmake --build . -j$(nproc); then
    echo "✅ Build successful!"
    echo "🗂  Executable located at: $BUILD_DIR/AppTeacher"
else
    echo "❌ Build failed!"
    exit 1
fi
