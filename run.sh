#!/usr/bin/env bash
set -e

echo "=== BikeAway Run Script ==="

OS="$(uname -s)"

# Path for different OS
EXE="./build/bin/bike_away"

if [[ "$OS" == MINGW* || "$OS" == CYGWIN* ]]; then
    EXE="./build/bin/bike_away.exe"
fi

if [[ ! -f "$EXE" ]]; then
    echo "Executable not found! Running build first..."
    ./build.sh
fi

echo "Running game..."
$EXE
