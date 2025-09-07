# Setup Basico OpenGL (glfw + glad) Linux
https://medium.com/geekculture/a-beginners-guide-to-setup-opengl-in-linux-debian-2bfe02ccd1e

# Compilar y ejecutar
```
g++ -I /usr/include/stb/ *.cpp glad.c -ldl -lglfw && ./a.out
```

# Esquema ciclo vida shaders OpenGL
(más o menos)

```
┌─────────────────────────────┐
│ 1. Crear objeto Shader      │
│ glCreateShader(type)        │
│ → devuelve un ID            │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 2. Cargar código GLSL       │
│ glShaderSource(id, source)  │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 3. Compilar Shader          │
│ glCompileShader(id)         │
│ (todavía no se puede usar)  │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 4. Crear Program            │
│ glCreateProgram() → ID      │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 5. Adjuntar Shaders         │
│ glAttachShader(program, id) │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 6. Linkar Programa          │
│ glLinkProgram(program)      │
│ (el programa ya es usable)  │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 7. Borrar Shaders sueltos   │
│ glDeleteShader(id)          │
│ (ya están dentro del        │
│  programa, no hacen falta)  │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ 8. Usar Programa            │
│ glUseProgram(program)       │
└─────────────────────────────┘

```
