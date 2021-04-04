#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <cmath>
#include <QObject>
#include <fstream>
#include "../level/level_common.h"

enum edit_mode {
    sel,
    draw
};

class Level : public QObject
{
    Q_OBJECT //just for using signal to access console
public:
    Level();
    void select_wall (float x, float y);
    void add_wall ();
    void delete_wall ();
    void delete_wall (wall wall);
    int save_level (std::ofstream &outfile);
    int load_level (std::ifstream &infile);

    std::vector<wall> walls;
    wall selected_wall;
    const float level_x = LEVEL_X;
    const float level_y = LEVEL_Y;
    const float cell_size = CELL_SIZE;
private:
    int wall_is_present (wall wall);
signals:
    void print_console (const std::string &s);
};

#endif // LEVEL_H
