#!/bin/bash

# Script to format all C source and header files using clang-format with LLVM style

set -e

# Check if clang-format is available
if ! command -v clang-format &>/dev/null; then
  echo "Error: clang-format is not installed or not in PATH"
  echo "Please install clang-format to use this script"
  exit 1
fi

echo "Formatting C source and header files with clang-format (LLVM style)..."

# Find all .c and .h files in the codebase and format them
find . -name "*.c" -o -name "*.h" | while read -r file; do
  # Skip files in build directory and other generated directories
  if [[ "$file" == *"/build/"* ]] || [[ "$file" == *"/target/"* ]] || [[ "$file" == *"/.git/"* ]]; then
    continue
  fi

  echo "Formatting: $file"
  clang-format -i -style=LLVM "$file"
done

echo "Formatting complete!"
