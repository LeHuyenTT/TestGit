#!/bin/bash
set -e

ROOT_DIR="${PWD}"
SCRIPTS_DIR="$ROOT_DIR/env"
LIBS_DIR="$ROOT_DIR/libs"

SCRIPTS=("install_cmake_3_20.sh" "install_qt5.sh" "install_opencv.sh" "install_socketio.sh" "install_ncnn.sh")

echo "📦 Setting up local environment..."

mkdir -p "$LIBS_DIR"
cd "$LIBS_DIR"

LOG_FILE="$ROOT_DIR/build_env.log"
echo "🧰 Build started at $(date)" > "$LOG_FILE"

function is_installed() {
    case "$1" in
        install_cmake_3_20.sh)
            [ -f "$LIBS_DIR/cmake_bin/bin/cmake" ] && return 0 ;;
        install_qt5.sh)
            [ -f "$LIBS_DIR/qt5_bin/usr/lib/qt5/bin/qmake" ] && return 0 ;;
        install_opencv.sh)
            [ -f "$LIBS_DIR/cv2_bin/lib/libopencv_core.so" ] && return 0 ;;
        install_socketio.sh)
            [ -f "$LIBS_DIR/SocketIO_bin/libsioclient.a" ] && return 0 ;;
        install_ncnn.sh)
            [ -f "$LIBS_DIR/ncnn_bin/lib/libncnn.a" ] || [ -f "$LIBS_DIR/ncnn_bin/install/lib/libncnn.a" ] && return 0 ;;
    esac
    return 1
}

# --- Run installation scripts ---
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

# --- Auto-export environment ---
echo "🌱 Loading local environment..."
source "$ROOT_DIR/export_env.sh"

echo "🎉 Environment setup completed successfully!"
echo "📄 Log file: $LOG_FILE"
