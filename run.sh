#!/usr/bin/env bash
set -e

echo "=== NTU Rush Run Script ==="

OS="$(uname -s)"

# Path for different OS
EXE="./build/bin/ntu_rush"

if [[ "$OS" == MINGW* || "$OS" == CYGWIN* ]]; then
    EXE="./build/bin/ntu_rush.exe"
fi

if [[ ! -f "$EXE" ]]; then
    echo "Executable not found! Running build first..."
    ./build.sh
fi

echo "Running game..."
$EXE
