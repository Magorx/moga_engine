#include "app_plugin_interface.h"
#include "redactor/engine.h"


RedactorPluginInterface::RedactorPluginInterface() {
    factory.target = new TargetFactory;
    factory.shader = new ShaderFactory;
    factory.widget = new WidgetFactory;

    feature_level = P::SHADER_SUPPORT;
}

RedactorPluginInterface::~RedactorPluginInterface() {

}

// extension
bool  RedactorPluginInterface::enable(std::string_view /*name*/) const {
    return false;
}

void *RedactorPluginInterface::get_func(std::string_view /*extension*/, std::string_view /*func*/) const {
    return nullptr;
}

void *RedactorPluginInterface::get_interface(const char */*extension*/, const char */*name*/) const {
    return nullptr;
}


// general
void RedactorPluginInterface::log(const char *fmt, ...) const {
    va_list args;
    va_start(args, fmt);
    logger.logv((int) Logger::Level::warning, "@@@@", "plugin", fmt, args);
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
