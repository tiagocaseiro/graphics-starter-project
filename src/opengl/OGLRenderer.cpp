#include "OGLRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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

    mWidth  = width;
    mHeight = height;

    mVertexBuffer.init();

    m_UniformBuffer.init();

    if(mTex.loadTexture("../textures/crate.png") == false)
    {
        return false;
    }

    if(mBasicShader.loadShaders("../shaders/basic.vert", "../shaders/basic.frag") == false)
    {
        return false;
    }

    if(mChangedShader.loadShaders("../shaders/changed.vert", "../shaders/changed.frag") == false)
    {
        return false;
    }
    return true;
}

void OGLRenderer::setSize(const int width, const int height)
{
    mWidth  = width;
    mHeight = height;

    mFramebuffer.resize(width, height);
    glViewport(0, 0, width, height);
}

void OGLRenderer::cleanup()
{
    mBasicShader.cleanup();
    mChangedShader.cleanup();
    mFramebuffer.cleanup();
    mVertexBuffer.cleanup();
    m_UniformBuffer.cleanup();
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

    glm::vec3 cameraPosition       = glm::vec3(0.4, 0.3, 1.0f);
    glm::vec3 cameraLookAtPosition = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 cameraUpVector       = glm::vec3(0.0, 1.0, 0.0);

    mProjectionMatrix =
        glm::perspective(glm::radians(90.0f), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 100.0f);

    float t = glfwGetTime();

    glm::mat4 view = glm::mat4(1.0);

    if(mActiveShader)
    {
        mActiveShader->use();
    }

    if(mActiveShader == &mBasicShader)
    {
        view = glm::rotate(glm::mat4(1.0), t, glm::vec3(0.0, 0.0, 1.0));
    }
    else
    {
        view = glm::rotate(glm::mat4(1.0), -t, glm::vec3(0.0, 0.0, 1.0));
    }

    mViewMatrix = glm::lookAt(cameraPosition, cameraLookAtPosition, cameraUpVector) * view;

    m_UniformBuffer.uploadUboData(mViewMatrix, mProjectionMatrix);
    mTex.bind();
    mVertexBuffer.bind();
    mVertexBuffer.draw(GL_TRIANGLES, 0, mTriangleCount);
    mVertexBuffer.unbind();
    mTex.bind();
    mFramebuffer.bind();

    mFramebuffer.drawToScreen();
}

void OGLRenderer::handleKeyEvents(const int key, const int scancode, const int action, const int mods)
{
    if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        if(mActiveShader == &mBasicShader)
        {
            mActiveShader = &mChangedShader;
        }
        else
        {
            mActiveShader = &mBasicShader;
        }
    }
}
