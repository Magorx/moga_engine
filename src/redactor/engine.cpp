#include "engine.h"


RedactorEngine::RedactorEngine(const char  *window_name,
                               const size_t screen_width,
                               const size_t screen_height,
                               const size_t pixel_size) :
MogaEngine(window_name, screen_width, screen_height, pixel_size),
tool_manager(new ToolManager(visual->get_renderer()))
{
    t_Brush *t_brush = new t_Brush(tool_manager);
    t_brush->set_size(5);

    tool_manager->add_tool(t_brush);

    t_Eraser *t_eraser = new t_Eraser(tool_manager);
    t_eraser->set_size(5);

    tool_manager->add_tool(t_eraser);

    tool_manager->set_active_tool(1);
}
