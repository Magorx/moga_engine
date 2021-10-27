#pragma once

#include <cstdio>

#include "redactor/layer.h"

#include "visual/renderer/renderer.h"


class Tool {
protected:
    Renderer *renderer;

    Layer *draw_layer;
    RGBA draw_color;

    bool mouse_down = false;
    Vec2d cur_pos = {0, 0};

    double size = 1;

public:
    Tool(Renderer *renderer) :
    renderer(renderer),
    draw_layer(nullptr),
    draw_color(0, 0, 0, 255)
    {}

    void set_draw_layer(Layer* layer) { draw_layer = layer; }
    void set_draw_color(RGBA color) { draw_color = color; }
    void set_renderer(Renderer *renderer_) { renderer = renderer_; }

    virtual void on_mouse_down(const Vec2d &pos) {
        mouse_down = true;
        cur_pos = pos;
    }

    virtual void on_mouse_up(const Vec2d &pos) {
        mouse_down = false;
        cur_pos = pos;
    }

    virtual void on_mouse_move(const Vec2d &from, const Vec2d &to) {
        cur_pos = to;
    }
    
    virtual void on_activate() {}
    virtual void on_deactivate() {}
};
