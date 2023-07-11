#!/bin/sh

valgrind --tool=callgrind ./decode.exe
callgrind_annotate callgrind.out.*
rm callgrind.out.*