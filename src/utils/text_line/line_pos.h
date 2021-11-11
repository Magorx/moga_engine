#pragma once


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
