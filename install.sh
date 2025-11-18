#!/usr/bin/env bash
set -e

echo "=== NTU Rush Install Script ==="

OS="$(uname -s)"

# -------- Mac --------
if [[ "$OS" == "Darwin" ]]; then
    echo "[Mac] Installing SFML via Homebrew..."
    brew install sfml
    echo "SFML installed!"
    exit 0
fi

# -------- Windows: Git Bash / MinGW --------
if [[ "$OS" == MINGW* || "$OS" == CYGWIN* ]]; then
    echo "[Windows] Installing SFML using vcpkg..."

    if ! command -v git >/dev/null 2>&1; then
        echo "請先安裝 Git！"
        exit 1
    fi

    # install vcpkg in project if not exist
    if [[ ! -d "vcpkg" ]]; then
        git clone https://github.com/microsoft/vcpkg.git
        cd vcpkg
        ./bootstrap-vcpkg.sh
        cd ..
    fi

    echo "[Windows] Installing SFML..."
    ./vcpkg/vcpkg install sfml:x64-windows

    echo "SFML installed via vcpkg!"
    echo "建議在 build.sh 中使用 -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake"
    exit 0
fi

# -------- Linux / WSL --------
echo "[Linux/WSL] Installing SFML..."
sudo apt update
sudo apt install libsfml-dev -y
echo "SFML installed!"
