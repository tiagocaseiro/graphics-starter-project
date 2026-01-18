#include "UniformBuffer.h"

#include <glm/gtc/type_ptr.hpp>

UniformBuffer::UniformBuffer(int bufferSize) : mBufferSize(bufferSize)
{
    glGenBuffers(1, &mUboBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() { glDeleteBuffers(1, &mUboBuffer); }

void UniformBuffer::uploadUboData(const std::vector<glm::mat4>& matrices)
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferData(GL_UNIFORM_BUFFER, mBufferSize, matrices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::uploadUboData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUboBuffer, 0, mBufferSize);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<UniformBuffer> UniformBuffer::make(int bufferSize)
{
    return std::shared_ptr<UniformBuffer>(new UniformBuffer(bufferSize));
}
