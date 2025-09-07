#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>

class Texture
{
public:
    // OpenGL texture ID
    GLuint ID;
    // Image metadata
    int width = 0;
    int height = 0;
    int channels = 0;

    // Load image from path and create GL texture (2D)
    explicit Texture(const char* imagePath);

    // Bind texture to given texture unit (defaults to GL_TEXTURE0)
    void Bind(GLenum textureUnit = GL_TEXTURE0);
    // Unbind from GL_TEXTURE_2D target
    void Unbind();
    // Delete texture resource
    void Delete();
};

#endif

