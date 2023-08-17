int main()
{
    int IMAGE_WIDTH = 4000;
    int IMAGE_HEIGHT = 6000;
    int rowSize = ((IMAGE_WIDTH * 3 + 3) & ~3) / 4;

    printf("Rowsize = %d\n", rowSize);

}
