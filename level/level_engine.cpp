#include "../level/level_engine.h"
#include <iostream>

Level::Level ()
{

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

int Level::load_level (const char *filename)
{
    std::ifstream infile;
    infile.open(filename, infile.binary | infile.in);
    if (!infile.is_open()) {
        std::cerr << "FAILED to open '" << filename << "'for read";
        return ERR_FILEOPEN;
    }
    int ret = load_level_common (walls, infile);
    switch (ret) {
    case ERR_FILETYPE:
        std::cerr << "ERROR: wrong filetype or major version";
        break;
    case ERR_VERSION:
        std::cerr << "ERROR: wrong version";
        break;
    default:
        break;
    }
    infile.close ();
    return ret;
}