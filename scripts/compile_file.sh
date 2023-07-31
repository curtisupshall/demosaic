#! /bin/bash

echo Copying files to /tmp
mkdir -p /tmp/demosaic440/
cp ./src/decode.c /tmp/demosaic440/

echo copy files for arm device to /demosaic_arm
mkdir -p ./demosaic_arm/
cp ./data/encoded/$1.bmp ./demosaic_arm/encoded.bmp

echo Compile decode.c to decode.exe
process_id=$!
/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 /tmp/demosaic440/decode.c -pg -o ./demosaic_arm/decode.exe
wait $process_id


echo COMPILE COMPLETE