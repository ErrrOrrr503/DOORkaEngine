#ifndef LEVEL_COMMON_H
#define LEVEL_COMMON_H
//! \file level_common.h
#include <cinttypes>
#include <vector>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <iostream>
#include "compress.h"
#include <cmath>
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
#define ERR_DECOMPRESS 3
//! Describe pls.
#define ERR_COMPRESS 4
//! Describe pls.
//</ERROR_CODES>

//<LEVEL_DIMENSIONS for coordinate system>
#define LEVEL_X 10000
#define LEVEL_Y 10000
#define CELL_SIZE 50
#define DEF_WALL_HEIGHT 50
#define DEF_WALL_COLOR_R 0.2f
#define DEF_WALL_COLOR_G 0.8f
#define DEF_WALL_COLOR_B 0.0f
//</LEVEL_DIMENSIONS for coordinate system>

/*
 * Level file:  {{}} - compression
 *        <fileheader>
 * names: {{<walls>    <texture_list_description>   <texture_list>}}
 * types: {{<<wall>[]>        <uint16_t[]>         <char[], no \0>}}
 *
 * texture_list is a vector of texture filenames(strings) => each element has dynamic size and can't be read, so
 * texture_list_description is a vector of sizes (uint16_t) of corresponding elements in lexture_list
*/

struct level_fileheader {
//common data:
    char filetype[FILETYPE_DESCR_LEN] = "DOORkaEngine_v00004";
    uint32_t version = 4;
    uint64_t walls_size; // in bytes
    uint64_t texture_list_description_size;
//editor only data:
    float prev_x = 0, prev_y = 0;
    float prev_prev_x = 0, prev_prev_y = 0;
    float prev_prev_prev_x = 0, prev_prev_prev_y = 0;
// compressor only data
    uint64_t data_codon_count;
    uint64_t data_decompressed_size;
    uint64_t data_compressed_size;
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
    wall ();
    wall (float new_x1, float new_y1, float new_x2, float new_y2);
    wall (float new_x1, float new_y1, float new_x2, float new_y2, bool is_colored);
    float x1 = 0, y1 = 0, zlo1 = 0, zhi1 = DEF_WALL_HEIGHT;
    float x2 = 0, y2 = 0, zlo2 = 0, zhi2 = DEF_WALL_HEIGHT;
    float normal_x = 0, normal_y = 0;
    float color[3] = {DEF_WALL_COLOR_R, DEF_WALL_COLOR_G, DEF_WALL_COLOR_B};
    int32_t texture_index = -1;  // -1 no texture; 0-32767 texture index in
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
int load_level_common (std::vector<wall> &walls, std::vector<std::string> &texture_list,
                       std::ifstream &infile);
int load_level_common (level_fileheader &fileheader, std::vector<wall> &walls, std::vector<std::string> &texture_list,
                       std::ifstream &infile);

#endif // LEVEL_COMMON_H
