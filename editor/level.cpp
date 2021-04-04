#include "level.h"

Level::Level()
{

}

void Level::select_wall (float x, float y)
{
    const float delta = 10;
    if (x - round (x / cell_size) * cell_size < delta && x - round (x / cell_size) * cell_size > - delta) {
        selected_wall.a1[0] = selected_wall.a2[0] = round (x / cell_size) * cell_size;
        selected_wall.a1[1] = floor (y / cell_size) * cell_size;
        selected_wall.a2[1] = (floor (y / cell_size) + 1) * cell_size;
    }
    if (y - round (y / cell_size) * cell_size < delta && y - round (y / cell_size) * cell_size > - delta) {
        selected_wall.a1[1] = selected_wall.a2[1] = round (y / cell_size) * cell_size;
        selected_wall.a1[0] = floor (x / cell_size) * cell_size;
        selected_wall.a2[0] = (floor (x / cell_size) + 1) * cell_size;
    }
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

int Level::wall_is_present (wall wall)
{
    for (size_t i = 0; i < walls.size (); i++) {
        if (walls[i].a1[0] == wall.a1[0] && walls[i].a1[1] == wall.a1[1] &&
            walls[i].a2[0] == wall.a2[0] && walls[i].a2[1] == wall.a2[1])
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
