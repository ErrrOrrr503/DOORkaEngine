#include "position.h"

player_position::player_position(GLFWwindow* window, const Level& level) :
    window_(window),
    location_(glm::vec3(0.0f, 0.0f, 0.1f)),
    direction_(glm::vec3(0.0f, 0.0f, -1.0f)),
    world_up_(glm::vec3(0.0f, 0.0f, 1.0f)),
    course_(0.0f),
    pitch_(0.0f),
    keyboard_rotation_enabled_(false),
    mouse_sensitivity_(0.01f),
    lin_velocity_(4.0f),
    ang_velocity_(2.0f),
    elapsed_time_(0.0f),
    last_frame_(0.0f),
    keys_()
{
    bsp_main_node = new bsp::bsp_tree_wall (level.walls);
}

player_position::player_position(GLFWwindow* window, const Level& level, glm::vec3 start_locationv, glm::vec3 start_directionv) :
    window_(window),
    location_(start_locationv),
    direction_(start_directionv),
    world_up_(glm::vec3(0.0f, 0.0f, 1.0f)),
    course_(0.0f),
    pitch_(0.0f),
    keyboard_rotation_enabled_(false),
    mouse_sensitivity_(0.01f),
    lin_velocity_(4.0f),
    ang_velocity_(2.0f),
    elapsed_time_(0.0f),
    last_frame_(0.0f),
    keys_()
{
    bsp_main_node = new bsp::bsp_tree_wall (level.walls);
}

player_position::~player_position ()
{
    delete bsp_main_node;
}

void player_position::relocateView()
{
    glm::vec3 new_location = location_;
    if (glfwGetKey(window_, keys_.mv_fwd) == GLFW_PRESS)
        new_location += lin_velocity_ * (GLfloat)elapsed_time_
        * glm::vec3(sinf(course_), cosf(course_), 0.0f);
    if (glfwGetKey(window_, keys_.mv_bck) == GLFW_PRESS)
        new_location -= lin_velocity_ * (GLfloat)elapsed_time_
        * glm::vec3(sinf(course_), cosf(course_), 0.0f);
    if (glfwGetKey(window_, keys_.mv_left) == GLFW_PRESS)
        new_location -= glm::normalize(glm::cross(direction_, world_up_))
        * lin_velocity_ * (GLfloat)elapsed_time_;
    if (glfwGetKey(window_, keys_.mv_right) == GLFW_PRESS)
        new_location += glm::normalize(glm::cross(direction_, world_up_))
        * lin_velocity_ * (GLfloat)elapsed_time_;

    new_location *= (float) CELL_SIZE;
    bsp_main_node->check_level_collision (location_ * (float) CELL_SIZE, new_location);
    location_ = new_location / (float) CELL_SIZE;
    
    if (!keyboard_rotation_enabled_)
        return;
    
    // else process rotation with keyboard and compute direction vector
    if (glfwGetKey(window_, keys_.rot_left) == GLFW_PRESS) {
        course_ -= ang_velocity_ * (GLfloat)elapsed_time_;
        if (course_ < -M_PI)
            course_ += (GLfloat) (M_PI + M_PI);
    }
    if (glfwGetKey(window_, keys_.rot_right) == GLFW_PRESS) {
        course_ += ang_velocity_ * (GLfloat)elapsed_time_;
        if (course_ > M_PI)
            course_ -= (GLfloat) (M_PI + M_PI);
    }
    if (glfwGetKey(window_, keys_.rot_up) == GLFW_PRESS) {
        if (pitch_ < M_PI_2 - MIN_ANGLE)
            pitch_ += (GLfloat) (ang_velocity_ * elapsed_time_);
    }
    if (glfwGetKey(window_, keys_.rot_down) == GLFW_PRESS) {
        if (pitch_ > -M_PI_2 + MIN_ANGLE)
            pitch_ -= (GLfloat) (ang_velocity_ * elapsed_time_);
    }

    direction_ = glm::vec3(
        sinf(course_) * cosf(pitch_),
        cosf(course_) * cosf(pitch_),
        sinf(pitch_)
    );
}

GLdouble player_position::acquireDeltaTime()
{
    GLdouble current_time = glfwGetTime();
    elapsed_time_ = current_time - last_frame_;
    last_frame_ = current_time;
    return elapsed_time_;
}

void player_position::setLinearVelocity(GLfloat value)
{
    lin_velocity_ = value;
}

void player_position::setAngularVelocity(GLfloat value)
{
    ang_velocity_ = value;
}

void player_position::setLocation(glm::vec3 positionv)
{
    location_ = positionv;
    relocateView();
}

void player_position::setDirection(glm::vec3 directionv)
{
    direction_ = directionv;
    relocateView();
}

glm::vec3 player_position::getDirection()
{
    return direction_;
}

glm::vec3 player_position::getLocation()
{
    return location_;
}

void player_position::setMouseSensitivity(GLfloat value)
{
    mouse_sensitivity_ = value;
}

void player_position::rotateViewMouse(GLfloat x_cursor_offset, GLfloat y_cursor_offset)
{
    course_ += x_cursor_offset * mouse_sensitivity_;
    pitch_ -= y_cursor_offset * mouse_sensitivity_;
    if (pitch_ > M_PI_2 - MIN_ANGLE)
        pitch_ = (GLfloat) (M_PI_2 - MIN_ANGLE);
    if (pitch_ < -M_PI_2 + MIN_ANGLE)
        pitch_ = (GLfloat) (-M_PI_2 + MIN_ANGLE);
    
    direction_ = glm::vec3(
        sinf(course_) * cosf(pitch_),
        cosf(course_) * cosf(pitch_),
        sinf(pitch_)
    );
}

void player_position::enableKeyboardRotation(bool state)
{
    keyboard_rotation_enabled_ = state;
}

player_position::key_container::key_container() :
    rot_up(GLFW_KEY_UP),
    rot_down(GLFW_KEY_DOWN),
    rot_left(GLFW_KEY_LEFT),
    rot_right(GLFW_KEY_RIGHT),
    mv_fwd(GLFW_KEY_W),
    mv_bck(GLFW_KEY_S),
    mv_left(GLFW_KEY_A),
    mv_right(GLFW_KEY_D)
{
}

player_position::key_container::key_container(
    GLint r_up, GLint r_down, GLint r_left, GLint r_right, 
    GLint m_fwd, GLint m_bck, GLint m_left, GLint m_right
) :
    rot_up(r_up),
    rot_down(r_down),
    rot_left(r_left),
    rot_right(r_right),
    mv_fwd(m_fwd),
    mv_bck(m_bck),
    mv_left(m_left),
    mv_right(m_right)
{
}

void player_position::setControls(const player_position::key_container& keys)
{
    keys_ = keys;
}

player_position::key_container player_position::getControls()
{
    return keys_;
}