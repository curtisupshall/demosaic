# demosaic

Image demosaicing algorithm.

## Prerequisites
You should be on `seng440.ece.uvic.ca`.

## Building the application

0. Clean any artifacts:
```bash
make clean
```

1. (Setup) encoding the images:
```bash
make encode
```

2. Compiling the demosaicing algorithm:
```bash
make arm
```

3. Running the algorithm:
```bash
make run
```

4. Generate the assembly:
```bash
make asm
```

The assembly is output to the /build directory.
