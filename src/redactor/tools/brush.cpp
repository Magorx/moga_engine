#include "brush.h"
#include "tool_manager.h"


 void t_Brush::draw_point(Vec2d pos) {
    if (!draw_layer) {
        return;
    }

    
}

t_Brush::t_Brush(Renderer *renderer) :
Tool(renderer)
{

}

void t_Brush::on_mouse_down(const Vec2d &pos) {
    Tool::on_mouse_down(pos);

}

void t_Brush::on_mouse_up(const Vec2d &pos) {
    Tool::on_mouse_up(pos);
}

void t_Brush::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    Tool::on_mouse_move(from, to);

}