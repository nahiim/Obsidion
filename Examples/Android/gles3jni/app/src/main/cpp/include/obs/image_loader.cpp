
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* loadImage(char const *filename, int *width, int *height,  int *channels_count)
{
    return stbi_load(filename, width, height, channels_count, 0);
}