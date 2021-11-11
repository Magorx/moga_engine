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

    void move_home();
    void move_end();

    void select_all(bool to_fix_anchor = false);

    bool anchor_magnet();
    void pull_anchor();

    bool is_selection();

};
