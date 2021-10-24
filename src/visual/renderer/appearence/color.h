#pragma once


#include "appearence.h"


class AppearenceColor : public Appearence {
    sf::Color color;
public:
    AppearenceColor(const RGBA &color): Appearence(), color(to_sf_color(color)) {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d) const override {
        return {{(float) on_screen_position[0], (float) on_screen_position[1]}, color};
    };

    void set_color(const RGBA &color_) { color = to_sf_color(color_); }
};
