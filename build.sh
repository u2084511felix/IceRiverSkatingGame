#!/usr/bin/env bash
set -e

set -a
source .env
set +a


# em++ \
#   src/main.cpp \
#   -Ithird_party/olcPixelGameEngine3 \
#   -std=c++20 \
#   -O0 \
#   -g \
#   -sASYNCIFY \
#   -sALLOW_MEMORY_GROWTH=1 \
#   -sSTACK_SIZE=1048576 \
#   -sEXPORTED_RUNTIME_METHODS=HEAPF32 \
#   -sMIN_WEBGL_VERSION=2 \
#   -sMAX_WEBGL_VERSION=2 \
#   -sUSE_LIBPNG=1 \
#   -sLLD_REPORT_UNDEFINED \
#   -o SeasonsChange.html

em++ \
  src/main.cpp \
  -Ithird_party/olcPixelGameEngine3 \
  -std=c++20 \
  -O3 \
  -sASYNCIFY \
  -sALLOW_MEMORY_GROWTH=1 \
  -sSTACK_SIZE=1048576 \
  -sEXPORTED_RUNTIME_METHODS=HEAPF32 \
  -sMIN_WEBGL_VERSION=2 \
  -sMAX_WEBGL_VERSION=2 \
  -sUSE_LIBPNG=1 \
  -sLLD_REPORT_UNDEFINED \
  -o dist/index.html