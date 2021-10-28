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

    void clear(const sf::Color &color) { target->clear(color); }

    void flush_to(Layer *layer) {
        if (!layer || !renderer) return;

        renderer->push_target(layer->get_target());
        renderer->draw_texture({0, 0}, (RTexture*) &target->getTexture());
        renderer->pop_target();
    }

};
