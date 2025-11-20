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
    GLFWwindow* rdWindow             = nullptr;
    unsigned int rdWidth             = 0;
    unsigned int rdHeight            = 0;
    unsigned int rdTriangleCount     = 0;
    unsigned int rdGltfTriangleCount = 0;
    float rdFrameTime                = 0.0f;
    float rdUIGenerateTime           = 0.0f;
    float rdViewAzimuth              = 320.0f;
    float rdViewElevation            = -15.0f;
    int rdMoveForward                = 0;
    int rdMoveRight                  = 0;
    int rdMoveUp                     = 0;
    float rdTickDiff                 = 0.0;
    glm::vec3 rdCameraWorldPosition  = glm::vec3(0.5f, 0.25f, 1.0f);
};