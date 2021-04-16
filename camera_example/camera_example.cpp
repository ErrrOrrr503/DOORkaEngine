/*
 * This example draws a polygonal sphere and sets up camera class for further use.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../camera/camera.h"
#include "../camera/position.h"

#ifdef DE_USE_SHADERS
#include "shader_source"
#endif

// Window size constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Main camera object
// It should be private object of our engine class
static camera* g_cumera;
static player_position* g_pos;

// Last cursor position
// They should be private objects of our engine class
static GLdouble g_lastx = SCR_WIDTH / 2;
static GLdouble g_lasty = SCR_HEIGHT / 2;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glEnable(GL_MULTISAMPLE);
}

void processInput(GLFWwindow *window)
{
    if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) &&
        (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
    g_cumera->resetWindowSize(width, height);
}

void cursor_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
    (void)window;
    g_pos->rotateViewMouse(xpos - g_lastx, ypos - g_lasty);
    g_lastx = xpos;
    g_lasty = ypos;
}

int main()
{
    // Initalize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DOORka", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate ();
        return 1;
    }     
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

#ifdef DE_USE_SHADERS
    // Compiling shader program
    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vshaderSource, NULL);
    glCompileShader(vshader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vshader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    unsigned int fshader;
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fshaderSource, NULL);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fshader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }
    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed\n" << infoLog << std::endl;
    }
    glDeleteShader(vshader); // They are no longer needed, we already have a ready-to-use shader program
    glDeleteShader(fshader);
    GLint colorVarLocation = glGetUniformLocation(shaderProgram, "requestedColor");
    GLint viewVarLocation = glGetUniformLocation(shaderProgram, "view");
    GLint perspVarLocation = glGetUniformLocation(shaderProgram, "perspective");
#endif

    // Make final initialization
    sphere sfera(0.0, 0.0, 0.0, 1.0, 98, 100);
    g_pos = new player_position(window);
    g_cumera = new camera(window, g_pos);
    init();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_callback);
    g_pos->setMouseSensitivity(0.005f);
#ifdef DE_SET_VIM_MODE
    // Seiing camera movement to hjkl like vim cursor movement.
    g_pos->setControls(camera::key_container(
        GLFW_KEY_UP, GLFW_KEY_DOWN,  GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
        GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_H, GLFW_KEY_L
    ));
#endif

    // Starting render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        g_pos->relocateView();
        glClear(GL_COLOR_BUFFER_BIT);
#ifdef DE_USE_SHADERS
        // Selecting shader program
        glUseProgram(shaderProgram);
        // Setting color
        glUniform4f(colorVarLocation, 1.0f, 1.0f, 0.1f, 1.0f);
        // Loading transform matrices into GPU shader program
        glUniformMatrix4fv(viewVarLocation, 1, GL_FALSE, glm::value_ptr(g_cumera->getViewMatrix()));
        glUniformMatrix4fv(perspVarLocation, 1, GL_FALSE, glm::value_ptr(g_cumera->getPerspMatrix()));
#else
        glMatrixMode (GL_MODELVIEW);
        glLoadMatrixf (glm::value_ptr(g_cumera->getViewMatrix()));
        glMatrixMode (GL_PROJECTION);
        glLoadMatrixf (glm::value_ptr(g_cumera->getPerspMatrix()));
        glColor3f (1.0f, 1.0f, 0.1f);
#endif
        sfera.draw();
        g_pos->acquireDeltaTime();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleaning up before leaving
    delete g_cumera;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
