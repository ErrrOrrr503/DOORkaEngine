#ifndef LEVEL_H
#define LEVEL_H
//! \file level.h
#include <vector>
#include <cmath>
#include <QObject>
#include <fstream>
#include "../level/level_common.h"
enum edit_mode {
    sel,
    draw
};

/*!
 * \brief The Level class
 * \details Used to store all level data required for fast editing.
 */
class Level : public QObject
{
    Q_OBJECT //just for using signal to access console
    float delta_ = 10.0f;
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
     * \brief Push the wall in the selected_wall buffer into walls vector.
     */
    void add_wall ();
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

    //! Vector where all level wall objects are stored. Unsorted.
    std::vector<wall> walls;
    /*!
     * \brief Buffer for a selected wall data.
     * \details The selected wall may or may not be in walls vector.
     */
    wall selected_wall;
    //! Max number of cell "columns" on a level
    const float level_x = LEVEL_X;
    //! Max number of cell "rows" on a level
    const float level_y = LEVEL_Y;
    //! Size of a single cell in GL units.
    const float cell_size = CELL_SIZE;
private:
    int wall_is_present (const wall& wall);
signals:
    void print_console (const std::string &s);
};

#endif // LEVEL_H
