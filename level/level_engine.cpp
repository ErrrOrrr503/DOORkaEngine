#include "../level/level_engine.h"

Level::Level ()
{

}

int Level::load_level (const char *filename)
{
    if(!fs::exists(filename)) {
        std::cerr << "FAILED to open '" << filename << "': file does not exist" << std::endl;
        return ERR_FILEOPEN;
    }
    std::ifstream infile;
    infile.open(filename, infile.binary | infile.in);
    if (!infile.is_open()) {
        std::cerr << "FAILED to open '" << filename << "' for read" << std::endl;
        return ERR_FILEOPEN;
    }
    int ret = load_level_common (walls, texture_list, infile);
    switch (ret) {
    case ERR_FILETYPE:
        std::cerr << "ERROR: wrong filetype or major version" << std::endl;
        break;
    case ERR_VERSION:
        std::cerr << "ERROR: wrong version" << std::endl;
        break;
    case 0:
        break;
    default:
        std::cerr << "ERROR: unknown error" << std::endl;
        break;
    }
    infile.close ();
    return ret;
}