#include "point_light.h"



Point_Light::Point_Light( const glm::dvec3& position, const glm::dvec3& color, double brightness )
    :
    position_( position )
{
    color_ = brightness * color;
}







glm::dvec3 Point_Light::position( void )
{
    return position_;
}

glm::dvec3 Point_Light::color( void )
{
    return color_;
}
