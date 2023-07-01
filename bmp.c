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
uint8_t _get_int_from_buffer(uint8_t bytes,
                             uint8_t offset,
                             char *buffer);
uint16_t _get_file_byte_number(FILE *fp);
// unsigned char *_get_file_byte_contents(FILE *fp, uint16_t file_byte_number);
int _validate_file_type(char buff[32]);
int _validate_depth(unsigned int depth);
uint8_t _get_pixel_array_start(char buff[32]);
uint8_t _get_width(char buff[32]);
uint8_t _get_height(char buff[32]);
uint8_t _get_depth(char buff[32]);

// Public function implementations

BMP *bopen(char *file_path)
{
    printf("bopen\n");

    char buff[32];
    BMP *bmp;
    FILE *fp;

    fp = fopen(file_path, "rb");
    if (!fp)
    {
        _throw_error("Unable to open file");
    }
    printf("file opened\n");

    // read file byte number
    bmp = (BMP *)malloc(sizeof(BMP));
    bmp->file_byte_number = _get_file_byte_number(fp);
    printf("file_byte_number: %d\n", bmp->file_byte_number);

    // read image format
    if (!fgets(buff, sizeof(buff), fp))
    {
        perror(file_path);
        exit(1);
    }
    printf("file format: %s\n", buff);

    if (!_validate_file_type(buff))
    {
        _throw_error("Invalid file type");
    }
    printf("file type validated %s\n", buff);

    bmp->pixel_array_start = _get_pixel_array_start(buff);
    printf("pixel_array_start: %d\n", bmp->pixel_array_start);

    bmp->width = _get_width(buff);
    bmp->height = _get_height(buff);
    bmp->depth = _get_depth(buff);

    printf("width: %d\n", bmp->width);
    printf("height: %d\n", bmp->height);
    printf("depth: %d\n", bmp->depth);

    if (!_validate_depth(bmp->depth))
    {
        _throw_error("Invalid file depth");
    }
    printf("depth validated\n");

    // read pixel array
    fseek(fp, bmp->pixel_array_start, SEEK_SET);
    bmp->pixels = (pixel *)malloc(sizeof(pixel) * bmp->width * bmp->height);
    fread(bmp->pixels, sizeof(pixel), bmp->width * bmp->height, fp);

    printf("pixel array read\n");

    return bmp;
}

uint8_t get_width(BMP *bmp)
{
    return bmp->width;
}

uint8_t get_height(BMP *bmp)
{
    return bmp->height;
}

uint8_t get_depth(BMP *bmp)
{
    return bmp->depth;
}

// void get_pixel_rgb(BMP* bmp, int x, int y, unsigned char* r, unsigned char* g, unsigned char* b)
// {
//     int index = y * bmp->width + x;
//     *r = bmp->pixels[index].red;
//     *g = bmp->pixels[index].green;
//     *b = bmp->pixels[index].blue;
// }

// void set_pixel_rgb(BMP* bmp, int x, int y, unsigned char r, unsigned char g, unsigned char b)
// {
//     int index = y * bmp->width + x;
//     bmp->pixels[index].red = r;
//     bmp->pixels[index].green = g;
//     bmp->pixels[index].blue = b;
// }

void bwrite(BMP *bmp, char *file_name)
{
    FILE *fp;

    fp = fopen(file_name, "wb");
    if (!fp)
    {
        _throw_error("Unable to open file");
    }
    printf("output file opened\n");

    // write header info
    fwrite("BM", 1, 2, fp);
    printf("file type written\n");
    
    

    fwrite(bmp->pixels, sizeof(pixel), bmp->width * bmp->height, fp);
    printf("pixel array written\n");

    fclose(fp);
    printf("output file closed\n");
}

void bclose(BMP *bmp)
{
    free(bmp->pixels);
    bmp->pixels = NULL;
    free(bmp);
    bmp = NULL;
}

// Private function implementations

void _throw_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

uint8_t _get_int_from_buffer(uint8_t bytes,
                             uint8_t offset,
                             char *buffer)
{
    uint8_t value = 0;
    int i;

    for (i = bytes - 1; i >= 0; --i)
    {
        value <<= 8;
        value += buffer[i + offset];
    }

    return value;
}

uint16_t _get_file_byte_number(FILE *fp)
{
    uint16_t byte_number;
    fseek(fp, 0, SEEK_END);
    byte_number = ftell(fp);
    rewind(fp);
    return byte_number;
}

// char *_get_file_byte_contents(FILE *fp, uint16_t file_byte_number)
// {
//     char *buffer[32] = (pixel *)malloc(file_byte_number * sizeof(pixel));
//     uint16_t result = fread(buffer, 1, file_byte_number, fp);

//     if (result != file_byte_number)
//     {
//         _throw_error("There was a problem reading the file");
//     }

//     return buffer;
// }

int _validate_file_type(char *file_byte_contents)
{
    return file_byte_contents[0] == 'B' && file_byte_contents[1] == 'M';
}

int _validate_depth(unsigned int depth)
{
    return depth == 24 || depth == 32;
}

uint8_t _get_pixel_array_start(char *file_byte_contents)
{
    return _get_int_from_buffer(PIXEL_ARRAY_START_BYTES, PIXEL_ARRAY_START_OFFSET, file_byte_contents);
}

uint8_t _get_width(char *file_byte_contents)
{
    return (uint8_t)_get_int_from_buffer(WIDTH_BYTES, WIDTH_OFFSET, file_byte_contents);
}

uint8_t _get_height(char *file_byte_contents)
{
    return (uint8_t)_get_int_from_buffer(HEIGHT_BYTES, HEIGHT_OFFSET, file_byte_contents);
}

uint8_t _get_depth(char *file_byte_contents)
{
    return (uint8_t)_get_int_from_buffer(DEPTH_BYTES, DEPTH_OFFSET, file_byte_contents);
}
