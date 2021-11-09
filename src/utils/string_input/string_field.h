#pragma once

#include "utils/logger.h"
#include <vector>
#include <utility>


class Line;

class LinePos {
    const Line *line;

public:
    int pos;

    LinePos(const Line *line, int pos);
    LinePos(const LinePos &line_pos);
    
    LinePos &operator=(const LinePos &other);

    void move_r();

    void move_l();

    inline int operator()() const {
        return pos;
    }
};

struct Cursor {
    LinePos pos;
    LinePos anchor;

    bool anchor_fixed = false;

    Cursor(LinePos pos);

    void fix_anchor();
    void free_anchor();

    void move_r();

    void move_l();

    void anchor_magnet();

    bool is_selection();

};

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

    void erase_next();
    void erase_prev();

    void cut(bool include_border = false);

    void cursor_l() { cursor.move_l(); }
    void cursor_r() { cursor.move_r(); }

    void fix_anchors();
    void free_anchors();

    inline const char *c_str() const { return &data[0]; }
    inline char *c_str() { return &data[0]; }

    inline void dump(char endline = '\n') const {
        int pos = cursor.pos();
        int anc = cursor.anchor();

        if (anc == pos) {
            printf("[");
            for (int i = 0; i < pos; ++i) {
                printf("%c", data[i]);
            }
            printf("|");
            for (int i = pos; i < len(); ++i) {
                printf("%c", data[i]);
            }
            printf("]%c", endline);
        } else {
            int mn = std::min(pos, anc);
            int mx = std::max(pos, anc);

            printf("[");
            for (int i = 0; i < mn; ++i) {
                printf("%c", data[i]);
            }
            printf("%c", mn == anc ? '{' : '|');
            for (int i = mn; i < mx; ++i) {
                printf("%c", data[i]);
            }
            printf("%c", mn == anc ? '|' : '}');
            for (int i = mx; i < len(); ++i) {
                printf("%c", data[i]);
            }
            printf("]%c", endline);
        }
    }
};

Cursor::Cursor(LinePos pos) :
pos(pos),
anchor(pos)
{}

void Cursor::fix_anchor()  { anchor_fixed = true; }
void Cursor::free_anchor() { anchor_fixed = false; }

void Cursor::move_r() {
    if (is_selection()) {
        if (pos() > anchor()) {
            anchor = pos;
            return;
        } else {
            
        }
    }

    pos.move_r();
    if (!anchor_fixed) {
        anchor = pos;
    }
}

void Cursor::move_l() {
    pos.move_l();
    if (!anchor_fixed) {
        anchor = pos;
    }
}

void Cursor::anchor_magnet() {
    if (pos() > anchor()) {
        pos = anchor;
    } else {
        anchor = pos;
    }
}

bool Cursor::is_selection() {
    return pos() != anchor();
}


Line::Line(size_t to_reserve) :
data(),
cursor(LinePos(this, 0))
{
    data.reserve(to_reserve);
}

int Line::len() const { return (int) data.size(); }

void Line::cut(bool include_border) {
    int from = cursor.pos();
    int to   = cursor.anchor();
    if (from > to) {
        std::swap(from, to);
    }

    for (int w = from, r = to + include_border; r < len(); ++w, ++r) {
        data[w] = data[r];
    }
    data.resize(data.size() - (to - from + include_border));

    cursor.anchor_magnet();
}

void Line::insert_char(char c) {
    cut();
    printf("ins on pos %d\n", cursor.pos());
    data.insert(data.begin() + cursor.pos(), c);

    if (cursor.pos() == len() - 1) {
        data.push_back('\0');
    }

    cursor.move_r();
}

void Line::put_char(char c) {
    cursor.move_l();
    cut(true);
    data.insert(data.begin() + cursor.pos(), c);
    // cursor.move_r();
}

void Line::erase_next() {
    if (cursor.is_selection()) {
        cut();
    } else {
        if (cursor.pos() == len() - 1) return;
        cut(true);
    }
}

void Line::erase_prev() {
    if (cursor.is_selection()) {
        cut();
    } else {
        if (!cursor.pos()) return;
        cursor.move_l();
        erase_next();
    }
}

void Line::fix_anchors() {
    cursor.fix_anchor();
}

void Line::free_anchors() {
    cursor.free_anchor();
}

LinePos::LinePos(const Line *line, int pos) :
line(line),
pos(pos)
{}

LinePos::LinePos(const LinePos &other) :
line(other.line),
pos(other.pos)
{}

LinePos &LinePos::operator=(const LinePos &other) {
    line = other.line;
    pos = other.pos;

    return *this;
}

void LinePos::move_r() {
    ++pos;
    pos = pos >= line->len() ? line->len() - 1 : pos;
}

void LinePos::move_l() {
    --pos;
    pos = pos < 0 ? 0 : pos;
}
