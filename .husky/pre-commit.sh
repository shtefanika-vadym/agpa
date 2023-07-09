#!/bin/sh
. "$(dirname "$0")/_/husky.sh"

# Run Cppcheck
cppcheck --language=c ../index.c