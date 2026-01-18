#include "UniformBuffer.h"

#include <glm/gtc/type_ptr.hpp>

UniformBuffer::UniformBuffer(int bindingPoint, int bufferSize) : mBufferSize(bufferSize), mBindingPoint(bindingPoint)
{
    glGenBuffers(1, &mUboBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() { glDeleteBuffers(1, &mUboBuffer); }

void UniformBuffer::uploadUboData(const std::vector<glm::mat4>& matrices)
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferData(GL_UNIFORM_BUFFER, mBufferSize, matrices.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mUboBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::uploadUboData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBufferRange(GL_UNIFORM_BUFFER, mBindingPoint, mUboBuffer, 0, mBufferSize);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<UniformBuffer> UniformBuffer::make(int bindingPoint, int bufferSize)
{
    return std::shared_ptr<UniformBuffer>(new UniformBuffer(bindingPoint, bufferSize));
}
