#ifndef DE_CAMERA_H
#define DE_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
private:
    // Connected window variables
    GLFWwindow* window_;
    GLint window_width_ = 800;
    GLint window_height_ = 800; 
    GLfloat ratio_ = 1.0f;
    // Camera position variables
    glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.1f);
    glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cam_up_ = glm::vec3(0.0f, 0.0f, 1.0f);
    GLfloat course_ = 0.0f;
    GLfloat pitch_ = 0.0f;
    // Camera velocity in unit/sec
    GLfloat lin_velocity_ = 4.0f;
    GLfloat ang_velocity_ = 2.0f;
    // Timing variables
    GLdouble elapsed_time_ = 0.0f;
    GLdouble last_frame_ = 0.0f;
    // Transform matrices
    glm::mat4 view_ = glm::mat4();
    glm::mat4 perspective_ = glm::mat4();
    // Perspective view parameters
    GLfloat field_of_view_ = glm::radians(70.0f);
    GLfloat near_clipping_plane_ = 0.1f;
    GLfloat far_clipping_plane_ = 1000.0f;

public:
    camera(GLFWwindow* window);
    camera(GLFWwindow* window, glm::vec3 start_positionv, glm::vec3 start_directionv);
    camera() = delete;
    ~camera() = default;
    void resetWindowSize(GLint width, GLint height);
    void relocateView();
    void resetPerspMatrix(GLfloat fov, GLfloat near_cp, GLfloat far_cp);
    void setFOV(GLfloat degrees);
    void setFarClippingPlane(GLfloat position);
    void setNearClippingPlane(GLfloat position);
    void setLinearVelocity(GLfloat value);
    void setAngularVelocity(GLfloat value);
    void setPosition(glm::vec3 positionv);
    void setDirection(glm::vec3 directionv);
    GLdouble acquireDeltaTime();
    glm::mat4 getViewMatrix();
    glm::mat4 getPerspMatrix();

private:
    void calculatePerspective_();
};

#endif // DE_CAMERA_H
