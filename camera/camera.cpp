#include "camera.h"
#include "position.h"

camera::camera(GLFWwindow* window, player_position* position)
{
    window_ = window;
    pos_ = position;
    glfwGetFramebufferSize (window, &window_width_, &window_height_);
    ratio_ = (GLfloat)window_width_ / (GLfloat)window_height_;
    calculatePerspective_();
}

void camera::resetWindowSize(GLint width, GLint height)
{
    this->window_width_ = width;
    this->window_height_ = height;
    ratio_ = (GLfloat)width / (GLfloat)height;
    calculatePerspective_();
}

glm::mat4 camera::getViewMatrix()
{
    return glm::lookAt(
        pos_->location_,
        pos_->location_ + pos_->direction_,
        cam_up_
        );
}

glm::mat4 camera::getPerspMatrix()
{
    return perspective_;
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
