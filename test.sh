#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

script_dir=$(dirname "$(realpath "$0")")
cd "${script_dir}"

cmake -B build
cd build

make
ctest --output-on-failure
