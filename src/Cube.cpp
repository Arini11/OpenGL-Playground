#include "Cube.h"
#include "shaderClass.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Crea buffers con 24 vértices (4 por cara) y 36 índices (2 triángulos por cara)
Cube::Cube()
{
    // pos (x,y,z) + tex (u,v)
    std::vector<float> vertices = {
        // Frente (z = +0.5)
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,

        // Detrás (z = -0.5)
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f,

        // Izquierda (x = -0.5)
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,

        // Derecha (x = +0.5)
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,

        // Abajo (y = -0.5)
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

        // Arriba (y = +0.5)
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,  2, 3, 0,        // Frente
        4, 5, 6,  6, 7, 4,        // Detrás
        8, 9,10, 10,11, 8,        // Izquierda
       12,13,14, 14,15,12,        // Derecha
       16,17,18, 18,19,16,        // Abajo
       20,21,22, 22,23,20         // Arriba
    };

    indexCount = static_cast<GLsizei>(indices.size());

    vao.Bind();
    vbo = new VBO(vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(float)));
    ebo = new EBO(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)));

    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    vao.LinkAttrib(*vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

Cube::~Cube()
{
    if (ebo) { ebo->Delete(); delete ebo; ebo = nullptr; }
    if (vbo) { vbo->Delete(); delete vbo; vbo = nullptr; }
    vao.Delete();
}

void Cube::Draw(Shader& shader, Texture& texture, GLint transformLoc, const glm::mat4& extra)
{
    // Modelo: T * Rz * Ry * Rx * S
    glm::mat4 I(1.0f);
    glm::mat4 T = glm::translate(I, position);
    glm::mat4 Rx = glm::rotate(I, rotation.x, glm::vec3(1,0,0));
    glm::mat4 Ry = glm::rotate(I, rotation.y, glm::vec3(0,1,0));
    glm::mat4 Rz = glm::rotate(I, rotation.z, glm::vec3(0,0,1));
    glm::mat4 S  = glm::scale(I, scale);
    glm::mat4 model = T * Rz * Ry * Rx * S;

    glm::mat4 M = extra * model;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(M));

    texture.Bind(GL_TEXTURE0);
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

