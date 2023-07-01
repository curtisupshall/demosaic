#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

#define PRINT_ERROR(arg) printf("ERROR: " arg "\n");
#define THRESHOLD 140

int main()
{

    // Read image into BMP struct
    BMP *bmp = bopen("input.bmp");

    // unsigned int x, y, width, height;
    // unsigned char r, g, b;

    // // Gets image width in pixels
    // width = get_width(bmp);

    // // Gets image height in pixels
    // height = get_height(bmp);

    // for (x = 0; x < width; x++)
    // {
    //     for (y = 0; y < height; y++)
    //     {
    //         // Gets pixel rgb values at point (x, y)
    //         get_pixel_rgb(bmp, x, y, &r, &g, &b);

    //         // Sets pixel rgb values at point (x, y)
    //         set_pixel_rgb(bmp, x, y, 255 - r, 255 - g, 255 - b);
    //     }
    // }

    // // Write bmp contents to file
    bwrite(bmp, "output.bmp");

    // // Free memory
    bclose(bmp);

    return 0;
}
