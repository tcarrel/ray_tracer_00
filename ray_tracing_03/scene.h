#pragma once



#include "intersection.h"
#include "point_light.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"



#include<glm\glm.hpp>
#include<vector>




class Scene
{
public:
    Scene( int max_d );

    glm::dvec3 cast_ray( Ray& ray );


    Point_Light& add_point_light( const glm::dvec3& position, const glm::dvec3& color, double brightness );

    Sphere& add_sphere( const glm::dvec3& center, double radius );
    Triangle& add_triangle( Triangle tri );
    Triangle& add_triangle( glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2 );
    Triangle& add_triangle( glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 normal );

    glm::dvec3 ambient_light( void );

    void set_ambient( uint8_t r, uint8_t g, uint8_t b );
    void set_ambient( double r, double g, double b );

    void set_bg( uint8_t r, uint8_t g, uint8_t b );
    void set_bg( double r, double g, double b );

    Intersection find_first_sphere_intersection( Ray& ray );
    Intersection find_first_triangle_intersection( Ray& ray );

    std::vector<Ray> generate_secondary_rays( size_t current_depth, Intersection X );

private:

    glm::dvec3 cast_primary_ray( Ray ray );
    bool precast_secondary_ray( Intersection ix, Ray ray );
    glm::dvec3 cast_secondary_ray( Ray ray );

    Intersection cast( Ray& ray );
    glm::dvec3 cast_diffuse_light_ray( Ray& ray, Point_Light& light, double l_dot_n );
    glm::dvec3 generate_secondary_rays( Intersection& intersection );

    int max_depth_{};

    std::vector<Sphere> spheres_;
    std::vector<Triangle> triangles_;
    std::vector<Point_Light> plights_;

    glm::dvec3 ambient_color_{ 0.1, 0.1, 0.1 };
    glm::dvec3 bg_color_{ 0.1, 0.2, 0.8 };
};
