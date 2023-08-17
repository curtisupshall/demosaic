encode:
	gcc src/encode.c -g -o encode.exe && ./encode.exe

decode:
	gcc src/decode.c -g -O0 -o decode.exe

arm:
	/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 src/decode.c -pg -o decode.out

asm:
	/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 src/decode.c -S -o decode.asm

test:
	gcc src/test.c -g -o decode.exe

clean:

	rm ./*.exe data/decoded/*.bmp data/encoded/*.bmp

.PHONY: encode, decode, clean
