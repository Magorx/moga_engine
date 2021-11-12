#include "dialog_window.h"
#include "engine/moga_engine.h"


v_DialogWindow::v_DialogWindow(const char *name, const Vec2d &size, WindowStyle *style) :
v_Window(name, {0, size}, style, true)
{
    Resources.engine->centrize_view_body(body);
    get_header()->get_button_hide()->to_delete = true;
}
