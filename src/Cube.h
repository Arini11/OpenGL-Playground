#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include <vector>
#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Shader;
class Texture;

// Malla de un cubo unitario centrado en el origen (pos + texcoords)
class Cube {
public:
    VAO vao;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
    GLsizei indexCount = 0;

    // Transform del cubo
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    // Rotación por ejes (radianes)
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};

    Cube();
    ~Cube();

    // Dibuja el cubo con el shader y textura dados; requiere uniform "transform"
    void Draw(Shader& shader, Texture& texture, GLint transformLoc, const glm::mat4& extra = glm::mat4(1.0f));
};

#endif

