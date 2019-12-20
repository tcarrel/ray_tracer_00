#pragma once



#include<glm\glm.hpp>
#include "global_constants.h"



struct Trace_args
{
    int rpp = RAYS_PER_PIXEL_default;
    int max_depth = MAX_DEPTH_default;
    glm::dvec3 miss_color = { 0, 0, 0 };

    size_t width = IMAGE_WIDTH_default;
    size_t height = IMAGE_HEIGHT_default;
};
