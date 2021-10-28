#include "tool_manager.h"
#include "redactor/canvas.h"


ToolManager::ToolManager(Renderer *renderer):
Tool(renderer),
active_canvas(nullptr),
draw_color({0, 0, 0, 255}),
tools()
{
    
}

void ToolManager::add_tool(Tool *tool) {
    if (!tool) {
        return;
    }

    tools.push_back(tool);
}

void ToolManager::set_active_tool(size_t idx) {
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

void ToolManager::set_active_canvas(Canvas *canvas) {
    active_canvas = canvas;
}

void ToolManager::update_active_tool() {
    if (!active_tool) {
        return;
    }

    active_tool->set_draw_layer(active_canvas->get_active_layer());
    active_tool->set_draw_color(draw_color);
    active_tool->set_draw_layer(active_canvas->get_draw_layer());
}

void ToolManager::on_mouse_down(const Vec2d &pos) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_down(pos);
    }
}

void ToolManager::on_mouse_up(const Vec2d &pos) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_up(pos);
    }
}

void ToolManager::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_move(from, to);
    }
}

void ToolManager::on_activate() {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_activate();
    }
}
void ToolManager::on_deactivate() {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_deactivate();
    }
}
