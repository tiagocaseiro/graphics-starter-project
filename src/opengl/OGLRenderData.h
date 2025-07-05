#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

struct OGLVertexData
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 uv;
};

struct OGLMesh
{
    std::vector<OGLVertexData> vertices;
};

struct OGLRenderData
{
    GLFWwindow* rdWindow         = nullptr;
    unsigned int rdWidth         = 0;
    unsigned int rdHeight        = 0;
    unsigned int rdTriangleCount = 0;
};