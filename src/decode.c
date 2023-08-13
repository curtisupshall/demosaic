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
    FILE *fp = fopen("data/encoded/clouds.bmp", "rb");
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

    int imageHeight = infoHeader.height;
    int imageWidth = infoHeader.width;

    // Calculate the row size in words (including padding)
    uint32_t rowSize = ((imageWidth * 3 + 3) & ~3) / 4;

    // Calculate the column size in words (including padding)
    uint32_t colSize = ((imageHeight * 3 + 3) & ~3) / 4;

    // Allocate memory for the pixel data
    uint32_t *pixels = (uint32_t*)malloc(rowSize * 4 * imageHeight);

    // Allocate memory buffer for image columns
    uint32_t *colBuffer = (uint32_t*)malloc(colSize * 4);

    printf("imageHeight: %d, imageWidth: %d\n", imageHeight, imageWidth);
    printf("rowSize: %d, colSize: %d\n", rowSize, colSize);



    // Read the pixel data
    fread(pixels, rowSize * 4 * imageHeight, 1, fp);

    fclose(fp);

    uint32_t x;
    uint32_t y;

    uint32_t ch1;
    uint32_t ch2;
    uint32_t tmp1;
    uint32_t tmp2;

    // GR bottom
    uint32_t k0;
    uint32_t k1;
    uint32_t k2;

    // GB bottom
    uint32_t k3;
    uint32_t k4;
    uint32_t k5;

    // GR top
    uint32_t k6;
    uint32_t k7;
    uint32_t k8;

    // GB top
    uint32_t k9;
    uint32_t k10;
    uint32_t k11;

    /**
     * Kernel configurations:
     * 
     *     K0, K3, K6, K9            K1, 4, 7, 10             K2, 5, 8, 11
     * +----+----+----+----+    +----+----+----+----+    +----+----+----+----+
     * | B1 | R0 | G0 | B0 |    | G1 | B1 | R0 | G0 |    | R1 | G1 | B1 | R0 |
     * +----+----+----+----+    +----+----+----+----+    +----+----+----+----+
     * 
     *   --> For GR rows, we use ch1 for green, ch2 for red.
     *   --> For GB rows, we use ch1 for green, ch2 for blue.
     */
    for (y = 0; y < imageHeight; y ++) {
        // Loop prologue
        k0 = pixels[y * rowSize];
        k1 = pixels[y * rowSize + 1];
        k2 = pixels[y * rowSize + 2];

        // 0. Read K1[R0] into ch2
        ch2 = k1;
        ch2 = ch2 & 0x0000FF00;
        ch2 = ch2 >> 8;

        // 0. Read K0[G0] into ch1
        ch1 = k0;
        ch1 = ch1 >> 8;

        for (x = 0; x < imageWidth / 4; x ++) {
            // 1. Read K1[R0] into ch2
            tmp2 = ch2;
            ch2 = k1;
            ch2 = ch2 & 0x0000FF00;
            ch2 = ch2 >> 8;
            tmp2 = tmp2 + ch2; // Combine ch2

            // 2. Write ch2 to K0[R0].
            tmp2 = tmp2 >> 1; // Divide by 2
            tmp2 = tmp2 << 16;
            k0 = k0 & 0xFF00FFFF;
            k0 = k0 | tmp2;

            // 3. Advance K0.
            pixels[(y * rowSize / 1) + (3 * x)] = k0; // Write K0 back to memory
            k0 = pixels[(y * rowSize / 1) + (3 * x) + 3]; // Read into K0

            // 4. Read K1[G1] into ch1
            tmp1 = ch1; // Copy ch1 into tmp1 to perform sum
            ch1 = k1;
            ch1 = ch1 & 0xFF000000;
            ch1 = ch1 >> 24;
            tmp1 = tmp1 + ch1; // Combine ch1

            // 5. Write ch1 to K1[G0]
            tmp1 = tmp1 >> 1; // Divide by 2
            k1 = k1 & 0xFFFFFF00; // TODO not needed?
            k1 = k1 | tmp1;

            // 6. Advance K1
            pixels[(y * rowSize / 1) + (3 * x) + 1] = k1; // Write k1 to memory
            k1 = pixels[(y * rowSize / 1) + (3 * x) + 3 + 1]; // Read into k1

            // 7. Read K0[G0] into ch1
            tmp1 = ch1;
            ch1 = k0;
            ch1 = ch1 >> 8;
            ch1 = ch1 & 0x000000FF;
            tmp1 = tmp1 + ch1;

            // 8. Read K2[R1] into ch2
            tmp2 = ch2;
            ch2 = k2;
            ch2 = ch2 & 0xFF000000;
            ch2 = ch2 >> 24;
            tmp2 = tmp2 + ch2; // Combine ch2
            
            // 9. Write ch2 to K2[R0]
            tmp2 = tmp2 >> 1; // Divide by 2
            k2 = k2 & 0xFFFFFF00; // TODO not needed?
            k2 = k2 | tmp2;

            // 10. Read K0[G0] into ch1
            tmp1 = ch1;
            ch1 = k0;
            ch1 = ch1 >> 8;
            ch1 = ch1 & 0x000000FF;
            tmp1 = tmp1 + ch1;
            
            // 11. Write ch1 to K2[G1]
            tmp1 = tmp1 >> 1; // Divide by 2
            tmp1 = tmp1 << 16;
            k2 = k2 & 0xFF00FFFF; // TODO not needed?
            k2 = k2 | tmp1;

            // 12. Advance K2
            pixels[(y * rowSize / 1) + (3 * x) + 2] = k2; // Write k2
            k2 = pixels[(y * rowSize / 1) + (3 * x) + 3 + 2]; // Read into k2
        }

        y ++;

        // 0. Preload ch1 with K1[G0]
        ch1 = k1;
        ch1 = ch1 & 0x000000FF;

        // 0. Load K0[B0] into ch2
        ch2 = k0;
        ch2 = ch2 & 0x000000FF;

        for (x = 0; x < imageWidth / 4; x ++) {
            // 1. Read K1[G0] into ch1
            tmp1 = ch1;
            ch1 = k1;
            ch1 = ch1 & 0x000000FF;
            tmp1 = tmp1 + ch1; // Combine ch1
            
            // 2. Read K1[B1] into ch2
            tmp2 = ch2;
            ch2 = k1;
            ch2 = ch2 & 0x00FF0000;
            ch2 = ch2 >> 16;
            tmp2 = tmp2 + ch2; // Combine ch2

            // 3. Write ch2 to K0[B1].
            tmp2 = tmp2 >> 1; // Divide by 2
            tmp2 = tmp2 << 24;
            k0 = k0 & 0x00FFFFFF;
            k0 = k0 | tmp2;

            // 4. Write ch1 to K0[G0]
            tmp1 = tmp1 >> 1; // Divide by 2
            tmp1 = tmp1 << 8;
            k0 = k0 & 0xFFFF00FF; // TODO not needed?
            k0 = k0 | tmp1;

            // 5. Advance K0.
            pixels[(y * rowSize / 1) + (3 * x)] = k0; // Write K0 back to memory
            k0 = pixels[(y * rowSize / 1) + (3 * x) + 3]; // Read into K0

            // 6. Read K1[G0] into ch1
            tmp1 = ch1;
            ch1 = k1;
            ch1 = ch1 & 0x000000FF;
            tmp1 = tmp1 + ch1;

            // 7. Read K2[G1] into ch1
            tmp1 = ch1;
            ch1 = k2;
            ch1 = ch1 & 0x00FF0000;
            ch1 = ch1 >> 16;
            tmp1 = tmp1 + ch1;

            // 8. Write ch1 to K1[G1]
            tmp1 = tmp1 >> 1; // Divide by 2
            tmp1 = tmp1 << 24;
            k1 = k1 & 0x00FFFFFF; // TODO not needed?
            k1 = k1 | tmp1;

            // 9. Advance K1
            pixels[(y * rowSize / 1) + (3 * x) + 1] = k1; // Write k1 to memory
            k1 = pixels[(y * rowSize / 1) + (3 * x) + 3 + 1]; // Read into k1

            // 10. Read K0[B0] into ch2
            tmp2 = ch2;
            ch2 = k0;
            ch2 = ch2 & 0x000000FF;
            tmp2 = tmp2 + ch2;

            // 11. Write ch2 to K2[B1]
            tmp2 = tmp2 >> 1; // Divide by 2
            tmp2 = tmp2 << 8;
            k2 = k2 & 0xFFFF00FF; // TODO not needed?
            k2 = k2 | tmp2;

            // 12. Advance K2
            pixels[(y * rowSize / 1) + (3 * x) + 2] = k2; // Write k2
            k2 = pixels[(y * rowSize / 1) + (3 * x) + 3 + 2]; // Read into k2
        }
    }

    /*
    // Read column
    int i;
    x = 0;
    for (y = 0; y < imageHeight; y ++) {
        // colBuffer[y] = pixels[(y * rowSize / 1) + (3 * x)];
        *((uint8_t*)colBuffer + y * 3) = pixels[(y * rowSize / 1) + (3 * x)];
    }

    // Create a temp hex file
    FILE *hexFp = fopen("out.bin", "wb");
    if (!outFp) {
        printf("Error creating temp hex file.\n");
        free(pixels);
        return 1;
    }

    fwrite(colBuffer, colSize * 4, 1, hexFp);

    // Clean up
    free(colBuffer);
    fclose(hexFp);

    */

    // Create a new output file to write the modified image
    FILE *outFp = fopen("data/decoded/clouds.bmp", "wb");
    if (!outFp) {
        printf("Error creating output file.\n");
        free(pixels);
        return 1;
    }


    // Write the BMP header and information header
    fwrite(&header, sizeof(BMPHeader), 1, outFp);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outFp);

    // Write the modified pixel data
    fwrite(pixels, rowSize * 4 * imageHeight, 1, outFp);

    // Clean up
    free(pixels);
    fclose(outFp);

    printf("Goodbye!\n");

    return 0;
}
