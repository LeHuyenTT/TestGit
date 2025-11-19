#!/bin/bash
set -e  # Dừng script khi có lỗi

ROOT_DIR="${PWD}"
SCRIPTS_DIR="$ROOT_DIR/env"
LIBS_DIR="$ROOT_DIR/libs"

# Danh sách các script cần chạy (cài cục bộ)
SCRIPTS=("install_cmake_3_20.sh" "install_qt5.sh" "install_opencv.sh" "install_socketio.sh" "install_ncnn.sh")

echo "📦 Setting up local environment..."

mkdir -p "$LIBS_DIR"
cd "$LIBS_DIR"

# File log
LOG_FILE="$ROOT_DIR/build_env.log"
echo "🧰 Build started at $(date)" > "$LOG_FILE"

# --- Export PATH cho các lib cục bộ (nếu đã tồn tại) ---
export PATH="$LIBS_DIR/cmake_bin/bin:$LIBS_DIR/qt5_bin/bin:$PATH"
export QTDIR="$LIBS_DIR/qt5_bin"
export CMAKE_PREFIX_PATH="$LIBS_DIR/qt5_bin/lib/cmake:$CMAKE_PREFIX_PATH"

# --- Hàm kiểm tra xem lib đã cài chưa ---
function is_installed() {
    case "$1" in
        install_cmake_3_20.sh)
            [ -x "$LIBS_DIR/cmake_bin/bin/cmake" ] && return 0 ;;
        install_qt5.sh)
            [ -x "$LIBS_DIR/qt5_bin/bin/qmake" ] && return 0 ;;
        install_opencv.sh)
            [ -d "$LIBS_DIR/cv2_bin" ] && [ -f "$LIBS_DIR/cv2_bin/lib/libopencv_core.so" ] && return 0 ;;
        install_socketio.sh)
            [ -d "$LIBS_DIR/SocketIO_bin" ] && [ -f "$LIBS_DIR/SocketIO_bin/libsioclient.a" ] && return 0 ;;
        install_ncnn.sh)
            [ -d "$LIBS_DIR/ncnn_bin" ] && [ -f "$LIBS_DIR/ncnn_bin/lib/libncnn.a" ] && return 0 ;;
        *)
            return 1 ;;
    esac
    return 1
}

# --- Chạy lần lượt từng script ---
for script in "${SCRIPTS[@]}"; do
    local_script="$SCRIPTS_DIR/$script"
    if [ ! -f "$local_script" ]; then
        echo "⚠️  Warning: $script not found. Skipping." | tee -a "$LOG_FILE"
        continue
    fi

    echo "────────────────────────────────────" | tee -a "$LOG_FILE"
    echo "▶️  Checking $script ..." | tee -a "$LOG_FILE"

    if is_installed "$script"; then
        echo "⏭️  $script already installed, skipping." | tee -a "$LOG_FILE"
        continue
    fi

    echo "🚀 Running $script ..." | tee -a "$LOG_FILE"
    if bash "$local_script" >>"$LOG_FILE" 2>&1; then
        echo "✅ $script completed successfully." | tee -a "$LOG_FILE"
    else
        echo "❌ $script failed! Check $LOG_FILE for details." | tee -a "$LOG_FILE"
        exit 1
    fi
done

echo "🎉 Environment setup completed successfully!"
echo "📄 Log file: $LOG_FILE"

# --- In đường dẫn môi trường sau khi hoàn tất ---
echo ""
echo "✅ Environment paths configured:"
echo "   PATH=$LIBS_DIR/cmake_bin/bin:$LIBS_DIR/qt5_bin/bin:\$PATH"
echo "   QTDIR=$LIBS_DIR/qt5_bin"
echo "   CMAKE_PREFIX_PATH=$LIBS_DIR/qt5_bin/lib/cmake:\$CMAKE_PREFIX_PATH"
