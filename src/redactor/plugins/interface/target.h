#pragma once


#include "redactor/plugin_std/std.hpp"
#include "redactor/layer/layer.h"


class Target : public P::RenderTarget {
    Layer *layer;
    bool to_delete_layer = false;
    bool to_flip = false;

public:
    Target(Layer *layer, bool to_delete_layer = false, bool to_flip = false);
    Target(Vec2d size);
    ~Target();

    virtual P::RenderTarget *get_copy() const override;

    virtual P::Vec2s get_size() const override;

    virtual P::RGBA get_pixel(size_t x, size_t y)          const override;
    virtual void    set_pixel(size_t x, size_t y, P::RGBA color) override;

    virtual P::RGBA *get_pixels() override;

    virtual void clear(P::RGBA color = 0) override;

// render
    virtual void render_circle      (P::Vec2f position, float radius, P::RGBA color, const P::RenderMode *render_mode)       override;
    virtual void render_line        (P::Vec2f start, P::Vec2f end, P::RGBA color, const P::RenderMode *render_mode)          override;
    virtual void render_triangle    (P::Vec2f p1, P::Vec2f p2, P::Vec2f p3, P::RGBA color, const P::RenderMode *render_mode) override;
    virtual void render_rectangle   (P::Vec2f p1, P::Vec2f p2, P::RGBA color, const P::RenderMode *render_mode)              override;
    
    virtual void render_texture     (P::Vec2f position, const RenderTarget *texture, const P::RenderMode *render_mode) override;
    virtual void render_pixels      (P::Vec2f position, const P::RGBA *data, size_t width, size_t height, const P::RenderMode *render_mode) override;

    virtual void apply_shader(const P::Shader *shader) override;
};
