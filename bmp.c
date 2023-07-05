#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"

// Constants

#define BITS_PER_BYTE 8

#define BLUE 0
#define GREEN 1
#define RED 2

#define PIXEL_ARRAY_START_BYTES 4
#define PIXEL_ARRAY_START_OFFSET 10

#define WIDTH_BYTES 4
#define WIDTH_OFFSET 18

#define HEIGHT_BYTES 4
#define HEIGHT_OFFSET 22

#define DEPTH_BYTES 2
#define DEPTH_OFFSET 28

// Private function declarations

void _throw_error(char *message);

void readInMetadata(BMPHeader *header, BMPInfo *info, FILE *stream);
uint32_t bytesToU32Big(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
uint32_t bytesToU32Little(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
// Public function implementations

BMP *bopen(FILE *fp, uint16_t data_length)
{
    printf("------------------bopen------------------\n");

    BMP *bmp;

    // init bmp
    bmp = (BMP *)malloc(sizeof(BMP));
    if (!bmp)
    {
        _throw_error("Unable to allocate memory for BMP struct");
    }

    // Read every pixel
    BGR8Bit pixels;
    // fread(&pixels, sizeof(BGR8Bit), data_length, fp);

    bmp->pixels = &pixels;
    printf("pixels: %d\n", bmp->pixels);

    printf("------------------bopen-Finished------------------\n");

    return bmp;
}

uint32_t readBitmap(BMPHeader *header, BMPInfo *info, BGR8Bit *data_ptr, size_t data_len, FILE *stream)
{
    // Read data
    readInMetadata(header, info, stream);

    // Read in every pixel
    return fread(data_ptr, sizeof(BGR8Bit), data_len, stream);
}

void bwrite(BMPHeader *header, BMPInfo *info, BGR8Bit *data_ptr, size_t data_len, FILE *stream)
{
    printf("------------------bwrite------------------\n");

    fwrite(header, sizeof_BMPHeader, 1, stream);
    fwrite(info, sizeof_BMPInfo, 1, stream);
    fwrite(data_ptr, sizeof(BGR8Bit), data_len, stream);

    printf("------------------bwrite-Finished------------------\n");
}

void readInMetadata(BMPHeader *header, BMPInfo *info, FILE *stream)
{
    // Fill in the header (handling little endian)
    (*header).signature[0] = fgetc(stream);
    (*header).signature[1] = fgetc(stream);
    (*header).size = bytesToU32Little(fgetc(stream), fgetc(stream),
                                      fgetc(stream), fgetc(stream));
    (*header).reserved = bytesToU32Little(fgetc(stream), fgetc(stream),
                                          fgetc(stream), fgetc(stream));
    (*header).data_offset = bytesToU32Little(fgetc(stream), fgetc(stream),
                                             fgetc(stream), fgetc(stream));

    // Read in the file info
    fread(info, sizeof_BMPInfo, 1, stream);
}

uint32_t getBitmapDataLength(FILE *stream)
{
    // Allocate the header and info data
    BMPHeader *header = (BMPHeader *)malloc(sizeof_BMPHeader);
    BMPInfo *info = (BMPInfo *)malloc(sizeof_BMPInfo);

    // Read data
    readInMetadata(header, info, stream);

    // Get the position of the first pixel
    uint32_t offset = (*header).data_offset;
    printf("offset: %d\n", offset);

    // Get the number of bytes in the file
    uint32_t num_bytes = (*header).size;
    printf("num_bytes: %d\n", num_bytes);

    // Free the used header and info
    free(header);
    free(info);

    // Return the number of pixels
    return num_bytes - offset;
}

uint32_t bytesToU32Big(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return ((a << 24) | (b << 16) | (c << 8) | (d));
}

uint32_t bytesToU32Little(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return ((a << 24) | (b << 16) | (c << 8) | (d));
}

void bclose(BMP *bmp)
{
    free(bmp->pixels);
    bmp->pixels = NULL;
    free(bmp);
    bmp = NULL;
}

// Private function implementations
void _validate_file_type(char signature[2])
{
    if (!(signature[0] == 'B' && signature[1] == 'M'))
    {
        _throw_error("Invalid file type");
    }
}

void _validate_depth(unsigned int depth)
{
    if (!(depth == 24 || depth == 32))
    {
        _throw_error("Invalid file depth");
    }
}

void _throw_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}
