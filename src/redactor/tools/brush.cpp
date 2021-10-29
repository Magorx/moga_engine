#include "brush.h"
#include "tool_manager.h"


 void t_Brush::draw_point(const Vec2d &pos) {
    if (!draw_layer || !renderer) {
        return;
    }

    renderer->push_target(draw_layer->get_target());
    renderer->draw_circle(pos, size, draw_color);
    renderer->pop_target();
}

t_Brush::t_Brush(Tool *manager) :
Tool(manager)
{

}

void t_Brush::on_mouse_down(const Vec2d &pos) {
    Tool::on_mouse_down(pos);

    draw_point(pos);
}

void t_Brush::on_mouse_up(const Vec2d &pos) {
    Tool::on_mouse_up(pos);
}

void t_Brush::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    Tool::on_mouse_move(from, to);

    if (!mouse_down) return;

    draw_point(from);
    draw_point(to);
}
