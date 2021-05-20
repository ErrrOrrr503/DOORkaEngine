#ifndef RENDER_H
#define RENDER_H

#include "../camera/camera.h"
#include "../camera/position.h"
#include "../level/level_engine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>


enum render_status_type {
    ok,
    should_close,
    err_glfw,
    err_glew,
    err_texture
};

enum render_draw_mode {
    gl12,
};

struct texture_res {
    texture_res (int nwidth, int nheight)
    {
        height = nheight;
        width = nwidth;
    }
    int height = 0;
    int width = 0;
};

class Textures {
public:
    ~Textures ();
    int init_gl12 (const Level &level);
    GLuint operator[] (size_t i);

private:
    GLint texture_mag_mapping_method_ = GL_LINEAR;
    GLint texture_min_mapping_method_ = GL_LINEAR_MIPMAP_LINEAR;
    const std::string textures_path_ = "./textures/";
    GLuint *texture_names_list_ = nullptr;
    std::vector<texture_res> texture_resolutions_;
};

class Render {
public:
    Render () = delete;
    Render (const Level &level);
    ~Render ();
    void draw ();
    render_status_type status ();
    void setCamera (glm::vec3 positionv, glm::vec3 directionv);

private:
    void draw_gl12 ();
    static void framebuffer_size_callback (GLFWwindow* window, int width, int height);
    static void cursor_callback (GLFWwindow* window, GLdouble xpos, GLdouble ypos);
    static void misc_keyboard_input (GLFWwindow *window, int key, int scancode, int action, int mods);

    Textures textures_;
    render_status_type status_ = ok;
    render_draw_mode draw_mode_ = gl12;
    GLFWwindow *window_;
    int width_ = 800;
    int height_ = 600;
    const Level *level_;
};

#endif