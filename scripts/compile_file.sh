#! /bin/bash

echo Copying files to /tmp

mkdir -p /tmp/seng440/
cp -r ./arm /tmp/seng440/

echo Compile decode.c
# process_id=$!
# /tmp/seng440/arm/call_metrics.sh
#/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 /tmp/seng440/decode.c -o ./decode.exe
# wait $process_id

# echo COMPILE COMPLETE