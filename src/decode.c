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

static const uint32_t IMAGE_WIDTH = 4000;
static const uint32_t IMAGE_HEIGHT = 6000;

// Calculate the row size in words (including padding)
static const uint32_t ROW_SIZE = 3000; //((IMAGE_WIDTH * 3 + 3) & ~3) / 4;

/**
 * Loads an image, returning a pointer to its pixel data.
 */
uint32_t *loadImage(char *path, BMPHeader *header, BMPInfoHeader *infoHeader)
{
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

    // Allocate memory for the pixel data
    pixels = (uint32_t *)malloc(ROW_SIZE * 4 * infoHeader->height);

    if (!pixels)
    {
        printf("Failed to allocate enough memory.\n");
        exit(1);
    }

    // Read the pixel data
    fread(pixels, ROW_SIZE * 4 * infoHeader->height, 1, fp);

    fclose(fp);
    return pixels;
}

void writeImage(char *path, uint32_t *pixels, BMPHeader *header, BMPInfoHeader *infoHeader)
{
    // Create a new output file to write the modified image
    FILE *outFp;

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

    // Write the modified pixel data
    fwrite(pixels, ROW_SIZE * 4 * infoHeader->height, 1, outFp);

    // Clean up
    free(pixels);
    fclose(outFp);
}

int decodeImage(uint32_t *pixels)
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
    for (y = 0; y < IMAGE_HEIGHT; y += 4)
    {
        asm("rowStrt:");

        // Loop prologue
        k0_1 = pixels[y * ROW_SIZE + 1];
        k0_2 = pixels[y * ROW_SIZE + 2];
        k0_3 = pixels[(y + 1) * ROW_SIZE];
        k0_4 = pixels[(y + 1) * ROW_SIZE + 1];
        k0_5 = pixels[(y + 1) * ROW_SIZE + 2];
        k1_0 = pixels[(y + 2) * ROW_SIZE];
        k1_1 = pixels[(y + 2) * ROW_SIZE + 1];
        k1_2 = pixels[(y + 2) * ROW_SIZE + 2];
        k1_3 = pixels[(y + 3) * ROW_SIZE];
        k1_4 = pixels[(y + 3) * ROW_SIZE + 1];
        k1_5 = pixels[(y + 3) * ROW_SIZE + 2];

        for (x = 0; x < IMAGE_WIDTH / 4; x++)
        {
            // Load TMP
            tmp0_r = p0_r;
            tmp1_r = p1_r;
            tmp0_gb = p0_gb;
            tmp1_gb = p1_gb;
            tmp0_b = p0_b;
            tmp1_b = p1_b;
            tmp0_gr = p0_gr;
            tmp1_gr = p1_gr;

            // Read into P
            p0_r = (k0_1 & 0x0000FF00) >> 8;
            p1_r = (k1_1 & 0x0000FF00) >> 8;
            p0_gb = (k1_1 & 0x0000FF00) >> 8;
            p1_gb = ((k1_4 & 0x000000FF) >> 1) << 8;
            p0_b = (k0_4 & 0x00FF0000) >> 16;
            p1_b = (k1_4 & 0x00FF0000) >> 16;
            p0_gr = (k0_1 & 0xFF000000) >> 24;
            p1_gr = (k1_1 & 0xFF000000) >> 24;

            // Write into Pixels
            pixels[y * ROW_SIZE + (3 * x)] = (k0_0 & 0xFF00FFFF) | ((tmp0_r + p0_r) >> 1) << 16;                                                      // Write K0_0 back to memory
            pixels[(y + 2) * ROW_SIZE + (3 * x)] = (k1_0 & 0xFF00FFFF) | ((((tmp1_r + p1_r) >> 1) << 16) >> 1) << 16;                                 // Write K1_0 back to memory
            pixels[(y + 1) * ROW_SIZE + (3 * x)] = (k0_3 & 0x00FFFFFF) | ((tmp0_b + p0_b) >> 1) << 24 | ((((tmp0_gb + p0_gb) >> 1) << 16) >> 1) << 8; // Write K0_3 back to memory
            pixels[(y + 3) * ROW_SIZE + (3 * x)] = (k1_3 & 0x00FFFFFF) | ((tmp1_b + p1_b) >> 1) << 24 | ((tmp1_gb + p1_gb) >> 1) << 8;                // Write K1_3 back to memory
            pixels[y * ROW_SIZE + (3 * x) + 1] = k0_1 | (tmp0_gr + p0_gr) >> 1;                                                                       // Write k0_1 to memory
            pixels[(y + 2) * ROW_SIZE + (3 * x) + 1] = k1_1 | (tmp1_gr + p1_gr) >> 1;                                                                 // Write k1_1 to memory

            // Advance K
            k0_0 = pixels[y * ROW_SIZE + (3 * x) + 3];           // Read into K0_0
            k1_0 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3];     // Read into K1_0
            k0_3 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3];     // Read into K0_3
            k1_3 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3];     // Read into K1_3
            k0_1 = pixels[y * ROW_SIZE + (3 * x) + 3 + 1];       // Read into k0_1
            k1_1 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k1_1

            // Write into K
            k0_4 = k0_4 | (((p0_gb + ((k0_5 & 0x00FF0000) >> 16)) >> 1) << 24);
            k1_4 = k1_4 | ((p1_gb + ((k1_5 & 0x00FF0000) >> 16)) >> 1) << 24;
            k0_2 = k0_2 | (p0_r + ((k0_2 & 0xFF000000) >> 24)) >> 1 | ((p0_gr + ((k0_0 >> 8) & 0x000000FF)) >> 1) << 16;
            k1_2 = k1_2 | (p1_r + ((k1_2 & 0xFF000000) >> 24)) >> 1 | ((p1_gr + ((k1_0 >> 8) & 0x000000FF)) >> 1) << 16;
            k0_5 = k0_5 | ((tmp0_b + (k0_3 & 0x000000FF)) >> 1) << 8;
            k1_5 = k1_5 | ((p1_b + (k1_3 & 0x000000FF)) >> 1) << 8;

            // Write into Pixels
            pixels[(y + 1) * ROW_SIZE + (3 * x) + 1] = k0_4; // Write k0_4 to memory
            pixels[(y + 3) * ROW_SIZE + (3 * x) + 1] = k1_4; // Write k1_4 to memory
            pixels[y * ROW_SIZE + (3 * x) + 2] = k0_2;       // Write k0_2
            pixels[(y + 2) * ROW_SIZE + (3 * x) + 2] = k1_2; // Write k1_2
            pixels[(y + 1) * ROW_SIZE + (3 * x) + 2] = k0_5; // Write k0_5
            pixels[(y + 3) * ROW_SIZE + (3 * x) + 2] = k1_5; // Write k1_5

            // Advance K
            k0_4 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k0_4
            k1_4 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k1_4
            k0_2 = pixels[y * ROW_SIZE + (3 * x) + 3 + 2];       // Read into k0_2
            k1_2 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k1_2
            k0_5 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k0_5
            k1_5 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k1_5
        }
    }

    for (y = 0; y < IMAGE_HEIGHT - 2; y += 2)
    {
        asm("ColStrt:");

        // Loop prologue
        k0_0 = pixels[y * ROW_SIZE];
        k0_1 = pixels[y * ROW_SIZE + 1];
        k0_2 = pixels[y * ROW_SIZE + 2];
        k0_3 = pixels[(y + 1) * ROW_SIZE];
        k0_4 = pixels[(y + 1) * ROW_SIZE + 1];
        k0_5 = pixels[(y + 1) * ROW_SIZE + 2];
        k1_0 = pixels[(y + 2) * ROW_SIZE];
        k1_1 = pixels[(y + 2) * ROW_SIZE + 1];
        k1_2 = pixels[(y + 2) * ROW_SIZE + 2];
        k1_3 = pixels[(y + 3) * ROW_SIZE];
        k1_4 = pixels[(y + 3) * ROW_SIZE + 1];
        k1_5 = pixels[(y + 3) * ROW_SIZE + 2];

        for (x = 0; x < IMAGE_WIDTH / 4; x++)
        {
            // * Write K0_3[B0, B1] + K1_3[B0, B1] to K1_0[R0].
            k1_0 = k1_0 | (((k0_3 & 0x000000FF) + (k1_3 & 0x000000FF)) >> 1) & 0x000000FF;
            k1_0 = k1_0 | ((k0_3 >> 1) + (k1_3 >> 1)) & 0xFF000000;
            k0_3 = k0_3 | (((k0_0 & 0x00FF0000) + (k1_0 & 0x00FF0000)) >> 1) & 0x00FF0000;
            k0_4 = k0_4 | ((k0_1 >> 1) & 0x0000FF00 + (k0_1 >> 1) & 0x0000FF00) & 0x0000FF00;
            k1_1 = k1_1 | ((k0_4 & 0x00FF0000) >> 1 & 0x00FF0000 + (k1_4 & 0x00FF0000) >> 1 & 0x00FF0000) & 0x00FF0000;
            k0_5 = k0_5 | (((k0_2 & 0x000000FF) + (k1_2 & 0x000000FF) >> 1) & 0x000000FF) | ((k0_2 >> 1) + (k1_2 >> 1) & 0xFF000000);
            k1_2 = k1_2 | ((((k0_5 & 0x0000FF00) >> 1) & 0x0000FF00) + (((k1_5 & 0x0000FF00) >> 1) & 0x0000FF00)) & 0x0000FF00;

            // Write into Pixels
            pixels[y * ROW_SIZE + (3 * x)] = k0_0;     // Write K0_0 back to memory
            pixels[(y + 2) * ROW_SIZE + (3 * x)] = k1_0;     // Write K1_0 back to memory
            pixels[(y + 1) * ROW_SIZE + (3 * x)] = k0_3;     // Write K0_3 back to memory
            pixels[(y + 3) * ROW_SIZE + (3 * x)] = k1_3;     // Write K1_3 back to memory
            pixels[(y + 2) * ROW_SIZE + (3 * x) + 1] = k1_1;     // Write k1_1 to memory
            pixels[(y + 1) * ROW_SIZE + (3 * x) + 1] = k0_4;     // Write k0_4 to memory
            pixels[y * ROW_SIZE + (3 * x) + 1] = k0_1;     // Write k0_1 to memory
            pixels[(y + 3) * ROW_SIZE + (3 * x) + 1] = k1_4;     // Write k1_4 to memory
            pixels[y * ROW_SIZE + (3 * x) + 2] = k0_2;     // Write k0_2
            pixels[(y + 2) * ROW_SIZE + (3 * x) + 2] = k1_2;     // Write k1_2
            pixels[(y + 1) * ROW_SIZE + (3 * x) + 2] = k0_5;     // Write k0_5
            pixels[(y + 3) * ROW_SIZE + (3 * x) + 2] = k1_5;     // Write k1_5
           
            // Advance K
            k0_0 = pixels[y * ROW_SIZE + (3 * x) + 3]; // Read into K0_0
            k1_0 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3]; // Read into K1_0
            k0_3 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3]; // Read into K0_3
            k1_3 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3]; // Read into K1_3
            k0_1 = pixels[y * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k0_1
            k1_1 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k1_1
            k0_4 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k0_4
            k1_4 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3 + 1]; // Read into k1_4
            k0_2 = pixels[y * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k0_2
            k1_2 = pixels[(y + 2) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k1_2
            k0_5 = pixels[(y + 1) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k0_5
            k1_5 = pixels[(y + 3) * ROW_SIZE + (3 * x) + 3 + 2]; // Read into k1_5
        }
    }
}

int main()
{
    const char *inFolder = "data/encoded";
    const char *outFolder = "data/decoded";

    DIR *dir;
    struct dirent *entry;

    BMPHeader header;
    BMPInfoHeader infoHeader;
    uint32_t *pixels;
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

            // Process image
            decodeImage(pixels);

            // Write image
            snprintf(imagePath, sizeof(imagePath), "%s/%s", outFolder, file_name);
            writeImage(imagePath, pixels, &header, &infoHeader);
        }
    }

    closedir(dir);

    printf("Goodbye!\n");
    exit(0);
}
