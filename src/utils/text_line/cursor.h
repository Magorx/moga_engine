#pragma once


#include "line_pos.h"


struct Cursor {
    LinePos pos;
    LinePos anchor;

    bool anchor_fixed = false;

    Cursor(LinePos pos);

    void fix_anchor();
    void free_anchor();

    void move_r();

    void move_l();

    bool anchor_magnet();

    bool is_selection();

};
