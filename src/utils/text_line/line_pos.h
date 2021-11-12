#pragma once


class Line;

class LinePos {
    const Line *line;

public:
    int pos;

    LinePos(const Line *line, int pos);
    LinePos(const LinePos &line_pos);
    
    LinePos &operator=(const LinePos &other);

    void set(int idx);

    void move_r();
    void move_l();

    void move_home();
    void move_end();

    inline int operator()() const {
        return pos;
    }
};
