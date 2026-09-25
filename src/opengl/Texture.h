#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

#include "tools/Macros.h"

SHARED_ONLY(Texture)

class Texture
{
public:
    static TextureShared make(const std::string& textureFilename, const bool flipImage = true);
    void bind();
    void unbind();

private:
    Texture(const unsigned char* const textureData, int texWidth, int texHeight, int numberChannels);

    GLuint mTexture = 0;
};