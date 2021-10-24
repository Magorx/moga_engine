#pragma once


#include <SFML/Graphics.hpp>

#include "utils/vec2d.h"
#include "visual/color/color.h"


using RMode = sf::RenderStates;
using RVertex = sf::Vertex;
using RColor = RGBA;
using RTexture = sf::Texture;


inline sf::Color to_sf_color(const RColor &color) {
    return sf::Color(sf::Uint32(color.ui32));
}


class Appearence {
protected:
    RMode rmode;
    Vec2d scale {1, 1};

public:
    Appearence() : rmode() {}

    virtual const sf::RenderStates *get_render_mode() const { return &rmode; };
    virtual       sf::RenderStates *get_render_mode()       { return &rmode; };

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d shape_position) const = 0;

    inline void set_scale(const Vec2d &scale_) { scale = scale_; }
    virtual void fit_for_size(const Vec2d &) {}
};

class AppearenceColor : public Appearence {
    sf::Color color;
public:
    AppearenceColor(const RGBA &color): Appearence(), color(to_sf_color(color)) {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d) const override {
        return {{(float) on_screen_position[0], (float) on_screen_position[1]}, color};
    };

    void set_color(const RGBA &color_) { color = to_sf_color(color_); }
};

class AppearenceTexture : public Appearence {
public:
    AppearenceTexture(RTexture *texture) { rmode.texture = texture; }
    AppearenceTexture() {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d shape_position) const override {
        return {{(float) on_screen_position[0], (float) on_screen_position[1]}, {(float) (shape_position[0] * scale[0]), (float) (shape_position[1] * scale[1])}};
    };

    inline void set_texture(RTexture *texture) { rmode.texture = texture; }

    virtual void fit_for_size(const Vec2d &size) override { set_scale({rmode.texture->getSize().x / size.x(), rmode.texture->getSize().y / size.y()}); }
};
