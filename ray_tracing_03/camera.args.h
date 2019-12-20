#pragma once



#include<glm/glm.hpp>
#include "global_constants.h"



struct Camera_ctor_args
{
    glm::dvec3 eye = { 0, 0, 0 };
    glm::dvec3 lookat = { 0.5, 0.5, 1.0 };
    glm::dvec3 up = { 0, 1.0, 0 };
    double width = static_cast<double>( IMAGE_WIDTH_default );
    double height = static_cast<double>( IMAGE_WIDTH_default );
    double vert_FOV = VERTICAL_FOV_default;
};
