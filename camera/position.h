#ifndef DE_PLAYER_POSITION_H
#define DE_PLAYER_POSITION_H
//! @file position.h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// Full declarations of all friend classes are required here.
#include "camera.h"
#include "../BSP/BSP.h"
#include "../level/level_engine.h"

class player_position
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
    GLFWwindow* window_;
    bsp::bsp_tree_wall* bsp_main_node;
    glm::vec3 location_ = glm::vec3(0.0f, 0.0f, 0.1f);
    glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 world_up_ = glm::vec3(0.0f, 0.0f, 1.0f);
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
    key_container keys_;

public:
    /*!
     * @brief Default constructor.
     * @details Removed: associated window required for initialization.
     */
    player_position() = delete;
    /*!
     * @brief Basic construcror.
     * @param window Pointer to the window overlooked by camera
     * @details Creates a camera object with default parameters and binds it to window *window.
     */
    player_position(GLFWwindow* window, const Level& level);
    /*!
     * Constructor with parameters.
     * @param window Pointer to the window overlooked by camera
     * @param start_positionv Initial camera position vector
     * @param start_directionv Initial camera direction vector
     * @details Creates a camera object connected to window *window with provided initial position and direction.
     */
    player_position(GLFWwindow* window, const Level& level, glm::vec3 start_locationv, glm::vec3 start_directionv);
    /*!
     * @brief Trivial destructor.
     */
    ~player_position();
    /*!
     * @brief Process keyboard and mouse input and re-calculate position and direction.
     */
    void relocateView();
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
    void setLocation(glm::vec3 positionv);
    /*!
     * @brief Get camera position.
     */
    glm::vec3 getLocation();
    /*!
     * @brief Set new view direction.
     * @param directionv New direction vector
     * @details Warning! This function does not calculate new angle used in view rotation.
     * Do not use it with enabled rotation control, there will be errors.
     */
    void setDirection(glm::vec3 directionv);
    /*!
     * @brief Get view direction.
     */
    glm::vec3 getDirection();
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
     * @brief Bind keys used to control camera.
     * @param keys Structure with key codes
     */
    void setControls(const player_position::key_container& keys);
    /*!
     * @brief Retrieve current control keybinding.
     * @details Use it when you need to change some control keys keeping the others.
     * Change fields of retrieved structure andd feed it to setControls().
     */
    player_position::key_container getControls();
    //! @brief See camera class (camera.h).
    friend glm::mat4 camera::getViewMatrix();
};

#endif // DE_PLAYER_POSITION_H