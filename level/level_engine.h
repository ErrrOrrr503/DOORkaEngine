#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <cmath>
#include <fstream>
#include "../level/level_common.h"

class Level
{
public:
    Level();
    int load_level (const char *filename);

    std::vector<wall> walls;
    std::vector<std::string> texture_list;
    const float level_x = LEVEL_X; //to level common?
    const float level_y = LEVEL_Y;
    const float cell_size = CELL_SIZE;
    float start_position[3];
    float start_direction[3];
private:
    int wall_is_present (wall wall);
};

#endif // LEVEL_H
