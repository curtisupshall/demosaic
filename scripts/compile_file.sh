#! /bin/bash

echo Copying files to /tmp
mkdir -p /tmp/seng440/
cp ./decode.c /tmp/seng440/

echo copy files for arm device to /demosaic_arm
mkdir -p ./demosaic_arm/
cp ./output/encoded.bmp ./demosaic_arm/

echo Compile decode.c to decode.exe
process_id=$!
/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 /tmp/seng440/decode.c -o ./demosaic_arm/decode.exe
wait $process_id


echo COMPILE COMPLETE