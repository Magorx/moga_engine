#pragma once


#include "tool.h"


class t_Brush : public Tool {
    void draw_point(const Vec2d &pos);

public:
    t_Brush(Tool *manager);

    virtual void on_mouse_down(const Vec2d &pos) override;
    virtual void on_mouse_up(const Vec2d &pos) override;
    virtual void on_mouse_move(const Vec2d &from, const Vec2d &to) override;
};
