#ifndef LEVEL_COMMON_H
#define LEVEL_COMMON_H

#include <cinttypes>
#include <vector>
#include <fstream>
#include <cstring>

/* file structure (in binary):
   <level_header><wall_section><other>
*/
#define FILETYPE_DESCR_LEN 16

//<ERROR_CODES>
#define ERR_FILETYPE -1
#define ERR_VERSION 1
#define ERR_FILEOPEN 2
//</ERROR_CODES>

//<LEVEL_DIMENSIONS for coordinate system>
#define LEVEL_X 10000
#define LEVEL_Y 10000
#define CELL_SIZE 50
//</LEVEL_DIMENSIONS for coordinate system>

struct level_fileheader {
    char filetype[FILETYPE_DESCR_LEN] = "DOORkaEngine_v0";
    uint32_t version = 0;
    uint64_t walls_size; // in bytes
    uint64_t other_size;
};

struct wall {
    float a1[4]; // x y zlow zhigh
    float a2[4];
    float color[3];
};

int load_level_common (std::vector<wall> &walls, std::ifstream &infile);

#endif // LEVEL_COMMON_H