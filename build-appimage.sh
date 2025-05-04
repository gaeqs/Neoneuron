#!/usr/bin/env bash


set -euo pipefail

# Default parameters
BUILD_DIR="${1:-cmake-build-release}"
APPDIR="$BUILD_DIR/appdir"
APP="neoneuron"
APP_NAME="Neoneuron"
LINUXDEPLOY="linuxdeploy-x86_64.AppImage"
DESKTOP_FILE="$APPDIR/usr/share/applications/${APP}.desktop"
ICON_FILE="$APPDIR/usr/share/icons/hicolor/512x512/apps/${APP}.png"
OUTPUT_DIR="out"

usage() {
  cat <<EOF
Usage: $(basename "$0") [build_directory]

Build the $APP AppImage using linuxdeploy.

Positional arguments:
  build_directory   Directory where the project was built (default: cmake-build-release)
EOF
  exit 1
}

# Show help if requested
if [[ "${1:-}" =~ ^(-h|--help)$ ]]; then
  usage
fi

echo "Configuring project..."
cmake -S . -B ${BUILD_DIR} -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
echo "Building and installing..."
cmake --build ${BUILD_DIR} --target install

# Copy desktop entry and icon into AppDir root
cp "$DESKTOP_FILE" "$APPDIR/${APP}.desktop"
cp "$ICON_FILE"   "$APPDIR/${APP}.png"

# Download linuxdeploy if missing
if [[ ! -f "$LINUXDEPLOY" ]]; then
  echo "⬇ Downloading linuxdeploy..."
  wget -q --show-progress "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$LINUXDEPLOY"
  chmod +x "$LINUXDEPLOY"
else
  echo "✅ linuxdeploy found"
fi

# Build the AppImage
./"$LINUXDEPLOY" \
  --appdir "$APPDIR" \
  --desktop-file "$APPDIR/${APP}.desktop" \
  --icon-file    "$APPDIR/${APP}.png" \
  --output appimage