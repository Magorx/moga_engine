#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/renderer.h"

#include "utils/affected.h"


struct Layer : public Affected<Layer> {
    Renderer *renderer;

    RRendTexture *target;
    RRendTexture *final_target;
    Vec2d size;

    int idx;

    bool visible = true;

    Layer(Renderer *renderer, Vec2d size, int idx = 0);

    virtual ~Layer() {
        delete target;
    }

    RRendTexture *get_target() {
        effects_applied = false;
        return target;
    }

    const RTexture *get_texture() {
        return &target->getTexture();
    }

    const RTexture *get_final_texture() {
        return &target->getTexture();
    }

    void clear(const RColor &color = {0, 0, 0, 0}) { target->clear(to_glib_color(color)); effects_applied = false; }

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
    RShader shader;

public:
    ShaderEffect(Layer *layer, const char *filename) : Effect(layer), shader() {
        shader.loadFromFile(filename, sf::Shader::Fragment);
        shader.setUniform("texture", sf::Shader::CurrentTexture);
    }

    virtual void apply() override {
        target->renderer->push_target(target->final_target);
        target->renderer->get_rstate()->rmode.blendMode = RBlend::none;
        target->renderer->get_rstate()->rmode.shader = &shader;
        target->renderer->draw_texture({0, 0}, target->get_final_texture(), true);
        target->renderer->pop_target();
    }
};
