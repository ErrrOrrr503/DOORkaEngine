#ifndef DE_CAMERA_H
#define DE_CAMERA_H
//! @file camera.h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*!
 * @brief FPS camera class for DOORka engine.
 * @details Emulates a FPS camera that can be moved in a zero-Y plane.
 * View orientation can be changed with both keyboard and mouse.
 * Mouse and keys sensitivity can be adjusted.
 */
class camera
{
public:
    //! @brief Structure where keybindings for camera control are stored.
    struct key_container
    {
        GLint rot_up, rot_down, rot_left, rot_right;
        GLint mv_fwd, mv_bck, mv_left, mv_right;
        key_container();
        key_container(GLint r_up, GLint r_down, GLint r_left, GLint r_right,
            GLint m_fwd, GLint m_bck, GLint m_left, GLint m_right);
        ~key_container() = default;
    };

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
    bool keyboard_rotation_enabled_ = false;
    const GLfloat MIN_ANGLE = 1e-5f;
    GLfloat mouse_sensitivity_ = 0.01f;
    // Camera velocity in unit/sec
    GLfloat lin_velocity_ = 4.0f;
    GLfloat ang_velocity_ = 2.0f;
    // Timing variables
    GLdouble elapsed_time_ = 0.0f;
    GLdouble last_frame_ = 0.0f;
    // Projection matrix
    glm::mat4 perspective_ = glm::mat4();
    // Perspective view parameters
    GLfloat field_of_view_ = glm::radians(70.0f);
    GLfloat near_clipping_plane_ = 0.1f;
    GLfloat far_clipping_plane_ = 1000.0f;
    key_container keys_;

public:
    /*!
     * @brief Basic construcror.
     * @param window Pointer to the window overlooked by camera
     * @details Creates a camera object with default parameters and binds it to window *window.
     */
    camera(GLFWwindow* window);
    /*!
     * Constructor with parameters.
     * @param window Pointer to the window overlooked by camera
     * @param start_positionv Initial camera position vector
     * @param start_directionv Initial camera direction vector
     * @details Creates a camera object connected to window *window with provided initial position and direction.
     */
    camera(GLFWwindow* window, glm::vec3 start_positionv, glm::vec3 start_directionv);
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
     * @brief Process keyboard and mouse input and re-calculate view matrix.
     */
    void relocateView();
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
     * @brief Set velocity of linear movement in units per second.
     * @param value New velocity value
     */
    void setLinearVelocity(GLfloat value);
    /*!
     * @brief Set keyboard view rotation velocity in units per second.
     * @param value New angular velocity value
     */
    void setAngularVelocity(GLfloat value);
    /*!
     * @brief Set sensitivity of mouse positioning.
     * @param value Mouse sensitivity value
     */
    void setMouseSensitivity(GLfloat value);
    /*!
     * @brief Set new camera position.
     * @param positionv New position vector
     * @details The view direction remains unchanged.
     */
    void setPosition(glm::vec3 positionv);
    /*!
     * @brief Set new view direction.
     * @param directionv New direction vector
     * @details Warning! This function does not calculate new angle used in view rotation.
     * Do not use it with enabled rotation control, there will be errors.
     */
    void setDirection(glm::vec3 directionv);
    /*!
     * Enable or disable view rotation with keyboard.
     * @param state True to enable, false to disable
     */
    void enableKeyboardRotation(bool state);
    /*!
     * @brief Rotate view correspondently to mouse movement.
     * @param x_cursor_offset Horizontal cursor offset.
     * @param y_cursor_offset Vertical cursor offset.
     * @details Warning: offset is not an absolute cursor position value. 
     * It should be the difference in position from the previous frame.
     */
    void rotateViewMouse(GLfloat x_cursor_offset, GLfloat y_cursor_offset);
    /*!
     * @brief Calculate time used to render the last frame.
     * @return Delta time value
     * @details Updates inner time variable of the class and returns its value.
     */
    GLdouble acquireDeltaTime();
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
    /*!
     * @brief Bind keys used to control camera.
     * @param keys Structure with key codes
     */
    void setControls(const camera::key_container& keys);

private:
    void calculatePerspective_();
};

#endif // DE_CAMERA_H
