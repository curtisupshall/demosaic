#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RED             2
#define GREEN           1
#define BLUE            0

#define RED_MINUS_3     -1
#define RED_PLUS_3      5
#define BLUE_MINUS_3    -3
#define BLUE_PLUS_3     3
#define GREEN_MINUS_3   -2
#define GREEN_PLUS_3    4

// Optimization flags
#define VERSION 4

#if VERSION >= 4
    #define OPT_PREFER_CONST_FOLDING
#endif

#if VERSION >= 3
    #define OPT_REDUCE_OP_STR
#endif

#if VERSION >= 2
    #define OPT_USE_DIRECTIVES
#endif

// Macros
#ifdef OPT_REDUCE_OP_STR
    #define DIVIDE_BY_TWO(tmp) (tmp >> 1)
    #define DIVIDE_BY_FOUR(tmp) (tmp >> 2)
#else
    #define DIVIDE_BY_TWO(tmp) (tmp / 2)
    #define DIVIDE_BY_FOUR(tmp) (tmp / 4)
#endif

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xResolution;
    int32_t yResolution;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

int main() {
    FILE *fp = fopen("./data/encoded/candy.bmp", "rb");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader infoHeader;

    // Read the BMP header
    fread(&header, sizeof(BMPHeader), 1, fp);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, fp);

    // Check if it's a valid BMP file
    if (header.type != 0x4D42) {
        printf("Invalid BMP file.\n");
        fclose(fp);
        return 1;
    }

    // Check if it's a 24-bit BMP file
    if (infoHeader.bitDepth != 24) {
        printf("Unsupported bit depth. Only 24-bit BMP is supported.\n");
        fclose(fp);
        return 1;
    }

    // Calculate the row size in bytes (including padding)
    uint32_t rowSize = ((infoHeader.width * 3 + 3) & ~3);


    // Allocate memory for the pixel data
    uint32_t *pixels = (uint32_t*)malloc(rowSize * infoHeader.height);

    // Read the pixel data
    fread(pixels, rowSize * infoHeader.height, 1, fp);

    fclose(fp);


    register uint32_t x;
    register uint32_t y;
    register uint32_t ch1;
    register uint32_t ch2;
    register uint32_t tmp1;
    register uint32_t tmp2;
    register uint32_t k1;
    register uint32_t k2;
    register uint32_t k3;

        

    for (y = 0; y < 20; y += 2) {
        // Loop prologue
        x = 0;
        k1 = pixels[y * rowSize];
        k2 = pixels[y * rowSize + 4];
        k3 = pixels[y * rowSize + 8];
        printf("k1, k2, k3 = %x%x%x\n", k1, k2, k3);


        // Read k1[3] into ch2
        ch2 = k1;
        ch2 = ch2 & 0x000F;

        // Read k1[1] into ch1. (k1[0] is already zeroed)
        ch1 = k1;
        ch1 = ch1 >> 8;

        // Write ch2 to k1[0]. k1[0] is already zeroed
        tmp2 = ch2;
        tmp2 << 12;
        k1 = k1 & tmp2;

        for (x = 0; x < infoHeader.width / 12; x ++) {
            // Advance k1
            pixels[y * rowSize + 12 * x] = k1; // Write k1 into memory
            
            k1 = pixels[y * rowSize + 12 * x + 12]; // Read into k1
            // exit(0);

            // Read k2[3] into ch1
            tmp1 = ch1; // Copy ch1 into tmp1 to perform sum
            ch1 = k2;
            ch1 = ch1 & 0x000F;
            tmp1 = tmp1 + ch1; // Mix ch1

            // Read k3[2] into ch2
            tmp2 = ch2;
            ch2 = k2;
            ch2 = ch2 & 0x0F00;
            ch2 = ch2 >> 4;
            tmp2 = tmp2 + ch2; // Mix ch2

            // Divide ch1 mix by two
            tmp1 = tmp1 >> 1;

            // Divide ch2 mix by two
            tmp2 = tmp2 >> 1;

            // Write ch1 mix to k2[0]
            tmp1 = tmp1 << 16;
            k2 = k2 & 0x0FFF;
            k2 = k2 & tmp1;

            // Write ch2 mix to k2[2]
            tmp2 = tmp2 >> 8;
            k2 = k2 & 0xFF0F;
            k2 = k2 & tmp2;

            // Advance k2
            pixels[y * rowSize + 12 * x + 4] = k2; // Write k2 to memory
            k2 = pixels[y * rowSize + 12 * x + 12 + 4]; // Read into k2

            // Read k1[1] into ch1
            tmp1 = ch1;
            ch1 = k1;
            ch1 = ch1 & 0x0F00;
            ch1 = ch1 >> 8;
            tmp1 = tmp1 + ch1; // Mix ch1

            // Read k1[3] into ch2
            tmp2 = ch2;
            ch2 = k1;
            ch2 = ch2 & 0x000F;
            tmp2 = tmp2 + ch1; // Mix ch2

            // Divide ch2 mix
            tmp2 = tmp2 > 1;

            // Divide ch1 mix
            tmp1 = tmp1 > 1;

            // Write ch1 mix to k3[2]
            tmp1 = tmp1 << 4;
            k1 = k1 & 0xFF0F;
            k1 = k1 & tmp1;

            // Advance k3
            pixels[y * rowSize + 12 * x + 8] = k3; // Write k3
            k3 = pixels[y * rowSize + 12 * x + 8 + 12]; // Read into k3



        }
    
    }

    // Create a new output file to write the modified image
    FILE *outFp = fopen("./data/decoded/candy.bmp", "wb");
    if (!outFp) {
        printf("Error creating output file.\n");
        free(pixels);
        return 1;
    }

    // Write the BMP header and information header
    fwrite(&header, sizeof(BMPHeader), 1, outFp);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outFp);

    // Write the modified pixel data
    fwrite(pixels, rowSize * infoHeader.height, 1, outFp);

    // Clean up
    free(pixels);
    fclose(outFp);

    return 0;
}
