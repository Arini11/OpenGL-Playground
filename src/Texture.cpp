#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"

Texture::Texture(const char* imagePath)
{
    // Load image (do not flip to keep behavior consistent with current shaders)
    unsigned char* bytes = stbi_load(imagePath, &width, &height, &channels, 0);

    // Determine source format from channel count
    GLenum srcFormat = GL_RGB;
    if (channels == 4) srcFormat = GL_RGBA;
    else if (channels == 1) srcFormat = GL_RED;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload pixels if available
    if (bytes)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, srcFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Cleanup CPU-side data and unbind
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

