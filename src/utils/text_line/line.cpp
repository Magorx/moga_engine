#include "line.h"


Line::Line(size_t to_reserve) :
data(),
cursor(LinePos(this, 0))
{
    data.resize(to_reserve, 0);
    data.resize(0);
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

void Line::put_str(const char *str) {
    if (cursor.is_selection()) {
        cut();
    }

    int l = strlen(str);
    for (int i = 0; i < l; ++i) {
        insert_char(str[i]);
    }
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

void Line::dump(char endline) const {
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