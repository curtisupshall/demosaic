#! /bin/bash

echo --START COMPILE SCRIPT--

filename=${1?param missing - file name.}

echo Copying files to /tmp

mkdir -p /tmp/seng440_demosaic/

cp ./src/$filename.c /tmp/seng440_demosaic/

echo Compile $filename.c to $filename.exe
process_id=$!
#/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3  /tmp/seng440_demosaic/$filename.c -o ./src/compile/$filename.exe
/opt/arm/4.3.2/bin/arm-linux-gcc  /tmp/seng440_demosaic/$filename.c -o ./src/compile/$filename.exe
/opt/arm/4.3.2/bin/arm-linux-gcc  /tmp/seng440_demosaic/$filename.c -o /tmp/seng440_demosaic/$filename.exe
wait $process_id

echo COMPILE COMPLETE
