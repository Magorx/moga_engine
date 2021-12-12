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
    Target(const char *path);
    virtual ~Target();

    const Layer *get_layer() const { return layer; }
    Layer *get_layer() { return layer; }
    void update(const P::RGBA *data);

    virtual P::RenderTarget *get_copy() const override;

    virtual P::Vec2s get_size() const override;

    virtual P::RGBA get_pixel(size_t x, size_t y)          const override;
    virtual void    set_pixel(size_t x, size_t y, const P::RGBA &color) override;

    virtual P::RGBA *get_pixels() const override;

    virtual void clear(const P::RGBA &color = 0) override;

// render
    virtual void render_circle      (const P::Vec2f &position, float radius, const P::RGBA &color, const P::RenderMode &render_mode)       override;
    virtual void render_line        (const P::Vec2f &start, const P::Vec2f &end, const P::RGBA &color, const P::RenderMode &render_mode)          override;
    virtual void render_triangle    (const P::Vec2f &p1, const P::Vec2f &p2, const P::Vec2f &p3, const P::RGBA &color, const P::RenderMode &render_mode) override;
    virtual void render_rectangle   (const P::Vec2f &p1, const P::Vec2f &p2, const P::RGBA &color, const P::RenderMode &render_mode)              override;
    
    virtual void render_texture     (const P::Vec2f &position, const RenderTarget *texture, const P::RenderMode &render_mode) override;
    virtual void render_pixels      (const P::Vec2f &position, const P::Vec2s &size, const P::RGBA *data, const P::RenderMode &render_mode) override;

    virtual void apply_shader(const P::Shader *shader) override;
};
