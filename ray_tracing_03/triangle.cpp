


#include "triangle.h"
#include<cmath>



#include "epsilon.h"
#include "intersection.h"
#include "ray.h"

#include<iostream>


Triangle::Triangle( glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3 )
    :
    Triangle( p1, p2, p3, glm::normalize( glm::cross( p2 - p1, p3 - p1 ) ) )
{}



Triangle::Triangle( glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 normal )
    :
    p0_( p0 ),
    p1_( p1 ),
    p2_( p2 ),
    basis_p0_to_p1_( p1 - p0 ),
    basis_p0_to_p2_( p2 - p0 ),
    normal_( normal ),
    d_( glm::dot( normal, p0 ) ),
    p0_to_p1_( p1 - p0 ),
    p1_to_p2_( p2 - p1 ),
    p2_to_p0_( p0 - p2 )
{
    ctor_body();
}



Intersection Triangle::test_intersection( const Ray& ray )
{
    return algorithm_B( ray );
}



glm::dvec3 Triangle::normal() const noexcept
{
    return normal_;
}



glm::dvec3 Triangle::center()
{
    constexpr double one_half = 0.5;
    constexpr double two_thirds = 2.0 / 3.0;

    return p2_ + ( two_thirds * ( ( p0_ + ( one_half * ( p1_ - p0_ ) ) - p2_ ) ) );
}



Intersection Triangle::algorithm_A( const Ray& ray )
{
    using glm::dot;
    using glm::cross;

    constexpr bool MISS = false;
    constexpr bool HIT = !MISS;

    //cull degenerates
    //if( normal_ == glm::dvec3{ 0, 0, 0 } )
    //{
    //    return { MISS };
    //}

    auto dir = ray.direction();
    auto r_origin = ray.origin();


    auto b = dot( normal_, dir );
    //parallelism
    if( b < k_EPSILON )
    {
        return { MISS };
    }

    auto w0 = r_origin - p0_;
    auto a = -dot( normal_, w0 );

    auto r = a / b;
    // cull back facing ?
    //if( r < 0.0 && ray.is_primary )
    //{
    //    return { MISS };
    //}

    auto Intersection_Point = r_origin + ( r * dir );

    auto w = Intersection_Point - p0_;
    auto wu = dot( w, basis_p0_to_p1_ );
    auto wv = dot( w, basis_p0_to_p2_ );

    auto s = ( uv_ * wv - vv_ * wu ) * inverse_D_;
    if( ( s < 0.0 ) || ( 1.0 < s ) )
    {
        return { MISS };
    }

    auto t = ( uv_ * wu - uu_ * wv ) * inverse_D_;
    if( ( t < 0.0 ) || ( 1.0 < ( s + t ) ) )
    {
        return { MISS };
    }

    auto dvec = Intersection_Point - r_origin;
    auto I_dist = sqrt( dot( dvec, dvec ) );

    return Intersection(
        HIT,
        ray.depth(),
        I_dist,
        Intersection_Point - ( 0.0001 * r_origin ),
        normal_,
        {},
        Primitive_Type::TRIANGLE );


    //if( ( s >= 0.0 ) & ( t >= 0.0 ) & ( ( s + t ) < 1.0 ) )
    //{
    //    auto dvec = Intersection_Point - r_origin;
    //    auto I_dist = sqrt( dot( dvec, dvec ) );

    //    return Intersection( HIT, ray.depth(), I_dist, Intersection_Point, normal_ );
    //}

    //return { MISS };

}

Intersection Triangle::algorithm_B( const Ray& ray )
{
    using glm::dot;
    using glm::cross;

    auto r_origin = ray.origin();
    auto r_dir = ray.direction();

    auto n_dot_raydir = dot( normal_, r_dir );
    if( n_dot_raydir < k_EPSILON )
    {
        return { false };
    }

    auto d = dot( normal_, p0_ );

    auto t = ( dot( normal_, r_origin ) + d ) / n_dot_raydir;

    if( t < 0.0 )
    {
        return { false };
    }

    auto P = r_origin + t * r_dir;

    auto vp0 = P - p0_;
    auto C = cross( p0_to_p1_, vp0 );
    if( dot( normal_, C ) < k_EPSILON )
    {
        return { false };
    }

    auto vp1 = P - p1_;
    C = cross( p1_to_p2_, vp1 );
    if( dot( normal_, C ) < k_EPSILON )
    {
        return { false };
    }

    auto vp2 = P - p2_;
    C = cross( p2_to_p0_, vp2 );
    if( dot( normal_, C ) < k_EPSILON )
    {
        return { false };
    }

    auto P_vec = P - r_origin;
    auto P_dist = sqrt( dot( P_vec, P_vec ) );

    return Intersection(
        true,
        ray.depth(),
        P_dist,
        P,
        normal_,
        {},
        Primitive_Type::TRIANGLE
    );
}

void Triangle::ctor_body( void )
{
    uu_ = glm::dot( basis_p0_to_p1_, basis_p0_to_p1_ );
    uv_ = glm::dot( basis_p0_to_p1_, basis_p0_to_p2_ );
    vv_ = glm::dot( basis_p0_to_p2_, basis_p0_to_p2_ );
    inverse_D_ = 1 / ( uv_ * uv_ - uu_ * vv_ );
}
