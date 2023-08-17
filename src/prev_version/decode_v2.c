#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#pragma pack(push, 1)
typedef struct
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct
{
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

/**
 * Loads an image, returning a pointer to its pixel data.
 */
uint32_t *loadImage(char *path, BMPHeader *header, BMPInfoHeader *infoHeader)
{
    uint32_t rowSize;
    uint32_t *pixels;
    FILE *fp;

    fp = fopen(path, "rb");
    if (!fp)
    {
        printf("Error opening file: %s.\n", path);
        exit(1);
    }

    // Read the BMP header
    fread(header, sizeof(BMPHeader), 1, fp);
    fread(infoHeader, sizeof(BMPInfoHeader), 1, fp);

    // Check if it's a valid BMP file
    if (header->type != 0x4D42)
    {
        printf("Invalid BMP file.\n");
        fclose(fp);
        exit(1);
    }

    // Check if it's a 24-bit BMP file
    if (infoHeader->bitDepth != 24)
    {
        printf("Unsupported bit depth. Only 24-bit BMP is supported.\n");
        fclose(fp);
        exit(1);
    }

    // Calculate the row size in words (including padding)
    rowSize = ((infoHeader->width * 3 + 3) & ~3) / 4;

    // Allocate memory for the pixel data
    pixels = (uint32_t *)malloc(rowSize * 4 * infoHeader->height);

    if (!pixels)
    {
        printf("Failed to allocate enough memory.\n");
        exit(1);
    }

    // Read the pixel data
    fread(pixels, rowSize * 4 * infoHeader->height, 1, fp);

    fclose(fp);
    return pixels;
}

void writeImage(char *path, uint32_t *pixels, BMPHeader *header, BMPInfoHeader *infoHeader)
{
    // Create a new output file to write the modified image
    FILE *outFp;
    uint32_t rowSize;

    outFp = fopen(path, "wb");
    if (!outFp)
    {
        printf("Error creating output file: %s\n", path);
        free(pixels);
        exit(1);
    }

    // Write the BMP header and information header
    fwrite(header, sizeof(BMPHeader), 1, outFp);
    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, outFp);

    rowSize = ((infoHeader->width * 3 + 3) & ~3) / 4;

    // Write the modified pixel data
    fwrite(pixels, rowSize * 4 * infoHeader->height, 1, outFp);

    // Clean up
    free(pixels);
    fclose(outFp);
}

int decodeImage(uint32_t *pixels, uint32_t rowSize, uint32_t imageWidth, uint32_t imageHeight)
{
    uint32_t x;
    uint32_t y;

    // GR bottom
    uint32_t k0_0;
    uint32_t k0_1;
    uint32_t k0_2;
    uint32_t p0_gr;
    uint32_t p0_r;
    uint32_t tmp0_gr;
    uint32_t tmp0_r;

    // GB bottom
    uint32_t k0_3;
    uint32_t k0_4;
    uint32_t k0_5;
    uint32_t p0_gb;
    uint32_t p0_b;
    uint32_t tmp0_gb;
    uint32_t tmp0_b;

    // GR top
    uint32_t k1_0;
    uint32_t k1_1;
    uint32_t k1_2;
    uint32_t p1_gr;
    uint32_t p1_r;
    uint32_t tmp1_gr;
    uint32_t tmp1_r;

    // GB top
    uint32_t k1_3;
    uint32_t k1_4;
    uint32_t k1_5;
    uint32_t p1_gb;
    uint32_t p1_b;
    uint32_t tmp1_gb;
    uint32_t tmp1_b;

    uint32_t tmp_mix1;
    uint32_t tmp_mix2;

    /**
     * Kernel configurations:
     *
     *      K0, K3, K6, K9            K1, 4, 7, 10             K2, 5, 8, 11
     *  +----+----+----+----+    +----+----+----+----+    +----+----+----+----+
     *  | B1 | R0 | G0 | B0 |    | G1 | B1 | R0 | G0 |    | R1 | G1 | B1 | R0 |
     *  +----+----+----+----+    +----+----+----+----+    +----+----+----+----+
     *
     *
     */
    for (y = 0; y < imageHeight; y += 4)
    {
        asm("RowStrt:");

        // Loop prologue
        k0_1 = pixels[y * rowSize + 1];
        k0_2 = pixels[y * rowSize + 2];
        k0_3 = pixels[(y + 1) * rowSize];
        k0_4 = pixels[(y + 1) * rowSize + 1];
        k0_5 = pixels[(y + 1) * rowSize + 2];
        k1_0 = pixels[(y + 2) * rowSize];
        k1_1 = pixels[(y + 2) * rowSize + 1];
        k1_2 = pixels[(y + 2) * rowSize + 2];
        k1_3 = pixels[(y + 3) * rowSize];
        k1_4 = pixels[(y + 3) * rowSize + 1];
        k1_5 = pixels[(y + 3) * rowSize + 2];

        // 0. Read K0_1[R0] into p0_r
        p0_r = k0_1;
        p0_r = p0_r & 0x0000FF00;
        p0_r = p0_r >> 8;

        // 0. Read K0_0[G0] into p0_gr
        p0_gr = k0_0;
        p0_gr = p0_gr >> 8;

        // 0. Preload p0_gb with K0_4[G0]
        p0_gb = k0_4;
        p0_gb = p0_gb & 0x000000FF;

        // 0. Load K0_3[B0] into p0_b
        p0_b = k0_3;
        p0_b = p0_b & 0x000000FF;

        // 0. Read K1_1[R0] into p1_r
        p1_r = k1_1;
        p1_r = p1_r & 0x0000FF00;
        p1_r = p1_r >> 8;

        // 0. Read K1_0[G0] into p1_gr
        p1_gr = k1_0;
        p1_gr = p1_gr >> 8;

        // 0. Preload p1_gb with K1_4[G0]
        p1_gb = k1_4;
        p1_gb = p1_gb & 0x000000FF;

        // 0. Load K1_3[B0] into p1_b
        p1_b = k1_3;
        p1_b = p1_b & 0x000000FF;

        for (x = 0; x < imageWidth / 4; x++)
        {
            // 1. Read K0_1[R0] into p0_r
            tmp0_r = p0_r;
            p0_r = k0_1;
            p0_r = p0_r & 0x0000FF00;
            p0_r = p0_r >> 8;
            tmp0_r = tmp0_r + p0_r; // Combine p0_r

            // 1. Read K1_1[R0] into p1_r
            tmp1_r = p1_r;
            p1_r = k1_1;
            p1_r = p1_r & 0x0000FF00;
            p1_r = p1_r >> 8;
            tmp1_r = tmp1_r + p1_r; // Combine p1_r

            // 1. Read K0_4[G0] into p0_gb
            tmp0_gb = p0_gb;
            p0_gb = k0_4;
            p0_gb = p0_gb & 0x000000FF;
            tmp0_gb = tmp0_gb + p0_gb; // Combine p0_gb

            // 1. Read K1_4[G0] into p1_gb
            tmp1_gb = p1_gb;
            p1_gb = k1_4;
            p1_gb = p1_gb & 0x000000FF;
            tmp1_gb = tmp1_gb + p1_gb; // Combine p1_gb

            // 2. Write p0_r to K0_0[R0].
            tmp0_r = tmp0_r >> 1; // Divide by 2
            tmp0_r = tmp0_r << 16;
            k0_0 = k0_0 & 0xFF00FFFF;
            k0_0 = k0_0 | tmp0_r;

            // 2. Write p1_r to K1_0[R0].
            tmp1_r = tmp1_r >> 1; // Divide by 2
            tmp1_r = tmp1_r << 16;
            k1_0 = k1_0 & 0xFF00FFFF;
            k1_0 = k1_0 | tmp1_r;

            // 2. Read K0_4[B1] into p0_b
            tmp0_b = p0_b;
            p0_b = k0_4;
            p0_b = p0_b & 0x00FF0000;
            p0_b = p0_b >> 16;
            tmp0_b = tmp0_b + p0_b; // Combine p0_b

            // 2. Read K1_4[B1] into p1_b
            tmp1_b = p1_b;
            p1_b = k1_4;
            p1_b = p1_b & 0x00FF0000;
            p1_b = p1_b >> 16;
            tmp1_b = tmp1_b + p1_b; // Combine p1_b

            // 3. Advance K0_0.
            pixels[y * rowSize + (3 * x)] = k0_0;     // Write K0_0 back to memory
            k0_0 = pixels[y * rowSize + (3 * x) + 3]; // Read into K0_0

            // 3. Advance K1_0.
            pixels[(y + 2) * rowSize + (3 * x)] = k1_0;     // Write K1_0 back to memory
            k1_0 = pixels[(y + 2) * rowSize + (3 * x) + 3]; // Read into K1_0

            // 3. Write p0_b to K0_3[B1].
            tmp0_b = tmp0_b >> 1; // Divide by 2
            tmp0_b = tmp0_b << 24;
            k0_3 = k0_3 & 0x00FFFFFF;
            k0_3 = k0_3 | tmp0_b;

            // 3. Write p1_b to K1_3[B1].
            tmp1_b = tmp1_b >> 1; // Divide by 2
            tmp1_b = tmp1_b << 24;
            k1_3 = k1_3 & 0x00FFFFFF;
            k1_3 = k1_3 | tmp1_b;

            // 4. Read K0_1[G1] into p0_gr
            tmp0_gr = p0_gr; // Copy p0_gr into tmp0_gr to perform sum
            p0_gr = k0_1;
            p0_gr = p0_gr & 0xFF000000;
            p0_gr = p0_gr >> 24;
            tmp0_gr = tmp0_gr + p0_gr; // Combine p0_gr

            // 4. Read K1_1[G1] into p1_gr
            tmp1_gr = p1_gr; // Copy p1_gr into tmp1_gr to perform sum
            p1_gr = k1_1;
            p1_gr = p1_gr & 0xFF000000;
            p1_gr = p1_gr >> 24;
            tmp1_gr = tmp1_gr + p1_gr; // Combine p1_gr

            // 4. Write p0_gb to K0_3[G0]
            tmp0_gb = tmp0_gb >> 1; // Divide by 2
            tmp0_gb = tmp0_gb << 8;
            k0_3 = k0_3 & 0xFFFF00FF; // TODO not needed?
            k0_3 = k0_3 | tmp0_gb;

            // 4. Write p1_gb to K1_3[G0]
            tmp1_gb = tmp1_gb >> 1; // Divide by 2
            tmp1_gb = tmp1_gb << 8;
            k1_3 = k1_3 & 0xFFFF00FF; // TODO not needed?
            k1_3 = k1_3 | tmp1_gb;

            // 5. Write p0_gr to K0_1[G0]
            tmp0_gr = tmp0_gr >> 1;   // Divide by 2
            k0_1 = k0_1 & 0xFFFFFF00; // TODO not needed?
            k0_1 = k0_1 | tmp0_gr;

            // 5. Write p1_gr to K1_1[G0]
            tmp1_gr = tmp1_gr >> 1;   // Divide by 2
            k1_1 = k1_1 & 0xFFFFFF00; // TODO not needed?
            k1_1 = k1_1 | tmp1_gr;

            // 5. Advance K0_3.
            pixels[(y + 1) * rowSize + (3 * x)] = k0_3;     // Write K0_3 back to memory
            k0_3 = pixels[(y + 1) * rowSize + (3 * x) + 3]; // Read into K0_3

            // 5. Advance K1_3.
            pixels[(y + 3) * rowSize + (3 * x)] = k1_3;     // Write K1_3 back to memory
            k1_3 = pixels[(y + 3) * rowSize + (3 * x) + 3]; // Read into K1_3

            // 6. Advance K0_1
            pixels[y * rowSize + (3 * x) + 1] = k0_1;     // Write k0_1 to memory
            k0_1 = pixels[y * rowSize + (3 * x) + 3 + 1]; // Read into k0_1

            // 6. Advance K1_1
            pixels[(y + 2) * rowSize + (3 * x) + 1] = k1_1;     // Write k1_1 to memory
            k1_1 = pixels[(y + 2) * rowSize + (3 * x) + 3 + 1]; // Read into k1_1

            // 6. Read K0_4[G0] into p0_gb
            tmp0_gb = p0_gb;
            p0_gb = k0_4;
            p0_gb = p0_gb & 0x000000FF;
            tmp0_gb = tmp0_gb + p0_gb;

            // 6. Read K1_4[G0] into p1_gb
            tmp1_gb = p1_gb;
            p1_gb = k1_4;
            p1_gb = p1_gb & 0x000000FF;

            // 7. Read K0_0[G0] into p0_gr
            tmp0_gr = p0_gr;
            p0_gr = k0_0;
            p0_gr = p0_gr >> 8;
            p0_gr = p0_gr & 0x000000FF;
            tmp0_gr = tmp0_gr + p0_gr;

            // 7. Read K1_0[G0] into p1_gr
            tmp1_gr = p1_gr;
            p1_gr = k1_0;
            p1_gr = p1_gr >> 8;
            p1_gr = p1_gr & 0x000000FF;
            tmp1_gr = tmp1_gr + p1_gr;

            // 7. Read K0_5[G1] into p0_gb
            tmp0_gb = p0_gb;
            p0_gb = k0_5;
            p0_gb = p0_gb & 0x00FF0000;
            p0_gb = p0_gb >> 16;
            tmp0_gb = tmp0_gb + p0_gb;

            // 7. Read K1_5[G1] into p1_gb
            tmp1_gb = p1_gb;
            p1_gb = k1_5;
            p1_gb = p1_gb & 0x00FF0000;
            p1_gb = p1_gb >> 16;
            tmp1_gb = tmp1_gb + p1_gb;

            // 8. Read K0_2[R1] into p0_r
            tmp0_r = p0_r;
            p0_r = k0_2;
            p0_r = p0_r & 0xFF000000;
            p0_r = p0_r >> 24;
            tmp0_r = tmp0_r + p0_r; // Combine p0_r

            // 8. Read K1_2[R1] into p1_r
            tmp1_r = p1_r;
            p1_r = k1_2;
            p1_r = p1_r & 0xFF000000;
            p1_r = p1_r >> 24;
            tmp1_r = tmp1_r + p1_r; // Combine p1_r

            // 8. Write p0_gb to K0_4[G1]
            tmp0_gb = tmp0_gb >> 1; // Divide by 2
            tmp0_gb = tmp0_gb << 24;
            k0_4 = k0_4 & 0x00FFFFFF; // TODO not needed?
            k0_4 = k0_4 | tmp0_gb;

            // 8. Write p1_gb to K1_4[G1]
            tmp1_gb = tmp1_gb >> 1; // Divide by 2
            tmp1_gb = tmp1_gb << 24;
            k1_4 = k1_4 & 0x00FFFFFF; // TODO not needed?
            k1_4 = k1_4 | tmp1_gb;

            // 9. Write p0_r to K0_2[R0]
            tmp0_r = tmp0_r >> 1;     // Divide by 2
            k0_2 = k0_2 & 0xFFFFFF00; // TODO not needed?
            k0_2 = k0_2 | tmp0_r;

            // 9. Write p1_r to K1_2[R0]
            tmp1_r = tmp1_r >> 1;     // Divide by 2
            k1_2 = k1_2 & 0xFFFFFF00; // TODO not needed?
            k1_2 = k1_2 | tmp1_r;

            // 9. Advance K0_4
            pixels[(y + 1) * rowSize + (3 * x) + 1] = k0_4;     // Write k0_4 to memory
            k0_4 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 1]; // Read into k0_4

            // 9. Advance K1_4
            pixels[(y + 3) * rowSize + (3 * x) + 1] = k1_4;     // Write k1_4 to memory
            k1_4 = pixels[(y + 3) * rowSize + (3 * x) + 3 + 1]; // Read into k1_4

            // 10. Read K0_0[G0] into p0_gr
            tmp0_gr = p0_gr;
            p0_gr = k0_0;
            p0_gr = p0_gr >> 8;
            p0_gr = p0_gr & 0x000000FF;
            tmp0_gr = tmp0_gr + p0_gr;

            // 10. Read K1_0[G0] into p1_gr
            tmp1_gr = p1_gr;
            p1_gr = k1_0;
            p1_gr = p1_gr >> 8;
            p1_gr = p1_gr & 0x000000FF;
            tmp1_gr = tmp1_gr + p1_gr;

            // 10. Read K0_3[B0] into p0_b
            tmp0_b = p0_b;
            p0_b = k0_3;
            p0_b = p0_b & 0x000000FF;
            tmp0_b = tmp0_b + p0_b;

            // 10. Read K1_3[B0] into p1_b
            tmp1_b = p1_b;
            p1_b = k1_3;
            p1_b = p1_b & 0x000000FF;
            tmp1_b = tmp1_b + p1_b;

            // 11. Write p0_gr to K0_2[G1]
            tmp0_gr = tmp0_gr >> 1; // Divide by 2
            tmp0_gr = tmp0_gr << 16;
            k0_2 = k0_2 & 0xFF00FFFF; // TODO not needed?
            k0_2 = k0_2 | tmp0_gr;

            // 11. Write p1_gr to K1_2[G1]
            tmp1_gr = tmp1_gr >> 1; // Divide by 2
            tmp1_gr = tmp1_gr << 16;
            k1_2 = k1_2 & 0xFF00FFFF; // TODO not needed?
            k1_2 = k1_2 | tmp1_gr;

            // 11. Write p0_b to K0_5[B1]
            tmp0_b = tmp0_b >> 1; // Divide by 2
            tmp0_b = tmp0_b << 8;
            k0_5 = k0_5 & 0xFFFF00FF; // TODO not needed?
            k0_5 = k0_5 | tmp0_b;

            // 11. Write p1_b to K1_5[B1]
            tmp1_b = tmp1_b >> 1; // Divide by 2
            tmp1_b = tmp1_b << 8;
            k1_5 = k1_5 & 0xFFFF00FF; // TODO not needed?
            k1_5 = k1_5 | tmp1_b;

            // 12. Advance K0_2
            pixels[y * rowSize + (3 * x) + 2] = k0_2;     // Write k0_2
            k0_2 = pixels[y * rowSize + (3 * x) + 3 + 2]; // Read into k0_2

            // 12. Advance K1_2
            pixels[(y + 2) * rowSize + (3 * x) + 2] = k1_2;     // Write k1_2
            k1_2 = pixels[(y + 2) * rowSize + (3 * x) + 3 + 2]; // Read into k1_2

            // 12. Advance K0_5
            pixels[(y + 1) * rowSize + (3 * x) + 2] = k0_5;     // Write k0_5
            k0_5 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 2]; // Read into k0_5

            // 12. Advance K1_5
            pixels[(y + 3) * rowSize + (3 * x) + 2] = k1_5;     // Write k1_5
            k1_5 = pixels[(y + 3) * rowSize + (3 * x) + 3 + 2]; // Read into k1_5
        }
    }

    for (y = 0; y < imageHeight - 2; y += 2)
    {
        asm("ColStrt:");

        // Loop prologue
        k0_0 = pixels[y * rowSize];
        k0_1 = pixels[y * rowSize + 1];
        k0_2 = pixels[y * rowSize + 2];
        k0_3 = pixels[(y + 1) * rowSize];
        k0_4 = pixels[(y + 1) * rowSize + 1];
        k0_5 = pixels[(y + 1) * rowSize + 2];
        k1_0 = pixels[(y + 2) * rowSize];
        k1_1 = pixels[(y + 2) * rowSize + 1];
        k1_2 = pixels[(y + 2) * rowSize + 2];
        k1_3 = pixels[(y + 3) * rowSize];
        k1_4 = pixels[(y + 3) * rowSize + 1];
        k1_5 = pixels[(y + 3) * rowSize + 2];

        for (x = 0; x < imageWidth / 4; x++)
        {
            // * Write K0_3[B0, B1] + K1_3[B0, B1] to K1_0[R0].
            tmp_mix1 = k0_3 & 0x000000FF;
            tmp_mix1 = tmp_mix1 + (k1_3 & 0x000000FF);
            tmp_mix1 = tmp_mix1 >> 1;
            tmp_mix1 = tmp_mix1 & 0x000000FF;
            tmp_mix2 = (k0_3 >> 1) + (k1_3 >> 1);
            tmp_mix2 = tmp_mix2 & 0xFF000000;
            k1_0 = k1_0 | tmp_mix1;
            k1_0 = k1_0 | tmp_mix2;

            // * Write K0_0[R0] + K1_0[R0] mix to K0_3[R0].
            tmp_mix1 = k0_0 & 0x00FF0000;
            tmp_mix2 = k1_0 & 0x00FF0000;
            tmp_mix1 = tmp_mix1 + tmp_mix2;
            tmp_mix1 = tmp_mix1 >> 1;
            tmp_mix1 = tmp_mix1 & 0x00FF0000;
            k0_3 = k0_3 | tmp_mix1;

            // * Write K0_1[R0] + K1_1[R0] mix to K0_4[R0].
            tmp_mix1 = k0_1 >> 1;
            tmp_mix2 = k0_1 >> 1; // OPT: tmp_mix2 = tmp_mix1
            tmp_mix1 = tmp_mix1 & 0x0000FF00;
            tmp_mix2 = tmp_mix2 & 0x0000FF00;
            tmp_mix1 = tmp_mix1 + tmp_mix2;
            tmp_mix1 = tmp_mix1 & 0x0000FF00;
            k0_4 = k0_4 | tmp_mix1;

            // * Write K0_4[B1] + K1_4[B1] to K1_1[B1].
            tmp_mix1 = (k0_4 & 0x00FF0000) >> 1;
            tmp_mix2 = (k1_4 & 0x00FF0000) >> 1;
            tmp_mix1 = tmp_mix1 & 0x00FF0000;
            tmp_mix2 = tmp_mix2 & 0x00FF0000;
            tmp_mix1 = tmp_mix1 + tmp_mix2;
            tmp_mix1 = tmp_mix1 & 0x00FF0000;
            k1_1 = k1_1 | tmp_mix1;

            // * Write K0_2[R0, R1] + K1_2[R0, R1] mix to K0_5[R0].
            tmp_mix1 = (k0_2 & 0x000000FF) + (k1_2 & 0x000000FF);
            tmp_mix1 = tmp_mix1 >> 1;
            tmp_mix1 = tmp_mix1 & 0x000000FF;
            tmp_mix2 = (k0_2 >> 1) + (k1_2 >> 1);
            tmp_mix2 = tmp_mix2 & 0xFF000000;
            k0_5 = k0_5 | tmp_mix1 | tmp_mix2;

            // * Write K0_5[B0] + K1_5[B0] mix to K1_2[B1].
            tmp_mix1 = (k0_5 & 0x0000FF00) >> 1;
            tmp_mix2 = (k1_5 & 0x0000FF00) >> 1; // OPT: tmp_mix2 = tmp_mix1
            tmp_mix1 = tmp_mix1 & 0x0000FF00;
            tmp_mix2 = tmp_mix2 & 0x0000FF00;
            tmp_mix1 = tmp_mix1 + tmp_mix2;
            tmp_mix1 = tmp_mix1 & 0x0000FF00;
            k1_2 = k1_2 | tmp_mix1;

            // 3. Advance K0_0.
            pixels[y * rowSize + (3 * x)] = k0_0;     // Write K0_0 back to memory
            k0_0 = pixels[y * rowSize + (3 * x) + 3]; // Read into K0_0

            // 3. Advance K1_0.
            pixels[(y + 2) * rowSize + (3 * x)] = k1_0;     // Write K1_0 back to memory
            k1_0 = pixels[(y + 2) * rowSize + (3 * x) + 3]; // Read into K1_0

            // 5. Advance K0_3.
            pixels[(y + 1) * rowSize + (3 * x)] = k0_3;     // Write K0_3 back to memory
            k0_3 = pixels[(y + 1) * rowSize + (3 * x) + 3]; // Read into K0_3

            // 5. Advance K1_3.
            pixels[(y + 3) * rowSize + (3 * x)] = k1_3;     // Write K1_3 back to memory
            k1_3 = pixels[(y + 3) * rowSize + (3 * x) + 3]; // Read into K1_3

            // 6. Advance K0_1
            pixels[y * rowSize + (3 * x) + 1] = k0_1;     // Write k0_1 to memory
            k0_1 = pixels[y * rowSize + (3 * x) + 3 + 1]; // Read into k0_1

            // 6. Advance K1_1
            pixels[(y + 2) * rowSize + (3 * x) + 1] = k1_1;     // Write k1_1 to memory
            k1_1 = pixels[(y + 2) * rowSize + (3 * x) + 3 + 1]; // Read into k1_1

            // 9. Advance K0_4
            pixels[(y + 1) * rowSize + (3 * x) + 1] = k0_4;     // Write k0_4 to memory
            k0_4 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 1]; // Read into k0_4

            // 9. Advance K1_4
            pixels[(y + 3) * rowSize + (3 * x) + 1] = k1_4;     // Write k1_4 to memory
            k1_4 = pixels[(y + 3) * rowSize + (3 * x) + 3 + 1]; // Read into k1_4

            // 12. Advance K0_2
            pixels[y * rowSize + (3 * x) + 2] = k0_2;     // Write k0_2
            k0_2 = pixels[y * rowSize + (3 * x) + 3 + 2]; // Read into k0_2

            // 12. Advance K1_2
            pixels[(y + 2) * rowSize + (3 * x) + 2] = k1_2;     // Write k1_2
            k1_2 = pixels[(y + 2) * rowSize + (3 * x) + 3 + 2]; // Read into k1_2

            // 12. Advance K0_5
            pixels[(y + 1) * rowSize + (3 * x) + 2] = k0_5;     // Write k0_5
            k0_5 = pixels[(y + 1) * rowSize + (3 * x) + 3 + 2]; // Read into k0_5

            // 12. Advance K1_5
            pixels[(y + 3) * rowSize + (3 * x) + 2] = k1_5;     // Write k1_5
            k1_5 = pixels[(y + 3) * rowSize + (3 * x) + 3 + 2]; // Read into k1_5
        }
    }
}

int main()
{
    asm("V2_Strt:");

    const char *inFolder = "data/encoded";
    const char *outFolder = "data/decoded";

    DIR *dir;
    struct dirent *entry;

    BMPHeader header;
    BMPInfoHeader infoHeader;
    uint32_t *pixels;
    uint32_t rowSize;
    char imagePath[1024];

    dir = opendir(inFolder);
    if (dir == NULL)
    {
        perror("Error opening encoded images directory");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the file has a .bmp extension
        const char *file_name = entry->d_name;
        const char *extension = strrchr(file_name, '.');

        if (extension != NULL && strcmp(extension, ".bmp") == 0)
        {
            snprintf(imagePath, sizeof(imagePath), "%s/%s", inFolder, file_name);
            printf("Decoding image: %s\n", imagePath);

            // Load image
            pixels = loadImage(imagePath, &header, &infoHeader);
            rowSize = ((infoHeader.width * 3 + 3) & ~3) / 4;

            // Process image
            decodeImage(pixels, rowSize, infoHeader.width, infoHeader.height);

            // Write image
            snprintf(imagePath, sizeof(imagePath), "%s/%s", outFolder, file_name);
            writeImage(imagePath, pixels, &header, &infoHeader);
        }
    }

    closedir(dir);

    printf("Goodbye!\n");
    exit(0);
}