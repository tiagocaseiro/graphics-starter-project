#include "ShaderStorageBuffer.h"

#include <glm/gtc/type_ptr.hpp>

ShaderStorageBuffer::ShaderStorageBuffer(int bindingPoint, int bufferSize)
    : mBufferSize(bufferSize), mBindingPoint(bindingPoint)
{
    glGenBuffers(1, &mBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBuffer::~ShaderStorageBuffer() { glDeleteBuffers(1, &mBuffer); }

void ShaderStorageBuffer::uploadData(const std::vector<glm::mat4>& matrices)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, mBufferSize, matrices.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, mBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::uploadData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBuffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, mBindingPoint, mBuffer, 0, mBufferSize);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::make(int bindingPoint, int bufferSize)
{
    return std::shared_ptr<ShaderStorageBuffer>(new ShaderStorageBuffer(bindingPoint, bufferSize));
}
