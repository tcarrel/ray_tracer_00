#pragma once



#include<glm\glm.hpp>



class Ray;
class Intersection;



class Triangle
{
public:

    Triangle( glm::dvec3 p1_pos, glm::dvec3 p2_pos, glm::dvec3 p3_pos );
    Triangle(
        glm::dvec3 p1_pos, glm::dvec3 p2_pos, glm::dvec3 p3_pos,
        glm::dvec3 normal );


    Intersection test_intersection( const Ray& ray );

    glm::dvec3 normal() const noexcept;
    glm::dvec3 center();
    
private:

    Intersection algorithm_A( const Ray& ray );
    Intersection algorithm_B( const Ray& ray );

    void ctor_body( void );

    glm::dvec3 p0_;
    glm::dvec3 p1_;
    glm::dvec3 p2_;

    glm::dvec3 normal_;

    //  AAAAAAAAAAAAAA

    glm::dvec3 basis_p0_to_p1_;
    glm::dvec3 basis_p0_to_p2_;

    double uu_;
    double uv_;
    double vv_;
    double inverse_D_;

    double d_{};

    //  BBBBBBBBBBBBBBBB

    glm::dvec3 p0_to_p1_;
    glm::dvec3 p1_to_p2_;
    glm::dvec3 p2_to_p0_;
};
