encode:
	gcc ./src/encode.c -o encode.exe && ./encode.exe

decode:
	gcc ./src/decode.c -o decode.exe && ./decode.exe

.PHONY: encode, decode
