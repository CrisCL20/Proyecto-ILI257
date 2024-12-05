#!/bin/bash

make clean
make

if [ $# -eq 0 ]; then
    echo "Usage: $0 <instance_path> $1 <n_params>"
    exit 1
fi

INSTANCE_PATH=$1
N_PARAMS=$2

python3 script.py "$INSTANCE_PATH" "$N_PARAMS"