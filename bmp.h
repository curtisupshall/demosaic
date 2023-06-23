
#ifndef BMP_H
#define BMP_H
#include <stdint.h>
#include <stdbool.h>
#include "bmpimage.h"

// load a BMP_Image from a file
BMP_Image *BMP_open(const char *filename);

// save a BMP_Image to a file
int BMP_save(const BMP_Image *image, const char *filename);

// free all memory associated with a BMP_Image
void BMP_destroy(BMP_Image *image);

#endif
