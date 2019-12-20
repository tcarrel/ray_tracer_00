#pragma once



#include<glm\glm.hpp>



class Point_Light
{
public:
    Point_Light( const glm::dvec3& position, const glm::dvec3& color, double brightness );

    glm::dvec3 position( void );
    glm::dvec3 color( void );

private:
    glm::dvec3 position_;
    glm::dvec3 color_;
};

