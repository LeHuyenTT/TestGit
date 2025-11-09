#!/bin/bash
set -e  # Dừng script khi có lỗi

ROOT_DIR="${PWD}"
SCRIPTS_DIR="$ROOT_DIR/env"
LIBS_DIR="$ROOT_DIR/libs"

# Danh sách các script cần chạy
SCRIPTS=("install_cmake_3_20.sh" "install_qt5.sh" "install_opencv.sh" "install_socketio.sh" "install_ncnn.sh")

echo "📦 Setting up environment..."

mkdir -p "$LIBS_DIR"
cd "$LIBS_DIR"

# File log
LOG_FILE="$ROOT_DIR/build_env.log"
echo "🧰 Build started at $(date)" > "$LOG_FILE"

# --- Hàm kiểm tra xem lib đã cài chưa ---
function is_installed() {
    case "$1" in
        install_cmake_3_20.sh)
            command -v cmake >/dev/null 2>&1 && return 0 ;;
        install_qt5.sh)
            command -v qmake >/dev/null 2>&1 && return 0 ;;
        install_opencv.sh)
            [ -d "$LIBS_DIR/cv2_bin" ] && return 0 ;;
        install_socketio.sh)
            [ -d "$LIBS_DIR/SocketIO_bin" ] && return 0 ;;
        install_ncnn.sh)
            [ -d "$LIBS_DIR/ncnn_bin" ] && return 0 ;;
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
