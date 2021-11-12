#include "line_pos.h"
#include "line.h"

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

void LinePos::set(int idx) {
    idx = idx < 0 ? 0 : idx;
    idx = idx >= line->len() ? line->len() - 1 : idx;
    
    pos = idx;
}

void LinePos::move_r() {
    ++pos;
    pos = pos >= line->len() ? line->len() - 1 : pos;
}

void LinePos::move_l() {
    --pos;
    pos = pos < 0 ? 0 : pos;
}

void LinePos::move_home() {
    pos = 0;
}

void LinePos::move_end() {
    pos = line->len() - 1;
    pos = pos < 0 ? 0 : pos;
}

