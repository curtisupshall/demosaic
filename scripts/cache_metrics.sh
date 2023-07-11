#!/bin/sh
make clean
make "CFLAGS= -mfloat-abi=softfp -mfpu=neon -static -O3 "
rm cachegrind.out.*
valgrind --tool=cachegrind ./demosaic
cg_annotate --auto=yes cachegrind.out.*