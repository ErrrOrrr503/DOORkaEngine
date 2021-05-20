#include "render.h"

//crunch... BF crunch for BF cringe... to use in c-callback of glfw
//well gCamera is only visible in render.cpp so it is not that global. And it can be altered by only render members.
//Maybe that's not that awful...
static player_position *gPos;
static camera *gCamera; 

Textures::~Textures ()
{
    if (texture_names_list_ != nullptr)
        delete[] texture_names_list_;
}

int Textures::init_gl12 (const Level &level)
{
    texture_names_list_ = new GLuint[level.texture_list.size ()];
    glGenTextures ((GLsizei) level.texture_list.size (), texture_names_list_);
    for (size_t i = 0; i < level.texture_list.size (); i++) {
        int width = 0, height = 0;
        std::cout << "Opening '" << textures_path_ + level.texture_list[i] << "'" << std::endl;
        GLubyte *raw_texture = SOIL_load_image ((textures_path_ + level.texture_list[i]).c_str (), &width, &height, 0, SOIL_LOAD_RGBA);
        if (raw_texture == NULL) {
            std::cerr << "Failed to load texture: '" << textures_path_ + level.texture_list[i] << "'" << std::endl;
            continue;
        }
        texture_resolutions_.push_back (texture_res(width, height));
        // bind
        glBindTexture (GL_TEXTURE_2D, texture_names_list_[i]);
        // setup binded
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_mag_mapping_method_);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_mapping_method_);
        // and generate texture and mipmap levels
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_texture);
        glGenerateTextureMipmap (texture_names_list_[i]);
        SOIL_free_image_data (raw_texture);
    }
    return 0;
}

GLuint Textures::operator[] (size_t i)
{
    return texture_names_list_[i];
}

Render::Render (const Level &level)
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
    gPos = new player_position (window_, level, glm::vec3 (level.start_position[0] / CELL_SIZE, level.start_position[1] / CELL_SIZE, level.start_position[2] / CELL_SIZE),
                                                glm::vec3 (level.start_direction[0], level.start_direction[1], level.start_direction[2]));
    gPos->setMouseSensitivity(0.005f);
    gCamera = new camera(window_, gPos);
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_MULTISAMPLE);
    if (textures_.init_gl12 (level)) {
        status_ = err_texture;
    }
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
    gPos->relocateView ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadMatrixf (glm::value_ptr(gCamera->getViewMatrix()));
    glMatrixMode (GL_PROJECTION);
    glLoadMatrixf (glm::value_ptr(gCamera->getPerspMatrix()));
    glColor3f (1.0f, 1.0f, 0.1f);
    //now camera is set and we can draw
    for (size_t i = 0; i < level_->walls.size (); i++) {
        if (!level_->walls[i].is_colored)
            continue;
        glColor3f (level_->walls[i].color[0], level_->walls[i].color[1], level_->walls[i].color[2]);
  
        if (level_->walls[i].texture_index != -1) {
            glEnable (GL_TEXTURE_2D);
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // replace wall color by texture
            glBindTexture (GL_TEXTURE_2D, textures_[level_->walls[i].texture_index]);
        }
        
        glBegin (GL_TRIANGLE_STRIP);
            glTexCoord2f (0, 1); // left floor
                glVertex3f (level_->walls[i].x1 / CELL_SIZE, level_->walls[i].y1 / CELL_SIZE, level_->walls[i].zlo1 / CELL_SIZE);
            glTexCoord2f (0, 0); // left ceiling
                glVertex3f (level_->walls[i].x1 / CELL_SIZE, level_->walls[i].y1 / CELL_SIZE, level_->walls[i].zhi1 / CELL_SIZE);
            glTexCoord2f (1, 1); // right floor
                glVertex3f (level_->walls[i].x2 / CELL_SIZE, level_->walls[i].y2 / CELL_SIZE, level_->walls[i].zlo2 / CELL_SIZE);
            glTexCoord2f (1, 0); // right ceiling
                glVertex3f (level_->walls[i].x2 / CELL_SIZE, level_->walls[i].y2 / CELL_SIZE, level_->walls[i].zhi2 / CELL_SIZE);
        glEnd ();
        
        glDisable (GL_TEXTURE_2D);
    }

    gPos->acquireDeltaTime();
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
    gPos->rotateViewMouse ((GLfloat) (xpos - lastx), (GLfloat) (ypos - lasty));
    lastx = xpos;
    lasty = ypos;
}


void Render::setCamera (glm::vec3 locationv, glm::vec3 directionv)
{
    gPos->setLocation (locationv);
    gPos->setDirection (directionv);
}

void Render::misc_keyboard_input (GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
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
            glm::vec3 pre_resize_direction = gPos->getDirection ();
            glfwSetWindowMonitor (window, NULL, windowed_xpos, windowed_ypos, windowed_width, windowed_height, GLFW_DONT_CARE);
            gPos->setDirection (pre_resize_direction);
            is_fullscreen = false;
        }
    }
}