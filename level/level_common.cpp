#include "level_common.h"
#include <iostream>

int load_level_common (std::vector<wall> &walls, std::ifstream &infile, size_t file_size)
{
    level_fileheader temp;
    return load_level_common (temp, walls, infile, file_size);
}

int load_level_common (level_fileheader &fileheader, std::vector<wall> &walls, std::ifstream &infile, size_t file_size)
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
    size_t data_size = file_size - sizeof (level_fileheader);
    unsigned char* walls_packed = new unsigned char[data_size];
    infile.read((char *) walls_packed, data_size);
    pos_t* walls_compressed = unpack(fileheader.walls_codon_count, walls_packed);
    delete [] walls_packed;
    walls.clear ();
    walls.resize (fileheader.walls_size / sizeof (wall));
    int state = decompress(walls_compressed, fileheader.walls_codon_count, (char *) &walls[0]);
    return state;
}
