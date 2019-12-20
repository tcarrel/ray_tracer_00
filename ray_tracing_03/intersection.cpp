#include "intersection.h"



Intersection::Intersection(
    bool hit,
    size_t depth,
    double dist,
    glm::dvec3 pos,
    glm::dvec3 norm,
    glm::dvec3 found_color,
    Primitive_Type tp )
    :
    is_a_hit( hit ),
    distance_( dist ),
    position_( pos ),
    normal_( norm ),
    depth_( depth ),
    color_found_( found_color ),
    type( tp )
{}



size_t Intersection::depth( void )
{
    return depth_;
}



size_t Intersection::new_depth( void )
{
    return depth_ + 1;
}



double Intersection::distance( void )
{
    return distance_;
}



glm::dvec3 Intersection::position( void )
{
    return position_;
}



glm::dvec3 Intersection::normal( void )
{
    return normal_;
}



Intersection Intersection::closer_of( Intersection A, Intersection B ) noexcept
{
    if( !A.is_a_hit && !B.is_a_hit )
    {
        return { false };
    }

    if( A.is_a_hit && !B.is_a_hit )
    {
        return A;
    }

    if( !A.is_a_hit && B.is_a_hit )
    {
        return B;
    }

    return ( A < B ) ? A : B;
}


// Operators are in file: intersection.operators.cpp
