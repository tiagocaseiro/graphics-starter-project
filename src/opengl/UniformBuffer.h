#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <glad/glad.h>

class UniformBuffer
{
public:
    static std::shared_ptr<UniformBuffer> make(int bindingPoint, int bufferSize);

    ~UniformBuffer();
    void uploadUboData(const std::vector<glm::mat4>& matrices);
    void uploadUboData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
    UniformBuffer(int bindingPoint, int bufferSize);

    const int mBufferSize;
    const int mBindingPoint;

    GLuint mUboBuffer = 0;
};