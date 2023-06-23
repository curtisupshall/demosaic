#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "bmpfunc.h"

#define PRINT_ERROR(arg) printf("ERROR: " arg "\n");
#define THRESHOLD 140

int main()
{

    BMP_Image *img = BMP_open("input.bmp");
    if (img == NULL)
    {
        PRINT_ERROR("Failed to open input.bmp");
        return EXIT_FAILURE;
    }

    printf("Image width: %d\n", img->width);
    printf("Image height: %d\n", img->height);
    printf("Image bytes per pixel: %d\n", img->bytes_per_pixel);
    printf("Image data size: %d\n", img->data_size);

    // BMP_invert(img);
    // BMP_color(img, 2);
    // BMP_color(img, 1);
    // BMP_color(img, 0);
    // BMP_edge(img, THRESHOLD);
    // BMP_gray(img);
    // BMP_equalize(img);
    // BMP_checker(img);
    // BMP_mix(img);

    // Save the file
    if (BMP_save(img, "output.bmp") == 0)
    {
        printf("Output file invalid!\n");
        BMP_destroy(img);
        return EXIT_FAILURE;
    }
    // Destroy the BMP image
    BMP_destroy(img);
    return EXIT_SUCCESS;
}
