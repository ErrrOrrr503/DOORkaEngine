

#include "../camera/camera.h"
#include "../level/level_engine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../camera_example/sphere.h"
using std::vector;

class bsp_tree_wall{
public:
    wall node_wall;
    bsp_tree_wall* front_bsp_tree;
    bsp_tree_wall* back_bsp_tree;
    bsp_tree_wall (const std::vector<wall> &level);
    void find_same_point(wall wall1, wall wall2, float* same_x, float* same_y);
    int check_point_position(wall splitter_wall, float point_x, float point_y);
    int check_wall_position(wall spliter_wall, wall checked_wall);
private:
} ;