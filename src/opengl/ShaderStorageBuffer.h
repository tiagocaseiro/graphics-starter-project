#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <glad/glad.h>

class ShaderStorageBuffer
{
public:
    static std::shared_ptr<ShaderStorageBuffer> make(int bindingPoint, int bufferSize);

    ~ShaderStorageBuffer();
    void uploadData(const std::vector<glm::mat4>& matrices);
    void uploadData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
    ShaderStorageBuffer(int bindingPoint, int bufferSize);

    const int mBufferSize;
    const int mBindingPoint;

    GLuint mBuffer = 0;
};