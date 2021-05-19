#ifndef LEVEL_H
#define LEVEL_H
//! \file level.h
#include <vector>
#include <cmath>
#include <QObject>
#include <fstream>
#include "../level/level_common.h"
#include "../level/compress.h"

/*
enum edit_mode {
    sel,
    unsel,
    draw_clipping,
    draw,
    set_pos
};*/

enum triangle_side_mode {
    both_sides, //default
    one_side
};

struct dot {
    float x;
    float y;
};

/*!
 * \brief The Level class
 * \details Used to store all level data required for fast editing.
 */
class Level : public QObject
{
    Q_OBJECT //just for using signal to access console
    float delta_ = 20.0f;
public:
    //! \brief Default constructor.
    Level() = default;
    /*!
     * \brief Check whether the given coordinates are near to any wall and place it into selected_wall buffer.
     * \param x X-coordinate of the cursor
     * \param y Y - coordinate
     * \details The required proximity is determined by delta value (10.0 by default).
     * The more delta value, the more distance is allowed.
     */
    void select_wall (float x, float y);
    /*!
     * \brief Check whether the given coordinates are near to any wall and place it into selected_wall buffer.
     * \param x X-coordinate of the cursor
     * \param y Y - coordinate
     * \details The required proximity is determined by delta value (10.0 by default).
     * The more delta value, the more distance is allowed.
     */
    void unselect_wall (float x, float y);
    /*!
     * \brief Push the wall in the selected_wall buffer into walls vector.
     */
    void add_wall_trio (float x, float y);
    /*!
     * \brief Push the wall in the selected_wall buffer into walls vector.
     */
    void add_wall_trio_clipping (float x, float y);
    /*!
     * \brief Delete selected wall from the walls vector.
     */
    void delete_wall ();
    /*!
     * \brief Delete given wall from the walls vector.
     * \param A wall to delete.
     */
    void delete_wall (const wall& wall);
    /*!
     * \brief Write level down in our specified format.
     * \param outfile Open file stream to write down
     * \return Returns 0 or fails
     */
    int save_level (std::ofstream &outfile);
    /*!
     * \brief Read input file and load level if it is of suitable format.
     * \param infile Open input file stream
     * \return 0 or non-zero error code.
     */
    int load_level (std::ifstream &infile);
    /*!
     * \brief Change delta value.
     * \param value
     * \details Seee select_wall for further reference.
     */
    void set_delta (float value);
    /*!
     * \brief swaps triangle base for new triangle
     * \details swaps prev and prev_prev_prev
     */
    void swap_triangle_sides ();
    void ctrl_z ();
    void select_texture (const std::string tex_filename);
    void set_start_position (const float x, const float y, const float z);
    void set_start_direction (const float x, const float y, const float z);
    //! Vector where all level wall objects are stored. Unsorted.
    std::vector<wall> walls;
    /*!
     * \brief Buffer for a selected wall data.
     * \details The selected wall may or may not be in walls vector.
     */
    wall *selected_wall = nullptr;
    //! Max number of cell "columns" on a level
    const float level_x = LEVEL_X;
    //! Max number of cell "rows" on a level
    const float level_y = LEVEL_Y;
    //! Size of a single cell in GL units.
    const float cell_size = CELL_SIZE;
    float wall_color[3] = {DEF_WALL_COLOR_R, DEF_WALL_COLOR_G, DEF_WALL_COLOR_B};
    float prev_x = 0, prev_y = 0;
    float prev_prev_x = 0, prev_prev_y = 0;
    float prev_prev_prev_x = 0, prev_prev_prev_y = 0;
    float start_position[3] = {0, 0, CELL_SIZE / 2};
    float start_direction[3] = {1, 0, 0};
    triangle_side_mode trig_side_mode = one_side;
    int32_t cur_texture_index = -1;

private:
    int wall_is_present (const wall &wall);
    void add_wall (float x1, float y1, float x2, float y2);

    std::vector<std::string> texture_list;
    std::vector<dot> dots;

signals:
    void print_console (const std::string &s);
};

#endif // LEVEL_H
