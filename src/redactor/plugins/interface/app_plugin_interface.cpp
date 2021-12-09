#include "app_plugin_interface.h"

#include "redactor/engine.h"


// void shader_apply(const P::RenderMode *render_mode) {
//     INIT_DRAW_OBJECTS_

//     layer->get_target()->setRepeated(false);

//     auto texture = &layer->get_target()->getTexture();

//     RRendTexture new_tx;
//     new_tx.create(texture->getSize().x, texture->getSize().y);
//     renderer->push_target(&new_tx);
//     PROCESS_RMODE_(render_mode);
//     renderer->get_rstate()->rmode.blendMode = RBlend::none;
//     renderer->get_rstate()->rmode.texture   = texture;
//     renderer->draw_texture({0, 0}, texture, true);
//     renderer->pop_target();

//     renderer->push_target(layer->get_target());

//     renderer->get_rstate()->rmode.blendMode = RBlend::none;
//     renderer->get_rstate()->rmode.texture   = &new_tx.getTexture();
//     renderer->draw_texture({0, 0}, &new_tx.getTexture(), true);
//     renderer->pop_target();

//     layer->get_target()->setRepeated(true);
// }

void shader_set_uniform_int(void *shader, const char *name, int  val) {
    if (!shader) return;

    RShader *rshader = (RShader*) shader;
    rshader->setUniform(name, val);
}

void shader_set_uniform_int_arr(void *shader, const char *name, int *val, size_t cnt) {
    if (!shader) return;

    for (size_t i = 0; i < cnt; ++i) {
        ((float*)val)[i] = (float) val[i];
    }

    RShader *rshader = (RShader*) shader;
    rshader->setUniformArray(name, (float*) val, cnt);

    for (size_t i = 0; i < cnt; ++i) {
        val[i] = (int) ((float*)val)[i];
    }
}

void shader_set_uniform_float(void *shader, const char *name, float  val) {
    if (!shader) return;

    RShader *rshader = (RShader*) shader;
    rshader->setUniform(name, val);
}

void shader_set_uniform_float_arr(void *shader, const char *name, float *val, size_t cnt) {
    if (!shader) return;

    RShader *rshader = (RShader*) shader;
    rshader->setUniformArray(name, val, cnt);
}

#undef INIT_DRAW_OBJECTS_
#undef PROCESS_RMODE_

// ============================================================================

RedactorPluginInterface::RedactorPluginInterface() {
    factory.target = new TargetFactory;
    factory.shader = new ShaderFactory;

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
