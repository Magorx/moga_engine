#include "engine.h"
#include "hotkey_bind.h"

#include "overall_redactor_reactions.h"

RedactorEngine::RedactorEngine(const char  *window_name,
                               const size_t screen_width,
                               const size_t screen_height,
                               const size_t pixel_size) :
MogaEngine(window_name, screen_width, screen_height, pixel_size),
tool_manager(new ToolManager(visual->get_renderer()))
{
    t_Brush *t_brush         = new t_Brush(tool_manager);
    t_Eraser *t_eraser       = new t_Eraser(tool_manager);
    t_ColorPipett *t_pipette = new t_ColorPipett(tool_manager);

    tool_manager->add_tool(t_brush, HotkeyBind::brush);
    tool_manager->add_tool(t_eraser, HotkeyBind::eraser);
    tool_manager->add_tool(t_pipette, HotkeyBind::pipette);

    tool_manager->set_active_tool(HotkeyBind::brush);

    main_view->e_key_down.add(new ToolManagerHotkeys(tool_manager), false);
    main_view->e_scroll.add(new ToolManagerScrollShiftToolSize(tool_manager), false);
}
