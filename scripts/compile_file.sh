#! /bin/bash

echo Copying files to /tmp
mkdir -p /tmp/demosaic440/
cp ./src/prev_version/decode_v1.c /tmp/demosaic440/decode.c

echo copy files for arm device to /demosaic_arm
mkdir -p ./demosaic_arm/
cp ./data/encoded/* ./demosaic_arm/*

echo Compile decode.c to decode.exe
process_id=$!
# gcc /tmp/demosaic440/decode.c -g -o ./demosaic_arm/metric.exe
# gcc /tmp/demosaic440/decode.c -S -o ./demosaic_arm/assembly.s
/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 /tmp/demosaic440/decode.c -pg -o ./demosaic_arm/decode_neon.exe
wait $process_id


echo COMPILE COMPLETE