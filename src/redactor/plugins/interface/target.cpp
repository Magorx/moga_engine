#include "target.h"

#include "engine/Resources.h"
#include "engine/moga_engine.h"


Target::Target(Layer *layer, bool to_delete_layer, bool to_flip) :
layer(layer),
to_delete_layer(to_delete_layer),
to_flip(to_flip)
{}

Target::Target(Vec2d size) :
layer(nullptr)
{
    auto renderer = App.engine->visual->get_renderer();
    if (!renderer) return;
    
    layer = new Layer(renderer, nullptr, size);
    to_delete_layer = true;
    to_flip = true;
}

Target::Target(const char *path) :
layer(nullptr)
{
    auto renderer = App.engine->visual->get_renderer();
    if (!renderer) return;
    
    layer = new Layer(renderer, path);
    to_delete_layer = true;
    to_flip = true;
}

Target::~Target() {
    if (to_delete_layer) delete layer;
}


P::RenderTarget *Target::get_copy() const {
    return new Target(layer->copy(), true);
}

P::Vec2s Target::get_size() const {
    return {(size_t) layer->size.x(), (size_t) layer->size.y()};
}

P::RGBA Target::Target::get_pixel(size_t x, size_t y) const {
    return (P::RGBA) layer->get_pixel_color({(double) x, (double) y}).ui32;
}

void Target::set_pixel(size_t x, size_t y, P::RGBA color) {
    auto img = layer->copy_to_image();
    RGBA rgba = color.ui32;
    img->setPixel(x, y, {rgba.r, rgba.g, rgba.b, rgba.a});
    layer->copy_from(img);
}

P::RGBA *Target::get_pixels() {
    auto img = layer->copy_to_image();
    auto size = img->getSize();
    
    P::RGBA *data = new P::RGBA[size.x * size.y];
    memcpy(data, img->getPixelsPtr(), size.x * size.y * sizeof(P::RGBA));
    
    return data;
}

void Target::clear(P::RGBA color) {
    layer->clear(color.ui32);
}

#define INIT_DRAW_OBJECTS_       \
auto renderer = layer->renderer; \

#define PROCESS_RMODE_(mode) \
do { auto rstate = renderer->get_rstate(); \
if (mode->blend == P::BlendMode::PPBM_COPY) rstate->rmode.blendMode = RBlend::none; \
if (mode->shader) {rstate->rmode.shader = (RShader*) mode->shader;} } while(0)

void Target::render_circle(P::Vec2f position, float radius, P::RGBA color, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_;
    Vec2d pos = {position.x, position.y};

    if (to_flip) {
        pos = layer->flip(pos);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_circle(pos, radius, color.ui32);
    renderer->pop_target();
}

void Target::render_line(P::Vec2f start, P::Vec2f end, P::RGBA color, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d from = {start.x, start.y};
    Vec2d to   = {end.x  , end.y  };

    if (to_flip) {
        from = layer->flip(from);
        to   = layer->flip(to  );
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_line(from, to, color.ui32);
    renderer->pop_target();
}

void Target::render_triangle(P::Vec2f p1_, P::Vec2f p2_, P::Vec2f p3_, P::RGBA color, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d p1 = {p1_.x, p1_.y};
    Vec2d p2 = {p2_.x, p2_.y};
    Vec2d p3 = {p3_.x, p3_.y};

    if (to_flip) {
        p1 = layer->flip(p1);
        p2 = layer->flip(p2);
        p3 = layer->flip(p3);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_triangle(p1, p2, p3, color.ui32);
    renderer->pop_target();
}

void Target::render_rectangle(P::Vec2f p1_, P::Vec2f p2_, P::RGBA color, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d p1 = {p1_.x, p1_.y};
    Vec2d p2 = {p2_.x, p2_.y};

    if (to_flip) {
        p1 = layer->flip(p1);
        p2 = layer->flip(p2);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_rectangle(p1, p2 - p1, color.ui32);
    renderer->pop_target();
}


void Target::render_texture(P::Vec2f position, const RenderTarget *texture, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_
    auto target = dynamic_cast<const Target*>(texture);
    auto to_draw = target->get_layer();

    bool to_flip = false;
    Vec2d pos = {position.x, position.y};
    if (this->to_flip ^ to_flip) {
        pos = layer->flip(pos);
        to_flip ^= true;
    }

    Vec2d shift = {(double) 0, - (double) target->get_size().y};
    
    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_texture(pos + shift, &to_draw->target->getTexture());
    renderer->pop_target();
}

void Target::render_pixels(P::Vec2f position, const P::RGBA *data, size_t width, size_t height, const P::RenderMode *render_mode) {
    INIT_DRAW_OBJECTS_

    bool to_flip = false;
    Vec2d pos = {position.x, position.y};
    if (this->to_flip ^ to_flip) {
        pos = layer->flip(pos);
        to_flip ^= true;
    }

    RTexture texture;
    texture.create(width, height);
    texture.update((uint8_t*) data);

    Vec2d shift = {(double) 0, - (double) height};

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_texture(pos + shift, &texture, to_flip);
    renderer->pop_target();
}

void Target::apply_shader(const P::Shader *shader) {

}

void Target::update(const P::RGBA *data) {
    layer->copy_from((RGBA*) data);
}
