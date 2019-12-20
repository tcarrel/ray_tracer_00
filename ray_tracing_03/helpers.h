#pragma once



#include<string>
#include<glm\glm.hpp>



std::string append_index_to_filename( const std::string& filename, int index = -1 );

glm::mat3 orthonormal_basis_from_ZY( const glm::vec3& Z, const glm::vec3& Y );


/// <summary>
/// Declares all of the comparison operators.
/// </summary>
#define OPERATOR_DECLS(Object_Name) \
bool operator==(const Object_Name& rhs); \
bool operator!=(const Object_Name& rhs); \
bool operator<=(const Object_Name& rhs); \
bool operator>=(const Object_Name& rhs); \
 bool operator<(const Object_Name& rhs); \
 bool operator>(const Object_Name& rhs);

