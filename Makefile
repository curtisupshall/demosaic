encode:
	gcc src/encode.c -o encode.exe && ./encode.exe

decode:
	gcc src/decode.c -o decode.exe && ./decode.exe

clean:

	rm ./*.exe data/decoded/*.bmp data/encoded/*.bmp

.PHONY: encode, decode, clean
