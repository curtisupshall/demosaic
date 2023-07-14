#!/bin/sh

rm cachegrind.out.*
valgrind --tool=cachegrind ./decode.exe
cg_annotate --auto=yes cachegrind.out.*