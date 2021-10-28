#pragma once


#include "tool.h"

#include "visual/renderer/renderer.h"

class Canvas;

class ToolManager {
    Renderer *renderer;

    Canvas *active_canvas;
    RGBA draw_color;

    std::vector<Tool*> tools;
    Tool *active_tool;

public:

    ToolManager(Renderer *renderer);

    void add_tool(Tool *tool);
    void set_active_tool(size_t idx);
    void update_active_tool();

    void set_active_canvas(Canvas *canvas);

    void set_renderer(Renderer *renderer_) { renderer = renderer_; }
};
