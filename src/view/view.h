#pragma once


#include "engine/tickable.h"
#include "visual/renderer/renderable_object.h"

#include "utils/vec3d.h"
#include "utils/vec2d.h"

#include "lambda.h"

#include "event_system/event_system.h"

#include <vector>


class AbstractView;


class AVPressAcceptor : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVPressAcceptor(AbstractView *button);

    EventAccResult operator()(const Event::MousePress &event) override;
};

class AVReleaseAcceptor : public EventAcceptor<AbstractView, Event::MouseRelease> {
public:
    AVReleaseAcceptor(AbstractView *button);

    EventAccResult operator()(const Event::MouseRelease &event) override;
};

class AVMoveAcceptor : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVMoveAcceptor(AbstractView *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};



struct ViewBody {
    Vec2d position;
    Vec2d size;

    inline Vec2d get_position() const { return position; }
    inline Vec2d get_size() const { return size; }

    inline void set_position(Vec2d position_) { position = position_; }
    inline void set_size(Vec2d size_) { size = size_; }

    inline bool is_inside(Vec2d click) { return click.x() >= 0 && click.y() >= 0 && click.x() <= size.x() && click.y() <= size.y(); }
};


class AbstractView : public RenderableObject, public Tickable, public EventSystem {
protected:
    ViewBody body;
    RenderableObject *texture;
    
    std::vector<AbstractView*> subviews;

    AVPressAcceptor   on_press;
    AVReleaseAcceptor on_release;
    AVMoveAcceptor    on_move;

public:
    AbstractView(ViewBody body, RenderableObject *texture = nullptr);
    virtual ~AbstractView();

    void add_subview(AbstractView *subview);
    void delete_subview(AbstractView *view);
    void delete_subview(size_t index);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void subtick(const double dt = 0, const double time = 0);

    virtual void render(Renderer *renderer) override;
    virtual void subrender(Renderer *renderer);

    inline bool is_inside(const Vec2d &click) { return body.is_inside(click); }

    inline RenderableObject *get_texture() { return texture; }

    ViewBody &get_body();

    friend AVPressAcceptor;
    friend AVMoveAcceptor;
    friend AVReleaseAcceptor;
};
