#include "level.h"

void Level::select_wall (float x, float y)
{
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
            selected_wall = &walls[i];
            selected_wall->zlo1 = selected_wall->zlo2 = 0;
            selected_wall->zhi1 = selected_wall->zhi2 = DEF_WALL_HEIGHT;
            selected_wall->color[0] = wall_color[0];
            selected_wall->color[1] = wall_color[1];
            selected_wall->color[2] = wall_color[2];
            selected_wall->is_colored = true;
            selected_wall->texture_index = cur_texture_index;
            return;
        }
    }
    selected_wall = nullptr; // no matching wall - reset selection
}

void Level::unselect_wall (float x, float y)
{
    select_wall (x, y);
    if (selected_wall == nullptr)
        return;
    selected_wall->color[0] = DEF_WALL_COLOR_R;
    selected_wall->color[1] = DEF_WALL_COLOR_G;
    selected_wall->color[2] = DEF_WALL_COLOR_B;
    selected_wall->is_colored = false;
}

void Level::add_wall_trio (float x, float y)
{
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

void Level::add_wall_trio_clipping (float x, float y)
{
    if ((x - round (x / cell_size) * cell_size < delta_ && x - round (x / cell_size) * cell_size > - delta_) &&
        (y - round (y / cell_size) * cell_size < delta_ && y - round (y / cell_size) * cell_size > - delta_))
    {
        x = round (x / cell_size) * cell_size;
        y = round (y / cell_size) * cell_size;
        add_wall_trio (x, y);
    }
}

void Level::add_wall (float x1, float y1, float x2, float y2)
{
    wall temp_wall (x1, y1, x2, y2, false);
    walls.push_back (temp_wall);
}

void Level::delete_wall ()
{
    int pos = wall_is_present (*selected_wall);
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
    std::vector<uint16_t> texture_list_description;
    uint64_t data_decompressed_size = 0, texture_list_size;
    for (size_t i = 0; i < texture_list.size (); i++) {
        texture_list_description.push_back (strlen (texture_list[i].c_str()));
        data_decompressed_size += texture_list_description[i];
    }
    texture_list_size = data_decompressed_size;
    data_decompressed_size += sizeof (wall) * walls.size ();
    data_decompressed_size += sizeof (uint16_t) * texture_list_description.size();
    level_fileheader fileheader;
    fileheader.walls_size = sizeof (wall) * walls.size ();
    fileheader.texture_list_description_size = sizeof (uint16_t) * texture_list_description.size();
    fileheader.prev_prev_prev_x = prev_prev_prev_x;
    fileheader.prev_prev_prev_y = prev_prev_prev_y;
    fileheader.prev_prev_x = prev_prev_x;
    fileheader.prev_prev_y = prev_prev_y;
    fileheader.prev_x = prev_x;
    fileheader.prev_y = prev_y;
    for (int i = 0; i < 3; i++) {
        fileheader.start_direction[i] = start_direction[i];
        fileheader.start_position[i] = start_position[i];
    }
    //data preparation for compression
    char *data_decompressed = new char[data_decompressed_size];
    uint64_t offset = 0;
    //walls
    memcpy (data_decompressed + offset, &walls[0], fileheader.walls_size);
    offset += fileheader.walls_size;
    //texture_list_description
    memcpy (data_decompressed + offset, &texture_list_description[0], fileheader.texture_list_description_size);
    offset += fileheader.texture_list_description_size;
    //texture_list
    for (size_t i = 0; i < texture_list.size (); i++) {
        memcpy (data_decompressed + offset, texture_list[i].c_str(), texture_list_description[i]);
        offset += texture_list_description[i];
    }
    emit print_console ("Preparing data finished: size = " + std::to_string (data_decompressed_size));
    //compression
    std::vector<pos_t> data_compressed = compress(data_decompressed, data_decompressed_size);
    fileheader.data_codon_count = data_compressed.size();
    unsigned char* data_packed = nullptr;
    size_t data_pck_size = pack(data_compressed, &data_packed);
    fileheader.data_compressed_size = data_pck_size;
    fileheader.data_decompressed_size = data_decompressed_size;
    //write
    outfile.write ((char *) &fileheader, sizeof (level_fileheader));
    outfile.write ((char *) data_packed, data_pck_size);
    //outfile.write (data_decompressed, data_decompressed_size);
    delete[] data_decompressed;
    outfile.flush ();
    delete [] data_packed;
    emit print_console ("Level saved. Header: " + std::to_string (sizeof (level_fileheader)) +
                        " bytes. Walls: " + std::to_string (fileheader.walls_size) + " bytes." +
                        "\n           texture_list + description: " + std::to_string (texture_list_size + fileheader.texture_list_description_size) +
                        " bytes. Codon count (all data) = " + std::to_string(fileheader.data_codon_count));
    return 0;
}

int Level::load_level (std::ifstream &infile)
{
    level_fileheader fileheader;
    int ret = load_level_common (fileheader, walls, texture_list, infile);
    switch (ret) {
    case ERR_FILETYPE:
        emit print_console ("ERROR: wrong filetype or major version");
        return ret;
    case ERR_VERSION:
        emit print_console ("ERROR: wrong version");
        return ret;
    case ERR_COMPRESS:
        emit print_console ("ERROR: compression failed");
        return ret;
    case ERR_DECOMPRESS:
        emit print_console ("ERROR: decompression failed");
        return ret;
    case 0:
        break;
    default:
        emit print_console ("ERROR: unknown error");
        return ret;
    }
    emit print_console ("loading " + std::to_string (walls.size () * sizeof (wall)) + " bytes to walls[]");
    emit print_console ("loading " + std::to_string (texture_list.size () * sizeof (wall)) + " bytes to texture_list[]");
    selected_wall = nullptr;
    prev_prev_prev_x = fileheader.prev_prev_prev_x;
    prev_prev_prev_y = fileheader.prev_prev_prev_y;
    prev_prev_x = fileheader.prev_prev_x;
    prev_prev_y = fileheader.prev_prev_y;
    prev_x = fileheader.prev_x;
    prev_y = fileheader.prev_y;
    for (int i = 0; i < 3; i++) {
        start_direction[i] = fileheader.start_direction[i];
        start_position[i] = fileheader.start_position[i];
    }
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

void Level::ctrl_z ()
{
    //remove 2 last walls and 1 dot
    if (walls.size () <= 3 || dots.size () < 3)
        return;
    walls.pop_back ();
    walls.pop_back ();
    dots.pop_back ();
    prev_x = dots[dots.size () - 1].x;
    prev_y = dots[dots.size () - 1].y;
    prev_prev_x = dots[dots.size () - 2].x;
    prev_prev_y = dots[dots.size () - 2].y;
    prev_prev_prev_x = dots[dots.size () - 3].x;
    prev_prev_prev_y = dots[dots.size () - 3].y;
    if (trig_side_mode != both_sides) {
        std::swap (prev_prev_x, prev_x);
        std::swap (prev_prev_y, prev_y);
    }
}

void Level::select_texture (const std::string tex_filename)
{
    for (size_t i = 0; i < texture_list.size (); i++) {
        if (tex_filename == texture_list[i]) {
            cur_texture_index = i;
            return;
        }
    }
    texture_list.push_back (tex_filename);
    cur_texture_index = texture_list.size () - 1;
}

void Level::set_start_position (const float x, const float y, const float z)
{
    start_position[0] = x;
    start_position[1] = y;
    start_position[2] = z;
}

void Level::set_start_direction (const float x, const float y, const float z)
{
    start_position[0] = x;
    start_position[1] = y;
    start_position[2] = z;
}
