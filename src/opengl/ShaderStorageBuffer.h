#pragma once

#include <memory>
#include <span>

#include <glm/glm.hpp>

#include <glad/glad.h>

template <typename T>
class ShaderStorageBuffer
{
public:
    static auto make(int bindingPoint, const std::span<const T>& buffer)
    {
        return std::shared_ptr<ShaderStorageBuffer>(new ShaderStorageBuffer(bindingPoint, buffer.size_bytes()));
    }

    ~ShaderStorageBuffer() { glDeleteBuffers(1, &mBuffer); }

    void uploadData(const std::vector<T>& matrices)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, mBufferSize, matrices.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, mBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    ShaderStorageBuffer(int bindingPoint, int bufferSize) : mBufferSize(bufferSize), mBindingPoint(bindingPoint)
    {
        glGenBuffers(1, &mBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    const int mBufferSize;
    const int mBindingPoint;

    GLuint mBuffer = 0;
};