#include "plugin_tool.h"


t_Plugin::t_Plugin(Tool *manager, RedactorPlugin *plugin) :
Tool(manager, plugin ? plugin->get_lib()->name : nullptr),
plugin(plugin)
{}

void t_Plugin::on_mouse_down(const Vec2d &pos) {
    plugin->on_mouse_press(pos);
}

void t_Plugin::on_mouse_up(const Vec2d &pos) {
    plugin->on_mouse_release(pos);
}

void t_Plugin::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    plugin->on_mouse_move(from, to);
}

