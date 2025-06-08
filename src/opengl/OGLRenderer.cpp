#include "OGLRenderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

bool OGLRenderer::init(const int width, const int height)
{
    if(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
    {
        return false;
    }

    if(GLAD_GL_VERSION_4_6 == false)
    {
        return false;
    }

    if(mFramebuffer.init(width, height) == false)
    {
        return false;
    }

    mVertexBuffer.init();

    if(mTex.loadTexture("../textures/crate.png") == false)
    {
        return false;
    }

    if(mBasicShader.loadShaders("../shaders/basic.vert", "../shaders/basic.frag") == false)
    {
        return false;
    }
    return true;
}

void OGLRenderer::setSize(const int width, const int height)
{
    mFramebuffer.resize(width, height);
    glViewport(0, 0, width, height);
}

void OGLRenderer::cleanup()
{
    mBasicShader.cleanup();
    mFramebuffer.cleanup();
    mVertexBuffer.cleanup();
    mTex.cleanup();
}

void OGLRenderer::uploadData(const OGLMesh& vertexData)
{
    mTriangleCount = vertexData.vertices.size();
    mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::draw()
{
    mFramebuffer.bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    mBasicShader.use();
    mTex.bind();
    mVertexBuffer.bind();
    mVertexBuffer.draw(GL_TRIANGLES, 0, mTriangleCount);
    mVertexBuffer.unbind();
    mTex.bind();
    mFramebuffer.bind();

    mFramebuffer.drawToScreen();
}
