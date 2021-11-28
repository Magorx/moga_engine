#include "app_plugin_interface.h"

#include "redactor/engine.h"


namespace appintr {

#define INIT_DRAW_OBJECTS_                                   \
auto tool_manager = App.app_engine->get_tool_manager(); if (!tool_manager) return; \
auto canvas = tool_manager->get_active_canvas();  if (!canvas) return; \
Layer *layer = nullptr; \
if (render_mode->draw_policy == PDrawPolicy::PPDP_ACTIVE) layer = canvas->get_active_layer(); else canvas->get_draw_layer(); if (!layer) return;        \
auto renderer     = App.app_engine->visual->get_renderer(); if (!renderer) return; \

#define PROCESS_RMODE_(mode) \
do { auto rstate = renderer->get_rstate(); \
if (mode->blend == PBlendMode::PPBM_COPY) rstate->rmode.blendMode = RBlend::none; \
if (mode->shader) rstate->rmode.shader = (RShader*) mode->shader; } while(0)


void render_circle(PVec2f position, float radius, PRGBA color, const PRenderMode *render_mode) {
    INIT_DRAW_OBJECTS_;

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_circle({position.x, position.y}, radius, color.ui32);
    renderer->pop_target();
}

void render_line(PVec2f start, PVec2f end, PRGBA color, const PRenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_line({start.x, start.y}, {end.x, end.y}, color.ui32);
    renderer->pop_target();
}

void render_triangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_triangle({p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}, color.ui32);
    renderer->pop_target();
}

void render_rectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_rectangle({p1.x, p1.y}, {p2.x - p1.x, p2.y - p1.y}, color.ui32);
    renderer->pop_target();
}

void render_pixels(PVec2f position, const PRGBA *data, size_t width, size_t height, const PRenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_circle({position.x, position.y}, 50, {255, 0, 0});
    renderer->pop_target();
}

#undef INIT_DRAW_OBJECTS_
#undef PROCESS_RMODE_

double general_get_absolute_time() {
    return App.engine->get_cur_time();
}

PRGBA general_get_color() {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return {0, 0, 0, 0};

    return tool_manager->get_draw_color().ui32;
}

float general_get_size() {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return 0;

    return tool_manager->get_size();
}

void general_release_pixels(PRGBA *pixels) {
    delete pixels;
}

void general_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger.log("@@@@", "plugin", fmt, args);
    va_end(args);
}

PRGBA *target_get_pixels() {
    auto tool_manager = App.app_engine->get_tool_manager(); if (!tool_manager) return nullptr;
    auto canvas = tool_manager->get_active_canvas(); if (!canvas) return nullptr;
    auto layer = canvas->get_active_layer(); if (!layer) return nullptr;

    RImage img = layer->get_target()->getTexture().copyToImage();
    auto img_pixels = img.getPixelsPtr();
    auto size = img.getSize();

    PRGBA *pixels = new PRGBA[size.x * size.y];
    memcpy(pixels, img_pixels, size.x * size.y);

    return pixels;
}

void target_get_size(size_t *width, size_t *height) {
    auto tool_manager = App.app_engine->get_tool_manager(); if (!tool_manager) return;
    auto canvas = tool_manager->get_active_canvas(); if (!canvas) return;
    auto layer = canvas->get_active_layer(); if (!layer) return;

    auto size = layer->get_target()->getSize();

    if (width) {
        *width = size.x;
    }
    if (height) {
        *height = size.y;
    }
}

bool  extensions_enable(const char *) {
    return false;
}

void* extensions_get_func(const char *) {
    return nullptr;
}

void init(PAppInterface *interface) {
    if (!interface) {
        return;
    }

    interface->std_version = 0;
    interface->reserved = nullptr;

    interface->general.feature_level = (PFeatureLevel) 0;
    interface->general.get_absolute_time = &general_get_absolute_time;
    interface->general.get_color         = &general_get_color; 
    interface->general.get_size          = &general_get_size;
    interface->general.release_pixels    = &general_release_pixels;
    interface->general.log               = &general_log;

    interface->render.circle    = &render_circle;
    interface->render.line      = &render_line;
    interface->render.triangle  = &render_triangle;
    interface->render.rectangle = &render_rectangle;
    interface->render.pixels    = &render_pixels;

    interface->extensions.enable   = &extensions_enable;
    interface->extensions.get_func = &extensions_get_func;
    
    interface->target.get_pixels = &target_get_pixels;
    interface->target.get_size   = &target_get_size;
}

}
