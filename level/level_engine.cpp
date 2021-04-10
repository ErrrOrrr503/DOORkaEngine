#include "../level/level_engine.h"
#include <iostream>

Level::Level ()
{

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