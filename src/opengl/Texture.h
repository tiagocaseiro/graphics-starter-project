#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

class Texture
{
public:
    static std::shared_ptr<Texture> make(const std::string& textureFilename, const bool flipImage = true);
    void bind();
    void unbind();

private:
    Texture(const unsigned char* const textureData, int texWidth, int texHeight, int numberChannels);

    GLuint mTexture = 0;
};