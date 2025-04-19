#pragma once

#include <glm/glm.hpp>

struct OGLVertexData
{
    glm::vec3 position;
    glm::vec2 uv;
};

struct OGLMesh
{
    std::vector<OGLVertexData> vertices;
};