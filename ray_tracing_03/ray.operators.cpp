


#include "ray.h"
#include "glm_prints.h"



#include<iostream>
#include<glm\glm.hpp>



std::ostream& operator<<( std::ostream& o, const Ray& r )
{
    return o << '[' << r.origin() << "-->" << r.direction() << ']';
}