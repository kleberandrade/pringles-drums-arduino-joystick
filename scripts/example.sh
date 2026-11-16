#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
"${CXX:-c++}" -std=c++11 -Wall -Wextra -Werror -pedantic -Iinclude \
  examples/simulate.cpp -o build/simulate
./build/simulate > build/simulation.txt
diff -u examples/expected.txt build/simulation.txt
cat build/simulation.txt
