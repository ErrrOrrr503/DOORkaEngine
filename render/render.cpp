#include "render.h"

static camera *gCamera; //crunch... BF crunch for BF cringe... to use in c-callback of glfw
//well gCamera is only visible in render.cpp so it is not that global. And it can be altered by only render members.
//Maybe that's not that awful...

Render::Render (Level &level)
{
    //fixme: start position of cam in level, will use (0 0 0) (1 0 0) instead
    level_ = &level;
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    window_ = glfwCreateWindow(width_, height_, "DOORkaEngine",  NULL, NULL);
    if (window_ == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate ();
        status_ = err_glfw;
    }     
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window_, cursor_callback);
    glfwSetKeyCallback(window_, misc_keyboard_input);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        status_ = err_glew;
    }
    draw_mode_ = gl12; //fixme: different renders according to hw, same interface;
    gCamera = new camera (window_, glm::vec3 (0, 0, 25.0 / CELL_SIZE), glm::vec3 (1, 0, 0));
    gCamera->setMouseSensitivity(0.005f);
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_MULTISAMPLE);
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
    //fixme check if window is correctly init
    if (window_ == NULL) {
        status_ = should_close;
        return;
    }
    if (glfwWindowShouldClose(window_)) {
        status_ = should_close;
        glfwDestroyWindow(window_);
        return;
    }
    gCamera->relocateView ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadMatrixf (glm::value_ptr(gCamera->getViewMatrix()));
    glMatrixMode (GL_PROJECTION);
    glLoadMatrixf (glm::value_ptr(gCamera->getPerspMatrix()));
    glColor3f (1.0f, 1.0f, 0.1f);
    //now camera is set and we can draw
    GLfloat wall[12];
    for (size_t i = 0; i < level_->walls.size (); i++) {
        glColor3f (level_->walls[i].color[0], level_->walls[i].color[1], level_->walls[i].color[2]);
        wall[0] = level_->walls[i].x1 / CELL_SIZE;
        wall[1] = level_->walls[i].y1 / CELL_SIZE;
        wall[2] = level_->walls[i].zlo1 / CELL_SIZE;
        wall[3] = level_->walls[i].x1 / CELL_SIZE;
        wall[4] = level_->walls[i].y1 / CELL_SIZE;
        wall[5] = level_->walls[i].zhi1 / CELL_SIZE;
        wall[6] = level_->walls[i].x2 / CELL_SIZE;
        wall[7] = level_->walls[i].y2 / CELL_SIZE;
        wall[8] = level_->walls[i].zlo2 / CELL_SIZE;
        wall[9] = level_->walls[i].x2 / CELL_SIZE;
        wall[10] = level_->walls[i].y2 / CELL_SIZE;
        wall[11] = level_->walls[i].zhi2 / CELL_SIZE;
        glEnableClientState (GL_VERTEX_ARRAY);
        glVertexPointer (3, GL_FLOAT, 0, wall);
        glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
        glLineWidth (2);
        glColor3f (0.8, 0.2, 0);
        glDrawArrays (GL_LINE_LOOP, 0, 4);
    }

    gCamera->acquireDeltaTime();
    glfwSwapBuffers(window_);
}

render_status_type Render::status ()
{
    return status_;
}

void Render::framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
    gCamera->resetWindowSize (width, height);
}

void Render::cursor_callback (GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
    static GLdouble lastx = xpos;
    static GLdouble lasty = ypos;
    (void)window;
    gCamera->rotateViewMouse(xpos - lastx, ypos - lasty);
    lastx = xpos;
    lasty = ypos;
}


void Render::setCamera (glm::vec3 positionv, glm::vec3 directionv)
{
    gCamera->setPosition (positionv);
    gCamera->setDirection (directionv);
}

void Render::misc_keyboard_input (GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        static bool is_fullscreen = false;
        static int windowed_width;
        static int windowed_height;
        static int windowed_xpos;
        static int windowed_ypos;
        if (!is_fullscreen) {
            glfwGetWindowSize (window, &windowed_width, &windowed_height);
            glfwGetWindowPos (window, &windowed_xpos, &windowed_ypos);
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            int count, maxres_index = 0;
            const GLFWvidmode *modes = glfwGetVideoModes (monitor, &count);
            for (int i = 1; i < count; i++) {
                if (modes[i].width >= modes[maxres_index].width && modes[i].height >= modes[maxres_index].height)
                    maxres_index = i;
            }
            glfwSetWindowMonitor (window, monitor, 0, 0, modes[maxres_index].width, modes[maxres_index].height, GLFW_DONT_CARE);
            is_fullscreen = true;
        }
        else {
            glm::vec3 pre_resize_direction = gCamera->getDirection ();
            glfwSetWindowMonitor (window, NULL, windowed_xpos, windowed_ypos, windowed_width, windowed_height, GLFW_DONT_CARE);
            gCamera->setDirection (pre_resize_direction);
            is_fullscreen = false;
        }
    }
}