#include "level_common.h"

int load_level_common (std::vector<wall> &walls, std::ifstream &infile)
{
    level_fileheader temp;
    return load_level_common (temp, walls, infile);
}

int load_level_common (level_fileheader &fileheader, std::vector<wall> &walls, std::ifstream &infile)
{
    fileheader.filetype[0] = 0;
    level_fileheader reference_fileheader;
    infile.read ((char *) &fileheader, sizeof (level_fileheader));
    if (memcmp (fileheader.filetype, reference_fileheader.filetype, FILETYPE_DESCR_LEN))
        return ERR_FILETYPE;
    if (fileheader.version != reference_fileheader.version) {
        return ERR_VERSION;
        //fixme on compatible version
    }
    walls.clear ();
    walls.resize (fileheader.walls_size / sizeof (wall));
    infile.read ((char *) &walls[0], fileheader.walls_size);
    return 0;
}
