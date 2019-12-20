#pragma once



#include<glm\glm.hpp>
#include "intersection.h"
#include "ray.h"



class Sphere
{
public:

    Sphere( const glm::dvec3& center, double radius );
    
    Intersection test_intersection( const Ray& ray );

private:

    glm::dvec3 center_;
    double radius_;
    double radius_squared_;
};
