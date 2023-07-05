#ifndef BMP_H
#define BMP_H
#include <stdint.h>

#define sizeof_BMPHeader 14
#define sizeof_BMPInfo 40
// Pixel structure
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} BGR8Bit;

typedef struct RGB8Bit
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB8Bit;

typedef struct BMPHeader
{
    // file signature
    char signature[2];

    // File size
    uint32_t size;

    // # of reserved bytes
    uint32_t reserved;

    // Start of image data
    uint32_t data_offset;
} BMPHeader;

typedef struct BMPInfo
{
    // Header size
    uint32_t size;

    // Image dimensions
    int32_t width;
    int32_t height;

    // Image settings
    uint16_t color_planes;
    uint16_t color_depth;
    uint32_t compression;
    uint32_t raw_size;

    // Resolution in pixels per meter
    int32_t horizontal_ppm;
    int32_t vertical_ppm;

    // Other settings
    uint32_t color_table;
    uint32_t important_colors;
} BMPInfo;

// BMP structure
typedef struct
{
    BMPHeader header;
    BMPInfo info;

    BGR8Bit *pixels;
} BMP;

// Public function declarations

BMP *bopen(FILE *fp, uint16_t data_length);

uint32_t readBitmap(BMPHeader *header, BMPInfo *info, BGR8Bit *data_ptr, size_t data_len, FILE *stream);
uint32_t getBitmapDataLength(FILE *stream);
void readInMetadata(BMPHeader *header, BMPInfo *info, FILE *stream);

void _validate_file_type(char signature[2]);
void _validate_depth(unsigned int depth);

void bwrite(BMPHeader *header, BMPInfo *info, BGR8Bit *data_ptr, size_t data_len, FILE *stream);
void bclose(BMP *bmp);

#endif
