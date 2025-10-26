#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool Texture::loadTexture(const std::string& textureFilename)
{
    int mTexWidth       = 0;
    int mTexHeight      = 0;
    int mNumberChannels = 0;

    GLenum constexpr fmt[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

    stbi_set_flip_vertically_on_load(true);

    unsigned char* textureData = stbi_load(textureFilename.c_str(), &mTexWidth, &mTexHeight, &mNumberChannels, 0);

    if(!textureData)
    {
        stbi_image_free(textureData);
        return false;
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, fmt[mNumberChannels - 1], mTexWidth, mTexHeight, 0, fmt[mNumberChannels - 1],
                 GL_UNSIGNED_BYTE, textureData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE, 0);

    stbi_image_free(textureData);

    return true;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE, mTexture);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE, 0);
}

void Texture::cleanup()
{
}
