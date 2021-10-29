#include "engine.h"


RedactorEngine::RedactorEngine(const char  *window_name,
                               const size_t screen_width,
                               const size_t screen_height,
                               const size_t pixel_size) :
MogaEngine(window_name, screen_width, screen_height, pixel_size),
tool_manager(new ToolManager(visual->get_renderer()))
{
    tool_manager->add_tool(new t_Brush(tool_manager));
    tool_manager->set_active_tool(0);
}
