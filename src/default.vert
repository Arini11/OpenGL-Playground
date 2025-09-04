#version 330 core

// Posicion
layout (location = 0) in vec3 aPos;
// Textura
layout (location = 1) in vec2 aTex;

// Transformacion
uniform mat4 transform;

out vec2 texCoord;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0); // lo mismo que (aPos.x, aPos.y, aPos.z, 1.0)
    texCoord = aTex;
}
