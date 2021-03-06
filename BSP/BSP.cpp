#include "BSP.h"

namespace bsp
{

void find_same_point(const wall& wall1, const wall& wall2, float* same_x, float* same_y){
    float a11 = wall1.x1 - wall1.x2;
    float a21 = wall1.y1 - wall1.y2;
    float a12 = wall2.x1 - wall2.x2;
    float a22 = wall2.y1 - wall2.y2;
    float c1 = wall2.x1 - wall1.x1;
    float c2 = wall2.y1 - wall1.y1;
    float det = a11*a22 - a12*a21;
    float det1 = c1*a22 - a12*c2;
    float t_param = det1/det;
    float finded_x = wall1.x1 + t_param*(wall1.x1 - wall1.x2);
    float finded_y = wall1.y1 + t_param*(wall1.y1 - wall1.y2);
    *same_x = finded_x;
    *same_y = finded_y;
}

int check_point_position (const wall& splitter_wall, float point_x, float point_y) {
    float scalar_mul = splitter_wall.normal_x*(splitter_wall.x1 - point_x) + 
                       splitter_wall.normal_y*(splitter_wall.y1 - point_y);
    //std::cout << "scalar_mul = " << scalar_mul << std::endl;
    if (float_compare (scalar_mul, 0) == 1)
        return 1;
    if (float_compare (scalar_mul, 0) == -1)
        return -1;
    return 0;
}
int check_wall_position (const wall& splitter_wall, const wall& checked_wall){
    //fixme: parallel walls
    int first_point_position = check_point_position(splitter_wall, checked_wall.x1, checked_wall.y1);
    int second_point_position = check_point_position(splitter_wall, checked_wall.x2, checked_wall.y2);
    if (first_point_position == 0)
        first_point_position = second_point_position;
    if (second_point_position == 0)
        second_point_position = first_point_position;
    //std::cout << "fp = " << first_point_position << " sp = " << second_point_position << std::endl;
    if (first_point_position == 1 && second_point_position == 1)
        return 1; //one side
    if (first_point_position == -1 && second_point_position == -1)
        return -1; //other side
    if (first_point_position * second_point_position < 0)
        return 2; //cross
    return 3; // ambiguous
}

bsp_tree_wall::~bsp_tree_wall() {
    if (front_bsp_tree != nullptr)
        delete front_bsp_tree;
    if (back_bsp_tree != nullptr)
        delete back_bsp_tree;
}

bsp_tree_wall::bsp_tree_wall (const std::vector<wall> &level) {

    std::vector<wall> front_walls;
    std::vector<wall> back_walls;

    wall splitter_wall = level[0];
    bool visible_walls_started = splitter_wall.is_colored;
    for (size_t i = 1; i < level.size (); i++){
        if (!level[i].is_colored) {
            continue;
        }
        if (!visible_walls_started) {
            splitter_wall = level[i];
            visible_walls_started = 1;
            continue;
        }
        wall checked_wall = level[i];
        int wall_position = check_wall_position(splitter_wall, checked_wall);
        if (wall_position == 1)
            front_walls.push_back(checked_wall);
        if (wall_position == -1)
            back_walls.push_back(checked_wall);
        if (wall_position == 3) {
            std::cout << "Ambiguous, putting to both trees." << std::endl;
            back_walls.push_back(checked_wall);
            front_walls.push_back(checked_wall);
        }
        if (wall_position == 2){
            std::cout << "splitting: (" << checked_wall.x1 << ", " << checked_wall.y1 << ") ("
                                        << checked_wall.x2 << ", " << checked_wall.y2 << ")" << std::endl;
            float same_x;
            float same_y;
            find_same_point (splitter_wall, checked_wall, &same_x, &same_y);
            wall new_wall1 (checked_wall.x1, checked_wall.y1, same_x, same_y);
            wall new_wall2 (same_x, same_y, checked_wall.x2, checked_wall.y2);
            if (check_point_position(splitter_wall, checked_wall.x1, checked_wall.y1) == 1) {
                front_walls.push_back(new_wall1);
                back_walls.push_back(new_wall2);
            }
            if (check_point_position(splitter_wall, checked_wall.x1, checked_wall.y1) == -1) {
                front_walls.push_back(new_wall2);
                back_walls.push_back(new_wall1);
            }
            if (check_point_position(splitter_wall, checked_wall.x1, checked_wall.y1) == 0) {
                front_walls.push_back(new_wall2);
                back_walls.push_back(new_wall1);
                front_walls.push_back(new_wall1);
                back_walls.push_back(new_wall2);
            }
        }
    }
    std::cout << "adding: (" << splitter_wall.x1 << ", " << splitter_wall.y1 << ") ("
                             << splitter_wall.x2 << ", " << splitter_wall.y2 << ") "
              << "normal: (" << splitter_wall.normal_x << ", " << splitter_wall.normal_y << ") ";
    std::cout << "walls[] size is " << level.size () << std::endl;
    node_wall = splitter_wall;
    if (back_walls.size() != 0)
        back_bsp_tree = new bsp_tree_wall(back_walls);
    else
        back_bsp_tree = nullptr;

    if (front_walls.size() != 0)
        front_bsp_tree = new bsp_tree_wall(front_walls);
    else
        front_bsp_tree = nullptr;
}

int check_line_and_wall_same_point (const wall& wall0, float vecx1, float vecx2, float vecy1, float vecy2){
    return check_two_lines_collision(vecx1, vecy1, vecx2, vecy2, wall0.x1, wall0.y1, wall0.x2, wall0.y2);
}

int check_wall_collision (glm::vec2 &forbidden_direction, const wall& wall0, const glm::vec3 &vec1, const glm::vec3 &vec2){
    float scalar_mul = wall0.normal_x * (vec2.x - vec1.x) + wall0.normal_y * (vec2.y - vec1.y);
    float sign = 1.0f;
    if (scalar_mul < 0) // movement vector is in opposite direction with normal
        sign = -1.0f;
    //std::cout << "location (" << vec1.x << ", " << vec1.y << " new (" << vec2.x << ", " << vec2.y << std::endl;
    if (check_line_and_wall_same_point(wall0, vec1.x, vec1.x + sign * wall0.normal_x * epsilon, vec1.y, vec1.y + sign * wall0.normal_y * epsilon) ||
        check_line_and_wall_same_point(wall0, vec1.x, vec2.x, vec1.y, vec2.y)) {
        forbidden_direction = glm::vec2(sign * wall0.normal_x, sign * wall0.normal_y);
        return 1;
    }
    // collision with points (corners) is weaker than with walls, no increment collision count, just set forbidden_direction
    if (((vec1.x - wall0.x1) * (vec1.x - wall0.x1) + (vec1.y - wall0.y1) * (vec1.y - wall0.y1) < epsilon * epsilon) &&
        ((wall0.x1 - vec1.x) * (vec2.x - vec1.x) + (wall0.y1 - vec1.y) * (vec2.y - vec1.y)) > 0)
    {
        forbidden_direction = glm::normalize(glm::vec2(wall0.x1 - vec1.x, wall0.y1 - vec1.y));
        return 2;
    }
    if (((vec1.x - wall0.x2) * (vec1.x - wall0.x2) + (vec1.y - wall0.y2) * (vec1.y - wall0.y2) < epsilon * epsilon) &&
        ((wall0.x2 - vec1.x) * (vec2.x - vec1.x) + (wall0.y2 - vec1.y) * (vec2.y - vec1.y)) > 0) 
    {
        forbidden_direction = glm::normalize(glm::vec2(wall0.x2 - vec1.x, wall0.y2 - vec1.y));
        return 2;
    }
    return 0;
}        

int check_two_lines_collision(float line1_x1, float line1_y1, float line1_x2, float line1_y2, float line2_x1, float line2_y1, float line2_x2, float line2_y2){
    float normal_x = line1_y2 - line1_y1;
    float normal_y = line1_x1 - line1_x2;
    float scalar_mul1 = (line2_x1 - line1_x1)*normal_x + (line2_y1 - line1_y1)*normal_y;  
    float scalar_mul2 = (line2_x2 - line1_x1)*normal_x + (line2_y2 - line1_y1)*normal_y; 
    normal_x = line2_y2 - line2_y1;
    normal_y = line2_x1 - line2_x2;
    float scalar_mul3 = (line1_x1 - line2_x1)*normal_x + (line1_y1 - line2_y1)*normal_y;  
    float scalar_mul4 = (line1_x2 - line2_x1)*normal_x + (line1_y2 - line2_y1)*normal_y;
    if ((scalar_mul1 * scalar_mul2 < 0) && (scalar_mul3 * scalar_mul4 < 0))
        return 1;
    return 0;
}

int bsp_tree_wall::check_level_collision (const glm::vec3 &pos, glm::vec3 &new_pos)
{
    wall temp_collised_wall;
    int collised_walls_num = 0;
    check_level_collision (pos, new_pos, temp_collised_wall, collised_walls_num);
    if (collised_walls_num == 1)
        check_level_collision (pos, new_pos, temp_collised_wall, collised_walls_num);
    return 0;
}

int bsp_tree_wall::check_level_collision (const glm::vec3 &pos, glm::vec3 &new_pos, wall &collised_wall, int &collised_walls_num){
    glm::vec2 forbidden_direction;
    int ret;
    if ((ret = check_wall_collision (forbidden_direction, node_wall, pos, new_pos))) {
        //std::cout << "COLLISION! with wall (" << node_wall.x1 << ", " << node_wall.y1 << ") ("
        //                                      << node_wall.x2 << ", " << node_wall.y2 << ")" << std::endl;
        if (ret == 1) {
            if (collised_walls_num == 0) { // first collision
                collised_walls_num++;
                collised_wall = node_wall;
            }
            else { // second true or false collision
                int is_other_wall = float_compare (collised_wall.x1, node_wall.x1) | float_compare (collised_wall.y1, node_wall.y1) |
                                    float_compare (collised_wall.x2, node_wall.x2) | float_compare (collised_wall.y2, node_wall.y2);
                if (is_other_wall) { // second true collision
                    collised_walls_num++;
                }
                // false collision is ignored
            }
        }
        if (collised_walls_num == 2) {
            new_pos = pos;
            return 0;
        }

        //no collision yet or false dual wall collision

        glm::vec3 allowed_direction (forbidden_direction.y, -forbidden_direction.x, 0);
        glm::vec3 delta (new_pos.x - pos.x, new_pos.y - pos.y, new_pos.z - pos.z);
        glm::vec3 allowed_delta = allowed_direction * (delta.x * allowed_direction.x + delta.y * allowed_direction.y);
        new_pos = pos + allowed_delta;

       // std::cout << "forbidden_direction (" << forbidden_direction.x << ", " << forbidden_direction.y << ")"
       //           << "allowed_direction (" << allowed_direction.x << ", " << allowed_direction.y << ")" << std::endl;
       // std::cout << "delta (" << delta.x << ", " << delta.y << ") allowed_delta (" << allowed_delta.x << ", " << allowed_delta.y << ")" << std::endl;
    }
    else {

    }
    
    int position_flag = check_point_position(node_wall, pos.x, pos.y);
    if (position_flag == 1) {
        if (front_bsp_tree != nullptr)
            front_bsp_tree -> check_level_collision(pos, new_pos, collised_wall, collised_walls_num);
    }
    if (position_flag == -1) {
        if (back_bsp_tree != nullptr)
            back_bsp_tree -> check_level_collision(pos, new_pos, collised_wall, collised_walls_num);
    }
    
    if (position_flag == 0) { // nearly impossible due to epsilon
        std::cout << "Warning: ambiguous player position!" << std::endl;
        if (back_bsp_tree != nullptr)
            back_bsp_tree -> check_level_collision(pos, new_pos, collised_wall, collised_walls_num);
        if (front_bsp_tree != nullptr)
            front_bsp_tree -> check_level_collision(pos, new_pos, collised_wall, collised_walls_num);
    }
    return 0;
}

int float_compare (float a1, float a2) {
    if (a1 - a2 > 0.00001)
        return 1;
    if (a1 - a2 < -0.00001)
        return -1;
    return 0;
}

int float_compare (float a1, float a2, float precision) {
    if (a1 - a2 > precision)
        return 1;
    if (a1 - a2 < -precision)
        return -1;
    return 0;
}

}