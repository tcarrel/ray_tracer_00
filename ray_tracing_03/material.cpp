


#include "material.h"
#include "material.type_enum.h"



glm::dvec3 Material::dbl_to_vec3( double val )
{
    return { val, val, val };
}



Material Material::make_diffuse( glm::dvec3 color )
{
    Material new_mat( Material_Type::DIFFUSE );

    new_mat.diffuse_ = color;

    return new_mat;
}

Material Material::make_specular( glm::dvec3 color, glm::dvec3 specularity )
{
    Material new_mat( Material_Type::GLOSSY );

    new_mat.diffuse_ = color;
    new_mat.specularity_ = specularity;

    return new_mat;
}

Material Material::make_specular( glm::dvec3 color, double specularity )
{
    return make_specular( color, dbl_to_vec3( specularity ) );
}

Material Material::make_reflective( glm::dvec3 color, glm::dvec3 reflectivity )
{
    Material new_mat( Material_Type::REFLECTIVE );

    new_mat.diffuse_ = color;
    new_mat.reflective_ = reflectivity;

    return new_mat;
}



Material Material::make_reflective( double color, glm::dvec3 reflectivity )
{
    return make_reflective( dbl_to_vec3( color ), reflectivity );
}



Material Material::make_reflective( glm::dvec3 color, double reflectivity )
{
    return make_reflective( color, dbl_to_vec3( reflectivity ) );
}



Material Material::make_reflective( double color, double reflectivity )
{
    return make_reflective( dbl_to_vec3( color ), dbl_to_vec3( reflectivity ) );
}

Material Material::make_transparent( glm::dvec3 color, glm::dvec3 reflectiviy, glm::dvec3 refractivity )
{
    Material new_mat( Material_Type::TRANSPARENT );

    new_mat.diffuse_ = color;
    new_mat.reflective_ = reflectiviy;
    new_mat.refractive_ = refractivity;

    return new_mat;
}

Material Material::make_transparent( double color, glm::dvec3 reflectivity, glm::dvec3 refractivity )
{
    return make_transparent( dbl_to_vec3( color ), reflectivity, refractivity );
}

Material Material::make_transparent( double color, double reflectivity, glm::dvec3 refractivity )
{
    return make_transparent( dbl_to_vec3( color ), dbl_to_vec3( reflectivity ), refractivity );
}

Material Material::make_transparent( double color, glm::dvec3 reflectivity, double refractivity )
{
    return make_transparent( dbl_to_vec3( color ), reflectivity, dbl_to_vec3( refractivity ) );
}

Material Material::make_transparent( glm::dvec3 color, double reflectivity, glm::dvec3 refractivity )
{
    return make_transparent( color, dbl_to_vec3( reflectivity ), refractivity );
}

Material Material::make_transparent( glm::dvec3 color, double reflectivity, double refractivity )
{
    return make_transparent( color, dbl_to_vec3( reflectivity ), dbl_to_vec3( refractivity ) );
}

Material Material::make_transparent( double color, double reflectiviy, double refractivity )
{
    return make_transparent( dbl_to_vec3( color ), dbl_to_vec3( reflectiviy ), dbl_to_vec3( refractivity ) );
}
