#pragma once

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
    // FrameBuffer mFrameBuffer;
    // VertexBuffer mVertexBuffer;
    // Texture mTex;

    int mTriangleCount = 0;
};