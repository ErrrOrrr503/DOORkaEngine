#include "level.h"

void Level::select_wall (float x, float y)
{
    /*
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
    */
    for (size_t i = 0; i < walls.size (); i++) {
        if (x + delta_ < std::min (walls[i].x1, walls[i].x2) ||
            x - delta_ > std::max (walls[i].x1, walls[i].x2) ||
            y + delta_ < std::min (walls[i].y1, walls[i].y2) ||
            y - delta_ > std::max (walls[i].y1, walls[i].y2))
        {
            continue;
        }
        //now cursor is close enough to the wall and we can gust determine if it belongs to the line (not just wall)
        float x1 = walls[i].x1;
        float x2 = walls[i].x2;
        float y1 = walls[i].y1;
        float y2 = walls[i].y2;
        float square_distance = ((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1) * ((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1) / \
                                ((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
        if (square_distance > delta_ * delta_)
            continue;
        else {
            selected_wall.x1 = x1;
            selected_wall.x2 = x2;
            selected_wall.y1 = y1;
            selected_wall.y2 = y2;
            break;
        }
    }
    selected_wall.zlo1 = selected_wall.zlo2 = 0;
    selected_wall.zhi1 = selected_wall.zhi2 = DEF_WALL_HEIGHT;
}

void Level::add_wall_trio (float x, float y)
{
    /*
    if (wall_is_present (selected_wall) < 0)
        walls.push_back(selected_wall);
    */
    //check collision with other dot
    for (size_t i = 0; i < dots.size (); i++) {
        float square_distance = (x - dots[i].x) * (x - dots[i].x) + (y - dots[i].y) * (y - dots[i].y);
        if (square_distance < delta_ * delta_) {
            x = dots[i].x;
            y = dots[i].y;
            break;
        }
    }
    if ((x == prev_x && y == prev_y) ||
        (x == prev_prev_x && y == prev_prev_y))
    {
        return; //trying to add same wall
    }
    dot temp_dot = {x, y};
    dots.push_back (temp_dot);

    //check wall crossing / matching // FIXME it's hard...

    if (dots.size() >= 2) {
        add_wall (x, y, prev_x, prev_y);
        if (!(prev_prev_x == prev_x && prev_prev_y == prev_y))
            add_wall (x, y, prev_prev_x, prev_prev_y);
    }
    if (dots.size () == 2)
        add_wall (prev_prev_x, prev_prev_y, prev_x, prev_y);

    prev_prev_prev_x = prev_prev_x;
    prev_prev_prev_y = prev_prev_y;
    prev_prev_x = prev_x;
    prev_prev_y = prev_y;
    prev_x = x;
    prev_y = y;
    if (trig_side_mode != both_sides) {
        std::swap (prev_prev_x, prev_x);
        std::swap (prev_prev_y, prev_y);
    }
}

void Level::add_wall (float x1, float y1, float x2, float y2)
{
    wall temp_wall;
    if (x1 < x2) {
        temp_wall.x1 = x1;
        temp_wall.y1 = y1;
        temp_wall.x2 = x2;
        temp_wall.y2 = y2;
    }
    else {
        temp_wall.x1 = x2;
        temp_wall.y1 = y2;
        temp_wall.x2 = x1;
        temp_wall.y2 = y1;
    }
    walls.push_back (temp_wall);
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
    fileheader.prev_prev_prev_x = prev_prev_prev_x;
    fileheader.prev_prev_prev_y = prev_prev_prev_y;
    fileheader.prev_prev_x = prev_prev_x;
    fileheader.prev_prev_y = prev_prev_y;
    fileheader.prev_x = prev_x;
    fileheader.prev_y = prev_y;
    outfile.write ((char *) &fileheader, sizeof (level_fileheader));
    outfile.write ((char *) &walls[0], fileheader.walls_size);
    outfile.flush ();
    emit print_console ("Level saved. Header: " + std::to_string (sizeof (level_fileheader)) +
                        " bytes. Walls: " + std::to_string (fileheader.walls_size) + " bytes.");
    return 0;
}

int Level::load_level (std::ifstream &infile)
{
    level_fileheader fileheader;
    int ret = load_level_common (fileheader, walls, infile);
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
    prev_prev_prev_x = fileheader.prev_prev_prev_x;
    prev_prev_prev_y = fileheader.prev_prev_prev_y;
    prev_prev_x = fileheader.prev_prev_x;
    prev_prev_y = fileheader.prev_prev_y;
    prev_x = fileheader.prev_x;
    prev_y = fileheader.prev_y;
    return ret;
}

void Level::set_delta (float value)
{
    this->delta_ = value;
}

void Level::swap_triangle_sides ()
{
    std::swap (prev_x, prev_prev_prev_x);
    std::swap (prev_y, prev_prev_prev_y);
}
