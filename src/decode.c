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
    FILE *fp = fopen("./data/encoded/beach.bmp", "rb");
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
    uint8_t *pixels = (uint8_t*)malloc(rowSize * infoHeader.height);

    // Read the pixel data
    fread(pixels, rowSize * infoHeader.height, 1, fp);

    fclose(fp);

    uint8_t *px;

    #ifdef OPT_USE_DIRECTIVES
        register uint32_t x;
        register uint32_t y;
        register uint32_t tmpR;
        register uint32_t tmpG;
        register uint32_t tmpB;
    #else
        uint32_t x;
        uint32_t y;
        uint32_t tmpR;
        uint32_t tmpG;
        uint32_t tmpB;
    #endif

    #ifdef OPT_PREFER_CONST_FOLDING
        uint32_t rowSizeIndex;
    #endif

    // Access pixel values
    for (y = 2; y < infoHeader.height - 2; y++) {
        for (x = 2; x < infoHeader.width - 2; x ++) {
            /**
             * --------------------------------------------------------
             * GR PIXEL
             * --------------------------------------------------------
             * 
             * - Red channel derived from left and right pixels;
             * - Blue channel derived from above and below pixels;
             *
             */
            #ifdef OPT_PREFER_CONST_FOLDING
                rowSizeIndex = y * rowSize;
                px = &pixels[rowSizeIndex + x * 3];
            #else
                px = &pixels[y * rowSize + x * 3];
            #endif

            // Interpolate Red channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpR = (
                    pixels[rowSizeIndex + 3 * x + RED_MINUS_3] +
                    pixels[rowSizeIndex + 3 * x + RED_PLUS_3]
                );
            #else
                tmpR = (
                    pixels[y * rowSize + ((x - 1) * 3) + RED] +
                    pixels[y * rowSize + ((x + 1) * 3) + RED]
                );
            #endif
            tmpR = DIVIDE_BY_TWO(tmpR);

            // Interpolate Blue channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpB = (
                    pixels[rowSizeIndex + rowSize + (x * 3) + BLUE] +
                    pixels[rowSizeIndex - rowSize + 3 * x + BLUE]
                );
            #else
                tmpB = (
                    pixels[(y + 1) * rowSize + (x * 3) + BLUE] +
                    pixels[(y - 1) * rowSize + 3 * x + BLUE]
                );
            #endif
            tmpB = DIVIDE_BY_TWO(tmpB);

            px[RED] = tmpR;
            px[BLUE] = tmpB;

            x++;

            /**
             * --------------------------------------------------------
             * R PIXEL
             * --------------------------------------------------------
             * 
             * - Green channel derived form four adjacent green pixels;
             * - Blue channel derived from four diagonal neighbouring blue pixels;
             *
             */
            #ifdef OPT_PREFER_CONST_FOLDING
                rowSizeIndex = y * rowSize;
                px = &pixels[rowSizeIndex + x * 3];
            #else
                px = &pixels[y * rowSize + x * 3];
            #endif

            // Interpolate Green channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpG = (
                    pixels[rowSizeIndex + rowSize + 3 * x + GREEN] +
                    pixels[rowSizeIndex - rowSize + 3 * x + GREEN] +
                    pixels[rowSizeIndex + 3 * x GREEN_MINUS_3]     +
                    pixels[rowSizeIndex + 3 * x + GREEN_PLUS_3]
                );
            #else
                tmpG = (
                    pixels[(y + 1) * rowSize + (x * 3) + GREEN] +
                    pixels[(y - 1) * rowSize + (x * 3) + GREEN] +
                    pixels[y * rowSize + ((x - 1) * 3) + GREEN] +
                    pixels[y * rowSize + ((x + 1) * 3) + GREEN]
                );
            #endif
            tmpG = DIVIDE_BY_FOUR(tmpG);

            // Interpolate Blue channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpB = (
                    pixels[rowSizeIndex + rowSize + 3 * x + BLUE_MINUS_3] +
                    pixels[rowSizeIndex + rowSize + 3 * x + BLUE_PLUS_3]  +
                    pixels[rowSizeIndex - rowSize + 3 * x + BLUE_MINUS_3] +
                    pixels[rowSizeIndex - rowSize + 3 * x + BLUE_PLUS_3]
                );
            #else
                tmpB = (
                    pixels[(y + 1) * rowSize + ((x - 1) * 3) + BLUE] +
                    pixels[(y + 1) * rowSize + ((x + 1) * 3) + BLUE] +
                    pixels[(y - 1) * rowSize + ((x - 1) * 3) + BLUE] +
                    pixels[(y - 1) * rowSize + ((x + 1) * 3) + BLUE]
                );
            #endif
            tmpB = DIVIDE_BY_FOUR(tmpB);

            px[GREEN] = tmpG;
            px[BLUE] = tmpB;
        }

        y++;
        for (x = 2; x < infoHeader.width - 2; x ++) {
            /**
             * --------------------------------------------------------
             * B PIXEL
             * --------------------------------------------------------
             * 
             * - Red channel derived from four diagonal neighbouring red pixels;
             * - Green channel derived form four adjacent green pixels;
             *
             */
            #ifdef OPT_PREFER_CONST_FOLDING
                rowSizeIndex = y * rowSize;
                px = &pixels[rowSizeIndex + x * 3];
            #else
                px = &pixels[y * rowSize + x * 3];
            #endif
            
            // Interpolate Red channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpR = (
                    pixels[rowSizeIndex + rowSize + 3 * x + RED_MINUS_3] +
                    pixels[rowSizeIndex + rowSize + 3 * x + RED_PLUS_3]  +
                    pixels[rowSizeIndex - rowSize + 3 * x + RED_MINUS_3] +
                    pixels[rowSizeIndex - rowSize + 3 * x + RED_PLUS_3]
                );
            #else
                tmpR = (
                    pixels[(y + 1) * rowSize + ((x - 1) * 3) + RED] +
                    pixels[(y + 1) * rowSize + ((x + 1) * 3) + RED] +
                    pixels[(y - 1) * rowSize + ((x - 1) * 3) + RED] +
                    pixels[(y - 1) * rowSize + ((x + 1) * 3) + RED]
                );
            #endif
            tmpR = DIVIDE_BY_FOUR(tmpR);

            // Interpolate Green channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpG = (
                    pixels[rowSizeIndex + rowSize + 3 * x + GREEN] +
                    pixels[rowSizeIndex - rowSize + 3 * x + GREEN] +
                    pixels[rowSizeIndex + ((x - 1) * 3) + GREEN]   +
                    pixels[rowSizeIndex + ((x + 1) * 3) + GREEN]
                );
            #else
                tmpG = (
                    pixels[(y + 1) * rowSize + 3 * x + GREEN] +
                    pixels[(y - 1) * rowSize + 3 * x + GREEN] +
                    pixels[y * rowSize + ((x - 1) * 3) + GREEN] +
                    pixels[y * rowSize + ((x + 1) * 3) + GREEN]
                );
            #endif
            tmpG = DIVIDE_BY_FOUR(tmpG);

            px[RED] = tmpR;
            px[GREEN] = tmpG;

            x++;

            /**
             * --------------------------------------------------------
             * GB PIXEL
             * --------------------------------------------------------
             * 
             * - Red channel derived from above and below pixels
             * - Blue channel derived from left and right pixels
             *
             */
            #ifdef OPT_PREFER_CONST_FOLDING
                rowSizeIndex = y * rowSize;
                px = &pixels[rowSizeIndex + x * 3];
            #else
                px = &pixels[y * rowSize + x * 3];
            #endif
            
            // Interpolate Red channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpR = (
                    pixels[rowSizeIndex + rowSize + (x * 3) + RED] +
                    pixels[rowSizeIndex - rowSize + (x * 3) + RED]
                );
            #else
                tmpR = (
                    pixels[(y + 1) * rowSize + (x * 3) + RED] +
                    pixels[(y - 1) * rowSize + (x * 3) + RED]
                );
            #endif
            tmpR = DIVIDE_BY_TWO(tmpR);

            // Interpolate Blue channel
            #ifdef OPT_PREFER_CONST_FOLDING
                tmpB = (
                    pixels[rowSizeIndex + 3 * x + BLUE_MINUS_3] +
                    pixels[rowSizeIndex + 3 * x + BLUE_PLUS_3]
                );
            #else
                tmpB = (
                    pixels[y * rowSize + ((x - 1) * 3) + BLUE] +
                    pixels[y * rowSize + ((x + 1) * 3) + BLUE]
                );
            #endif
            tmpB = DIVIDE_BY_TWO(tmpB);

            px[RED] = tmpR;
            px[BLUE] = tmpB;
        }
    }

    // Create a new output file to write the modified image
    FILE *outFp = fopen("./data/decoded/beach.bmp", "wb");
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
