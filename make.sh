#!/bin/bash

export PATH="$HOME/n64chain/bin:$PATH"
export N64_LIBGCCDIR="$HOME/n64chain/lib/gcc/mips64-elf/10.2.0"
export N64_NEWLIBDIR="$HOME/n64chain/mips64-elf/lib"
export N64_NEWLIBINCDIR="$HOME/n64chain/mips64-elf/include"
export ROOT=/etc/n64

rm -rf ./build
mkdir -p ./build/obj
make