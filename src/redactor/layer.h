#pragma once


#include "visual/renderer/gate_type_aliases.h"


struct Layer {
    RTexture *target;
    Vec2d size;

    int idx;

    bool visible = true;

    Layer(Vec2d size, int idx = 0):
    target(nullptr),
    size(size)
    {
        target = new RTexture;
        target->create((float) size.x(), (float) size.y());
    }

    ~Layer() {
        delete target;
    }

};
