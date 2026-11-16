#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
for test in tests/*.cpp; do
  name=$(basename "$test" .cpp)
  "${CXX:-c++}" -std=c++11 -Wall -Wextra -Werror -pedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g \
    -Iinclude -Itests/fakes "$test" -o "build/$name"
  "./build/$name"
done
