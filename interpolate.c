#include <stdint.h>
#include <math.h>

void bilinear_interpolation(float *data, uint32_t input_width,
                            uint32_t input_height, uint32_t output_width,
                            uint32_t output_height, float *output)
{
    float x_ratio, y_ratio;

    if (output_width > 1)
    {
        x_ratio = ((float)input_width - 1.0) / ((float)output_width - 1.0);
    }
    else
    {
        x_ratio = 0;
    }

    if (output_height > 1)
    {
        y_ratio = ((float)input_height - 1.0) / ((float)output_height - 1.0);
    }
    else
    {
        y_ratio = 0;
    }

    for (int i = 0; i < output_height; i++)
    {
        for (int j = 0; j < output_width; j++)
        {
            float x_l = floor(x_ratio * (float)j);
            float y_l = floor(y_ratio * (float)i);
            float x_h = ceil(x_ratio * (float)j);
            float y_h = ceil(y_ratio * (float)i);

            float x_weight = (x_ratio * (float)j) - x_l;
            float y_weight = (y_ratio * (float)i) - y_l;

            float a = data[(int)y_l * input_width + (int)x_l];
            float b = data[(int)y_l * input_width + (int)x_h];
            float c = data[(int)y_h * input_width + (int)x_l];
            float d = data[(int)y_h * input_width + (int)x_h];

            float pixel = a * (1.0 - x_weight) * (1.0 - y_weight) +
                          b * x_weight * (1.0 - y_weight) +
                          c * y_weight * (1.0 - x_weight) +
                          d * x_weight * y_weight;

            output[i * output_width + j] = pixel;
        }
    }
}

