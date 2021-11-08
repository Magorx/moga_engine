#pragma once

#include "utils/logger.h"
#include <vector>
#include <utility>

class Cursor;
class Line;
class LinePos;

/*
[some_sting ]
           ^ cursor.pos()
*/

class Line {
    std::vector<char>   data;
    Cursor cursor;

public:
    Line(size_t to_reserve = 10);

    int len() const;
    
    void insert_char(char c);
    void put_char(char c);

    void delete_char();
    void backspace_char();

    void cut(bool include_border = false);

    void fix_anchors();
    void free_anchors();
};


class LinePos {
    const Line *line;

public:
    int pos;

    LinePos(const Line *line, int pos) :
    line(line),
    pos(pos)
    {}

    LinePos &operator=(const LinePos &other) {
        line = other.line;
        pos = other.pos;
    }

    void move_r() {
        ++pos;
        pos = pos >= line->len() ? line->len() - 1 : pos;
    }

    void move_l() {
        --pos;
        pos = pos < 0 ? 0 : pos;
    }

    int operator()() const {
        return pos;
    }
};


struct Cursor {
    LinePos pos;
    LinePos anchor;

    bool anchor_fixed = false;


    Cursor(LinePos pos) :
    pos(pos),
    anchor(pos)
    {}

    void fix_anchor()  { anchor_fixed = true; }
    void free_anchor() { anchor_fixed = false; }

    void move_r() {
        pos.move_r();
        if (!anchor_fixed) {
            anchor = pos;
        }
    }

    void move_l() {
        pos.move_l();
        if (!anchor_fixed) {
            anchor = pos;
        }
    }

    void anchor_magnet() {
        if (pos() > anchor()) {
            pos = anchor;
        } else {
            anchor = pos;
        }
    }

    bool is_selection() {
        return pos() != anchor();
    }

};

Line::Line(size_t to_reserve = 10) :
data(),
cursor(LinePos(this, 0))
{}

int Line::len() const { return (int) data.size(); }

void Line::cut(bool include_border) {
    int from = cursor.pos();
    int to   = cursor.anchor();
    if (from > to) {
        std::swap(from, to);
    }

    for (int w = from, r = to + include_border; r < data.size(); ++w, ++r) {
        data[w] = data[r];
    }
    data.resize(data.size() - (from - to + include_border));

    cursor.anchor_magnet();
}

void Line::insert_char(char c) {
    cut();
    data.insert(data.begin() + cursor.pos(), c);
    cursor.move_r();
}

void Line::put_char(char c) {
    cut(true);
    data.insert(data.begin() + cursor.pos(), c);
    cursor.move_r();
}

void Line::delete_char() {
    cut(true);
}

void Line::backspace_char() {
    if (!cursor.pos()) return;

    if (cursor.is_selection()) {
        cut();
    } else {
        cursor.move_l();
        delete_char();
    }
}

void Line::fix_anchors() {
    cursor.fix_anchor();
}

void Line::free_anchors() {
    cursor.free_anchor();
}
