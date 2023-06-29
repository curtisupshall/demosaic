# To build for locally run `make CC=clang`
CC=/opt/arm/4.3.2/bin/arm-linux-gcc
# These are mostly here to on to cover our buts during the base setup
CFLAGS= -std=c99 -O3
ASMCFLAGS= -static -S -std=c99 -O3
LIB = bmp.c #interpolate.c #bmpfunc.c #Read_PPM.c
SRC = $(LIB) main.c
OBJ = ${SRC:.c=.o}
TEST_SRC = $(LIB) test.c
TEST_OBJ = ${TEST_SRC:.c=.o}

all: demosaic

# Create the object files for our libraries
.c.o:
	$(CC) -c $(CFLAGS) $<

# You can now peak at the asm output by running `make main.s`
.c.s:
	$(CC) -c -S $(CFLAGS) $<

# Generate the final executable
demosaic: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.PHONY: test
test: $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -o $@
	./test

clean:
	$(RM) cow2.bpm demosaic test  *.o
