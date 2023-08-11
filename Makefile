encode:
	gcc src/encode.c -g -o encode.exe && ./encode.exe

decode:
	gcc src/decode.c -g -O0 -o decode.exe

clean:

	rm ./*.exe data/decoded/*.bmp data/encoded/*.bmp

.PHONY: encode, decode, clean
