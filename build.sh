#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

echo -e "${YELLOW}Cleaning up existing build${NC}"
rm -rf ./build

echo -e "${YELLOW}Preparing build${NC}"
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE}

echo -e "${YELLOW}Building the project${NC}"
cmake --build build
