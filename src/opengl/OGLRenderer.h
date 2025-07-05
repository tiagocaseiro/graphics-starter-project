#pragma once

#include "FrameBuffer.h"
#include "OGLRenderData.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "UserInterface.h"
#include "VertexBuffer.h"

struct GLFWWindow;

class OGLRenderer
{
public:
    bool init(const int width, const int height, GLFWwindow* window);
    void setSize(const int width, const int height);
    void cleanup();
    void uploadData(const OGLMesh& vertexData);
    void draw();
    void handleKeyEvents(const int key, const int scancode, const int action, const int mods);

private:
    Shader mBasicShader;
    Shader mChangedShader;
    Framebuffer mFramebuffer;
    VertexBuffer mVertexBuffer;
    UniformBuffer m_UniformBuffer;
    Texture mTex;

    glm::mat4 mViewMatrix       = glm::mat4(1.0);
    glm::mat4 mProjectionMatrix = glm::mat4(1.0);

    Shader* mActiveShader = &mBasicShader;
    OGLRenderData mRenderData;
    UserInterface mUserInterface;
};