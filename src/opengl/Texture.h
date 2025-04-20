#pragma once

#include <string>

#include <glad/glad.h>

class Texture
{
public:
    bool loadTexture(const std::string& textureFilename);
    void bind();
    void unbind();
    void cleanup();

private:
    GLuint mTexture = 0;
};