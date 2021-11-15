#pragma once


#include "appearence.h"


class AppearenceTexture : public Appearence {
    sf::Image saved_image;
public:
    Vec2d transform = {1, 1};
    Vec2d screen_shift = {0, 0};

    AppearenceTexture(const RTexture *texture, const Vec2d &transform = {1, 1}, const Vec2d &screen_shift = {0, 0}) : 
    transform(transform) ,
    screen_shift(screen_shift)
    { rmode.texture = texture; }
    
    AppearenceTexture() {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d shape_position) const override {
        return {{(float) (screen_shift[0] + on_screen_position[0]), (float) (screen_shift[1] + on_screen_position[1])}, {(float) (shape_position[0] * scale[0] * transform[0]), (float) (shape_position[1] * scale[1] * transform[1])}};
    };

    virtual RColor get_px_color(Vec2d shape_position) const override {
        if (!rmode.texture) return {0, 0, 0, 0};
        shape_position *= scale * transform;

        shape_position.content[0] = fmin(fmax(shape_position.content[0], 0), saved_image.getSize().x - 1);
        shape_position.content[1] = fmin(fmax(shape_position.content[1], 0), saved_image.getSize().y - 1);

        auto sf_color = saved_image.getPixel(shape_position.x(), shape_position.y());
        return {sf_color.r, sf_color.g, sf_color.b, sf_color.a};
    }

    inline void update_image() { if (rmode.texture) saved_image = rmode.texture->copyToImage(); }

    inline void set_texture(const RTexture *texture) { rmode.texture = texture; }
    inline void set_transform(const Vec2d &transform_) { transform = transform_; }
    inline void set_screen_shift(const Vec2d &screen_shift_) { screen_shift = screen_shift_; }

    virtual void fit_for_size(const Vec2d &size) override { if (rmode.texture) set_scale({rmode.texture->getSize().x / size.x(), rmode.texture->getSize().y / size.y()}); }
};
