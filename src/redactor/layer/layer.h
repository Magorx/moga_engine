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
        return target;
    }

    const RTexture *get_texture() {
        return &target->getTexture();
    }

    void clear(const RColor &color = {0, 0, 0, 0}) { target->clear(to_glib_color(color)); }

    void flush_to(Layer *layer, bool to_flip = false, RMode rmode = {});

};
