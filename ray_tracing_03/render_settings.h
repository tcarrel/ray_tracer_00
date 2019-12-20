#pragma once


#include<string>

#include "global_constants.h"



struct Render_Settings
{
    int rpp = RAYS_PER_PIXEL_default; // Rays Per Pixel.
    int max_depth = MAX_DEPTH_default;
    bool incremental_save = false;
    struct
    {
        int width = IMAGE_WIDTH_default; // Image width, pixels.
        int height = IMAGE_HEIGHT_default; // Image height, pixels.
        std::string filename = OUTPUT_FILENAME_default;
        bool overwrite = false;
        int format = 0;
        int jpg_quality = 0;
    } image;
    struct
    {
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;
    } miss_color;
};
