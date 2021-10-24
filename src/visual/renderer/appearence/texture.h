#pragma once


#include "appearence.h"


class AppearenceTexture : public Appearence {
public:
    AppearenceTexture(RTexture *texture) { rmode.texture = texture; }
    AppearenceTexture() {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d shape_position) const override {
        return {{(float) on_screen_position[0], (float) on_screen_position[1]}, {(float) (shape_position[0] * scale[0]), (float) (shape_position[1] * scale[1])}};
    };

    inline void set_texture(RTexture *texture) { rmode.texture = texture; }

    virtual void fit_for_size(const Vec2d &size) override { if (rmode.texture) set_scale({rmode.texture->getSize().x / size.x(), rmode.texture->getSize().y / size.y()}); }
};
