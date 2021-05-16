#include "level_common.h"
#include <iostream>

wall::wall (float new_x1, float new_y1, float new_x2, float new_y2){
    x1 = new_x1;
    y1 = new_y1;
    x2 = new_x2;
    y2 = new_y2;
    float norm = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    if ( (x1 - x2) >= 0 ){
        normal_y = (x1 - x2)/norm;
        normal_x = (y2 - y1)/norm;
    }
    else {
        normal_y = (x2 - x1)/norm;
        normal_x = (y1 - y2)/norm;
    }
    color[0] = DEF_WALL_COLOR_R;
    color[1] = DEF_WALL_COLOR_G;
    color[2] = DEF_WALL_COLOR_B;
    is_colored = true;
}

wall::wall (float new_x1, float new_y1, float new_x2, float new_y2, bool is_colored) {
    x1 = new_x1;
    y1 = new_y1;
    x2 = new_x2;
    y2 = new_y2;
    float norm = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    if ( (x1 - x2) >= 0 ){
        normal_y = (x1 - x2)/norm;
        normal_x = (y2 - y1)/norm;
    }
    else {
        normal_y = (x2 - x1)/norm;
        normal_x = (y1 - y2)/norm;
    }
    color[0] = DEF_WALL_COLOR_R;
    color[1] = DEF_WALL_COLOR_G;
    color[2] = DEF_WALL_COLOR_B;
    this->is_colored = is_colored;
}

wall::wall () {
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    zlo1 = 0;
    zlo2 = 0;
    zhi1 = DEF_WALL_HEIGHT;
    zhi2 = DEF_WALL_HEIGHT;
    normal_x = 0;
    normal_y = 0;
    color[0] = DEF_WALL_COLOR_R;
    color[1] = DEF_WALL_COLOR_G;
    color[2] = DEF_WALL_COLOR_B;
    texture_index = -1;
    is_colored = false;
}

int load_level_common (std::vector<wall> &walls, std::vector<std::string> &texture_list,
                       std::ifstream &infile)
{
    level_fileheader temp;
    return load_level_common (temp, walls, texture_list, infile);
}

int load_level_common (level_fileheader &fileheader, std::vector<wall> &walls, std::vector<std::string> &texture_list,
                       std::ifstream &infile)
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
    //decompress
    unsigned char* data_packed = new unsigned char[fileheader.data_compressed_size];
    infile.read((char *) data_packed, fileheader.data_compressed_size);
    pos_t* data_compressed = unpack(fileheader.data_codon_count, data_packed);
    delete[] data_packed;
    char* data_decompressed = new char[fileheader.data_decompressed_size];
    int state = decompress(data_compressed, fileheader.data_codon_count, data_decompressed);
    delete[] data_compressed;
    if (state != COMP_OK)
        return ERR_DECOMPRESS;
    //init structs
    size_t offset = 0;
    //walls
    walls.clear ();
    walls.resize (fileheader.walls_size / sizeof (wall));
    memcpy (&walls[0], data_decompressed + offset, fileheader.walls_size);
    offset += fileheader.walls_size;

    //from now crunch starts
    //texture_list_description
    std::vector<uint16_t> texture_list_description;
    texture_list_description.resize (fileheader.texture_list_description_size / sizeof (uint16_t));
    memcpy (&texture_list_description[0], data_decompressed + offset, fileheader.texture_list_description_size);
    offset += fileheader.texture_list_description_size;
    //texture_list
    texture_list.clear ();
    for (size_t i = 0; i < texture_list_description.size (); i++) {
        //assuming that std::string contains an array of N symbols and termination null is awful for cross-platform and not
        //switching to char*: file stores char* and load/save make convertions, however strlen(c_str) may be != size(): utf8, utf16, utf9146
        char *temp_str = new char[texture_list_description[i] + 1]; //terminating 0;
        memcpy (temp_str, data_decompressed + offset, texture_list_description[i] / sizeof(char));
        texture_list.push_back (std::string (temp_str));
        offset += texture_list_description[i] / sizeof(char);
        delete[] temp_str;
    }
    delete[] data_decompressed;
    return 0;
}
