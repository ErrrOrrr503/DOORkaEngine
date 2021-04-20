#ifndef LEVEL_COMMON_H
#define LEVEL_COMMON_H
//! \file level_common.h
#include <cinttypes>
#include <vector>
#include <fstream>
#include <cstring>
#include <filesystem>
#include "compress.h"
namespace fs = std::filesystem;

/*!
 * \details Description file for level storage format.
 * Covers both file format and its representation in editor program.
 * file structure (in binary):
 *     <level_header><wall_section><other>
*/

//! Describe pls.
#define FILETYPE_DESCR_LEN 20

//<ERROR_CODES>
//! Describe pls.
#define ERR_FILETYPE -1
//! Describe pls.
#define ERR_VERSION 1
//! Describe pls.
#define ERR_FILEOPEN 2
//! Describe pls.
//</ERROR_CODES>

//<LEVEL_DIMENSIONS for coordinate system>
#define LEVEL_X 10000
#define LEVEL_Y 10000
#define CELL_SIZE 50
#define DEF_WALL_HEIGHT 50
#define DEF_WALL_COLOR_R 0.2
#define DEF_WALL_COLOR_G 0.8
#define DEF_WALL_COLOR_B 0.0
//</LEVEL_DIMENSIONS for coordinate system>

struct level_fileheader {
//common data:
    char filetype[FILETYPE_DESCR_LEN] = "DOORkaEngine_v00002";
    uint32_t version = 1;
    uint64_t walls_size; // in bytes
    uint64_t other_size;
//editor only data:
    float prev_x = 0, prev_y = 0;
    float prev_prev_x = 0, prev_prev_y = 0;
    float prev_prev_prev_x = 0, prev_prev_prev_y = 0;
// compressor only data
    uint64_t walls_codon_count;
};

/*!
 * \brief The wall struct
 * \details Holds properties of a wall as a single object.
 * In other words, it describes both of wall's visible sides.
 * x1, y1, - coordinates of {point with smaller x and smaller y coordinates? correct if I'm wrong} Correction: only with smaller x is possible
 * x2, y2 - bigger coordinates.
 * zlow, zhi - bottom and top z coordinates of a wall.
 * For now zlow1 = zlow2, zhi1 = zhi2 - these should be equal.
 * color - color vector that will be displayes if no texture loaded.
 */
struct wall {
    float x1 = 0, y1 = 0, zlo1 = 0, zhi1 = DEF_WALL_HEIGHT;
    float x2 = 0, y2 = 0, zlo2 = 0, zhi2 = DEF_WALL_HEIGHT;
    float color[3] = {DEF_WALL_COLOR_R, DEF_WALL_COLOR_G, DEF_WALL_COLOR_B};
    bool is_colored = false;
};

/*!
 * \brief Read file and load level data into memory.
 * \param walls Walls vecror where walls data is stored.
 * \param infile Input file stream.
 * \return 0 or non-zero error code.
 * \details Walls vector should be initialized but not necessarily pre-allocated.
 * If stored, all data in the vector will be destroyed and replaced by level data.
 */
int load_level_common (std::vector<wall> &walls, std::ifstream &infile, size_t file_size);
int load_level_common (level_fileheader &fileheader, std::vector<wall> &walls, std::ifstream &infile, size_t file_size);

#endif // LEVEL_COMMON_H
