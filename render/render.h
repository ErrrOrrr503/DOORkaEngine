#ifndef RENDER_H
#define RENDER_H

#define GLEW_STATIC

#include "../camera/camera.h"
#include "../level/level_engine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../camera_example/sphere.h"

enum render_status_type {
    ok,
    should_close,
    err_glfw,
    err_glew
};

enum render_draw_mode {
    gl12,
};

class Render {
public:
    Render () = delete;
    Render (Level &level);
    ~Render ();
    void draw ();
    render_status_type status ();
    void setCamera (glm::vec3 positionv, glm::vec3 directionv);

private:
    void draw_gl12 ();
    static void framebuffer_size_callback (GLFWwindow* window, int width, int height);
    static void cursor_callback (GLFWwindow* window, GLdouble xpos, GLdouble ypos);
    static void misc_keyboard_input (GLFWwindow *window, int key, int scancode, int action, int mods);

    render_status_type status_ = ok;
    render_draw_mode draw_mode_ = gl12;
    GLFWwindow *window_;
    int width_ = 800;
    int height_ = 600;
    Level *level_;
};

#endif