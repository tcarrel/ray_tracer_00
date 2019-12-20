


#include "epsilon.h"
#include "sphere.h"



#include<algorithm>
#include<cmath>
#include<glm\glm.hpp>



Sphere::Sphere( const glm::dvec3& center, double radius )
    :
    center_( center ),
    radius_( radius ),
    radius_squared_( radius * radius )
{}



Intersection Sphere::test_intersection( const Ray & ray )
{
    //Solve:
    //
    //   t²d∙d + 2*(o-p)∙d + (o-p)∙(o-p)-r² = 0

    constexpr auto HIT = true;
    constexpr auto MISS = false;

    const auto origin = ray.origin();
    const auto direction = ray.direction();

    auto op = center_ - origin;
    auto b = glm::dot( op, direction );
    auto det = b * b - glm::dot( op, op ) + radius_squared_;

    if( det < 0 )
    {
        return { MISS };
    }

    det = sqrt( det );

    auto neg_T = b - det;
    auto pos_T = b + det;
    if( neg_T < k_EPSILON && pos_T < k_EPSILON )
    {
        return { MISS };
    }

    auto t = neg_T > k_EPSILON ? neg_T : pos_T;
    auto hit_pos = ray.point_on_ray_at( t );
    auto normal = glm::normalize( hit_pos - center_ );
    bool inside = glm::dot( normal, direction ) > 0;
    if( inside )
    {
        normal = -normal;
    }


    return Intersection( HIT, ray.depth(), t, hit_pos /*+ ( 0.0001 * normal )*/, normal, {}, Primitive_Type::SPHERE );
}
