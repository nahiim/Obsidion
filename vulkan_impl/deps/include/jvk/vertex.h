
#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>



struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 tex_coord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};