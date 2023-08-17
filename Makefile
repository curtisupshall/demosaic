encode:
	gcc src/encode.c -g -o encode.exe && ./encode.exe

decode:
	gcc src/decode.c -g -o decode.exe

arm:
	/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 src/decode.c -pg -o decode.out

run:
	qemu-arm ./decode.out

asm:
	/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 src/decode.c -S -o build/decode.asm

gprof:
	gprof ./decode.out ./gmon.out > analysis.txt

clean:
	rm -rf ./*.exe ./*.out data/decoded/*.bmp data/encoded/*.bmp

.PHONY: encode, decode, clean
