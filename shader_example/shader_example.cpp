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

// Window size constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Camera position variables
glm::vec3 g_camera_pos(0.0f, 0.0f, 0.1f);
glm::vec3 g_camera_direction(0.0f, 0.0f, -1.0f);
glm::vec3 g_camera_up(0.0f, 1.0f, 0.0f);
float g_camera_angle = 0.0f;
float g_pitch_angle = 0.0f;
// Camera velocity in unit/sec
float g_camera_velocity = 2.0f;
float g_angular_velocity = 2.0f;
// Timing variables
float g_elapsed_time = 0.0f;
float g_last_frame = 0.0f;

// Shader source code (maybe it's a good idea to write in into a separate file and #include it)
const GLchar* vshaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 view;\n"
"uniform mat4 perspective;\n"
"void main()\n"
"{\n"
"    gl_Position = perspective * view * vec4(aPos, 1.0);\n"
"}\n\0";
const GLchar* fshaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 requestedColor;\n"
"void main()\n"
"{\n"
"    FragColor = requestedColor;\n"
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera_pos += g_camera_velocity * g_elapsed_time
        * glm::vec3(g_camera_direction.x, 0.0f, g_camera_direction.z);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera_pos -= g_camera_velocity * g_elapsed_time
        * glm::vec3(g_camera_direction.x, 0.0f, g_camera_direction.z);;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera_pos -= glm::normalize(glm::cross(g_camera_direction, g_camera_up)) 
        * g_camera_velocity * g_elapsed_time;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera_pos += glm::normalize(glm::cross(g_camera_direction, g_camera_up)) 
        * g_camera_velocity * g_elapsed_time;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        g_camera_angle -= g_angular_velocity * g_elapsed_time;
        if (g_camera_angle < -M_PI + 0.001f)
            g_camera_angle += M_PI + M_PI;
        g_camera_direction = glm::vec3(
            sinf(g_camera_angle) * cosf(g_pitch_angle),
            sinf(g_pitch_angle),
            -cosf(g_camera_angle) * cosf(g_pitch_angle));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        g_camera_angle += g_angular_velocity * g_elapsed_time;
        if (g_camera_angle > M_PI - 0.001f)
            g_camera_angle -= M_PI + M_PI;
        g_camera_direction = glm::vec3(
            sinf(g_camera_angle) * cosf(g_pitch_angle),
            sinf(g_pitch_angle),
            -cosf(g_camera_angle) * cosf(g_pitch_angle));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (g_pitch_angle < M_PI_2)
            g_pitch_angle += g_angular_velocity * g_elapsed_time;
        g_camera_direction = glm::vec3(
            sinf(g_camera_angle) * cosf(g_pitch_angle),
            sinf(g_pitch_angle),
            -cosf(g_camera_angle) * cosf(g_pitch_angle));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (g_pitch_angle > -M_PI_2)
            g_pitch_angle -= g_angular_velocity * g_elapsed_time;
        g_camera_direction = glm::vec3(
            sinf(g_camera_angle) * cosf(g_pitch_angle),
            sinf(g_pitch_angle),
            -cosf(g_camera_angle) * cosf(g_pitch_angle));
    }

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
    sphere sfera(0.0, 0.0, 0.0, 1.0, 98, 100);
    init();
    glm::mat4 view;
    glm::mat4 perspective;

    // Starting render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        // Getting the location of uniform variable with color vector
        GLint colorVarLocation = glGetUniformLocation(shaderProgram, "requestedColor");
        GLint viewVarLocation = glGetUniformLocation(shaderProgram, "view");
        GLint perspVarLocation = glGetUniformLocation(shaderProgram, "perspective");
        glUseProgram(shaderProgram);
        // Setting color
        glUniform4f(colorVarLocation, 1.0f, 1.0f, 0.1f, 1.0f);
        // Setting view matrix
        view = glm::lookAt(g_camera_pos, g_camera_pos + g_camera_direction, g_camera_up);
        glUniformMatrix4fv(viewVarLocation, 1, GL_FALSE, glm::value_ptr(view));
        // Setting perspective projection matrix
        perspective = glm::perspective<float>(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(perspVarLocation, 1, GL_FALSE, glm::value_ptr(perspective));
        sfera.draw();
        glfwSwapBuffers(window);
        float cur_time = glfwGetTime();
        g_elapsed_time = cur_time - g_last_frame;
        g_last_frame = cur_time;
        glfwPollEvents();
    }

    // Cleaning up before leaving
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
