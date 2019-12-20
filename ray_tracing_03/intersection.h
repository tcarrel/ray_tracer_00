#pragma once



#include "helpers.h"
#include "ray.h"
#include<glm\glm.hpp>
#include<vector>



enum class Primitive_Type
{
    UNKNOWN_PRIMITIVE_TYPE = -1,
    SPHERE,
    TRIANGLE,
    POINT_LIGHT
};



class Intersection
{
public:
    bool is_a_hit = false;

    const Primitive_Type type = Primitive_Type::UNKNOWN_PRIMITIVE_TYPE;

    Intersection(
        bool hit,
        size_t depth = 0,
        double dist = 0,
        glm::dvec3 pos = {},
        glm::dvec3 norm = {},
        glm::dvec3 found_color = {},
        Primitive_Type tp = Primitive_Type::UNKNOWN_PRIMITIVE_TYPE );

    size_t depth( void );
    size_t new_depth( void );
    double distance( void );
    glm::dvec3 position( void );
    glm::dvec3 normal( void );

    static Intersection closer_of( Intersection A, Intersection B ) noexcept;

    Intersection& operator=( const Intersection& rhs );

    OPERATOR_DECLS(Intersection)

private:

    double distance_{}; //along ray.
    glm::dvec3 position_{};
    glm::dvec3 normal_{};
    size_t depth_;
    glm::dvec3 color_found_{};
    // ptr to object.
};

