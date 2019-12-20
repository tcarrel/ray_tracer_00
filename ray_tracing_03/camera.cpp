#include "camera.h"
#include "global_constants.h"
#include "helpers.h"
#include "random_wrapper.h"




#include<cmath>



Camera::Camera( const Camera_ctor_args& args )
    :
    center_( args.eye ),
    axes_( orthonormal_basis_from_ZY( glm::normalize( args.lookat - args.up ), args.up ) ),
    aspect_ratio_( args.width / args.height ),
    plane_distance_( 1.0 / tan( args.vert_FOV * PI / 360.0 ) ),
    reciprocal_height_( 1.0 / args.height ),
    reciprocal_width_( 1.0 / args.width )
{}




Camera::Camera( glm::dvec3 eye, glm::dvec3 lookat, glm::dvec3 up, double width, double height, double vert_FOV )
    :
    center_( eye ),
    axes_( orthonormal_basis_from_ZY( glm::normalize( lookat - up ), up ) ),
    aspect_ratio_( width / height ), 
    plane_distance_( 1.0 / tan( vert_FOV * PI / 360.0 ) ),
    reciprocal_height_( 1.0 / height ),
    reciprocal_width_( 1.0 / width )
{}

Camera::Camera( const Camera & copy_from )
{
    *this = copy_from;
}

Camera& Camera::operator==( const Camera& copy_from )
{
    this->center_ = copy_from.center_;
    this->axes_ = copy_from.axes_;
    this->aspect_ratio_ = copy_from.aspect_ratio_;
    this->plane_distance_ = copy_from.plane_distance_;
    this->reciprocal_height_ = copy_from.reciprocal_height_;
    this->reciprocal_width_ = copy_from.reciprocal_width_;

    return *this;
}



Ray Camera::make_ray( int pixel_x, int pixel_y, bool is_first_pass )
{
    double x_offset = 0.5L, y_offset = 0.5L;
    
    if(!is_first_pass )
    {
        auto rng = RNG::Rand<double>( 0, 1 );

        x_offset = rng.value();
        y_offset = rng.value();
    }

    auto x = ( pixel_x + x_offset ) * reciprocal_width_;
    auto y = ( pixel_y + y_offset ) * reciprocal_height_;

    constexpr glm::dvec3 x_axis{ 1.0, 0.0, 0.0 };
    constexpr glm::dvec3 y_axis{ 0.0, 1.0, 0.0 };
    constexpr glm::dvec3 z_axis{ 0.0, 0.0, 1.0 };

    auto x_contribution = x_axis * ( -x ) * aspect_ratio_;
    auto y_contribution = y_axis * ( -y );
    auto z_contribution = z_axis * plane_distance_;

    auto direction = glm::normalize( x_contribution + y_contribution + z_contribution );

    return Ray( center_, direction );
}
