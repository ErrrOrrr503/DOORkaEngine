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
    const float level_x = LEVEL_X; //to level common?
    const float level_y = LEVEL_Y;
    const float cell_size = CELL_SIZE;
private:
    int wall_is_present (wall wall);
};

#endif // LEVEL_H
