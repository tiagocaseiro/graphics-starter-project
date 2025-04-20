#pragma once

#include <glad/glad.h>

class OGLMesh;

class VertexBuffer
{
public:
    void init();
    void uploadData(const OGLMesh& vertexData);
    void bind();
    void unbind();
    void draw(GLuint mode, const int start, const int num);
    void cleanup();

private:
    GLuint mVAO       = 0;
    GLuint mVertexVBO = 0;
};