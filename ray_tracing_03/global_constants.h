#pragma once



#include "glm_prints.h"



constexpr int IMAGE_WIDTH_default = 1920;
constexpr int IMAGE_HEIGHT_default = 1080;
constexpr int RAYS_PER_PIXEL_default = 10;
constexpr int MAX_DEPTH_default = 20;
constexpr auto OUTPUT_FILENAME_default = "OUTPUT";
constexpr double VERTICAL_FOV_default = 45.0;



constexpr long double PI = 3.14159265358979323846264338327950288419716939937510L;



constexpr auto OUTPUT_FILE_SUBFOLDER = "output\\";


#ifdef NDEBUG
constexpr bool DEBUGGING = false;
#else
constexpr bool DEBUGGING = true;
#endif
