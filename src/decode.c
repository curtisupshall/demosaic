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

    // printf("imageHeight: %d, imageWidth: %d\n", imageHeight, imageWidth);
    // printf("rowSize: %d, colSize: %d\n", rowSize, colSize);

    // Read the pixel data
    fread(pixels, rowSize * 4 * imageHeight, 1, fp);

    fclose(fp);

    uint32_t x;
    uint32_t y;

    uint32_t ch1;
    uint32_t ch2;
    
    //uint32_t tmp1;
    //uint32_t tmp2;

    // GR bottom
    uint32_t k0_0;
    uint32_t k0_1;
    uint32_t k0_2;
    int p0_gr;
    int p0_r;
    int tmp0_gr;
    int tmp0_r;

    // GB bottom
    uint32_t k0_3;
    uint32_t k0_4;
    uint32_t k0_5;
    int p0_gb;
    int p0_b;
    int tmp0_gb;
    int tmp0_b;

    // GR top
    uint32_t k1_0;
    uint32_t k1_1;
    uint32_t k1_2;
    int p1_gr;
    int p1_r;
    int tmp1_gr;
    int tmp1_r;

    // GB top
    uint32_t k1_3;
    uint32_t k1_4;
    uint32_t k1_5;
    int p1_gb;
    int p1_b;
    int tmp1_gb;
    int tmp1_b;

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
    for (y = 0; y < imageHeight; y += 2) {
        // Loop prologue
        k0_0 = pixels[y * rowSize];
        k0_1 = pixels[y * rowSize + 1];
        k0_2 = pixels[y * rowSize + 2];

        // Loop prologue
        k0_3 = pixels[(y + 1) * rowSize];
        k0_4 = pixels[(y + 1) * rowSize + 1];
        k0_5 = pixels[(y + 1) * rowSize + 2];

        // 0. Read K1[R0] into ch2
        ch2 = k0_1;
        ch2 = ch2 & 0x0000FF00;
        ch2 = ch2 >> 8;

        // 0. Read K0[G0] into ch1
        ch1 = k0_0;
        ch1 = ch1 >> 8;

        // GR row
        for (x = 0; x < imageWidth / 4; x ++) {
            // 1. Read K1[R0] into ch2
            tmp0_r = ch2;
            ch2 = k0_1;
            ch2 = ch2 & 0x0000FF00;
            ch2 = ch2 >> 8;
            tmp0_r = tmp0_r + ch2; // Combine ch2

            // 2. Write ch2 to K0[R0].
            tmp0_r = tmp0_r >> 1; // Divide by 2
            tmp0_r = tmp0_r << 16;
            k0_0 = k0_0 & 0xFF00FFFF;
            k0_0 = k0_0 | tmp0_r;

            // 3. Advance K0.
            pixels[y * rowSize + (3 * x)] = k0_0; // Write K0 back to memory
            k0_0 = pixels[y * rowSize + (3 * x) + 3]; // Read into K0

            // 4. Read K1[G1] into ch1
            tmp0_gr = ch1; // Copy ch1 into tmp0_gr to perform sum
            ch1 = k0_1;
            ch1 = ch1 & 0xFF000000;
            ch1 = ch1 >> 24;
            tmp0_gr = tmp0_gr + ch1; // Combine ch1

            // 5. Write ch1 to K1[G0]
            tmp0_gr = tmp0_gr >> 1; // Divide by 2
            k0_1 = k0_1 & 0xFFFFFF00; // TODO not needed?
            k0_1 = k0_1 | tmp0_gr;

            // 6. Advance K1
            pixels[y * rowSize + (3 * x) + 1] = k0_1; // Write k0_1 to memory
            k0_1 = pixels[y * rowSize + (3 * x) + 3 + 1]; // Read into k0_1

            // 7. Read K0[G0] into ch1
            tmp0_gr = ch1;
            ch1 = k0_0;
            ch1 = ch1 >> 8;
            ch1 = ch1 & 0x000000FF;
            tmp0_gr = tmp0_gr + ch1;

            // 8. Read K2[R1] into ch2
            tmp0_r = ch2;
            ch2 = k0_2;
            ch2 = ch2 & 0xFF000000;
            ch2 = ch2 >> 24;
            tmp0_r = tmp0_r + ch2; // Combine ch2
            
            // 9. Write ch2 to K2[R0]
            tmp0_r = tmp0_r >> 1; // Divide by 2
            k0_2 = k0_2 & 0xFFFFFF00; // TODO not needed?
            k0_2 = k0_2 | tmp0_r;

            // 10. Read K0[G0] into ch1
            tmp0_gr = ch1;
            ch1 = k0_0;
            ch1 = ch1 >> 8;
            ch1 = ch1 & 0x000000FF;
            tmp0_gr = tmp0_gr + ch1;
            
            // 11. Write ch1 to K2[G1]
            tmp0_gr = tmp0_gr >> 1; // Divide by 2
            tmp0_gr = tmp0_gr << 16;
            k0_2 = k0_2 & 0xFF00FFFF; // TODO not needed?
            k0_2 = k0_2 | tmp0_gr;

            // 12. Advance K2
            pixels[y * rowSize + (3 * x) + 2] = k0_2; // Write k0_2
            k0_2 = pixels[y * rowSize + (3 * x) + 3 + 2]; // Read into k0_2
        }

        // 0. Preload ch1 with K4[G0]
        ch1 = k0_4;
        ch1 = ch1 & 0x000000FF;

        // 0. Load K3[B0] into ch2
        ch2 = k0_3;
        ch2 = ch2 & 0x000000FF;

        // GB row
        for (x = 0; x < imageWidth / 4; x ++) {
            // 1. Read K4[G0] into ch1
            tmp0_gb = ch1;
            ch1 = k0_4;
            ch1 = ch1 & 0x000000FF;
            tmp0_gb = tmp0_gb + ch1; // Combine ch1
            
            // 2. Read K4[B1] into ch2
            tmp0_b = ch2;
            ch2 = k0_4;
            ch2 = ch2 & 0x00FF0000;
            ch2 = ch2 >> 16;
            tmp0_b = tmp0_b + ch2; // Combine ch2

            // 3. Write ch2 to K3[B1].
            tmp0_b = tmp0_b >> 1; // Divide by 2
            tmp0_b = tmp0_b << 24;
            k0_3 = k0_3 & 0x00FFFFFF;
            k0_3 = k0_3 | tmp0_b;

            // 4. Write ch1 to K3[G0]
            tmp0_gb = tmp0_gb >> 1; // Divide by 2
            tmp0_gb = tmp0_gb << 8;
            k0_3 = k0_3 & 0xFFFF00FF; // TODO not needed?
            k0_3 = k0_3 | tmp0_gb;

            // 5. Advance K3.
            pixels[(y + 1) * rowSize + (3 * x)] = k0_3; // Write K3 back to memory
            k0_3 = pixels[(y + 1) * rowSize + (3 * x) + 3]; // Read into K3

            // 6. Read K4[G0] into ch1
            tmp0_gb = ch1;
            ch1 = k0_4;
            ch1 = ch1 & 0x000000FF;
            tmp0_gb = tmp0_gb + ch1;

            // 7. Read K5[G1] into ch1
            tmp0_gb = ch1;
            ch1 = k0_5;
            ch1 = ch1 & 0x00FF0000;
            ch1 = ch1 >> 16;
            tmp0_gb = tmp0_gb + ch1;

            // 8. Write ch1 to K4[G1]
            tmp0_gb = tmp0_gb >> 1; // Divide by 2
            tmp0_gb = tmp0_gb << 24;
            k0_4 = k0_4 & 0x00FFFFFF; // TODO not needed?
            k0_4 = k0_4 | tmp0_gb;

            // 9. Advance K4
            pixels[(y + 1) * rowSize + (3 * x) + 1] = k0_4; // Write k0_4 to memory
            k0_4 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 1]; // Read into k0_4

            // 10. Read K3[B0] into ch2
            tmp0_b = ch2;
            ch2 = k0_3;
            ch2 = ch2 & 0x000000FF;
            tmp0_b = tmp0_b + ch2;

            // 11. Write ch2 to K5[B1]
            tmp0_b = tmp0_b >> 1; // Divide by 2
            tmp0_b = tmp0_b << 8;
            k0_5 = k0_5 & 0xFFFF00FF; // TODO not needed?
            k0_5 = k0_5 | tmp0_b;

            // 12. Advance K5
            pixels[(y + 1) * rowSize + (3 * x) + 2] = k0_5; // Write k0_5
            k0_5 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 2]; // Read into k0_5
        }
    }

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
