#include <iostream>
#include <vector>
#include <cmath>
#include "../level/level_common.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//float pos_x = 0.0;
//float pos_y = 0.0;
//using std::vector;

namespace bsp
{

const float epsilon = 10.0f;

class bsp_tree_wall{
public:
    bsp_tree_wall(const std::vector<wall> &level);
    ~bsp_tree_wall();
    int check_level_collision (const glm::vec3 &pos, glm::vec3 &new_pos, wall &collised_wall, int &collized_walls_num);
    int check_level_collision (const glm::vec3 &pos, glm::vec3 &new_pos);

    wall node_wall;
    bsp_tree_wall* front_bsp_tree = nullptr;
    bsp_tree_wall* back_bsp_tree = nullptr;
private:
} ;


int check_two_lines_collision(float line1_x1, float line1_y1, float line1_x2, float line1_y2, float line2_x1, float line2_y1, float line2_x2, float line2_y2);
void find_same_point(const wall& wall1, const wall& wall2, float* same_x, float* same_y);
int check_point_position (const wall& splitter_wall, float point_x, float point_y);
int check_line_and_wall_same_point (const wall& wall0, float vecx1, float vecx2, float vecy1, float vecy2);
int check_wall_collision (glm::vec2 &forbidden_direction, const wall& wall0, const glm::vec3 &vec1, const glm::vec3 &vec2);
int check_two_lines_collision(float line1_x1, float line1_y1, float line1_x2, float line1_y2, float line2_x1, float line2_y1, float line2_x2, float line2_y2);
int float_compare (float a1, float a2);
int float_compare (float a1, float a2, float precision);

}