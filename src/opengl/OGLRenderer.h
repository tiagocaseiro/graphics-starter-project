#pragma once

#include "FrameBuffer.h"
#include "OGLRenderData.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"

class OGLRenderer
{
public:
    bool init(const int width, const int height);
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
    Texture mTex;

    Shader* mActiveShader = &mBasicShader;

    int mTriangleCount = 0;
};