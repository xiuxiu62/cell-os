#!/bin/bash

# Build if it does not exist
[[ ! -f "./bin/os.bin" ]] && bash ./scripts/build.sh

# Emulate the binary
qemu-system-x86_64 -hda ./bin/os.bin
