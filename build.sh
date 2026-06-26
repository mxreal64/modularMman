#!/usr/bin/env bash
set -e

# Clean only output binaries, keeping your precious gcm.cache intact
rm -f *.o *.out

# Trigger your dynamic standard library module caching trick if missing
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

# Step 1: Compile your mman module interface using the pre-built gcm.cache
g++ -std=c++26 -fmodules-ts -O3 -c modularMman.cppm -o modularMman.o

# Step 2: Compile the test harness using your fast module interface linkage
g++ -std=c++26 -fmodules-ts -O3 main.cpp modularMman.o -o test_mman.out

echo "Build optimized! Execute with: ./test_mman.out"
