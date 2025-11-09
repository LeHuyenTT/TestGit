#!/bin/bash
set -e  # Dừng ngay khi gặp lỗi

ROOT_DIR="${PWD}/.."
LIBS_DIR="${ROOT_DIR}/libs"
SOCKETIO_SRC_DIR="${LIBS_DIR}/socket.io-client-cpp"
SOCKETIO_BIN_DIR="${LIBS_DIR}/SocketIO_bin"

echo "📦 Installing dependencies for Socket.IO C++ client..."
sudo apt update
sudo apt install -y git cmake build-essential libboost-system-dev libboost-thread-dev libssl-dev

# --- Step 1: Prepare directories ---
echo "🗂 Preparing directories..."
rm -rf "$SOCKETIO_SRC_DIR" "$SOCKETIO_BIN_DIR"
mkdir -p "$SOCKETIO_SRC_DIR" "$SOCKETIO_BIN_DIR"

# --- Step 2: Clone repo ---
echo "⬇️  Cloning socket.io-client-cpp..."
git clone --recurse-submodules https://github.com/socketio/socket.io-client-cpp.git "$SOCKETIO_SRC_DIR"

# --- Step 3: Build ---
echo "⚙️  Building Socket.IO client..."
cd "$SOCKETIO_SRC_DIR"
mkdir -p build && cd build
cmake .. \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DCMAKE_BUILD_TYPE=Release
make -j"$(nproc)"

# --- Step 4: Copy libraries ---
echo "📤 Copying build artifacts..."
cp -v libsioclient.a* "$SOCKETIO_BIN_DIR" || true
cp -v libsioclient_tls.a* "$SOCKETIO_BIN_DIR" || true
cp -r ../src "$SOCKETIO_BIN_DIR/src"

# --- Step 5: Cleanup ---
cd "$ROOT_DIR"
rm -rf "$SOCKETIO_SRC_DIR"

echo "✅ Socket.IO client library installed successfully."
echo "📁 Output: $SOCKETIO_BIN_DIR"
