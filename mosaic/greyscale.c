#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    unsigned char b, g, r;
} Pixel;
#pragma pack(pop)

void convertToGrayscale(const Pixel* colorPixels, Pixel* grayscalePixels, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            unsigned char grayValue = (colorPixels[index].r + colorPixels[index].g + colorPixels[index].b) / 3;
            grayscalePixels[index].r = grayValue;
            grayscalePixels[index].g = grayValue;
            grayscalePixels[index].b = grayValue;
        }
    }
}

int main() {
    const char* inputFileName = "input.bmp";
    const char* outputFileName = "output.bmp";

    FILE* inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        printf("Error: Failed to open the input file.\n");
        return 1;
    }

    // Read the bitmap header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, inputFile);

    // Extract image dimensions
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // Calculate image size in bytes
    int imageSize = width * height * sizeof(Pixel);

    // Allocate memory for pixel data
    Pixel* colorPixels = (Pixel*)malloc(imageSize);
    Pixel* grayscalePixels = (Pixel*)malloc(imageSize);

    // Read the pixel data
    fread(colorPixels, sizeof(unsigned char), imageSize, inputFile);
    fclose(inputFile);

    // Convert to grayscale
    convertToGrayscale(colorPixels, grayscalePixels, width, height);

    // Write the grayscale pixel data to the output file
    FILE* outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        printf("Error: Failed to open the output file.\n");
        free(colorPixels);
        free(grayscalePixels);
        return 1;
    }

    // Write the bitmap header
    fwrite(header, sizeof(unsigned char), 54, outputFile);

    // Write the grayscale pixel data
    fwrite(grayscalePixels, sizeof(unsigned char), imageSize, outputFile);
    fclose(outputFile);

    // Cleanup
    free(colorPixels);
    free(grayscalePixels);

    printf("Conversion complete. Output file: %s\n", outputFileName);

    return 0;
}