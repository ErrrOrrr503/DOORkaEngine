/*
 * This example draws a polygonal sphere using GLSL shaders.
 * Also sets esc key to close the window.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "sphere.h"

// Window size constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Shader source code (maybe it's a good idea to write in into a separate file and #include it)
const GLchar* vshaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";
const GLchar* fshaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glEnable(GL_MULTISAMPLE);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

int main()
{
    // Initalize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DOORka", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Compiling shader program
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

    // Make final initialization
    sphere sfera(0.0, 0.0, 0.0, 1.0, 98, 10);
    init();

    // Starting render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        sfera.draw();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    // Cleaning up before leaving
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}