#!/bin/sh

make clean
make "-mfloat-abi=softfp -mfpu=neon -static -O3"
valgrind --tool=callgrind ./demosaic
callgrind_annotate callgrind.out.*
rm callgrind.out.*