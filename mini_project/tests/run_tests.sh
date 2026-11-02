#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic src/*.cpp -o airports
./airports --test
./airports --experiment
