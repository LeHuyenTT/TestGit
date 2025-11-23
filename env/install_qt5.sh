#!/bin/bash
set -e
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIBS_DIR="$ROOT_DIR/libs"
QT_INSTALL_DIR="$LIBS_DIR/qt5_bin"
mkdir -p "$QT_INSTALL_DIR"

echo "📦 Installing full Qt5 locally into $QT_INSTALL_DIR ..."

# Tạo cache riêng để tránh yêu cầu sudo
APT_CACHE="$QT_INSTALL_DIR/apt-cache"
mkdir -p "$APT_CACHE/archives/partial"
echo "dir::cache::$APT_CACHE;" > apt.conf
echo "dir::state::$APT_CACHE/state;" >> apt.conf
echo "dir::etc::sourcelist /etc/apt/sources.list;" >> apt.conf
echo "dir::etc::sourceparts /etc/apt/sources.list.d;" >> apt.conf

# Tải các gói .deb (không cài vào hệ thống)
apt-get -o Dir::Etc::sourcelist=/etc/apt/sources.list \
        -o Dir::Etc::sourceparts=/etc/apt/sources.list.d \
        -o Debug::NoLocking=true \
        -o Dir::Cache=$APT_CACHE \
        --download-only install -y \
        qtbase5-dev qtbase5-dev-tools qtdeclarative5-dev qttools5-dev qttools5-dev-tools qtmultimedia5-dev

# Giải nén tất cả .deb vào thư mục local
find "$APT_CACHE/archives" -name "*.deb" -exec dpkg-deb -x {} "$QT_INSTALL_DIR" \;

# Kiểm tra file chính
if [ -f "$QT_INSTALL_DIR/usr/lib/x86_64-linux-gnu/libQt5Core.so" ] && \
   [ -x "$QT_INSTALL_DIR/usr/lib/qt5/bin/moc" ]; then
  echo "✅ Full Qt5 installed successfully into $QT_INSTALL_DIR"
else
  echo "❌ Qt5 installation incomplete, missing core libs or tools"
  exit 1
fi

echo ""
echo "👉 To use it:"
echo "   export PATH=\"$QT_INSTALL_DIR/usr/lib/qt5/bin:\$PATH\""
echo "   export Qt5_DIR=\"$QT_INSTALL_DIR/usr/lib/x86_64-linux-gnu/cmake/Qt5\""
echo "   export CMAKE_PREFIX_PATH=\"$QT_INSTALL_DIR/usr/lib/x86_64-linux-gnu/cmake:\$CMAKE_PREFIX_PATH\""
