#!/usr/bin/env bash
set -euo pipefail

SRC="ASSIGNMENT2.CPP"
OUT="socialnet"

# compile
if ! command -v g++ >/dev/null 2>&1; then
  echo "g++ not found in PATH. Install g++ (MinGW / WSL / GCC) and retry."
  exit 1
fi

echo "Compiling $SRC -> $OUT"
g++ -std=c++11 -O2 "$SRC" -o "$OUT"

# run if argument provided (input file), otherwise show run hint
if [ $# -ge 1 ]; then
  echo "Running with input file: $1"
  ./"$OUT" < "$1"
else
  echo "Build successful."
  echo "To run: ./$OUT < commands.txt"
fi

