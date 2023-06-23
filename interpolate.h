#include <stdint.h>
#ifndef INTERPOLATE_H
#define INTERPOLATE_H


void bilinear_interpolation(float *data, uint32_t input_width,
                            uint32_t input_height, uint32_t output_width,
                            uint32_t output_height, float *output);

#endif