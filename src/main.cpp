#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void vertexSetup();
GLFWwindow* init();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertices coordinates
/*
GLfloat vertices[] =
{
//    X                         Y
	-0.5f     , -0.5f * float(sqrt(3)) / 3     , 0.0f, // Lower left corner
	0.5f      , -0.5f * float(sqrt(3)) / 3     , 0.0f, // Lower right corner
	0.0f      , 0.5f  * float(sqrt(3)) * 2 / 3 , 0.0f, // Upper corner
	-0.5f / 2 , 0.5f  * float(sqrt(3)) / 6     , 0.0f, // Inner left
	0.5f / 2  , 0.5f  * float(sqrt(3)) / 6     , 0.0f, // Inner right
	0.0f      , -0.5f * float(sqrt(3)) / 3     , 0.0f // Inner down
};

GLfloat vertices[] =
{
//    X                         Y
	-1.0f     , -1.0f * float(sqrt(3)) / 3     , 0.0f, // Lower left corner
	1.0f      , -1.0f * float(sqrt(3)) / 3     , 0.0f, // Lower right corner
	0.0f      , 1.0f  * float(sqrt(3)) / 3 , 0.0f, // Upper corner
	-1.0f / 2 , 1.0f  * float(sqrt(3)) / 6     , 0.0f, // Inner left
	1.0f / 2  , 1.0f  * float(sqrt(3)) / 6     , 0.0f, // Inner right
	0.0f      , -1.0f * float(sqrt(3)) / 3     , 0.0f // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};
*/


// Cuadrado
GLfloat vertices[] =
{
//                  COORDENADAS                 TEXTURA
//                              [x, y, z, u, v]
	-1.0f / 2    , 1.0f  / 2   , 0.0f,         0.0f, 0.0f, // Arriba izq
	1.0f  / 2    , 1.0f  / 2   , 0.0f,         1.0f, 0.0f, // Arriba der
	-1.0f / 2    , -1.0f / 2   , 0.0f,         0.0f, 1.0f, // Abajo izq
    1.0f  / 2    , -1.0f / 2   , 0.0f,         1.0f, 1.0f, // Abajo der
};
GLuint indices[] =
{
	0, 2, 3, // Triangulo izq
    0, 1, 3 // Triangulo der
};


float rotation = 0.0f; // ángulo en radianes



int main()
{

    // 1. Inicializar
    GLFWwindow* window = init();

    // 2. Shaders
    Shader shaderProgram("default.vert", "default.frag");

    // Localizamos uniform una vez
    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");

    // 3. VAO / VBO
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes: position (loc=0) and texcoords (loc=1)
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


    // Texturas
    int imgWidth, imgHeight,  numColCh;
    unsigned char* bytes = stbi_load("default_dirt.png",&imgWidth, &imgHeight, &numColCh, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni,0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        glBindTexture(GL_TEXTURE_2D, texture);


        const glm::mat4 MATRIZ_IDENTIDAD = glm::mat4(1.0f);

        // Compensar aspect ratio para que el quad sea cuadrado en pantalla
        int fbW = 0, fbH = 0;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        float aspectFixX = (fbW > 0 && fbH > 0) ? (static_cast<float>(fbH) / static_cast<float>(fbW)) : 1.0f;
        glm::mat4 aspectScale = glm::scale(MATRIZ_IDENTIDAD, glm::vec3(aspectFixX, 1.0f, 1.0f));

        // Rotación en Z (plano de pantalla)
        glm::mat4 rotacion = glm::rotate(MATRIZ_IDENTIDAD, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 transform = aspectScale * rotacion;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            // Bind textura (si está creada) y luego el VAO
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Bind the VAO so OpenGL knows to use it
            VAO1.Bind();

		// Dibujamos el triángulo (3 índices)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---------------------------
    // 4) LIMPIEZA (al salir)
    // ---------------------------
    // Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
    glDeleteTextures(1,&texture);
	shaderProgram.Delete();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Cerrar con ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Rotar 10º al pulsar ESPACIO
    static bool spaceWasDown = false;
    int spaceNow = glfwGetKey(window, GLFW_KEY_SPACE);
    if (spaceNow == GLFW_PRESS && !spaceWasDown)
    {
        rotation += glm::radians(10.0f);
    }
    spaceWasDown = (spaceNow == GLFW_PRESS);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* init()
{
    // glfw: inicializar, con v.3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw: ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Playground", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    return window;
}
