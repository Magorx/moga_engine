#pragma once


#include "engine/tickable.h"
#include "visual/renderer/renderable_object.h"

#include "utils/vec3d.h"
#include "utils/vec2d.h"

#include "lambda.h"

#include "event_system/event_system.h"

#include <vector>


class AbstractView;


class AVMissPressBlocker : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVMissPressBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVMissReleaseBlocker : public EventAcceptor<AbstractView, Event::MouseRelease> {
public:
    AVMissReleaseBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class AVMissMoveBlocker : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVMissMoveBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class AVRenderCallAcceptor : public EventAcceptor<AbstractView, Event::RenderCall> {
public:
    AVRenderCallAcceptor(AbstractView *view);

    EventAccResult operator()(const Event::RenderCall &event, const EventAccResult *cur_res = nullptr) override;
};


struct ViewBody {
    Vec2d position;
    Vec2d size;

    inline Vec2d get_position() const { return position; }
    inline Vec2d get_size() const { return size; }

    inline void set_position(Vec2d position_) { position = position_; }
    inline void set_size(Vec2d size_) { size = size_; }

    inline bool is_inside(Vec2d click) { return click.x() >= 0 && click.y() >= 0 && click.x() < size.x() && click.y() < size.y(); }
};


namespace Align {
    const ViewBody LURD ({{-1.0, -1.0}, { 0.0,  0.0}});
    const ViewBody LUCD ({{-0.5, -1.0}, { 0.5,  1.0}});
    const ViewBody LULD ({{ 0.0, -1.0}, { 1.0,  0.0}});
}


class AbstractView : public RenderableObject, public Tickable, public EventSystem {
protected:
    ViewBody body;
    ViewBody fit_body;
    
    AbstractView *parent;
    std::vector<AbstractView*> subviews;

    friend AVMissPressBlocker;
    friend AVMissMoveBlocker;
    friend AVMissReleaseBlocker;

public:
    AbstractView(ViewBody body, AbstractView *parent = nullptr);
    virtual ~AbstractView();

    void add_subview(AbstractView *subview);
    void delete_subview(AbstractView *view);
    void delete_subview(size_t index);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void subtick(const double dt = 0, const double time = 0);

    virtual void render(Renderer *renderer) override;
    virtual void subrender(Renderer *renderer);

    virtual bool is_active() const { return true; };

    void fit(const Vec2d &left_up, const Vec2d &right_down, bool absolute_fit = false);
    void fit_proportional(const Vec2d &left_up, const Vec2d &right_down);
    void fit_absolute(const Vec2d &left_up, const Vec2d &right_down);

    virtual void recalculate_fit_body();
    virtual void refit();
    void refit_in_parent();

    inline bool is_inside(const Vec2d &click) { return body.is_inside(click); }
    inline bool is_inside(const Vec2d &from, const Vec2d &to) { return body.is_inside(from) || body.is_inside(to); }

    inline void set_parent(AbstractView *parent_) { parent = parent_; }

    ViewBody &get_body();

};
