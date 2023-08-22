
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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

int encode(char* inPath, char* outPath) {


    FILE *fp = fopen(inPath, "rb");
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
    if (header.type != 0x4D42)
    {
        printf("Invalid BMP file.\n");
        fclose(fp);
        return 1;
    }

    // Check if it's a 24-bit BMP file
    if (infoHeader.bitDepth != 24)
    {
        printf("Unsupported bit depth. Only 24-bit BMP is supported.\n");
        fclose(fp);
        return 1;
    }

    // Calculate the row size in bytes (including padding)
    uint32_t rowSize = ((infoHeader.width * 3 + 3) & ~3);

    // Allocate memory for the pixel data
    uint8_t *pixels = (uint8_t *)malloc(rowSize * infoHeader.height);

    // Read the pixel data
    fread(pixels, rowSize * infoHeader.height, 1, fp);

    fclose(fp);

    int y;
    int x;
    int z;

    // Access pixel values
    for (y = 0; y < infoHeader.height; y++)
    {
        for (x = 0; x < infoHeader.width; x++)
        {
            uint8_t *px = &pixels[y * rowSize + x * 3];
            // printf("Pixel (%d, %d): R=%d, G=%d, B=%d\n", x, y, px[2], px[1], px[0]);

            // Green pixel
            px[2] = 0; // px[1]; // Red channel
            px[0] = 0; // px[1]; // Blue channel

            x++;
            px = &pixels[y * rowSize + x * 3];
            // Red pixel
            px[1] = 0; //px[2]; // Green channel
            px[0] = 0; //px[2]; // Blue channel
        }
        y++;
        for (z = 0; z < infoHeader.width; z++)
        {
            uint8_t *px = &pixels[y * rowSize + z * 3];
            // Blue pixel
            px[2] = 0; //px[0]; 
            px[1] = 0; //px[0];

            z++;
            px = &pixels[y * rowSize + z * 3];
            // Green pixel
            px[2] = 0; // px[1]; 
            px[0] = 0; // px[1];
        }
    }

    // Create a new output file to write the modified image
    FILE *outFp = fopen(outPath, "wb");
    if (!outFp)
    {
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

int main()
{
    const char *inFolder = "data/original";
    const char *outFolder = "data/encoded";

    DIR *dir;
    struct dirent *entry;

    BMPHeader header;
    BMPInfoHeader infoHeader;
    uint32_t *pixels;
    uint32_t rowSize;
    char imagePath[1024];
    char writePath[1024];

    dir = opendir(inFolder);
    if (dir == NULL) {
        perror("Error opening encoded images directory");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Check if the file has a .bmp extension
        const char *file_name = entry->d_name;
        const char *extension = strrchr(file_name, '.');

        if (extension != NULL && strcmp(extension, ".bmp") == 0) {
            snprintf(imagePath, sizeof(imagePath), "%s/%s", inFolder, file_name);
            snprintf(writePath, sizeof(writePath), "%s/%s", outFolder, file_name);
            printf("Encoding image: %s\n", imagePath);

            // Process image
            encode(imagePath, writePath);
        }
        
    }

    closedir(dir);

    printf("Goodbye!\n");
    exit(0);
}
