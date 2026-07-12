#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

INSTALL_PREFIX="${1:-/usr/local}"   # default to /usr/local, override with first arg

echo -e "${YELLOW}Installing vcpkg dependencies${NC}"
vcpkg install

# Detect vcpkg triplet from installed directory (e.g. arm64-osx, x64-osx, x64-linux)
VCPKG_TRIPLET=$(ls "${PWD}/vcpkg_installed" | grep -v vcpkg | head -1)
VCPKG_PREFIX="${PWD}/vcpkg_installed/${VCPKG_TRIPLET}"

echo -e "${YELLOW}Cleaning up existing build${NC}"
rm -rf ./build

echo -e "${YELLOW}Preparing build (triplet: ${VCPKG_TRIPLET})${NC}"
cmake -B build -GNinja -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE} -DCMAKE_PREFIX_PATH="${VCPKG_PREFIX}" -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Release

echo -e "${YELLOW}Building the project${NC}"
cmake --build build

echo -e "${YELLOW}Installing to ${INSTALL_PREFIX}${NC}"
# Use sudo only for system paths that require elevated permissions
if [[ "${INSTALL_PREFIX}" == /usr/* || "${INSTALL_PREFIX}" == /opt/* ]]; then
    sudo cmake --install build
else
    cmake --install build
fi

echo -e "${GREEN}Done. Ensure ${INSTALL_PREFIX}/bin is on your PATH.${NC}"
