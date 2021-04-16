#include "BSP.h"
void bsp_tree_wall::find_same_point(wall wall1, wall wall2, float* same_x, float* same_y){
    float a11 = wall1.x1 - wall1.x2;
    float a21 = wall1.y1 - wall1.y2;
    float a12 = wall2.x1 - wall2.x2;
    float a22 = wall2.y1 - wall2.y2;
    float c1 = wall2.x1 - wall1.x1;
    float c2 = wall2.y1 - wall1.y1;
    float det = a11*a22 - a12*a21;
    float det1 = c1*a22 - a12*c2;
    float t_param   = det1/det;
    float finded_x = wall1.x1 + t_param*(wall1.x1 - wall1.x2);
    float finded_y = wall1.y1 + t_param*(wall1.y1 - wall1.y2);
    *same_x = finded_x;
    *same_y = finded_y;
}
int bsp_tree_wall::check_point_position (wall splitter_wall, float point_x, float point_y){
    float normal_x = splitter_wall.y1 - splitter_wall.y2;
    float normal_y = splitter_wall.x2 - splitter_wall.x1;
    float scalar_mul = normal_x*(splitter_wall.x1 - point_x) + normal_y*(splitter_wall.y1 - point_y);
    if (scalar_mul >= 0)
        return 1;
    else
        return -1;

}
int bsp_tree_wall::check_wall_position (wall splitter_wall, wall checked_wall){
    int first_point_position = check_point_position(splitter_wall, checked_wall.x1, checked_wall.y1);
    int second_point_position = check_point_position(splitter_wall, checked_wall.x2, checked_wall.y2);
    if (first_point_position == 1 && second_point_position == 1)
        return 1;
    if (first_point_position == -1 && second_point_position == -1)
        return -1;
    if (first_point_position*second_point_position < 0)
        return 2;
    return 2;
}

bsp_tree_wall::bsp_tree_wall (const std::vector<wall> &level) {
    if (level.size() == 1){
        node_wall = level[0];
        front_bsp_tree = NULL;
        back_bsp_tree = NULL;
        return ;
    }

    vector<wall> front_walls;
    vector<wall> back_walls;
    wall splitter_wall = level[0];
    for (size_t i = 1; i < level.size(); ++i){
        wall checked_wall = level[i];
        int wall_position = check_wall_position(splitter_wall, checked_wall);
        if (wall_position == 1)
            front_walls.push_back(checked_wall);
        if (wall_position == -1)
            back_walls.push_back(checked_wall);
        if (wall_position == 2){
            wall new_wall1 = checked_wall;
            wall new_wall2 = checked_wall;
            float same_x;
            float same_y;
            find_same_point (splitter_wall, checked_wall, &same_x, &same_y);
            new_wall1. x1 = same_x;
            new_wall1. y1 = same_y;
            new_wall2. x2 = same_x;
            new_wall2. y2 = same_y;
            if (check_wall_position(splitter_wall, new_wall1) == 1){
                front_walls.push_back(new_wall1);
                back_walls.push_back(new_wall2);}
            else {
                front_walls.push_back(new_wall2);
                back_walls.push_back(new_wall1);
            } 

        
        }

    node_wall = splitter_wall;
    front_bsp_tree = new bsp_tree_wall(front_walls);
    back_bsp_tree = new bsp_tree_wall(back_walls);
    return;


    }


    


}

int main(){

}