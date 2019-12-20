#include "glm_prints.h"
#include<iostream>



std::ostream& operator<<( std::ostream& os, const glm::dvec3& vec )
{
    return os << '{' << vec.x << ',' << vec.y << ',' << vec.z << '}';
}
