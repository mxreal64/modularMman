#!/usr/bin/env bash
set -e

if [ ! -d "gcm.cache" ]; then
    echo "Building standard library module cache..."
    GCC_VERSION=$(g++ -dumpversion)
    STD_MODULE_PATH="/usr/include/c++/${GCC_VERSION}/bits/std.cc"
    
    if [ -f "${STD_MODULE_PATH}" ]; then
        # Compile GCC's internal std.cc module definition into your local workspace
        g++ -std=c++26 -fmodules-ts -O3 -c "${STD_MODULE_PATH}"
    else
        echo "Error: Could not locate system std.cc file automatically."
        exit 1
    fi
fi

echo "[Pipeline] Standard library module cache verified."

g++ -std=c++26 -fmodules-ts -O3 -c modularMman.cppm -o modularMman.o

echo "done, now go code"
