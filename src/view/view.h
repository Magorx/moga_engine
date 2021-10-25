#pragma once


#include "engine/tickable.h"
#include "visual/renderer/renderable_object.h"
#include "visual/renderer/appearence.h"

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

class AVDragAcceptor : public EventAcceptor<AbstractView, Event::MouseDrag> {
    bool allow_out_of_bounds;
public:
    AVDragAcceptor(AbstractView *view, bool allow_out_of_bounds = true);

    EventAccResult operator()(const Event::MouseDrag &event, const EventAccResult *cur_res = nullptr) override;
};

class AVDragEmitter : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVDragEmitter(AbstractView *view);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class AVPressFocuser : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVPressFocuser(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredMoveBlocker : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVCoveredMoveBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredPressBlocker : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVCoveredPressBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredReleaseBlocker : public EventAcceptor<AbstractView, Event::MouseRelease> {
public:
    AVCoveredReleaseBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCloseAcceptor : public EventAcceptor<AbstractView, Event::Close> {
public:
    AVCloseAcceptor(AbstractView *view);

    EventAccResult operator()(const Event::Close &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCloseGenerator : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVCloseGenerator(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


class AVAnimatorPress : public EventAcceptor<AbstractView, Event::MousePress> {
    Appearence *appr_pressed;
public:
    AVAnimatorPress(AbstractView *view, Appearence *appr);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVAnimatorMove : public EventAcceptor<AbstractView, Event::MouseMove> {
    Appearence *appr_hovered;
    Appearence *appr_idle;
public:
    AVAnimatorMove(AbstractView *view, Appearence *appr_hovered, Appearence *appr_idle);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;

    inline Appearence *get_idle_appr() { return appr_idle; }
};

class AVAnimatorRelease : public EventAcceptor<AbstractView, Event::MouseRelease> {
    Appearence *appr_hovered;
    Appearence *appr_idle;
public:
    AVAnimatorRelease(AbstractView *view, Appearence *appr_hovered, Appearence *appr_idle);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;

    inline Appearence *get_idle_appr() { return appr_idle; }
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


class AbstractView : public RenderableObject, public EventSystem {
protected:
    const char *view_id;

    ViewBody body;
    ViewBody fit_body;
    
    AbstractView *parent;
    std::vector<AbstractView*> subviews;

    Appearence *appearence;

    friend AVMissPressBlocker;
    friend AVMissMoveBlocker;
    friend AVMissReleaseBlocker;
    friend AVDragAcceptor;
    friend AVDragEmitter;

    bool pressed;
    bool focuseable;
    
    bool appearenced;

public:
    AbstractView(ViewBody body, AbstractView *parent = nullptr);
    virtual ~AbstractView();

    virtual void add_subview(AbstractView *subview);
    void delete_subview(AbstractView *view);
    void delete_subview(size_t index);

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

    inline bool is_pressed() const { return pressed; }

    inline bool is_focuseable() { return focuseable; }
    inline void set_focuseable(bool focuseable_) { focuseable = focuseable_;  }

    inline void set_appearence(Appearence *appearence_) { appearence = appearence_; }

    inline void set_view_id(const char *view_id_) { view_id = view_id_; }
    inline const char *get_view_id() { return view_id; }

    ViewBody &get_body();

};
