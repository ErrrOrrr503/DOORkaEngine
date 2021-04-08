#include "level.h"

void Level::select_wall (float x, float y)
{
    if (x - round (x / cell_size) * cell_size < delta_ && x - round (x / cell_size) * cell_size > - delta_) {
        selected_wall.x1 = selected_wall.x2 = round (x / cell_size) * cell_size;
        selected_wall.y1 = floor (y / cell_size) * cell_size;
        selected_wall.y2 = (floor (y / cell_size) + 1) * cell_size;
    }
    if (y - round (y / cell_size) * cell_size < delta_ && y - round (y / cell_size) * cell_size > - delta_) {
        selected_wall.y1 = selected_wall.y2 = round (y / cell_size) * cell_size;
        selected_wall.x1 = floor (x / cell_size) * cell_size;
        selected_wall.x2 = (floor (x / cell_size) + 1) * cell_size;
    }
    selected_wall.zlo1 = selected_wall.zlo2 = 0;
    selected_wall.zhi1 = selected_wall.zhi2 = DEF_WALL_HEIGHT;
}

void Level::add_wall ()
{
    if (wall_is_present (selected_wall) < 0)
        walls.push_back(selected_wall);
}

void Level::delete_wall ()
{
    int pos = wall_is_present (selected_wall);
    if (pos < 0)
        return;
    walls.erase (walls.begin () + pos);
}

void Level::delete_wall (const wall& wall)
{
    int pos = wall_is_present (wall);
    if (pos < 0)
        return;
    walls.erase (walls.begin () + pos);
}

int Level::wall_is_present (const wall& wall)
{
    for (size_t i = 0; i < walls.size (); i++) {
        if (walls[i].x1 == wall.x1 && walls[i].y1 == wall.y1 &&
            walls[i].x2 == wall.x2 && walls[i].y2 == wall.y2)
            return i;
    }
    return -1;
}

int Level::save_level (std::ofstream &outfile)
{
    level_fileheader fileheader;
    fileheader.walls_size = sizeof (wall) * walls.size ();
    outfile.write ((char *) &fileheader, sizeof (level_fileheader));
    outfile.write ((char *) &walls[0], fileheader.walls_size);
    outfile.flush ();
    emit print_console ("Level saved. Header: " + std::to_string (sizeof (level_fileheader)) +
                        " bytes. Walls: " + std::to_string (fileheader.walls_size) + " bytes.");
    return 0;
}

int Level::load_level (std::ifstream &infile)
{
    int ret = load_level_common (walls, infile);
    switch (ret) {
    case ERR_FILETYPE:
        emit print_console ("ERROR: wrong filetype or major version");
        break;
    case ERR_VERSION:
        emit print_console ("ERROR: wrong version");
        break;
    default:
        break;
    }
    emit print_console ("loading " + std::to_string (walls.size () * sizeof (wall)) + " bytes to walls[]");
    selected_wall = {};
    return ret;
}

void Level::set_delta (float value)
{
    this->delta_ = value;
}
