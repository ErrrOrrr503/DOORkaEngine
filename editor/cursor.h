#ifndef CURSOR_H
#define CURSOR_H

#include <cmath>

enum edit_mode {
    plain,
    draw_mode,
    draw_clipping_mode,
    sel_mode,
    unsel_mode,
    set_pos_mode,
    none
    //enum forbids equal enumenators for different enums? strange, errors could be given on type resolving when ambigious
};

class Cursor
{
public:
    Cursor();
    ~Cursor() = default;
    edit_mode mode;
    void set (float x_raw, float y_raw, float height, float width, float center_x, float center_y, float angle, float scale);
    void antirotate (float angle);
    void antiscale (float scale);
    float cur_x;
    float cur_y;
    float pressed_x;
    float pressed_y;
    float trans_x;
    float trans_y;
    float scale;
    float pressed_x_raw;
    float pressed_y_raw;
};

#endif // CURSOR_H
