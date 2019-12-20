


#include "ray.h"



#include<glm\glm.hpp>



size_t Ray::quantity_created_{ 0 };



Ray::Ray( glm::dvec3 origin, glm::dvec3 direction, size_t depth, bool is_primary_ray )
    :
    origin_( origin ),
    direction_( glm::normalize( direction ) ),
    depth_( depth ),
    is_primary( is_primary_ray )
{
    ++quantity_created_;
}



const glm::dvec3& Ray::origin( void ) const noexcept
{
    return origin_;
}



const glm::dvec3& Ray::direction( void ) const noexcept
{
    return direction_;
}



const glm::dvec3 Ray::point_on_ray_at( double t ) const noexcept
{
    return origin_ + ( t * direction_ );
}



size_t Ray::depth( void ) const noexcept
{
    return depth_;
}

size_t Ray::new_depth( void ) const noexcept
{
    return depth_ + 1;
}



void Ray::save_point_light_color( glm::dvec3 pl_color )
{
    pl_color_ = pl_color;
}



std::optional<glm::dvec3>& Ray::point_light_color( void )
{
    return pl_color_;
}



size_t Ray::count( void ) noexcept
{
    return quantity_created_;
}


// Operators are in file: ray.operators.cpp
