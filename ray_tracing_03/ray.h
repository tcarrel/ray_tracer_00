#pragma once



#include<iosfwd>
#include<optional>
#include<glm\glm.hpp>



class Ray
{
public:

    Ray( glm::dvec3 origin, glm::dvec3 direction, size_t depth = 0, bool is_primary_ray = true );

    const glm::dvec3& origin( void ) const noexcept;
    const glm::dvec3& direction( void ) const noexcept;

    const glm::dvec3 point_on_ray_at( double t ) const noexcept;

    size_t depth( void ) const noexcept;
    size_t new_depth( void ) const noexcept;

    void save_point_light_color( glm::dvec3 pl_color );
    std::optional<glm::dvec3>& point_light_color( void );

    static size_t count( void ) noexcept;

    const bool is_primary{ true };

private:

    glm::dvec3 origin_;
    glm::dvec3 direction_;
    size_t depth_;

    static size_t quantity_created_;

    std::optional<glm::dvec3> pl_color_{};
};

std::ostream& operator<<( std::ostream& o, const Ray& r );
