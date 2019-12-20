


#include "helpers.h"



std::string append_index_to_filename( const std::string& filename, int index )
{
    if( index < 0 )
    {
        return filename + " (#)";
    }

    return filename + " (" + std::to_string( index ) + ')';
}



glm::mat3 orthonormal_basis_from_ZY( const glm::vec3& Z, const glm::vec3& Y )
{
    auto XX = glm::cross( Y, Z );
    auto YY = glm::normalize( glm::cross( XX, Y ) );

    return glm::mat3( XX, YY, Z );
}
