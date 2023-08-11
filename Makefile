encode:
	gcc src/encode.c -g -o encode.exe && ./encode.exe

decode:
	gcc src/decode.c -g -o decode.exe # && ./decode.exe

clean:

	rm ./*.exe data/decoded/*.bmp data/encoded/*.bmp

.PHONY: encode, decode, clean
