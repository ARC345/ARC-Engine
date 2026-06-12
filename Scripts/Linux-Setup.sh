#!/bin/bash
# ARC-Engine Linux Setup Script
# Installs dependencies required to build ARC-Engine on Linux

set -e

echo "[ARC-Engine] Installing build dependencies..."

# Update package lists
sudo apt-get update -qq

# Install essential build tools
sudo apt-get install -y -qq \
    build-essential \
    cmake \
    git \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libxext-dev \
    libwayland-dev \
    libxkbcommon-dev \
    libdbus-1-dev \
    zenity \
    premake5 2>/dev/null || true

# Check for premake5 (download if not available)
if ! command -v premake5 &>/dev/null; then
    echo "[ARC-Engine] Premake5 not found, downloading..."
    PREMAKE_VERSION="5.0.0-beta2"
    PREMAKE_URL="https://github.com/premake/premake-core/releases/download/v${PREMAKE_VERSION}/premake-${PREMAKE_VERSION}-linux.tar.gz"
    wget -q "$PREMAKE_URL" -O /tmp/premake5.tar.gz
    sudo tar -xzf /tmp/premake5.tar.gz -C /usr/local/bin/ premake5
    sudo chmod +x /usr/local/bin/premake5
    rm /tmp/premake5.tar.gz
fi

echo "[ARC-Engine] Generating project files..."
premake5 gmake2

echo "[ARC-Engine] Setup complete! Run 'make' to build."
echo "  Debug:   make config=debug"
echo "  Release: make config=release"
echo "  Dist:    make config=dist"
