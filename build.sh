#!/usr/bin/env bash
set -e

echo "=== BikeAway Build Script ==="

# Detect PLATFORM
OS="$(uname -s)"

# ---- Create build folder ----
mkdir -p build
cd build

# ---- Mac ----
if [[ "$OS" == "Darwin" ]]; then
    echo "[Mac] Running CMake..."

    # Locate Homebrew packages for CMake
    SFML_DIR=$(brew --prefix sfml)/lib/cmake/SFML
    NLOHMANN_PREFIX=$(brew --prefix nlohmann-json 2>/dev/null || true)
    if [[ -z "$NLOHMANN_PREFIX" ]]; then
        echo "[Mac] nlohmann-json not found via Homebrew. Install with: brew install nlohmann-json"
    fi

    cmake .. \
        -DSFML_DIR="$SFML_DIR" \
        -DCMAKE_PREFIX_PATH="$NLOHMANN_PREFIX"
    cmake --build .
    exit 0
fi

# ---- Windows (Git Bash / MINGW) ----
if [[ "$OS" == MINGW* || "$OS" == CYGWIN* ]]; then
    echo "[Windows] Detected (Git Bash / MinGW)"

    # 如果有 vcpkg，自動使用
    if command -v vcpkg >/dev/null 2>&1; then
        echo "[Windows] vcpkg detected"
        cmake .. -DCMAKE_TOOLCHAIN_FILE="$(vcpkg fetch cmake-toolchain-file)"
    else
        echo "[Windows] No vcpkg detected!"
        echo "請先安裝 SFML 或執行 ./install.sh"
        cmake ..
    fi

    cmake --build .
    exit 0
fi

# ---- WSL or Linux ----
echo "[Linux/WSL] Running CMake..."
cmake ..
cmake --build .
