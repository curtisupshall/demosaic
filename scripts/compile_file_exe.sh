#! /bin/bash

echo Copying files to /tmp

mkdir -p /tmp/seng440/
cp ./src/basic.c /tmp/seng440/

echo Compile basic.c to basic.exe
process_id=$!
#/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3  /tmp/seng440/basic.c -o ./src/compile/basic.exe
/opt/arm/4.3.2/bin/arm-linux-gcc  /tmp/seng440/basic.c -o ./src/compile/basic.exe
wait $process_id

echo COMPILE COMPLETE
