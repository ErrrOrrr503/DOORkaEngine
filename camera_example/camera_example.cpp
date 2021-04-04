/*
 * This example draws a polygonal sphere using GLSL shaders.
 * Also sets esc key to close the window.
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

#include "shader_source"

// Window size constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Main camera object
// It should be private object of our engine class
static camera* g_cumera;

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
    g_cumera->rotateViewMouse(xpos - g_lastx, ypos - g_lasty);
    g_lastx = xpos;
    g_lasty = ypos;
}

int main()
{
    // Initalize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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

    /* Compiling shader program
    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vshaderSource, NULL);
    glCompileShader(vshader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fshader;
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fshaderSource, NULL);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vshader); // They are no longer needed, we already have a ready-to-use shader program
    glDeleteShader(fshader);
    */

    // Make final initialization
    sphere sfera(0.0, 0.0, 0.0, 1.0, 98, 100);
    g_cumera = new camera(window);
    init();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_callback);

    //GLint colorVarLocation = glGetUniformLocation(shaderProgram, "requestedColor");
    //GLint viewVarLocation = glGetUniformLocation(shaderProgram, "view");
    //GLint perspVarLocation = glGetUniformLocation(shaderProgram, "perspective");

    // Starting render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        g_cumera->relocateView();
        glClear(GL_COLOR_BUFFER_BIT);
        // Getting the location of uniform variable with color vector
        //glUseProgram(shaderProgram);
        // Setting color
        //glUniform4f(colorVarLocation, 1.0f, 1.0f, 0.1f, 1.0f);
        //glUniformMatrix4fv(viewVarLocation, 1, GL_FALSE, glm::value_ptr(cumera.getViewMatrix()));
        //glUniformMatrix4fv(perspVarLocation, 1, GL_FALSE, glm::value_ptr(cumera.getPerspMatrix()));
        glMatrixMode (GL_MODELVIEW);
        glLoadMatrixf (glm::value_ptr(g_cumera->getViewMatrix()));
        glMatrixMode (GL_PROJECTION);
        glLoadMatrixf (glm::value_ptr(g_cumera->getPerspMatrix()));
        glColor3f (1.0f, 1.0f, 0.1f);
        sfera.draw();
        g_cumera->acquireDeltaTime();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleaning up before leaving
    delete g_cumera;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
