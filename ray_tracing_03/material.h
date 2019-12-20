#pragma once



#include<glm\glm.hpp>
#include<utility>



#include "material.type_enum.h"



class Material
{
public:
    static Material make_diffuse( glm::dvec3 color );
    static Material make_specular( glm::dvec3 color, glm::dvec3 specularity );
    static Material make_specular( glm::dvec3 color, double specularity );
    static Material make_reflective( glm::dvec3 color, glm::dvec3 reflectivity );
    static Material make_reflective( double color, glm::dvec3 reflectivity );
    static Material make_reflective( glm::dvec3 color, double reflectivity );
    static Material make_reflective( double color, double reflectivity );
    static Material make_transparent( glm::dvec3 color, glm::dvec3 reflectiviy, glm::dvec3 refractivity );
    static Material make_transparent( double color, glm::dvec3 reflectiviy, glm::dvec3 refractivity );
    static Material make_transparent( double color, double reflectiviy, glm::dvec3 refractivity );
    static Material make_transparent( double color, glm::dvec3 reflectiviy, double refractivity );
    static Material make_transparent( glm::dvec3 color, double reflectiviy, glm::dvec3 refractivity );
    static Material make_transparent( glm::dvec3 color, double reflectiviy, double refractivity );
    static Material make_transparent( double color, double reflectiviy, double refractivity );

private:

    Material( Material_Type type ) : type_( type )
    {}

    const Material_Type type_;

    static glm::dvec3 dbl_to_vec3( double val );

    glm::dvec3 diffuse_;
    glm::dvec3 specularity_;
    glm::dvec3 reflective_;
    glm::dvec3 refractive_;
};

