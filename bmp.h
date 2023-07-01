#ifndef BMP_H
#define BMP_H
#include <stdint.h>

// Pixel structure
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel;

// BMP structure
typedef struct
{
    uint16_t file_byte_number;
    uint8_t pixel_array_start;

    uint8_t width;
    uint8_t height;
    uint8_t depth;

    pixel *pixels;
} BMP;

// Public function declarations

BMP *bopen(char *file_path);
// BMP *b_deep_copy(BMP *to_copy);
uint8_t get_width(BMP *bmp);
uint8_t get_height(BMP *bmp);
uint8_t get_depth(BMP *bmp);
// void get_pixel_rgb(BMP *bmp, int x, int y, unsigned char *r, unsigned char *g, unsigned char *b);
// void set_pixel_rgb(BMP *bmp, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void bwrite(BMP *bmp, char *file_name);
void bclose(BMP *bmp);

#endif
