#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/renderer.h"


struct Layer {
    Renderer *renderer;

    RRendTexture *target;
    Vec2d size;

    int idx;

    bool visible = true;

    Layer(Renderer *renderer, Vec2d size, int idx = 0):
    renderer(renderer),
    target(nullptr),
    size(size),
    idx(idx)
    {
        target = new RRendTexture;
        target->create((float) size.x(), (float) size.y());
        target->setRepeated(true);
    }

    ~Layer() {
        delete target;
    }

    RRendTexture *get_target() {
        return target;
    }

    const RTexture *get_texture() {
        return &target->getTexture();
    }

    void clear(const RColor &color = {0, 0, 0, 0}) { target->clear(to_glib_color(color)); }

    void flush_to(Layer *layer, bool to_flip = false, RMode rmode = {}) {
        if (!layer || !renderer) return;

        renderer->push_target(layer->get_target());
        renderer->set_render_state(rmode);
        renderer->draw_texture({0, 0}, &target->getTexture(), to_flip);
        renderer->pop_target();
    }

};
