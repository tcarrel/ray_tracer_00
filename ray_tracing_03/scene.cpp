#include "epsilon.h"
#include "scene.h"
#include "global_constants.h"

#include<numeric>

#include<iostream>



Scene::Scene( int max_d ) :
    max_depth_( max_d )
{}



glm::dvec3 Scene::cast_ray( Ray& ray )
{
    if( ray.depth() >= max_depth_ )
    {
        return ambient_color_;
    }

    return ray.is_primary ? cast_primary_ray( ray ) : cast_secondary_ray( ray );

    //auto intersection = Intersection::closer_of(
    //    find_first_sphere_intersection( ray ),
    //    find_first_triangle_intersection( ray ) );

    //if( !intersection.is_a_hit )
    //{
    //    //return is_primary ? bg_color_ : ambient_color_;
    //    if( is_primary )
    //    {
    //        return bg_color_;
    //    }

    //    auto pl_col = ray.point_light_color();

    //    if( pl_col.has_value() )
    //    {
    //        return pl_col.value();
    //    }

    //    return ambient_color_;
    //}

    //auto secondaries = generate_secondary_rays( intersection.position(), ray.depth() );
    //glm::dvec3 color( 0, 0, 0 );

    //if( secondaries.empty() )
    //{
    //    return ambient_color_;
    //}

    //for( auto sray : secondaries )
    //{
    //    color += cast_ray( sray, sray.is_primary );
    //}

    //return ( 1.0 / static_cast<double>( secondaries.size() ) ) * color;
}



glm::dvec3 Scene::generate_secondary_rays( Intersection& intersection )
{

    std::vector<glm::dvec3> colors;
    for( auto light : plights_ )
    {
        auto origin = intersection.position();
        auto target = light.position();

        auto direction = target - origin;

        if( glm::dot( intersection.normal(), direction ) < k_EPSILON )
        {
            //Normal points away-ish from light.
            Ray ray( origin, direction, intersection.new_depth(), false );
            colors.push_back( cast_ray( ray ) );
        }
        else
        {
            Ray ray( origin, direction, intersection.new_depth(), false );
            colors.push_back( cast_diffuse_light_ray(
                ray,
                light,
                glm::dot( intersection.normal(), glm::normalize( direction ) ) ) );
        }
    }

    auto sum = std::accumulate( colors.begin(), colors.end(), glm::dvec3( 0.0, 0.0, 0.0 ) );

    return ( 1.0 / static_cast<double>( colors.size() ) ) * sum;
}



Point_Light& Scene::add_point_light( const glm::dvec3& position, const glm::dvec3& color, double brightness )
{
    if constexpr( DEBUGGING )
    {
        std::cout << "Adding point light at:\t" << position << ", color\t" << color << '\n';
    }

    return plights_.emplace_back( position, color, brightness );
//    spheres_.emplace_back( position, brightness );
}

Sphere& Scene::add_sphere( const glm::dvec3& center, double radius )
{
    if constexpr( DEBUGGING )
    {
        std::cout << "Adding sphere at:\t" << center << " with radius " << radius << '\n';
    }

    return spheres_.emplace_back( center, radius );
}



Triangle& Scene::add_triangle( Triangle tri )
{
    triangles_.push_back( tri );
    return triangles_.back();
}



Triangle& Scene::add_triangle( glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2 )
{
    return triangles_.emplace_back( p0, p1, p2 );
}



Triangle& Scene::add_triangle( glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 normal )
{
    return triangles_.emplace_back( p0, p1, p2, normal );
}



glm::dvec3 Scene::ambient_light( void )
{
    return ambient_color_;
}



void Scene::set_ambient( uint8_t r, uint8_t g, uint8_t b )
{
    set_ambient( static_cast<double>( r ) / 255.0, static_cast<double>( g ) / 255.0, static_cast<double>( b ) / 255.0 );
}

void Scene::set_ambient( double r, double g, double b )
{
    if( r < 0.0 ) r = 0.0;
    if( g < 0.0 ) g = 0.0;
    if( b < 0.0 ) b = 0.0;
    if( r > 1.0 ) r = 1.0;
    if( g > 1.0 ) g = 1.0;
    if( b > 1.0 ) b = 1.0;

    ambient_color_.r = r;
    ambient_color_.g = g;
    ambient_color_.b = b;
}

void Scene::set_bg( uint8_t r, uint8_t g, uint8_t b )
{
    set_bg( static_cast<double>( r ) / 255.0, static_cast<double>( g ) / 255.0, static_cast<double>( b ) / 255.0 );
}

void Scene::set_bg( double r, double g, double b )
{
    if( r < 0.0 ) r = 0.0;
    if( g < 0.0 ) g = 0.0;
    if( b < 0.0 ) b = 0.0;
    if( r > 1.0 ) r = 1.0;
    if( g > 1.0 ) g = 1.0;
    if( b > 1.0 ) b = 1.0;

    bg_color_.r = r;
    bg_color_.g = g;
    bg_color_.b = b;
}



Intersection Scene::find_first_sphere_intersection( Ray& ray )
{
    Intersection closest( false );

    for( auto sphere : spheres_ )
    {
        auto ix = sphere.test_intersection( ray );

        if( (!closest.is_a_hit && ix.is_a_hit))
        {
            closest = ix;
            continue;
        }

        if( ix.is_a_hit && ( ix < closest ) )
        {
            closest = ix;
        }
    }

    return closest;
}



Intersection Scene::find_first_triangle_intersection( Ray& ray )
{
    Intersection closest( false );

    for( auto tri : triangles_ )
    {
        auto ix = tri.test_intersection( ray );

        if( ( !closest.is_a_hit && ix.is_a_hit ) || ( ix < closest ) )
        {
            closest = ix;
        }
    }

    return closest;
}



std::vector<Ray> Scene::generate_secondary_rays( size_t current_depth, Intersection X )
{
    std::vector<Ray> pl_rays;

    size_t new_depth = current_depth + 1;

    auto origin = X.position();
    auto normal = X.normal();

    //Logic based on material.

    for( auto pl : plights_ )
    {
        auto target = pl.position();
        auto direction = glm::normalize( target - origin );

        switch( X.type )
        {
        case Primitive_Type::TRIANGLE:
            origin += 0.0001 * direction;
            break;
        default:
            ;
        }


        if( glm::dot( origin, direction ) >= k_EPSILON )
        {
            pl_rays.emplace_back( origin, direction, new_depth, false );
            pl_rays.back().save_point_light_color( pl.color() );
        }
    }

    return pl_rays;
}



glm::dvec3 Scene::cast_primary_ray( Ray ray )
{
    auto ix = cast( ray );

    if( !ix.is_a_hit )
    {
        return bg_color_;
    }

    auto secondaries = generate_secondary_rays( ray.depth(), ix );

    if( secondaries.empty() )
    {
        return ambient_color_;
    }

    glm::dvec3 color = ambient_color_;

    for( auto ray2 : secondaries )
    {
        auto ray_color = cast_ray( ray2 );

        color += glm::dot( glm::normalize( ray2.direction() ), ix.normal() ) * ray_color;
    }

    return ( 1.0 / static_cast<double>( secondaries.size() + 1 ) )* color;
}

bool Scene::precast_secondary_ray( Intersection ix, Ray ray )
{
    return false;
}



glm::dvec3 Scene::cast_secondary_ray( Ray ray )
{
    if( ray.depth() >= max_depth_ )
    {
        return ambient_color_;
    }

    auto ix = cast( ray );

    if( !ix.is_a_hit )
    {
        auto color = ray.point_light_color();
        if( color.has_value() )
        {
            return color.value();
        }

        return ambient_color_;
    }

    auto color = ambient_color_;
    auto more_rays = generate_secondary_rays( ray.depth(), ix );
    auto qty = static_cast<double>( more_rays.size() + 1UL );

    for( auto ray2 : more_rays )
    {
        color += cast_secondary_ray( ray2 );
    }

    return ( 1.0 / qty ) * color;

    return ambient_color_;
}



Intersection Scene::cast( Ray& ray )
{
    return Intersection::closer_of(
        find_first_sphere_intersection( ray ),
        find_first_triangle_intersection( ray ) );
}



glm::dvec3 Scene::cast_diffuse_light_ray( Ray& ray, Point_Light& light, double l_dot_n )
{
    if( ray.depth() >= max_depth_ )
    {
        return ambient_color_;
    }

    Intersection ix = cast( ray );

    if( ix.is_a_hit )
    {
        return generate_secondary_rays( ix );
    }
    //for( auto sphere : spheres_ )
    //{
    //    sph_ix = sphere.test_intersection( ray );
    //}

    //Intersection tri_ix( false );
    //for( auto triangle : triangles_ )
    //{
    //    tri_ix = triangle.test_intersection( ray );
    //}

    //if( sph_ix.is_a_hit && !tri_ix.is_a_hit )
    //{
    //    return generate_secondary_rays( sph_ix );
    //}

    //if( !sph_ix.is_a_hit && tri_ix.is_a_hit )
    //{
    //    return generate_secondary_rays( tri_ix );
    //}

    //if( sph_ix.is_a_hit && tri_ix.is_a_hit )
    //{
    //    return generate_secondary_rays( ( sph_ix <= tri_ix ) ? sph_ix : tri_ix );
    //}

    return l_dot_n * light.color();
}
