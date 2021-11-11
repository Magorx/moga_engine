#pragma once

#include "utils/logger.h"
#include <vector>
#include <utility>

#include "line_pos.h"
#include "cursor.h"

/*
[some_sting ]
           ^ cursor.pos()
*/

class Line {
    std::vector<char> data;
    Cursor cursor;

public:
    Line(size_t to_reserve = 10);

    int len() const;
    
    void insert_char(char c);
    void put_char(char c);

    void put_str(const char *str);

    void erase_next();
    void erase_prev();

    void cut(bool include_border = false);

    void cursor_l() { cursor.move_l(); }
    void cursor_r() { cursor.move_r(); }
    bool cursor_mag() { return cursor.anchor_magnet(); }

    int selection_l() { return std::min(cursor.pos(), cursor.anchor()); }
    int selection_r() { return std::max(cursor.pos(), cursor.anchor()); }

    int cursor_pos() { return cursor.pos(); }
    int anchor_pos() { return cursor.anchor(); }



    void fix_anchors();
    void free_anchors();

    inline const char *c_str() const { return &data[0]; }
    inline char *c_str() { return &data[0]; }

    void dump(char endline = '\n') const;
};
