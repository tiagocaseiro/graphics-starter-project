#pragma once

#include <glad/glad.h>

class Framebuffer
{
public:
    bool init(const int width, const int height);
    bool resize(const int width, const int height);
    void bind();
    void unbind();
    void drawToScreen();
    void cleanup();

private:
    bool checkComplete();

    int mBufferWidth    = 640;
    int mBufferHeight   = 480;
    GLuint mBuffer      = 0;
    GLuint mColorTex    = 0;
    GLuint mDepthBuffer = 0;
};