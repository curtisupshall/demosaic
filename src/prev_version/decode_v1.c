#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define RED 2
#define GREEN 1
#define BLUE 0

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
// uint8_t *loadImage(char *path, BMPHeader *header, BMPInfoHeader *infoHeader)
// {

// }

void writeImage(char *path, uint8_t *pixels, BMPHeader *header, BMPInfoHeader *infoHeader)
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

    rowSize = ((infoHeader->width * 3 + 3) & ~3);

    // Write the modified pixel data
    fwrite(pixels, rowSize * infoHeader->height, 1, outFp);

    // Clean up
    free(pixels);
    fclose(outFp);
}

int decodeImage(uint8_t *pixels, uint32_t rowSize, int32_t imageWidth, int32_t imageHeight)
{
    uint8_t *px;
    int y, x;
    // Access pixel values
    for (y = 2; y < imageHeight - 2; y++)
    {
        asm("rowStrt:");

        for (x = 2; x < imageWidth - 2; x++)
        {
            // Gr pixel
            px = &pixels[y * rowSize + x * 3];

            // Red channel derived from left and right pixels
            px[RED] = (pixels[y * rowSize + ((x - 1) * 3) + RED] + pixels[y * rowSize + ((x + 1) * 3) + RED]) / 2;

            // Blue channel derived from above and below pixels
            px[BLUE] = (pixels[(y + 1) * rowSize + (x * 3) + BLUE] + pixels[(y - 1) * rowSize + (x * 3) + BLUE]) / 2;

            x++;

            // R pixel
            px = &pixels[y * rowSize + x * 3];

            // Green channel derived form four adjacent green pixels
            px[GREEN] = (pixels[(y + 1) * rowSize + (x * 3) + GREEN] +
                         pixels[(y - 1) * rowSize + (x * 3) + GREEN] +
                         pixels[y * rowSize + ((x - 1) * 3) + GREEN] +
                         pixels[y * rowSize + ((x + 1) * 3) + GREEN]) /
                        4;

            // Blue channel derived from four diagonal neighbouring blue pixels
            px[BLUE] = (pixels[(y + 1) * rowSize + ((x - 1) * 3) + BLUE] +
                        pixels[(y + 1) * rowSize + ((x + 1) * 3) + BLUE] +
                        pixels[(y - 1) * rowSize + ((x - 1) * 3) + BLUE] +
                        pixels[(y - 1) * rowSize + ((x + 1) * 3) + BLUE]) /
                       4;
        }

        y++;
        for (x = 0; x < imageWidth; x++)
        {
            asm("ColStrt:");

            // B pixel
            px = &pixels[y * rowSize + x * 3];

            // Red channel derived from four diagonal neighbouring red pixels
            px[RED] = (pixels[(y + 1) * rowSize + ((x - 1) * 3) + RED] +
                       pixels[(y + 1) * rowSize + ((x + 1) * 3) + RED] +
                       pixels[(y - 1) * rowSize + ((x - 1) * 3) + RED] +
                       pixels[(y - 1) * rowSize + ((x + 1) * 3) + RED]) /
                      4;

            // Green channel derived form four adjacent green pixels
            px[GREEN] = (pixels[(y + 1) * rowSize + (x * 3) + GREEN] +
                         pixels[(y - 1) * rowSize + (x * 3) + GREEN] +
                         pixels[y * rowSize + ((x - 1) * 3) + GREEN] +
                         pixels[y * rowSize + ((x + 1) * 3) + GREEN]) /
                        4;

            x++;

            // Gb pixel
            px = &pixels[y * rowSize + x * 3];

            // Red channel derived from above and below pixels
            px[RED] = (pixels[(y + 1) * rowSize + (x * 3) + RED] + pixels[(y - 1) * rowSize + (x * 3) + RED]) / 2;

            // Blue channel derived from left and right pixels
            px[BLUE] = (pixels[y * rowSize + ((x - 1) * 3) + BLUE] + pixels[y * rowSize + ((x + 1) * 3) + BLUE]) / 2;
        }
    }
}

int main()
{
    const char *inFolder = "data/encoded";
    const char *outFolder = "data/decoded";

    DIR *dir;
    struct dirent *entry;

    FILE *fp;

    BMPHeader header;
    BMPInfoHeader infoHeader;
    uint8_t *pixels;
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
            // pixels = loadImage(imagePath, &header, &infoHeader);

            fp = fopen(imagePath, "rb");
            if (!fp)
            {
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
            rowSize = ((infoHeader.width * 3 + 3) & ~3);

            // Allocate memory for the pixel data
            uint8_t *pixels = (uint8_t *)malloc(rowSize * infoHeader.height);

            // Read the pixel data
            fread(pixels, rowSize * infoHeader.height, 1, fp);

            fclose(fp);

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
