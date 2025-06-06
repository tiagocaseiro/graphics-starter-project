#pragma once

#include "FrameBuffer.h"
#include "Texture.h"
#include "VertexBuffer.h"


class OGLMesh;

class OGLRenderer
{
public:
    bool init(const int width, const int height);
    void setSize(const int width, const int height);
    void cleanup();
    void uploadData(const OGLMesh& vertexData);
    void draw();

private:
    // Shader mBasicShader;
    Framebuffer mFramebuffer;
    VertexBuffer mVertexBuffer;
    Texture mTex;

    int mTriangleCount = 0;
};