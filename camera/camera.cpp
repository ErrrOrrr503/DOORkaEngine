#include "camera.h"

camera::camera(GLFWwindow* window)
{
    window_ = window;
    glfwGetFramebufferSize (window, &window_width_, &window_height_);
    ratio_ = (GLfloat)window_width_ / (GLfloat)window_height_;
    calculatePerspective_();
    // А как узнать размер окна на старте программы?
    // Как вообще его узнать в произвольный момент? (by errrorrr: look upwards)
}

camera::camera(GLFWwindow* window, glm::vec3 start_positionv, glm::vec3 start_directionv)
{
    window_ = window;
    glfwGetFramebufferSize (window, &window_width_, &window_height_);
    ratio_ = (GLfloat)window_width_ / (GLfloat)window_height_;
    direction_ = start_directionv;
    position_ = start_positionv;
    calculatePerspective_();
}

void camera::resetWindowSize(GLint width, GLint height)
{
    this->window_width_ = width;
    this->window_height_ = height;
    ratio_ = (GLfloat)width / (GLfloat)height;
    calculatePerspective_();
}

void camera::relocateView()
{
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        position_ += lin_velocity_ * (GLfloat)elapsed_time_
        * glm::vec3(direction_.x, 0.0f, direction_.z);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        position_ -= lin_velocity_ * (GLfloat)elapsed_time_
        * glm::vec3(direction_.x, 0.0f, direction_.z);;
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        position_ -= glm::normalize(glm::cross(direction_, cam_up_))
        * lin_velocity_ * (GLfloat)elapsed_time_;
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        position_ += glm::normalize(glm::cross(direction_, cam_up_))
        * lin_velocity_ * (GLfloat)elapsed_time_;
    
    if (!keyboard_rotation_enabled_)
        return;
    
    // else process rotation with keyboard and compute direction vector
    if (glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS) {
        course_ -= ang_velocity_ * (GLfloat)elapsed_time_;
        if (course_ < -M_PI)
            course_ += M_PI + M_PI;
    }
    if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        course_ += ang_velocity_ * (GLfloat)elapsed_time_;
        if (course_ > M_PI)
            course_ -= M_PI + M_PI;
    }
    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS) {
        if (pitch_ < M_PI_2 - MIN_ANGLE)
            pitch_ += ang_velocity_ * (GLfloat)elapsed_time_;
    }
    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (pitch_ > -M_PI_2 + MIN_ANGLE)
            pitch_ -= ang_velocity_ * (GLfloat)elapsed_time_;
    }

    direction_ = glm::vec3(
        sinf(course_) * cosf(pitch_),
        sinf(pitch_),
        -cosf(course_) * cosf(pitch_)
    ); 
}

glm::mat4 camera::getViewMatrix()
{
    return glm::lookAt(position_, position_ + direction_, cam_up_);
}

glm::mat4 camera::getPerspMatrix()
{
    return perspective_;
}

GLdouble camera::acquireDeltaTime()
{
    GLdouble current_time = glfwGetTime();
    elapsed_time_ = current_time - last_frame_;
    last_frame_ = current_time;
    return elapsed_time_;
}

void camera::calculatePerspective_()
{
    perspective_ = glm::perspective<float>(
        field_of_view_,
        ratio_,
        near_clipping_plane_,
        far_clipping_plane_
    );
}

void camera::resetPerspMatrix(GLfloat fov, GLfloat near_cp, GLfloat far_cp)
{
    field_of_view_ = fov;
    near_clipping_plane_ = near_cp;
    far_clipping_plane_ = far_cp;
    calculatePerspective_();
}

void camera::setFOV(GLfloat degrees)
{
    field_of_view_ = degrees;
    calculatePerspective_();
}

void camera::setFarClippingPlane(GLfloat position)
{
    far_clipping_plane_ = position;
    calculatePerspective_();
}

void camera::setNearClippingPlane(GLfloat position)
{
    near_clipping_plane_ = position;
    calculatePerspective_();
}

void camera::setLinearVelocity(GLfloat value)
{
    lin_velocity_ = value;
}

void camera::setAngularVelocity(GLfloat value)
{
    ang_velocity_ = value;
}

void camera::setPosition(glm::vec3 positionv)
{
    position_ = positionv;
    relocateView();
}

void camera::setDirection(glm::vec3 directionv)
{
    direction_ = directionv;
    relocateView();
}

void camera::setMouseSensitivity(GLfloat value)
{
    mouse_sensitivity_ = value;
}

void camera::rotateViewMouse(GLfloat x_cursor_offset, GLfloat y_cursor_offset)
{
    course_ += x_cursor_offset * mouse_sensitivity_;
    pitch_ -= y_cursor_offset * mouse_sensitivity_;
    if (pitch_ > M_PI_2 - MIN_ANGLE)
        pitch_ = M_PI_2 - MIN_ANGLE;
    if (pitch_ < -M_PI_2 + MIN_ANGLE)
        pitch_ = -M_PI_2 + MIN_ANGLE;
    
    direction_ = glm::vec3(
        sinf(course_) * cosf(pitch_),
        sinf(pitch_),
        -cosf(course_) * cosf(pitch_)
    );
}

void camera::enableKeyboardRotation(bool state)
{
    keyboard_rotation_enabled_ = state;
}