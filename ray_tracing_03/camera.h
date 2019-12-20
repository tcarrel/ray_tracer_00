#pragma once



#include<glm\glm.hpp>
#include "ray.h"
#include "global_constants.h"



#include "camera.args.h"



class Camera
{
public:

    Camera( const Camera_ctor_args& args );
    Camera( glm::dvec3 eye, glm::dvec3 lookat, glm::dvec3 up, double width, double height, double vert_FOV );
    //Camera( double image_world_height, double dist_to_cam, int width_in_pixels, int height_in_pixels );
    Camera( const Camera& copy_from );

    Camera& operator==( const Camera& copy_from );

    Ray make_ray( int pixel_x, int pixel_y, bool is_first_pass );

private:

    glm::dvec3 center_;
    glm::mat3 axes_;

    double aspect_ratio_;
    double plane_distance_;
    double reciprocal_width_;
    double reciprocal_height_;
};

