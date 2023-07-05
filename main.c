#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

#define PRINT_ERROR(arg) printf("ERROR: " arg "\n");
#define THRESHOLD 140

#define input_file "input.bmp"
#define output_file "output.bmp"
// https://github.com/ewpratten/smalltools/blob/master/bmpinfo/main.c#L45
// Alot of this is based with uint32, but starting with this and try to convert to uint8
int main()
{

    FILE *fp;
    fp = fopen(input_file, "rb");
    if (!fp)
    {
        perror("Unable to open file");
    }

    // Get data length
    uint32_t data_length = getBitmapDataLength(fp);
    printf("data length: %d\n", data_length);
    fseek(fp, 0, SEEK_SET);

    // Create file header and info data
    BMPHeader header;
    BMPInfo info;
    RGB8Bit pixels;

    fread(&header, sizeof_BMPHeader, 1, fp);
    printf("header size: %d\n", header.size);
    printf("header data_offset: %d\n", header.data_offset);
    printf("header reserved: %d\n", header.reserved);
    printf("header signature: %s\n", header.signature);

    fread(&info, sizeof_BMPInfo, 1, fp);
    printf("info size: %d\n", info.size);
    printf("info width: %d\n", info.width);
    printf("info height: %d\n", info.height);
    printf("info color_planes: %d\n", info.color_planes);
    printf("info color_depth: %d\n", info.color_depth);
    printf("info compression: %d\n", info.compression);
    printf("info raw_size: %d\n", info.raw_size);
    printf("info horizontal_ppm: %d\n", info.horizontal_ppm);
    printf("info vertical_ppm: %d\n", info.vertical_ppm);
    printf("info color_table: %d\n", info.color_table);
    printf("info important_colors: %d\n", info.important_colors);

    // readBitmap(&header, &info, &pixels, data_length, fp);
    // readInMetadata(&header, &info, fp);
    //BELOOW THIS CAUSES A SEGMENATION FAULT FOR SOME REASON
    // fread(&pixels, sizeof(RGB8Bit), data_length, fp);

    _validate_file_type(header.signature);
    printf("file type validated %s\n", header.signature);

    _validate_depth(info.color_depth);
    printf("depth validated\n");

    // fread(&pixels, sizeof(BGR8Bit), data_length, fp);

    // Read image into BMP struct
    // BMP *bmp = bopen(fp, data_length);

    FILE *op;
    op = fopen(output_file, "wb");
    if (!op)
    {
        perror("Unable to open file");
    }

    // Write bmp contents to file
    // bwrite(&header, &info, bmp->pixels, data_length, op);

    // // // Free memory
    // bclose(bmp);

    return 0;
}
