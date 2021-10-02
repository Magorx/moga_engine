#pragma once


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

    inline bool clicked(Vec2d click) { click -= position; return click.x() <= size.x() && click.y() <= size.y(); }
};


class View : public Renderable {
    ViewBody body;
    RenderableObject *texture;
    
    std::vector<View*> subviews;

    Lambda *reaction;

public:
    View(ViewBody body, RenderableObject *texture = nullptr, Lambda *reaction = nullptr);
    virtual ~View();

    void add_subview(View *subview);
    void delete_subview(View *view);
    void delete_subview(size_t index);

    virtual void render   (Renderer *renderer) override;
    virtual void subrender(Renderer *renderer);

    virtual void on_click(Vec2d click);
    virtual void subclick(Vec2d click);

    virtual void on_hover(Vec2d from, Vec2d to);
    virtual void subhover(Vec2d from, Vec2d to);

    virtual void on_release(Vec2d release);
    virtual void subrelease(Vec2d release);

    inline bool clicked(const Vec2d &click) { return body.clicked(click); }

    inline Lambda *get_reaction() { return reaction; };
    inline void set_reaction(Lambda *reaction_) { reaction = reaction_; }

    ViewBody &get_body();
};
