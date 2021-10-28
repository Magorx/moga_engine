#include "canvas.h"


v_Canvas::v_Canvas(const ViewBody &body, Renderer *renderer, ToolManager *tool_manager) :
v_Highlighter(body)
{
    canvas = new Canvas(renderer, tool_manager, body.size);

    set_appearence(new AppearenceTexture(canvas->get_final_layer()->get_texture()));
}

v_Canvas::~v_Canvas() {
    delete appearence;
}
