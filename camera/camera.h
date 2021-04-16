#ifndef DE_CAMERA_H
#define DE_CAMERA_H
//! @file camera.h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// This declaraion is required to make this class declaration.
// It won't compile if we include full declaration here.
class player_position;

/*!
 * @brief FPS camera class for DOORka engine.
 * @details Emulates a FPS camera that can be moved in a zero-Y plane.
 * View orientation can be changed with both keyboard and mouse.
 * Mouse and keys sensitivity can be adjusted.
 */
class camera
{
private:
    // Connected window variables
    GLFWwindow* window_;
    GLint window_width_ = 800;
    GLint window_height_ = 800; 
    GLfloat ratio_ = 1.0f;
    // Up vector
    glm::vec3 cam_up_ = glm::vec3(0.0f, 0.0f, 1.0f);
    // Projection matrix
    glm::mat4 perspective_ = glm::mat4();
    // Perspective view parameters
    GLfloat field_of_view_ = glm::radians(70.0f);
    GLfloat near_clipping_plane_ = 0.1f;
    GLfloat far_clipping_plane_ = 1000.0f;
    // This object is used to obtain and change camera position
    player_position* pos_;

public:
    /*!
     * @brief Basic construcror.
     * @param window Pointer to the window overlooked by camera
     * @details Creates a camera object with default parameters and binds it to window *window.
     */
    camera(GLFWwindow* window, player_position* position);
    /*!
     * @brief Default constructor.
     * @details Removed: a camera cannot exist without window connected to it.
     */
    camera() = delete;
    /*!
     * @brief Trivial destructor.
     */
    ~camera() = default;
    /*!
     * @brief Reset projection matrix so as to fit in new window geometry.
     * @param width New window width
     * @param height New window height
     * @details Call this function in window resize callback function \
     * to prevent objects in the screen from stretching.
     */
    void resetWindowSize(GLint width, GLint height);
    /*!
     * @brief Re-calculate perspective projection matrix with new values.
     * @param fov Field of view in degrees
     * @param near_cp Position of near clipping plane
     * @param far_cp Position of far clipping plane
     */
    void resetPerspMatrix(GLfloat fov, GLfloat near_cp, GLfloat far_cp);
    /*!
     * @brief Set new field of view and re-calculate projection matrix.
     * @param degrees New field of view in degrees
     */
    void setFOV(GLfloat degrees);
    /*!
     * @brief Set new far clipping plane and re-calculate projection matrix.
     * @param position Position of new far clipping plane
     */
    void setFarClippingPlane(GLfloat position);
    /*!
     * @brief Set new near clipping plane and re-calculate projection matrix.
     * @param position Position of new near clipping plane
     */
    void setNearClippingPlane(GLfloat position);
    /*!
     * @brief Get view matrix corresponding to the camera position.
     * @return Matrix value
     * @details Warning: it calculates the matrix each call. 
     * Consider using a variable when calling several in one frame.
     */
    glm::mat4 getViewMatrix();
    /*!
     * @brief Get perspective projection matrix.
     * @return Matrix value
     */
    glm::mat4 getPerspMatrix();

private:
    void calculatePerspective_();
};

#endif // DE_CAMERA_H
