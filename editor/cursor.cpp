#include "cursor.h"

Cursor::Cursor()
{
    type = none;
    cur_x = 0;
    cur_y = 0;
}

void Cursor::set (float x_raw, float y_raw, float height, float width , float center_x, float center_y, float angle, float scale)
{
    pressed_x_raw = x_raw;
    pressed_y_raw = y_raw;
    pressed_x = pressed_x_raw - width / 2 + center_x;
    pressed_y = height / 2 - pressed_y_raw + center_y;
    this->scale = scale;
    antirotate (angle);
    antiscale(scale);
}

void Cursor::antirotate (float angle)
{
    trans_x = pressed_x * cos(angle * M_PI / 180) + pressed_y * sin(angle * M_PI / 180);
    trans_y = -pressed_x * sin(angle * M_PI / 180) + pressed_y * cos(angle * M_PI / 180);
    cur_x = trans_x * scale;
    cur_y = trans_y * scale;
}

void Cursor::antiscale (float scale)
{
    cur_x = trans_x * scale;
    cur_y = trans_y * scale;
    this->scale = scale;
}
