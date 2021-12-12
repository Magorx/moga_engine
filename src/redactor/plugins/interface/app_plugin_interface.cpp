#include "app_plugin_interface.h"
#include "redactor/engine.h"


RedactorPluginInterface::RedactorPluginInterface() :
plugin(nullptr)
{
    factory.target = new TargetFactory;
    factory.shader = new ShaderFactory;
    factory.widget = new WidgetFactory;

    feature_level = P::SHADER_SUPPORT | P::SETTINGS_SUPPORT;
}

RedactorPluginInterface::~RedactorPluginInterface() {

}

// extension
bool  RedactorPluginInterface::ext_enable(const char */*name*/) const {
    return false;
}

void *RedactorPluginInterface::ext_get_func(const char */*extension*/, const char */*func*/) const {
    return nullptr;
}

void *RedactorPluginInterface::ext_get_interface(const char */*extension*/, const char */*name*/) const {
    return nullptr;
}

void RedactorPluginInterface::ext_register_as(const char */*extension*/) const {
    
}


// general
void RedactorPluginInterface::log(const char *fmt, ...) const {
    const char *plugin_name = plugin ? plugin->lib->name : "a_plugin";

    va_list args;
    va_start(args, fmt);
    logger.logv((int) Logger::Level::warning, "plgn", plugin_name, fmt, args);
    va_end(args);
}

double RedactorPluginInterface::get_absolute_time() const {
    return App.engine->get_cur_time();
}


P::RGBA RedactorPluginInterface::get_color() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return {0, 0, 0, 0};

    return tool_manager->get_draw_color().ui32;
}

float RedactorPluginInterface::get_size() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return 0;

    return tool_manager->get_size();
}


// target
P::RenderTarget *RedactorPluginInterface::get_target() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return nullptr;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return nullptr;

    auto layer = canvas->get_active_layer();
    if (!layer) return nullptr;

    return new Target(layer, false, true);
}

P::RenderTarget *RedactorPluginInterface::get_preview() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return nullptr;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return nullptr;

    auto layer = canvas->get_draw_layer();
    if (!layer) return nullptr;

    return new Target(layer);
}

void RedactorPluginInterface::flush_preview() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return;

    canvas->flush_draw_to_active();
}

void RedactorPluginInterface::set_color(const P::RGBA &color) const {
    auto tm = App.app_engine->get_tool_manager(); if (!tm) return;

    tm->set_draw_color(color.ui32);
}

void RedactorPluginInterface::set_size(float size) const {
    auto tm = App.app_engine->get_tool_manager(); if (!tm) return;

    tm->set_size(size);
}

void RedactorPluginInterface::set_plugin(const RedactorPlugin *plugin_) {
    plugin = plugin_;
}