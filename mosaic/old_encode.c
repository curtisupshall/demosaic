#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
    FILE *readFilePointer = fopen("cow.bmp", "rb");
    if (!readFilePointer) {
        printf("Error opening file.\n");
        return 1;
    }

    BMPHeader colorHeader;
    BMPInfoHeader colorInfoHeader;

    // Read the BMP header
    fread(&colorHeader, sizeof(BMPHeader), 1, readFilePointer);
    fread(&colorInfoHeader, sizeof(BMPInfoHeader), 1, readFilePointer);

    // Check if it's a valid BMP file
    if (colorHeader.type != 0x4D42) {
        printf("Invalid BMP file.\n");
        fclose(readFilePointer);
        return 1;
    }

    // Check if it's a 24-bit BMP file
    if (colorInfoHeader.bitDepth != 24) {
        printf("Unsupported bit depth. Only 24-bit BMP is supported.\n");
        fclose(readFilePointer);
        return 1;
    }

    // Calculate the row size in bytes (including padding)
    uint32_t colorRowSize = ((colorInfoHeader.width * 3 + 3) & ~3);
    uint32_t greyscaleRowSize = ((colorInfoHeader.width + 3) & ~3);

    // Allocate memory for the pixel data
    uint8_t *colorPixels = (uint8_t*)malloc(colorRowSize * colorInfoHeader.height);
    uint8_t *greyscalePixels = (uint8_t*)malloc(greyscaleRowSize * colorInfoHeader.height);

    // Read the pixel data
    fread(colorPixels, colorRowSize * colorInfoHeader.height, 1, readFilePointer);

    fclose(readFilePointer);

    // Access pixel values
    for (int y = 0; y < colorInfoHeader.height; y++) {
        for (int x = 0; x < colorInfoHeader.width; x ++) {
            uint8_t *px = &colorPixels[y * colorRowSize + x * 3];

            // Green pixel
            px[2] = 0; // Red channel
            px[0] = 0; // Blue channel

            x++;
            px = colorPixels[y * colorRowSize + x * 3];
            // Red pixel
            px[1] = 0; // Green channel
            px[0] = 0; // Blue channel
        }
        y++;
        for (int x = 0; x < colorInfoHeader.width; x ++) {
            uint8_t *px = &colorPixels[y * colorRowSize + x * 3];
            // Blue pixel
            px[2] = 0; 
            px[1] = 0;

            x++;
            px = &colorPixels[y * colorRowSize + x * 3];
            // Green pixel
            px[2] = 0; 
            px[0] = 0;
        }
    }

    // Create a new output file to write the modified image
    FILE *writeFilePointer = fopen("output/encoded.bmp", "wb");
    if (!writeFilePointer) {
        printf("Error creating output file.\n");
        free(colorPixels);
        free(greyscalePixels);
        return 1;
    }

    // Write the BMP header and information header
    fwrite(&colorHeader, sizeof(BMPHeader), 1, writeFilePointer);
    fwrite(&colorInfoHeader, sizeof(BMPInfoHeader), 1, writeFilePointer);

    // Write the modified pixel data
    fwrite(colorPixels, colorRowSize * colorInfoHeader.height, 1, writeFilePointer);

    // Clean up
    free(colorPixels);
    free(greyscalePixels);
    fclose(writeFilePointer);

    return 0;
}
