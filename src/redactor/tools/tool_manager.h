#pragma once


#include "redactor/canvas.h"
#include "tool.h"

#include "visual/renderer/renderer.h"


class ToolManager {
    Renderer *renderer;

    Canvas *active_canvas;
    RGBA draw_color;

    std::vector<Tool*> tools;
    Tool *active_tool;

    ToolManager(Renderer *renderer):
    renderer(renderer),
    active_canvas(nullptr),
    draw_color({0, 0, 0, 255}),
    tools()
    {
        
    }

    void add_tool(Tool *tool) {
        if (!tool) {
            return;
        }

        tools.push_back(tool);
    }

    void set_active_tool(size_t idx) {
        if (idx >= tools.size()) {
            return;
        }

        Tool *tool = tools[idx];
        if (active_tool == tool) {
            return;
        }

        active_tool->on_deactivate();
        tool->on_activate();

        active_tool = tool;
        update_active_tool();
    }

    void set_active_canvas(Canvas *canvas) {
        active_canvas = canvas;
    }

    void update_active_tool() {
        if (!active_tool) {
            return;
        }

        active_tool->set_draw_layer(active_canvas->get_active_layer());
        active_tool->set_draw_color(draw_color);
    }
};
