#!/usr/bin/env bash
set -e

echo "=== NTU Rush SFML2 Install Script ==="

OS="$(uname -s)"

# 小工具：印訊息用
info()  { echo "[INFO] $*"; }
warn()  { echo "[WARN] $*"; }
error() { echo "[ERROR] $*" >&2; }

########################################
# macOS：用 Homebrew 安裝 SFML 2.x
########################################
if [[ "$OS" == "Darwin" ]]; then
    info "Detected macOS"

    if ! command -v brew >/dev/null 2>&1; then
        error "找不到 Homebrew，請先安裝：/bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        exit 1
    fi

    # 優先試試看 sfml@2（有些 tap 會提供）
    if brew info sfml@2 >/dev/null 2>&1; then
        info "Installing sfml@2 via Homebrew..."
        brew install sfml@2
    else
        info "Homebrew 沒有 sfml@2，改裝 sfml（目前版本也是 2.x / 2.6.x）..."
        brew install sfml
    fi

    info "SFML 2 安裝完成！你可以在 CMake 時指定："
    info "  -DSFML_DIR=\$(brew --prefix sfml)/lib/cmake/SFML"
    exit 0
fi

########################################
# Windows（Git Bash / MinGW / Cygwin）：用 vcpkg 安裝 SFML 2.x
########################################
if [[ "$OS" == MINGW* || "$OS" == CYGWIN* ]]; then
    info "Detected Windows (Git Bash / MinGW / Cygwin)"

    if ! command -v git >/dev/null 2>&1; then
        error "找不到 git，請先安裝 Git for Windows 再重試。"
        exit 1
    fi

    # 如果專案底下還沒有 vcpkg，就 clone 一份
    if [[ ! -d "vcpkg" ]]; then
        info "Cloning vcpkg..."
        git clone https://github.com/microsoft/vcpkg.git
        cd vcpkg

        info "Bootstrapping vcpkg..."
        # 在 Git Bash 下用 .bat 開啟
        if [[ -f "./bootstrap-vcpkg.bat" ]]; then
            cmd //c bootstrap-vcpkg.bat
        else
            ./bootstrap-vcpkg.sh
        fi
        cd ..
    else
        info "vcpkg already exists, skip clone."
    fi

    info "Installing SFML 2 (port: sfml:x64-windows) via vcpkg..."
    ./vcpkg/vcpkg install sfml:x64-windows

    info "SFML 2 安裝完成！"
    info "CMake 建議加入："
    info "  -DCMAKE_TOOLCHAIN_FILE=\$(pwd)/vcpkg/scripts/buildsystems/vcpkg.cmake"
    info "並在 CMakeLists.txt 裡保留： find_package(SFML 2.5 COMPONENTS ... REQUIRED)"
    exit 0
fi

########################################
# Linux / WSL：用套件管理員安裝 SFML 2.x
########################################
info "Detected Linux / WSL (or other Unix-like)"
if command -v apt >/dev/null 2>&1; then
    info "Using apt to install libsfml-dev (SFML 2.x)..."
    sudo apt update
    sudo apt install -y libsfml-dev
    info "SFML 2 安裝完成！"
else
    warn "目前只自動支援 apt，其他發行版請手動用套件管理員安裝 SFML 2（libsfml-dev）。"
fi
