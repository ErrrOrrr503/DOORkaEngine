#include "render.h"

camera *gCamera; //crunch... BF crunch for BF cringe... to use in c-callback of glfw
//well gCamera is only visible in render.cpp so it is not that global. And it can be altered by only render members.
//Maybe that's not that awful...

Render::Render (Level &level)
{
    //fixme: start position of cam in level, will use (0 0 0) (1 0 0) instead
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    window_ = glfwCreateWindow(width_, height_, "DOORkaEngine", NULL, NULL);
    if (window_ == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate ();
        status_ = err_glfw;
    }     
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        status_ = err_glew;
    }
    draw_mode_ = gl12; //fixme: different renders according to hw, same interface;
    gCamera = new camera (window_, glm::vec3 (1, 0, 0), glm::vec3 (0, 0, 0));
}

Render::~Render ()
{
    delete gCamera;
}

void Render::draw ()
{
    if (draw_mode_ == gl12)
        draw_gl12 ();
}

void Render::draw_gl12 ()
{
    if (glfwWindowShouldClose(window_)) {
        status_ = should_close;
        return;
    }
    sphere sfera(0.0, 0.0, 0.0, 1.0, 98, 100);
    gCamera->relocateView ();
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadMatrixf (glm::value_ptr(gCamera->getViewMatrix()));
    glMatrixMode (GL_PROJECTION);
    glLoadMatrixf (glm::value_ptr(gCamera->getPerspMatrix()));
    glColor3f (1.0f, 1.0f, 0.1f);
    sfera.draw();
    gCamera->acquireDeltaTime();
    glfwSwapBuffers(window_);
}

render_status_type Render::status ()
{
    return status_;
}

void Render::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    gCamera->resetWindowSize (width, height);
}

void Render::setCamera (glm::vec3 positionv, glm::vec3 directionv)
{
    gCamera->setPosition (positionv);
    gCamera->setDirection (directionv);
}