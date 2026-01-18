#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <glad/glad.h>

class UniformBuffer
{
public:
    static std::shared_ptr<UniformBuffer> make();

    ~UniformBuffer();
    void uploadUboData(const std::vector<glm::mat4>& matrices);
    void uploadUboData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
    UniformBuffer();

    GLuint mUboBuffer = 0;
};