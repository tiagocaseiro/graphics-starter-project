#include "Texture.h"

#include <stb_image.h>

std::shared_ptr<Texture> Texture::make(const std::string& textureFilename, const bool flipImage)
{
    int texWidth       = 0;
    int texHeight      = 0;
    int numberChannels = 0;

    stbi_set_flip_vertically_on_load(flipImage);

    unsigned char* textureData = stbi_load(textureFilename.c_str(), &texWidth, &texHeight, &numberChannels, 0);

    if(!textureData)
    {
        stbi_image_free(textureData);
        return nullptr;
    }

    std::shared_ptr<Texture> texture =
        std::shared_ptr<Texture>(new Texture(textureData, texWidth, texHeight, numberChannels));

    stbi_image_free(textureData);

    return texture;
}

Texture::Texture(const unsigned char* const textureData, int texWidth, int texHeight, int numberChannels)
{
    static constexpr GLenum fmt[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, fmt[numberChannels - 1], texWidth, texHeight, 0, fmt[numberChannels - 1],
                 GL_UNSIGNED_BYTE, textureData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE, 0);
}

void Texture::bind() { glBindTexture(GL_TEXTURE, mTexture); }

void Texture::unbind() { glBindTexture(GL_TEXTURE, 0); }