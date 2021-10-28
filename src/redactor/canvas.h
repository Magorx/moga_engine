#pragma once


#include <vector>

#include "layer.h"
#include "tools/tool_manager.h"


class Canvas {
    Renderer *renderer;
    ToolManager *tool_manager;
    Vec2d size;

    std::vector<Layer*> layers;
    Layer *active_layer;

    Layer *draw_layer;
    Layer *final_layer;

public:
    Canvas(Renderer *renderer, ToolManager *tool_manager, Vec2d size):
    renderer(renderer),
    tool_manager(tool_manager),
    size(size)
    {
        if (!renderer || !tool_manager) {
            printf("Canvas was created without renderer or tool_manager, luckily to crush now\n");
        }

        active_layer = new Layer(renderer, size);
        layers.push_back(active_layer);

        draw_layer = new Layer(renderer, size);
        final_layer = new Layer(renderer, size);
    }

    ~Canvas() {
        for (auto layer : layers) {
            delete layer;
        }

        delete draw_layer;
    }

    Layer *get_active_layer() { return active_layer; }
    Layer *get_final_layer() { return final_layer; }
    Layer *get_draw_layer() { return draw_layer; }

    void flush_draw_to_active() {
        draw_layer->flush_to(active_layer);
    }

    void flush_to_final() {
        for (auto layer : layers) {
            layer->flush_to(final_layer);
        }
    }

    void new_layer() {
        active_layer = new Layer(renderer, size);
        layers.push_back(active_layer);
    }

    void on_mouse_down(const Vec2d &pos) {
        tool_manager->on_mouse_down(pos);
    }

    void on_mouse_up(const Vec2d &pos) {
        tool_manager->on_mouse_up(pos);
    }

    void on_mouse_move(const Vec2d &from, const Vec2d &to) {
        tool_manager->on_mouse_move(from, to);
    }
    
    void on_activate() {
        tool_manager->on_activate();
    }
    void on_deactivate() {
        tool_manager->on_deactivate();
    }

};
