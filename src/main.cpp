#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Cube.h"

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


// (El cuadrado anterior se ha reemplazado por una clase Cube)


float rotation = 0.0f; // ángulo en radianes
float posX = 0.0f;     // posición en X
float posY = 0.0f;     // posición en Y
float scaleFactor = 1.0f; // escala uniforme



int main()
{

    // 1. Inicializar
    GLFWwindow* window = init();

    // 2. Shaders
    Shader shaderProgram("default.vert", "default.frag");

    // Localizamos uniform una vez
    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");

    // 3. Configuración para 3D y lista de cubos
    glEnable(GL_DEPTH_TEST);
    std::vector<Cube> cubes;
    cubes.emplace_back();


    // Texturas (usamos la clase Texture)
    Texture texture0("default_dirt.png");

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();


        const glm::mat4 MATRIZ_IDENTIDAD = glm::mat4(1.0f);

        // Compensar aspect ratio para que el quad sea cuadrado en pantalla
        int fbW = 0, fbH = 0;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        float aspectFixX = (fbW > 0 && fbH > 0) ? (static_cast<float>(fbH) / static_cast<float>(fbW)) : 1.0f;
        glm::mat4 aspectScale = glm::scale(MATRIZ_IDENTIDAD, glm::vec3(aspectFixX, 1.0f, 1.0f));

        // Extra: mantener aspecto X
        glm::mat4 extra = glm::scale(MATRIZ_IDENTIDAD, glm::vec3(aspectFixX, 1.0f, 1.0f));

        // Actualizamos el primer cubo con el input actual
        cubes[0].position = glm::vec3(posX, posY, 0.0f);
        cubes[0].rotation = glm::vec3(0.0f, 0.0f, rotation);
        cubes[0].scale    = glm::vec3(scaleFactor);

        // Dibujar cubos
        for (auto& c : cubes)
        {
            c.Draw(shaderProgram, texture0, transformLoc, extra);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---------------------------
    // 4) LIMPIEZA (al salir)
    // ---------------------------
    // Delete all the objects we've created
    texture0.Delete();
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

    // Movimiento WASD continuo con delta time
    static double lastTime = glfwGetTime();
    double now = glfwGetTime();
    float dt = static_cast<float>(now - lastTime);
    lastTime = now;

    const float speed = 0.8f; // unidades por segundo
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        posY += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        posY -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        posX -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        posX += speed * dt;

    // Escala con + y - (teclado principal y numérico)
    const float zoomSpeed = 1.2f; // unidades de escala por segundo
    bool plusPressed = glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS;
    bool minusPressed = glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS;
    if (plusPressed)
        scaleFactor += zoomSpeed * dt;
    if (minusPressed)
        scaleFactor -= zoomSpeed * dt;
    // límites razonables de escala
    if (scaleFactor < 0.1f) scaleFactor = 0.1f;
    if (scaleFactor > 5.0f) scaleFactor = 5.0f;

    // Limitar para que no se salga de pantalla
    int fbW = 0, fbH = 0;
    glfwGetFramebufferSize(window, &fbW, &fbH);
    float aspectFixX = (fbW > 0 && fbH > 0) ? (static_cast<float>(fbH) / static_cast<float>(fbW)) : 1.0f;

    float c = fabsf(cosf(rotation));
    float s = fabsf(sinf(rotation));
    // Media anchura/altura del AABB tras escala, aspect y rotación (NDC)
    float halfW = 0.5f * scaleFactor * (aspectFixX * c + s);
    float halfH = 0.5f * scaleFactor * (aspectFixX * s + c);

    // Clamp en coordenadas clip [-1,1]
    float minX = -1.0f + halfW;
    float maxX =  1.0f - halfW;
    float minY = -1.0f + halfH;
    float maxY =  1.0f - halfH;

    // Convertir clamp X a espacio de posición (antes del aspect),
    // ya que x_ndc = aspectFixX * posX en el pipeline actual
    float minXPos = (aspectFixX > 0.0f) ? (minX / aspectFixX) : minX;
    float maxXPos = (aspectFixX > 0.0f) ? (maxX / aspectFixX) : maxX;

    if (posX < minXPos) posX = minXPos;
    if (posX > maxXPos) posX = maxXPos;
    if (posY < minY) posY = minY;
    if (posY > maxY) posY = maxY;
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
