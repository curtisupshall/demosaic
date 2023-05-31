#! /bin/bash

/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 -S /tmp/seng440/basic.c -o ./src/compile/basic.s
git add ./src/compile/basic.s
git commit -m "Compiled basic.c to basic.s"
git push origin master
