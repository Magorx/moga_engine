#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/animation/animation.h"


inline sf::Color to_sf_color(const RColor &color) {
    return sf::Color(sf::Uint32(color.ui32));
}


class Appearence {
protected:
    RMode rmode;
    Vec2d scale {1, 1};

public:
    Appearence() : rmode() {}
    virtual ~Appearence() {}

    virtual const sf::RenderStates *get_render_mode() const { return &rmode; };
    virtual       sf::RenderStates *get_render_mode()       { return &rmode; };

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d shape_position) const = 0;

    inline void set_scale(const Vec2d &scale_) { scale = scale_; }
    virtual void fit_for_size(const Vec2d &) {}

    virtual void activate() {}
};
