#pragma once


#include "engine/tickable.h"
#include "visual/renderer/renderable_object.h"

#include "utils/vec3d.h"
#include "utils/vec2d.h"

#include "lambda.h"

#include <vector>


struct ViewBody {
    Vec2d position;
    Vec2d size;

    inline Vec2d get_position() const { return position; }
    inline Vec2d get_size() const { return size; }

    inline void set_position(Vec2d position_) { position = position_; }
    inline void set_size(Vec2d size_) { size = size_; }

    inline bool is_clicked(Vec2d click) { return click.x() >= 0 && click.y() >= 0 && click.x() <= size.x() && click.y() <= size.y(); }
};


class AbstractView : public RenderableObject, public Tickable {
protected:
    ViewBody body;
    RenderableObject *texture;
    
    std::vector<AbstractView*> subviews;

    MouseLambda *on_click;
    MouseLambda *on_hover;
    MouseLambda *on_release;
    MouseLambda *on_tick;

public:
    AbstractView(ViewBody body, RenderableObject *texture = nullptr,
                 MouseLambda *on_click   = nullptr, 
                 MouseLambda *on_hover   = nullptr,
                 MouseLambda *on_release = nullptr,
                 MouseLambda *on_tick    = nullptr);
    virtual ~AbstractView();

    void add_subview(AbstractView *subview);
    void delete_subview(AbstractView *view);
    void delete_subview(size_t index);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void subtick(const double dt = 0, const double time = 0);

    virtual void render   (Renderer *renderer) override;
    virtual void subrender(Renderer *renderer);

    virtual void clicked(Vec2d click);
    virtual void subclick(Vec2d click);

    virtual void hovered (Vec2d from, Vec2d to);
    virtual void subhover(Vec2d from, Vec2d to);

    virtual void released  (Vec2d release);
    virtual void subrelease(Vec2d release);

    inline bool is_clicked(const Vec2d &click) { return body.is_clicked(click); }

    inline MouseLambda *get_on_click() { return on_click; };
    inline void set_on_click(MouseLambda *on_click_) { on_click = on_click_; }

    inline MouseLambda *get_on_release() { return on_release; };
    inline void set_on_release(MouseLambda *on_release_) { on_release = on_release_; }

    inline MouseLambda *get_on_hover() { return on_hover; };
    inline void set_on_hover(MouseLambda *on_hover_) { on_hover = on_hover_; }

    inline MouseLambda *get_on_tick() { return on_tick; };
    inline void set_on_tick(MouseLambda *on_tick_) { on_tick = on_tick_; }

    inline RenderableObject *get_texture() { return texture; }

    ViewBody &get_body();
};
