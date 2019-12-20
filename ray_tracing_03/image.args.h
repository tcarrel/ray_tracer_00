#pragma once



#include<string>
#include "global_constants.h"



struct Image_ctor_args
{
    int width = IMAGE_WIDTH_default;
    int height = IMAGE_HEIGHT_default;
    int rpp = RAYS_PER_PIXEL_default;
    int format = 0;
    int jpg_quality = 0;
    bool incremental_saves = false;
    bool overwrite = false;

    std::string filename = OUTPUT_FILENAME_default;
};

