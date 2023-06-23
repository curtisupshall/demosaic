#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpimage.h"

#define HIST_WIDTH 256
#define HALF_WIDTH (HIST_WIDTH / 2)
#define QUARTER_WIDTH (HIST_WIDTH / 4)

void BMP_invert(BMP_Image *img)
{
    int pxl;
    for (pxl = 0; pxl < (img->data_size); pxl++)
    {
        img->data[pxl] = 255 - (img->data[pxl]);
    }
}

void BMP_color(BMP_Image *img, int clr)
{
    int pxl;
    for (pxl = clr; pxl < (img->data_size); pxl++)
    {
        // set the other color components to zero
        if ((pxl % 3) != clr)
        {
            img->data[pxl] = 0;
        }
    }
}

static int RGB2Gray(char red, char green, char blue)
{
    // this is a commonly used formula
    double gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
    return (int)gray;
}

void BMP_edge(BMP_Image *img, int thrshd)
{
    // create a two-dimension array for the gray level
    int width = img->width;
    int height = img->height;
    char **twoDGray = malloc(sizeof(char *) * height);
    int row;
    int col;
    for (row = 0; row < height; row++)
    {
        twoDGray[row] = malloc(sizeof(char *) * width);
    }
    // convert RGB to gray
    int pxl = 0;
    for (row = 0; row < height; row++)
    {
        for (col = 0; col < width; col++)
        {
            twoDGray[row][col] = RGB2Gray(img->data[pxl + 2],
                                          img->data[pxl + 1],
                                          img->data[pxl]);
            pxl += 3;
        }
    }
    // detect edges and save the edges in the image
    pxl = 0;
    for (row = 0; row < height; row++)
    {
        pxl += 3; // skip the first pixel in each row
        for (col = 1; col < width; col++)
        {
            int diff = twoDGray[row][col] -
                       twoDGray[row][col - 1];
            // take the absolute value
            if (diff < 0)
            {
                diff = -diff;
            }
            if (diff > thrshd) // an edge
            {
                // set color to white
                img->data[pxl + 2] = 255;
                img->data[pxl + 1] = 255;
                img->data[pxl] = 255;
            }
            else // not an edge
            {
                // set color to black
                img->data[pxl + 2] = 0;
                img->data[pxl + 1] = 0;
                img->data[pxl] = 0;
            }
            pxl += 3;
        }
    }
    for (row = 0; row < height; row++)
    {
        free(twoDGray[row]);
    }
    free(twoDGray);
}

void BMP_gray(BMP_Image *img)
{
    int pxl;
    for (pxl = 0; pxl < (img->data_size); pxl += 3)
    {
        unsigned char gray = RGB2Gray(img->data[pxl + 2],
                                      img->data[pxl + 1],
                                      img->data[pxl]);
        img->data[pxl + 2] = gray;
        img->data[pxl + 1] = gray;
        img->data[pxl] = gray;
    }
}

// void BMP_equalize(BMP_Image *img)
// {
//     int pxl;
//     unsigned char redmin = 255;
//     unsigned char redmax = 0;
//     unsigned char greenmin = 255;
//     unsigned char greenmax = 0;
//     unsigned char bluemin = 255;
//     unsigned char bluemax = 0;
//     // find the maximum and the minimum values of each color
//     for (pxl = 0; pxl < (img->data_size); pxl += 3)
//     {
//         unsigned char red = img->data[pxl + 2];
//         unsigned char green = img->data[pxl + 1];
//         unsigned char blue = img->data[pxl];
//         if (redmin > red)
//         {
//             redmin = red;
//         }
//         if (redmax < red)
//         {
//             redmax = red;
//         }
//         if (greenmin > green)
//         {
//             greenmin = green;
//         }
//         if (greenmax < green)
//         {
//             greenmax = green;
//         }
//         if (bluemin > blue)
//         {
//             bluemin = blue;
//         }
//         if (bluemax < blue)
//         {
//             bluemax = blue;
//         }
//     }
//     // calculate the scaling factors
//     // max and min must be different to prevent
//     // divided by zero error
//     double redscale = 1.0;
//     double greenscale = 1.0;
//     double bluescale = 1.0;
//     if (redmax > redmin)
//     {
//         redscale = 255.0 / (redmax - redmin);
//     }
//     if (greenmax > greenmin)
//     {
//         greenscale = 255.0 / (greenmax - greenmin);
//     }
//     if (bluemax > bluemin)
//     {
//         bluescale = 255.0 / (bluemax - bluemin);
//     }

//     // equalize the pixels
//     for (pxl = 0; pxl < (img->data_size); pxl += 3)
//     {
//         if (redmax > redmin)
//         {
//             img->data[pxl + 2] = (int)(redscale *
//                                        (img->data[pxl + 2] - redmin));
//         }
//         if (greenmax > greenmin)
//         {
//             img->data[pxl + 1] = (int)(greenscale *
//                                        (img->data[pxl + 1] - greenmin));
//         }
//         if (bluemax > bluemin)
//         {
//             img->data[pxl] = (int)(bluescale *
//                                    (img->data[pxl] - bluemin));
//         }
//     }
// }

// void BMP_checker(BMP_Image *img)
// {
//     // resize the image
//     img->width = HIST_WIDTH;
//     img->height = HIST_WIDTH;
//     img->data_size =
//         HIST_WIDTH * HIST_WIDTH * (img->bytes_per_pixel);
//     (img->header).width = HIST_WIDTH;
//     (img->header).height = HIST_WIDTH;
//     (img->header).size =
//         img->data_size + sizeof(BMP_Header);
//     (img->header).imagesize = img->data_size;

//     // release the memory storing the data
//     free(img->data);

//     // allocate memory for the correct size
//     img->data =
//         malloc(sizeof(unsigned char) * (img->data_size));
//     if ((img->data) == NULL)
//     {
//         return;
//     }

//     // set all pixels to black
//     bzero(img->data,
//           sizeof(unsigned char) * (img->data_size));
//     int row;
//     int col;
//     int ind;
//     // lower left corner is white
//     int brightness;
//     for (row = 0; row < HALF_WIDTH; row++)
//     {
//         for (col = 0; col < HALF_WIDTH; col++)
//         {
//             ind = 3 * (row * HIST_WIDTH + col);
//             brightness =
//                 row / QUARTER_WIDTH + col / QUARTER_WIDTH;
//             brightness = 64 * brightness + 127;
//             img->data[ind] = brightness;
//             img->data[ind + 1] = brightness;
//             img->data[ind + 2] = brightness;
//         }
//     }
//     // lower right corner is red
//     for (row = 0; row < HALF_WIDTH; row++)
//     {
//         for (col = HALF_WIDTH; col < HIST_WIDTH; col++)
//         {
//             brightness =
//                 row / QUARTER_WIDTH + col / QUARTER_WIDTH;
//             brightness = 64 * brightness + 63;
//             ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind + 2] = brightness;
//         }
//     }
//     // upper left corner is green
//     for (row = HALF_WIDTH; row < HIST_WIDTH; row++)
//     {
//         for (col = 0; col < HALF_WIDTH; col++)
//         {
//             brightness =
//                 row / QUARTER_WIDTH + col / QUARTER_WIDTH;
//             brightness = 64 * brightness + 63;
//             ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind + 1] = brightness;
//         }
//     }
//     // upper right corner is red
//     for (row = HALF_WIDTH; row < HIST_WIDTH; row++)
//     {
//         for (col = HALF_WIDTH; col < HIST_WIDTH; col++)
//         {
//             brightness =
//                 row / QUARTER_WIDTH + col / QUARTER_WIDTH;
//             brightness = 64 * (brightness - 4) + 127;
//             ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind] = brightness;
//         }
//     }
// }

// void BMP_mix(BMP_Image *img)
// {
//     // resize the image
//     img->width = HIST_WIDTH;
//     img->height = HIST_WIDTH;
//     img->data_size =
//         HIST_WIDTH * HIST_WIDTH * (img->bytes_per_pixel);
//     (img->header).width = HIST_WIDTH;
//     (img->header).height = HIST_WIDTH;
//     (img->header).size =
//         img->data_size + sizeof(BMP_Header);
//     (img->header).imagesize = img->data_size;

//     // release the memory storing the data
//     free(img->data);

//     // allocate memory for the correct size
//     img->data =
//         malloc(sizeof(unsigned char) * (img->data_size));
//     if ((img->data) == NULL)
//     {
//         return;
//     }

//     // set all pixels to black
//     bzero(img->data,
//           sizeof(unsigned char) * (img->data_size));

//     // draw red
//     int centerx = 127;
//     int centery = 80;
//     int radius = 70;
//     int row;
//     int col;
//     for (col = centerx - radius; col < centerx + radius;
//          col++)
//     {
//         int xval = col - centerx;
//         int yrange =
//             (int)sqrt(radius * radius - xval * xval);
//         for (row = centery - yrange; row < centery + yrange;
//              row++)
//         {
//             int ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind + 2] = 255;
//         }
//     }

//     // draw green
//     centerx = 87;
//     centery = 167;
//     for (col = centerx - radius; col < centerx + radius;
//          col++)
//     {
//         int xval = col - centerx;
//         int yrange =
//             (int)sqrt(radius * radius - xval * xval);
//         for (row = centery - yrange; row < centery + yrange;
//              row++)
//         {
//             int ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind + 1] = 255;
//         }
//     }

//     // draw blue
//     centerx = 167;
//     centery = 167;
//     for (col = centerx - radius; col < centerx + radius;
//          col++)
//     {
//         int xval = col - centerx;
//         int yrange =
//             (int)sqrt(radius * radius - xval * xval);
//         for (row = centery - yrange; row < centery + yrange;
//              row++)
//         {
//             int ind = 3 * (row * HIST_WIDTH + col);
//             img->data[ind] = 255;
//         }
//     }
// }