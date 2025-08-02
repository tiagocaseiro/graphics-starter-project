#pragma once

#include "opengl/OGLRenderData.h"

class Camera
{
public:
    glm::mat4 getViewMatrix(const OGLRenderData& renderData);

private:
    glm::vec3 mWorldPos      = glm::vec3(0.5f, 0.25f, 1.0f);
    glm::vec3 mViewDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 mWorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
};