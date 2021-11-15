#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/renderer.h"

#include "utils/affected.h"


class Canvas;


struct Layer : public Affected<Layer> {
    Renderer *renderer;
    Canvas *canvas;

    RRendTexture *target;
    RRendTexture *final_target;
    Vec2d size;

    int idx;

    bool visible = true;

    RImage saved_image;
    bool saved_image_done = false;
    bool saved_image_needed = true;

    Layer(Renderer *renderer, Canvas *canvas, Vec2d size, int idx = 0);

    virtual ~Layer() {
        delete target;
    }

    inline void set_canvas(Canvas *canvas_) {
        canvas = canvas_;
    }

    inline Canvas* get_canvas() { return canvas; }

    void copy_from(RTexture *img) {
        renderer->push_target(get_target());
        renderer->set_render_state(sf::BlendNone);
        renderer->draw_texture({0, 0}, img, true);
        renderer->pop_target();
    }

    void fill_with(RTexture *img);
    
    void force_redraw();

    inline RRendTexture *get_target() {
        force_redraw();
        return target;
    }

    const RTexture *get_texture() {
        return &target->getTexture();
    }

    const RTexture *get_final_texture() {
        return &target->getTexture();
    }

    RColor get_pixel_color(const Vec2d &position);

    void clear(const RColor &color = {0, 0, 0, 0}) { target->clear(to_glib_color(color)); force_redraw(); }

    void flush_to(Layer *layer, bool to_flip = false, bool to_apply_effects = false, RMode rmode = {});

};


class FlushToFinalTexture : public Effect<Layer> {
public:
    FlushToFinalTexture(Layer *layer) : Effect(layer) {}

    virtual void apply() override {
        target->renderer->push_target(target->final_target);
        target->renderer->set_render_state(sf::BlendNone);
        target->renderer->draw_texture({0, 0}, target->get_texture(), true);
        target->renderer->pop_target();
    }
};


class ShaderEffect : public Effect<Layer> {
protected:
    RShader *shader;

public:
    ShaderEffect(Layer *layer, const char *filename) : Effect(layer), shader(new RShader) {
        shader->loadFromFile(filename, sf::Shader::Fragment);
        shader->setUniform("texture", sf::Shader::CurrentTexture);
    }

    virtual void apply() override {
        target->renderer->push_target(target->final_target);
        target->renderer->get_rstate()->rmode.blendMode = RBlend::none;
        target->renderer->get_rstate()->rmode.shader = shader;
        target->renderer->draw_texture({0, 0}, target->get_final_texture(), true);
        target->renderer->pop_target();
    }
};
